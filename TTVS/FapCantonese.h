
#pragma once
#ifndef CSTHEAD_FAP_CANTONESE_H_
#define CSTHEAD_FAP_CANTONESE_H_

#include "FapTTVS.h"
#include "FapMandarin.h"

namespace CSTHead
{
    ///
    /// @brief  The class for generating Viseme or FAP sequence from Chinese Cantonese syllable sequence
    ///
    class FapCantonese : public FapTTVS
    {
    public:
        FapCantonese();
        virtual ~FapCantonese();

    protected:
        struct VISEME_MAP
        {
            char  syllable[10]; ///< Cantonese syllable
            int   visemeIdx[3]; ///< Separated viseme index
            float duration[3];  ///< Duration (percentage) for each viseme
        };
        std::vector<VISEME_MAP> m_visemeMap;    ///< Viseme table mapping Cantonese syllable to visemes

    protected:
        bool loadVisemeMap(const char* szModelPath);
        int  convertSyllable2Viseme(const char* syllable, int* viseme, float* duration);
        bool generateVisemes(std::vector<std::string>& syllables, std::vector<float>& durations, std::vector<int>& visemes, std::vector<float>& begtimes);
        bool loadSyllableInfo(const char* filename, std::vector<std::string>& syllables, std::vector<float>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo);

    public:
        // Model file manipulation
        virtual bool initModel(const char* szModelPath);
        virtual bool freeModel();

    public:
        // Generate FAP sequence from input file of syllable sequence (detailed information in CPP file)
        bool generateFAPs(const char* syllableFileName);

    public:
        // Generate Viseme sequence from input file of syllable sequence (detailed information in CPP file)
        bool generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes);
    };
}

#endif//CSTHEAD_FAP_CANTONESE_H_
