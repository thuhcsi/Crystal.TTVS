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
 * The Original Code is XFace:XEngine
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */
#pragma once

#include <vector>
#include <algorithm>
#include <cassert>

namespace XEngine{

template <typename DataType>
class Buffer
{
	std::vector<DataType> m_data;
public:
	//! [] operator overloaded (non-const version)
	DataType& operator[](int i){ assert(i >= 0 && i < m_data.size());return m_data[i];};
	//! [] operator overloaded (const version)
	DataType operator[] (int i) const {assert(i >= 0 && i < m_data.size());return m_data[i];};
		
	const std::vector<DataType>& getData() const {return m_data;}
	const DataType* const getRawPointer() const {return &m_data[0];}
	inline void setData(const std::vector<DataType>& data);
	inline void setData(const DataType* pData, size_t sz);
	size_t size() const {return m_data.size();}

	Buffer(void){}
	~Buffer(void){}
};

template <typename DataType>
void Buffer<DataType>::setData(const std::vector<DataType>& data)
{
	if(data.size() != m_data.size())
		m_data.resize(data.size());
	
	std::copy(data.begin(), data.end(), m_data.begin());
}

template <typename DataType>
void Buffer<DataType>::setData(const DataType* pData, size_t sz)
{
	if(sz != m_data.size())
		m_data.resize(sz);
		
	std::copy(pData, pData + sz, m_data.begin());
}

} // namespace XEngine