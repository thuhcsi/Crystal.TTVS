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
	\file	TextureLoaderGL.h
	\brief	Loads the textures into OpenGL
*/

#ifndef GLTEXTURELOADER_H_
#define GLTEXTURELOADER_H_

#include <list>
#include "ITextureLoader.h"

namespace XEngine{
/*!
	\brief	Texture loader class for OpenGL.
	\ingroup XEngine
	\author	Koray Balci
	\version 1.0
	\date   August 2002

	Loads the textures from files (later might create textures dynamically) using
	OpenGL function calls and stores the texture list. Deletes the textures on exit.
	An STL list is used to store the textures.

	\note Do NOT destroy the instance of this class if you still want to use the 
	textures created by it. It DELETEs all the textures created by this instance.

	In the framework, it is used in TextureManager class as a member variable. So, 
	if possible use TextureManager to load/unload and manage your textures. 
*/
class TextureLoaderGL : public ITextureLoader 
{
	typedef std::list<unsigned int> TEXLIST; 
public:
	//! Unloads the OpenGL texture from the ITexture instance.
	void unLoad(const ITexture* pTexture);
	//! Loads the texture with the filename to the ITexture instance.
	bool load(const std::string& filename, ITexture* pTexture);
	//! Default constructor
	TextureLoaderGL();
	virtual ~TextureLoaderGL();

private:
	TEXLIST m_TextureList;
};
} // namespace XFace
#endif // GLTEXTURELOADER_H_
