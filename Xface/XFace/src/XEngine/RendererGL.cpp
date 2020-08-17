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
 * The Original Code is Xface Core Library; OpenGL Renderer.
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

#include <XEngine/RendererGL.h>
#ifdef WINCE
    #pragma message("		Adding OpenGL|ES lib (Embedded)...")
    #pragma comment( lib, "libGLES_CM.lib")
#elif WIN32
	#pragma message("		Adding OpenGL lib...")
	#pragma comment( lib, "OpenGL32.lib")
#endif
#include <XEngine/DeformableGeometry.h>
#include <XEngine/ITexture.h>

#include <XEngine/Drawable.h>
#include <XEngine/MeshManager.h>
#include <XEngine/TextureManager.h>
#include <string>
#include <iostream>
#include <fstream>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace XEngine{

RendererGL::RendererGL() : m_maxBufferID(0), m_rotateX(0), m_rotateY(0), m_rotateZ(0)
{
	initGLExtensions();
}
void RendererGL::initGLExtensions()
{
	/*
	//hanson: 2006/04/05	 
	// nullify the pointers
	glBindBufferARB           = NULL;
	glDeleteBuffersARB        = NULL;
	glGenBuffersARB           = NULL;
	glIsBufferARB             = NULL;
	glBufferDataARB           = NULL;
	glBufferSubDataARB        = NULL;
	glGetBufferSubDataARB     = NULL;
	glMapBufferARB            = NULL;
	glUnmapBufferARB          = NULL;
	glGetBufferParameterivARB = NULL;
	glGetBufferPointervARB    = NULL;
	glActiveTextureARB			= NULL;
	glClientActiveTextureARB	= NULL;
	*/

	// set the bool query storage to all false
	memset(&m_supported[0], 0, sizeof(bool)*MAX_EXTENSIONS);

	const unsigned char* ext = glGetString (GL_EXTENSIONS);
	if (ext==0) {
		return; 
	}
	std::string ext_str = (char*)ext;
/*
	std::ofstream extensionDump("extensions.txt");
	extensionDump << ext_str;
	extensionDump.close();
*/

	// GL_ARB_multitexture
	if(std::string::npos != ext_str.find("GL_ARB_multitexture") )
	{
		m_supported[XGL_ARB_multitexture] = true;

		/* Obtain the address of the extension entry points. */
		//glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)	wglGetProcAddress("glActiveTextureARB");
		//glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
		// Load the rest IF you need them, no need to store unused function pointers, yes I am lazy as well..
/*
		glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)	wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC) wglGetProcAddress("glMultiTexCoord1dARB");
        glMultiTexCoord1dvARB = (PFNGLMULTITEXCOORD1DVARBPROC) wglGetProcAddress("glMultiTexCoord1dvARB");
        glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
        glMultiTexCoord1fvARB = (PFNGLMULTITEXCOORD1FVARBPROC) wglGetProcAddress("glMultiTexCoord1fvARB");
        glMultiTexCoord1iARB = (PFNGLMULTITEXCOORD1IARBPROC) wglGetProcAddress("glMultiTexCoord1iARB");
        glMultiTexCoord1ivARB = (PFNGLMULTITEXCOORD1IVARBPROC) wglGetProcAddress("glMultiTexCoord1ivARB");
        glMultiTexCoord1sARB = (PFNGLMULTITEXCOORD1SARBPROC) wglGetProcAddress("glMultiTexCoord1sARB");
        glMultiTexCoord1svARB = (PFNGLMULTITEXCOORD1SVARBPROC) wglGetProcAddress("glMultiTexCoord1svARB");
        glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC) wglGetProcAddress("glMultiTexCoord2dARB");
        glMultiTexCoord2dvARB = (PFNGLMULTITEXCOORD2DVARBPROC) wglGetProcAddress("glMultiTexCoord2dvARB");
        glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) wglGetProcAddress("glMultiTexCoord2fvARB");
        glMultiTexCoord2iARB = (PFNGLMULTITEXCOORD2IARBPROC) wglGetProcAddress("glMultiTexCoord2iARB");
        glMultiTexCoord2ivARB = (PFNGLMULTITEXCOORD2IVARBPROC) wglGetProcAddress("glMultiTexCoord2ivARB");
        glMultiTexCoord2sARB = (PFNGLMULTITEXCOORD2SARBPROC) wglGetProcAddress("glMultiTexCoord2sARB");
        glMultiTexCoord2svARB = (PFNGLMULTITEXCOORD2SVARBPROC) wglGetProcAddress("glMultiTexCoord2svARB");
        glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC) wglGetProcAddress("glMultiTexCoord3dARB");
        glMultiTexCoord3dvARB = (PFNGLMULTITEXCOORD3DVARBPROC) wglGetProcAddress("glMultiTexCoord3dvARB");
        glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
        glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC) wglGetProcAddress("glMultiTexCoord3fvARB");
        glMultiTexCoord3iARB = (PFNGLMULTITEXCOORD3IARBPROC) wglGetProcAddress("glMultiTexCoord3iARB");
        glMultiTexCoord3ivARB = (PFNGLMULTITEXCOORD3IVARBPROC) wglGetProcAddress("glMultiTexCoord3ivARB");
        glMultiTexCoord3sARB = (PFNGLMULTITEXCOORD3SARBPROC) wglGetProcAddress("glMultiTexCoord3sARB");
        glMultiTexCoord3svARB = (PFNGLMULTITEXCOORD3SVARBPROC) wglGetProcAddress("glMultiTexCoord3svARB");
        glMultiTexCoord4dARB = (PFNGLMULTITEXCOORD4DARBPROC) wglGetProcAddress("glMultiTexCoord4dARB");
        glMultiTexCoord4dvARB = (PFNGLMULTITEXCOORD4DVARBPROC) wglGetProcAddress("glMultiTexCoord4dvARB");
        glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
        glMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC) wglGetProcAddress("glMultiTexCoord4fvARB");
        glMultiTexCoord4iARB = (PFNGLMULTITEXCOORD4IARBPROC) wglGetProcAddress("glMultiTexCoord4iARB");
        glMultiTexCoord4ivARB = (PFNGLMULTITEXCOORD4IVARBPROC) wglGetProcAddress("glMultiTexCoord4ivARB");
        glMultiTexCoord4sARB = (PFNGLMULTITEXCOORD4SARBPROC) wglGetProcAddress("glMultiTexCoord4sARB");
        glMultiTexCoord4svARB = (PFNGLMULTITEXCOORD4SVARBPROC) wglGetProcAddress("glMultiTexCoord4svARB");
*/
    }
	
	// GL_ARB_vertex_buffer_object
	/*
	//hanson :2006/04/05
	if(std::string::npos != ext_str.find("GL_ARB_vertex_buffer_object") )
	{
		m_supported[XGL_ARB_vertex_buffer_object] = true;
		glBindBufferARB           = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
        glDeleteBuffersARB        = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
        glGenBuffersARB           = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
        glIsBufferARB             = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
        glBufferDataARB           = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
        glBufferSubDataARB        = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
        glGetBufferSubDataARB     = (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress("glGetBufferSubDataARB");
        glMapBufferARB            = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
        glUnmapBufferARB          = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
        glGetBufferPointervARB    = (PFNGLGETBUFFERPOINTERVARBPROC)wglGetProcAddress("glGetBufferPointervARB");
	}
	*/
}

