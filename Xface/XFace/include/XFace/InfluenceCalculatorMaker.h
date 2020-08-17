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
 * The Original Code is Xface Core Library;
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
	\file InfluenceCalculatorMaker.h
	\brief InfluenceCalculator pluggable factory maker
*/
#pragma once
#ifndef INFLUENCECALCULATORMAKER_H_
#define INFLUENCECALCULATORMAKER_H_


#include "IInfluenceCalculator.h"
#include <string>
#include <map>
#include <vector>

namespace XFace{

/*!
	\brief InfluenceCalculator pluggable factory maker
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   April 2004

	Implementation of "Pluggable Factory" design pattern. This class is responsible
	for creation of IInfluenceCalculator derived classes. Note that it is an abstract base class, 
	you should always derive a class and implement InfluenceCalculatorMaker::makeInfluenceCalculator 
    function with proper return value specialization for correct IInfluenceCalculator type.
	
	An IInfluenceCalculator derived class must always have a InfluenceCalculatorMaker derived class complementing
	it. XFaceEd automatically adds available IInfluenceCalculator derived classes to its
	influence set and exposes it to the end user. In addition, only this way the core library can 
    use the influence calculation mechanism properly, for utilizing correct influence calculator.
*/
class InfluenceCalculatorMaker
{
	typedef std::map<std::string, InfluenceCalculatorMaker* > MakerMap;
	static MakerMap& registry()
	{
		static MakerMap s_registry;
		return s_registry;
	}
public:
	static std::vector<std::string> getMakerList();
	static IInfluenceCalculator* newInfluenceCalculator(const std::string&, float w, unsigned short fapID);
	virtual ~InfluenceCalculatorMaker(void){};

protected:
    //! ctor only accessible to derived classes.
	InfluenceCalculatorMaker(const std::string& className);
	//! Derived classes should reimplement this pure virtual.
	virtual IInfluenceCalculator* makeInfluenceCalculator(float w, unsigned short fapID) const = 0;
	
};

} // namespace XFace

#endif // INFLUENCECALCULATORMAKER_H_
