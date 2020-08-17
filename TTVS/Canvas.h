
#ifndef _CST_CANVAS_H_
#define _CST_CANVAS_H_

class Canvas
{
public:
    /// Constructor
    Canvas(HDC hDC) : m_hDC(hDC) {}

    /// Destructor
    virtual ~Canvas() {m_hDC=NULL;}

    ///
    /// @brief  Get device context for rendering
    ///
    HDC getDC() {return m_hDC;}

protected:
    HDC m_hDC;  ///< Device context for rendering
};


///
/// @brief  The bitmap canvas class for off-screen (memory bitmap) rendering
///
class CanvasBmp : public Canvas
{
public:
    /// Constructor
    CanvasBmp(HDC hdc);

    /// Destructor
    virtual ~CanvasBmp();

protected:
    HBITMAP m_hBitmap;  ///< Bitmap for storing the rendered stuff
};

#endif//_CST_CANVAS_H_
