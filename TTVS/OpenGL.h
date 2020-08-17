
#ifndef _CST_OPEN_GL_H_
#define _CST_OPEN_GL_H_

///
/// @brief  The interface for OpenGL initialization class
///
class OpenGL
{
public:
    /// Constructor
    OpenGL();

    /// Destructor
    virtual ~OpenGL();

public:
    ///
    /// @brief  Initialize the OpenGL
    ///
    /// @param  [in] pPraram    The handle (HWND) of the displaying window.
    ///
    virtual bool initialize(void* pParam);

    ///
    /// @brief  Terminate the OpenGL
    ///
    virtual bool terminate();

    ///
    /// @brief  Resize the OpenGL window
    ///
    virtual void resize(int width, int height) const;

    ///
    /// @brief  Set the light of the OpenGL window
    ///
    virtual void setLight(bool lightOn) const;

    ///
    /// @brief  Perform the pre-processing and preparation before rendering
    ///
    virtual void renderBegin() const;

    ///
    /// @brief  Perform the post-processing after rendering
    ///
    virtual void renderEnd() const;

    ///
    /// @brief  Read the bitmap pixels (only B/G/R 3 bytes for one pixel) from the OpenGL rendering content
    ///
    /// The pixel format must be B/G/R (not R/G/B), i.e. 3 bytes (24 bits) for one pixel.
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
    /// @brief  Setup the default environment variables
    ///
    virtual void setupDefaultEnvironment() const;

    ///
    /// @brief  Exchange the front and back buffers
    ///
    virtual void swapBuffers() const;

    ///
    /// @brief  Flip the bitmap vertically
    ///
    const unsigned char* flipBitmap(unsigned char* pBitmap, int width, int height, int pixelBytes) const;
};

#endif//_CST_OPEN_GL_H_
