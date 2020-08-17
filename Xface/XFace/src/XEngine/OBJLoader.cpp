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
 * The Original Code is Xface Core Library; OBJ 3D File Loader.
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

#include <XEngine/OBJLoader.h>

#include <XEngine/MeshManager.h>
#include <XEngine/TextureManager.h>
#include <string>
#include <strstream>
#include <fstream>
#include <sstream>
#include <iterator>

namespace XEngine{

OBJLoader::OBJLoader(void) : m_pMesh(0)
{
}

OBJLoader::~OBJLoader(void)
{
}

void OBJLoader::onVertex(std::istream& pStream)
{
	Vector3 tempVert;
	pStream >> tempVert.x >> tempVert.y >> tempVert.z;
	m_Vertices.push_back(tempVert);
}

void OBJLoader::onNormal(std::istream& pStream)
{
	Vector3 tempVert;
	pStream >> tempVert.x >> tempVert.y >> tempVert.z;
	m_Normals.push_back(tempVert);
}

void OBJLoader::onTexCoord(std::istream& pStream)
{
	Vertex2D tempVert;
	pStream >> tempVert.s >> tempVert.t;
	m_TexCoords.push_back(tempVert);
}

void OBJLoader::onMaterial(std::istream& pStream, const std::string& path)
{
	std::string name;
	pStream >> name;

	std::ifstream fp((path+name).c_str());
	if(fp.fail())
		return;

	std::list<std::string> texNames;
	std::string texfile, key_str, emptyline;
	while (fp >> key_str)
	{
		if (key_str == "map_Kd")
		{
			fp >> texfile;
			m_TexFiles.push(texfile);
		}
		else // skip comments & not used parts of file
		{
			std::getline(fp, emptyline);
		}
		key_str.clear();
	}
}

void OBJLoader::onGroup(std::istream& pStream)
{
	if(m_pMesh)
		createMeshNode();

	std::string name;
	pStream >> name;
	
	m_pMesh = new DeformableGeometry(name);
}

void OBJLoader::decodeFace( const std::string& s )
{
	std::stringstream face(s);
	int vert, tex, norm, ind = 0;
	char delim = '/';
	face >> vert >> delim;
	m_Face.push_back(vert - 1);
	while(face.good())
	{
		if(ind == 0)
		{
			face >> tex >> delim;
			m_FaceTex.push_back(tex - 1);
		}
		else
		{
			face >> norm >> delim;
			m_FaceNormal.push_back(norm - 1);
		}
		ind++;
	}
//	OutputDebugString("\n");
}

void OBJLoader::onFace(std::istream& pStream)
{
//	OutputDebugString("New Face\n");
	std::string line;
	std::getline(pStream, line);
	std::stringstream line_stream(line);

	std::istream_iterator<std::string> it(line_stream), end;
	while (it != end)
	{
		decodeFace(*it);	
		++it;
	}

	// add to storage
	m_Indices.push_back(m_Face);
	m_Face.clear();

	if(!m_FaceNormal.empty())
	{
		m_IndicesNormal.push_back(m_FaceNormal);
		m_FaceNormal.clear();
	}

	if(!m_FaceTex.empty())
	{
		m_IndicesTex.push_back(m_FaceTex);
		m_FaceTex.clear();
	}
}

void OBJLoader::createMeshNode(void)
{
	std::vector<Vertex2D> d_TexCoords = m_TexCoords;

	// making texture coords indices exactly the same as vertex indices,
	// unfortunately, in OBJ format, this isn't a rule, so we enforce it here
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
	if(!m_Normals.empty())
		m_pMesh->setNormals(m_Normals);
	else
		m_pMesh->computeVertexNormals();
	if(!m_TexCoords.empty())
		m_pMesh->setTexCoords(m_TexCoords);
	
	MeshManager::getInstance()->registerMesh(m_pMesh);
	m_meshNames.push_back(m_pMesh->getName());

	// create the corresponding texture
	if(!m_TexFiles.empty())
	{
		TextureManager::getInstance()->load((m_path + m_TexFiles.front()), m_pMesh->getName());
		m_TexFiles.pop();
	}

/*
	m_Vertices.clear();
	m_Normals.clear();
	m_TexCoords.clear();
	m_IndexOffset += m_Indices.size();
	m_Indices.clear();
*/
}



//http://www.codeguru.com/forum/showthread.php?threadid=247571
std::list<Drawable*> OBJLoader::loadModel(const std::string &filename, const std::string& dir)
{
	m_path = dir;
	std::list<Drawable*> drawables;
	m_meshNames.clear();
	std::string modelfile = dir + filename;
	std::ifstream fp(modelfile.c_str());
	if(fp.fail())
		return drawables;

	std::string linestring, key_str;
	while (fp >> key_str)
	{	if (key_str == "v")
			onVertex(fp);
		else if(key_str == "vt")
			onTexCoord(fp);
		else if(key_str == "vn")
			onNormal(fp);
		else if(key_str == "f")
			onFace(fp);
		else if(key_str == "g")
			onGroup(fp);
		else if(key_str == "mtllib")
			onMaterial(fp, dir);
		else // skip comments & not used parts of OBJ structure
		{
			std::getline(fp, linestring);
		}
		key_str.clear();
	}
	createMeshNode();

	Drawable* pDrawable(0);
	std::list<std::string>::iterator it = m_meshNames.begin();
	while(it != m_meshNames.end())
	{
		pDrawable = new Drawable;
		pDrawable->setName(*it);
		pDrawable->setMeshName(*it);
		pDrawable->setTexName(*it);
		drawables.push_back(pDrawable);
		++it;
	}
	return drawables;
}

}  // namespace XFace