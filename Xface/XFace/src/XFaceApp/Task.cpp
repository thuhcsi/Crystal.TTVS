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

#include <XFaceApp/Task.h>
#include <iostream>

namespace XFaceApp{

TaskDictionary TaskDictionary::s_dict;
/*!
	Dictionary is created here. Note that constructor is called only once per thread.
*/
TaskDictionary::TaskDictionary()
{
	m_dictionary.insert(std::make_pair("RESUME_PLAYBACK", 0));
	m_dictionary.insert(std::make_pair("STOP_PLAYBACK", 0));
	m_dictionary.insert(std::make_pair("PAUSE_PLAYBACK", 0));
	m_dictionary.insert(std::make_pair("REWIND_PLAYBACK", 0));

	m_dictionary.insert(std::make_pair("RENDER_FRAME", 0));
	m_dictionary.insert(std::make_pair("ADVANCE_FRAME", 0));

	m_dictionary.insert(std::make_pair("LOAD_FAP", 1));
	m_dictionary.insert(std::make_pair("LOAD_WAV", 1));
	m_dictionary.insert(std::make_pair("LOAD_FDP", 2));

	m_dictionary.insert(std::make_pair("UPLOAD_FAP", 1));
	m_dictionary.insert(std::make_pair("UPLOAD_WAV", 1));
	m_dictionary.insert(std::make_pair("UPLOAD_FDP", 1));
	m_dictionary.insert(std::make_pair("UPLOAD_APML", 2));
}

/*!
	Gets the parameter indexed at id. If an invalid (out of bounds) index is passed
	an empty string is returned. 
*/
std::string Task::getParameter(unsigned int id) const
{
	std::string ret = "";
	try{
		ret = m_params.at(id);
	}
	catch(std::exception &exc){
		std::cerr << exc.what() << "Task::getParameter" << id << std::endl;
	}
	
	return ret;
}

void Task::setName(const std::string& name) 
{
	if(TaskDictionary::isTask(name))
		m_name = name;
}

} // namespace XFaceApp