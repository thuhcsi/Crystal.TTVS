
#include <Windows.h>
#include "Canvas.h"

CanvasBmp::CanvasBmp(HDC hdc) : Canvas(hdc)
{
    // get the desktop width and height as the maximum bitmap width and height
    int width  = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // create a memory DC compatible with the screen
    HDC hDC = CreateCompatibleDC(hdc);

    // create a bitmap compatible with the DC
    // must use CreateDIBSection(), and this means all pixel ops must be synchronized
    // using calls to GdiFlush() (see CreateDIBSection() docs)
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 24;       // RGB
    bmi.bmiHeader.biCompression = BI_RGB;   // uncompressed

    // create bitmap canvas for OpenGL rendering
    DWORD *pBmpBits = NULL;
    HBITMAP hBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pBmpBits, 0, 0);

    //  1. pBmpBits contains 3 bytes per point
    //  2. Lines are from the bottom to the top
    //  3. Points in the line are from the left to the right
    //  4. Bytes in one point are BGR (blue, green, red) not RGB
    //  5. Don't delete pBmpBits, it will be automatically deleted when delete hBmp

    // select the bitmap into the DC
    SelectObject(hDC, hBmp);

    // save the context
    m_hDC = hDC;
    m_hBitmap = hBmp;
}

CanvasBmp::~CanvasBmp()
{
    // delete context objects
    DeleteObject(m_hBitmap); // delete bitmap canvas
    DeleteDC(m_hDC);         // delete DC
}
