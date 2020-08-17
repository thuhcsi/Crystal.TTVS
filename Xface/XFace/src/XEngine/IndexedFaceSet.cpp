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
 * The Original Code is Xface Core Library; IndexedFaceSet Data.
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

#include <XEngine/IndexedFaceSet.h>
#include <algorithm>
#include <iterator>

namespace XEngine{

IndexedFaceSet& IndexedFaceSet::copyFrom(const IndexedFaceSet& rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	Geometry::copyFrom(rhs);
	// copy data
	m_indices = rhs.m_indices;
	
	return *this;
}

/*!
	Indices are set here. If a face is not a triangle it is divided into triangles 
	so that the final geometry contains only triangles (which is a Good Thing!).
	\param indices A vector of vector of unsigned short values representing indices to 
	vertices. Inner vector represents each face (polygon) and the outer vector is
	the whole colection of faces.
*/
void IndexedFaceSet::setIndices(const std::vector< std::vector<unsigned short> > &indices)
{
	std::vector<unsigned short> tri_indices;

	std::vector< std::vector<unsigned short> >::const_iterator it;
	std::vector<unsigned short>::const_iterator it2;
	for (it = indices.begin(); it != indices.end(); ++it)
	{
		// triangulazing the polygons here..
		it2 = it->begin() + 2;
		do
		{
			tri_indices.push_back(*it->begin());
			tri_indices.push_back(*(it2 - 1));
			tri_indices.push_back(*it2);
			++it2;
		}while (it2 != it->end());
	}

	m_indices = tri_indices;

	//m_indices.setData(tri_indices);
}

void IndexedFaceSet::setIndices(const std::vector<unsigned short> &indices)
{
	m_indices = indices;
	//m_indices.setData(indices);
}

/*!
	Computes the vertex normals if the mesh has an indexed structure.
	For computing the vertex normals, simply the face normals for each polygon 
	is found by crossing two edges of the polygon, then corresponding vertices' 
	normals are summed up with the face normal. At last, all the vertex normals 
	are normalized. Might be implemented faster, but since this step is done 
	only once in the initialization of the mesh, it is not a bottleneck. 
	However, for dynamically changing geometry, some optimizations can be 
	considered.
*/
void IndexedFaceSet::computeVertexNormals()
{
	size_t nVertices = m_vertices.size();
	std::vector<Vector3> normals;
	// Reset normals to zero
	std::fill_n(std::back_inserter(normals), nVertices, Vector3(0, 0, 0));
	
	// We are working ONLY with triangles
	size_t nSides = 3;
	size_t i;
	
	// Compute face normal for each face and sum up for the corresponding vertices
	Vector3 facenormal;
	for (i = 0; i < m_indices.size() - nSides; i+= nSides)
	{
		facenormal = computeFaceNormal(m_vertices[m_indices[i]], 
										m_vertices[m_indices[i + 1]], 
										m_vertices[m_indices[i + 2]]);

		normals[m_indices[i]]		+= facenormal;
		normals[m_indices[i + 1]]	+= facenormal;
		normals[m_indices[i + 2]]	+= facenormal;
	}

	// Normalize the normals
	for(i = 0; i < nVertices; ++i)
		normals[i].normalize();

	setNormals(normals);
//	m_normals.assign(pNormals, pNormals + nVertices);
}

}  // namespace XFace