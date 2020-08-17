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
 * The Original Code is Xface Core Library; Matrix4 Math.
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


#include <XMath/Matrix4.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace XMath{

Matrix4::Matrix4()
{

}

Matrix4::Matrix4 (float f00, float f01, float f02, float f03,
    float f10, float f11, float f12, float f13, 
	float f20, float f21, float f22, float f23,
	float f30, float f31, float f32, float f33)
{
	m_data[0] = f00; m_data[1] = f01; m_data[2] = f02; m_data[3] = f03;
	m_data[4] = f10; m_data[5] = f11; m_data[6] = f12; m_data[7] = f13;
	m_data[8] = f20; m_data[9] = f21; m_data[10] = f22; m_data[11] = f23;
	m_data[12] = f30; m_data[13] = f31; m_data[14] = f32; m_data[15] = f33;
}

Matrix4::Matrix4(const float mat[16])
{
	memcpy(m_data, mat, 16*sizeof(float));
}

Matrix4::Matrix4(const Matrix4& pCopy)
{
	memcpy(m_data, pCopy.m_data, 16*sizeof(float));
}

Matrix4& Matrix4::operator=(const Matrix4 &rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	memcpy(m_data, rhs.m_data, 16*sizeof(float));
    
    return *this;  // Assignment operator returns left side.
}

/*!
	Matrix by Vector Multiplication operator overloaded.
	\warning Multiplied according to OpenGL matrix format!
*/
Vector3 Matrix4::operator *(const Vector3& rhs) const
{
	float tx = 
		  m_data[0] * rhs.x 
		+ m_data[4] * rhs.y 
		+ m_data[8] * rhs.z 
		+ m_data[12];
	float ty = 
		  m_data[1] * rhs.x 
		+ m_data[5] * rhs.y 
		+ m_data[9] * rhs.z 
		+ m_data[13];
	float tz = 
		  m_data[2] * rhs.x 
		+ m_data[6] * rhs.y 
		+ m_data[10]* rhs.z 
		+ m_data[14];
	return Vector3(tx,ty,tz);
}

/*!
	Matrix by Matrix Multiplication operator overloaded.
	\warning Not tested and can be optimized as well.
*/
Matrix4 Matrix4::operator *(const Matrix4& rhs)
{
	Matrix4 result;

	double	sum;
	int	index, alpha, beta;		// loop vars
	
	for (index = 0; index < 4; index++)			// perform multiplcation the slow and safe way
	{
		for (alpha = 0; alpha < 4; alpha++)
		{
			sum = 0.0f;

			for (beta = 0; beta < 4; beta++)
				sum += m_data[index + beta*4] * rhs[alpha*4 + beta];

			result[index + alpha*4] = (float)sum;
		}	// end for alpha
	}	// end for index

	return result;
}

/*!
	Matrix by Scalar Multiplication operator overloaded.
	\note Not tested and can be optimized as well.
*/
Matrix4 Matrix4::operator *(float scalar) const
{
	Matrix4 result;

	for(int i = 0; i < 16; ++i)
		result[i] = m_data[i]*scalar;

	return result;
}

/*!
	Returns the transpose of the current matrix data. 
	Does not change the content of this matrix.
*/
Matrix4 Matrix4::getTranspose() const
{
	return Matrix4(m_data[0], m_data[4], m_data[8], m_data[12],
		m_data[1], m_data[5], m_data[9], m_data[13],
		m_data[2], m_data[6], m_data[10], m_data[14],
		m_data[3], m_data[7], m_data[11], m_data[15]);
}

/*!
	Loads the identity matrix, erasing current data.
*/
void Matrix4::loadIdentity()
{
	memset(m_data, 0, 16*sizeof(float));
	m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1.0f;
}

/*!
	Gets the inverse of the transformation matrix. 
	\note Only usable for OpenGL transform matrix inverse
*/
Matrix4 Matrix4::getInverseTransform() const
{
	return Matrix4(	m_data[0],
						m_data[4],
						m_data[8],
						0.0f,
						m_data[1],
						m_data[5],
						m_data[9],
						0.0f,
						m_data[2],
						m_data[6],
						m_data[10],
						0.0f,
						-(m_data[0]*m_data[12]+m_data[1]*m_data[13]+m_data[2]*m_data[14]),
						-(m_data[4]*m_data[12]+m_data[5]*m_data[13]+m_data[6]*m_data[14]),
						-(m_data[8]*m_data[12]+m_data[9]*m_data[13]+m_data[10]*m_data[14]),
						1.0f);
/*
	//////////////////////////////////////////////
	//// Calculate Transpose Of Source Matrix ////
	//////////////////////////////////////////////
 	
	Matrix4 temp;
	temp.m_data[0] = m_data[0];
	temp.m_data[1] = m_data[4];
	temp.m_data[2] = m_data[8];

	temp.m_data[4] = m_data[1];
	temp.m_data[5] = m_data[5];
	temp.m_data[6] = m_data[6];

	temp.m_data[8] = m_data[2];
	temp.m_data[9] = m_data[6];
	temp.m_data[10] = m_data[10];

	//////////////////////////////////////////////////////////////////////
	//// Transform Negated Source Position Into New Coordinate System ////
	//////////////////////////////////////////////////////////////////////

	temp.m_data[12] = (-m_data[12]*temp.m_data[0])+(-m_data[13]*temp.m_data[4])+(-m_data[14]*temp.m_data[8]);
	temp.m_data[13] = (-m_data[12]*temp.m_data[1])+(-m_data[13]*temp.m_data[5])+(-m_data[14]*temp.m_data[9]);
	temp.m_data[14] = (-m_data[12]*temp.m_data[2])+(-m_data[13]*temp.m_data[6])+(-m_data[14]*temp.m_data[10]);

	temp.m_data[3] = temp.m_data[7] = temp.m_data[11] = 0;
	temp.m_data[15] = 1.0f;

	*this = temp;
*/
}

/*!
	Negates the contents of the matrix.
*/
void Matrix4::negate(void)
{
	for (int i = 0; i < 16; i++)
		m_data[i] = -m_data[i];
}
} // namespace XMath