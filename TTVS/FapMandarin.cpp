
#include "FapMandarin.h"
#include "HeadMovement.h"
#include <fstream>
#include <ctype.h>

namespace CSTHead
{
    FapMandarin::FapMandarin()
    {
    }

    FapMandarin::~FapMandarin()
    {
    }


    /**
     *  @brief  Convert the pinyin string to viseme list
     *
     *  The pinyin string is converted to viseme list according to following table:
        @verbatim
        0: NA
        1: d1,t1,n1
        2: d2,t2,n2
        3: l1
        4: l2
        5: g1,k1,h1
        6: g2,k2,h2
        7: j,q,x1
        8: j,q,x2
        9: zh,ch,sh,r1
        10: zh,ch,sh,r2
        11: z,c,s1
        12: z,c,s2
        13: b1,p1,m1//a,u
        14: f
        15: a ,ang
        16: ai,an
        17: ao
        18: o
        19: ou
        20: e
        21: ei,en,eng
        22: er
        23: i
        24: u
        25: ü

        声母	  开口呼  	 齐齿呼	    合口呼	   撮口呼
        b,p,m	  爸	       比	    不(似'爸')	-
        f	      发           -	    福(似'发')   -
        d,t,n	  大	     地(似'大')	毒	     女(似'毒')
        l	      拉	       -	    路	     吕(似'路')
        g,k,h	  哈         	-	    姑	         -
        j,q,x	  -	           机	    -	        句
        zh,ch,sh,r	  沙	       -	    书	         -
        z,c,s	  杂	       -	    组	         -
        ia=i+a; ie=i+e; in=i+ei; ing=i+eng
        iao=i+ao;  iou=i+ou;
        ian=i+an;  iang=i+ang
        ua=u+a;   uo=u+o;  uai=u+ai;
        uei=u+ei;  uan=u+an;  uen=u+en;
        uang=u+ang;  ueng(ong)=u+eng;
        üan=ü+an;üe=ü+e,ün=ü+ei，iong=ü+eng
        @endverbatim
     *
     *  @param  pinyin [in]     The pinyin string to be converted
     *  @param  viseme [out]    Return the viseme list
     *
     *  @return Return the number of visemes in the viseme list result
     */
    int FapMandarin::convertPinyin2Viseme(const char* pinyin, int* viseme)
    {
        int i = 0;
        unsigned char j = 0;

        viseme[0]=0;
        while(pinyin[i]>96 && pinyin[i]<123)
        {
            switch (pinyin[i])
            {
            case 'b':   case 'p':   case 'm':
                viseme[j++]=13;
                break;
            case 'f':
                viseme[j++]=14;
                if (pinyin[i+1]=='u') 
                {
                    viseme[j++]=14;
                    i++;
                }
                break;
            case 'd':   case 't':   case 'n':
                viseme[j++]=1;
                break;
            case 'l':
                viseme[j++]=3;
                break;
            case 'g':   case 'k':   case 'h':
                viseme[j++]=5;
                break;
            case 'j':   case 'q':   case 'x':
                viseme[j++]=7;
                break;
            case 'z':   case 'c':   case 's':
                if (pinyin[i+1]=='h')
                {
                    viseme[j++]=9;
                    i++;
                }
                else
                {
                    viseme[j++]=11;
                }
                break;
            case 'r':
                viseme[j++]=9;
                break;
            case 'a':
                if (pinyin[i+1]=='o')
                {
                    viseme[j++]=17;
                    i++;
                }
                else if (pinyin[i+1]=='i')
                {
                    viseme[j++]=16;
                    i++;
                }
                else
                {
                    if (pinyin[i+1]=='n')
                    {
                        if (pinyin[i+2]=='g')
                        {
                            viseme[j++]=15;
                            i+=2;
                        }
                        else
                        {
                            viseme[j++]=16;
                            i++;
                        }
                    }
                    else
                        viseme[j++]=15;
                }
                break;
            case 'o':
                if (pinyin[i+1]=='u')
                {
                    viseme[j++]=19;
                    i++;
                }
                else if (pinyin[i+1]=='n' && pinyin[i+2]=='g')
                {
                    if(j>0&&viseme[j-1]<12)
                        viseme[j-1]++;//开口与合口的区别
                    viseme[j++]=24;//u
                    //viseme[j++]=21;//eng
                    i+=2;
                }
                else
                    viseme[j++]=18;
                break;
            case 'e':
                if (pinyin[i+1]=='i' || pinyin[i+1]=='n')
                {
                    viseme[j++]=21;
                    i++;
                    if (pinyin[i]=='n' && pinyin[i+1]=='g')
                        i++;
                }
                else if (pinyin[i+1]=='r')
                {
                    viseme[j++]=22;
                    i++;
                }
                else
                    viseme[j++]=20;
                break;
            case 'i':
                if (!(j>0 && (viseme[j-1]==7 || viseme[j-1]==9 || viseme[j-1]==11)))
                {
                    if (pinyin[i+1]=='o' && pinyin[i+1]=='n' && pinyin[i+1]=='g')
                    {
                        if (j>0 && viseme[j-1]<12)
                            viseme[j-1]++;  //开口与合口的区别
                        viseme[j++]=25;
                        viseme[j++]=21;
                        i+=3;
                    }
                    else
                    {   
                        //in=i+ei; ing=i+eng
                        viseme[j++]=23;
                        if (viseme[i+1]=='n')
                        {
                            viseme[j++]=21;
                            if (pinyin[i+2]=='g')
                                i++;
                            i++;
                        }
                    }
                }
                else
                    viseme[j++]=viseme[j-1];
                break;
            case 'u':
                if (j>0 && viseme[j-1]<12)
                    viseme[j-1]++;  //开口与合口的区别
                if (pinyin[i+1]=='e')
                {
                    viseme[j++]=25;
                    viseme[j++]=20;
                    i++;
                }
                else if (pinyin[i+1]=='n')
                {
                    viseme[j++]=24;
                    viseme[j++]=21;
                    i++;
                }
                else
                {
                    if (i>0 && pinyin[i-1]=='y') 
                        viseme[j++]=25;
                    else
                        viseme[j++]=24;
                }
                break;
            case 'v':
                if (j>0 && viseme[j-1]<12)
                    viseme[j-1]++;  //开口与合口的区别
                if (pinyin[i+1]=='e')
                {
                    viseme[j++]=25;
                    viseme[j++]=20;
                    i++;
                }
                else if (pinyin[i+1]=='n')
                {
                    viseme[j++]=25;
                    viseme[j++]=21;
                    i++;
                }
                else
                    viseme[j++]=25;
                break;
            case 'y':
                if (pinyin[i+1]=='a'|| pinyin[i+1]=='e')
                    viseme[j++]=23; //i
                else if (pinyin[i+1]=='o')
                {
                    if (pinyin[i+2]=='u')
                    {
                        viseme[j++]=23;//i
                        viseme[j++]=19;//ou
                        i+=2;
                    }
                    else if (pinyin[i+2]=='n'&& pinyin[i+3]=='g')
                    {
                        viseme[j++]=25; //v
                        //viseme[j++]=23;//i
                        //viseme[j++]=24;//u
                        //viseme[j++]=20;//eng
                        i+=3;
                    }
                }
                break;
            case 'w':
                if (pinyin[i+1]=='a' || pinyin[i+1]=='e')
                    viseme[j++]=24;//u
                break;
            default:
                assert(false || "Pinyin error!");
                break;
            }
            i++; 
        }
        if(!j)
            viseme[j++]=0;//0 for mouth closing, not 1
        return j;
    }

