

#pragma once
#ifndef CSTHEAD_PAD_TO_HEAD_H_
#define CSTHEAD_PAD_TO_HEAD_H_

#include <vector>

namespace CSTHead
{
    class HeadMovement
    {
    public:
        HeadMovement() {}
        ~HeadMovement() {}

    public:
        struct PAD
        {
            float P;
            float A;
            float D;
        };

        struct SyllableInfo
        {
            PAD  pad;
            int  start_frame_no;
            int  end_frame_no;
            bool stressed;
            int  tone;
        };

    public:
        // Generate FAP stream based on PAD, Tone and Stress
        static void generateFAPs(const std::vector<SyllableInfo>& sylInfo, const std::vector<int>& pwInfo, std::vector< std::vector<float> >& faps);

    protected:
        // set head movement average position and amplitude for each PW 
        static void GetHeadPos(const PAD pad,float &pos,float &amp);

        // get mean value of a series data of a vector
        static float GetMeanValue(const std::vector<float> data,const int s,const int e);

        // get PAD combination TYPE
        static int GetPADType(const PAD pad);

        static void smooth(std::vector<float> &data,const int win_len);

        static unsigned int getRandSeed();
    };
}

#endif//CSTHEAD_PAD_TO_HEAD_H_
