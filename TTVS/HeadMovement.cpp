
// Natural Head movement synthesis for Talking Avatar
// Author: ShenZHANG
// Date:   2007/10/09

// input1: PAD stream(for each Prosodic Word)
// input2: Tone stream (for each syllable 0,1,2,3,4)
// input3: Stress stream (for each syllable 0,1)
// (input4): Syllable stream (pinyin,just used for viseme generation)

// output: FAP stream file

//input1 example
//----------------------------------------------------------
// start_frame_no    end_frame_no    P_Value A_Value D_Value
//    45                 79              0     0.5     0
//    91                 119             0      0      0
//----------------------------------------------------------

//input2+input3 exmaple
//----------------------------------------------------------
// start_frame_no   end_frame_no    Tone    Stress
//    45                53          4       1
//    54                60          2       0
//----------------------------------------------------------

//(input4 example :not used)
//----------------------------------------------------------
//  pinyin      start_frame     end_frame
//  tai             45              53
//  ping            54              60
//----------------------------------------------------------*/

//output example:(FAP file with head and facial movement, 8 FAP)
//----------------------------------------------------------
// 2.1 LinLin::FapFile 25 1597
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
// 0 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 
//----------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include <iterator>

#ifndef WINCE
#include <time.h>
#else
#include <Windows.h>        // for SYSTEMTIME, there is no time() under WINCE
#endif
#include "HeadMovement.h"

namespace CSTHead
{
    unsigned int HeadMovement::getRandSeed()
    {
#ifdef WINCE
        SYSTEMTIME  sysTime;
        FILETIME    fileTime;
        GetSystemTime(&sysTime);
        SystemTimeToFileTime( &sysTime, &fileTime );
        return fileTime.dwLowDateTime;
#else
        return (unsigned)time(NULL);
#endif
    }

