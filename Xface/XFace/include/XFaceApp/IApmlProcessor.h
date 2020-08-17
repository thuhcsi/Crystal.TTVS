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
 * The Original Code is XfaceApp Application Library.
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
	\file		IApmlProcessor.h
	\brief		APML caller interface
*/

#pragma once
#ifndef IAPMLPROCESSOR_H_
#define IAPMLPROCESSOR_H_


#include <string>
#include "TaskHandlerBase.h"

namespace XFaceApp{

/*!
	\brief APML caller interface
	\ingroup XFaceApp
	\author Koray Balci
	Provides the interface for calling APML script processor.
*/
class IApmlProcessor
{
protected:
	IApmlProcessor(void){}
public:
	/*! Processes the APML string 
		\param apmlString APML contents
		\param lang Language, "eng" for English, "it" for Italian
	*/
	virtual bool process(const std::string& apmlString, const std::string& lang) = 0;
	//! Sets the parameters to the APML proceesor executable
	virtual void setParameters(const std::string& params) = 0;
	//! Sets the path to the APML proceesor executable
	virtual void setPath(const std::string& path) = 0;
	//! Gets the path to the APML proceesor executable
	virtual const std::string& getPath() const = 0;
	//! Sets the name of the APML proceesor executable
	virtual void setExecutable(const std::string&) = 0;
	//! Gets the name of the APML proceesor executable
	virtual const std::string& getExecutable() const = 0;
	//! Gets the name of the APML proceesor parameters
	virtual const std::string& getParameters() const = 0;
	
	virtual ~IApmlProcessor(void){}
};

} // namespace XFaceApp

#endif // IAPMLPROCESSOR