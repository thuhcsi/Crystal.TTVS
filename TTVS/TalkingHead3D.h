
#ifndef _CST_TALKING_HEAD_3D_H_
#define _CST_TALKING_HEAD_3D_H_


#include "TalkingHead.h"

// forward class reference
namespace XFace {class FaceBase;}
namespace XFaceApp {class ModelCamera;}
namespace CSTHead {class FapTTVS; class FapMandarin; class FapCantonese; class FapEnglish;}
class OpenGL;


///
/// @brief  The class for Talking Head manipulation
///
class CSTHEAD_API TalkingHead3D : public TalkingHead
{
public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head initialization related operations
    //
    //////////////////////////////////////////////////////////////////////////

    TalkingHead3D(void* hWnd);
    virtual ~TalkingHead3D();

    virtual bool loadModel(const wchar_t *fileName, const wchar_t *filePath);
    virtual bool unloadModel();
    virtual bool rebindTextures();


public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head rendering related operations
    //
    //////////////////////////////////////////////////////////////////////////

    virtual void render();
    virtual void update();
    virtual void rewind();
    virtual void next();
    virtual bool isEnd() const;
    virtual int getFPS() const;

public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head animation related operations
    //
    //////////////////////////////////////////////////////////////////////////

    virtual bool setEmotion(ETYPE emotionType, float emotionValue);
    virtual bool generateAnimation(const wchar_t *fileName, const wchar_t *languageTag);
    virtual bool readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const;

    ///
    /// @brief  Load Talking Head animation parameters from the FAP file.
    ///         Call this function before playing and rendering the animation.
    ///
    /// @param  [in] fileName   Name of the file which stores the FAP parameters for Talking Head animation
    ///
    virtual bool loadAnimationFromFAP(const wchar_t *fileName);

public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head camera related operations (for mouse gestures)
    //
    //////////////////////////////////////////////////////////////////////////

    virtual bool cameraModeSet(CMODE mode);
    virtual bool cameraStart(int x, int y);
    virtual bool cameraEnd();
    virtual bool cameraUpdate(int x, int y);
    virtual bool cameraApply();
    virtual bool cameraWindow(int w, int h);
    virtual bool cameraTranslation(float x, float y, float z);
    virtual bool cameraRotation(float x, float y, float z, float angle);


protected:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Member variables
    //
    //////////////////////////////////////////////////////////////////////////

    XFaceApp::ModelCamera*  m_pCamera;          ///< XFace model camera
    XFace::FaceBase*        m_pFaceEntry;       ///< XFace rendering module
    CSTHead::FapTTVS*       m_pFapStream;       ///< FAP generation module
    CSTHead::FapMandarin*   m_pFapMandarin;     ///< Mandarin FAP generation module
    CSTHead::FapCantonese*  m_pFapCantonese;    ///< Cantonese FAP generation module
    CSTHead::FapEnglish*    m_pFapEnglish;      ///< English FAP generation module
    OpenGL*                 m_pOpenGL;          ///< OpenGL rendering module
};

#endif//_CST_TALKING_HEAD_3D_H_