    void HeadMovement::generateFAPs(const std::vector<SyllableInfo>& syl_info, const std::vector<int>& pw_frm_info, std::vector< std::vector<float> >& FAP_stream)
    {
        if (syl_info.size() == 0 || pw_frm_info.size() == 0)
            return;

        const int FRAME_RATE = 25;
        //TODO
        std::vector<int> pw_info = pw_frm_info; // for PW merge purpose

        int pw_num = (int)pw_info.size()/2;  //pw_info = (start_syl_no,end_syl_no)

        //step1. set the PAD value for each PW (the maximum PAD value among all the syllable)
        std::vector<PAD> pw_pad;
        //pw_pad.reserve(pw_num);
        for (int i=0;i<pw_num;i++)
        {
            int ii = i*2;       //counter
            int j=pw_info[ii];  //start_syl_no
            PAD max_pad = syl_info[j].pad;
            float max_pad_sum =  max_pad.P+max_pad.A+max_pad.D;
            for (j=pw_info[ii]+1;j<pw_info[ii+1];j++)
            {
                float tmp_pad_sum = syl_info[j].pad.P + syl_info[j].pad.A + syl_info[j].pad.D;
                if (tmp_pad_sum>max_pad_sum)
                {
                    max_pad_sum = tmp_pad_sum;
                    max_pad = syl_info[j].pad;
                }
            }
            pw_pad.push_back(max_pad);
        }

        //option:merge consecutive PW with same PAD into one PW
        //the average PW length is better around 5-6 syllables
        int last_idx = 0;
        PAD last_pad = pw_pad[0];
        int ph_num = 0;
        int cnt = 0;
        std::vector<PAD> tmp_pad;
        std::vector<int> tmp_pw_info;
        for (int i=1;i<pw_num;i++)
        {
            PAD cur_pad = pw_pad[i];
            if (cur_pad.P==last_pad.P && 
                cur_pad.A==last_pad.A &&
                cur_pad.D==last_pad.D && cnt<0) // original: cnt<2
            {
                // cnt<0, do not mergy
                cnt++;
                continue;
            }else
            {
                cnt = 0;
                ph_num++;
                tmp_pad.push_back(last_pad);
                tmp_pw_info.push_back(pw_info[last_idx*2]);
                tmp_pw_info.push_back(pw_info[(i-1)*2+1]);
                last_pad = cur_pad;
                last_idx = i;
            }
        }
        tmp_pad.push_back(last_pad);
        tmp_pw_info.push_back(pw_info[last_idx*2]);
        tmp_pw_info.push_back(pw_info[(pw_num-1)*2+1]);
        pw_info = tmp_pw_info;
        pw_pad = tmp_pad;
        pw_num = (int)pw_info.size()/2;

        // step2. set the head amp and pos for each PW
        std::vector<float> head_pos;
        std::vector<float> head_amp;
        head_pos.reserve(pw_num);
        head_amp.reserve(pw_num);
        std::fill_n(std::back_inserter(head_pos),pw_num,0.0f);
        std::fill_n(std::back_inserter(head_amp),pw_num,0.0f);
        srand( getRandSeed() );
        for (int i=0;i<pw_num;i++)
        {
            GetHeadPos(pw_pad[i],head_pos[i],head_amp[i]);
        }

        //step3. set the peak and valley point in headmovement for each PW
        std::vector<int> head_peak;
        std::vector<int> head_valley;
        std::vector<int> head_frm_len;
        std::fill_n(std::back_inserter(head_peak),pw_num,0);
        std::fill_n(std::back_inserter(head_valley),pw_num,0);
        std::fill_n(std::back_inserter(head_frm_len),pw_num,0);
        for (int i=0;i<pw_num;i++)
        {
            int ii = i*2;
            int head_frm = syl_info[pw_info[ii]].start_frame_no;
            int tail_frm = syl_info[pw_info[ii+1]].end_frame_no;
            int pw_frm_len = tail_frm - head_frm + 1;
            int peak_frm = 0;
            int valley_frm = 0;
            if (i==0)       // the first PW(sentence initial)
            {
                peak_frm = (int)(pw_frm_len*0.2);
                valley_frm = (int)(pw_frm_len*0.8);
                //original
                //peak_frm = (int)(pw_frm_len*0.8);
                //valley_frm = (int)(pw_frm_len*0.2);
            }else
            {
                //GetPeakPos(pw_info[ii],pw_info[ii+1],pw_pad[i],syl_info,&peak_frm,&valley_frm);
                int pw_syl_num = pw_info[ii+1] - pw_info[ii] + 1;
                if (pw_syl_num==0)      //silence
                {
                    peak_frm = (int)(pw_frm_len*0.25)+ head_frm;
                    valley_frm = (int)(pw_frm_len*0.75)+ head_frm;
                }else if(pw_syl_num==1) //only one syllable
                {
                    int s = head_frm;
                    int e = tail_frm;
                    if(syl_info[pw_info[ii]].stressed==1 || syl_info[pw_info[ii]].tone==4) //stress or emphasis tone
                    {
                        peak_frm = (s+e)/2;
                        valley_frm = e+(e-s)/2;
                    }
                    else
                    {
                        peak_frm = s;
                        valley_frm = e;
                    }
                }
                else                    //more than 2 syllables
                {
                    bool is_set = false;
                    for (int k=pw_info[ii];k<pw_info[ii+1];k++) //find the first stress
                    {
                        if (syl_info[k].stressed==1)
                        {
                            int s = syl_info[k].start_frame_no;
                            int e = syl_info[k].end_frame_no;
                            peak_frm = (s+e)/2; 
                            if(k-pw_info[ii] + 1 > pw_syl_num/2)
                            {
                                valley_frm = syl_info[pw_info[ii]].start_frame_no;
                            }
                            else
                            {
                                valley_frm = syl_info[pw_info[ii+1]].end_frame_no;
                            }
                            is_set = true;
                            break;
                        }
                    }
                    if (!is_set)    //find first falling tone 
                    {
                        for (int k=pw_info[ii];k<pw_info[ii+1];k++)
                        {
                            if (syl_info[k].tone == 4)
                            {
                                int s = syl_info[k].start_frame_no;
                                int e = syl_info[k].end_frame_no;
                                peak_frm = (s+e)/2; 
                                if(k - pw_info[ii] + 1> pw_syl_num/2)
                                {
                                    valley_frm = syl_info[pw_info[ii]].start_frame_no;
                                }
                                else
                                {
                                    valley_frm = syl_info[pw_info[ii+1]].end_frame_no;
                                }
                                is_set = true;
                                break;
                            }
                        }
                    }
                    if (!is_set)    //find other syllable on the boundary
                    {
                        int peak_idx = pw_info[ii];
                        int valley_idx = pw_info[ii+1];
                        if(pw_syl_num>4)
                        {
                            peak_idx = (int)(pw_syl_num*0.25);
                            valley_idx = (int)(pw_syl_num*0.75);       
                        }
                        peak_frm = syl_info[peak_idx].start_frame_no;
                        valley_frm = syl_info[valley_idx].end_frame_no;
                    }
                }
                peak_frm -= head_frm;
                valley_frm -= head_frm;
            }
            head_peak[i] = peak_frm;
            head_valley[i] = valley_frm;
            head_frm_len[i] = pw_frm_len;
        }

        //step4 interpolate the head location for each PW
        std::vector<float> head_y;
        std::vector<float> head_x;
        int total_len_frm = syl_info[pw_info[pw_num*2-1]].end_frame_no + 2*FRAME_RATE;
        head_y.reserve(total_len_frm);
        head_x.reserve(total_len_frm);
        std::fill_n(std::back_inserter(head_y),total_len_frm,0.0f);
        std::fill_n(std::back_inserter(head_x),total_len_frm,0.0f);
        const float PI = 3.1415926f;
        std::vector<float> y;
        for (int i=0;i<pw_num;i++)
        {
            y.clear();
            y.reserve(head_frm_len[i]);
            std::fill_n(std::back_inserter(y),head_frm_len[i],0.0f);
            const int peak_range_L = 5;     //the left part of peak point
            float sF = PI/(2*peak_range_L);
            float sA = head_amp[i]*0.75f;
            int s = head_peak[i] - peak_range_L;
            int e = head_peak[i]+1;
            if(s<0)s=0;
            if(e>head_frm_len[i])e=head_frm_len[i];
            for(int x=s;x<e;x++)
            {
                y[x] = sA * sin(sF*(x-head_peak[i]) + PI/2);
            }
            int peak_left = s;

            const int peak_range_R = 10;    // the right part of peak point
            sF = PI/(2*peak_range_R);
            sA = head_amp[i]*0.75f;
            s = head_peak[i];
            e = head_peak[i]+peak_range_R+1;
            if(s<0)s=0;
            if(e>head_frm_len[i])e=head_frm_len[i];
            for(int x=s;x<e;x++)
            {
                y[x] = sA * sin(sF*(x-head_peak[i]) + PI/2);
            }
            int peak_right = e;

            int valley_range = 0;
            if( head_valley[i]> peak_right)
            {
                valley_range = head_valley[i] - peak_right;
            }else if(head_valley[i]<peak_left)
            {
                valley_range = peak_left - head_valley[i];
            }
            if(valley_range>0)
            {
                sA = -head_amp[i]*0.25f;
                sF = PI/(2 * valley_range);
                s = head_valley[i] - valley_range;
                e = head_valley[i] + valley_range+1;
                if(s<0)s=0;
                if(e>head_frm_len[i])e=head_frm_len[i];
                for(int x=s;x<e;x++)
                {
                    y[x] = sA * sin(sF*(x-head_valley[i]) + PI/2);
                }
            }
            for (int x=0;x<head_frm_len[i];x++)
            {
                y[x] += head_amp[i]*0.2f + head_pos[i];
            }

            int head_frame = syl_info[pw_info[i*2]].start_frame_no;
            for (int x=0;x<head_frm_len[i];x++)
            {
                head_y[x+head_frame] = y[x];
            }
            // for horizontal movement
            if (head_frm_len[i]>FRAME_RATE)
            {
                float sign = 1;
                if(rand() < (RAND_MAX/2)) sign = -1;
                for (int x=0;x<head_frm_len[i];x++)
                {
                    head_x[x+head_frame] = sign*y[x];
                }
            }
        }

        //step5: smooth the boundary
        const int t_size = 5;
        std::vector<float> tmp_y = head_y;
        for (int i=0;i<pw_num;i++)
        {
            int ii = i*2;
            int head_frame = syl_info[pw_info[ii]].start_frame_no;
            int tail_frame = syl_info[pw_info[ii+1]].end_frame_no;
            for (int k = head_frame - t_size;k<=head_frame+t_size;k++)
            {
                if (k<0 || k>total_len_frm) continue;
                int s = k-t_size;if(s<0)s=0;
                int e = k+t_size;if(e>=total_len_frm)e=total_len_frm-1;
                head_y[k] = GetMeanValue(tmp_y,s,e);
            }
            for (int k=tail_frame-t_size;k<=tail_frame+t_size;k++)
            {
                if (k<0 || k>total_len_frm)continue;
                int s = k-t_size;if(s<0)s=0;
                int e = k+t_size;if(e>=total_len_frm)e=total_len_frm-1;
                head_y[k] = GetMeanValue(tmp_y,s,e);
            }
        }
        smooth(head_x,21);
        smooth(head_y,5);

        //step6. get the FAP stream

        const int AU_SCALE = 50000;
        const float IRISD_SCALE = 20480/5.66434f;
        const float ENS_SCALE = 30480/25.5803f;
        const float AVG_HEAD = GetMeanValue(head_y,0,total_len_frm-1);

        FAP_stream.clear();
        for (int x=0;x<total_len_frm;x++)
        {
            float delta = 0;
            float delta2 = 0;
            float delta3 = head_y[x]*100;
            if (head_y[x] >= AVG_HEAD*0.7)
            {
                delta = -head_y[x]*0.05f;
                delta2 = head_y[x]*1.5f;
            }else if((x%(2*FRAME_RATE))==0 && rand()<(RAND_MAX/2))
            {
                delta = 0.5;
            }
            std::vector<float> fap_set;
            fap_set.reserve(68);
            std::fill_n(std::back_inserter(fap_set),68,0.0f);
            fap_set[18] = delta*IRISD_SCALE;
            fap_set[19] = delta*IRISD_SCALE;
            fap_set[32] = delta2*ENS_SCALE;
            fap_set[33] = delta2*ENS_SCALE;
            fap_set[47] = head_y[x]*AU_SCALE;
            fap_set[48] = delta3*100;
            fap_set[49] = delta3*100;
            FAP_stream.push_back(fap_set);
        }
        return;
    }

