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
 * The Original Code is Xface Core Library; Mesh Data.
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

#include <XEngine/Geometry.h>

#include <algorithm>

namespace XEngine{

/*!
	We do NOT override the = operator but implement a new method, because in some cases,
	we only want to copy the data, not the name of the Geometry.
*/
Geometry& Geometry::copyFrom(const Geometry& rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	m_vertices	= rhs.m_vertices;
	m_normals	= rhs.m_normals;
	m_texCoords = rhs.m_texCoords;

	return *this;
}


/*!
	Vertices are set through here. 
*/
void Geometry::setVertices(const std::vector<Vector3> &vertices)
{
	m_vertices = vertices;
}

/*!
	\todo check back here
*/
void Geometry::setVertices(const Vector3* pVert, unsigned int size)
{
	m_vertices.assign(pVert, pVert + size);
}

/*!
	Vertex normals are set through here. 
*/
void Geometry::setNormals(const std::vector<Vector3> &normals)
{
	m_normals = normals;
}

/*!
	Texture coordinates are set through here. 
*/
void Geometry::setTexCoords(const std::vector<Vertex2D> &texCoords)
{
	m_texCoords = texCoords;
}

/*!
	Computes the facenormal by crossing two vectors defined by (p2-p1) and (p3-p1).
	The parameters are assumed to be passed in counter-clockwise order.
*/
Vector3 Geometry::computeFaceNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 u, v;
	u.x = p2.x - p1.x; u.y = p2.y - p1.y; u.z = p2.z - p1.z;
	v.x = p3.x - p1.x; v.y = p3.y - p1.y; v.z = p3.z - p1.z;

	Vector3 facenormal = u.unitCross(v);

	return facenormal;
}

}  // namespace XFace