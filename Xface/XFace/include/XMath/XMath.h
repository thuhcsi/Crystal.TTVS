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
 * The Original Code is Xface Core Library; Math
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
	\file	XMath.h
	\brief	Intersection related functions
*/

#ifndef XMATH_H_
#define XMATH_H_

#include "Ray3.h"
#include "Vector3.h"
//#include <XEngine/IndexedFaceSet.h>
#include <XEngine/DeformableGeometry.h>

//WUZY
//using XEngine::IndexedFaceSet;
using XEngine::DeformableGeometry;
namespace XMath{
	float lengthSqrRayPnt3(const Ray3& ray, const Vector3& pnt);
    //WUZY
	//bool intersectMeshRay3(const IndexedFaceSet* pMesh, const Ray3& ray, unsigned short& index);
	//bool intersectMeshRay3(const IndexedFaceSet* pMesh, const Ray3& ray, Vector3& pnt);
    bool intersectMeshRay3(const DeformableGeometry* pMesh, const Ray3& ray, unsigned short& index);
    bool intersectMeshRay3(const DeformableGeometry* pMesh, const Ray3& ray, Vector3& pnt);

	// by Tomas Moller, May 2000, see raytri.cpp for more info
	int intersect_triangle(double orig[3], double dir[3],
		       double vert0[3], double vert1[3], double vert2[3],
		       double *t, double *u, double *v);
	int intersect_triangle1(double orig[3], double dir[3],
		       double vert0[3], double vert1[3], double vert2[3],
		       double *t, double *u, double *v);
	int intersect_triangle2(double orig[3], double dir[3],
		       double vert0[3], double vert1[3], double vert2[3],
		       double *t, double *u, double *v);
	int intersect_triangle3(double orig[3], double dir[3],
		       double vert0[3], double vert1[3], double vert2[3],
		       double *t, double *u, double *v);

}

#endif // XMATH_H_