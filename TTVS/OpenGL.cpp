
#include <fstream>
#include "OpenGL.h"

#if defined(USE_EGL)
#   include <GLES/gl.h>
#else
#   include <Windows.h>
#   include <gl/GL.h>
#   include <gl/GLU.h>
#endif

OpenGL::OpenGL()
{
}

OpenGL::~OpenGL()
{
    terminate();
}

bool OpenGL::initialize(void* pParam)
{
    return false;
}

bool OpenGL::terminate()
{
    return true;
}

void OpenGL::setupDefaultEnvironment() const
{
    // turn off the lights
    setLight(false);

#if !defined(USE_EGL) // EGL does not support GL_COLOR_MATERIAL
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
#endif

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
}

void OpenGL::resize(int width, int height) const
{
    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity(); 
    if (height==0) height = 1;

    // setup the perspective projection matrix
    GLfloat viewAngle = 5.0;
    GLfloat aspectRatio = (GLfloat)width/height;
    GLfloat zNear = 1.0;
    GLfloat zFar = 10000.0;
#if defined(USE_EGL)
    GLfloat top = (GLfloat)tan(viewAngle*3.1415926/360.0) * zNear;
    GLfloat right = aspectRatio * top;
    glFrustumf(-right, right, -top, top, zNear, zFar);
#else
    gluPerspective(viewAngle, aspectRatio, zNear, zFar);
#endif

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void OpenGL::setLight(bool lightOn) const
{
    if (lightOn)
    {
        // turn on the lights

        // lights 0
        GLfloat  ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat  diffuseLight0[] = { 0.45f, 0.45f, 0.45f, 1.0f };
        GLfloat      specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat LightPosition0[] = {150.0f, 50.0f, 0.0f, 0.2f};

        // setup and enable light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular0);
        glLightfv(GL_LIGHT0,GL_POSITION,LightPosition0);
        glEnable(GL_LIGHT0);


        // lights 1
        GLfloat  ambientLight1[] = { 0.15f, 0.15f, 0.15f, 0.1f };
        GLfloat  diffuseLight1[] = { 0.25f, 0.25f, 0.2f, 0.1f };
        GLfloat      specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat LightPosition1[] = {-150.0f, 50.0f, 0.0f, 0.0f};

        // setup and enable light 1
        glLightfv(GL_LIGHT1,GL_AMBIENT,ambientLight1);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseLight1);
        glLightfv(GL_LIGHT1,GL_SPECULAR,specular1);
        glLightfv(GL_LIGHT1,GL_POSITION,LightPosition1);
        glEnable(GL_LIGHT1);


        // lights 2
        GLfloat  ambientLight2[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        GLfloat  diffuseLight2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat      specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat LightPosition2[] = { 20.0f, -50.0f, -120.0f, 0.0f};

        // setup and enable light 2
        glLightfv(GL_LIGHT2,GL_AMBIENT,ambientLight2);
        glLightfv(GL_LIGHT2,GL_DIFFUSE,diffuseLight2);
        glLightfv(GL_LIGHT2,GL_SPECULAR,specular2);
        glLightfv(GL_LIGHT2,GL_POSITION,LightPosition2);
        glEnable(GL_LIGHT2);
    }
    else
    {
        // turn off the lights

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);

        // light 0
        GLfloat  ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat  diffuseLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        GLfloat      specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat LightPosition0[] = {150.0f, 50.0f, 60.0f, 0.2f};
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight0);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight0);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular0);
        glLightfv(GL_LIGHT0,GL_POSITION,LightPosition0);
        glEnable(GL_LIGHT0);
    }
}

void OpenGL::renderBegin() const
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

#if defined(USE_EGL)
    glDepthRangef(0.00002, 1.0);
#else
    glDepthRange(0.00002, 1.0);
#endif
}

void OpenGL::renderEnd() const
{
    glPopMatrix();
    glFinish();
    glFlush();

    // swap front and back buffers
    swapBuffers();
}

void OpenGL::swapBuffers() const
{
    // implemented in specialized
}

bool OpenGL::readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const
{
#if defined(USE_EGL)
    return false;
#else
    // read from window
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBitmapBits);
    //glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pBitmapBits);

    // flip the bitmap
    flipBitmap(pBitmapBits, width, height, 3);

    return true;
#endif
}

const unsigned char* OpenGL::flipBitmap(unsigned char* pBitmap, int width, int height, int pixelBytes) const
{
    int nRowBytes = sizeof(unsigned char) * pixelBytes * width;
    unsigned char* pTmpRow = new unsigned char[pixelBytes * width];
    unsigned char* pSrcRow = pBitmap;
    unsigned char* pTrgRow = pBitmap + nRowBytes * (height-1);

    for (int i = 0; i< height/2; i++)
    {
        memcpy(pTmpRow, pTrgRow, nRowBytes);
        memcpy(pTrgRow, pSrcRow, nRowBytes);
        memcpy(pSrcRow, pTmpRow, nRowBytes);
        pSrcRow += nRowBytes;
        pTrgRow -= nRowBytes;
    }

    delete[] pTmpRow;
    return pBitmap;
}
