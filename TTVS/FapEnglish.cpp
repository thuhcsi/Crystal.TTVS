
#include "FapEnglish.h"
#include "HeadMovement.h"
#include <fstream>
#include <ctype.h>

namespace CSTHead
{
    FapEnglish::FapEnglish()
    {
    }

    FapEnglish::~FapEnglish()
    {
    }


    /**
     *  @brief  Convert the phoneme sequence string to viseme list
     *
     *  The English phoneme string is converted to viseme list according to the following table:
        @verbatim
        Phoneme Sample_Word Viseme_ID Comments
            ax  abide       22
            n   near        1,2     具有两种发音口型的音素，视位序号<12且不为0，其中单数视位序号对应开口音，双数对应闭口音
            l   look        3,4     For the phoneme with 2 possible visemes, odd Viseme_ID corresponds to 开口音, even ID for 闭口音
            s   see         11,12
            ih  ing         23
            t   tea         1,2
            r   rare        25
            k   care        5,6
            iy  easy        23
            d   door        1,2
            m   mom         13
            er  seller      22
            eh  bed         16
            aa  hard        15
            ae  cat         16
            b   beach       13
            z   zoo         11,12
            p   pop         13
            ow  low         19
            g   glass       5,6
            f   few         14
            ey  day         21
            ao  log         17
            v   view        24
            ay  child       16
            ng  ing         21
            uw  use         19
            hh  help        5,6
            w   what        18
            sh  ship        9,10
            ah  but         15
            jh  logist      10==>23 jh是由10和23两个口型视位连接而成的 ("jh" corresponds to 2 visemes: 10 and 23)
            ch  cheep       9,10
            y   y before uw 23
            aw  down        17
            th  thanks      7
            uh  wood        24
            oy  boy         17==>23 oy是由17和23两个口型视位连接而成的 ("oy" corresponds to 2 visems: 17 and 23)
            dh  these       7
            zh  pleasure    9,10
        @endverbatim
     *
     *  @param  phoneme [in]    The phoneme sequence string to be converted
     *  @param  viseme  [out]   Return the viseme list
     *
     *  @return Return the number of visemes in the viseme list result
     */
    int FapEnglish::convertPhoneme2Viseme(const char* phoneme, int* viseme)
    {
        int i = 0;
        unsigned char j = 0; // for the number of generated visemes

        viseme[0]=0;
        while(phoneme[i]>96 && phoneme[i]<123)
        {
            switch (phoneme[i])
            {
            case 'a':
                if (phoneme[i+1]=='x') // ax: 22
                {
                    viseme[j++]=22;
                    i++;
                }
                else if (phoneme[i+1]=='a'||phoneme[i+1]=='h') // aa, ah: 15
                {
                    viseme[j++]=15;
                    i++;
                }
                else if (phoneme[i+1]=='e'||phoneme[i+1]=='y') // ae, ay: 16
                {
                    viseme[j++]=16;
                    i++;
                }
                else if (phoneme[i+1]=='o'||phoneme[i+1]=='w') // ao, aw: 17
                {
                    viseme[j++]=17;
                    i++;
                }
                break;
            case 'e':
                if (phoneme[i+1]=='r') // er: 22
                {
                    viseme[j++]=22;
                    i++;
                }
                else if (phoneme[i+1]=='h') // eh: 16
                {
                    if (j>0 && viseme[j-1]<12) // for the phoneme with 2 possible visemes,
                        viseme[j-1]++;         // its viseme would change to 闭口音 before "eh" by increasing 1
                    viseme[j++]=16;
                    i++;
                }
                else if (phoneme[i+1]=='y') // ey: 21
                {
                    if (j>0 && viseme[j-1]<12) // change viseme to 闭口音 before "ey"
                        viseme[j-1]++; 
                    viseme[j++]=21;
                    i++;
                }
                break;
            case 'i':
                if (phoneme[i+1]=='h' || phoneme[i+1]=='y') // ih, iy: 23
                {
                    viseme[j++]=23;
                    i++;
                }
                break;
            case 'n': 
                if (phoneme[i+1]=='g') // ng: 21
                {
                    viseme[j++]=21;
                    i++;
                }
                else // n: 1
                {
                    viseme[j++]=1; // for phoneme with 2 possible visemes, default to 开口音 (with odd Viseme_ID)
                }
                break;
            case 'l': // l: 3
                viseme[j++]=3;
                break;
            case 'z':
            case 'c':
            case 's':
                if (phoneme[i+1]=='h') // zh, ch, sh: 9
                {
                    viseme[j++]=9;
                    i++;
                }
                else // z, c, s: 11
                {
                    viseme[j++]=11;
                }
                break;
            case 'r': // r: 25
                viseme[j++]=25;
                break;
            case 'g':
            case 'k':  // g, k: 5
                viseme[j++]=5;
                break;
            case 'h':
                if (phoneme[i+1]=='h') // hh: 5
                {
                    viseme[j++]=5;
                    i++;
                }
                break;
            case 'b':
            case 'p':
            case 'm': // b, p, m: 13
                viseme[j++]=13;
                break;
            case 'o':
                if (j>0 && viseme[j-1]<12) // change viseme to 闭口音 before "ow" or "oy"
                    viseme[j-1]++; 
                if (phoneme[i+1]=='w') // ow: 19
                {
                    viseme[j++]=19;
                    i++;
                }
                else if (phoneme[i+1]=='y') // oy: 17==>23 ("oy" corresponds to 2 visemes)
                {
                    viseme[j++]=17;
                    viseme[j++]=23;
                    i++;
                }
                break;
            case 'f': // f: 14
                viseme[j++]=14;
                break;
            case 'v': // v: 24
                viseme[j++]=24;
                break;
            case 'd':  
                if (phoneme[i+1]=='h') // dh: 7
                {
                    viseme[j++]=7;
                    i++;
                }
                else // d: 1
                {
                    viseme[j++]=1;
                }
                break;
            case 'w': // w: 18
                viseme[j++]=18;
                break;
            case 'j':
                if (phoneme[i+1]=='h') // jh: 10==>23 ("jh" corresponds to 2 visemes)
                {
                    viseme[j++]=10;
                    viseme[j++]=23;
                    i++;
                }
                break;
            case 'y': // y: 23
                viseme[j++]=23;
                break;
            case 't':
                if (phoneme[i+1]=='h') // th: 7
                {
                    viseme[j++]=7;
                    i++;
                }
                else // t: 1
                {
                    viseme[j++]=1;
                }
                break;
            case 'u':
                if (j>0 && viseme[j-1]<12) // change viseme to 闭口音 before "uw" or "uh"
                    viseme[j-1]++;
                if (phoneme[i+1]=='w') // uw: 19
                {
                    viseme[j++]=19;
                    i++;
                }
                else if (phoneme[i+1]=='h') // uh: 24
                {
                    viseme[j++]=24;
                    i++;
                }
                break;
            default:
                assert(false || "phoneme sequence error!");
                break;
            }
            i++; 
        }
        if(!j)
            viseme[j++]=0; //0 for mouth closing, not 1
        return j;
    }

