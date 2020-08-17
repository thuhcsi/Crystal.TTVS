
#include "OpenGLES.h"

#ifdef WINCE
#   pragma comment( lib, "libGLES_CM.lib")
#   pragma message("        Adding OpenGL|ES: libGLES_CM.lib")

OpenGLES::OpenGLES()
{
    m_display = EGL_NO_DISPLAY;
    m_surface = EGL_NO_SURFACE;
    m_context = EGL_NO_CONTEXT;
}

OpenGLES::~OpenGLES()
{
    terminate();
}

bool OpenGLES::initialize(void* pParam)
{
    // Get window and device context
    HWND hWnd = (HWND)pParam;
    if (hWnd == NULL)
        return false;
    HDC hDC = GetDC(hWnd);
    if (hDC == NULL)
        return false;

    // initialize EGL
    EGLDisplay display = eglGetDisplay((NativeDisplayType)hDC);
    if (display == EGL_NO_DISPLAY)
        return false;

    int major, minor;
    if (eglInitialize(display, &major, &minor) == EGL_FALSE)
        return false;

    // setup context
    EGLConfig config;
    int cfgNum = 0;
    if (eglGetConfigs(display, &config, 1, &cfgNum) == EGL_FALSE || cfgNum == 0)
        return false;

    // create window surface
    EGLSurface surface = eglCreateWindowSurface(display, config, (NativeWindowType)hWnd, 0);
    if (surface == EGL_NO_SURFACE)
        return false;

    // create context
    EGLContext context = eglCreateContext(display, config, NULL, NULL);
    if (context == EGL_NO_CONTEXT)
        return false;

    // setup context
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
        return false;

    // store the information
    m_display = display;
    m_surface = surface;
    m_context = context;

    // post setup for default environment
    setupDefaultEnvironment();

    return true;
}

bool OpenGLES::terminate()
{
    // clear context
    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (m_context != EGL_NO_CONTEXT)
    {
        eglDestroyContext(m_display, m_context);
        m_context = NULL;
    }

    // destroy surface
    if (m_surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_display, m_surface);
        m_surface = NULL;
    }

    // terminate display
    if (m_display != EGL_NO_DISPLAY)
    {
        eglTerminate(m_display);
        m_display = NULL;
    }

    return true;
}

void OpenGLES::swapBuffers() const
{
    eglSwapBuffers(m_display, m_surface);
}

#else//WINCE

OpenGLES::OpenGLES()
{
}

OpenGLES::~OpenGLES()
{
    terminate();
}

bool OpenGLES::initialize(void* pParam)
{
    // post setup for default environment
    setupDefaultEnvironment();
    return true;
}

bool OpenGLES::terminate()
{
    return true;
}

void OpenGLES::swapBuffers() const
{
}

#endif//WINCE

