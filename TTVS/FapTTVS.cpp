
#include "FapTTVS.h"
#include "TalkingHead.h"
#include <fstream>
#include <cstdio>
#include <cassert>

namespace CSTHead
{
    FapTTVS::FapTTVS(void) : m_bReady(false), m_currentFAPIdx(0), m_bModelReady(false)
    {
        m_FPS = 25;
        m_version = 2.1f;
        //
        m_headPitchStyle = abs(rand())%50;
        m_headYawStyle   = abs(rand())%50;
        m_headRollStyle  = abs(rand())%50;
        m_headPitchValue = 0;
        m_headYawValue   = 0;
        m_headRollValue  = 0;
        //
        memset(m_emotionValue, 0, sizeof(float)*7);
    }

    FapTTVS::~FapTTVS(void)
    {
    }

    bool FapTTVS::initModel(const char* szModelPath)
    {
        //
        if (!initEmotionModel(szModelPath))
            return false;
        if (!initVisemeModel(szModelPath))
            return false;
        m_bModelReady = true;
        return true;
    }

    bool FapTTVS::freeModel()
    {
        m_bModelReady = false;
        return true;
    }

    bool FapTTVS::open(std::istream& input, const FAPU& fapu)
    {
        if (!m_bModelReady)
            return false;

        m_bReady = false;

        // load FAPs from input stream
        std::string oneline;
        if (input)
        {
            // for FAP data storage
            m_FAPs.clear();
            std::vector<float> oneFAP;
            oneFAP.resize(68);
            // read FAPs for each line
            while (input)
            {
                // get one line
                for (size_t i = 0; i < 68; ++i)
                    input >> oneFAP[i];
                // save one line
                if (input)
                    m_FAPs.push_back(oneFAP);
            }
        }

        // set FAPs
        scaleFAPs(fapu);
        adjustFAPs();
        rewind();

        m_bReady = true;
        return true;
    }

    void FapTTVS::next()
    {
        // get the next FAP
        // clear if we are at the end (will cause crash somewhere probably, if not handled correctly)
        size_t fapCount = m_FAPs.size();
        if (m_currentFAPIdx == fapCount)
            m_currentFAP.clear();
        else if (m_currentFAPIdx == fapCount-1)
        {
            ++m_currentFAPIdx;
            m_currentFAP.clear();
        }
        else
            m_currentFAP = m_FAPs[++m_currentFAPIdx];
    }

    void FapTTVS::rewind()
    {
        m_currentFAPIdx = 0;
        if (m_FAPs.size()!=0)
            m_currentFAP = m_FAPs[0];
        else
            m_currentFAP.clear();
    }

    /**
     *	@brief  Adjust the FAP values to make sure it is ok
     *
     *  from XFace:
     *      This method adjusts the FAP values coming from xml2fap tool.
     *      In my opinion (Koray) FAPs has to come to this point already adjusted.
     *      But adding this method for a temporary time being.
     */
    void FapTTVS::adjustFAPs()
    {
        for(unsigned long i = 0; i < m_FAPs.size(); ++i)
        {
            if ( m_FAPs[i][51]!=0 && m_FAPs[i][4]==0)   // 8.2 VERT
                m_FAPs[i][4]=m_FAPs[i][51];
            if ( m_FAPs[i][4]!=0 && m_FAPs[i][51]==0 )	// 2.3 VERT
                m_FAPs[i][51]=m_FAPs[i][4];
            if ( m_FAPs[i][57]!=0 && m_FAPs[i][10]==0)	// 8.8 VERT
                m_FAPs[i][10]=m_FAPs[i][57];
            if ( m_FAPs[i][10]!=0 && m_FAPs[i][57]==0)	// 2.9 VERT
                m_FAPs[i][57]=m_FAPs[i][10];
            if ( m_FAPs[i][56]!=0 && m_FAPs[i][9]==0)	// 8.7 VERT
                m_FAPs[i][9]=m_FAPs[i][56];
            if ( m_FAPs[i][9]!=0 && m_FAPs[i][56]==0)	// 2.8 VERT
                m_FAPs[i][56]=m_FAPs[i][9];

            if ( m_FAPs[i][50]!=0 && m_FAPs[i][3]==0)	// 8.1 VERT
                m_FAPs[i][3]=m_FAPs[i][50];
            if ( m_FAPs[i][3]!=0 && m_FAPs[i][50]==0 )	// 2.2 VERT
                m_FAPs[i][50]=m_FAPs[i][3];
            if ( m_FAPs[i][55]!=0 && m_FAPs[i][8]==0)	// 8.6 VERT
                m_FAPs[i][8]=m_FAPs[i][55];
            if ( m_FAPs[i][8]!=0 && m_FAPs[i][55]==0)	// 2.7 VERT
                m_FAPs[i][55]=m_FAPs[i][8];
            if ( m_FAPs[i][54]!=0 && m_FAPs[i][7]==0)	// 8.5 VERT
                m_FAPs[i][7]=m_FAPs[i][54];
            if ( m_FAPs[i][7]!=0 && m_FAPs[i][54]==0)	// 2.6 VERT
                m_FAPs[i][54]=m_FAPs[i][7];

            if ( m_FAPs[i][5]!=0 && m_FAPs[i][52]==0)	// 2.4 ORIZ. 8.3 ORIZ.
                m_FAPs[i][52]=m_FAPs[i][5];
            if ( m_FAPs[i][52]!=0 && m_FAPs[i][5]==0)
                m_FAPs[i][5]=m_FAPs[i][52];

            if ( m_FAPs[i][6]!=0 && m_FAPs[i][53]==0)	// 2.5 ORIZ. 8.4 ORIZ.
                m_FAPs[i][53]=m_FAPs[i][6];
            if ( m_FAPs[i][53]!=0 && m_FAPs[i][6]==0)
                m_FAPs[i][6]=m_FAPs[i][53];

            if ( m_FAPs[i][11]!=0 && m_FAPs[i][58]==0)	// 2.4 VERT. 8.3 VERT.
                m_FAPs[i][58]=m_FAPs[i][11];

            if ( m_FAPs[i][12]!=0 && m_FAPs[i][59]==0)	// 2.5 VERT. 8.4 VERT.
                m_FAPs[i][59]=m_FAPs[i][12];
        }
    }

