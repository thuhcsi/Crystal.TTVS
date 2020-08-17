
#ifndef _CST_TALKING_HEAD_CONCATENATE_H_
#define _CST_TALKING_HEAD_CONCATENATE_H_

#include "TalkingHead.h"

// forward class reference
namespace CSTHead {class FapTTVS; class FapMandarin; class FapCantonese; class FapEnglish;}
namespace CSTHead {class VectorInt; class VectorHandle; class Rect;}
class CanvasBmp;

///
/// @brief  The class for Concatenative Talking Head manipulation
///
class CSTHEAD_API TalkingHeadConcatenate : public TalkingHead
{
public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head initialization related operations
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Constructor
    ///
    /// @param  [in] hWnd       Handle (HWND) of the displaying window. NULL for off-screen rendering.
    /// @param  [in] keepRatio  Whether keep the vertical-horizontal ratio of the viseme image while rendering
    /// @param  [in] drawAlways Whether render module will alway redraw the window, even though displaying context is not changed
    ///                         (If it is false, the window will only be redrawn fater displaying context is changed.)
    ///
    TalkingHeadConcatenate(void* hWnd, bool keepRatio=true, bool drawAlways=false);

    ///
    /// @brief  Destructor
    ///
    virtual ~TalkingHeadConcatenate();

    ///
    /// @brief  Load the model (a series of viseme images) from the path, given the format template of the file name
    ///
    /// @param  [in] fileNameFmt    Format template of the file name of the viseme images
    /// @param  [in] filePath       Path where files are located
    ///
    virtual bool loadModel(const wchar_t *fileNameFmt, const wchar_t *filePath);

    ///
    /// @brief  Unload the model (deleted the viseme images)
    ///
    virtual bool unloadModel();


public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head animation related operations
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Generate the Talking Head face animation.
    ///
    /// @param  [in] fileName       Name of the file which stores the parameters for Talking Head animation
    /// @param  [in] languageTag    Language identification tag ("zh-cmn" for Mandarin, "zh-yue" for Cantonese)
    ///
    virtual bool generateAnimation(const wchar_t *fileName, const wchar_t *languageTag);

    ///
    /// @brief  Read the bitmap pixels (only B/G/R 3 bytes for one pixel) from the Talking Head rendering content
    ///
    /// @param  [in]  width         Width of the bitmap to be read
    /// @param  [in]  height        Height of the bitmap to be read
    /// @param  [out] pBitmapBits   Return the read bitmap data
    ///
    virtual bool readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const;


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
    virtual bool cameraWindow(int w, int h);


protected:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Member variables
    //
    //////////////////////////////////////////////////////////////////////////

    CSTHead::FapTTVS*       m_pFapStream;       ///< FAP generation module
    CSTHead::FapMandarin*   m_pFapMandarin;     ///< Mandarin FAP generation module
    CSTHead::FapCantonese*  m_pFapCantonese;    ///< Cantonese FAP generation module
    CSTHead::FapEnglish*    m_pFapEnglish;      ///< English FAP generation module

    CanvasBmp*              m_pCanvasBmp;       ///< Bitmap canvas for off-screen (memory bitmap) rendering
    int                     m_FPS;              ///< Number of frames per second to render
    unsigned long           m_BgColor;          ///< Background color
    void*                   m_hWndDC;           ///< Device context to the displaying window
    CSTHead::Rect*          m_pRectWndLT;       ///< Displaying area of left or top sub-window
    CSTHead::Rect*          m_pRectWndRB;       ///< Displaying area of right or bottom sub-window
    CSTHead::Rect*          m_pRectWndTH;       ///< Displaying area of Talking Head sub-window
    bool                    m_bKeepRatio;       ///< If keep vertical-horizontal ratio of viseme image when rendering
    bool                    m_bRedrawAlways;    ///< If the render module will alway redraw the window (Even though displaying context is not changed)

    CSTHead::VectorHandle*  m_pVisemeBmps;      ///< Handles to viseme bitmaps
    int                     m_nVisemeBmpWidth;  ///< Width  of viseme bitmap
    int                     m_nVisemeBmpHeight; ///< Height of viseme bitmap
    int                     m_nVisemeIdx;       ///< Index of current displaying viseme image
    int                     m_nVisemeIdxLst;    ///< Index of last displaying viseme image

    CSTHead::VectorInt*     m_pFrameVisemes;    ///< List of visemes at each frame
    size_t                  m_nFrameIdx;        ///< Index of current frame

};

#endif//_CST_TALKING_HEAD_CONCATENATE_H_
