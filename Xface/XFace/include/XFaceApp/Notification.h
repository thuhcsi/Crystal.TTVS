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
/*! 
	\file		Notification.h
	\brief		Notification message to server for remote communication.
*/
#pragma once

#include <XFaceApp/Task.h>
#include <string>

namespace XFaceApp{

/*!
	\brief Notification message to server for remote communication.
	\ingroup XFaceApp
	\author Koray Balci
	\version 1.0
	\date   June 2004

	Notification objects are used for communication between the client and the server. 
	Server always sends	clients messages as Notification objects translated to XML strings.
	Every Notification object represents the status of a Task object (only ERROR notification
	is a small exception to that). So, owner and task ID properties are the same as of Task object's.
*/
class Notification
{
public:
	/*!
		Status/Fate of the task can be one of these.
	*/
	enum TaskStatus{
		kUnknown = 0,	//!< Who knows what happened to the poor task, used for initialization only/usually
		kQueued,		//!< Acknowledgement notification sent immediately after the Task is received & queued
		kStarted,		//!< Task is started, notify the Task owner
		kFinished,		//!< Task is finished, notify the Task owner
		kDiscarded,		//!< Task is discarded (very unprobable) by some overriding task.
		kInProgress,	//!< Client querried the task, and it is actually in progress at the moment.
		kInQueue,		//!< Client querried the task, and it is still in task queue.
		kError			//!< Something went wrong somewhere.
	};
private:
	unsigned short m_ownerId;
	unsigned int m_taskId;
	std::string m_name;
	TaskStatus m_status;
public:
	//! In case of unknown errors, we use this canned method that creates an error notification.
	static Notification error() {
		Notification note("ERROR");
		note.setStatus(kError);
		return note;
	}
	//!@{Accessor
	void setStatus(TaskStatus status) {m_status = status;}
	TaskStatus getStatus() const {return m_status;}
	unsigned short getOwnerID() const {return m_ownerId;}
	unsigned long getTaskID() const {return m_taskId;}
	const std::string& getName() const {return m_name;}
	//!@}

	//! Creates a Notification with default values for the given name as parameter.
	Notification(const std::string& name, unsigned short owner=0, unsigned int task=0);		
	//! Creates Notification for the given Task.
	Notification(const Task& task, const TaskStatus& status);
};

}// namespace XFaceApp