    /**
     *  @brief  Scale the FAPs using the FAPU
     */
    void FapTTVS::scaleFAPs(const FAPU& fapu)
    {
        for(unsigned long j = 0; j < m_FAPs.size(); ++j)
            scaleFAPVect(m_FAPs[j], fapu);
    }

    /**
     *	@brief  Scale the FAPs using the FAPU
     */
    void FapTTVS::scaleFAPVect(std::vector<float>& FAPs, const FAPU& fapu)
    {
        for(size_t i = 0; i < 68; ++i)
        {
            if( ( (i > 1) && (i < 5) ) || 
                ( (i > 6) && (i < 14)) ||
                ( (i > 14)&& (i < 18)) ||
                (i == 43) || (i == 45) || (i == 50) || (i == 51) ||
                ( (i > 53) && (i < 60)) )
            {
                FAPs[i] *= fapu.MNS;
            }
            else if ( (i == 5) || (i == 6) || (i == 14) || (i == 42) || 
                (i == 44) || (i == 52) || (i == 53) )
            {
                FAPs[i] *= fapu.MW;
            }
            else if ( ( (i > 17) && (i < 22) ) || (i == 28) || (i == 29))
            {
                FAPs[i] *= fapu.IRISD;
            }
            else if ( ( (i > 21) && (i < 26) ) || 
                ( (i > 45) && (i < 50) ) )
            {
                FAPs[i] *= fapu.AU;
            }
            else if ( ( (i > 25) && (i < 28) ) || 
                ( (i > 35) && (i < 40) ) )
            {
                FAPs[i] *= fapu.ES;
            }
            else if( ( (i > 29) && (i < 36) ) || 
                ( (i > 39) && (i < 42) ) || (i > 59) )
            {
                FAPs[i] *= fapu.ENS;
            }
        }
    }

    /**
     *	@brief  Un-scale the FAPs using FAPU, (Convert the FAPs to the unit of FAPU)
     */
    void FapTTVS::unscaleFAPVect(std::vector<float>& FAPs, const FAPU& fapu)
    {
        for(size_t i = 0; i < 68; ++i)
        {
            if( ( (i > 1) && (i < 5) ) || 
                ( (i > 6) && (i < 14)) ||
                ( (i > 14)&& (i < 18)) ||
                (i == 43) || (i == 45) || (i == 50) || (i == 51) ||
                ( (i > 53) && (i < 60)) )
            {
                FAPs[i] /= fapu.MNS;
            }
            else if ( (i == 5) || (i == 6) || (i == 14) || (i == 42) || 
                (i == 44) || (i == 52) || (i == 53) )
            {
                FAPs[i] /= fapu.MW;
            }
            else if ( ( (i > 17) && (i < 22) ) || (i == 28) || (i == 29))
            {
                FAPs[i] /= fapu.IRISD;
            }
            else if ( ( (i > 21) && (i < 26) ) || 
                ( (i > 45) && (i < 50) ) )
            {
                FAPs[i] /= fapu.AU;
            }
            else if ( ( (i > 25) && (i < 28) ) || 
                ( (i > 35) && (i < 40) ) )
            {
                FAPs[i] /= fapu.ES;
            }
            else if( ( (i > 29) && (i < 36) ) || 
                ( (i > 39) && (i < 42) ) || (i > 59) )
            {
                FAPs[i] /= fapu.ENS;
            }
        }
    }


