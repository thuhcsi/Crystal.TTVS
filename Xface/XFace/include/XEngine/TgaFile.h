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
	\file	TgaFile.h
	\brief	TGA file loader for textures.
*/
#pragma once

#ifndef TGAFILE_H_
#define TGAFILE_H_

#include "ITextureFile.h"

#include <stdio.h>
namespace XEngine{
	/*!
		\brief	TGA file loader for textures.
		\ingroup XEngine
		\author	Koray Balci
		\version 1.0
		\date   April 2003

		Loads TGA files for textures.
	*/
	class TgaFile : public ITextureFile
	{
		bool checkSize( int x ) const;
		unsigned char* loadRGB(FILE* fp, int size);
		unsigned char* loadRGBA(FILE* fp, int size);
		unsigned char* loadGray(FILE* fp, int size);
	public:
		bool loadold(const std::string& filename);
        bool load(const std::string& filename);
		TgaFile(void);
		~TgaFile(void);
	};
}
#endif //TGAFILE_H_
