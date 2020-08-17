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
	\file	Ray3.h
	\brief	Ray representation in 3D
*/

#ifndef RAY3_H_
#define RAY3_H_

#include "Vector3.h"

namespace XMath{

/*!
	\brief		3D ray representation
	\ingroup	XMath
	\author		Koray Balci
	\version	1.0
	\date		April 2003

	Class that represents a ray in 3D, no operations (methods) are involved, only data abstraction for
	the time being.
*/
class Ray3
{
	Vector3 m_origin;
	Vector3 m_direction;
public:
	//@{
	//! Accessor
	const Vector3& getOrigin() const {return m_origin;};
	const Vector3& getDirection() const {return m_direction;};
	void setOrigin(const Vector3& org){m_origin = org;};
	void setDirection(const Vector3& dir){m_direction = dir;};
	//@}
	Ray3(const Vector3& org, const Vector3& dir) : m_origin(org), m_direction(dir){};
	Ray3(void){};
};
}
#endif
