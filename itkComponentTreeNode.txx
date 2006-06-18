/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeNode.txx,v $
  Language:  C++
  Date:      $Date: 2005/09/25 16:22:46 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkComponentTreeNode_txx
#define _itkComponentTreeNode_txx

#include "itkComponentTreeNode.h"

namespace itk
{

/** Constructor */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>::ComponentTreeNode()
{
  m_Parent = NULL;
}

/** Destructor */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>::~ComponentTreeNode() 
{
  if ( m_Parent )
    {
    m_Parent->Remove(this);
    }
    
 for ( int i=m_Children.size() ; i > 0; i-- )
   {
   m_Children[i-1]->SetParent(NULL);
   m_Children[i-1] = 0;
   }
  m_Children.clear();
  m_Parent = NULL;
  m_Data = 0;
}

/** Return the parent node */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>* 
ComponentTreeNode<TPixel, TIndex, TValue>
::GetParent( ) const 
{
  return m_Parent;
}

/** Get a child */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>* 
ComponentTreeNode<TPixel, TIndex, TValue>
::GetChild( int number ) const 
{
  if ( (unsigned int)number < m_Children.size() )
    {
    return m_Children[number];
    }
  else
    {
    return NULL;
    }
}

/** Set the value of a node */
template <typename TPixel, typename TIndex, typename TValue>
TValue ComponentTreeNode<TPixel, TIndex, TValue>::Set(const TValue data)
{
  TValue help = m_Data;
  m_Data = data;
  return help;
}

/** Get the data of node */
template <typename TPixel, typename TIndex, typename TValue>
const TValue& ComponentTreeNode<TPixel, TIndex, TValue>::Get() const 
{
  return m_Data;
}

/** Return true if has a parent */
template <typename TPixel, typename TIndex, typename TValue>
bool 
ComponentTreeNode<TPixel, TIndex, TValue>::HasParent() const 
{
  return (m_Parent)?true:false;
}

/** Set the parent node */
template <typename TPixel, typename TIndex, typename TValue>
void 
ComponentTreeNode<TPixel, TIndex, TValue>::SetParent( ComponentTreeNode<TPixel, TIndex, TValue>* node) 
{
  m_Parent = node;
}

/** Return true if the node has children */
template <typename TPixel, typename TIndex, typename TValue>
bool ComponentTreeNode<TPixel, TIndex, TValue>::HasChildren() const 
{
  return (m_Children.size()>0)?true:false;
}

/** Return the number of children */
template <typename TPixel, typename TIndex, typename TValue>
int 
ComponentTreeNode<TPixel, TIndex, TValue>
::CountChildren( ) const 
{
  return m_Children.size();
}

/** Remove a child node from the current node */
template <typename TPixel, typename TIndex, typename TValue>
bool 
ComponentTreeNode<TPixel, TIndex, TValue>
::Remove( ComponentTreeNode<TPixel, TIndex, TValue> *n ) 
{
  typename std::vector<Pointer>::iterator pos;
  pos = std::find(m_Children.begin(), m_Children.end(), n );
  if ( pos != m_Children.end() ) 
    {
    m_Children.erase(pos);
    n->SetParent(NULL);
    return true;
    }
  return false;
}

/** Replace a child by a new one */
template <typename TPixel, typename TIndex, typename TValue>
bool ComponentTreeNode<TPixel, TIndex, TValue>::ReplaceChild( ComponentTreeNode<TPixel, TIndex, TValue> *oldChild, ComponentTreeNode<TPixel, TIndex, TValue> *newChild )
{
  int size = m_Children.size();

  for ( int i=0; i<size; i++ )
    {
    if ( m_Children[i] == oldChild )
      {
      m_Children[i] = newChild;
      return true;
      }
    }
  return false;
}

/** Return the child position given a node */
template <typename TPixel, typename TIndex, typename TValue>
int ComponentTreeNode<TPixel, TIndex, TValue>::ChildPosition( const ComponentTreeNode<TPixel, TIndex, TValue> *node ) const 
{
  for ( unsigned int i=0; i < m_Children.size(); i++ )
    {
    if ( m_Children[i] == node )
      {
      return i;
      }
    }
  return -1;
}

/** Return the child position given an element, the first child found. */
template <typename TPixel, typename TIndex, typename TValue>
int ComponentTreeNode<TPixel, TIndex, TValue>::ChildPosition( TValue element ) const 
{
  for ( unsigned int i=0; i < m_Children.size(); i++ ) 
    {
    if ( m_Children[i]->Get() == element )
    return i;
    }
  return -1;
}

/** Add a child node */
template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>::AddChild( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
  node->SetParent(this);
  m_Children.push_back(node);
}

/** Add a child at a specific position in the children list */
template <typename TPixel, typename TIndex, typename TValue>
void 
ComponentTreeNode<TPixel, TIndex, TValue>
::AddChild( int number, ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{  
  int size = m_Children.size();

  if ( number > size ) 
    {
    for ( int i=size; i <= number; i++ )
      {
      m_Children[i] = NULL;
      }
    m_Children[number] = node;
    return;
    }

  ComponentTreeNode<TPixel, TIndex, TValue>* old = m_Children[number];
  if ( old != NULL )
    {
    delete old;
    }
  m_Children[number] = node;
}

/** Get the number of children given a name and a depth */
template <typename TPixel, typename TIndex, typename TValue>
unsigned int
ComponentTreeNode<TPixel, TIndex, TValue>
::GetNumberOfChildren( unsigned int depth, char * name ) const
{
  typename ChildrenListType::const_iterator it = m_Children.begin();
  typename ChildrenListType::const_iterator itEnd = m_Children.end();

  unsigned int cnt = 0;
  while(it != itEnd)
    {
    if(name == NULL || strstr(typeid(**it).name(), name))
      {
      cnt++;
      }
    it++;
    }

  it = m_Children.begin();
  itEnd = m_Children.end();
  if( depth > 0 )
    {
    while(it != itEnd)
      {
      cnt += (*it)->GetNumberOfChildren( depth-1, name );
      it++;
      }
    }

  return cnt;
}

/** Get children given a name and a depth */
template <typename TPixel, typename TIndex, typename TValue>
typename ComponentTreeNode<TPixel, TIndex, TValue>::ChildrenListType* 
ComponentTreeNode<TPixel, TIndex, TValue>
::GetChildren( unsigned int depth, char * name) const
{
  ChildrenListType * children = new ChildrenListType;

  typename ChildrenListType::const_iterator childrenListIt = 
    m_Children.begin();
  typename ChildrenListType::const_iterator childrenListEnd = 
    m_Children.end();

  while( childrenListIt != childrenListEnd )
    {
    if( name == NULL || strstr(typeid(**childrenListIt).name(), name) )
      {
      children->push_back(*childrenListIt);
      }
    if( depth > 0 )
      {
      ChildrenListType * nextchildren = (**childrenListIt).GetChildren(depth-1,
                                                                       name);  
      // Add the child to the current list
      typename ChildrenListType::const_iterator nextIt = nextchildren->begin();
      while(nextIt != nextchildren->end())
        {
        children->push_back(*nextIt);
        nextIt++;
        }
      delete nextchildren;
      }
    childrenListIt++;
    }

  return children;
}


} // namespace itk

#endif
