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
 * The Original Code is Xface Core Library;.
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
	\file FapFile.h
	\brief MPEG-4 FAPs loader from disk.
*/
#pragma once
#ifndef FAPFILE_H_
#define FAPFILE_H_


#include "IFapStream.h"
#include <string>
#include <list>

namespace XFace{
	class FAPU;
	
/*!
	\brief MPEG-4 FAPs loader from disk.
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	This class accesses the contents of a FAP file from a disk and using the base 
	class interface (IFapStream) provides FAP data to its clients. 
*/
class FAPFile :	public IFapStream
{
	std::vector< std::vector<float> > m_FAPs;
	unsigned long m_currentFAPID;
	void scaleFAPs(const FAPU& fapu);
	static void scaleFAPVect(std::vector<float>& faps, const FAPU& fapu);
	static void unscaleFAPVect(std::vector<float>& faps, const FAPU& fapu);
	void adjustFAPs();
	bool m_bLoaded;
public:
	bool isEnd() const {return m_currentFAPID == m_FAPs.size() - 1;}
	bool isOpen() const {return m_bLoaded;}
	void next();
	void rewind();
	bool open(std::istream&, const FAPU&);
	virtual size_t getFAPCount() const {return m_FAPs.size();}
	virtual unsigned long getCurrentFAPId() const{return m_currentFAPID;}

	//! Saves fap stream to disk
	static bool save(IFapStream& faps, const FAPU& fapu, const std::string& filename);
	
	FAPFile(void);
	~FAPFile(void);
};
} // namespace XFace

#endif // FAPFILE_H_

