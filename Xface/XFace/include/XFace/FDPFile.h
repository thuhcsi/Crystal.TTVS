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
 * The Original Code is Xface Core Library; FDP Streaming
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
	\file	FDPFile.h
	\brief	FDP file streaming
*/

#ifndef FDPFILE_H_
#define FDPFILE_H_

#include <tinyxml.h>

#include <string>
#include <list>
#include "FDP.h"
#include <XEngine/ModelFileFactory.h>


using XEngine::MeshInfo;

namespace XFace{
/*!
   \brief MPEG-4 FDP loader/writer from/to disk.
   \ingroup XFace
   \author Koray Balci

 Reads/Writes the MPEG-4 Facial Definition Parameters stored in XML format. 
 
*/
class FDPFile
{
	FDP* m_pFDP;
    float m_version;
	bool load(TiXmlDocument& doc, FDP* pFDP);
	void loadHeader(const TiXmlNode* pNode);
	void loadFDPItem(const TiXmlNode* pNode);
	bool loadSource(const TiXmlNode* pNode);
	void saveSource(TiXmlElement& x_xfdp);
	std::list<MeshInfo> m_meshList;
public:
    float getVersion() const {return m_version;};	
	std::list<MeshInfo> getMeshList() const {return m_meshList;}			// yes make a full copy in return
	void setMeshList(const std::list<MeshInfo>& meshes) {m_meshList = meshes;}

	bool load(const std::string& filename, FDP* pFDP);
	bool load(std::istream& input, FDP* pFDP);
	bool save(const std::string& filename, const FDP* pFDP);
	
	// class constructor
	FDPFile();
};
} // namespace XFace
#endif // FDPFILE_H_

