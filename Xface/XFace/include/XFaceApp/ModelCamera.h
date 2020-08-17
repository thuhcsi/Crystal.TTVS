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
 * The Original Code is XfaceApp Application Library.
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
	\file		ModelCamera.h
	\brief		Camera class
*/
#ifndef MODELCAMERA_H_
#define MODELCAMERA_H_

#include <XMath/Matrix4.h>
#include <XMath/AxisAngle.h>

using XMath::Matrix4;
using XMath::Vector3;
using XMath::AxisAngle;

namespace XFaceApp{

/*!
	\brief Camera class
	\ingroup XFaceApp
	\author Koray Balci

	\todo This class should be in another name space, in some other place, XEngine perhaps? 
*/
class ModelCamera
{
	long m_wndWidth;
	long m_wndHeight;
	Vector3 m_start;
	long m_panX;
	long m_panY;

	float m_rotAngle;
	Vector3 m_rotAxis;
	float m_zoom;
	Matrix4 m_RotMatrix;
	void rotate(int x, int y);
	void zoom(int z);
	void pan(int x, int y);
public:
	enum kMODE
	{
		IDLE = 0,
		ZOOM,
		ROTATE,
		PAN
	};

	void setScreenSize(long w, long h) {m_wndWidth = w; m_wndHeight = h;}
	void setMode(kMODE mode){m_mode = mode;};
	void start(int x, int y);
	void end();
	void update(int x, int y);
	void apply();
	void setDistance(float dist){m_zoom = dist;};
	void setTranslation(const Vector3& trans) {m_zoom = trans.z; m_panX = (long)trans.x; m_panY = (long)trans.y;}

	AxisAngle getAxisAngle() const;
	void setAxisAngle(const AxisAngle& axisAngle);
	Vector3 getTranslation() const {Vector3 trans((float)m_panX, (float)m_panY, m_zoom); return trans;}
	ModelCamera();
private:
	kMODE m_mode;
};
} // namespace XFace
#endif //MODELCAMERA_H_