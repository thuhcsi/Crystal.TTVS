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
	\file		ITimer.h
	\brief		Timer interface
*/
#pragma once
#ifndef ITIMER_H_
#define ITIMER_H_

namespace XFaceApp{

	/*!
		\brief Timer interface
		\ingroup XFaceApp
		\author Koray Balci
		Provides timer interface for animation synchronization.
	*/
	class ITimer
	{
	public:
		virtual unsigned long start() = 0;
		virtual void wait(unsigned long) = 0;
		virtual unsigned long getElapsedTime(bool mark) = 0;
		virtual unsigned long getTotalElapsedTime(bool mark) = 0;
		ITimer(void){}
		virtual ~ITimer(void){}
	};

} // namespace XFaceApp

#endif // ITIMER_H_