    /**
     *  @brief  Generate the visemes and corresponding durations
     *
     *  @param  pinyin    [in]  The pinyin string
     *  @param  pauses    [in]  The pauses before each pinyin (in: ms)
     *  @param  durations [in]  The durations for each pinyin (in: ms)
     *  @param  numbers   [in]  The number of syllables (pinyin)
     *
     *  @param  visemes  [out]  Return the viseme no.
     *  @param  begtimes [out]  Return the corresponding beginning time for each viseme (in: ms)
     */
    bool FapMandarin::generateVisemes(const char* pinyin, const int* pauses, const int* durations, int numbers, std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        int i=0, j=0, k;
        int pidx[5], pnum;
        float passedTime=0, interTime;

        visemes.clear();
        begtimes.clear();
        passedTime = 0;

        for (i=0; i<numbers; i++)
        {
            // silence
            visemes.push_back(0);
            begtimes.push_back(passedTime);
            passedTime += pauses[i];

            // skip to letters
            while (isspace(pinyin[j])) j++;
            //while (!(pinyin[j]>='a' && pinyin[j]<='z')) j++;

            // Convert one Pinyin to viseme list
            pnum = convertPinyin2Viseme(&pinyin[j], pidx);
            interTime = 1.0f*durations[i]/pnum;

            // save information
            for (k=0; k<pnum; k++)
            {
                visemes.push_back(pidx[k]);
                begtimes.push_back(passedTime);
                passedTime += interTime;
            }

            // skip current pinyin
            if (i<numbers-1)
                while (!isspace(pinyin[j])) j++;
                //while (pinyin[j]>='a' && pinyin[j]<='z') j++;
        }

        // last silence
        visemes.push_back(0);
        begtimes.push_back(passedTime);

        return true;
    }