    /**
     *  @brief  Generate the visemes and corresponding durations from the phoneme sequence string and durations
     *
     *  @param  phoneme   [in]  The phoneme sequence string
     *  @param  pauses    [in]  The pauses before each phoneme (in: ms)
     *  @param  durations [in]  The durations for each phoneme (in: ms)
     *  @param  numbers   [in]  The number of phonemes
     *
     *  @param  visemes  [out]  Return the viseme list
     *  @param  begtimes [out]  Return the corresponding beginning time for each viseme (in: ms)
     *
     *  @return true for successful operation
     */
    bool FapEnglish::generateVisemes(const char* phoneme, const int* pauses, const int* durations, int numbers, std::vector<int>& visemes, std::vector<float>& begtimes)
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
            while (isspace(phoneme[j])) j++;
            //while (!(phoneme[j]>='a' && phoneme[j]<='z')) j++;

            // Convert one phoneme to viseme list
            pnum = convertPhoneme2Viseme(&phoneme[j], pidx);
            interTime = 1.0f*durations[i]/pnum;

            // save information
            for (k=0; k<pnum; k++)
            {
                visemes.push_back(pidx[k]);
                begtimes.push_back(passedTime);
                passedTime += interTime;
            }

            // skip current phonetic_symbol
            if (i<numbers-1)
                while (!isspace(phoneme[j])) j++;
                //while (phoneme[j]>='a' && phoneme[j]<='z') j++;
        }

        // last silence
        visemes.push_back(0);
        begtimes.push_back(passedTime);

        return true;
    }

    /**
     *  @brief  Load phoneme sequence information from file,
     *          the format of the file is: (phoneme, pause_dur (second), duration, pw_boundary, dummy, stressed, P, A, D)
     *
     *  @param  filename  [in]  The file name to load phoneme sequence information from
     *
     *  @param  phonemes  [out] Return the English phoneme sequence
     *  @param  pauses    [out] Return the pause duration before each phoneme (in ms)
     *  @param  durations [out] Return the duration of each phoneme (in ms)
     *  @param  sylInfo   [out] Return the syllable related information
     *  @param  pwInfo    [out] Return the prosodic word related information
     *
     *  @return Whether information is loaded successfully
     */
    bool FapEnglish::loadPhonemeInfo(const char* filename, std::vector<char>& phonemes, std::vector<int>& pauses, std::vector<int>& durations, std::vector<HeadMovement::SyllableInfo>& sylInfo, std::vector<int>& pwInfo)
    {
        // get phoneme, pauses, durations, and HeadMovement related information
        char  phoneme[10], *p;
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
            sscanf(oneContent.c_str(), "%s %f %f %d %d %d %f %f %f\n", phoneme, &pause, &duration, &pwboundary, &tone, &stressed, &fP, &fA, &fD);

            // save phonetic_symbol, pause, duration
            for (p=phoneme; p!=NULL && *p!='\0'; p++)
                phonemes.push_back(*p);
            phonemes.push_back(' ');
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

    /**
     *  @brief  Generate the FAPs from English phoneme sequence file,
     *          the format of the file is: (phoneme, pause_dur (second), duration, pw_boundary, dummy, stressed, P, A, D)
     *
     *  @param  filename  [in]  The file name
     *
     *  @return true for successful operation
     */
    bool FapEnglish::generateFAPs(const char* filename)
    {
        std::vector<char> phonemes;
        std::vector<int>  pauses;
        std::vector<int>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // get phoneme, pauses, durations, and HeadMovement related information
        loadPhonemeInfo(filename, phonemes, pauses, durations, sylInfo, pwInfo);

        if (phonemes.size() == 0 || pauses.size() == 0 || durations.size() == 0)
        {
            // no data, return directly
            return true;
        }

        // generate visemes and FAPs
        std::vector<int> visemes;
        std::vector<float> begintimes;
        // generate visemes
        generateVisemes(&phonemes[0], &pauses[0], &durations[0], (int)durations.size(), visemes, begintimes);
        // map visemes to FAPs
        mapVisemes2FAPs(visemes, begintimes);
        // add emotion
        addEmotion(m_FAPs, 0, (int)m_FAPs.size());
        // add eye movement
        addEyeMotion(m_FAPs, 0, (int)m_FAPs.size());
        // add random head movement
        addHeadRandomMotion(m_FAPs, 0, (int)m_FAPs.size());
        //add expressive head movement
        addHeadExpressiveMotion(sylInfo, pwInfo);
        //addHeadMovement(&phonetic_symbols[0], &pauses[0], &durations[0], (int)durations.size());

        return true;
    }

    /**
     *  @brief  Generate the viseme list from English phoneme sequence file,
     *          the format of the file is: (phoneme, pause_dur (second), duration, pw_boundary, dummy, stressed, P, A, D)
     *
     *  @param  filename  [in]  The file name
     *
     *  @param  visemes  [out]  Return the viseme list
     *  @param  begtimes [out]  Return the corresponding beginning time for each viseme (in: ms)
     *
     *  @return true for successful operation
     */
    bool FapEnglish::generateVisemes(const char* filename, std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        std::vector<char> phonemes;
        std::vector<int>  pauses;
        std::vector<int>  durations;
        std::vector<HeadMovement::SyllableInfo> sylInfo;
        std::vector<int> pwInfo;

        // load phoneme sequence information from file
        loadPhonemeInfo(filename, phonemes, pauses, durations, sylInfo, pwInfo);

        if (phonemes.size() == 0 || pauses.size() == 0 || durations.size() == 0)
        {
            // no data, return directly
            return true;
        }

        // generate visemes
        generateVisemes(&phonemes[0], &pauses[0], &durations[0], (int)durations.size(), visemes, begtimes);

        return true;
    }
}
