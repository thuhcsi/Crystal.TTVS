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

#include <XFaceApp/ApplicationBase.h>
#include <fstream>
#include <iostream>
#include <strstream>
#include <cassert>

namespace XFaceApp{ 
	
ApplicationBase::ApplicationBase(void) : m_pFace(0), m_pFapStream(0), m_pTimer(0), m_pRenderer(0), m_pSound(0), 
	m_pFapGenerator(0), m_bPausePlayback(true), m_bBusy(false)
{
}

ApplicationBase::~ApplicationBase(void)
{
	delete m_pFace;
	m_pFace = 0;
	delete m_pFapStream;
	m_pFapStream = 0;
	delete m_pTimer;
	m_pTimer = 0;
	delete m_pSound;
	m_pSound = 0;
	delete m_pRenderer;
	m_pRenderer = 0;
	delete m_pFapGenerator;
	m_pFapGenerator = 0;
}

/*!
	Provides basic query mechanism. 
	\todo The mechanism can be improved and reverted to Task/Notification type easily. 
	Or at least we can add a new Task type for QUERY and implement handlers. 
*/
bool ApplicationBase::query(const QUERY& q)
{
	switch(q)
	{
	case QUERY_FAPS_FINISHED:
		assert(m_pFapStream);
		return m_pFapStream->isEnd();
		break;
	case QUERY_FAPS_LOADED:
		assert(m_pFapStream);
		return m_pFapStream->isOpen();
		break;
	//case QUERY_BUSY:
	//	return m_bBusy;
	}

	return false;
}

/*!
	Pure virtual create* methods are called here to create necessary objects and stored in 
	interface pointers. This way, we provide the mechanism but developers might be interested/need
	using platform dependent or external library dependent classes, or just improve the already 
	supplied classes in Xface.

	Sound, Xface::FaceBase, timer, renderer, fap stream, APML processor objects are created.
*/
bool ApplicationBase::init()
{
	m_pSound = createSound();
	m_pTimer = createTimer();
	m_pFace	 = createFace();
	m_pRenderer = createRenderer();
	m_pFapStream = createFapStream();
	m_pFapGenerator = createApmlProcessor();

	return true;
}

/*!
	Mutes the audio. Good for testing purposes during development!
*/
void ApplicationBase::muteAudio(bool bSnd)
{
	m_pSound->mute(bSnd);
}

/*!
	Renders the current frame. 
	No Notification is fired.
*/
void ApplicationBase::onRenderFrame() const
{
	assert(m_pFace && "ApplicationBase::onRenderFrame()");
	
	renderBegin();
		m_pFace->render();
	renderEnd();
/*	
	// No need to fire notification for this one
	fireNotification(Notification::kFinished);
*/
}

/*!
	Advances the FAP frame, and updates the deformation.
	No Notification is fired.
*/
void ApplicationBase::onAdvanceFrame() const
{
	assert(m_pFapStream && m_pFace && "ApplicationBase::onAdvanceFrame()");

	const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
	if(!FAPs.empty())
	{
		m_pFace->update(FAPs);
		m_pFapStream->next();
	}
/*	
	// No need to fire notification for this one
	fireNotification(Notification::kFinished);
*/
}

/*!
	Starts/Resumes the playback of FAP stream and the WAV file and sends back 
	Notification::kFinished to the client. On error, Notification::kError
	is returned. Also yields the message pump for checking if there is a pause
	or stop Task arrived or not. If by chance, another resume playback Task is 
	in the task queue, it is not executed until tht current task finishes.
*/
bool ApplicationBase::onResumePlayback()
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	if(!m_pFapStream->isOpen())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;	// if no fap, sth wrong, skip this Task (returning true)
	}
	m_bBusy = true;

	// Notification
	if(m_pCurrTask->getName() == "RESUME_PLAYBACK")
		fireNotification(Notification::kStarted);
	
	synchronize(true);
	if(m_bPausePlayback)
		m_pSound->pause();
	
	m_bPausePlayback = false;
	do{
		onAdvanceFrame();
		onRenderFrame();
		synchronize(false);
		if(yield())
			processTask();
	}
	while(!m_pFapStream->isEnd() && !m_bPausePlayback);

	m_bBusy = false;

	if(!m_bPausePlayback)
	{
		onStopPlayback();
		m_bPausePlayback = true;
	}

	// Notification
	if(m_pCurrTask->getName() == "RESUME_PLAYBACK")
		fireNotification(Notification::kFinished);
	
	return true;
}

/*!
	Pauses the playback and fires Notification::kFinished.
*/
void ApplicationBase::onPausePlayback()
{
	if (m_pFapStream->getCurrentFAPId() != 0)
	{
		if(!m_bPausePlayback)	// pausing
		{
			m_pSound->pause();
			m_bPausePlayback = true;
		}
		else					// resuming
			onResumePlayback();
	}

	// Notification
	if(m_pCurrTask->getName() == "PAUSE_PLAYBACK")
		fireNotification(Notification::kFinished);
}

/*!
	Stops and rewinds the FAP stream and the WAV file to beginning and fires
	Notification::kFinished.
*/
void ApplicationBase::onStopPlayback()
{
	m_bPausePlayback = true;
	m_pSound->stop();
	m_pFapStream->rewind();
	const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
	if(!FAPs.empty())
	{
		m_pFace->update(FAPs);
		renderBegin();
			m_pFace->render();
		renderEnd();		
	}
	
	// Notification
	if(m_pCurrTask->getName() == "STOP_PLAYBACK")
		fireNotification(Notification::kFinished);
}

