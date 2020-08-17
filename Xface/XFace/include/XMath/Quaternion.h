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
 * The Original Code is XFace::XMath
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */


/*! 
	\file		XQuaternion.h
	\brief		Quaternion class
*/

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Matrix4.h"
#include "AxisAngle.h"

namespace XMath{
/*!
	\brief		Quaternion class
	\author		Koray Balci
	\version	1.0
	\date		May 2002

	Quaternion class for robust 3D operations (rotation especially).
	\note No inlining has been done yet. All the inlines and other optimizations are to be 
	added in a later stage when we have a better understanding of bottlenecks.
*/
class Quaternion  
{
public:
	Quaternion Inverse() const;
	Quaternion Conjugate() const;
	float Norm() const;
	AxisAngle ToAxisAngle() const;
	Quaternion& FromAxisAngle(const AxisAngle& axisAngle);
	Quaternion& FromRotationMatrix(Matrix4 &mat);
	Matrix4 ToRotationMatrix() const;
	//! Loads identity quaternion
	void Identity();
	//! Data members
	float x, y, z, w;
	//! Additional Constructor
	Quaternion(float pitch, float yaw, float roll);
	//! Additional Constructor
	Quaternion(float ix, float iy, float iz, float iw);
	//! Additional Constructor
	Quaternion(const AxisAngle& axisAngle);
	//! Copy Constructor
	Quaternion(const Quaternion & rhs);
	//! Default Constructor
	Quaternion() : x(0), y(0), z(0), w(1.0f) {};
	//! = operator overloaded
	Quaternion& operator=(const Quaternion& rhs);
	//! * operator overloaded
	Quaternion operator*(const Quaternion& rhs) const;
	//! + operator overloaded
	Quaternion operator+(const Quaternion& rhs) const;
};

} // namespace XMath

#endif // XQUATERNION_H_


