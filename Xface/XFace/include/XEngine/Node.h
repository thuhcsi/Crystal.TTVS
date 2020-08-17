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
	\file	Node.h
	\brief	Scenegraph node abstraction.
*/

#pragma once

#include <list>
#include <vector>

namespace XEngine{

template <class NodeType>
class Node
{
	typedef std::list<Node<NodeType>* > CHILDREN;
	
	CHILDREN m_children;
	typename CHILDREN::iterator m_iterator;

	Node<NodeType>* m_pParent;
	NodeType* m_pNodeContent;

public:

	Node(NodeType* pNodeContent) : m_pNodeContent(pNodeContent), m_pParent(0) {}
	virtual ~Node(void){}

	//! Returns the parent of this node
	Node<NodeType>* const getParent() const {return m_pParent;}
	//! Removes child from the child list
	void removeChild(Node<NodeType>* pChild)
	{
		m_children.remove(pChild);
	}
	//! Returns a child from the child list
	NodeType* const getNextChild() 
	{
		if(m_iterator == m_children.end())
			return 0;
	
		NodeType *pRet = (*m_iterator)->m_pNodeContent;
		m_iterator++;
		return pRet;
	}
	//! Adds a child to the child list
	void addChild(Node<NodeType>* const pChild)
	{
		pChild->m_pParent = this;
		m_children.push_back(pChild);
	
		// can't think of anyplace better to init the iterator...
		m_iterator = m_children.begin();	
	};

	//! Returns the number of children
	size_t getChildrenCount(){ return m_children.size();}

	void clearNodes()
	{
		CHILDREN::iterator it = m_children.begin();
		while(it != m_children.end())
		{
			(*it)->clearNodes();
			delete *it;
		}
		m_children.clear();
	}
};

} // namespace XEngine