    void HeadMovement::GetHeadPos(const PAD pad,float &pos,float &amp)
    {
        const float AMP_AVG[] = {0.0391f,   0.0575f,    0.0587f,    0.0807f,    0.1150f,    0.0752f,    0.0778f,    0.0279f,    0.0423f};
        const float AMP_STD[] = {0.0142f,   0.0249f,    0.0279f,    0.0373f,    0,          0.0525f,    0.0496f,    0.0196f,    0.0258f};
        const float POS_AVG[] = {-0.0035f, -0.0248f,   -0.0146f,   -0.0163f,    0.0071f,   -0.0042f,   -0.0171f,   -0.0213f,   -0.0216f};
        const float POS_STD[] = {0.0108f,   0.0195f,    0.0167f,    0.0214f,    0,          0.0116f,    0.0124f,    0.0183f,    0.0181f}; 

        const float AMP_RANGE[] = { 0.0076f,    0.0294f,    0.0460f,    0.0723f,    0.1150f};
        const float POS_RANGE[] = {-0.0035f,   -0.0248f,   -0.0146f,   -0.0163f};
        int t = GetPADType(pad);
        if(t<5)
        {
            amp = AMP_RANGE[t-1] + rand()*1.0f/RAND_MAX * (AMP_RANGE[t] - AMP_RANGE[t-1]);
            pos = POS_RANGE[t-1] + rand()*1.0f/RAND_MAX * 0.001f;
        }
        else
        {
            amp = 0.017f;
            pos = 0;
        }
        return;
    }

