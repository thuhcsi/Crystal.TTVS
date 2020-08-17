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
	\file	ITexture.h
	\brief	Base class for texture mapping
*/

#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include "NamedObj.h"

namespace XEngine{
/*!
	\brief	Base class for texture mapping
	\author	Koray Balci
	\ingroup XEngine
	\version 1.0 (August 2002) 
			<br>1.1 (Jan 2003)

	Serves as a base class for texture mapping. Stores the texture name and ID,
	ITexture instances are created in ITextureLoader interface and registered
	to TextureManager using TextureManager::load() member.
	
	Each instance should be named whit a unique string and registered to TextureManager
	for consistent behaviour. 

	\sa Texture2D, ITextureLoader, TextureLoaderGL, TextureManager
*/
	class ITexture : public NamedObj
	{
	public:
		/*!
			Enumeration for the types of textures, currently there is only one, 
			but later, we will need 1D and 3D as well perhaps.
		*/
		enum TEXTURETYPE
		{
			TEXTURE2D		//!< 2D Texture type identifier
		};
		//! Returns the type of texture
		virtual TEXTURETYPE getTextureType() const = 0;
		//! Returns the texture id attached to this object
		const unsigned int getTextureID() const {return m_TexID;};
		//! Constructor
		ITexture(const std::string& name) : m_TexID(0), NamedObj(name) {};
		//! Destructor
		virtual ~ITexture(){};
		unsigned int m_TexID;
	};
}
#endif // ITEXTURE_H_
