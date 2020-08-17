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
 * The Original Code is Xface Core Library; Utility.
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
	\file	NamedObj.h
	\brief	Utility class for named classes.
*/

#ifndef NAMEDOBJ_H_
#define NAMEDOBJ_H_

#include <string>
namespace XEngine{

    /*!
        \brief Named classes base 
        \ingroup XEngine
        \author Koray Balci
        \version 1.0
        \date   Nov 2003

	    A class that serves so little.. Thought it would be a good idea
	    to not repeat adding get/set Name stuff everytime it is necessary for
	    a class, however, not so sure anymore, might be breaking some holly OO rule! 
	*/
	class NamedObj
	{
		std::string m_Name;
	protected:
	    //! Thou shall not create an instance of this class.
		NamedObj(const std::string& name) : m_Name(name) {}
		//! I know when protected, no need to make it virtual, so what?
		virtual ~NamedObj(void){}
	public:
		//! Accessor for Name prop.
		void setName(const std::string& name) {m_Name = name;};
		//! Accessor for Name prop.
		const std::string& getName() const {return m_Name;};
	};
}

#endif // NAMEDOBJ_H_
