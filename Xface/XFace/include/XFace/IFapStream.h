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
 * The Original Code is Xface Core Library; MPEG4 Fap.
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
	\file IFapStream.h
	\brief Interface for MPEG-4 FAPs streaming.
*/
#ifndef IFAPSTREAM_H_
#define IFAPSTREAM_H_

#include <cassert>
#include <iostream>
#include "FDP.h" // to retrieve FAPU class

namespace XFace{
/*!
	\brief Interface for MPEG-4 FAPs streaming.
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	Interface for the FAP streaming. This base class defines the methodology to
	access FAP stream. Derived classes may include disk access and network streaming
	to load the FAPs to this interface.
*/
class IFapStream
{
protected:
	std::vector<float> m_currentFAP; //!< current FAP value
	short m_FPS; //!< Number of frames per second to render
	float m_version; //!< Version of the file
public:
	//! query open
	virtual bool isEnd() const = 0;
	//! query open
	virtual bool isOpen() const = 0;
	//! open/read the stream
	virtual bool open(std::istream&, const FAPU&) = 0;
	//! Advance to the next FAP.
	virtual void next() = 0; 
	//! Rewind to the first FAP.
	virtual void rewind() = 0;
	//! Retrieves FAP count
	virtual size_t getFAPCount() const = 0;
	virtual unsigned long getCurrentFAPId() const = 0;
	//! Retrieve file version
	float getVersion() const {return m_version;}
	//! Retrieve number of frames per second.
	short getFPS() const {return m_FPS;};
	//! Retrieve current FAP
	const std::vector<float>& getCurrentFAP() const {return m_currentFAP;};
	IFapStream(void) : m_FPS(0){};
	virtual ~IFapStream(void){};
};
} // namespace XFace
#endif // IFAPSTREAM_H_
