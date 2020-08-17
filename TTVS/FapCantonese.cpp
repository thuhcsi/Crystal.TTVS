
#include "FapCantonese.h"
#include <fstream>

#ifndef WIN32
#include <strings.h>
#define _stricmp strcasecmp
#endif

namespace CSTHead
{
    FapCantonese::FapCantonese()
    {
    }

    FapCantonese::~FapCantonese()
    {
    }

    bool FapCantonese::initModel(const char* szModelPath)
    {
        if (!loadVisemeMap(szModelPath))
        {
            return false;
        }
        return FapTTVS::initModel(szModelPath);
    }

    bool FapCantonese::freeModel()
    {
        m_visemeMap.clear();
        return FapTTVS::freeModel();
    }

    /**
     *  @brief  Load the map table for mapping the Cantonese syllables to visemes
     */
    bool FapCantonese::loadVisemeMap(const char* szModelPath)
    {
        m_visemeMap.clear();
        // load Cantonese IPA (viseme) map
        std::string strFileName = szModelPath;
        strFileName += "/../Fap/CanIPAMap.txt";
        FILE* file = fopen(strFileName.c_str(), "rt");
        if (file==NULL)
            return false;
        char cdump[10];
        int orgIdx, newIdx;
        std::vector<int> ipaMap;
        while (fscanf(file, "%d %s %d\n", &orgIdx, cdump, &newIdx)!=EOF)
        {
            ipaMap.push_back(newIdx);
        }
        fclose(file);
        // load Cantonese syllable separation list
        strFileName = szModelPath;
        strFileName += "/../Fap/CanSyllable.txt";
        file = fopen(strFileName.c_str(), "rt");
        if (file==NULL)
            return false;
        VISEME_MAP oneMap;
        char syllable[10];
        float fD, sD, tD;
        int   fV, sV, tV;
        while (fscanf(file, "%s %f %f %f %s %s %s %d %d %d\n", syllable, &fD, &sD, &tD, cdump, cdump, cdump, &fV, &sV, &tV)!=EOF)
        {
            strcpy(oneMap.syllable, syllable);
            oneMap.visemeIdx[0] = ipaMap[fV];
            oneMap.visemeIdx[1] = ipaMap[sV];
            oneMap.visemeIdx[2] = ipaMap[tV];
            oneMap.duration[0]  = fD;
            oneMap.duration[1]  = sD;
            oneMap.duration[2]  = tD;
            m_visemeMap.push_back(oneMap);
        }
        fclose(file);
        return true;
    }

    /**
     *  @brief  Map one Cantonese syllable to viseme and return corresponding duration
     *
     *  @param  syllable [in]   The Cantonese syllable string
     *  @param  viseme   [out]  Return the visemes
     *  @param  duration [out]  Return the viseme duration (percentage)
     *
     *  @return Return the viseme number in the viseme list result
     */
    int FapCantonese::convertSyllable2Viseme(const char* syllable, int* viseme, float* duration)
    {
        // remove the tone information
        char newsyl[10] = {0};
        size_t syllen = strlen(syllable);
        if (syllen>0 && syllable[syllen-1]>='0' && syllable[syllen-1]<='9')
            strncpy(newsyl, syllable, syllen-1);
        else
            strcpy(newsyl, syllable);

        // find syllable in the map table
        size_t i = 0;
        for (i=0; i<m_visemeMap.size(); i++)
            if (_stricmp(m_visemeMap[i].syllable, newsyl)==0)
                break;
        if (i>=m_visemeMap.size())
        {
            // no entry found, return viseme 0
            viseme[0] = 0;
            duration[0] = 1;
            return 1;
        }

        // get the viseme and duration
        VISEME_MAP& maps = m_visemeMap[i];
        int pnum = 0;
        for (int k=0; k<3; k++)
            if (maps.duration[k]!=0)
            {
                viseme[pnum] = maps.visemeIdx[k];
                duration[pnum] = maps.duration[k];
                pnum ++;
            };
        return pnum;
    }

    /**
     *  @brief  Generate the visemes and corresponding durations
     *
     *  @param  syllableFileName [in]   The filename where Cantonese syllable list is stored (output of CUVocal)
     *
     *  @param  syllables [in]  The Cantonese syllable list
     *  @param  durations [in]  The durations for each Cantonese syllable (unit: in ms)
     *  @param  visemes   [out] Return the viseme no.
     *  @param  begtimes  [out] Return the corresponding beginning time for each viseme (unit: in ms)
     */
    bool FapCantonese::generateVisemes(std::vector<std::string>& syllables, std::vector<float>& durations, std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        visemes.clear();
        begtimes.clear();

        if (syllables.size() != durations.size())
            return false;

        // get viseme information
        float  pdur[5], passedTime = 0.0f;
        int    pnum, pidx[5], visemeNum = 0;
        size_t sylNumber = syllables.size();
        for (size_t i=0; i<sylNumber; i++)
        {
            pnum = convertSyllable2Viseme(syllables[i].c_str(), pidx, pdur);
            for (int k=0; k<pnum; k++)
            {
                visemes.push_back(pidx[k]);
                begtimes.push_back(passedTime);
                passedTime += durations[i] * pdur[k];
            }
        }
        // last silence
        visemes.push_back(0);
        begtimes.push_back(passedTime);
        return true;
    }

    /**
     *  @brief  Generate the FAPs from Cantonese syllable list
     *
     *  @param  syllableFileName [in]   The filename where Cantonese syllable list is stored (output of CUVocal)
     */
    bool FapCantonese::generateFAPs(const char* syllableFileName)
    {
        std::vector<std::string> syllables;
        std::vector<float>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // get Jyutpings, durations, and HeadMovement related information
        loadSyllableInfo(syllableFileName, syllables, durations, sylInfo, pwInfo);

        // generate visemes and FAPs
        std::vector<int> visemes;
        std::vector<float> begintimes;
        // generate visemes
        generateVisemes(syllables, durations, visemes, begintimes);
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
        return true;
    }

    bool FapCantonese::loadSyllableInfo(const char* filename, std::vector<std::string>& syllables, std::vector<float>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo)
    {
        // get Jyutpings, durations, and HeadMovement related information
        HeadMovement::SyllableInfo sylOne;
        float passedFrames = 0;
        int   syllableIdx  = 0;
        int   pwStartIdx   = 0;
        char  syllable[10];
        float pause, duration, fP, fA, fD;
        int   pwboundary, tone, stressed;
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
            sscanf(oneContent.c_str(), "%s %f %f %d %d %d %f %f %f\n", syllable, &pause, &duration, &pwboundary, &tone, &stressed, &fP, &fA, &fD);

            // save Jyutping, duration
            syllables.push_back(syllable);
            durations.push_back(duration*1000);

            // head movement information
            sylOne.pad.P = fP;
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

    bool FapCantonese::generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        std::vector<std::string> syllables;
        std::vector<float>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // get Jyutpings, durations, and HeadMovement related information
        loadSyllableInfo(filename, syllables, durations, sylInfo, pwInfo);

        // generate visemes
        generateVisemes(syllables, durations, visemes, begtimes);

        return true;
    }
}
