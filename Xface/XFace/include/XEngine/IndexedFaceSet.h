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
 * The Original Code is Xface Core Library
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
	\file	IndexedFaceSet.h
	\brief	Indexed mesh geometry implemented.
*/

#ifndef INDEXEDFACESET_H_
#define INDEXEDFACESET_H_

#include "Geometry.h"
#include <vector>

namespace XEngine{

/*!
	\brief Indexed mesh geometry implemented.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003

	Implements the indexed face set from the Geometry class. Each polygon is a 
	triangle in the geometry, if not they are corrected automatically when the 
    indices are set by the loader.

	See base class documentation for use and more info.
	\sa Geometry
*/
class IndexedFaceSet :	public Geometry
{
protected:
	std::vector<unsigned short> m_indices;
public:
	void computeVertexNormals();
	//! overriding base class virtual method
	IndexedFaceSet& copyFrom(const IndexedFaceSet& rhs);
	//! Returns the number of indices.
	size_t getIndexCount() const {return m_indices.size();}
	//! Accessor for the indices to the triangles.
	const unsigned short* getIndices() const {return &m_indices[0];}//{return m_indices.getRawPointer();}
	void setIndices(const std::vector< std::vector<unsigned short> > &indices);
	void setIndices(const std::vector< unsigned short > &indices);
	
	IndexedFaceSet(const std::string& name) : Geometry(name){};
	~IndexedFaceSet(void){};
};
}

#endif //INDEXEDFACESET_H_
