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
 * The Original Code is Xface Core Library; XFaceApp.
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

#include <XFaceApp/XMLUtils.h>

#if defined(USE_EGL)
    #ifdef _DEBUG
    #pragma message("		Adding TinyXML STL lib (WinCE Debug)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MDd compiler switch...")
    #pragma comment(lib, "tinyxmlSTL_CE")
    #else
    #pragma message("		Adding TinyXML STL lib (WinCE Release)...")
    #pragma message("		Note: TinyXMLSTL should be compiled externally using /MD compiler switch...")
    #pragma comment(lib, "tinyxmlSTL_CE")
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

#include <tinyxml.h>
#include <strstream>
#include <fstream>
#include <sstream>

namespace XFaceApp{
std::string XMLUtils::xmlify(const Task& task)
{
	std::stringstream retVal;
	retVal << "<task name=\"";
	retVal << task.getName();
	retVal << "\" ownerId=\"";
	retVal << task.getOwnerID();
	retVal << "\" taskId=\"";
	retVal << task.getID();
	retVal << "\">";
	retVal << std::endl;
	for(unsigned short i = 0; i < task.getParamCount(); ++i)
	{
		retVal << "<param>";
		retVal << task.getParameter(i);
		retVal << "</param>";
		retVal << std::endl;
	}
	retVal << "</task>";
	retVal << '\0';

	return retVal.str();
}

std::string XMLUtils::xmlify(const Notification& note)
{
	std::stringstream retVal;
	//retVal << "<xface>";
	retVal << "<notify name=\"";
	retVal << note.getName();
	retVal << "\" ownerId=\"";
	retVal << note.getOwnerID();
	retVal << "\" taskId=\"";
	retVal << note.getTaskID();
	retVal << "\" status=\"";
	retVal << note.getStatus();
	retVal << "\"/>";

	return retVal.str();
}

Notification XMLUtils::xmlToNotification(const std::string& xmlString)
{
	TiXmlElement el("");
	std::istrstream xmlStream(xmlString.c_str());
	xmlStream >> el;
	std::string identifier(el.Value());
	if(identifier != "notify")
		return Notification::error();

	const char* name = el.Attribute("name");
	if(!name)	// if no name exists, sth is seriously wrong, return immediately!
		return Notification::error();
	    
	unsigned short ownerId = 0;
	unsigned int taskId = 0;
	int dummy;
	// get OwnerID
	int ret = el.QueryIntAttribute("ownerId", &dummy);
	if(ret == TIXML_SUCCESS)
		ownerId = (unsigned short) dummy;

	// get Task ID
	ret = el.QueryIntAttribute("taskId", &dummy);
	if(ret == TIXML_SUCCESS)
		taskId = (unsigned int) dummy;

	Notification note(name, ownerId, taskId);
    
	// get status
	ret = el.QueryIntAttribute("status", &dummy);
	if(ret == TIXML_SUCCESS)
		note.setStatus((Notification::TaskStatus) dummy);

	return note;
}
/*!
	Bug fix: changed the code
	\code
	while( grand_child = child->IterateChildren( grand_child ) )
		param << *grand_child;	// strange conversion, changing to above method for this reason
	\endcode

	to
	\code
	while( grand_child = child->IterateChildren( grand_child ) )
		param2 = grand_child->ToText()->Value();
	\endcode
	because end of line characters inside the FAP file are converted to some character in TinyXML << 
	operator, and this prevents istrstream object created later on to move properly in the string
	that has the FAP file contents.

	Perhaps the one alternative solution is to use TinyXML as well during creation of the XML string.
*/
Task XMLUtils::xmlToTask(const std::string& xmlString)
{
	Task task("", 0);	// don't worry, we will fill inside soon..

	// spaces are especially important for APML, because of a BUG in APML processor!
	TiXmlBase::SetCondenseWhiteSpace(false);
		
	TiXmlElement el("");
	std::istrstream xmlStream(xmlString.c_str());
	xmlStream >> el;
	std::string identifier(el.Value());
	if(identifier != "task")
		return task;

	const char* name = el.Attribute("name");
	if(name)	// if no name exists, sth is seriously wrong, return immediately!
		task.setName(name);
	int dummy;
	// get OwnerID
	int ret = el.QueryIntAttribute("ownerId", &dummy);
	if(ret == TIXML_SUCCESS)
		task.setOwnerID((unsigned short) dummy);

	// get Task ID
	ret = el.QueryIntAttribute("taskId", &dummy);
	if(ret == TIXML_SUCCESS)
		task.setID((unsigned int) dummy);

	// iterate through all parameters
	TiXmlNode* child = 0;
	while( child = el.IterateChildren( "param", child ) )
	{
		// param might contain xml inside too, like in apml or fdp
		// or not like in fap, so we should act accordingly
		std::ostringstream param;
		std::string param2;
		TiXmlNode* grand_child = 0;
		while( grand_child = child->IterateChildren( grand_child ) )
		{
			if(grand_child->Type() == TiXmlNode::TEXT)
				param2 = grand_child->ToText()->Value();
			else
				param2 << *grand_child;	// conversion for newline char breaks the code, so added above if for FAP file parsing
		}

		task.pushParameter(param2);
	}

	TiXmlBase::SetCondenseWhiteSpace(true);
	
	return task;
}
} //namespace XFaceApp
