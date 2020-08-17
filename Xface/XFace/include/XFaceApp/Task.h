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
	\file		Task.h
	\brief		Task related.
*/

#pragma once
#ifndef TASK_H_
#define TASK_H_


#include <string>
#include <map>
#include <deque>

namespace XFaceApp{

/*!
	\brief Dictionary of available Task's
	\ingroup XFaceApp
	\author Koray Balci
	
	TaskDictionary class stores the list of Task types, and serve other classes as a 
	dictionary to check. You can query the existance of a Task type, get the number of 
	parameters, and also add new Task types on the fly to extend the capabilities of
	the library. Note that, TaskDictionary is a singleton and a monolithic class with all 
	member functions being static, therefore you can only have one dictionary of tasks 
	per application instance.

	Here is the list of currently available Task types and their meanings;
	- \b RESUME_PLAYBACK \n
	Starts the playback of the currently loaded FAP file. 
	\sa ApplicationBase::onResumePlayback
	- \b STOP_PLAYBACK \n
	Stops the playback and rewinds the FAP file to the beginning. 
	\sa ApplicationBase::onStopPlayback
	- \b PAUSE_PLAYBACK \n
	Pauses the playback.
	\sa ApplicationBase::onPausePlayback
	- \b REWIND_PLAYBACK \n
	Rewinds the playback. 
	\sa ApplicationBase::onRewindPlayback
	- \b RENDER_FRAME \n
	Renders the current frame.
	\sa ApplicationBase::onRenderFrame
	- \b ADVANCE_FRAME \n
	Advances the frame
	\sa ApplicationBase::onAdvanceFrame
	- \b LOAD_FAP \n
	Loads FAP
	\sa ApplicationBase::onLoadFAP
	- \b LOAD_WAV \n
	Loads WAV
	\sa ApplicationBase::onLoadWAV
	- \b LOAD_FDP \n
	Loads FDP
	\sa ApplicationBase::onLoadFDP
	- \b UPLOAD_FAP \n
	UpLoads FAP
	\sa ApplicationBase::onUpLoadFAP
	- \b UPLOAD_FDP \n
	UpLoads FDP
	\sa ApplicationBase::onUpLoadFDP
	- \b UPLOAD_APML \n
	Uploads APML script
	\sa ApplicationBase::onUpLoadAPML
	- \b UPLOAD_WAV \n
	Not used currently
	\todo Should implement a way to upload binary files first to implement this (UPLOAD_WAV) Task, note that it 
	is also necessary to be able to upload binary (compressed) FAP files.
	

*/
class TaskDictionary
{
	typedef std::map<std::string, int> DictMap;
	DictMap m_dictionary;
	TaskDictionary();
	static TaskDictionary s_dict;
public:
	/*!
		\param name Task name
		\return true if it is a Task
	*/
	static bool isTask(const std::string& name)
	{
		DictMap::const_iterator it = s_dict.m_dictionary.find(name);
		if(it != s_dict.m_dictionary.end())
			return true;
		return false;
	}
	/*!
		\param name Task name
		\return number of parameters
	*/
	static int queryParamCount(const std::string& name)
	{
		DictMap::const_iterator it = s_dict.m_dictionary.find(name);
		if(it != s_dict.m_dictionary.end())
			return it->second;
		return -1;
	}
	/*!
		Used to register a new Task type.
		\param name Task name
		\param nParam Number of parameters
	*/
	static void registerTask(const std::string& name, int nParam)
		{s_dict.m_dictionary.insert(std::make_pair(name, nParam));}
};

/*!
	\brief Task abstraction
	\ingroup XFaceApp
	\author Koray Balci

	For controlling Xface, clients have one of two ways. First is the use of classes implemented
	as usual, and the second is using the Task mechanism. We define a set of Task types, and this 
	class stores the properties of these tasks. Every Task object has a name, set of parameters, 
	an ID and owner ID. Name is a string representing the actual role of the object. You can find
	more information about Task names/types in TaskDictionary class documentation. Every Task object
	has also a set of string parameters depending on the Task type it belongs to. ID is supposed to
	be a unique number representing the Task instance. It might have been assigned automatically 
	during construction, but we chose it to be assigned externally for the time being. Owner ID 
	represents the thread/application which has created and issued this task to Xface. This value 
	is used for feedback notification (more on this: Notification class) mechanism.

	Here is a sample of usage from XfaceClient, where we issue a "resume playback" task through TCP/IP;
	\code
	// we create a task with the client Id and a unique task ID.
	Task task("RESUME_PLAYBACK", m_clientId, ++m_taskCount + 1000000*m_clientId);
	// following function translates the Task to XML string 
	std::string msg = XMLUtils::xmlify(task);
	// Here we send the string through network
	m_pSocket->Write(msg.c_str(), msg.size() + 1);
	\endcode

	On the other hand, if this is a standalone application, or we have direct access to 
	XFaceApp::TaskHandlerBase derived class, we can issue the Task as follows;
	\code
	Task playtask("RESUME_PLAYBACK");
	m_pApp->newTask(playtask);	// m_pApp is a pointer to a TaskHandlerBase derived class instance.
	\endcode

	Note that, we do not have to (and in fact should not) pass owner id and task id for the Task object, 
	because, it is defaulted to 0, and this way we prevent creating/sending Notification messages.
	After all, the latter use is only for the thread that has access to the TaskHandlerBase class. A class
	derived from TaskHandlerBase, tailored for your own purposes can implement event notification 
	mechanism much more efficiently. 
	
	\b Future: In the future versions, we can add plenty of things to this mechanism. Parameters 
	can be in types other than string. ID can be assigned automatically and invisibly, however
	this can create a set of new problems, not sure.. And finally, we can implement a whole class
	hierarchy instead of using strings for naming Task types. For the time being, the system is 
	sufficient for our needs. 
	
*/
class Task
{
	std::string m_name;
	std::deque<std::string> m_params;
	unsigned short m_ownerID;
	unsigned int m_id;
public:
	//! Clears the parameters
	void clearParams() {m_params.clear();}
	//! Returns the number of currently pushed parameters
	unsigned short getParamCount() const {return (unsigned short)m_params.size();}
	//! Sets the name, first checks the existence of the Task by calling TaskDictionary::isTask
	void setName(const std::string& name);
	//! Returns the name of the Task.
	const std::string& getName() const {return m_name;}
	//! Pushes a new parameter string to the parameter collection.
	void pushParameter(const std::string& param) {m_params.push_back(param);}
	//! Returns the parameter indexed by id.
	std::string getParameter(unsigned int id) const;
	//! Returns the owner ID
	unsigned short getOwnerID() const {return m_ownerID;}
	//! Sets the owner ID
	void setOwnerID(unsigned short id = 0) {m_ownerID = id;}
	//! Gets Task ID
	unsigned int getID() const {return m_id;}
	//! Sets Task ID
	void setID(unsigned int id = 0) {m_id = id;}
	/*!
		Note that during construction, task name is checked against TaskDictionary,
		so, if an invalid task name is inserted, the name of the task is empty string.
		If you are not sure about the validity of the task name, try calling 
		TaskDictionary::isTask beforehand.
	*/
	Task(const std::string& name, unsigned short owner=0, unsigned int id=0) 
		: m_ownerID(owner), m_id(id), m_name("") {setName(name);}
};

} // namespace XFaceApp

#endif // TASK_H_