
#include "OpenGLW.h"

#ifdef WIN32
#   pragma comment( lib, "OpenGL32.lib")
#   pragma message("        Adding OpenGL: OpenGL32.lib")
#   pragma comment( lib, "Glu32.lib")
#   pragma message("        Adding OpenGL: Glu32.lib")
#   pragma comment( lib, "GlAux.lib"	 )
#   pragma message("        Adding OpenGL: GLAux.lib")
#endif


OpenGLW::OpenGLW()
{
    m_hWnd  = NULL;
    m_hDC   = NULL;
    m_hGLRC = NULL;
}

OpenGLW::~OpenGLW()
{
    terminate();
}


bool OpenGLW::setupPixelFormat(HDC hDC) const
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

    // choose the pixel format
    int pixelformat = ChoosePixelFormat(hDC, &pfd);
    if (pixelformat == 0)
    {
        pixelformat = 1;
        if (DescribePixelFormat(hDC, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
            return false;
    }

    // set the pixel format
    if (!SetPixelFormat(hDC, pixelformat, &pfd))
        return false;

    return true;
}

bool OpenGLW::initialize(void* pParam)
{
    // setup the window and device context
    HWND hWnd = (HWND)pParam;
    if (hWnd == NULL)
        hWnd = GetDesktopWindow();
    HDC hDC = GetDC(hWnd);
    if (hDC == NULL)
        return false;

    // setup the pixel format
    if (!setupPixelFormat(hDC))
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

    // post setup for default environment
    setupDefaultEnvironment();

    return true;
}

bool OpenGLW::terminate()
{
    // clear OpenGL RC
    wglMakeCurrent(NULL, NULL);

    // delete RC
    if (m_hGLRC)
    {
        wglDeleteContext(m_hGLRC);
        m_hGLRC = NULL;
    }

    // release the DC
    if (m_hWnd && m_hDC)
    {
        ReleaseDC(m_hWnd, m_hDC);
        m_hWnd = NULL;
        m_hDC  = NULL;
    }

    return true;
}

void OpenGLW::swapBuffers() const
{
    SwapBuffers(wglGetCurrentDC());
}
