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
 * The Original Code is Xface Core Library; FDP.
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

#include <algorithm>
#include <XFace/FDPItem.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>


namespace XFace{

void FDPItem::clearInfluenceCalculators()
{
	std::for_each(m_InfluenceCalculators.begin(), m_InfluenceCalculators.end(), clearInfluenceCalculator_functor());
}

bool FDPItem::removeInfluenceCalculator(const std::string& type, float w, unsigned short fap)
{
	// seek the coresponding InfluenceCalculator in fdp item
	INFLUENCECALC_MAP::iterator it = m_InfluenceCalculators.begin();
	while(it != m_InfluenceCalculators.end())
	{
		if((it->first->getFapID() == fap) && (it->first->getCoefficient() == w)
			&& (it->second == type))
		{
			m_InfluenceCalculators.erase(it);	// and erase it!
			return true;
		}
		++it;
	}

	return false;
}

void FDPItem::modifyInfluenceCalculator(unsigned int order, const std::string& type, float w, unsigned short fap)
{
	// if it is existing, seek & destroy first
	if(order < m_InfluenceCalculators.size())
	{
		// seek the coresponding InfluenceCalculator in fdp item
		INFLUENCECALC_MAP::iterator it = m_InfluenceCalculators.begin();
		unsigned int i = 0;
		while(i < order)
		{
			++it;++i;
		}	
		delete it->first;
		m_InfluenceCalculators.erase(it);
	}
	IInfluenceCalculator* pInfluence = InfluenceCalculatorMaker::newInfluenceCalculator(type, w, fap);
	addInfluenceCalculator(pInfluence, type);
}

} // namespace XFace