
#include <fstream>
#include <XFace/FaceBase.h>
#include <XFace/IFapStream.h>
#include <XFace/FapFile.h>
#include <XFaceApp/ModelCamera.h>
#include <XEngine/RendererGL.h>
#include <XMath/Vector3.h>
#include <XMath/AxisAngle.h>
#include "TalkingHead3D.h"
#include "FapMandarin.h"
#include "FapCantonese.h"
#include "FapEnglish.h"

#if defined(USE_EGL)
#include "OpenGLES.h"
#else
#include "OpenGLW.h"
#endif

using XFaceApp::ModelCamera;
using XFace::FaceBase;
using XFace::FAPFile;
using XMath::AxisAngle;
using XMath::Vector3;
using CSTHead::FapMandarin;
using CSTHead::FapCantonese;
using CSTHead::FapEnglish;

#if defined(USE_EGL)
    // linking with Embedded version
    #ifdef _DEBUG
    #pragma comment( lib, "XFace_CE.lib")
    #pragma message("       Linking with XFace_CEd.lib (WinCE Debug)")
    #else
    #pragma comment( lib, "XFace_CE.lib")
    #pragma message("       Linking with XFace_CE.lib (WinCE Release)")
    #endif
#else
    // linking with Windows version
    #ifdef _DEBUG
    #pragma comment( lib, "XFace.lib")
    #pragma message("       Linking with XFace.lib (Win32 Debug)")
    #else
    #pragma comment( lib, "XFace.lib")
    #pragma message("       Linking with XFace.lib (Win32 Release)")
    #endif
#endif

TalkingHead3D::TalkingHead3D(void* hWnd) : TalkingHead(hWnd)
{
    // initialize instances
    m_bLoaded = false;
    m_pCamera = new ModelCamera();
    m_pFaceEntry = new FaceBase();
    m_pFapMandarin  = new FapMandarin();
    m_pFapCantonese = new FapCantonese();
    m_pFapEnglish = new FapEnglish();
    m_pFapStream = m_pFapMandarin;
    cameraModeSet(ZOOM);

    // initialize OpenGL
#if defined(USE_EGL)
    m_pOpenGL = new OpenGLES();
#else
    m_pOpenGL = new OpenGLW();
#endif
    if (m_pOpenGL)
        m_pOpenGL->initialize(hWnd);
}

TalkingHead3D::~TalkingHead3D()
{
    // unload model
    unloadModel();

    // close OpenGL
    if (m_pOpenGL)
        m_pOpenGL->terminate();

    // delete instances
    delete m_pFapMandarin;
    delete m_pFapCantonese;
    delete m_pFapEnglish;
    delete m_pFaceEntry;
    delete m_pCamera;
    delete m_pOpenGL;

    // reset instances
    m_pCamera = NULL;
    m_pFaceEntry = NULL;
    m_pFapMandarin = NULL;
    m_pFapCantonese = NULL;
    m_pFapEnglish = NULL;
    m_pFapStream = NULL;
    m_pOpenGL = NULL;
    m_bLoaded = false;
}


//////////////////////////////////////////////////////////////////////////
//
//  Model related operations
//
//////////////////////////////////////////////////////////////////////////

// defined in TalkingHeadConcatenate.cpp
extern std::string wstr2str(const std::wstring &wstr);

bool TalkingHead3D::loadModel(const wchar_t *fileName, const wchar_t *filePath)
{
    if (m_bLoaded)
        return false;

    std::string strFile = wstr2str(fileName);
    std::string strPath = wstr2str(filePath);

    // Initialize XFace model
    if (!m_pFaceEntry->init(strFile, strPath))
    {
        m_bLoaded = false;
        return false;
    }

    // Initialize FAP generating module
    if (!m_pFapMandarin->initModel(strPath.c_str()))
    {
        m_bLoaded = false;
        return false;
    }
    if (!m_pFapCantonese->initModel(strPath.c_str()))
    {
        m_bLoaded = false;
        return false;
    }
    if (!m_pFapEnglish->initModel(strPath.c_str()))
    {
        m_bLoaded = false;
        return false;
    }

    // Set global translation and rotation of the whole face (in FDP file)
    m_pCamera->setTranslation(m_pFaceEntry->getFDP()->getGlobalTranslation());
    m_pCamera->setAxisAngle(m_pFaceEntry->getFDP()->getGlobalAxisAngle());

    m_bLoaded = true;
    return true;
}

bool TalkingHead3D::unloadModel()
{
    if (!m_bLoaded)
        return true;

    // destroy the XFace model
    m_pFaceEntry->destroy();

    // close FAP generating models
    m_pFapMandarin->freeModel();
    m_pFapCantonese->freeModel();
    m_pFapEnglish->freeModel();

    // set the loading flag
    m_bLoaded = false;
    return true;
}

bool TalkingHead3D::rebindTextures()
{
    m_pFaceEntry->rebindTextures();
    return true;
}


//////////////////////////////////////////////////////////////////////////
//
//  Talking Head rendering related operations
//
//////////////////////////////////////////////////////////////////////////

void TalkingHead3D::render()
{
    if (m_pOpenGL)
        m_pOpenGL->renderBegin();

    // set camera zooming
    m_pCamera->apply();

    // render each drawable here
    if (m_bLoaded)
        m_pFaceEntry->render();

    //RendererGL renderer;
    //std::list<Drawable*> faceDrawables = m_pFaceEntry->getDrawables();
    //std::list<Drawable*>::const_iterator it = faceDrawables.begin();
    //while (it!=faceDrawables.end())
    //{
    //    renderer.render(*it);
    //    ++it;
    //}

    if (m_pOpenGL)
        m_pOpenGL->renderEnd();
}

