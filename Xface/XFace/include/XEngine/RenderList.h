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
 * The Original Code is Xface Core Library.
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
	\file	RenderList.h
	\brief	Render list storing Drawables and calling proper IRenderer methods.
*/
#ifndef RENDERLIST_H_
#define RENDERLIST_H_

#include <list>
#include "Drawable.h"
#include "IRenderer.h"


namespace XEngine{
/*!
	\brief Render list storing Drawables and calling proper IRenderer methods.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003

	This class keeps a list of pointers to Drawable instances. Once renderList() 
	method is called, contents of the list are sent to IRenderer sequentially.
*/
class RenderList
{
	typedef std::list<const Drawable*> RLIST;
	RLIST m_List;
public:
	//! Adds a Drawable pointer to the list.
	void addDrawable(const Drawable* item){m_List.push_back(item);};
	//! Removes a Drawable with given name.
	void removeDrawable(const std::string& name);
	
	//! Clears the list.
	void clearList(){m_List.clear();};
	//! Sends the contents of the list to IRenderer sequentially.
	void renderList(const IRenderer* pRenderer);

	RenderList(void);
	~RenderList(void);
};
}
#endif
