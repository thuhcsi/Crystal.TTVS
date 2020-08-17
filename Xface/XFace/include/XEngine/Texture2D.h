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
	\file	Texture2D.h
	\brief	2D texture mapping
*/

#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include "ITexture.h"

namespace XEngine{
	/*!
		\brief	2D texture mapping
		\ingroup XEngine
		\author	Koray Balci
		\version 1.0
		\date   August 2002

		2D (ordinary) texture mapping parameters are stored here.

		Here is how it is used in RendererGL using OpenGL;
		\code
		// Get a pointer to base class for the texture;
		Drawable* pDrawable; // A valid instance
		...
		const ITexture* pTexture = TextureManager::getInstance()->getTexture(pDrawable->getTexName());
		// if it is a 2D texture
		if(pTexture->getTextureType() == ITexture::TEXTURE2D)
		{
			glEnable(GL_TEXTURE_2D);	
			glBindTexture(GL_TEXTURE_2D, pTexture->getTextureID());
		}
		\endcode
	*/
	class Texture2D : public ITexture  
	{
	public:
		//! Returns the type of the texture. (TEXTURE2D)
		TEXTURETYPE getTextureType() const {return TEXTURE2D;};
		//! Default Constructor
		Texture2D(const std::string& name) : ITexture(name){};
		//! Destructor
		virtual ~Texture2D(){};
	};
}
#endif // XTEXTURE2D_H_
