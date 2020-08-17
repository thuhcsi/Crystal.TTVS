
#if defined(WIN32) || defined(WINCE)
#   include <windows.h>
#endif
#include <vector>
#include "TalkingHeadConcatenate.h"
#include "FapMandarin.h"
#include "FapCantonese.h"
#include "FapEnglish.h"
#include "Canvas.h"

namespace CSTHead
{
    class VectorInt : public std::vector<int> {};
    class VectorHandle : public std::vector<HBITMAP> {};
    class Rect : public RECT {};
};

TalkingHeadConcatenate::TalkingHeadConcatenate(void* hWnd, bool keepRatio, bool drawAlways) : TalkingHead(hWnd)
{
    // create instances
    m_bLoaded = false;
    m_pFapMandarin  = new CSTHead::FapMandarin();
    m_pFapCantonese = new CSTHead::FapCantonese();
    m_pFapEnglish   = new CSTHead::FapEnglish();
    m_pFapStream = m_pFapMandarin;

    // get device context
    // create bitmap canvas if necessary
    m_pCanvasBmp = NULL;
    if (hWnd == NULL)
    {
        m_pCanvasBmp = new CanvasBmp(NULL);
        m_hWndDC     = m_pCanvasBmp->getDC();
    }
    else
    {
        m_hWndDC = GetDC((HWND)hWnd);
    }

    // create windows
    m_pRectWndLT = new CSTHead::Rect();
    m_pRectWndRB = new CSTHead::Rect();
    m_pRectWndTH = new CSTHead::Rect();
    m_bKeepRatio = keepRatio;
    m_BgColor    = RGB(128, 128, 205);
    m_FPS        = 25;
    m_bRedrawAlways = drawAlways;

    // clear visemes
    m_pVisemeBmps      = new CSTHead::VectorHandle();
    m_nVisemeIdx       = 0;
    m_nVisemeIdxLst    = 0;
    m_nVisemeBmpWidth  = 0;
    m_nVisemeBmpHeight = 0;

    m_pFrameVisemes = new CSTHead::VectorInt();
    m_nFrameIdx     = 0;
}

TalkingHeadConcatenate::~TalkingHeadConcatenate()
{
    // unload model
    unloadModel();

    // delete instances
    delete m_pFapMandarin;
    delete m_pFapCantonese;
    delete m_pFapEnglish;
    delete m_pRectWndLT;
    delete m_pRectWndRB;
    delete m_pRectWndTH;
    delete m_pCanvasBmp;

    // reset instances
    m_pFapMandarin = NULL;
    m_pFapCantonese= NULL;
    m_pFapEnglish  = NULL;
    m_pFapStream   = NULL;

    m_pRectWndLT   = NULL;
    m_pRectWndRB   = NULL;
    m_pRectWndTH   = NULL;
    m_hWndDC       = NULL;
    m_bLoaded      = false;
    m_pCanvasBmp   = NULL;

    // clear visemes
    delete m_pVisemeBmps;
    delete m_pFrameVisemes;

    m_pVisemeBmps      = NULL;
    m_nVisemeIdx       = 0;
    m_nVisemeIdxLst    = 0;
    m_nVisemeBmpWidth  = 0;
    m_nVisemeBmpHeight = 0;

    m_pFrameVisemes = NULL;
    m_nFrameIdx     = 0;
}


//////////////////////////////////////////////////////////////////////////
//
//  Model related operations
//
//////////////////////////////////////////////////////////////////////////

std::string wstr2str(const std::wstring &wstr)
{
    int iLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    char* destBuf = new char[iLen+1];
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, destBuf, iLen, NULL, NULL) ;
    destBuf[iLen] = 0;
    std::string strTemp = destBuf;
    delete []destBuf;
    return strTemp;
}


