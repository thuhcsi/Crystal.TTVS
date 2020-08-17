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
 * - John (johnson.tsing@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file VRML97Regularizer.h
	\brief Loads VRML97 file, regularize it and exports to a new file
*/
#ifndef VRML97REGULARIZE_H_
#define VRML97REGULARIZE_H_

#include "ISceneLoader.h"
#include "IndexedFaceSet.h"


namespace XEngine{
	
/*!
	\brief Loads VRML97 file, regularizes it and exports to a new file
	\ingroup XEngine
	\author John
	\version 1.0
	\date   March 2016
			
	Loads VRML97 files, regularizes it (to make one-to-one mapping of vertices, normals and texCoords),
	and then exports to a new file.
*/
class VRML97Regularizer
{
	void onIndexedFaceSet(std::istream& iStream, std::streampos& lstPos, std::string& outStr) const;
	void onCoord(std::istream& iStream, std::vector<Vector3>& vertices) const;
	void onTexCoord(std::istream& iStream, std::vector<Vertex2D>& texCoords) const;
	void onNormal(std::istream& iStream, std::vector<Vector3>& vertices) const;
	void onIndex(std::istream& iStream, std::vector< std::vector<unsigned short> >& store, size_t number=0) const;
	void readTill(std::istream& iStream, std::streampos begPos, std::streampos endPos, std::string& outStr) const;
	void readTill(std::istream& iStream, std::streampos begPos, const std::string& tgtStr, std::string& outStr) const;
public:
	void regularize(const std::string &infilename, const std::string &outfilename);
	VRML97Regularizer(void){};
	~VRML97Regularizer(void){};
};
} // namespace XFace
#endif //VRML97REGULARIZE_H_
