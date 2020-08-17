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

/*! 
	\file FDP.h
	\brief FDP information is here
*/
#ifndef FDP_H_
#define FDP_H_

#include "FDPItem.h"
#include <XMath/Vector3.h>
#include <XMath/AxisAngle.h>
#include <list>
#include <string>

namespace XFace{
	
	/*!
		\brief MPEG4 FAPU info is in here
		\ingroup XFace
		\author Koray Balci
		\version 1.0
		\date   June 2003
		Used as a data structure for MPEG4 FAPU information. 
	*/
	class FAPU
	{
	public:
		FAPU() : ES(0), IRISD(0), ENS(0), MNS(0), MW(0), AU(0.00001f) {};
		float ES, IRISD, ENS, MNS, MW, AU;
	};

	using XMath::Vector3;
	using XMath::AxisAngle;
/*!
	\brief MPEG4 FDP data as a whole
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	MPEG4 FDP data is stored in this class.
*/
class FDP
{
public:
	typedef std::list<FDPItem*> FDPITEMS;
private:
	FDPITEMS m_Items;
	FAPU m_fapu;
	AxisAngle m_axisAngle;
	Vector3 m_translation;  //!< Global translation
public:
	void removeItemsByAffects(const std::string &aff);

	//@{
	//! Accessor
	const FDPITEMS& getItems() const {return m_Items;};
	int getItemCount() const {return (int)m_Items.size();};
	void setES0(float es){m_fapu.ES = es / 1024.0f;};
	void setIRISD0(float irisd){m_fapu.IRISD = irisd / 1024.0f;};
	void setENS0(float ens){m_fapu.ENS = ens / 1024.0f;};
	void setMNS0(float mns){m_fapu.MNS = mns / 1024.0f;};
	void setMW0(float mw){m_fapu.MW = mw / 1024.0f;};
	void setFAPU(const FAPU& fapus){m_fapu = fapus;};

	float getES() const {return m_fapu.ES;};
	float getIRISD() const {return m_fapu.IRISD;};
	float getENS() const {return m_fapu.ENS;};
	float getMNS() const {return m_fapu.MNS;};
	float getMW() const {return m_fapu.MW;};
	const FAPU& getFAPU() const {return m_fapu;};

	void setGlobalTranslation(const Vector3& tr) {m_translation = tr;}
	void setGlobalTranslation(float x, float y, float z) {m_translation.x = x; m_translation.y = y; m_translation.z = z;}
	void setGlobalRotation(const AxisAngle& axisAng) {m_axisAngle = axisAng;}
	void setGlobalRotation(float x, float y, float z, float angle) {m_axisAngle.setAxis(x, y, z); m_axisAngle.setAngle(angle);}

	Vector3 getGlobalRotationAxis() const {return m_axisAngle.getAxis();}
	float getGlobalRotationAngle() const {return m_axisAngle.getAngle();}
	const AxisAngle& getGlobalAxisAngle() const {return m_axisAngle;}
	Vector3 getGlobalTranslation() const {return m_translation;}
	//@}

	//! Inserts a new FDPItem to the list stored inside.
	void insertItem(FDPItem* item){m_Items.push_back(item);};
	//! Finds and retrieves FDPItem from the collection stored inside.
	const FDPItem* findItem(const std::string& name, const std::string& aff) const;
	FDP(void) : m_translation(0, 0, 0){};
	~FDP();
};
	}
#endif // FDP_H_

