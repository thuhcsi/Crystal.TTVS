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
 * The Original Code is Xface Core Library; Model Loader
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
	\file	VRML1Loader.h
	\brief	VRML1 Loader
*/
#ifndef VRML1LOADER_H_
#define VRML1LOADER_H_
#include "ISceneLoader.h"

#include "DeformableGeometry.h"

namespace XEngine{
	
/*!
	\brief VRML1 Loader
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Loads 3D VRML1 files, creates the meshes and registers them through MeshManager.
*/
class VRML1Loader :	public IModelLoader
{
	std::vector<Vector3> m_Vertices;
	std::vector<Vertex2D> m_TexCoords;
	std::vector<Vector3> m_Normals;
	std::vector< std::vector<unsigned short> > m_Indices;
	std::vector< std::vector<unsigned short> > m_IndicesTex;
	std::vector< std::vector<unsigned short> > m_IndicesNormal;
	DeformableGeometry* m_pMesh;

	void createMeshNode(void);
	void onIndex(std::istream& pStream, std::vector< std::vector<unsigned short> >& store);
	void onCoordinate3(std::istream& pStream);
	void onTexCoordinate2(std::istream& pStream);
	void onIndexedFaceSet(std::istream& pStream);

public:
	//! Overriden base class method for loading scene
	std::list<Drawable*> loadModel(const std::string &filename, const std::string& dir ="./" );
	VRML1Loader(void);
	~VRML1Loader(void);
};
} // namespace XFace

#endif // VRML1LOADER_H_
