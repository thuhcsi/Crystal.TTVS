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
 * The Original Code is Xface Core Library; Math.
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

#include <XMath/XMath.h>

namespace XMath{
float lengthSqrRayPnt3(const Ray3& ray, const Vector3& pnt)
{
	float dist;
	Vector3 d(ray.getDirection());
	Vector3 o(ray.getOrigin());
	float t0 = d.dot(pnt - o) / d.dot(d);
	if(t0 <= 0)
		dist = (pnt - o).lengthSqr();
	else
		dist = (pnt - (o + d*t0)).lengthSqr();
	
	return dist;
}

bool intersectMeshRay3(const DeformableGeometry* pMesh, const Ray3& ray, Vector3& pnt)
{
    //WUZY
	//const Vector3* pVert = pMesh->getVertices();
    const Vector3* pVert = pMesh->getDeformedVertices();
	const unsigned short* pInd = pMesh->getIndices();
	double t, u, v; // unused vars for now
	for(size_t i = 0; i < pMesh->getIndexCount(); i+=3)
	{
		Vector3 a(pVert[pInd[i]]);
		Vector3 b(pVert[pInd[i+1]]);
		Vector3 c(pVert[pInd[i+2]]);
		
		// back facing tris eliminated
		if((c-b).cross(c-a).dot(ray.getDirection()) < 0)
			continue;

		// double precision conversion
		Vector3 o = ray.getOrigin();
		Vector3 d = ray.getDirection();
		double orig[3] = {o.x, o.y, o.z};
		double dir[3] = {d.x, d.y, d.z};

		double vert0[3] = {a.x, a.y, a.z};
		double vert1[3] = {b.x, b.y, b.z};
		double vert2[3] = {c.x, c.y, c.z};
		
		// go for ray-tri test
		if(intersect_triangle(orig, dir, vert2, vert0, vert1, &t, &u, &v))
		{
			pnt = c*(1.0f - u - v) + a*u + b*v;
/*			Vector3 d(ray.getDirection());
			Vector3 o(ray.getOrigin());
			float t0 = d.dot(pnt - o) / d.dot(d);

			pnt = (o + d*t0);
*/			
			return true;
		}
	}
	return false;
}
bool intersectMeshRay3(const DeformableGeometry* pMesh, const Ray3& ray, unsigned short& index)
{
    //WUZY
	//const Vector3* pVert = pMesh->getVertices();
    const Vector3* pVert = pMesh->getDeformedVertices();
	const unsigned short* pInd = pMesh->getIndices();
	double t, u, v; // unused vars for now
	for(size_t i = 0; i < pMesh->getIndexCount(); i+=3)
	{
		Vector3 a(pVert[pInd[i]]);
		Vector3 b(pVert[pInd[i+1]]);
		Vector3 c(pVert[pInd[i+2]]);
		
		// back facing tris eliminated
		if((c-b).cross(c-a).dot(ray.getDirection()) < 0)
			continue;

		// double precision conversion
		Vector3 o = ray.getOrigin();
		Vector3 d = ray.getDirection();
		double orig[3] = {o.x, o.y, o.z};
		double dir[3] = {d.x, d.y, d.z};

		double vert0[3] = {a.x, a.y, a.z};
		double vert1[3] = {b.x, b.y, b.z};
		double vert2[3] = {c.x, c.y, c.z};
		
		// go for ray-tri test
		float dist, distmin = 100000;
		if(intersect_triangle(orig, dir, vert2, vert0, vert1, &t, &u, &v))
		{
			// calculate the minimum distanced vertex from the three vertices to the ray
			for (int k = 0; k < 3; ++k)
			{
				dist = lengthSqrRayPnt3(ray, pVert[pInd[i+k]]);
				if( dist < distmin )
				{
					distmin = dist;
					index = pInd[i + k];
				}
			}
			
			return true;
		}
	}
	return false;
}

}
