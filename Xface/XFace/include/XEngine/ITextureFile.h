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
	\file	ITextureFile.h
	\brief	Interface for image file loaders for textures.
*/
#ifndef ITEXTUREFILE_H_
#define ITEXTUREFILE_H_

#include <string>

namespace XEngine{
/*!
	\brief	Interface for image file loaders for textures.
	\ingroup XEngine
	\author	Koray Balci
	\version 1.0
	\date   April 2003

	Interface for texture loading. Derived classes implement the load() member 
	function in order to support loading of various image file formats.
*/
class ITextureFile
{
protected:
	int m_Width;	//!< Width of the image
	int m_Height;	//!< Height of the image
	int m_nBits;	//!< Number of bits per pixel.
	unsigned char* m_pData; //!< Image data here
public:
	//! Accessor for loaded data.
	unsigned char* getData() {return m_pData;};
	//! Accessor for number of bits per pixel.
	int getNBits() const {return m_nBits;};
	//! Accessor for width of the image.
	int getWidth() const {return m_Width;}
	//! Accessor for height of the image.
	int getHeight() const {return m_Height;}
	//! Pure virtual interface method for loading files. Main entry point.
	virtual bool load(const std::string& filename) = 0;
	ITextureFile(void) : m_pData(0), m_Width(0), m_Height(0), m_nBits(0) {};
	virtual ~ITextureFile(void){
			if (m_pData)
			{
				delete [] m_pData;
				m_pData = 0;
			}
		};
};
} // namespace XFace
#endif //ITEXTUREFILE_H_
