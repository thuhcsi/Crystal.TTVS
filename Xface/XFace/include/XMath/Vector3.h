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
	\file		Vector3.h
	\brief		3D Vector class
*/

#ifndef VECTOR3_H_
#define VECTOR3_H_

/*!
	\addtogroup XMath XMath
	Math related classes are in this module.
*/

#include <math.h>

/*!
	\namespace XMath This namespace is separate from XFace namespace and provides general math functionality.
*/
namespace XMath{
/*!
	\brief		3D Vector class
	\ingroup XMath
	\author		Koray Balci
	\version	1.0
	\date		April 2002

	Class that represents a vector in 3D, and necessary mathematical operations.
	\note A small bit of advice can be usage of *=, +=, -= instead of *, +, - for optimization purposes.
	Because, each time the latter versions are called, a new Vector3 object is created, and returned.
*/
class Vector3  
{
public:
//	void Rotate(float angle, const Vector3& axis);
	//! /= operator overloaded for scalar division
	inline Vector3& operator/= (float scalar);
	//! / operator overloaded for scalar division
	inline Vector3 operator/ (float scalar) const;
	//! Negation operation overloaded
	inline Vector3 operator- () const;
	//! -= operator overloaded
	inline Vector3& operator-= (const Vector3& rhs);
	//! += operator overloaded
	inline Vector3& operator+= (const Vector3& rhs);
	//! subtraction operator overloaded
	inline Vector3 operator- (const Vector3& rhs) const;
	//! + operator overloaded
	inline Vector3 operator+ (const Vector3& rhs) const;
	//! Unitize the current vector
	inline Vector3& normalize(float tolerance = 1e-06);
	//! Computes square of magnitude of the vector 
	inline float lengthSqr() const;
	//! Computes magnitude of the vector
	inline float length() const;
	//! *= operator for scalar multiplication
	inline Vector3& operator *=(const float scalar);
	//! * operator for scalar multiplication
	inline Vector3 operator*(const float scalar) const;
	//! Cross Product with a unit vector output
	inline Vector3 unitCross (const Vector3& rhs) const;
	//! Cross Product
	inline Vector3 cross(const Vector3& rhs) const;
	//! Dot Product
	inline float dot(const Vector3& rhs) const;
	//! != operator overloaded
	inline bool operator!= (const Vector3& rhs) const;
	//! == operator overloaded
	inline bool operator== (const Vector3& rhs) const;
	//! = operator overloaded
	inline Vector3 &operator=(const Vector3 & rhs);
	//! Copy Constructor
	Vector3(const Vector3& pCopy);
	//! Additional Constructor
	Vector3(const float ix, const float iy, const float iz);
	//! Default Constructor
	Vector3(){};
	//! Destructor
	~Vector3(){};
	//! float* type cast operator
	inline operator float*() {return &x;};
	//! const float* type cast operator
	inline operator const float*() const {return &x;};
	//! x component in 3D space
	float x;
	//! y component in 3D space
	float y;
	//! z component in 3D space
	float z;
};

//! We define infinity value for avoiding divide by zero cases
#ifndef INFINITY
#define INFINITY 3.402823466e+38F
#endif


inline Vector3 &Vector3::operator=(const Vector3 &rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

    x = rhs.x;
    y = rhs.y;
	z = rhs.z;

    return *this;  // Assignment operator returns left side.
}

inline bool Vector3::operator== (const Vector3& rhs) const
{
    return ( x == rhs.x && y == rhs.y && z == rhs.z );
}

inline bool Vector3::operator!= (const Vector3& rhs) const
{
    return ( x != rhs.x || y != rhs.y || z != rhs.z );
}

inline Vector3 Vector3::cross(const Vector3& rhs) const
{
	return Vector3(y*rhs.z - z*rhs.y,
		z*rhs.x - x*rhs.z,
		x*rhs.y - y*rhs.x);
}

inline float Vector3::length() const
{
	return (float) sqrt(x*x + y*y + z*z);
}

inline float Vector3::lengthSqr() const
{
	return x*x + y*y + z*z;
}

inline float Vector3::dot(const Vector3& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

inline Vector3 Vector3::operator*(const float scalar) const
{
	Vector3 res;
	res.x = x*scalar;
	res.y = y*scalar;
	res.z = z*scalar;
	
	return res;
}

inline Vector3& Vector3::operator *=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return (*this);
}

inline Vector3 Vector3::operator+ (const Vector3& rhs) const
{
    Vector3 result;
    result.x = x + rhs.x;
    result.y = y + rhs.y;
    result.z = z + rhs.z;
    return result;
}

inline Vector3 Vector3::operator- (const Vector3& rhs) const
{
    Vector3 result;
    result.x = x - rhs.x;
    result.y = y - rhs.y;
    result.z = z - rhs.z;
    return result;
}

inline Vector3& Vector3::operator+= (const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

inline Vector3& Vector3::operator-= (const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}


inline Vector3 Vector3::operator- () const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator/ (float scalar) const
{
    Vector3 result;

	// A small optimization since multiplication is faster than division.
    if ( scalar != 0.0 )
    {
        float inv_scalar = 1.0f/scalar;
        result.x = inv_scalar*x;
        result.y = inv_scalar*y;
        result.z = inv_scalar*z;
        return result;
    }
    else
    {
        return Vector3(INFINITY, INFINITY, INFINITY);
    }
}

inline Vector3& Vector3::operator/= (float scalar)
{
	// A small optimization since multiplication is faster than division.
    if ( scalar != 0.0 )
    {
        float inv_scalar = 1.0f/scalar;
        x *= inv_scalar;
        y *= inv_scalar;
        z *= inv_scalar;
    }
    else
    {
        x = INFINITY;
        y = INFINITY;
        z = INFINITY;
    }

    return *this;
}

/*!
	Makes the current Vector a unit vector.
	\param tolerance If the actual magnitude is smaller than this value, unitizing
	cannot be done, and a value of 0 magnitude is returned.
	\return Returns the old magnitude (or 0 if too small before) of the vector
*/
inline Vector3& Vector3::normalize(float tolerance)
{
    float mag = length();

    if ( mag > tolerance )
    {
        float inv_mag = 1.0f/mag;
        x *= inv_mag;
        y *= inv_mag;
        z *= inv_mag;
    }
    else
    {
        mag = tolerance;
    }

    return *this;
}
/*!
	A special version of cross multiplicatio that returns a unit vector as a result.
	Might be handy for lighting calculations especially.
	\return A unit vector perpendicular to the plane defined by this vector and the vector passed as parameter.
*/
inline Vector3 Vector3::unitCross (const Vector3& rhs) const
{
    Vector3 result;

    result.x = y*rhs.z - z*rhs.y;
    result.y = z*rhs.x - x*rhs.z;
    result.z = x*rhs.y - y*rhs.x;
    result.normalize();

    return result;
}
inline Vector3::Vector3(const Vector3& pCopy)
{
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
}

inline Vector3::Vector3(const float ix, const float iy, const float iz)
{
	x = ix;
	y = iy;
	z = iz;
}
}
#undef INFINITY 
#endif // VECTOR3_H_

