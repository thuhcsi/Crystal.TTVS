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
	\file	XLineSegment.h
	\brief	Representation of a 3D line segment
*/
#ifndef XMATH_LINESEGMENT_H_
#define XMATH_LINESEGMENT_H_

#include "Vector3.h"

namespace XMath{

/*!
	\brief	Representation of a 3D line segment
	\ingroup XMath
	\author		Koray Balci
	\version	1.0
	\date		April 2003

	Class that represents a line segment in 3D, no operations (methods) are involved, only data abstraction for
	the time being.
*/
class LineSegment3
{
	Vector3 m_start, m_end;
public:
	LineSegment3(void){}
	LineSegment3(const Vector3& s, const Vector3& e) : m_start(s), m_end(e) {}
	//@{
	//! Accessor
	const Vector3& getStart() const {return m_start;}
	const Vector3& getEnd() const {return m_end;}

	void setStart(const Vector3& s)	{m_start = s;}
	void setEnd(const Vector3& e)	{m_end = e;}
	//@}
};

}
#endif // XMATH_LINESEGMENT_H_