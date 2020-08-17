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
 * The Original Code is Xface Core Library; Render List.
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

#include <XEngine/RenderList.h>
#include <XEngine/IRenderer.h>

namespace XEngine{

RenderList::RenderList(void)
{
}

RenderList::~RenderList(void)
{
	clearList();
}

void RenderList::renderList(const IRenderer* pRenderer)
{
	RLIST::iterator it = m_List.begin();
	while (it != m_List.end())
	{
		pRenderer->render(*it);
		++it;
	}
}

void RenderList::removeDrawable(const std::string& name)
{
	RLIST::iterator it = m_List.begin();
	while (it != m_List.end())
	{
		if( (*it)->getMeshName() == name)
		{
			m_List.remove(*it);
			break;
		}
		++it;
	}
}
}  // namespace XFace