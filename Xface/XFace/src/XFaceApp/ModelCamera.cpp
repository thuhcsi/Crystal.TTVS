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

#include <XFaceApp/ModelCamera.h>
#include <XMath/Quaternion.h>
using XMath::Quaternion;

#ifdef USE_EGL
    #include <GLES/gl.h>
#elif WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
    #include <GL/gl.h>
#endif

namespace XFaceApp{

ModelCamera::ModelCamera() 
	: m_wndWidth(0), m_wndHeight(0), m_zoom(-10), m_rotAngle(0)
	, m_mode(IDLE), m_panY(0), m_panX(0), m_rotAxis(Vector3(0, 1, 0))
{
	m_RotMatrix.loadIdentity();
}

void ModelCamera::end()
{
	/* save where we are */
	glPushMatrix();
		/* update our stored matrix with a new rotation */
		glLoadIdentity();
		if( m_rotAngle )
			glRotatef( m_rotAngle, m_rotAxis.x, m_rotAxis.y, m_rotAxis.z);
		glMultMatrixf( m_RotMatrix );
   
		glGetFloatv( GL_MODELVIEW_MATRIX, m_RotMatrix );
	glPopMatrix();
	
	m_rotAngle = 0;
}

void ModelCamera::start(int x, int y)
{
	/* store the down spot for later */
    m_start.x = (float)x;
	m_start.y = (float)y;
	m_start.z = (float)y;
}

void ModelCamera::update(int x, int y)
{
	switch(m_mode)
	{
	case ROTATE:
		rotate(x, y);
		break;
	case ZOOM:
		zoom(y);
		break;
	case PAN:
		pan(x, y);
	default:
		break;
	}
}
void ModelCamera::rotate(int x, int y)
{
	// calculate percentages from centre of screen 
    float horPct = (float)(x - m_wndWidth/2 ) / (float)(m_wndWidth/2);
    float verPct = (float)(y - m_wndHeight/2 ) / (float)(m_wndHeight/2);

	// small speed up    
	float xDif = x - m_start.x;
	float yDif = y - m_start.y;

	// get percentages for rotations 
    m_rotAxis.y = (1.0f - fabs(verPct))*xDif;
    m_rotAxis.x = (1.0f - fabs(horPct))*yDif;
    m_rotAxis.z = horPct/2*yDif - verPct/2*xDif;
	m_rotAxis.normalize();

    // amount is distance between current and initial pos
    m_rotAngle = sqrt( xDif*xDif + yDif*yDif);
}
void ModelCamera::zoom(int z)
{
	// change zoom and reset reference
	m_zoom += m_start.z - z;
	m_start.z = z;
}

void ModelCamera::pan(int x, int y)
{
	m_panX -= m_start.x - x;
	m_panY += m_start.y - y;
	
	m_start.y = y;
	m_start.x = x;
}

void ModelCamera::apply()
{
	glLoadIdentity();

	/* move the image back from the camera */
	glTranslatef( m_panX, m_panY, m_zoom );

	/* rotate image about its origin */
	if( m_rotAngle )
		glRotatef( m_rotAngle, m_rotAxis.x, m_rotAxis.y, m_rotAxis.z);
	glMultMatrixf( m_RotMatrix );
}
	
AxisAngle ModelCamera::getAxisAngle() const
{
	Matrix4 mat;
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );
	Quaternion quat;
	quat.FromRotationMatrix(mat);
	return quat.ToAxisAngle();
}

void ModelCamera::setAxisAngle(const AxisAngle& axisAngle)
{
	Quaternion quat(axisAngle);
	m_RotMatrix = quat.ToRotationMatrix();
	apply();
}
} // namespace XFace