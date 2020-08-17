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

// Quaternion.cpp: implementation of the Quaternion class.
//
//////////////////////////////////////////////////////////////////////

#include "XMath/Quaternion.h"

#include <math.h>

namespace XMath{

const float PI       =  3.14159265358979323846f;		// Pi

#define	DEGTORAD(x)	( ((x) * PI) / 180.0f )
#define	RADTODEG(x)	( ((x) * 180.0f) / PI )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Quaternion::Quaternion(const Quaternion & rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
}

Quaternion::Quaternion(float ix, float iy, float iz, float iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}
/*!
	Constructor for creating quaternion from axis angle representation
	\param axisAngle axis-angle representation
*/
Quaternion::Quaternion(const AxisAngle& axisAngle)
{
	FromAxisAngle(axisAngle);
}

/*!
	Constructor for creating quaternion from euler angles
	\param pitch angle for X axis in degrees
	\param yaw angle for Y axis in degrees
	\param roll angle for Z axis in degrees
*/
Quaternion::Quaternion(float pitch, float yaw, float roll)
{
	float ax = DEGTORAD(pitch)*0.5f;
	float ay = DEGTORAD(yaw)*0.5f;
	float az = DEGTORAD(roll)*0.5f;
	const float sinx = (float)sin(ax);
	const float cosx = (float)cos(ax);
	const float siny	= (float)sin(ay);
	const float cosy = (float)cos(ay);
	const float sinz = (float)sin(az);
	const float cosz = (float)cos(az);
	const float cosxcosy = cosx*cosy;
	const float sinxsiny = sinx*siny;

	x = sinz * cosxcosy     - cosz * sinxsiny;
	y = cosz * sinx * cosy + sinz * cosx * siny;
	z = cosz * cosx * siny - sinz * sinx * cosy;
	w = cosz * cosxcosy     + sinz * sinxsiny;
}

Quaternion& Quaternion::operator=(const Quaternion& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

Quaternion Quaternion::operator +(const Quaternion& rhs) const
{
	return Quaternion(rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w);
}

Quaternion Quaternion::operator *(const Quaternion& rhs) const
{
	float rx, ry, rz, rw;

	rw = rhs.w*w - rhs.x*x - rhs.y*y - rhs.z*z;

	rx = rhs.w*x + rhs.x*w + rhs.y*z - rhs.z*y;
	ry = rhs.w*y + rhs.y*w + rhs.z*x - rhs.x*z;
	rz = rhs.w*z + rhs.z*w + rhs.x*y - rhs.y*x;

	return(Quaternion(rx, ry, rz, rw));
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}
float Quaternion::Norm() const
{
	return x*x + y*y + z*z + w*w;
}

Quaternion Quaternion::Inverse() const
{
    float norm = Norm();
    if ( norm > 0.0f )
    {
        float invNorm = 1.0f/norm;
        return Quaternion(-x*invNorm,-y*invNorm,-z*invNorm, w*invNorm);
    }
    else
    {
        return Quaternion();
    }
}
void Quaternion::Identity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

/*!
	Passage between quaternion->matrix representation.
	\return A valid 4x4 rotation matrix.
*/
Matrix4 Quaternion::ToRotationMatrix() const
{
	Matrix4 mat;
    float fTx  = 2.0f*x;
    float fTy  = 2.0f*y;
    float fTz  = 2.0f*z;
    float fTwx = fTx*w;
    float fTwy = fTy*w;
    float fTwz = fTz*w;
    float fTxx = fTx*x;
    float fTxy = fTy*x;
    float fTxz = fTz*x;
    float fTyy = fTy*y;
    float fTyz = fTz*y;
    float fTzz = fTz*z;

    mat[0] = 1.0f - (fTyy+fTzz);
    mat[1] = fTxy - fTwz;
    mat[2] = fTxz + fTwy;
    
	mat[4] = fTxy + fTwz;
    mat[5] = 1.0f - (fTxx+fTzz);
    mat[6] = fTyz - fTwx;
    
	mat[8] = fTxz - fTwy;
    mat[9] = fTyz + fTwx;
    mat[10] = 1.0f - (fTxx+fTyy);

	mat[3]  = mat[7] = mat[11] = 0.0f;
	mat[12] = mat[13] = mat[14] = 0.0f; 
    mat[15] = 1.0f;

	return mat;
}

/*!
	Passage between matrix->quaternion representation
	\param mat A valid 4x4 rotation matrix
*/
Quaternion& Quaternion::FromRotationMatrix(Matrix4 &mat)
{
	// Calculate the trace of the matrix T from the equation:
	//  T = 4 - 4xx  - 4yy  - 4zz
    float T = 1.0f + mat[0] + mat[5] + mat[10];


	double s;

	// If the trace of the matrix is greater than zero, then
	//perform an "instant" calculation.
	if ( T > 0.00000001 ) 
	{
		s = sqrt(T) * 2.0;
		x = (float)(( mat[9] - mat[6] ) / s);
		y = (float)(( mat[2] - mat[8] ) / s);
		z = (float)(( mat[4] - mat[1] ) / s);
		w = (float)(0.25 * s);
	}
    //If the trace of the matrix is equal to zero then identify
    //which major diagonal element has the greatest value.
    else if ( mat[0] > mat[5] && mat[0] > mat[10] )  
	{	
		// Column 0: 
        s  = sqrt( 1.0f + mat[0] - mat[5] - mat[10] ) * 2.0;
        x = (float)(0.25f * s);
        y = (float)((mat[4] + mat[1]) / s);
        z = (float)((mat[2] + mat[8]) / s);
        w = (float)((mat[9] - mat[6]) / s);
	
    } 
	else if ( mat[5] > mat[10] ) 
	{			
		// Column 1: 
        s  = sqrt( 1.0f + mat[5] - mat[0] - mat[10] ) * 2.0;
        x = (float)((mat[4] + mat[1] ) / s);
        y = (float)(0.25f * s);
        z = (float)((mat[9] + mat[6] ) / s);
        w = (float)((mat[2] - mat[8] ) / s);

    } 
	else 
	{						
		// Column 2:
        s  = sqrt( 1.0f + mat[10] - mat[0] - mat[5] ) * 2.0;
        x = (float)((mat[2] + mat[8] ) / s);
        y = (float)((mat[9] + mat[6] ) / s);
        z = (float)(0.25f * s);
        w = (float)((mat[4] - mat[1] ) / s);
    }

	return *this;
}

/*
	q = [nx*sin(phi/2) ny*sin(phi/2) nz*sin(phi/2) cos(phi/2)] = [qv qs]
*/
Quaternion& Quaternion::FromAxisAngle(const AxisAngle& axisAngle)
{
	Vector3 axis = axisAngle.getAxis();
	float sinang = sin(axisAngle.getAngle() / 2);
	x = axis.x * sinang;
	y = axis.y * sinang;
	z = axis.z * sinang;
	w = cos(axisAngle.getAngle() / 2);
	
	return *this;
}
/*
	The quaternion representing the rotation is
	q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
*/
AxisAngle Quaternion::ToAxisAngle() const
{
	float t   = w*w;
	float rsa = t<0.99999999f ? 1.0f/sqrt(1.0f-t) : 1.0f;
	Vector3 axis(x*rsa, y*rsa, z*rsa);
	float angle = acos(w)*2.0f;

	return AxisAngle(axis, acos(w)*2.0f);
}
} // namespace XMath