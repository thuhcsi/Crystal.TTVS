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

#include <XEngine/VRML1Loader.h>
#include <XEngine/Buffer.h>

#include <string>
#include <strstream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <XEngine/MeshManager.h>
#include <XEngine/TextureManager.h>

namespace XEngine{

VRML1Loader::VRML1Loader(void) : m_pMesh(0)
{
}

VRML1Loader::~VRML1Loader(void)
{
}

std::list<Drawable*> VRML1Loader::loadModel(const std::string &filename, const std::string& dir)
{
	std::list<Drawable*> drawables;
	std::list<std::string> meshNames;
	std::string modelfile = dir + filename;
	std::ifstream fp(modelfile.c_str());
	if(fp.fail())
		return drawables;
	std::string linestring, key_str, meshname;
	int cnt = 0;
	Drawable* pDrawable(0);
    while (fp >> key_str)
	{
		if (key_str == "Coordinate3")
		{
			// create a new mesh
			if(m_pMesh)
				createMeshNode();
			std::strstream meshname_str;
			meshname_str << filename << "_" << cnt++ << '\0';
			meshname = meshname_str.str();
			meshNames.push_back(meshname);
			m_pMesh = new DeformableGeometry(meshname);
			
			pDrawable = new Drawable;
			pDrawable->setMeshName(meshname);
			pDrawable->setName(meshname);
			drawables.push_back(pDrawable);
		
			onCoordinate3(fp);
//			std::cout << key_str << " " << meshname << std::endl;
		}
		else if (key_str == "TextureCoordinate2")
		{
			onTexCoordinate2(fp);
//			std::cout << key_str << std::endl;
		}
		else if (key_str == "IndexedFaceSet")
		{
			onIndexedFaceSet(fp);
//			std::cout << key_str << std::endl;
		}
		else if (key_str == "Texture2")
		{
			std::string texName("");
			while (texName != "filename")
				fp >> texName;
	
			fp >> texName;
			std::string texfile = texName.substr(1, texName.size() - 2);
			//TextureManager::getInstance()->load(dir + texfile, texfile/*meshname*/);
			//pDrawable->setTexName(texfile);
//			std::cout << key_str << " " << texfile << std::endl;

            // modified by John (john.zywu@gmail.com)
            // use "filename" directly as "texname" for later "rebindTextures()" use
            TextureManager::getInstance()->load(dir + texfile, dir + texfile);
            pDrawable->setTexName(dir + texfile);
		}
	}
	
	createMeshNode();
	return drawables;
}

void VRML1Loader::onCoordinate3(std::istream& pStream)
{
	std::string dummy(""), linestring;
	//while (dummy != "point")
	while ( dummy != "[" )
		pStream >> dummy;

	Vector3 vert;
	while ( dummy != "]" )
	{
		pStream >> vert.x >> vert.y >> vert.z >> dummy;
		m_Vertices.push_back(vert);
		// sloppy check for fast implementation, better than nothing
		std::getline(pStream, linestring); // if there is a comment at the end of the line
	/*	if(dummy != ",")
			std::cout << vert.x << ',' << vert.y << vert.z << std::endl;
	*/
	}

//	std::cout << "# of vertices" << m_Vertices.size() << std::endl;
}

void VRML1Loader::onTexCoordinate2(std::istream& pStream)
{
	std::string dummy(""), linestring;
	//while (dummy != "point")
	while ( dummy != "[" )
		pStream >> dummy;

	Vertex2D vert;	
	while ( dummy != "]" )
	{
		pStream >> vert.x >> vert.y >> dummy;
		std::getline(pStream, linestring); // if there is a comment at the end of the line
		m_TexCoords.push_back(vert);
	/*	if(dummy != ",")
			std::cout << vert.x << ',' << vert.y << vert.z << std::endl;
	*/
	}
//	std::cout << "# of tex coords" << m_TexCoords.size() << std::endl;
}

void VRML1Loader::onIndexedFaceSet(std::istream& pStream)
{
	std::string dummy("");
	// get indices to vertices
	while (dummy != "coordIndex")
		pStream >> dummy;
	onIndex(pStream, m_Indices);

	// get indices to texture coords
	while ( (dummy != "textureCoordIndex") && (dummy != "}") )
		pStream >> dummy;
	if (dummy == "textureCoordIndex")
		onIndex(pStream, m_IndicesTex);

	// get indices to normals (not used but recomputed in fact)
	while ( (dummy != "normalIndex") && (dummy != "}") )
		pStream >> dummy;
	if (dummy == "normalIndex")
		onIndex(pStream, m_IndicesNormal);
}

void VRML1Loader::onIndex(std::istream& pStream, std::vector< std::vector<unsigned short> >& store)
{
	std::string dummy("");
	while ( dummy != "[" )
		pStream >> dummy;

	long ind(0);
	std::vector<unsigned short> polyInd;
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
//	std::cout << "# of polys: " << store.size() << std::endl;
}

/*!
	\note direct copy/paste from OBJLoader
*/
void VRML1Loader::createMeshNode(void)
{
	std::vector<Vertex2D> d_TexCoords = m_TexCoords;

	// making texture coords indices exactly the same as vertex indices,
	// unfortunately, in VRML1 format, this isn't a rule, so we enforce it here
	if(!m_TexCoords.empty())
	{
		std::vector< std::vector<unsigned short> >::iterator it, it_t;
		std::vector<unsigned short>::iterator it2, it2_t;
		it_t = m_IndicesTex.begin();
		for(it = m_Indices.begin(); it != m_Indices.end(); ++it, ++it_t)
		{
			it2_t = it_t->begin();
			for(it2 = it->begin(); it2 != it->end(); ++it2, ++it2_t)
			{
				m_TexCoords[*it2] = d_TexCoords[*it2_t];
			}
		}
	}

	// Indices are attached as IndexedFaceSet objects as children to the mesh node
	m_pMesh->setVertices(m_Vertices);
	m_pMesh->setIndices(m_Indices);
	if(!m_Normals.empty()) // no normals, always empty
		m_pMesh->setNormals(m_Normals);
	else
		m_pMesh->computeVertexNormals();
	if(!m_TexCoords.empty())
		m_pMesh->setTexCoords(m_TexCoords);
	
	MeshManager::getInstance()->registerMesh(m_pMesh);

	m_Vertices.clear();
	m_TexCoords.clear();
	m_Normals.clear();
	m_Indices.clear();
	m_IndicesTex.clear();
	m_IndicesNormal.clear();
}
} // namespace XFace