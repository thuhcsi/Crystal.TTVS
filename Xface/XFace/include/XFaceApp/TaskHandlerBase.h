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
	\file		TaskHandlerBase.h
	\brief		Task handler base.
*/

#pragma once
#ifndef TASKHANDLERBASE_H_
#define TASKHANDLERBASE_H_

#include <string>
#include <queue>
#include "Task.h"

namespace XFaceApp{

/*!
	\brief	Task handler base
	\ingroup XFaceApp
	\author Koray Balci

	Task objects are issued and processed using this class. Current implementation has
	a Task queue (std::queue), and you can push new Tasks to the queue using 
	TaskHandlerBase::newTask method. Then, when TaskHandlerBase::processTask method is called, 
	the Task in queue is popped, and processed using TaskHandlerBase::dispatch method. 
	In TaskHandlerBase::dispatch (private & non-virtual method), depending on Task type, 
	the task is routed to appropriate handler method. If a time spanning task is being processed 
	when the system tries to process a new task, it automatically waits until the end of the 
	current task. That's why time spanning task handler methods return boolean values. All the 
	task handler methods are defined as virtual with empty bodies, which means this class, by 
	itself does nothing. You should never use this class directly, hence the destructor of the 
	class is protected.	This class only implements and hides the Task processing mechanism 
	from its clients.
	\sa ApplicationBase
*/
class TaskHandlerBase
{
	std::queue<Task> m_taskQueue;
	//! Dispatches Task.
	bool dispatch(const Task& task);
public:
	//! Post a new Task through here
	virtual void newTask(const Task& task);
	//! Process the Task queue
	virtual bool processTask(); 
	//! Discard all pending Task objects in the queue
	void discardPendingTasks();
protected:
	//! Pointer to current task that is being processed is stored for firing proper Notification(s)
	Task* m_pCurrTask;
	//!@{ Task handler with empty body.
	virtual void onRenderFrame() const {};
	virtual void onAdvanceFrame() const {};
	virtual bool onResumePlayback() {return true;};
	virtual void onPausePlayback(){};
	virtual void onStopPlayback() {};
	virtual void onRewindPlayback() const {};
	virtual bool onLoadFDP(const std::string& param1, const std::string& param2) {return true;}
	virtual bool onLoadFAP(const std::string& param) {return true;};
	virtual bool onLoadWAV(const std::string& param) {return true;};
	virtual bool onUpLoadFAP(const std::string& param) const {return true;};
	virtual bool onUpLoadAPML(const std::string& param1, const std::string& param2) {return true;};
	virtual bool onUpLoadFDP(const std::string& param){return true;};
	//!@}
	virtual ~TaskHandlerBase(void){};
};

} // namespace XFaceApp

#endif // TASKHANDLERBASE_H_