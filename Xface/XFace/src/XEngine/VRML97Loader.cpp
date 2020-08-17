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
 * The Original Code is Xface Core Library; VRML1 Loader.
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

#include <XEngine/VRML97Loader.h>

#include <string>
#include <strstream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#ifdef WIN32
#include <direct.h> // not sure if os independent (not think so)
#endif

#include <XEngine/MeshManager.h>
#include <XEngine/TextureManager.h>
#include <XEngine/Transform.h>

namespace XEngine{

std::list<Drawable*> VRML97Loader::loadModel(const std::string &filename, const std::string& dir)
{
	Drawable* pDrawable = 0;
	Transform* pTrans = 0;
	std::list<Drawable*> drawables;
	std::list<std::string> meshNames;
	std::string modelfile = dir + filename;
	std::ifstream fp(modelfile.c_str());
	if(fp.fail())
		return drawables;
	std::string linestring, key_str, meshname, texname;
	int cnt = 0;
	while (fp >> key_str)
	{
		if (key_str == "Transform")
		{
			// delete pTrans;
			pTrans = new Transform;
			while (key_str != "children")
			{
				fp >> key_str;
				if(key_str == "translation")
				{
					Vector3 tr(0, 0, 0);
					fp >> tr.x >> tr.y >> tr.z;
					pTrans->setTranslation(tr);
				}
				else if(key_str == "rotation")
				{
					Vector3 rot(0, 1, 0);
					float ang;
					fp >> rot.x >> rot.y >> rot.z >> ang;
					ang = 180.0f * ang / 3.1415926535f;
					pTrans->setRotation(AxisAngle(rot, ang));
				}
				else if(key_str == "scale")
				{
					Vector3 sc(1, 1, 1);
					fp >> sc.x >> sc.y >> sc.z;
					pTrans->setScale(sc);
				}
			}
		}
		else if (key_str == "Shape")
		{
			pDrawable = new Drawable;
			drawables.push_back(pDrawable);
			if(pTrans)
				pDrawable->setTransform(*pTrans);
		}
		else if (key_str == "texture")
		{
			fp >> key_str;
			if(key_str == "ImageTexture")
			{
				while(key_str != "url")
					fp >> key_str;
				fp >> texname;

				// the filename is stored in double quotes, let's get rid of them
				texname = texname.substr(1, texname.size() - 2);
				//TextureManager::getInstance()->load(dir + texname, texname);
				//pDrawable->setTexName(texname);
				std::cout << texname << "\n";

                // modified by John (john.zywu@gmail.com)
                // use "filename" directly as "texname" for later "rebindTextures()" use
                TextureManager::getInstance()->load(dir + texname, dir + texname);
                pDrawable->setTexName(dir + texname);
			}
		}
		else if (key_str == "geometry")
		{
			// get/create the name for the mesh
			std::string label, dummy;
			fp >> dummy;
			if(dummy == "DEF")
			{
				fp >> label >> dummy;
			}
			else if(dummy == "IndexedFaceSet")
			{
				std::strstream meshname_str;
				meshname_str << filename << "_" << cnt++ << '\0';
				label = meshname_str.str();
			}
				
			meshNames.push_back(label);
			std::cout << key_str << " " << label << std::endl;
			
			if(dummy == "IndexedFaceSet") // dummy should have IndexedFaceSet no matter what
			{
				// process the indexed face set
				onIndexedFaceSet(fp, label);
				pDrawable->setMeshName(label);
				pDrawable->setName(label);
			}
		}
	}

	return drawables;
}

/*!
	\todo Normals (if any) are not processed
*/
IndexedFaceSet* VRML97Loader::onIndexedFaceSet(std::istream& pStream, const std::string& label) const
{
	std::string dummy("");
	pStream >> dummy;
	short nBlock = 0;
	if(dummy == "{")
		nBlock++;
	else 
		return 0; // sth wrong!

	std::vector<Vector3> vertices;
	std::vector<Vertex2D> texCoords;
	std::vector<Vector3> normals;
	std::vector< std::vector<unsigned short> > indices, indicesTex, indicesNorm;

	while (nBlock > 0)
	{
		pStream >> dummy;
		if(dummy == "{")
			nBlock++;
		else if (dummy == "}")
			nBlock--;
		else if(dummy == "coord")
			onCoord(pStream, vertices);
		else if(dummy == "texCoord")
			onTexCoord(pStream, texCoords);
		else if(dummy == "normal")
			onNormal(pStream, normals);
		else if(dummy == "coordIndex")
		{
			onIndex(pStream, indices);
//			std::cout << "coordIndex: " << indices.size() << std::endl;	
		}
		else if(dummy == "texCoordIndex")
		{
			onIndex(pStream, indicesTex);
//			std::cout << "texCoordIndex: " << indicesTex.size() << std::endl;	
		}
		else if(dummy == "normalIndex")
		{
			onIndex(pStream, indicesNorm, indices.size());
		}
	}

	DeformableGeometry* pMesh = new DeformableGeometry(label);

	std::vector<Vertex2D> d_TexCoords = texCoords;

	// making texture coords indices exactly the same as vertex indices,
	// unfortunately, in VRML97 format, this isn't a rule, so we enforce it here
	if(!texCoords.empty())
	{
		std::vector< std::vector<unsigned short> >::iterator it, it_t;
		std::vector<unsigned short>::iterator it2, it2_t;
		it_t = indicesTex.begin();
		for(it = indices.begin(); it != indices.end(); ++it, ++it_t)
		{
			it2_t = it_t->begin();
			for(it2 = it->begin(); it2 != it->end(); ++it2, ++it2_t)
			{
				texCoords[*it2] = d_TexCoords[*it2_t];
			}
		}
	}

	// Indices are attached as IndexedFaceSet objects as children to the mesh node
	pMesh->setVertices(vertices);
	pMesh->setIndices(indices);
	if(!normals.empty()) // no normals, always empty
		pMesh->setNormals(normals);
	else
		pMesh->computeVertexNormals();
	if(!texCoords.empty())
		pMesh->setTexCoords(texCoords);
	
	MeshManager::getInstance()->registerMesh(pMesh);
	return pMesh;
}

void VRML97Loader::onCoord(std::istream& pStream, std::vector<Vector3>& vertices) const
{
	std::string dummy("");
	while(dummy != "point")
		pStream >> dummy;

	while(dummy != "[")
		pStream >> dummy;

	Vector3 coord3;
	while(dummy != "]")
	{
		pStream >> coord3.x >> coord3.y >> coord3.z;
		vertices.push_back(coord3);
		pStream >> dummy; // should be comma (,) or end (])
	}
	while(dummy != "}")
		pStream >> dummy; // should be (})
//	std::cout << "num coord: " << vertices.size() << std::endl;
}

void VRML97Loader::onTexCoord(std::istream& pStream, std::vector<Vertex2D>& texCoords) const
{
	std::string dummy("");
	while(dummy != "point")
		pStream >> dummy;

	while(dummy != "[")
		pStream >> dummy;

	Vertex2D coord2;
	while(dummy != "]")
	{
		pStream >> coord2.x >> coord2.y;
		texCoords.push_back(coord2);
		pStream >> dummy; // should be comma (,) or end (])
	}
	
	while(dummy != "}")
		pStream >> dummy; // should be (})
//	std::cout << "num texcoord: " << texCoords.size() << std::endl;
}

void VRML97Loader::onNormal(std::istream& pStream, std::vector<Vector3>& normals) const
{
	std::string dummy("");
	while(dummy != "vector")
		pStream >> dummy;

	while(dummy != "[")
		pStream >> dummy;
	pStream >> dummy;

	Vector3 coord3;
	while(dummy != "]")
	{
		std::stringstream strStream(dummy);
		strStream >> coord3.x;
		pStream >> coord3.y >> coord3.z;
		normals.push_back(coord3);
		pStream >> dummy; // should be comma (,)
		pStream >> dummy; // should be end (]) or number
	}
	while(dummy != "}")
		pStream >> dummy; // should be (})
	//	std::cout << "num normals: " << normals.size() << std::endl;
}

void VRML97Loader::onIndex(std::istream& pStream, std::vector< std::vector<unsigned short> >& store, size_t number) const
{
	std::string dummy("");
	while ( dummy != "[" )
		pStream >> dummy;

	long ind(0);
	std::vector<unsigned short> polyInd;
	if (number != 0)
	{
		for (size_t i=0; i<number; ++i)
		{
			while (pStream >> ind >> dummy)
			{			
				if(ind == -1)
					break;
				polyInd.push_back((unsigned short)ind);
			}
			store.push_back(polyInd);
			polyInd.clear();
		}
		while (dummy != "]")
			pStream >> dummy;
		return;
	}
	while ( dummy != "]" )
	{
		while (pStream >> ind >> dummy)
		{			
			if(ind == -1)
				break;
			polyInd.push_back((unsigned short)ind);
		}
		store.push_back(polyInd);
		polyInd.clear();
	}
}
} // namespace XFace
