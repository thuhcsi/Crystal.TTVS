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
 * The Original Code is Xface Core Library; FDP File Loader.
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

#include <XFace/FDPFile.h>
#include <XFace/FDPItem.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>

#if defined(USE_EGL)
    #ifdef _DEBUG
    #pragma message("		Adding TinyXML STL lib (WinCE Debug)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MDd compiler switch...")
    #pragma comment(lib, "tinyxmlSTL_CE")
    #else
    #pragma message("		Adding TinyXML STL lib (WinCE Release)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MD compiler switch...")
    #pragma comment(lib, "tinyxmlSTL_CE.lib")
    #endif
#else
    #ifdef _DEBUG
    #pragma message("		Adding TinyXML STL lib (Win32 Debug)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MDd compiler switch...")
    #pragma comment(lib, "tinyxmlSTL")
    #else
    #pragma message("		Adding TinyXML STL lib (Win32 Release)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MD compiler switch...")
    #pragma comment(lib, "tinyxmlSTL")
    #endif
#endif

#include <iostream>
#include <string>
#include <map>
#ifdef TIXML_USE_STL
	#include <sstream>
	#include <strstream>
#else
	#include <stdio.h>
#endif


#include <strstream>

using namespace XMath;
namespace XFace{

// class constructor
FDPFile::FDPFile() : m_version(0.2f), m_pFDP(0)
{
}

bool FDPFile::save(const std::string& filename, const FDP* pFDP)
{
	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", "", "yes");
	doc.InsertEndChild(decl);

	TiXmlComment comment;
	comment.SetValue("XFaceEd generated MPEG-4 FDP specification file");
	doc.InsertEndChild(comment);
	TiXmlElement x_xfdp("xfdp");	// main body
		comment.SetValue("Header information for the model");
		x_xfdp.InsertEndChild(comment);
	
		TiXmlElement x_head("head");	// header information
			comment.SetValue("Version of the FDP specification file (this file)");
			x_head.InsertEndChild(comment);
	
			TiXmlElement x_file("file");	// file element
			std::strstream fileversion;
			fileversion << getVersion() << '\0';
			x_file.SetAttribute("version", fileversion.str());
			x_head.InsertEndChild(x_file);

			comment.SetValue("3D model (mesh) info");
			x_head.InsertEndChild(comment);
			TiXmlElement x_model("model");	// model element
//			x_model.SetAttribute("filename", pFDP->getModelFilename().c_str());
			x_model.SetAttribute("fdpcount", pFDP->getItemCount());
			x_head.InsertEndChild(x_model);

			comment.SetValue("FAPU (Facial Animation Parameter Units) for the model");
			x_head.InsertEndChild(comment);
			TiXmlElement x_fapu("fapu");	// fapu element
			
			std::strstream es, irisd, ens, mns, mw;
			es		<< pFDP->getES()*1024		<< '\0';
			irisd	<< pFDP->getIRISD()*1024	<< '\0';
			ens		<< pFDP->getENS()*1024		<< '\0';
			mns		<< pFDP->getMNS()*1024		<< '\0';
			mw		<< pFDP->getMW()*1024		<< '\0';

			x_fapu.SetAttribute("ES0",		es.str());	// es0
			x_fapu.SetAttribute("IRISD0",	irisd.str());	// irisd0
			x_fapu.SetAttribute("ENS0",		ens.str());	// ens0
			x_fapu.SetAttribute("MNS0",		mns.str());	// mns0
			x_fapu.SetAttribute("MW0",		mw.str());	// mw0			
			x_head.InsertEndChild(x_fapu);

			// Global translation
			comment.SetValue("Global Translation info for the whole face");
			x_head.InsertEndChild(comment);
			TiXmlElement x_trans("translation");		// translation element
			Vector3 trans = pFDP->getGlobalTranslation();
			std::strstream transx, transy, transz;
			transx << trans.x << '\0';
			transy << trans.y << '\0';
			transz << trans.z << '\0';
			x_trans.SetAttribute("x", transx.str());	// translation in x
			x_trans.SetAttribute("y", transy.str());	// translation in y
			x_trans.SetAttribute("z", transz.str());	// translation in z
			x_head.InsertEndChild(x_trans);

			// Global rotation
			comment.SetValue("Global Rotation info for the whole face");
			x_head.InsertEndChild(comment);
			TiXmlElement x_rot("rotation");					// rotation element
			Vector3 rot = pFDP->getGlobalRotationAxis();
			std::strstream rotx, roty, rotz, rotang;
			rotx << rot.x << '\0';
			roty << rot.y << '\0';
			rotz << rot.z << '\0';
			rotang << pFDP->getGlobalRotationAngle() << '\0';
			x_rot.SetAttribute("axis_x", rotx.str());		// rotation axis x
			x_rot.SetAttribute("axis_y", roty.str());		// rotation axis y
			x_rot.SetAttribute("axis_z", rotz.str());		// rotation axis z
			x_rot.SetAttribute("axis_angle", rotang.str());	// rotation angle
			x_head.InsertEndChild(x_rot);

	x_xfdp.InsertEndChild(x_head);

	// mesh and texture binds
	saveSource(x_xfdp);

	// now the fdp's...
	comment.SetValue("FDP points and required information for each of them starts here");
	x_xfdp.InsertEndChild(comment);
	
	const FDPItem* pItem;
	const FDP::FDPITEMS items = pFDP->getItems();
	FDP::FDPITEMS::const_iterator it = items.begin();
	while (it != items.end())
	{
		pItem = *it;
		++it;
		TiXmlElement x_item("fdp");
		x_item.SetAttribute("name", pItem->getName().c_str());
		x_item.SetAttribute("index", pItem->getIndex());
		x_item.SetAttribute("affects", pItem->getAffects().c_str());
			// Now the indices of AOI
			TiXmlElement x_indices("indices");
			std::strstream indices;
			const FDPItem::AOI_STORE aoi_data = pItem->getAOI();
			if(aoi_data.empty())
				continue;
			FDPItem::AOI_STORE::const_iterator it2 = aoi_data.begin();
			while (it2 != aoi_data.end())
			{
				indices << *it2 << " ";
				++it2;
			}
			indices << '\0';
			
			TiXmlText x_aoi(indices.str());
			x_indices.InsertEndChild(x_aoi);

			x_item.InsertEndChild(x_indices);

		
			const std::map<IInfluenceCalculator*, std::string> influence = pItem->getInfluenceCalculators();
			std::map<IInfluenceCalculator*, std::string>::const_iterator it = influence.begin();
			while(it != influence.end())
			{
				TiXmlElement x_influence("influence");
				std::strstream weight, fapID;
					// weight
					weight << it->first->getCoefficient() << '\0';
					x_influence.SetAttribute("weight",  weight.str());
					
					// fap ID
					fapID << it->first->getFapID() << '\0';
					x_influence.SetAttribute("fap", fapID.str());

					// type
					x_influence.SetAttribute("type", it->second);
				x_item.InsertEndChild(x_influence);
				++it;
			}

		x_xfdp.InsertEndChild(x_item);
	}
	doc.InsertEndChild(x_xfdp);

	doc.SaveFile(filename.c_str());
	return true;
}

/*!
	\bug When a mesh is UNloaded, we do not remove the textures that are bound to that mesh from
	the list of textures. We should.. Now only making a simple check here. In any case, this
	bug is not a cause for crash or misbehaviour, just bloats the configuration file.
*/
void FDPFile::saveSource(TiXmlElement& x_xfdp)
{
	TiXmlComment comment;
	TiXmlElement x_source("source");	// source information

		// add a description comment
		comment.SetValue("3D model (mesh) info");
		x_source.InsertEndChild(comment);		

        // add identity
        TiXmlElement x_entity("entity");
        x_entity.SetAttribute("alias", "Rest");
        x_entity.SetAttribute("category", "Expression");

		std::list<MeshInfo>::const_iterator it = m_meshList.begin();
		while (it != m_meshList.end())
		{
			TiXmlElement x_file("mesh");	// mesh element
			x_file.SetAttribute("file", it->file.c_str());
			x_file.SetAttribute("format", it->format.c_str());
			x_entity.InsertEndChild(x_file);
			++it;
		}
        x_source.InsertEndChild(x_entity);

	x_xfdp.InsertEndChild(x_source);

}

bool FDPFile::load(std::istream& input, FDP* pFDP)
{
	TiXmlDocument doc;
	std::string xmlString;
	// get the input stream contents to a string
	while (!input.eof())
	{
		std::string dummy;
		input >> dummy;
		xmlString.append(dummy);
	}

	// parse the xml string
	if(!doc.Parse(xmlString.c_str()))
		return false;

	return load(doc, pFDP);
}

bool FDPFile::load(const std::string& filename, FDP* pFDP)
{
	TiXmlDocument doc( filename.c_str() );
	if(!doc.LoadFile())
		return false;
	return load(doc, pFDP);
}

bool FDPFile::load(TiXmlDocument& doc, FDP* pFDP)
{
	m_pFDP = pFDP;
	
	// get to the body
	TiXmlNode* pNode = 0;
	pNode = doc.FirstChild( "xfdp" );
	assert( pNode );
	
	// and move in to the header
	pNode = pNode->FirstChild( "head" );
	assert( pNode );

	// load header
	loadHeader(pNode);

	// and move in to the source
	pNode = pNode->NextSibling( "source" );
	assert( pNode );
	if(!loadSource(pNode))
		return false;

	
	// make sure we skip everything and move to first FDP item in file
	pNode = pNode->NextSibling("fdp");
	while (pNode)
	{
		// load the fdp items
		loadFDPItem(pNode);
		pNode = pNode->NextSibling("fdp");
	}

	return true;
}

bool FDPFile::loadSource(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check

    const TiXmlNode* pChild = 0;
    pChild = pNode->FirstChild( "entity" );
    while (pChild)
    {
        MeshInfo info;
        const TiXmlNode* pMesh = 0;
        pMesh = pChild->FirstChild( "mesh" );
        while (pMesh)
        {
            const TiXmlElement* fileElement = 0;
            fileElement = pMesh->ToElement();
            assert(fileElement);
            const char* filename = fileElement->Attribute("file");
            if (!filename)
                info.file.assign("");
            else
                info.file.assign(filename);
            const char* fmt = fileElement->Attribute("format");
            if (!fmt)
                info.format.assign("");
            else
                info.format.assign(fmt);
            const char* path = fileElement->Attribute("path");
            if(!path)
                info.path.assign("");
            else
                info.path.assign(path);
            m_meshList.push_back(info);
            pMesh = pMesh->NextSibling( "mesh" );
        }
        pChild = pChild->NextSibling( "entity" );
    }
/*
    const TiXmlNode* pChild = 0;
    pChild = pNode->FirstChild( "entity" );
    while (pChild)
    {
        MeshInfo info;
        const TiXmlNode* pMesh = 0;
        pMesh = pNode->FirstChild( "mesh" );
        while (pMesh)
        {
            const TiXmlElement* fileElement = 0;
            fileElement = pChild->ToElement();
            assert(fileElement);
            const char* filename = fileElement->Attribute("file");
            info.file.assign(filename);
            const char* fmt = fileElement->Attribute("format");
            info.format.assign(fmt);
            const char* path = fileElement->Attribute("path");
            if(!path)
                info.path.assign("");
            else
                info.path.assign(path);

            m_meshList.push_back(info);
            pChild = pChild->NextSibling("mesh");
        }
    }
*/
	return true;
}

void FDPFile::loadHeader(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check
		
	const TiXmlNode* pChild = 0;
	pChild = pNode->FirstChild( "file" );
	if(pChild)
	{
		const TiXmlElement* fileElement = 0;
 		fileElement = pChild->ToElement();
		assert(fileElement);
		const char* version = fileElement->Attribute("version");
	}
	
	pChild = pNode->FirstChild( "model" );
	if(pChild)
	{
		const TiXmlElement* pModelEl = 0;
		pModelEl = pChild->ToElement();
		assert(pModelEl);
		// NOT PROCESSING FDP COUNT YET
		//	std::cout << "Number of FDP is: "<< modelElement->Attribute("fdpcount") << std::endl;
	}

	pChild = pNode->FirstChild( "fapu" );
	if(pChild)
	{
		const TiXmlElement* pFAPU = 0;
		pFAPU = pChild->ToElement();
		assert(pFAPU);
		float val = 0;
		const char* att = pFAPU->Attribute("ES0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setES0(val);
		}

		att = pFAPU->Attribute("IRISD0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setIRISD0(val);
		}
		att = pFAPU->Attribute("ENS0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setENS0(val);
		}
		att = pFAPU->Attribute("MNS0");
		if(att)
		{
			val = (float)atof(att);
			m_pFDP->setMNS0(val);
		}
		att = pFAPU->Attribute("MW0");
		if(att)
		{
			val = (float)atof(att);
			m_pFDP->setMW0(val);
		}
	}

	// load translation
	pChild = pNode->FirstChild( "translation" );
	if(pChild)
	{
		const TiXmlElement* pTrans = 0;
		pTrans = pChild->ToElement();
		Vector3 trans(0, 0, 0);
		const char* att = pTrans->Attribute("x");
		if (att)
			trans.x = (float)atof(att);
		att = pTrans->Attribute("y");
		if (att)
			trans.y = (float)atof(att);
		att = pTrans->Attribute("z");
		if (att)
			trans.z = (float)atof(att);

		m_pFDP->setGlobalTranslation(trans);
	}
	
	// load global rotation
	pChild = pNode->FirstChild( "rotation" );
	if(pChild)
	{
		const TiXmlElement* pRot = 0;
		pRot = pChild->ToElement();
		Vector3 rot(0, 0, 0);
		float ang = 0;
		const char* att = pRot->Attribute("axis_x");
		if (att)
			rot.x = (float)atof(att);
		att = pRot->Attribute("axis_y");
		if (att)
			rot.y = (float)atof(att);
		att = pRot->Attribute("axis_z");
		if (att)
			rot.z = (float)atof(att);
		att = pRot->Attribute("axis_angle");
		if (att)
			ang = (float)atof(att);

		m_pFDP->setGlobalRotation(AxisAngle(rot, ang));
	}

	
}
void FDPFile::loadFDPItem(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check
	const TiXmlElement* pFDP = 0;
	pFDP = pNode->ToElement();
	assert(pFDP);
	if(strcmp("fdp", pFDP->Value()) != 0)
		return;

	// process the attributes of this FDP
	std::string name(pFDP->Attribute("name"));
	// create a new item (will be deleted in dtor of FDP class)
	FDPItem* pItem = new FDPItem(name);
	// get the index
	int index = 0;
	pFDP->Attribute("index", &index);
	pItem->setControlPoint((unsigned short)index);

	// get the affected mesh
	const char* att = pFDP->Attribute("affects");
	if (att)
	{
		std::string aff(att);
		pItem->setAffects(aff);
	}

	// get the indices	
	const TiXmlNode* pChild = 0;
	pChild = pNode->FirstChild( "indices" );
	if(pChild)
	{
		const TiXmlText* pText = pChild->FirstChild()->ToText();
		if(pText) // no indices inside
		{
			std::string indices(pText->Value());
			std::string delim(" ");
		
			size_t n = indices.length();
			size_t start, stop;
	 
			start = indices.find_first_not_of(delim);
			while ((start >= 0) && (start < n)) 
			{
				stop = indices.find_first_of(delim, start);
				if ((stop < 0) || (stop > n)) 
					stop = n;
				
				pItem->addAOIIndex(atoi(indices.substr(start, stop - start).c_str()));
				start = indices.find_first_not_of(delim, stop+1);
			}
		}
	}

	// load the influences
	pChild = 0;
	while( pChild = pNode->IterateChildren("influence", pChild) )
	{
		const TiXmlElement* pDefItem = pChild->ToElement();
		if (pDefItem)
		{
			unsigned short fapID = 0;
			att = pDefItem->Attribute("fap");
			if(att)
				fapID = (unsigned short)atoi(att);
			
			float w = 0;
			att = pDefItem->Attribute("weight");
			if(att)
				w = (float)atof(att);
			att = pDefItem->Attribute("type");
			if(att)
			{
				IInfluenceCalculator* pInfluence = InfluenceCalculatorMaker::newInfluenceCalculator(att, w, fapID);
				if(pInfluence)
					pItem->addInfluenceCalculator(pInfluence, att);
			}
		}
	}
	
	m_pFDP->insertItem(pItem);
}

}  // namespace XFace