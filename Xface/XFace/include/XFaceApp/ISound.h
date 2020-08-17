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
	\file		ISound.h
	\brief		Sound player interface
*/

#pragma once
#ifndef ISOUND_H_
#define ISOUND_H_

#include <string>

namespace XFaceApp{

	/*!
		\brief Sound player interface
		\ingroup XFaceApp
		\author Koray Balci
		Provides the interface for sound playback operation.
	*/
	class ISound
	{
	public:
		virtual bool loadWAV(const std::string& filename) = 0;
		virtual void jump(float percentage) = 0;
		virtual void mute(bool) = 0;
		virtual void unload() = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;
		virtual ~ISound(void){}
	};
}

#endif // ISOUND_H_