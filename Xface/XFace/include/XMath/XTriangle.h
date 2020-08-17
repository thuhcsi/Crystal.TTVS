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
	\file	XTriangle.h
	\brief	Triangle representation
*/

#ifndef XTRIANGLE_H_
#define XTRIANGLE_H_


#include "Vector3.h"

namespace XMath{

/*!
	\brief		3D triangle representation
	\ingroup	XMath
	\author		Koray Balci
	\version	1.0
	\date		April 2003

	Class that represents a triangle in 3D, no operations (methods) are involved, only data abstraction for
	the time being.
*/
class Triangle3
{
	Vector3 m_origin, m_edge0, m_edge1;
public:
	Triangle3(void){}
	//! Creates a triangle from three vectors (vectors represent 3d points!! not edges)
	Triangle3(const Vector3& p0, const Vector3& p1, const Vector3& p2) 
		: m_origin(p0), m_edge0(p1-p0), m_edge1(p2-p0){}
	//@{
	//! Accessor
	const Vector3& getOrigin() const {return m_origin;}
	const Vector3& getEdge0() const {return m_edge0;}
	const Vector3& getEdge1() const {return m_edge1;}

	void setOrigin(const Vector3& org)	{m_origin = org;}
	void setEdge0(const Vector3& ed0)	{m_edge0 = ed0;}
	void setEdge1(const Vector3& ed1)	{m_edge1 = ed1;}
	//@}
};

}
#endif // XTRIANGLE_H_