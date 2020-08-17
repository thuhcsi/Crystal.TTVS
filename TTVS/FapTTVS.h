
/**
 *  @file   FapTTVS.h
 *
 *	@brief  MPEG-4 FAPs generator from TTVS (Text-to-Visual-Speech) engine
 */

#pragma once
#ifndef CSTHEAD_FAP_TTVS_H_
#define CSTHEAD_FAP_TTVS_H_

#include <XFace/IFapStream.h>
#include "HeadMovement.h"

namespace CSTHead
{
    using XFace::IFapStream;
    using XFace::FAPU;

    /**
     *  @brief   MPEG-4 FAPs generator from TTVS (Text-to-Visual-Speech) engine
     *  @ingroup CSTHead
     *  @author  John.ZY.WU
     *  @version 1.0
     *  @date    2005-8-15
     *
     *  This class generates the FAPs from text with TTVS engine.
     *  The FAPs are generated using the dynamic viseme (visual-phoneme) model, which is
     *  the work of ZhiMing WANG and LianHong CAI from Tsinghua University, Beijing, China.
     *  For more detailed information please visit:
     *      http://hcsi.cs.tsinghua.edu.cn
     */
    class FapTTVS :	public IFapStream
    {
    protected:
        bool    m_bReady;                           ///< Whether FAPs are ready
        size_t  m_currentFAPIdx;                    ///< Current FAP index
        std::vector< std::vector<float> > m_FAPs;   ///< Generated FAPs
    private:
        // Head movement controls
        int m_headPitchStyle;
        int m_headYawStyle;
        int m_headRollStyle;
        int m_headPitchValue;
        int m_headYawValue;
        int m_headRollValue;
    private:
        struct FAP_DATA
        {
            char  name[12];
            float faps[68];
        };
        FAP_DATA m_visemeModel[29];     ///< Viseme model
        FAP_DATA m_emotionModel[7];     ///< Emotion model (FAP values for each basic emotion)
        float    m_emotionValue[7];     ///< Emotion control values
        bool     m_bModelReady;         ///< Whether Viseme and Emotion models are ready

    protected:
        // Model initialization
        bool initEmotionModel(const char* szModelPath);
        bool initVisemeModel(const char* szModelPath);

    protected:
        // Emotion movement
        bool addEmotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum);
        // Eye movement
        bool closeEye(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum);
        bool addEyeMotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum);
        // Head movement
        bool moveHead(std::vector< std::vector<float> >& faps, int nFrameIdx, int nFreq);
        bool addHeadRandomMotion(std::vector< std::vector<float> >& faps, int nFrameBeg, int nFrameNum);
        bool addHeadExpressiveMotion(const std::vector<HeadMovement::SyllableInfo> &sylInfo, const std::vector<int> &pwInfo);
        // FAP generation from viseme list
        bool mapVisemes2FAPs(std::vector<int>& visemes, std::vector<float>& begtimes);

    protected:
        // FAP adjustment
        void adjustFAPs();
        void scaleFAPs(const FAPU& fapu);
        static void scaleFAPVect(std::vector<float>& faps, const FAPU& fapu);
        static void unscaleFAPVect(std::vector<float>& faps, const FAPU& fapu);

    public:
        FapTTVS(void);
        ~FapTTVS(void);

        // Inherited from IFapStream
        virtual bool isEnd()  const {return m_currentFAPIdx==m_FAPs.size();};
        virtual bool isOpen() const {return m_bReady;};
        virtual bool open(std::istream&, const FAPU&);

        // Inherited from IFapStream
        virtual size_t getFAPCount() const {return m_FAPs.size();};
        virtual unsigned long getCurrentFAPId() const{return (unsigned long)m_currentFAPIdx;};
        virtual void next();
        virtual void rewind();

        // FAP model initialization and emotion type set
        bool setEmotion(int emotionType, float emotionValue);
        virtual bool initModel(const char* szModelPath);
        virtual bool freeModel();
    };
}

#endif//CSTHEAD_FAP_TTVS_H_