void RendererGL::prepareBufferedMesh(IndexedFaceSet* pMesh)
{
	if(!isExtSupported(XGL_ARB_vertex_buffer_object))
		return;
 /*   
	// Create a Vertex Buffer Object and an Element Buffer Object
	int sz = pMesh->getVertexCount();
    int nArrayObjectSize = (2* sizeof(Vector3) + sizeof(Vertex2D)) * pMesh->getVertexCount();
	char *pData = new char[nArrayObjectSize];
	memcpy(pData, pMesh->getVertices(), sizeof(Vector3)*sz);
	memcpy(&pData[sizeof(Vector3)*sz], pMesh->getNormals(), sizeof(Vector3)*sz);
	memcpy(&pData[2*sizeof(Vector3)*sz], pMesh->getTexCoords(), sizeof(Vertex2D)*sz);

    int nParam_ArrayObjectSize = 0;

	unsigned int id = 0;
    glGenBuffersARB( 1, &id );
	pMesh->setBufferID(id);
	m_maxBufferID = max(id, m_maxBufferID);

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, id );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, pData, GL_STATIC_DRAW_ARB );

    glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

    if( nParam_ArrayObjectSize <= 0 )
        return;
    
    // Don't forget the Element Buffer Object to hold our quad's indices.
	nArrayObjectSize = sizeof(unsigned short) * pMesh->getIndexCount();
    nParam_ArrayObjectSize = 0;

    glGenBuffersARB( 1, &id );
	pMesh->setIndexBufferID(id);
	m_maxBufferID = max(id, m_maxBufferID);

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, id );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, pMesh->getIndices(), GL_STATIC_DRAW_ARB );

    glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

    if( nParam_ArrayObjectSize <= 0 )
		return;

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );

	delete []pData;
*/
}

