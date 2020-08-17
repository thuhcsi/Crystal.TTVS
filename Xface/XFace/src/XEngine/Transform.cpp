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

#include <XEngine/Transform.h>

namespace XEngine{

Transform::Transform(void) : m_scale(1, 1, 1), m_translation(0, 0, 0), m_rotation(0, 0, 0, 1), m_bNeedUpdate(false)
{
	m_worldTransform.loadIdentity();
	m_localTransform.loadIdentity();
}

void Transform::reset()
{
	m_scale = Vector3(1, 1, 1);
	m_translation = Vector3(0, 0, 0);
	m_rotation = Quaternion(0, 0, 0, 1);
	m_bNeedUpdate = true;
}

void Transform::rotate(const Quaternion& qRot)
{
	m_rotation = m_rotation * qRot;
	m_bNeedUpdate = true;
}
	
void Transform::translate(const Vector3& trans)
{
	m_translation += trans;
	m_bNeedUpdate = true;
}

// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate
void Transform::update(const Matrix4& parentMat)
{
	if(m_bNeedUpdate)
	{
		Matrix4 rotMat = m_rotation.ToRotationMatrix();
		Matrix4 scaleMat;
		scaleMat.loadIdentity();
		scaleMat[0] = m_scale.x;
		scaleMat[5] = m_scale.y;
		scaleMat[10] = m_scale.z;

		m_localTransform = rotMat * scaleMat;
		m_localTransform[12] = m_translation.x;
		m_localTransform[13] = m_translation.y;
		m_localTransform[14] = m_translation.z;

		m_bNeedUpdate = false;
	}

	m_worldTransform = m_localTransform * parentMat; 
}
    
} // namespace XEngine