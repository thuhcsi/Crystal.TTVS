
#include <Windows.h>
#include <fstream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "OpenGLBmp.h"


#ifdef WIN32
#pragma comment( lib, "OpenGL32.lib")
#pragma message("        Adding OpenGL: OpenGL32.lib")
#pragma comment( lib, "Glu32.lib")
#pragma message("        Adding OpenGL: Glu32.lib")
#pragma comment( lib, "GlAux.lib"	 )
#pragma message("        Adding OpenGL: GLAux.lib")
#endif


OpenGLBmp::OpenGLBmp()
{
    m_nCanvasWidth  = 0;
    m_nCanvasHeight = 0;
    m_hOrgBmpCanvas = NULL;
    m_hCurBmpCanvas = NULL;
    m_pBitmapBits   = NULL;
}

OpenGLBmp::~OpenGLBmp()
{
    terminate();
}


bool OpenGLBmp::setupPixelFormat(void* hDC, bool drawToBitmap)
{
    //static PIXELFORMATDESCRIPTOR pfd =
    //{
    //    sizeof(PIXELFORMATDESCRIPTOR),      // size of this pfd
    //        1,                              // version number
    //        PFD_DRAW_TO_WINDOW |            // support window
    //        PFD_SUPPORT_OPENGL |            // support OpenGL
    //        PFD_DOUBLEBUFFER,               // double buffered
    //        PFD_TYPE_RGBA,                  // RGBA type
    //        24,                             // 24-bit color bits
    //        0, 0, 0, 0, 0, 0,               // color bits ignored
    //        0,                              // no alpha buffer
    //        0,                              // shift bit ignored
    //        0,                              // no accumulation buffer
    //        0, 0, 0, 0,                     // accumulation bits ignored
    //        32,                             // 32-bit z-buffer
    //        0,                              // no stencil buffer
    //        0,                              // no auxiliary buffer
    //        PFD_MAIN_PLANE,                 // main layer
    //        0,                              // reserved
    //        0, 0, 0                         // layer masks ignored
    //};

    // setup format descriptor
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 24;
    pfd.cDepthBits  = 32;
    pfd.iLayerType  = PFD_MAIN_PLANE;

    if (drawToBitmap)
    {
        // draw to bitmap
        pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;// | PFD_DOUBLEBUFFER | PFD_SUPPORT_GDI;
    }

    // choose the pixel format
    int pixelformat = ChoosePixelFormat((HDC)hDC, &pfd);
    if (pixelformat == 0)
    {
        pixelformat = 1;
        if (DescribePixelFormat((HDC)hDC, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
            return false;
    }

    // set the pixel format
    // must be done *after* the bitmap is selected into DC for "drawToBitmap"
    if (!SetPixelFormat((HDC)hDC, pixelformat, &pfd))
        return false;

    return true;
}

bool OpenGLBmp::startBitmapContext()
{
    // get the desktop width and height as the maximum bitmap width and height
    int width  = m_nCanvasWidth  = GetSystemMetrics(SM_CXSCREEN);
    int height = m_nCanvasHeight = GetSystemMetrics(SM_CYSCREEN);

    // create a memory DC compatible with the screen
    HDC hDC = CreateCompatibleDC(NULL);
    m_hDC   = hDC;
    if (hDC == NULL)
        return false;

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
    //HBITMAP hBmp = CreateCompatibleBitmap(hDC, width, height);
    m_hCurBmpCanvas = hBmp;
    m_pBitmapBits   = pBmpBits;

    // select the bitmap canvas into the DC
    m_hOrgBmpCanvas = SelectObject(hDC, (HBITMAP)m_hCurBmpCanvas);
    if (m_hOrgBmpCanvas == NULL || m_hCurBmpCanvas == NULL)
        return false;

    // setup the pixel format
    if (!setupPixelFormat(hDC, true))
        return false;

    // create the OpenGL resource context (RC) and make it current to the thread
    m_hGLRC = wglCreateContext(hDC);
    if (m_hGLRC == NULL)
        return false;

    if (wglMakeCurrent(hDC, (HGLRC)m_hGLRC))
        return true;
    else
        return false;
}

bool OpenGLBmp::startWindowContext(void *hWindow)
{
    // setup the window and device context
    HWND hWnd = (HWND)hWindow;
    if (hWnd == NULL)
        return false;
    HDC hDC = GetDC(hWnd);
    if (hDC == NULL)
        return false;

    // setup the pixel format
    if (!setupPixelFormat(hDC, false))
    {
        ReleaseDC(hWnd, hDC);
        return false;
    }

    // setup OpenGL (associate OpenGL to current DC)
    HGLRC hGLRC = wglCreateContext(hDC);
    if (hGLRC == NULL)
    {
        ReleaseDC(hWnd, hDC);
        return false;
    }

    if (!wglMakeCurrent(hDC, hGLRC))
    {
        wglDeleteContext(hGLRC);
        ReleaseDC(hWnd, hDC);
        return false;
    }

    // store the information
    m_hWnd  = hWnd;
    m_hDC   = hDC;
    m_hGLRC = hGLRC;
    return true;
}

void OpenGLBmp::endContext()
{
    // clear OpenGL RC
    wglMakeCurrent(NULL, NULL);

    // delete RC
    if (m_hGLRC)
        wglDeleteContext((HGLRC)m_hGLRC);

    if (m_hCurBmpCanvas)
    {
        // rendering to bitmap, delete objects
        SelectObject((HDC)m_hDC, (HGDIOBJ)m_hOrgBmpCanvas); // remove bitmap from DC
        DeleteObject((HBITMAP)m_hCurBmpCanvas); // delete bitmap canvas
        DeleteDC((HDC)m_hDC); // delete DC
    }
    else
    {
        // rendering to window
        // must release DC
        ReleaseDC((HWND)m_hWnd, (HDC)m_hDC);
    }

    // set to empty
    m_hWnd          = NULL;
    m_hDC           = NULL;
    m_hGLRC         = NULL;
    m_nCanvasWidth  = 0;
    m_nCanvasHeight = 0;
    m_hOrgBmpCanvas = NULL;
    m_hCurBmpCanvas = NULL;
    m_pBitmapBits   = NULL;
}

bool OpenGLBmp::initialize(void* pParam)
{
    // setup the OpenGL context for rendering
    if (pParam == NULL)
    {
        // start the bitmap context
        if (!startBitmapContext())
            return false;
    }
    else
    {
        // start the common window context
        // associate OpenGL to current window
        if (!startWindowContext((HWND)pParam))
            return false;
    }

    // turn off the lights
    setLight(false);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Our shading model--Gouraud (smooth)
    glShadeModel(GL_SMOOTH);

    // Culling (hide by covering)
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);

    // Set the clear (background) color
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_ALPHA_TEST);

    return true;
}