    /**
     *  @brief  Generate the FAPs from Mandarin pinyin string
     *
     *  @param  pinyin    [in]  The pinyin string
     *  @param  pauses    [in]  The pauses before each pinyin (in: ms)
     *  @param  durations [in]  The durations for each pinyin (in: ms)
     *  @param  numbers   [in]  The number of syllables (pinyin)
     */
    bool FapMandarin::generateFAPs(const char* pinyin, int* pauses, int* durations, int numbers)
    {
        std::vector<int> visemes;
        std::vector<float> begintimes;
        // generate visemes
        generateVisemes(pinyin, pauses, durations, numbers, visemes, begintimes);
        // map visemes to FAPs
        mapVisemes2FAPs(visemes, begintimes);
        // add emotion
        addEmotion(m_FAPs, 0, (int)m_FAPs.size());
        // add eye movement
        addEyeMotion(m_FAPs, 0, (int)m_FAPs.size());
        // add random head movement
        addHeadRandomMotion(m_FAPs, 0, (int)m_FAPs.size());
        // add expressive head movement
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;
        {
            // prepare information
            float passedFrames = 0;
            int   pwStartIdx = 0;
            for (int syllableIdx=0; syllableIdx<numbers; syllableIdx++)
            {
                HeadMovement::SyllableInfo sylOne;
                sylOne.pad.P = 0;
                sylOne.pad.A = 0;
                sylOne.pad.D = 0;
                sylOne.stressed = false;
                sylOne.tone = 0;

                passedFrames += pauses[syllableIdx] / 1000.0f * m_FPS;
                sylOne.start_frame_no = (int)passedFrames;
                passedFrames += durations[syllableIdx] / 1000.0f * m_FPS;
                sylOne.end_frame_no = (int)passedFrames;

                if (syllableIdx == pwStartIdx+2)
                {
                    // Previous syllable is prosodic boundary
                    // Put focus on current syllable
                    sylOne.pad.P = 1;
                    sylOne.pad.A = 1;
                    sylOne.stressed = true;
                }

                if (syllableIdx % 5 == 4)
                {
                    // Prosodic boundary
                    pwInfo.push_back(pwStartIdx);
                    pwInfo.push_back(syllableIdx);
                    pwStartIdx = syllableIdx + 1;
                }
                sylInfo.push_back(sylOne);
            }
        }
        addHeadExpressiveMotion(sylInfo, pwInfo);
        return true;
    }

    /**
     *  @brief  Generate the FAPs from Mandarin syllable pinyin file,
     *          the format of the file is: (pinyin, pause_dur (second), duration, pw_boundary, tone, stressed, P, A, D)
     *
     *  @param  filename  [in]  The file name
     */
    bool FapMandarin::generateFAPs(const char* filename)
    {
        std::vector<char> pinyins;
        std::vector<int>  pauses;
        std::vector<int>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // get pinyin, pauses, durations, and HeadMovement related information
        loadPinyinInfo(filename, pinyins, pauses, durations, sylInfo, pwInfo);

		if (pinyins.size() == 0 || pauses.size() == 0 || durations.size() == 0)
		{
			// no data, return directly
			return true;
		}

        // generate visemes and FAPs
        std::vector<int> visemes;
        std::vector<float> begintimes;
        // generate visemes
        generateVisemes(&pinyins[0], &pauses[0], &durations[0], (int)durations.size(), visemes, begintimes);
        // map visemes to FAPs
        mapVisemes2FAPs(visemes, begintimes);
        // add emotion
        addEmotion(m_FAPs, 0, (int)m_FAPs.size());
        // add eye movement
        addEyeMotion(m_FAPs, 0, (int)m_FAPs.size());
        // add random head movement
        addHeadRandomMotion(m_FAPs, 0, (int)m_FAPs.size());
        // add expressive head movement
        addHeadExpressiveMotion(sylInfo, pwInfo);
        //addHeadMovement(&pinyins[0], &pauses[0], &durations[0], (int)durations.size());

        return true;
    }

