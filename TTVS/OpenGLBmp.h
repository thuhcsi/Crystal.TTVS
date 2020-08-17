
#ifndef _CST_OPEN_GL_BMP_CANVAS_H_
#define _CST_OPEN_GL_BMP_CANVAS_H_

#include "OpenGLW.h"

///
/// @brief  The OpenGL initialization class for both on-screen (real window) and off-screen (memory bitmap) rendering
///
class OpenGLBmp : public OpenGLW
{
public:
    /// Constructor
    OpenGLBmp();

    /// Destructor
    virtual ~OpenGLBmp();

public:
    ///
    /// @brief  Initialize the OpenGL
    ///
    /// @param  [in] pPraram    The handle (HWND) of the displaying window. NULL for off-screen rendering
    ///
    virtual bool initialize(void* pParam);

    ///
    /// @brief  Terminate the OpenGL
    ///
    virtual bool terminate();

    ///
    /// @brief  Read the bitmap pixels (only B/G/R 3 bytes for one pixel) from the OpenGL rendering content
    ///
    /// The pixel format must be B/G/R, i.e. 3 bytes (24 bits) for one pixel.
    /// The width and height must be the same as the last resize() operation.
    /// Furthermore, the width and height must be aligned to 4 (i.e. width%4 == 0, height%4 == 0).
    /// Otherwise, the read bitmap data might be distorted (misaligned).
    ///
    /// @param  [in]  width         The width of the bitmap to be read
    /// @param  [in]  height        The height of the bitmap to be read
    /// @param  [out] pBitmapBits   Return the read bitmap data
    ///
    virtual bool readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const;

protected:
    ///
    /// @brief  Setup the pixel format for OpenGL
    ///
    bool setupPixelFormat(void *hDC, bool drawToBitmap);

    ///
    /// @brief  Start the OpenGL context for rendering directly to window
    ///
    bool startWindowContext(void *hWnd);

    ///
    /// @brief  Start the OpenGL context for rendering to bitmap
    ///
    bool startBitmapContext();

    ///
    /// @brief  End the OpenGL context, restore and delete all the context information
    ///
    void endContext();

protected:
    ///
    /// @brief  Save the bitmap content to the file
    ///
    static void saveBitmapFile(const char* fileName, unsigned char* pBitmap, int width, int height);

protected:
    int    m_nCanvasWidth;  ///< The width of the bitmap canvas
    int    m_nCanvasHeight; ///< The height of the bitmap canvas
    void * m_hOrgBmpCanvas; ///< The original bitmap replaced by current bitmap
    void * m_hCurBmpCanvas; ///< The current entire bitmap canvas for off-screen rendering
    void * m_pBitmapBits;   ///< The pointer for reading the bitmap bits

};

#endif//_CST_OPEN_GL_BMP_CANVAS_H_
