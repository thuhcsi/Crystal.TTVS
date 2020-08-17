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
	\file	TextureManager.h
	\brief	Textures are stored and managed here.
*/

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <map>
#include <string>
#include "ITexture.h"
#include "ITextureLoader.h"

namespace XEngine{
	/*!
		\brief Textures are stored and managed here.
		\ingroup XEngine
		\author Koray Balci
		\version 1.0
		\date   May 2003

		Textures (ITexture instances) are loaded, stored and managed through here. 
		All the textures are stored in a hash map (currently) with a string representing 
		the name as the key of the hash. TextureManager is a singleton class.

		Here is a sample usage for loading a texture;
		\code
		TextureManager::getInstance()->load("..\\john_lo_obj02.bmp", "john_lo_obj02");
		// this is optional but good to do, associate the texture to a Drawable
		Drawable* pDrawable = new Drawable;
		pDrawable->setTexName("john_lo_obj00");
		\endcode

		And here is how we access the loaded texture (from RendererGL);
		\code
		const ITexture* pTexture = TextureManager::getInstance()->getTexture(pDrawable->getTexName());
		\endcode
	*/
	class TextureManager
	{
	/*	struct eqstr
		{
		bool operator()(const char* s1, const char* s2) const
		{
			return (strcmp(s1, s2) != 0);	// might be == 0 (oops!!!)
		}
		};

		typedef std::hash_map<const char*, const ITexture*, std::hash_compare<const char*, eqstr> > TEXSTORAGE;
		//typedef std::hash_map<const char*, const IMesh*, std::hash<const char*>, eqstr > MESHSTORAGE;
	*/
		typedef std::map<std::string, const ITexture*> TEXSTORAGE;
		TEXSTORAGE m_Storage;
		static TextureManager* m_pThisTexMan;
		ITextureLoader* m_pTexLoader;
		
		TextureManager(void);
		~TextureManager(void);
	public:
		const ITexture* load(const std::string& filename, const std::string& texname );
		void unLoad(const ITexture* pTexture);
		void unLoad(const std::string& name);

		static void releaseInstance();
		static TextureManager* getInstance();
		void destroyAll();
		void registerTexture(const ITexture* pTexture);
		void unregisterTexture(const ITexture* pTexture);
		const ITexture* getTexture(const std::string& name) const;

        void rebindTextures();
	};
}
#endif //TEXTUREMANAGER_H_