    /**
     *  @brief  Generate the FAPs from Mandarin syllable pinyin file,
     *          the format of the file is: (pinyin, startFrameNo, endFrameNo),
     *          the frame no is calculated with the frame rate
     *
     *  @param  filename  [in]  The file name
     *  @param  framerate [in]  The frame rate
     */
    bool FapMandarin::generateFAPs(const char* filename, int framerate)
    {
        std::vector<char> pinyins;
        std::vector<int>  pauses;
        std::vector<int>  durations;
        // get pinyin, pauses, and durations
        char  pinyin[10], *p;
        int   startFrame, endFrame;
        float lastFrame = 0, timePerFrame = 1000.0f/framerate;
        FILE* file = fopen(filename, "rt");
        if (file==NULL)
            return false;
        while (fscanf(file, "%s %d %d\n", pinyin, &startFrame, &endFrame)!=EOF)
        {
            // save pinyin
            for (p=pinyin; p!=NULL && *p!='\0'; p++)
                pinyins.push_back(*p);
            pinyins.push_back(' ');
            // save pause
            pauses.push_back((int)((startFrame-lastFrame)*timePerFrame));
            // save duration
            durations.push_back((int)((endFrame-startFrame)*timePerFrame));
            lastFrame = (float)endFrame;
        }
        fclose(file);

		if (pinyins.size() == 0 || pauses.size() == 0 || durations.size() == 0)
		{
			// no data, return directly
			return true;
		}

        std::vector<int> visemes;
        std::vector<float> begintimes;
        // generate visemes
        generateVisemes(&pinyins[0], &pauses[0], &durations[0], (int)durations.size(), visemes, begintimes);
        // map visemes to FAPs
        mapVisemes2FAPs(visemes, begintimes);
        // add emotion
        //addEmotion(m_FAPs, 0, (int)m_FAPs.size());
        // add head eye movement
        //moveHeadEye(m_FAPs, 0, (int)m_FAPs.size());
        return true;
    }

    bool FapMandarin::loadPinyinInfo(const char* filename, std::vector<char>& pinyins, std::vector<int>& pauses, std::vector<int>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo)
    {
        // get pinyin, pauses, durations, and HeadMovement related information
        char  pinyin[10], *p;
        float pause, duration, fP, fA, fD;
        int   pwboundary, tone, stressed;
        HeadMovement::SyllableInfo sylOne;
        float passedFrames = 0;
        int   pwStartIdx  = 0;
        int   syllableIdx = 0;

        std::ifstream file(filename, std::ios::in|std::ios::binary);
        if (!file)
            return false;
        std::string oneContent;
        while (std::getline(file, oneContent))
        {
            if (oneContent.length() == 0 || oneContent[0] == '#')
            {
                // comments
                continue;
            }
            sscanf(oneContent.c_str(), "%s %f %f %d %d %d %f %f %f\n", pinyin, &pause, &duration, &pwboundary, &tone, &stressed, &fP, &fA, &fD);

            // save pinyin, pause, duration
            for (p=pinyin; p!=NULL && *p!='\0'; p++)
                pinyins.push_back(*p);
            pinyins.push_back(' ');
            pauses.push_back((int)(pause*1000));
            durations.push_back((int)(duration*1000));

            // save head movement information
            sylOne.pad.P = fP;
            sylOne.pad.A = fA;
            sylOne.pad.D = fD;
            sylOne.stressed = (stressed==1);
            sylOne.tone = tone;

            passedFrames += pause * m_FPS;
            sylOne.start_frame_no = (int)passedFrames;
            passedFrames += duration * m_FPS;
            sylOne.end_frame_no = (int)passedFrames;

            //if (syllableIdx == pwStartIdx+1)
            //{
            //    // Previous syllable is prosodic boundary
            //    // Put focus on current syllable
            //    sylOne.pad.P = 1;
            //    sylOne.pad.A = 1;
            //    sylOne.stressed = true;
            //}

            if (pwboundary == 1)
            {
                // Prosodic boundary
                pwInfo.push_back(pwStartIdx);
                pwInfo.push_back(syllableIdx);
                pwStartIdx = syllableIdx + 1;
            }
            sylInfo.push_back(sylOne);
            syllableIdx ++;
        }
        return true;
    }

    bool FapMandarin::generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        std::vector<char> pinyins;
        std::vector<int>  pauses;
        std::vector<int>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // load syllable information from file
        loadPinyinInfo(filename, pinyins, pauses, durations, sylInfo, pwInfo);

		if (pinyins.size() == 0 || pauses.size() == 0 || durations.size() == 0)
		{
			// no data, return directly
			return true;
		}

        // generate visemes
        generateVisemes(&pinyins[0], &pauses[0], &durations[0], (int)durations.size(), visemes, begtimes);

        return true;
    }
}
