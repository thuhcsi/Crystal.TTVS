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
 * The Original Code is Xface Core Library; TCP Messaging.
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
	\file	FDPItem.h
	\brief	FDP item data representation
*/

#ifndef FDPITEM_H_
#define FDPITEM_H_

#include <XFace/IInfluenceCalculator.h>
#include <set>
#include <map>
#include <vector>
#include <string>

namespace XFace{


/*!
	\brief MPEG4 FDP data
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	MPEG4 FDP data is stored in this class. Every FDPItem defines a specific FDP region on the face model, 
	and all are stored (usually) in FDP class as a collection. 
*/
class FDPItem
{
	struct clearInfluenceCalculator_functor
	{
		void operator() (const std::pair<IInfluenceCalculator* const, std::string>& inf) const{
			delete inf.first;
		}
	};
	std::map<IInfluenceCalculator*, std::string> m_InfluenceCalculators;
	bool m_hasControlPoint;
	std::string m_name;					//!< ID of this FDP
	unsigned short m_controlPoint;		//!< Which vertex this FDP actually refers to in original model
	std::string m_affects;
public:
	typedef std::set<unsigned short> AOI_STORE;
	//! A map, instead of a vector<IInfluenceCalculator*> is used, just to have the type info for saving/loading FDP to disk!
	typedef std::map<IInfluenceCalculator*, std::string> INFLUENCECALC_MAP;
private:
	AOI_STORE m_AOI;	//!< Area of influence 

public:
	//@{
	//! Accessor
//	void addAOI(AOI_STORE &aoi){m_AOI.insert(aoi.begin(), aoi.end());};
	const AOI_STORE getAOI() const {return m_AOI;};
	const std::string getName() const {return m_name;};
	void setName(const std::string& name) {m_name = name;};
	unsigned short getIndex() const {return m_controlPoint;};
	bool hasControlPoint() const {return m_hasControlPoint;};
	void setControlPoint(unsigned short ind) {m_controlPoint = ind; m_hasControlPoint = true;};
	const std::string& getAffects() const {return m_affects;};
	void setAffects(const std::string& aff) {m_affects = aff;};
	void addAOIIndex(unsigned short ind) {m_AOI.insert(ind);};
	void removeAOIIndex(unsigned short ind) {m_AOI.erase(ind);};
	bool removeInfluenceCalculator(const std::string& type, float w, unsigned short id);
	void modifyInfluenceCalculator(unsigned int order, const std::string& type, float w, unsigned short fap);
	void resetAOI() {m_AOI.clear();}
	int getAOICount() const {return (int)m_AOI.size();};
	//@}

	const INFLUENCECALC_MAP& getInfluenceCalculators() const {return m_InfluenceCalculators;}
	void addInfluenceCalculator(IInfluenceCalculator* pInf, const std::string& type){m_InfluenceCalculators.insert(std::make_pair(pInf, type));}
	FDPItem(const std::string& name) : m_hasControlPoint(false), m_name(name), m_controlPoint(0), m_affects(""){};
	~FDPItem(){clearInfluenceCalculators();}
	void clearInfluenceCalculators();
};
	} // namespace XFace
#endif