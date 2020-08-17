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
 * The Original Code is Xface Core Library; Deformation
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
	\file	RaisedCosInfluence.h
	\brief	Raised Cosine deformation 
*/
#ifndef RAISEDCOSINFLUENCE_H_
#define RAISEDCOSINFLUENCE_H_

#define M_PI 3.14159265358979323846

#include "IInfluenceCalculator.h"
#include "InfluenceCalculatorMaker.h"

namespace XFace{

	using XMath::Vector3;

/*!
	\brief Raised cosine deformation base
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Implements the base for Raised cosine deformation. Note that, this is still an abstract base class, 
	derive a new class from this one and implement the necessary pure virtual function. You can also
	use already implemented derived classes for fast use.

*/
class RaisedCosInfluence : public IInfluenceCalculator
{
	/*!
		calculates and initializes the weight values, by applying raised cosine function * Influence weight value
	*/
	struct weight_functor
	{
	private:
		float m_maxDist, m_weight;
	public:
		weight_functor(float mDist, float w) : m_maxDist(mDist), m_weight(w) {}
		void operator() (float& val){
			val = (float)(cos((M_PI*(val)/m_maxDist)) + 1)*m_weight;
		}
	};

	virtual float calculateDistance(const Vector3& p1, const Vector3& p2) = 0; 
	float calculateDistances(const FDPItem* const pFDP);
protected:
	std::vector<float> m_weights;
public:
	const std::vector<float>& getWeights() const {return m_weights;}
	RaisedCosInfluence(float weight, unsigned short fapID) :  IInfluenceCalculator(weight, fapID){}
	void init(const FDPItem* const pFDP);
};

/*!
	\brief Raised cosine deformation as a sphere
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Spherical deformation is done in this implementation. This means that, distance map to the control point
	is calculated in 3 dimensions, and when deformation is applied, every vertex affected moves according 
	to its 3d distance to the control point.
*/
class RaisedCosInfluenceSph : public RaisedCosInfluence
{
	float calculateDistance(const Vector3& p1, const Vector3& p2);
public:
	RaisedCosInfluenceSph(float weight, unsigned short fapID) :  RaisedCosInfluence(weight, fapID){}
};

/*!
	\brief Raised cosine influence maker
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Maker for spherical deformation.
*/
class RaisedCosInfluenceSphMaker : public InfluenceCalculatorMaker
{
private:
	RaisedCosInfluenceSphMaker() : InfluenceCalculatorMaker("RaisedCosInfluenceSph"){}
	RaisedCosInfluenceSph* makeInfluenceCalculator(float w, unsigned short fapID) const
	{
		return new RaisedCosInfluenceSph(w, fapID);
	}
	static const RaisedCosInfluenceSphMaker m_regRaisedCosInfluenceSph;
};

/*!
	\brief Raised cosine deformation as a wave in X direction
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Wave deformation is done in this implementation. This means that, distance map to the control point
	is calculated in only one particular dimensions (here in X), and when deformation is applied, every vertex 
	affected moves according to that distance to the control point.
*/
class RaisedCosInfluenceWaveX: public RaisedCosInfluence
{
	float calculateDistance(const Vector3& p1, const Vector3& p2);
public:
	RaisedCosInfluenceWaveX(float weight, unsigned short fapID) :  RaisedCosInfluence(weight, fapID){}
};

/*!
	\brief Raised cosine influence maker
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Maker for wave deformation in X direction.
*/
class RaisedCosInfluenceWaveXMaker : public InfluenceCalculatorMaker
{
private:
	RaisedCosInfluenceWaveXMaker() : InfluenceCalculatorMaker("RaisedCosInfluenceWaveX"){}
	RaisedCosInfluenceWaveX* makeInfluenceCalculator(float weight, unsigned short fapID) const
	{
		return new RaisedCosInfluenceWaveX(weight, fapID);
	}
	static const RaisedCosInfluenceWaveXMaker m_regRaisedCosInfluenceWaveX;
};

/*!
	\brief Raised cosine deformation as a wave in Y direction
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Wave deformation is done in this implementation. This means that, distance map to the control point
	is calculated in only one particular dimensions (here in Y), and when deformation is applied, every vertex 
	affected moves according to that distance to the control point.
*/
class RaisedCosInfluenceWaveY: public RaisedCosInfluence
{
	float calculateDistance(const Vector3& p1, const Vector3& p2);
public:
	RaisedCosInfluenceWaveY(float weight, unsigned short fapID) :  RaisedCosInfluence(weight, fapID){}
};

/*!
	\brief Raised cosine Influence maker
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Maker for wave deformation in Y direction.
*/
class RaisedCosInfluenceWaveYMaker : public InfluenceCalculatorMaker
{
private:
	RaisedCosInfluenceWaveYMaker() : InfluenceCalculatorMaker("RaisedCosInfluenceWaveY"){}
	RaisedCosInfluenceWaveY* makeInfluenceCalculator(float weight, unsigned short fapID) const
	{
		return new RaisedCosInfluenceWaveY(weight, fapID);
	}
	static const RaisedCosInfluenceWaveYMaker m_regRaisedCosInfluenceWaveY;
};

/*!
	\brief Raised cosine deformation as a wave in Z direction
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Wave deformation is done in this implementation. This means that, distance map to the control point
	is calculated in only one particular dimensions (here in Z), and when deformation is applied, every vertex 
	affected moves according to that distance to the control point.
*/
class RaisedCosInfluenceWaveZ: public RaisedCosInfluence
{
	float calculateDistance(const Vector3& p1, const Vector3& p2);
public:
	RaisedCosInfluenceWaveZ(float weight, unsigned short fapID) :  RaisedCosInfluence(weight, fapID){}
};

/*!
	\brief Raised cosine Influence maker
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2004

	Maker for wave deformation in Z direction.
*/
class RaisedCosInfluenceWaveZMaker : public InfluenceCalculatorMaker
{
private:
	RaisedCosInfluenceWaveZMaker() : InfluenceCalculatorMaker("RaisedCosInfluenceWaveZ"){}
	RaisedCosInfluenceWaveZ* makeInfluenceCalculator(float weight, unsigned short fapID) const
	{
		return new RaisedCosInfluenceWaveZ(weight, fapID);
	}
	static const RaisedCosInfluenceWaveZMaker m_regRaisedCosInfluenceWaveZ;
};

} // namespace XFace

#endif // RAISEDCOSINFLUENCE_H_
