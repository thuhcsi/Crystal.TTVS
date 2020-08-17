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
	\file	MeshManager.h
	\brief	Meshes are stored and managed here.
*/
#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

#include <map>
#include <string>
#include "DeformableGeometry.h"

namespace XEngine{
/*!
	\brief Meshes are stored and managed here.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003

	Meshes (IndexedFaceSet instances) are stored and managed through here. 
	All the meshes are stored in a hash map (currently) with a string representing 
	the name as the key of the hash. MeshManager is a singleton class.

	Here is a typical usage in ISceneLoader;
	\code
	IndexedFaceSet* m_pMesh = new IndexedFaceSet; // Create an instance (IndexedFaceSet IS-A IMesh)
	... // Initialization, etc..
	
	// MeshManager is a singleton class.
	MeshManager::getInstance()->registerMesh(m_pMesh); // Add the mesh to MeshManager
	\endcode

	And here is how we access the loaded Mesh (from RendererGL);
	\code
	Drawable* pDrawable; // Assume it is initialized correctly.
	...
	std::string MeshName = pDrawable->getMeshName();
	const DeformableGeometry* pMesh = MeshManager::getInstance()->getMesh(MeshName);
	\endcode
	
*/
class MeshManager
{
	typedef std::map<std::string, DeformableGeometry* > MESHSTORAGE;

	MESHSTORAGE m_Storage;
	static MeshManager* m_pThis;
	MeshManager(void);
	~MeshManager(void);
public:
	static void releaseInstance();
	static MeshManager* getInstance();
	void destroyAll();
	bool removeMesh(const std::string& name);
	void registerMesh(DeformableGeometry* pMesh);
	DeformableGeometry* const getMesh(const std::string& name) const;
};
}

#endif //MESHMANAGER_H_
