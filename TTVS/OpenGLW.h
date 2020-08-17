
#ifndef _CST_OPEN_GL_WINDOW_H_
#define _CST_OPEN_GL_WINDOW_H_

#include "OpenGL.h"

#if !defined(USE_EGL)
#   include <Windows.h>
#   include <gl/GL.h>
#   include <gl/GLU.h>
#endif

///
/// @brief  The OpenGL initialization class on Microsoft Windows operation system
///
class OpenGLW : public OpenGL
{
public:
    /// Constructor
    OpenGLW();

    /// Destructor
    virtual ~OpenGLW();

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

protected:
    ///
    /// @brief  Exchange the front and back buffers
    ///
    virtual void swapBuffers() const;

    ///
    /// @brief  Setup the pixel format for OpenGL
    ///
    bool setupPixelFormat(HDC hDC) const;

protected:
    HWND  m_hWnd;   ///< The window handle for OpenGL rendering
    HDC   m_hDC;    ///< The device context for OpenGL rendering
    HGLRC m_hGLRC;  ///< The OpenGL resource context (HGLRC)
};

#endif//_CST_OPEN_GL_WINDOW_H_
