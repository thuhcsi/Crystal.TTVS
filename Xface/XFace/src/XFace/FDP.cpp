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
 * The Original Code is Xface Core Library; FDP Data.
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

#include <XFace/FDP.h>

namespace XFace{

FDP::~FDP()
{
	FDPITEMS::iterator it(m_Items.begin());
	while ( it != m_Items.end() )
	{
		delete *it;
		++it;
	}
}

const FDPItem* FDP::findItem(const std::string& name, const std::string& aff) const
{
	FDPITEMS::const_iterator it(m_Items.begin());
	while ( it != m_Items.end() )
	{
		if( ((*it)->getName() == name) && ( (*it)->getAffects() == aff) )
			return *it;
		++it;
	}
	
	return 0;
}

void FDP::removeItemsByAffects(const std::string &aff)
{
	FDPITEMS::iterator it = m_Items.begin();
	while (it != m_Items.end())
	{
		if((*it)->getAffects() == aff)
		{
			delete *it;
			it = m_Items.erase(it);
		}
		else
			++it;
	}
}
} // namespace XFace