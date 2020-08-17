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
	\file	ITextureLoader.h
	\brief	Base class for texture loading to any API.
*/

#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <string>
#include "ITexture.h"

namespace XEngine{
/*!
	\brief	Base class for texture loading
	\ingroup XEngine
	\author	Koray Balci
	\version 1.0
	\date   August 2002

	Serves as a base class for texture loading. Note that, loading a texture is an 
	API (D3D/OpenGL) dependant operation. To ensure API independence, we use this 
	pure abstract base class.
*/
class ITextureLoader  
{
public:
	//! UnLoads a texture attached to an ITexture object.
	virtual void unLoad(const ITexture* pTexture) = 0;
	//! Loads the texture
	virtual bool load(const std::string& filename, ITexture* pTexture) = 0;
	//! Default constructor
	ITextureLoader(){};
	//! Destructor
	virtual ~ITextureLoader(){};
};
} // namespace XFace
#endif // TEXTURELOADER_H_
