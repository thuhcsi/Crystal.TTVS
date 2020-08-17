
#ifndef _CST_OPEN_GLES_H_
#define _CST_OPEN_GLES_H_

#include "OpenGL.h"

#if defined(USE_EGL)
#   include <GLES/gl.h>
#ifdef WINCE
#   include <GLES/egl.h>
#endif
#else
#   error Must be used under Embedded OpenGL version
#endif

///
/// @brief  The OpenGL initialization class for embedded system
///
class OpenGLES : public OpenGL
{
public:
    /// Constructor
    OpenGLES();

    /// Destructor
    virtual ~OpenGLES();

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

protected:
#ifdef WINCE
    EGLDisplay m_display;   ///< The display connection for OpenGL rendering
    EGLSurface m_surface;   ///< The window surface for OpenGL rendering
    EGLContext m_context;   ///< The OpenGL rendering context
#endif//WINCE
};

#endif//_CST_OPEN_GLES_H_
