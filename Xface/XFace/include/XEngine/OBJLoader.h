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
	\file	OBJLoader.h
	\brief	Wavefront OBJ loader.
*/

#ifndef OBJLOADER_H_
#define OBJLOADER_H_


#include "ISceneLoader.h"
#include "VertexStruct.h"
#include "DeformableGeometry.h"

#include <vector>
#include <queue>
#include <istream>

namespace XEngine{
	
/*!
	\brief Loads Wavefront OBJ files for the scene graph.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   April 2003
			
	Loads Wavefront OBJ files, creates the scene graph nodes, and attaches them.
	Uses MeshManager class to register the meshes loaded. 
*/
class OBJLoader : public IModelLoader
{
	std::queue<std::string> m_TexFiles;
	std::list<std::string> m_meshNames;
	//unsigned int m_IndexOffset;
	void decodeFace( const std::string& s );
	void onMaterial(std::istream& pStream, const std::string& path);
	void onGroup(std::istream& pStream);
	void onFace(std::istream& pStream);
	void onVertex(std::istream& pStream);
	void onTexCoord(std::istream& pStream);
	void onNormal(std::istream& pStream);

	void createMeshNode(void);
	DeformableGeometry* m_pMesh;

	std::string m_path;
	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vertex2D> m_TexCoords;
	std::vector<unsigned short> m_Face;
	std::vector<unsigned short> m_FaceTex;
	std::vector<unsigned short> m_FaceNormal;
	std::vector< std::vector<unsigned short> > m_Indices;
	std::vector< std::vector<unsigned short> > m_IndicesTex;
	std::vector< std::vector<unsigned short> > m_IndicesNormal;
public:
	std::list<Drawable*> loadModel(const std::string &filename, const std::string& dir="./");
	OBJLoader(void);
	~OBJLoader(void);
};
} // namespace XFace
#endif //OBJLOADER_H_
