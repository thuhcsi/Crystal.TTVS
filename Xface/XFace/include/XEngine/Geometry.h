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
 * The Original Code is Xface Core Library; XEngine.
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
	\file	Geometry.h
	\brief	Defines interface for the mesh geometry.
*/

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "VertexStruct.h"

#include <vector>
#include <XMath/Vector3.h>
#include "NamedObj.h"

using namespace XMath;
namespace XEngine{


/*!
	\brief Defines interface for the mesh geometry.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003
			
	Base class for the triangle mesh. Stores the vertices (not indices) and other
	related data. Every IMesh object is named and stored in MeshManager to be used 
	through its name. The same name is assigned to Drawable instance and the make
	a couple for the mesh, MeshManager stores the IMesh instances, and RenderList 
	stores the Drawables and Renderer renders IMesh instances by retrieving them 
	using the registered names in Drawable instance. 

	IMesh objects are created and stored inside ISceneLoader (or derived) instances,
	and stored in MeshManager class. 

	Here is a typical usage in ISceneLoader;
	\code
	IndexedFaceSet* m_pMesh = new IndexedFaceSet("TestMesh"); // Create an instance (IndexedFaceSet IS-A IMesh)
	// Set a unique name in ctor
	m_pMesh->setVertices(m_Vertices);	// Set the vertices and others..
	m_pMesh->setIndices(m_Indices);		// Set the indices
	m_pMesh->createMesh();			// You SHOULD call this one to get normals correctly!

	// MeshManager is a singleton class.
	MeshManager::getInstance()->registerMesh(m_pMesh); // Add the mesh to MeshManager
	\endcode

	Then when you need to use that mesh in a different place (e.g. in Renderer);
	\code
	const IndexedGFaceSet* test = MeshManager::getInstance()->getMesh("TestMesh");
	\endcode

	\note Texture coordinates, normals and vertices are stored in seperate arrays. 
	Maybe we can consider using a single array (interleaved) later on. And also 
	currently only one texture layer is supported. For multitexturing we might add
	more arrays for texture coordinates in the future versions.
*/
class Geometry : public NamedObj
{
protected:
	// data members
	std::vector<Vector3> m_vertices;
	std::vector<Vertex2D> m_texCoords;
	std::vector<Vector3> m_normals;
/*
	bool m_bBuffered;
	int m_bufferID;
*/
public:
/*	bool isBufferedMesh() const {return m_bBuffered;};
	void setBufferedMesh(bool buf){m_bBuffered = buf;};
	void setBufferID(int val){m_bufferID = val;};
	int getBufferID() const {return m_bufferID;};
*/
	virtual Geometry& copyFrom(const Geometry& rhs);
	virtual void setVertices(const Vector3* pVert, unsigned int size);
	virtual void setVertices(const std::vector<Vector3> &vertices);
	virtual void setNormals(const std::vector<Vector3> &normals);
	void setTexCoords(const std::vector<Vertex2D> &texCoords);
	//! Accessor for vertices (x, y, z).
	const Vector3* const getVertices() const {return &m_vertices[0];};
	//const Vector3* const getVertices() const {return m_vertices.getRawPointer();}
	//! Accessor for normals.
	const Vector3* const getNormals() const {return &m_normals[0];}//{return m_normals.getRawPointer();}
	//! Accessor for texture coordinates.
	const Vertex2D* getTexCoords() const {return &m_texCoords[0];}
	//! Accessor for vertex count.
	size_t getVertexCount() const {return m_vertices.size();}
	
	static Vector3 computeFaceNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	Geometry(const std::string& name) : NamedObj(name){};
	virtual ~Geometry(void){};
};
}
#endif // GEOMETRY_H_
