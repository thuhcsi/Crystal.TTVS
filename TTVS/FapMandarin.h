
#pragma once
#ifndef CSTHEAD_FAP_MANDARIN_H_
#define CSTHEAD_FAP_MANDARIN_H_

#include "FapTTVS.h"
#include "HeadMovement.h"

namespace CSTHead
{
    ///
    /// @brief  The class for generating Viseme or FAP sequence from Chinese Mandarion pinyin sequence
    ///
    class FapMandarin : public FapTTVS
    {
    public:
        FapMandarin();
        virtual ~FapMandarin();

    protected:
        int  convertPinyin2Viseme(const char* pinyin, int* viseme);
        bool generateVisemes(const char* pinyin, const int* pauses, const int* durations, int numbers, std::vector<int>& visemes, std::vector<float>& begtimes);
        bool loadPinyinInfo(const char* filename, std::vector<char>& pinyins, std::vector<int>& pauses, std::vector<int>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo);

    public:
        // Generate FAP sequence from input file of pinyin sequence (detailed information in CPP file)
        bool generateFAPs(const char* pinyin, int* pauses, int* durations, int numbers);
        bool generateFAPs(const char* filename);
        bool generateFAPs(const char* filename, int framerate);

    public:
        // Generate Viseme sequence from input file of pinyin sequence (detailed information in CPP file)
        bool generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes);
    };
}

#endif//CSTHEAD_FAP_MANDARIN_H_