bool OpenGLBmp::terminate()
{
    endContext();
    return true;
}

bool OpenGLBmp::readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const
{
    if (m_hCurBmpCanvas)
    {
        // read from bitmap
        //GdiFlush();

        //////////////////////////////////////////////////////////////////////////
        // Read the data directly

        int nSrcRowBytes = sizeof(unsigned char) * 3 * m_nCanvasWidth;
        int nTrgRowBytes = sizeof(unsigned char) * 3 * width;
        unsigned char* pSrcRow = (unsigned char*)m_pBitmapBits;
        unsigned char* pTrgRow = pBitmapBits + (height-1) * nTrgRowBytes;

        for (int i = 0; i < height; i++)
        {
            memcpy(pTrgRow, pSrcRow, nTrgRowBytes);
            pSrcRow += nSrcRowBytes;
            pTrgRow -= nTrgRowBytes;
        }

        return true;
    }
    else
    {
        // read from window
        return OpenGL::readBitmapPixels(pBitmapBits, width, height);
    }
}

void OpenGLBmp::saveBitmapFile(const char* fileName, unsigned char* pBitmap, int width, int height)
{
    // Fill header
    BITMAPINFOHEADER bih;
    bih.biWidth         = width;
    bih.biHeight        = height;
    bih.biPlanes        = 1;
    bih.biBitCount      =  3 * 8; // RGB
    bih.biSize          = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage     = 3 * width * height;
    bih.biCompression   = BI_RGB;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed       = 0;
    bih.biClrImportant  = 0;

    BITMAPFILEHEADER bfh;
    bfh.bfType      = MAKEWORD('B', 'M');
    bfh.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bih.biSizeImage;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write to the file
    FILE *fp = fopen(fileName, "wb");
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(pBitmap, sizeof(unsigned char), bih.biSizeImage, fp);
    fclose(fp);
}

int saveBitmapFile(HBITMAP hBitmap, const char *FileName)
{
    HDC hDC;
    //当前分辨率下每象素所占字节数
    int iBits;
    //位图中每象素所占字节数
    WORD wBitCount;
    //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
    DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
    //位图属性结构
    BITMAP Bitmap;
    //位图文件头结构
    BITMAPFILEHEADER bmfHdr;
    //位图信息头结构
    BITMAPINFOHEADER bi;
    //指向位图信息头结构
    LPBITMAPINFOHEADER lpbi;
    //定义文件，分配内存句柄，调色板句柄
    HANDLE fh, hDib, hPal,hOldPal=NULL;

    //计算位图文件每个像素所占字节数
    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1) wBitCount = 1;
    else if (iBits <= 4) wBitCount = 4;
    else if (iBits <= 8) wBitCount = 8;
    else wBitCount = 24;

    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;

    dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

    //为位图内容分配内存
    hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    // 处理调色板
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = ::GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }

    // 获取该调色板下新的像素值
    GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

    //恢复调色板
    if (hOldPal)
    {
        ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ::ReleaseDC(NULL, hDC);
    }

    //创建位图文件
    fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);


    if (fh == INVALID_HANDLE_VALUE) return 1;

    // 设置位图文件头
    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
    // 写入位图文件头
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    // 写入位图文件其余内容
    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    //清除
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);

    return 0;
}