    inline int HeadMovement::GetPADType(const PAD pad)
    {
        const float P = pad.P;
        const float A = pad.A;
        const float D = pad.D;
        int type = 7;
        if (P==0)
        {
            if (A==0)
            {
                type = 1;
            }else if(A==0.5)
            {
                type = 2;
            }else if(A==1)
            {
                type = 5;
            }
        }else if(P==1)
        {
            if (A==0)
            {
                type = 6;
            }else if(A==0.5)
            {
                type = 3;
            }else if(A==1)
            {
                type = 4;
            }
        }
        return type;
    }

    // set the peak and valley point of head movement in each PW based on intra-pw-rule
    //void GetPeakPos(const int head,const int tail,const PAD pad,int &peak,int &valley)
    //{
    //}

    float HeadMovement::GetMeanValue(const std::vector<float> data,const int s,const int e)
    {
        float mean_val = 0;
        for (int i=s;i<=e;i++)
        {
            mean_val += data[i];
        }
        mean_val = mean_val/(e-s+1);
        return mean_val;
    }

    void HeadMovement::smooth(std::vector<float> &data,const int win_len)
    {
        std::vector<float> tmp_data = data;
        size_t half_win = win_len/2;
        for (size_t i=0; i<data.size(); i++)
        {
            size_t s = (i < half_win) ? 0 : i-half_win;
            size_t e = i + half_win;
            if (e >= data.size()) e = data.size() - 1;
            float mean_val = 0;
            for (size_t k=s; k<=e; k++)
                mean_val += tmp_data[k];
            data[i] = mean_val / (e-s+1);
        }
    }
}