/*!
	Rewinds the FAP stream and the WAV file to beginning and fires
	Notification::kFinished.
*/
void ApplicationBase::onRewindPlayback() const
{
	if(m_pFapStream->isOpen())
		m_pFapStream->rewind();	
	m_pSound->stop();

	// Notification
	if(m_pCurrTask->getName() == "REWIND_PLAYBACK")
		fireNotification(Notification::kFinished);
}

/*!
	Loads the FDP file from the server (XfacePlayer) harddisk.
	\param param1 is the filename
	\param param2 is the path
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadFDP(const std::string& param1, const std::string& param2)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1 == "") // param2 may be empty..
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// Notification
	if(m_pCurrTask->getName() == "LOAD_FDP")
		fireNotification(Notification::kStarted);

	delete m_pFace;
	m_pFace = new FaceBase;
	m_pFace->init(param1, param2);	

	// Notification
	if(m_pCurrTask->getName() == "LOAD_FDP")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	Loads the FAP file from the server (XfacePlayer) side. 
	\param param is the filename
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadFAP(const std::string& param)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_pCurrTask->getName() == "LOAD_FAP")
		fireNotification(Notification::kStarted);

	std::ifstream input(param.c_str());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	if(!m_pFapStream->open(input, m_pFace->getFAPU()))
		std::cout << "Unable to load FAP file: " << param << "\n";
	input.close();

	// Notification
	if(m_pCurrTask->getName() == "LOAD_FAP")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	Loads the WAV file from the server (XfacePlayer) disk. 
	\param param is the filename
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadWAV(const std::string& param)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_pCurrTask->getName() == "LOAD_WAV")
		fireNotification(Notification::kStarted);

	m_pSound->loadWAV(param);
	// Notification
	if(m_pCurrTask->getName() == "LOAD_WAV")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	\param param is the FAP contents
*/
bool ApplicationBase::onUpLoadFAP(const std::string& param) const
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_FAP")
		fireNotification(Notification::kStarted);

	std::istrstream input(param.c_str(), (std::streamsize)param.size());
	std::istream mem_input(input.rdbuf());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	if(!m_pFapStream->open(mem_input, m_pFace->getFAPU()))
	{
		std::cout << "Unable to load FAP file: " << param << "\n";
		fireNotification(Notification::kError);
	}
	//input.close();

	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_FAP")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	Receives an FDP file from client.
	\param param is the filename
	\note It's not a good idea to send fdp files over the newtwork, unless accompaniying model and texture files
	are existent in server side, with the correct paths. ANyway, if everything is correctly in place, all will
	work fine. You should think such that, the fdp file is in the working directory of the server, and the path to 
	art files in fdp file are set accordingly.
*/
bool ApplicationBase::onUpLoadFDP(const std::string& param)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_FDP")
		fireNotification(Notification::kStarted);

	delete m_pFace;
	m_pFace = new FaceBase;
	//use string stream version for FaceBase::init
	std::istrstream fdpXML(param.c_str());
	m_pFace->init(fdpXML);

	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_FDP")
		fireNotification(Notification::kFinished);

	// Render one frame, so that changes are visible
	onRenderFrame();
	return true;
}

/*!
	Processes the APML string passed (from remote client possibly). Current 
	IAPMLProcessor implementation (Expl2fapProcessor class) also loads the FAP 
	and WAV file produced automatically by issuing proper Tasks.
	\param param1 is the APML string to process
	\param param2 is the language option ("eng" for English, "it" for Italian, default is "it")
	Notification::kError is sent on error and Notification::kStarted is sent on start and 
	Notification::kFinished when finished, to the client.
*/
bool ApplicationBase::onUpLoadAPML(const std::string& param1, const std::string& param2)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1 == "")	// param2 might be empty
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_APML")
		fireNotification(Notification::kStarted);

	// actual processing
	if(!m_pFapGenerator->process(param1, param2))
	{
		fireNotification(Notification::kError);
		return true;
	}

	// load the fap & wav files
	std::string path = m_pFapGenerator->getPath();
	onLoadFAP(path + "TempFile.fap");
	onLoadWAV(path + "TempFile.wav");
	
	// Notification
	if(m_pCurrTask->getName() == "UPLOAD_APML")
		fireNotification(Notification::kFinished);

	return true;
}

void ApplicationBase::fireNotification(const Notification::TaskStatus status) const
{
	if(m_pCurrTask->getOwnerID())	// if it is not this thread who scheduled the task
	{
		Notification note(*m_pCurrTask, status);
		fireNotification(note);
	}
}
/*!
	Synchronizes the animation to frames per seconds required by current FAP
	being played back. 
	\todo Check this function, not tested extensively. Especially in debug mode, we seem to have 
	some problem, should skip the frames, but not as it seems, lagging animation. 
*/
void ApplicationBase::synchronize(bool bStart) const
{
	static unsigned int frame_count = 0;
	static unsigned int missed_frames = 0;
	static unsigned int frame_time = 1000 / m_pFapStream->getFPS();
	
	// this means, we are beginning playback now (first frame is about to be played)
	if(bStart)
	{
		m_pTimer->start(); 
		// do init
		missed_frames = 0;
		frame_count = 0;
		frame_time = 1000 / m_pFapStream->getFPS(); // might have changed fap file
	}
	
	unsigned int elapsed_frames = m_pTimer->getTotalElapsedTime(false) / frame_time;

	// if slow, catch up
	while(elapsed_frames > frame_count)
	{
		frame_count++;
		missed_frames++;
		std::cout << "missed frames: " << missed_frames << std::endl;
	}

	// if fast, slow down
	if( m_pTimer->getElapsedTime(false) < frame_time)
		m_pTimer->wait(frame_time - m_pTimer->getElapsedTime(false));

	frame_count++;
	// don't forget to record the new timestamp
	m_pTimer->getElapsedTime(true);
}
}

