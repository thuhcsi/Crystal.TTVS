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
 * The Original Code is Xface Core Library; Texture Manager.
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

#include <XEngine/TextureManager.h>
#include <XEngine/Texture2D.h>

#include <XEngine/TextureLoaderGL.h>

namespace XEngine{


TextureManager* TextureManager::m_pThisTexMan = 0;
TextureManager::TextureManager(void)
{
	m_pTexLoader = new TextureLoaderGL;
}

TextureManager::~TextureManager(void)
{
	destroyAll();
	delete m_pTexLoader;
}

/*!
	Singleton class accessor for the one and only instance of this class.
*/
TextureManager* TextureManager::getInstance()
{
	if (!m_pThisTexMan)
		m_pThisTexMan = new TextureManager();
	return m_pThisTexMan;
}

/*!
	Releases the one and only instance.
*/
void TextureManager::releaseInstance()
{
	delete m_pThisTexMan;
	m_pThisTexMan = 0;
}

/*!
	Registers a texture object to the storage.
*/
void TextureManager::registerTexture(const ITexture* pTexture)
{
	m_Storage.insert(std::make_pair(pTexture->getName().c_str(), pTexture));
}

/*!
	Removes a texture from the storage.
*/
void TextureManager::unregisterTexture(const ITexture* pTexture)
{
	TEXSTORAGE::iterator it = m_Storage.find(pTexture->getName().c_str());
	if(it != m_Storage.end())
	{
		m_pTexLoader->unLoad(it->second);
		delete it->second;
		m_Storage.erase(it);
	}	
}

/*!
	Retrieves a texture with the name passed. 
	\return 0 if unsucessful, a valid pointer to ITexture if successful.
*/
const ITexture* TextureManager::getTexture(const std::string& name) const
{
	TEXSTORAGE::const_iterator it = m_Storage.find(name.c_str());
	if(it == m_Storage.end())
		return 0;

	return it->second;
}

/*!
	Destroys all of the textures in the storage.
*/
void TextureManager::destroyAll()
{
	TEXSTORAGE::iterator it = m_Storage.begin();
	while (it != m_Storage.end())
	{
		delete it->second;
		++it;
	}
	m_Storage.clear();
}

/*!
	Loads a texture from a file, creates an ITexture instance, binds the texture to it,
	and registers it to texture storage.
	\param filename Name of the image file to be used as a texture.
	\param texname Name of the texture object to be created and binded to texture.
*/
const ITexture* TextureManager::load(const std::string& filename, const std::string& texname )
{
	TEXSTORAGE::const_iterator it = m_Storage.find(texname);
	if(it != m_Storage.end())
		return it->second;
	ITexture* pTexture = new Texture2D(texname);
	if ( m_pTexLoader->load(filename, pTexture) )
	{
		registerTexture(pTexture);
	}
	else
	{
		delete pTexture;
		pTexture = 0;
	}

	return pTexture;
}

void TextureManager::rebindTextures()
{
    // added by John (john.zywu@gmail.com)

    // store the "filename" and "texname"
    std::map<std::string, std::string> textureNames;
    for (TEXSTORAGE::iterator it = m_Storage.begin(); it != m_Storage.end(); it ++)
    {
        // use "texname" as "filename" directly for texture loading
        textureNames.insert(std::make_pair(it->second->getName(), it->second->getName()));
    }

    // destroy the old textures
    destroyAll();
    delete m_pTexLoader;
    m_pTexLoader = new TextureLoaderGL();

    // rebind the new texture
    for (std::map<std::string, std::string>::iterator it = textureNames.begin(); it != textureNames.end(); it ++)
    {
        // use "texname" as "filename" directly for texture loading
        load(it->first, it->second);
    }
}

/*!
	Unloads the texture completely and removes it from the storage.
	\param pTexture A valid, existant ITexture instance.
*/
void TextureManager::unLoad(const ITexture* pTexture)
{
	m_pTexLoader->unLoad(pTexture);
	unregisterTexture(pTexture);
}

/*!
	Unloads the texture completely and removes it from the storage.
	\param name Name of the texture object in the storage.
*/
void TextureManager::unLoad(const std::string& name)
{
	unLoad(getTexture(name));
}

}  // namespace XFace