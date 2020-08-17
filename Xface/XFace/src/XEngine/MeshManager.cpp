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
 * The Original Code is Xface Core Library; Mesh Manager.
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

#include <XEngine/MeshManager.h>

namespace XEngine{

MeshManager* MeshManager::m_pThis = 0;
MeshManager::MeshManager(void)
{
}

MeshManager::~MeshManager(void)
{
	destroyAll();
}

/*!
	Singleton class accessor for the one and only instance of this class.
*/
MeshManager* MeshManager::getInstance()
{
	if (!m_pThis)
		m_pThis = new MeshManager();
	return m_pThis;
}

/*!
	Releases the one and only instance.
*/
void MeshManager::releaseInstance()
{
	delete m_pThis;
	m_pThis = 0;
}

/*!
	Registers a mesh object to the storage.
*/
void MeshManager::registerMesh(DeformableGeometry* pMesh)
{
	m_Storage.insert(std::make_pair(pMesh->getName().c_str(), pMesh));
}


bool MeshManager::removeMesh(const std::string& name)
{
	MESHSTORAGE::iterator it = m_Storage.find(name.c_str());
	if(it == m_Storage.end())
		return false;
	
	delete it->second;
	m_Storage.erase(it);
	
	return true;
}
/*!
	Retrieves a mesh with the name passed. 
	\return 0 if unsucessful, a valid pointer to IMesh if successful.
*/
DeformableGeometry* const MeshManager::getMesh(const std::string& name) const
{
	MESHSTORAGE::const_iterator it = m_Storage.find(name.c_str());
	if(it == m_Storage.end())
		return 0;

	return it->second;
}


/*!
	Destroys all of the meshes in the storage.
*/
void MeshManager::destroyAll()
{
	MESHSTORAGE::iterator it = m_Storage.begin();
	while (it != m_Storage.end())
	{
		delete it->second;
		++it;
	}
	m_Storage.clear();
}

}  // namespace XFace