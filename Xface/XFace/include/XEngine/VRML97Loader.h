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
	\file VRML97Loader.h
	\brief Loads VRML97 files
*/
#ifndef VRML97LOADER_H_
#define VRML97LOADER_H_

#include "ISceneLoader.h"
#include "IndexedFaceSet.h"


namespace XEngine{
	
/*!
	\brief Loads VRML97 files
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   April 2004
			
	Loads VRML97 files, creates the meshes and registers them through MeshManager.
	Code that does the job, saves the day, but not safe, lots of pitfalls ignored, 
	huge possibility of crash and room for improvement.
	
	\todo Not finished yet, need to implement full scene graph struct for proper use.
*/
class VRML97Loader : public IModelLoader
{
	IndexedFaceSet* onIndexedFaceSet(std::istream& pStream, const std::string& label) const;
	void onCoord(std::istream& pStream, std::vector<Vector3>& vertices) const;
	void onTexCoord(std::istream& pStream, std::vector<Vertex2D>& texCoords) const;
	void onNormal(std::istream& pStream, std::vector<Vector3>& normals) const;
	void onIndex(std::istream& pStream, std::vector< std::vector<unsigned short> >& store, size_t number=0) const;
public:
	//! Overriden base class method for loading scene
	std::list<Drawable*> loadModel(const std::string &filename, const std::string& dir ="./" );
	VRML97Loader(void){};
	~VRML97Loader(void){};
};
} // namespace XFace
#endif //VRML97LOADER_H_
