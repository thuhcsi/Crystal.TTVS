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
 * The Original Code is Xface Core Library; 3D Model File Loader.
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

#include <XEngine/ModelFileFactory.h>

#include <XEngine/ObjLoader.h>
#include <XEngine/VRML1Loader.h>
#include <XEngine/VRML97Loader.h>
#include <XEngine/MeshManager.h>
#include <fstream>
#include <iostream>

#include <algorithm>

namespace XEngine{

//! List of files in memory are stored here as a static var
std::list<MeshInfo> ModelFileFactory::s_filenames;

/*!
    Load the mesh from here.
*/
std::list<Drawable*> ModelFileFactory::loadModelFile(const std::string& filename, const std::string& path)
{
	std::list<Drawable*> drawables;
	MeshInfo info;
	IModelLoader* pLoader = 0;
	std::string ext(filename.begin() + filename.find_last_of('.') + 1, filename.end());
	if(ext == "obj")
	{
		pLoader = new OBJLoader;
	}
	else if( (ext == "wrl")	|| (ext == "WRL"))
	{
		std::string modelfile = path + filename;
		std::ifstream fp(modelfile.c_str());
		if(fp.fail())
			return drawables;

		std::string format, version;
		fp >> format >> version;
		fp.close();
		if(format != "#VRML")
			return drawables;

		if(version == "V1.0")
			pLoader = new VRML1Loader;
		else
			pLoader = new VRML97Loader;
	}
	else 
		return drawables;

	// load the model, return value stores the drawables
	drawables = pLoader->loadModel(filename, path);
	delete pLoader;

	// if the file is not loaded correctly, this list is empty
	if(drawables.empty())
		return drawables;
	
	// save the load info
	info.format = ext;
	info.file = filename;
	info.drawables = drawables;

	s_filenames.push_back(info);

	return drawables;
}

/*!
    UnLoad a single mesh file from here.
*/
MeshInfo ModelFileFactory::unloadModelFile(const std::string& filename)
{
	MeshInfo retVal;
	FILEMESHES::iterator it = s_filenames.begin();
	while (it != s_filenames.end())
	{
		if(it->file == filename)
		{
			MeshManager* pMM = MeshManager::getInstance();
			std::list<Drawable*>::iterator mesh_it = it->drawables.begin();
			while (mesh_it != it->drawables.end())
			{
				pMM->removeMesh((*mesh_it)->getMeshName());
				++mesh_it;
			}
			retVal = *it;
			s_filenames.erase(it);
			return retVal;
		}
		++it;
	}

	return retVal;
}

	
/*!
    UnLoads all the mesh files from memory.
*/
void ModelFileFactory::unloadAllFiles()
{
	MeshManager* pMM = MeshManager::getInstance();
	FILEMESHES::iterator it = s_filenames.begin();
	while (it != s_filenames.end() && !s_filenames.empty())
	{
		unloadModelFile(it->file);
		it = s_filenames.begin();
	}
	s_filenames.clear();
}

/*!
    Checks is the file is already loaded or not.
    \return 0 pointer if it is not loaded, pointer to MeshInfo already loaded otherwise.
*/
const MeshInfo* ModelFileFactory::isFileLoaded(const std::string& filename)
{
	FILEMESHES::const_iterator it = s_filenames.begin();
	while (it != s_filenames.end())
	{
		if(it->file == filename)
			return &(*it);
		++it;
	}
	return 0;
}
}// namespace XFace
