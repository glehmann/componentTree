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
ComponentTreeNode<TPixel, TIndex, TValue>
::ComponentTreeNode()
{
  m_Parent = NULL;
  m_FirstIndex = -1;
  m_LastIndex = -1;
}

/** Destructor */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>
::~ComponentTreeNode() 
{
  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    delete *it;
    }
  m_Children.clear();
}




/** Return the number of children */
template <typename TPixel, typename TIndex, typename TValue>
int 
ComponentTreeNode<TPixel, TIndex, TValue>
::CountChildren( ) const 
{
  int size = 1;
  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    size += (*it)->CountChildren();
    }
  return size;
}

/** Return the number of children */
template <typename TPixel, typename TIndex, typename TValue>
int 
ComponentTreeNode<TPixel, TIndex, TValue>
::Depth( ) const 
{
  int depth = 0;
  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    depth = std::max( depth, (*it)->Depth() );
    }
  return depth + 1;
}

/** Remove a child node from the current node */
template <typename TPixel, typename TIndex, typename TValue>
bool 
ComponentTreeNode<TPixel, TIndex, TValue>
::RemoveChild( ComponentTreeNode<TPixel, TIndex, TValue> *n ) 
{
  typename ChildrenListType::iterator pos = std::find(m_Children.begin(), m_Children.end(), n );
  if ( pos != m_Children.end() )
    {
    m_Children.erase(pos);
    n->SetParent(NULL);
    return true;
    }
  return false;
}

/** Add a child node */
template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::AddChild( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
  assert( node != this );
  // assert( node->GetPixel() > this->GetPixel() );
  assert( !this->HasChild( node ) );

  node->SetParent(this);
  m_Children.push_back(node);

  assert( this->HasChild( node ) );
  assert( node->GetParent() == this );
}

template <typename TPixel, typename TIndex, typename TValue>
bool ComponentTreeNode<TPixel, TIndex, TValue>
::HasChild( ComponentTreeNode<TPixel, TIndex, TValue> *node ) const
{
  return std::find(m_Children.begin(), m_Children.end(), node ) != m_Children.end();
}

template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::TakeChildrenFrom( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
  assert( node != this );

  m_Children.splice( m_Children.begin(), node->GetChildren() );

  assert( node->GetChildren().empty() );
}





template <typename TPixel, typename TIndex, typename TValue>
typename ComponentTreeNode<TPixel, TIndex, TValue>::Self *
ComponentTreeNode<TPixel, TIndex, TValue>
::Clone() const
{
  // create a new node to clone this one
  Self * node = new Self();
  // copy the ivars
  node->SetAttribute( m_Attribute );
  node->SetParent( NULL );
  node->SetPixel( m_Pixel );
  node->SetFirstIndex( m_FirstIndex );
  node->SetLastIndex( m_LastIndex );
  // and copy clone the childs
  for( ChildrenListConstIteratorType it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
    assert( (*it)->GetParent() == this );

    Self * child = (*it)->Clone();
    node->AddChild( child );
    }
    
  assert( this->CountChildren() == node->CountChildren() );
  
  return node;
}

} // namespace itk

#endif