    /**
     *  @brief  Set the emotion control value
     *
     *  @param  emotionType  [in]   The emotion type for which control value is set
     *  @param  emotionValue [in]   The control value to be set
     */
    bool FapTTVS::setEmotion(int emotionType, float emotionValue)
    {
        if (emotionType<NEUTRAL || emotionType>SURPRISE)
            return false;
        m_emotionValue[emotionType] = emotionValue;
        // update FAP
        std::vector<float> oneFAP;
        oneFAP.clear();
        for (int j=0; j<68; j++)
        {
            float value = 0;
            for (int i=NEUTRAL; i<=SURPRISE; i++)
                value += m_emotionModel[i].faps[j]*m_emotionValue[i];
            oneFAP.push_back(value);
        }
        m_FAPs.clear();
        m_FAPs.push_back(oneFAP);
        return true;
    }

    /**
     *  @brief  Add emotion to the FAPs
     */
    bool FapTTVS::addEmotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum)
    {
        for (int i=nFrameBeg; i<nFrameBeg+nFrameNum; i++)
            for (int j=0; j<68; j++)
                for (int k=NEUTRAL; k<=SURPRISE; k++)
                    faps[i][j] += m_emotionModel[k].faps[j]*m_emotionValue[k];
        return true;
    }

    /**
     *  @brief  Move the head
     */
    bool FapTTVS::moveHead(std::vector< std::vector<float> >& faps, int nFrameIdx, int nFreq)
    {
        nFreq = (nFreq==0)?50:nFreq;
        int i = nFrameIdx;
        // head pitch
        if (m_headPitchStyle>0)
        {
            m_headPitchStyle --;
            faps[i][47] = 0;
        }
        else if (m_headPitchStyle==0)
        {
            int random_n = rand();
            if (random_n%2)
                m_headPitchStyle = -1;
            else
                m_headPitchStyle = -2;
            m_headPitchValue = 0;
        }
        else
        {
            if (m_headPitchValue>15 || m_headPitchValue<-15)
            {
                if (m_headPitchStyle==-1)
                    m_headPitchStyle = -2;
                else
                    m_headPitchStyle = -1;
            }
            if (m_headPitchStyle==-1)
                faps[i][47] = 100.0f*m_headPitchValue--;
            else
                faps[i][47] = 100.0f*m_headPitchValue++;
            if (m_headPitchValue==0)
                m_headPitchStyle = abs(rand())%nFreq;
        }
        // head yaw
        if (m_headYawStyle>0)
        {
            m_headYawStyle --;
            faps[i][48] = 0;
        }
        else if (m_headYawStyle==0)
        {
            m_headYawStyle = -1;
            m_headYawValue = 0;
        }
        else
        {
            if (m_headYawValue>15)
                m_headYawStyle = -2;
            if (m_headYawStyle==-1)
                faps[i][48] = 100.0f*m_headYawValue++;
            else
                faps[i][48] = 100.0f*m_headYawValue--;
            if (m_headYawValue==0)
                m_headYawStyle = abs(rand())%nFreq;
        }
        // head roll
        if (m_headRollStyle>0)
        {
            m_headRollStyle --;
            faps[i][49] = 0;
        }
        else if (m_headRollStyle==0)
        {
            m_headRollStyle = -1;
            m_headRollValue = 0;
        }
        else
        {
            if (m_headRollValue>15)
                m_headRollStyle = -2;
            if (m_headRollStyle==-1)
                faps[i][49] = 50.0f*m_headRollValue++;
            else
                faps[i][49] = 50.0f*m_headRollValue--;
            if (m_headRollValue==0)
                m_headRollStyle = abs(rand())%nFreq;
        }
        return true;
    }

    /**
     *  @brief  Close the eyes then open
     */
    bool FapTTVS::closeEye(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum)
    {
        int nCenter = nFrameBeg+nFrameNum/2;
        for (int i=18; i<=19; i++)
            for (int j=nFrameBeg; j<nFrameBeg+nFrameNum; j++)
                faps[j][i] = 1050.0f*(nFrameNum-abs(nCenter-j))/nFrameNum; // top eyelid
        for (int i=20; i<=21; i++)
            for (int j=nFrameBeg; j<nFrameBeg+nFrameNum; j++)
                faps[j][i] = 250.0f*(nFrameNum-abs(nCenter-j))/nFrameNum;  // bottom eyelid
        return true;
    }

    /**
     *  @brief  Initialize the emotion (expression) model
     */
    bool FapTTVS::initEmotionModel(const char* szModelPath)
    {
        std::string strFileName = szModelPath;
        strFileName += "/../Fap/7_basic_emotion.fap";
        std::ifstream input(strFileName.c_str());
        if (input.fail())
            return false;
        std::string name;
        std::string stuff;
        bool isInit = true;
        bool isMask = true;
        int  mask[68];
        int  expressionNo = NEUTRAL;
        int  tmpFPS = 0;
        size_t nFAPs = 0;
        while ( !input.fail() )
        {
            if (input.peek() == '#') // "expression name"
            {
                input >> stuff; // #
                input >> name;  // expression name
                if (name=="anger")
                    expressionNo = ANGER;
                else if (name=="disgust")
                    expressionNo = DISGUST;
                else if (name=="fear")
                    expressionNo = FEAR;
                else if (name=="sadness")
                    expressionNo = SADNESS;
                else if (name=="surprise")
                    expressionNo = SURPRISE;
                else if (name=="joy")
                    expressionNo = JOY;
                else if (name=="neutral")
                    expressionNo = NEUTRAL;
                std::getline(input, stuff);
            }
            else if (isInit) // header info
            {
                input >> stuff; // version
                input >> stuff; // name
                input >> tmpFPS;// FPS
                input >> nFAPs; // nFAPs
                std::getline(input, stuff);
                isInit = false;
            }
            else if (isMask) // mask
            {
                for (int i=0; i<68; ++i)
                    input >> mask[i];
                isMask = false;	
                std::getline(input, stuff);
            }
            else // FAP
            {
                // fill the name now
                strcpy(m_emotionModel[expressionNo].name, name.c_str());
                // first item is the index of the current FAP (ensures synchronization)
                int row;
                input >> row;
                float* fap_set = m_emotionModel[expressionNo].faps;
                float fap_value;
                // then the values
                for (int i=0; i<68; ++i)
                {
                    if (mask[i]!=0)
                    {
                        input >> fap_value;
                        fap_set[i] = fap_value;
                    }
                }
                isMask = true;
                std::getline(input, stuff);
            }
        }
        return true;
    }

    /**
     *  @brief  Initialize the viseme model
     */
    bool FapTTVS::initVisemeModel(const char* szModelPath)
    {
        memset(m_visemeModel, 0, sizeof(FAP_DATA)*29);
        // load viseme FAP model
        std::string strFileName = szModelPath;
        strFileName += "/../Fap/fap.fdb";
        FILE* file = fopen(strFileName.c_str(), "rb");
        if (file==NULL)
            return false;
        fseek(file, 0, SEEK_END);
        int num = ftell(file) / sizeof(FAP_DATA);
        fseek(file, 0, SEEK_SET);
        fread(m_visemeModel, sizeof(FAP_DATA), num, file);
        fclose(file);
        return true;
    }

    /**
     *  @brief  Generate the FAP serials from the viseme list
     *
     *  @param  visemes  [in]   The viseme list
     *  @param  begtimes [in]   The begin time for each viseme
     */
    bool FapTTVS::mapVisemes2FAPs(std::vector<int>& visemes, std::vector<float>& begtimes)
    {
        // for FAP data storage
        std::vector<float> oneFAP;
        std::vector< std::vector<float> >& allFAP = m_FAPs;
        oneFAP.resize(68);
        allFAP.clear();

        // generate FAP for each time instance
        static int frameTime = 1000/m_FPS;
        int i=0, numbers = (int)visemes.size();
        float curTime = (numbers>0)?begtimes[0]:0;
        float lastTime = curTime;
        int lastViseme = 0, thisViseme; 
        while (i<numbers)
        {
            thisViseme = visemes[i];
            if (thisViseme==0)
            {
                if (i==numbers-1)
                {
                    // last silence, close mouth
                    float ratio, startTime = curTime, endTime = begtimes[i];
                    curTime += frameTime;
                    while (curTime<endTime)
                    {
                        ratio = (endTime-curTime)/(endTime-startTime);
                        for (int x=0; x<68; x++)
                            oneFAP[x] = m_visemeModel[lastViseme].faps[x]*ratio;
                        allFAP.push_back(oneFAP);
                        curTime += frameTime;
                    };
                    // last frame, force to be silence
                    for (int x=0; x<68; x++)
                        oneFAP[x] = (float)m_visemeModel[0].faps[x];
                    allFAP.push_back(oneFAP);
                    curTime += frameTime;
                }
                else if (i+1<numbers && begtimes[i+1]-begtimes[i]>3*frameTime)
                {
                    // close mouth if silence length > 3 frames
                    if (lastViseme!=0)
                    {
                        // if left is not silence, then close the mouth
                        float ratios[2] = {0.67f, 0.33f};
                        for (int y=0; y<2; y++)
                        {
                            for (int x=0; x<68; x++)
                                oneFAP[x] = m_visemeModel[lastViseme].faps[x]*ratios[y];
                            allFAP.push_back(oneFAP);
                            curTime += frameTime;
                        }
                    }
                    // insert silence
                    while (curTime<begtimes[i+1]-frameTime)
                    {
                        for (int x=0; x<68; x++)
                            oneFAP[x] = (float)m_visemeModel[0].faps[x];
                        allFAP.push_back(oneFAP);
                        curTime += frameTime;
                    };
                    lastViseme = 0;
                    lastTime = curTime;
                }
            }
            else
            {
                // none silence visemes
                float ratio, coar;
                float startTime = lastTime, endTime = begtimes[i];
                while (curTime+frameTime<endTime)
                {
                    curTime += frameTime;
                    ratio = (curTime-startTime)/(endTime-startTime);
                    if (thisViseme<14 && thisViseme%2)  // no modification for "b,p,m"
                        coar = 1;
                    else
                        coar = 1; // 0.8f
                    for (int x=0; x<68; x++)
                    {
                        oneFAP[x] = m_visemeModel[lastViseme].faps[x] +
                            (m_visemeModel[thisViseme].faps[x]-m_visemeModel[lastViseme].faps[x])*ratio*coar;
                    }
                    allFAP.push_back(oneFAP);
                }
                lastViseme = thisViseme;
                lastTime = endTime;
            }
            i++;
        }
        return true;
    }

    /**
     *  @brief  Add eye movement (eye close) to the FAPs
     */
    bool FapTTVS::addEyeMotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum)
    {
        int headFreq = 50*40/(1000/m_FPS);
        int eyeFrames = 5*40/(1000/m_FPS);
        for (int i=nFrameBeg; i<nFrameBeg+nFrameNum; i++)
        {
            if (m_headPitchStyle>headFreq*3/5 && m_headYawStyle%(headFreq*1/5)==0)
            {
                if (i+eyeFrames<nFrameBeg+nFrameNum)
                    closeEye(faps, i, eyeFrames);
            }
        }
        return true;
    }

    /**
     *  @brief  Add random head movement (head pitch/yaw/roll) to the FAPs
     */
    bool FapTTVS::addHeadRandomMotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum)
    {
        int headFreq = 50*40/(1000/m_FPS);
        for (int i=nFrameBeg; i<nFrameBeg+nFrameNum; i++)
        {
            moveHead(faps, i, headFreq);
        }
        return true;
    }

    /**
     *  @brief  Add expressive head movement, according to Syllable (P,A,D,Tone,Stressed) information
     *
     *  @param  sylInfo [in]    The syllable information, including P, A, D, Tone, Stressed.
     *  @param  pwInfo  [in]    The prosodic word boundary information
     */
    bool FapTTVS::addHeadExpressiveMotion(const std::vector<HeadMovement::SyllableInfo> &sylInfo, const std::vector<int> &pwInfo)
    {
        std::vector< std::vector<float> > faps;

        // get head movement
        HeadMovement::generateFAPs(sylInfo, pwInfo, faps);

        // add head movement
        size_t frameNum = (m_FAPs.size() < faps.size()) ? m_FAPs.size() : faps.size();
        size_t fapNum = 0;
        if (frameNum>0)
        {
            if (m_FAPs[0].size() != faps[0].size())
                return false;
            fapNum = m_FAPs[0].size();
        }
        for (size_t i=0; i<frameNum; i++)
            for (size_t j=0; j<fapNum; j++)
                m_FAPs[i][j] += faps[i][j];
        return true;
    }
}
