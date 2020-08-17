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
 * The Original Code is Xface Core Library;.
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
	\file Drawable.h
	\brief Stores the information for drawable objects.
*/

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <string>
#include <cassert>
#include "Transform.h"


/*!
	\addtogroup XEngine XEngine
	XEngine is the 3D engine used by XFace library. It is a simple implementation,
	specialized for XFace, implemented as generic as possible.
*/


/*!
	\namespace XEngine 3D engine go under this namespace.
*/
namespace XEngine{

/*!
	\brief Stores the information for drawable objects.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003
			
	Stores the information about the drawables that can be rendered. We store the
	name of the Mesh and Textures (for the moment, others in the future) as std::string
	member variables. This class acts only as an alias that stores the references
	(names) of the real data. 

	We store the Drawable instances for head, eyes, etc in RenderList object. Renderer
	(or derived RendererGL) class use the information in Drawable class and retrieves
	the appropriate data and renders them.
*/
class Drawable
{
	bool m_bTextureOn;
	std::string m_name;
	//! Name of the Mesh associated
	std::string m_MeshName;
	//! Names of the textures (Up to 4 textures)
	std::string m_TexName[4];
	//! Transformation info
	Transform m_transform;
	
public:
    //! Enable texture mapping on/off for this drawable.
	void enableTexture(bool mode) {m_bTextureOn = mode;}
	//! Query if texture mapping is enabled.
	bool isTextureOn() const {return m_bTextureOn;}
	//! Supp. Constructor with name prop
	Drawable(const std::string& name) : m_name(name){};
	//! Default CTor
	Drawable() : m_bTextureOn(false), m_name(""){};
	//! DTor (not virtual!)
	~Drawable(){};
	//@{ Name property accessor
	const std::string& getName() const {return m_name;}
	void setName(const std::string& name) {m_name = name; m_bTextureOn = true;}
	//@}
	//@{ Transform property accessor
	const Transform& getTransform() const {return m_transform;}
	void setTransform(const Transform& tr) {m_transform = tr;}
	//@}
	//! Sets the name of associated mesh.
	void setMeshName(const std::string& MeshName) {m_MeshName = MeshName;};
	//! Retrieves the name of associated mesh.
	const std::string& getMeshName() const {return m_MeshName;};
	/*! Sets the name of the texture for each layer. Default parameter use the 
	first texture unit.
	\param id Name of the texture.
	\param layer Order of texture unit to be used for this texture. 
 Defaulted to 0 and cannot be greater than 3.
	*/
	void setTexName(const std::string id, unsigned short layer = 0)
	{
		assert(layer < 4);
		m_TexName[layer] = id;
	};
	/*! Retrieves the name of the texture for each layer. Default parameter use the 
	first texture unit.
	\param layer Order of texture unit to be used for this texture.
	*/
	const std::string& getTexName(unsigned short layer = 0) const 
	{
		assert(layer < 4);
		return m_TexName[layer];
	};
};
	} // namespace XEngine
#endif // DRAWABLE_H_
