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
 * The Original Code is Xface Core Library.
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
	\file	Transform.h
	\brief	Transform node for scenegraph.
*/
#pragma once
#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <XMath/Vector3.h>
#include <XMath/Matrix4.h>
#include <XMath/Quaternion.h>

using namespace XMath;

namespace XEngine{

/*!
	\brief	Transform node for scenegraph.
	\author	Koray Balci
	\ingroup XEngine
	\date April 2004 
		
	Represents the transformation node of the scenegraph by storing translation 
	and rotation information.
	\sa Drawable, Geometry
*/
class Transform
{
	bool m_bNeedUpdate;
	Quaternion m_rotation;
	Vector3 m_translation;
	Vector3 m_scale;
	Matrix4 m_localTransform;
	Matrix4 m_worldTransform;
public:
	const Matrix4& getWorldTransform() const {return m_worldTransform;}
	const Matrix4& getLocalTransform() const {return m_localTransform;}
	void update(const Matrix4& parentMat);
	void reset();
	void rotate(const Quaternion& qRot);
	void translate(const Vector3& trans);
    //!@{Accessor
	Vector3 getTranslation() const {return m_translation;}
	Vector3 getScale() const {return m_scale;}
	void setTranslation(const Vector3& trans) {m_translation = trans;m_bNeedUpdate = true;}
	void setTranslation(float x, float y, float z) {m_translation = Vector3(x, y, z);m_bNeedUpdate = true;}
	void setRotation(const Quaternion& qRot) {m_rotation = qRot;m_bNeedUpdate = true;}
	void setScale(const Vector3& scale) {m_scale = scale; m_bNeedUpdate = true;}
	//!@}
	Transform(void);
	virtual ~Transform(){};
};

} // namespace XEngine

#endif // TRANSFORM_H_
