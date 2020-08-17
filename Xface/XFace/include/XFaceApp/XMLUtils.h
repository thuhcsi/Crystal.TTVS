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
/*! 
	\file		XMLUtils.h
	\brief		XML related functions.
*/

#pragma once
#ifndef XMLUTILS_H_
#define XMLUTILS_H_

#include <XFaceApp/Task.h>
#include <XFaceApp/Notification.h>

#include <string>

namespace XFaceApp{

/*!
	\brief XML related functions.
	\ingroup XFaceApp
	\author Koray Balci
	
	Various utility functions for XML go here. All members are static, this class
	exists just to avoid polluting global namespace.
*/
class XMLUtils
{
public:
	//! Creates the XML sentence for the Task task as a std::string
	static std::string xmlify(const Task& task);

	//! Creates the XML sentence for the Notification note as a std::string
	static std::string xmlify(const Notification& note);

	//! Creates the Notification object from the XML sentence passed as a std::string
	static Notification xmlToNotification(const std::string& xmlString);
	
	//! Creates the Task object from the XML sentence passed as a std::string
	static Task xmlToTask(const std::string& xmlString);
};

} // namespace XFaceApp

#endif // XMLUTILS_H_