bool TalkingHeadConcatenate::loadModel(const wchar_t *fileNameFmt, const wchar_t *filePath)
{
    if (m_bLoaded)
        return false;

    // initialize FAP generating module
    std::string strPath = wstr2str(filePath);
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

    // load viseme images
    fileNameFmt = L"%02d.bmp";
    wchar_t visemeFile[MAX_PATH];
    std::wstring fullPath, visemePath = std::wstring(filePath) + L"/../VisemeImages/";
    int nVisemeNum = 26;
    m_pVisemeBmps->reserve(nVisemeNum);
    for (int i=0; i<nVisemeNum; i++)
    {
        _snwprintf(visemeFile, MAX_PATH, fileNameFmt, i);
        fullPath = visemePath + visemeFile;
#if defined(WINCE)
        HBITMAP hVisemeBmp = (HBITMAP)SHLoadDIBitmap(fullPath.c_str());
#else
        HBITMAP hVisemeBmp = (HBITMAP)LoadImageW(NULL, fullPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
#endif
        //if (hVisemeBmp == NULL)
        //{
        //    // load failed
        //    unloadModel();
        //    return false;
        //}
        m_pVisemeBmps->push_back(hVisemeBmp);
    }
    // get viseme image size
    for (int i=0; i<nVisemeNum; i++)
    {
        if ( m_pVisemeBmps->at(i) != NULL )
        {
            BITMAP bm = {0};
            GetObject(m_pVisemeBmps->at(i), sizeof(BITMAP), &bm);
            m_nVisemeBmpHeight = bm.bmHeight;
            m_nVisemeBmpWidth  = bm.bmWidth;
            break;
        }
    }
    m_nVisemeIdxLst = -1;
    m_nVisemeIdx    = 0;

    m_bLoaded = true;

    // resize the displaying window
    cameraWindow(m_pRectWndRB->right, m_pRectWndRB->bottom);

    return true;
}

bool TalkingHeadConcatenate::unloadModel()
{
    if (!m_bLoaded)
        return true;

    // clear viseme images
    for (size_t i=0; i<m_pVisemeBmps->size(); i++)
    {
        DeleteObject(m_pVisemeBmps->at(i));
    }
    
    m_pVisemeBmps->clear();
    m_nVisemeIdx       = 0;
    m_nVisemeIdxLst    = 0;
    m_nVisemeBmpWidth  = 0;
    m_nVisemeBmpHeight = 0;

    m_pFrameVisemes->clear();
    m_nFrameIdx = 0;

    // close FAP generating models
    m_pFapMandarin->freeModel();
    m_pFapCantonese->freeModel();
    m_pFapEnglish->freeModel();

    // set the loading flag
    m_bLoaded = false;

    // resize the displaying window
    cameraWindow(m_pRectWndRB->right, m_pRectWndRB->bottom);

    return true;
}


//////////////////////////////////////////////////////////////////////////
//
//  Talking Head animation related operations
//
//////////////////////////////////////////////////////////////////////////

bool TalkingHeadConcatenate::generateAnimation(const wchar_t *fileName, const wchar_t *languageTag)
{
    if (!m_bLoaded)
        return false;

    // get visemes and begtimes
    std::vector<int> visemes;
    std::vector<float> begtimes;
    std::string strFile = wstr2str(fileName);
    std::wstring lang = languageTag;
    if (lang == L"zh-cmn")
    {
        // Mandarin
        m_pFapStream = m_pFapMandarin;
        m_pFapMandarin->generateVisemes(strFile.c_str(), visemes, begtimes);
    }
    else if (lang == L"zh-yue")
    {
        // Cantonese
        m_pFapStream = m_pFapCantonese;
        m_pFapCantonese->generateVisemes(strFile.c_str(), visemes, begtimes);
    }
    else if (lang == L"en")
    {
        // English
        m_pFapStream = m_pFapEnglish;
        m_pFapEnglish->generateVisemes(strFile.c_str(), visemes, begtimes);
    }
    else
    {
        return false;
    }

    // build the viseme list for each frame
    float frameDur = 1000.0f/m_FPS, curTime = 0;
    m_pFrameVisemes->clear();
    for (size_t i=1; i<begtimes.size(); i++)
    {
        while (curTime < begtimes[i])
        {
            m_pFrameVisemes->push_back(visemes[i-1]);
            curTime += frameDur;
        }
    }
    if (visemes.size() > 0)
        m_pFrameVisemes->push_back(visemes.back()); // last frame
    m_nFrameIdx = 0;

    return true;
}

bool TalkingHeadConcatenate::readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const
{
    // read bitmap pixels of current frame

    // create a memory DC compatible with the screen
    HDC hDCMem = CreateCompatibleDC((HDC)m_hWndDC);

    // create a bitmap compatible with the DC
    // must use CreateDIBSection(), and this means all pixel ops must be synchronized
    // using calls to GdiFlush() (see CreateDIBSection() docs)
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 24;       // RGB
    bmi.bmiHeader.biCompression = BI_RGB;   // uncompressed

    // create bitmap for memory DC
    unsigned char* pSrcBits = NULL;
    HBITMAP hBmpMem = CreateDIBSection(hDCMem, &bmi, DIB_RGB_COLORS, (void**)&pSrcBits, 0, 0);

    // select the bitmap canvas into the DC
    SelectObject(hDCMem, hBmpMem);

    // copy screen bitmap into the memory one
    BitBlt(hDCMem, 0, 0, width, height, (HDC)m_hWndDC, 0, 0, SRCCOPY);

    // copy to external buffer
    //  1. pSrcBits contains 3 bytes per point
    //  2. Lines are from the bottom to the top
    //  3. Points in the line are from the left to the right
    //  4. Bytes in one point are BGR (blue, green, red) not RGB
    //  5. Don't delete pSrcBits, it will be automatically deleted when delete hBmpMem

    // flip the image
    long rowBytes = 3 * width;
    unsigned char* pSrcRow = pSrcBits;
    unsigned char* pTrgRow = pBitmapBits + (height-1) * rowBytes;
    for (int c = 0; c < height; c++)
    {
        memcpy(pTrgRow, pSrcRow, rowBytes);
        pSrcRow += rowBytes;
        pTrgRow -= rowBytes;
    }

    // release DC
    DeleteObject(hBmpMem);
    DeleteDC(hDCMem);

    return false;
}


//////////////////////////////////////////////////////////////////////////
//
//  Talking Head rendering related operations
//
//////////////////////////////////////////////////////////////////////////

bool TalkingHeadConcatenate::cameraWindow(int w, int h)
{
    memset(m_pRectWndLT, 0, sizeof(RECT));
    memset(m_pRectWndRB, 0, sizeof(RECT));
    memset(m_pRectWndTH, 0, sizeof(RECT));
    
    // size of full screen is stored in RightBottom window
    m_pRectWndRB->right  = w;
    m_pRectWndRB->bottom = h;

    if (!isLoaded())
    {
        // size of full screen is already stored in RightBottom window
        return true;
    }

    // force redraw after resizing the window
    m_nVisemeIdxLst = -1;

    if (!m_bKeepRatio)
    {
        // display Talking Head in full screen
        m_pRectWndTH->right  = w;
        m_pRectWndTH->bottom = h;
        m_pRectWndRB->left   = w;
        m_pRectWndRB->top    = h;
        return true;
    }

    // display Talking Head while keeping vertical-horizontal ratio
    // re-calculate the displaying area of the Talking Head
    float fHS = (float)w / m_nVisemeBmpWidth;   // horizontal scale
    float fVS = (float)h / m_nVisemeBmpHeight;  // vertical scale
    if (fHS > fVS)
    {
        // vertical is fully fitted
        // adopt fVS
        m_pRectWndTH->top    = m_pRectWndLT->top    = m_pRectWndRB->top    = 0;
        m_pRectWndTH->bottom = m_pRectWndLT->bottom = m_pRectWndRB->bottom = h;
        m_pRectWndTH->left   = (int)(w - m_nVisemeBmpWidth*fVS) / 2;
        m_pRectWndTH->right  = (int)(w + m_nVisemeBmpWidth*fVS) / 2;
        m_pRectWndLT->left   = 0;
        m_pRectWndLT->right  = m_pRectWndTH->left;
        m_pRectWndRB->left   = m_pRectWndTH->right;
        m_pRectWndRB->right  = w;
    }
    else
    {
        // horizontal is fully fitted
        // adopt fHS
        m_pRectWndTH->left   = m_pRectWndLT->left  = m_pRectWndRB->left  = 0;
        m_pRectWndTH->right  = m_pRectWndLT->right = m_pRectWndRB->right = w;;
        m_pRectWndTH->top    = (int)(h - m_nVisemeBmpHeight*fHS) /2;
        m_pRectWndTH->bottom = (int)(h + m_nVisemeBmpHeight*fHS) /2;
        m_pRectWndLT->top    = 0;
        m_pRectWndLT->bottom = m_pRectWndTH->top;
        m_pRectWndRB->top    = m_pRectWndTH->bottom;
        m_pRectWndRB->bottom = h;
    }
    return true;
}

void TalkingHeadConcatenate::render()
{
    // DC, background color
    HDC hDCTgt = (HDC)m_hWndDC;
    HBRUSH brush = CreateSolidBrush(m_BgColor);

    if (!m_bLoaded)
    {
        // not loaded
        // just fill background
        FillRect(hDCTgt, m_pRectWndRB, brush);
        return;
    }

    // render current frame on the displaying window

    if (!m_bRedrawAlways && m_nVisemeIdx == m_nVisemeIdxLst)
        return;
    m_nVisemeIdxLst = m_nVisemeIdx;

    // create memory DC
    HDC hDCMem = CreateCompatibleDC(hDCTgt);

    // load bitmap
    SelectObject(hDCMem, m_pVisemeBmps->at(m_nVisemeIdx));

    // draw
    // fill background
    FillRect(hDCTgt, m_pRectWndLT, brush);
    FillRect(hDCTgt, m_pRectWndRB, brush);
    // stretch to displaying area
#if !defined(WINCE)
    SetStretchBltMode(hDCTgt, COLORONCOLOR);
#endif
    StretchBlt(hDCTgt, m_pRectWndTH->left, m_pRectWndTH->top, m_pRectWndTH->right-m_pRectWndTH->left, m_pRectWndTH->bottom-m_pRectWndTH->top, hDCMem, 0, 0, m_nVisemeBmpWidth, m_nVisemeBmpHeight, SRCCOPY);

    // release DC
    DeleteDC(hDCMem);
}

void TalkingHeadConcatenate::update()
{
    // update the index of the displaying viseme image
    if (!isEnd())
        m_nVisemeIdx = m_pFrameVisemes->at(m_nFrameIdx);
    else
        m_nVisemeIdx = 0;
}

void TalkingHeadConcatenate::rewind()
{
    m_nFrameIdx = 0;
}

void TalkingHeadConcatenate::next()
{
    // advance to next frame
    if (!isEnd())
    {
        //not last frame
        m_nFrameIdx ++;
    }
}

bool TalkingHeadConcatenate::isEnd() const
{
    return m_nFrameIdx == m_pFrameVisemes->size();
}

int TalkingHeadConcatenate::getFPS() const
{
    return m_FPS;
}
