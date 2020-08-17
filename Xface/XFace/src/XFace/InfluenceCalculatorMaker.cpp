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

#include <XFace/InfluenceCalculatorMaker.h>

namespace XFace{

/*!
    Base class constructor registers the derived class identifier
    to its list of influencers.
    \param className derived class identifier (should be unique for every influence calc.)
*/	
InfluenceCalculatorMaker::InfluenceCalculatorMaker(const std::string& className)
{
	registry().insert(std::make_pair(className, this));
}

/*!
    Returns the list of influence calculator makers as a vector. 
    \note Do NOT call this method frequently, every time a new vector is created 
    and returned.
*/
std::vector<std::string> InfluenceCalculatorMaker::getMakerList()
{
	std::vector<std::string> ret;
	MakerMap::const_iterator it = registry().begin();
	while(it != registry().end())
	{
		ret.push_back(it->first);
		++it;
	}

	return ret;
}

/*!
    Clients should call this static method, to create influence calculators. It accesses the
    proper factory (maker) and requests a new influence calculator and returns it.
    \param className derived class unique maker identifier. 
    \param weight with this weight value,
    \param fapID and used for this FAP.
*/
IInfluenceCalculator* InfluenceCalculatorMaker::newInfluenceCalculator(const std::string& className, float weight, unsigned short fapID)
{
	MakerMap::iterator it = registry().find(className);
	if(it != registry().end())
		return it->second->makeInfluenceCalculator(weight, fapID);
	else
		return 0;
}

}// namespace XFace
