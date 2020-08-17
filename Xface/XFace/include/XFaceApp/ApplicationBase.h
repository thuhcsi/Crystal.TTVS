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
	\file		ApplicationBase.h
	\brief		Base for application dev classes
*/

#pragma once
#ifndef APPLICATIONBASE_H_
#define APPLICATIONBASE_H_

#include <XFaceApp/TaskHandlerBase.h>
#include <XFaceApp/Notification.h>
#include <XFace/FaceBase.h>
#include <XFaceApp/ISound.h>
#include <XFaceApp/ITimer.h>
#include <XFace/IFapStream.h>
#include <XFaceApp/IApmlProcessor.h>

/*!
	\brief	Application related classes are under this namespace.
	\ingroup XFaceApp
	\author Koray Balci
	\version 1.0
	\date   May 2003
*/
namespace XFaceApp{

using namespace XFace;

/*!
	\brief	Base for application dev classes
	\ingroup XFaceApp
	\author Koray Balci
	
	Abstract base class for Xface related Task processing. Implements the empty Task handlers
	defined in base class TaskHandlerBase, but pure virtual create* methods also enforces
	derivation from this class. For basic use, task handlers in this class does the job, at 
	least Xface related job. For example, for adding GUI related things, you should 
	reimplement these task handlers, and call the implementations provided here from derived ones.
	So, the main idea is to provide the basic mechanism and hide the Xface related implementation
	while letting users to develop their own application using various available libraries and 
	technologies. 
*/
class ApplicationBase :	public TaskHandlerBase
{
	bool m_bBusy;
	bool m_bPausePlayback;
protected:
	FaceBase* m_pFace;
	ISound* m_pSound;
	ITimer* m_pTimer;
	IRenderer* m_pRenderer;
	IFapStream* m_pFapStream;
	IApmlProcessor* m_pFapGenerator;

	//!@{ Task Handler
	virtual void onRenderFrame() const;
	virtual void onAdvanceFrame() const;
	virtual bool onResumePlayback();
	virtual void onPausePlayback();
	virtual void onStopPlayback();
	virtual void onRewindPlayback() const;
	virtual bool onLoadFDP(const std::string& param1, const std::string& param2);
	virtual bool onLoadFAP(const std::string& param);
	virtual bool onLoadWAV(const std::string& param);
	virtual bool onUpLoadFAP(const std::string& param) const;
	virtual bool onUpLoadAPML(const std::string& param1, const std::string& param2);
	virtual bool onUpLoadFDP(const std::string& param);
	//!@}

	/*!
		You should provide an application specific (probably external library dependent)
		yield functionality which enables processing the task queue, while a task is 
		already being done. We use wxWidgets yield method to achieve this in XfacePlayer
		implementation (see wxFace class).
	*/
	virtual bool yield() = 0;
	//!	Creates an object implementing IApmlProcessor interface.
	virtual IApmlProcessor* createApmlProcessor() const = 0;
	//!	Creates an object implementing ITimer interface.
	virtual ITimer* createTimer() const = 0;
	//!	Creates an object implementing ISound interface.
	virtual ISound* createSound() const = 0;
	//!	Creates a FaceBase object (or a derived one)
	virtual FaceBase* createFace() const = 0;
	//!	Creates an object implementing XFace::IRenderer interface.
	virtual IRenderer* createRenderer() const = 0;
	//!	Creates an object implementing IFapStream interface.
	virtual IFapStream* createFapStream() const = 0;
	void fireNotification(const Notification::TaskStatus status) const;
	/*!
		Fires the Notification, implementation is probably TCP/IP related, 
		hence left out of core library and implemented in derived class in XfacePlayer.
	*/
	virtual void fireNotification(const Notification& note) const = 0;

	/*!
		Some GUI libraries (wxWidgets, SDL, MFC) for OpenGL windowing operation require
		pre and especially post processing for double buffering. Derived classes should 
		re-implement these empty methods properly
	*/
	virtual void renderBegin() const {};
	/*!
		Some GUI libraries (wxWidgets, SDL, MFC) for OpenGL windowing operation require
		pre and especially post processing for double buffering. Derived classes should 
		re-implement these empty methods properly
	*/
	virtual void renderEnd() const {};
	void synchronize(bool) const;
	ApplicationBase(void);
public:
	/*!
		Basic query mechanism. To be changed..
	*/
	enum QUERY{
		QUERY_FAPS_FINISHED, 
		QUERY_FAPS_LOADED
//		QUERY_BUSY
	};

	IApmlProcessor* const getFapGenerator() const {return m_pFapGenerator;}
	const FaceBase* const getFace() {return m_pFace;}
	virtual void muteAudio(bool bSnd);
	bool query(const QUERY& q);
	virtual bool init();
	virtual ~ApplicationBase(void);
};

}

#endif // APPLICATIONBASE_H_