void TalkingHead3D::rewind()
{
    // rewind the fap stream to begin
    if (m_bLoaded)
        m_pFapStream->rewind();
}

void TalkingHead3D::next()
{
    if (m_bLoaded)
        m_pFapStream->next();
}

void TalkingHead3D::update()
{
    if (!m_bLoaded)
        return;
    // get current fap
    const std::vector<float> &fap = m_pFapStream->getCurrentFAP();
    // update the face engine
    if (!fap.empty())
        m_pFaceEntry->update(fap);
}

bool TalkingHead3D::isEnd() const
{
    if (!m_bLoaded)
        return true;
    return m_pFapStream->isEnd();
}

int TalkingHead3D::getFPS() const
{
    if (!m_bLoaded)
        return 0;
    // return the frame rate (Frames Per Second)
    return m_pFapStream->getFPS();
}


//////////////////////////////////////////////////////////////////////////
//
//  Camera related operations
//
//////////////////////////////////////////////////////////////////////////

bool TalkingHead3D::cameraStart(int x, int y)
{
    m_pCamera->start(x, y);
    return true;
}

bool TalkingHead3D::cameraEnd()
{
    m_pCamera->end();
    return true;
}

bool TalkingHead3D::cameraUpdate(int x, int y)
{
    m_pCamera->update(x, y);
    return true;
}

bool TalkingHead3D::cameraApply()
{
    m_pCamera->apply();
    return true;
}

bool TalkingHead3D::cameraWindow(int w, int h)
{
    // resize the camera
    m_pCamera->setScreenSize(w, h);
    // resize the OpenGL
    m_pOpenGL->resize(w, h);
    return true;
}

bool TalkingHead3D::cameraModeSet(CMODE mode)
{
    ModelCamera::kMODE cMode;
    switch(mode)
    {
    case ZOOM:  cMode = ModelCamera::ZOOM;  break;
    case ROTATE:cMode = ModelCamera::ROTATE;break;
    case PAN:   cMode = ModelCamera::PAN;   break;
    }
    m_pCamera->setMode(cMode);
    m_CameraMode = mode;
    return true;
}

bool TalkingHead3D::cameraTranslation(float x, float y, float z)
{
    Vector3 trans;
    trans.x = x;
    trans.y = y;
    trans.z = z;
    m_pCamera->setTranslation(trans);
    return true;
}

bool TalkingHead3D::cameraRotation(float x, float y, float z, float angle)
{
    AxisAngle rot;
    rot.setAxis(x, y, z);
    rot.setAngle(angle);
    m_pCamera->setAxisAngle(rot);
    return true;
}


//////////////////////////////////////////////////////////////////////////
//
//  Talking Head animation related operations
//
//////////////////////////////////////////////////////////////////////////

bool TalkingHead3D::setEmotion(ETYPE emotionType, float emotionValue)
{
    if (!m_bLoaded)
        return false;
    m_pFapMandarin->setEmotion(emotionType, emotionValue);
    m_pFapCantonese->setEmotion(emotionType, emotionValue);
    m_pFapEnglish->setEmotion(emotionType, emotionValue);
    // update
    // open FAP stream
    std::ifstream fapfile;
    if (!m_pFapStream->open(fapfile, m_pFaceEntry->getFAPU()))
        return false;
    std::vector<float> fap = m_pFapStream->getCurrentFAP();
    if (!fap.empty())
    {
        m_pFaceEntry->update(fap);
    }
    return true;
}

bool TalkingHead3D::generateAnimation(const wchar_t *fileName, const wchar_t *languageTag)
{
    if (!m_bLoaded)
        return false;

    std::string strFile = wstr2str(fileName);
    std::wstring lang = languageTag;
    if (lang == L"zh-cmn")
    {
        // Mandarin
        m_pFapStream = m_pFapMandarin;
        m_pFapMandarin->generateFAPs(strFile.c_str());
    }
    else if (lang == L"zh-yue")
    {
        // Cantonese
        m_pFapStream = m_pFapCantonese;
        m_pFapCantonese->generateFAPs(strFile.c_str());
    }
    else if (lang == L"en")
    {
        // English
        m_pFapStream = m_pFapEnglish;
        m_pFapEnglish->generateFAPs(strFile.c_str());
    }
    else
    {
        return false;
    }

    // open FAP stream
    // The following procedure will NOT load FAPs from an FAP stream,
    // as the FAP stream has been set to be open failed.
    // The FAPs generated by the above method will be kept unchanged
    std::ifstream fapfile;
    fapfile.setstate(fapfile.rdstate() | std::ios::failbit);
    if (!m_pFapStream->open(fapfile, m_pFaceEntry->getFAPU()))
        return false;
    return true;
}

bool TalkingHead3D::loadAnimationFromFAP(const wchar_t *fileName)
{
    if (!m_bLoaded)
        return false;

    // Assume we are using Mandarin FAP stream.
    // In fact, it does not matter whether we use Mandarin or Cantonese FAP steam.
    // All the animation parameters are stored in FAP file directly.
    m_pFapStream = m_pFapMandarin;

    // open FAP stream
    std::string strFile = wstr2str(fileName);
    std::ifstream fapfile(strFile.c_str());
    if (!fapfile)
        return false;

    // load FAPs from FAP stream
    if (!m_pFapStream->open(fapfile, m_pFaceEntry->getFAPU()))
        return false;
    return true;
}

bool TalkingHead3D::readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const
{
    if (m_pOpenGL == NULL)
        return false;
    return m_pOpenGL->readBitmapPixels(pBitmapBits, width, height);
}
