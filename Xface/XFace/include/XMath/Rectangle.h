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
 * The Original Code is Xface Core Library; TCP Messaging.
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
	\file	Rectangle.h
	\brief	Rectangle representation
*/

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <XEngine/VertexStruct.h>

namespace XMath{

	using XEngine::Vertex2D;
/*!
	\brief		2D rectangle representation
	\ingroup	XMath
	\author		Koray Balci
	\version	1.0
	\date		April 2003

	Class that represents a rectangle in 2D, no operations (methods) are involved, only data abstraction for
	the time being.
*/
class Rectangle2
{
	Vertex2D m_topleft;
	Vertex2D m_bottomright;
public:
	//@{
	//! Accessor
	const Vertex2D& getTopLeft() const {return m_topleft;};
	const Vertex2D& getBottomRight() const {return m_bottomright;};
	float getX0() const {return m_topleft.x;};
	float getY0() const {return m_topleft.y;};
	float getX1() const {return m_bottomright.x;};
	float getY1() const {return m_bottomright.y;};
	float getWidth() const {return m_bottomright.x - m_topleft.x;};
	float getHeight() const {return m_bottomright.y - m_topleft.y;}; 

	void setX0(float _x0) {m_topleft.x = _x0;};
	void setY0(float _y0) {m_topleft.y = _y0;};
	void setX1(float _x1) {m_bottomright.x = _x1;};
	void setY1(float _y1) {m_bottomright.y = _y1;};
	//@}

	Rectangle2(float _x0 = 0, float _y0 = 0, float _x1 = 0, float _y1 = 0)
	{
		m_topleft.x = _x0;
		m_topleft.y = _y0;
		m_bottomright.x = _x0;
		m_bottomright.y = _y0;
	}
	/*!
		Checks whether the x/y values lie inside the rectangle.
		\note top bottom x/y reversed!! it's not a bug but just convention..
	*/
	bool isPointInRect(float x, float y) const
	{
		if(x > m_topleft.x && x < m_bottomright.x && y < m_topleft.y && y > m_bottomright.y)
			return true;
		return false;
	}
};
}
#endif