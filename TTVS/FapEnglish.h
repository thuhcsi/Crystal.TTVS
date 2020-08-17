
#pragma once
#ifndef CSTHEAD_FAP_ENGLISH_H_
#define CSTHEAD_FAP_ENGLISH_H_

#include "FapTTVS.h"
#include "HeadMovement.h"

namespace CSTHead
{
    ///
    /// @brief  The class for generating Viseme or FAP sequence from English Phoneme sequence
    ///
    class FapEnglish : public FapTTVS
    {
    public:
        FapEnglish();
        virtual ~FapEnglish();

    protected:
        int  convertPhoneme2Viseme(const char* phoneme, int* viseme);
        bool generateVisemes(const char* phoneme, const int* pauses, const int* durations, int numbers, std::vector<int>& visemes, std::vector<float>& begtimes);
        bool loadPhonemeInfo(const char* filename, std::vector<char>& phonemes, std::vector<int>& pauses, std::vector<int>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo);

    public:
        // Generate FAP sequence from input file of phoneme sequence (detailed information in CPP file)
        bool generateFAPs(const char* filename);

    public:
        // Generate Viseme sequence from input file of phoneme sequences (detailed information in CPP file)
        bool generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes);
    };
}

#endif//CSTHEAD_FAP_English_H_
