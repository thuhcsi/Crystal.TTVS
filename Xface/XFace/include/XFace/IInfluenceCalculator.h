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
 * The Original Code is Xface Core Library; Deformation.
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
	\file IInfluenceCalculator.h
	\brief Interface for deforming indexed face set meshes.
*/

#ifndef IINFLUENCECALCULATOR_H_
#define IINFLUENCECALCULATOR_H_


#include <XMath/Vector3.h>
#include <vector>

/*!
	\addtogroup XFace XFace
	All face related XFace classes under XFace namespace are in this module.
*/


/*!
	\namespace XFace This is the main namespace XFace that the whole face library resides.
*/
namespace XFace{

	using XMath::Vector3;
	class FDPItem;

/*!
	\brief Interface for defining the weights in the zone of influence for deformation.
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   May 2003

	IInfluenceCalculator defines the way you should implement new classes to define zone of influence. 
	As this is an abstract base class, you cannot create an instance of IInfluenceCalculator, but you should 
	derive your classes from it.
*/
	class IInfluenceCalculator
	{
	protected:
		float m_coef;
		unsigned short m_fapID;
	public:
		virtual void init(const FDPItem* const pFDP) = 0;
		virtual ~IInfluenceCalculator(void){};
		virtual const std::vector<float>& getWeights() const = 0;
		IInfluenceCalculator(float coef, unsigned short fapID) : m_fapID(fapID), m_coef(coef){};
		virtual float getCoefficient() const {return m_coef;}
		unsigned short getFapID() const {return m_fapID;}
	};

} // namespace XFace

#endif // IINFLUENCECALCULATOR_H_