void RendererGL::deleteBuffers()
{
	if(!isExtSupported(XGL_ARB_vertex_buffer_object))
		return;

	for (unsigned int id = 1; id < m_maxBufferID; ++id)
	{
		/*
		//hanson:2006/04/05
		if( glIsBufferARB(id) )
			glDeleteBuffersARB( 1, &id );
			*/
	}
}

void RendererGL::doTexture(const ITexture& tex) const
{
	if(tex.getTextureType() == ITexture::TEXTURE2D)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex.getTextureID());
	}
}

void RendererGL::doGeometry(DeformableGeometry& mesh) const
{
//	if(mesh.getBufferID() > 0)
	if(false)
	{
/*		glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh.getBufferID() );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mesh.getIndexBufferID());

		long offset = sizeof(Vector3)*mesh.getVertexCount();
		glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(offset));
		glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(2*offset));

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);

		glDrawElements( GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
		//glDrawArrays( GL_TRIANGLES, 0, mesh.getIndexCount()/3);

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY);

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
*/	}
	else
	{
		if(mesh.needUpdate())
			mesh.update();
		glVertexPointer( 3, GL_FLOAT, 0, mesh.getDeformedVertices());
		glNormalPointer( GL_FLOAT, 0, mesh.getNormals());
		glTexCoordPointer( 2, GL_FLOAT, 0, mesh.getTexCoords());

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		
		glDrawElements( GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_SHORT, mesh.getIndices() );
		
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY);
	}
}

void RendererGL::doTransform(const Transform& trans) const
{
    Vector3 tr = trans.getTranslation();
    glTranslatef(tr.x, tr.y, tr.z);

    Vector3 sc = trans.getScale();
    glScalef(sc.x, sc.y, sc.z);

/*	Vector3 rotAx = trans.getRotationAxis();
	glRotatef(trans.getRotationAngle(), rotAx.x, rotAx.y, rotAx.z);

	Vector3 tr = trans.getTranslation();
	glTranslatef(tr.x, tr.y, tr.z);

	Vector3 sc = trans.getScale();
	glScalef(sc.x, sc.y, sc.z);
*/
}

/*!
	Renders a Drawable instance using OpenGL.
*/
void RendererGL::render(const Drawable* pDrawable) const
{
	GLenum err = glGetError();
		
	std::string MeshName = pDrawable->getMeshName();		
	DeformableGeometry* pMesh = MeshManager::getInstance()->getMesh(MeshName);
	if(!pMesh)
		return;	
	glPushMatrix();
	//------------------------------------------------------------------------
	if (MeshName=="song_hands-FACES"||
		MeshName=="song_legs-FACES" ||		
		MeshName=="song_suit-FACES" ||
        MeshName=="song_body-FACES" ||
        MeshName=="song_tube-FACES" ||
        MeshName=="zhang_body-FACES" ||
        MeshName=="cloth-FACES" ||
        MeshName=="body-FACES")

    {
    }
	else
    {
		glRotatef(m_rotateX, 1.0f, 0.0f, 0.0f );
		glRotatef(m_rotateY, 0.0f, 1.0f, 0.0f );
		glRotatef(m_rotateZ, 0.0f, 0.0f, 1.0f );
	}
	//------------------------------------------------------------------------
		//add by hanson 2006/03/26
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER,0.1f);
		glEnable(GL_ALPHA_TEST);
		
		//////////////////////////////////////////////////////////////////////////
		
	
	doTransform(pDrawable->getTransform());

	if(pDrawable->isTextureOn())
	{
		const ITexture* pTexture = TextureManager::getInstance()->getTexture(pDrawable->getTexName());
		if(pTexture)
			doTexture(*pTexture);
	}
	
	doGeometry(*pMesh);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

}  // namespace XFace