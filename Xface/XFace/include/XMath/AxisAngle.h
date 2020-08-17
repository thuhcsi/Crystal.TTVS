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

#pragma once

#ifndef AXISANGLE_H_
#define AXISANGLE_H_

#include <XMath/Vector3.h>

namespace XMath{

class AxisAngle
{
	Vector3 m_axis;
	float m_angle; //!< angle in radians
public:
	void setAngle(float ang) {m_angle = ang;}
	float getAngle() const {return m_angle;}

	void setAxis(const Vector3& axis) {m_axis = axis;}
	void setAxis(float x, float y, float z) {m_axis = Vector3(x, y, z);}
	Vector3 getAxis() const {return m_axis;}
	AxisAngle(void): m_axis(0, 0, 1), m_angle(0) {}
	AxisAngle(const Vector3& axis, float angle) :m_axis(axis), m_angle(angle){}
};

} // namespace XMath

#endif // AXISANGLE_H_