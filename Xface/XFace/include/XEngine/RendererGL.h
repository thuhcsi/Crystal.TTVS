/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Xface Core Library.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	RendererGL.h
	\brief	OpenGL renderer.
*/
#ifndef RENDERERGL_H_
#define RENDERERGL_H_

#include "IRenderer.h"
#include "ITexture.h"
#include "Transform.h"
#include "DeformableGeometry.h"

#ifdef WIN32
#include <windows.h>
#endif

#if defined(USE_EGL)
#include <GLES/gl.h>
#else
#include <gl\gl.h>
//#include <gl\glext.h>
#endif

#define MAX_EXTENSIONS 1024

namespace XEngine{
/*!
	\brief OpenGL renderer.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003

	OpenGL implementation of the IRenderer interface. Using OpenGL API, rendering
	is handled.
*/
class RendererGL : public IRenderer
{
	unsigned int m_maxBufferID;
	enum XGL_EXTENSIONS
	{
		XGL_ARB_multitexture,
		XGL_ARB_vertex_buffer_object
	};
    /*
	//! Declare function pointers for extensions
	//! Multitexture
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
	// ARB_vertex_buffer_object
	PFNGLBINDBUFFERARBPROC           glBindBufferARB;
	PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB;
	PFNGLGENBUFFERSARBPROC           glGenBuffersARB;
	PFNGLISBUFFERARBPROC             glIsBufferARB;
	PFNGLBUFFERDATAARBPROC           glBufferDataARB;
	PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB;
	PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB;
	PFNGLMAPBUFFERARBPROC            glMapBufferARB;
	PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB;
	PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
	PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB;
    */

	bool isExtSupported(XGL_EXTENSIONS ext) const {return m_supported[ext];};
	bool m_supported[MAX_EXTENSIONS];	// assuming we will not support more than 1024 extensions :)
	void initGLExtensions();
	void prepareBufferedMesh(IndexedFaceSet* pMesh);
	void deleteBuffers();

	float m_rotateX, m_rotateY, m_rotateZ;
	void doTexture(const ITexture& tex) const;
	void doGeometry(DeformableGeometry& mesh) const;
	void doTransform(const Transform& trans) const;
public:
	void setRotation(float ang_x, float ang_y, float ang_z)
	{m_rotateX = ang_x; m_rotateY = ang_y; m_rotateZ = ang_z;}
	void render(const Drawable* pDrawable) const;
	RendererGL(void);
	~RendererGL(void){deleteBuffers();};
};
}
#endif

