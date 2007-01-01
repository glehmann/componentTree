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
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::ComponentTreeNode()
{
  m_Parent = NULL;
  m_FirstIndex = -1;
  m_LastIndex = -1;
}

/** Destructor */
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::~ComponentTreeNode() 
{
  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    delete *it;
    }
  m_Children.clear();
}




/** Return the number of children */
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
int 
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
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
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
int 
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
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
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
bool 
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::RemoveChild( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *n ) 
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
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::AddChild( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *node ) 
{
  assert( node != this );
  // assert( node->GetPixel() > this->GetPixel() );
  assert( !this->HasChild( node ) );

  node->SetParent(this);
  m_Children.push_back(node);

  assert( this->HasChild( node ) );
  assert( node->GetParent() == this );
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
bool ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::HasChild( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *node ) const
{
  return std::find(m_Children.begin(), m_Children.end(), node ) != m_Children.end();
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::TakeChildrenFrom( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *node ) 
{
  assert( node != this );

  m_Children.splice( m_Children.begin(), node->GetChildren() );

  assert( node->GetChildren().empty() );
}




/** Return the number of indexes */
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
int 
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::CountIndexes( const LinkedListArrayType & listArray ) const 
{
  int size = 0;
  IndexType current = m_FirstIndex;
  while( current != -1 )
    {
    size++;
    current = listArray[ current ];
    }

  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    size += (*it)->CountIndexes( listArray );
    }

  return size;
}
 
/** Remove an index */
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
bool 
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::RemoveIndex( const IndexType & idx, LinkedListArrayType & listArray ) 
{
/*  IndexListIteratorType pos = m_Indexes.begin();
  IndexListIteratorType previous = m_Indexes.before_begin();
  while( pos != m_Indexes.end() )
    {
    if ( *pos == idx ) 
      {
      if( pos == m_LastIndex )
        {
        m_LastIndex = previous;
        }
      m_Indexes.erase(pos);
      return true;
      }
    previous = pos;
    pos++;
    }
  return false;*/
}

/** Add an index */
template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::AddIndex( const IndexType & idx, LinkedListArrayType & listArray ) 
{
  assert( !this->HasIndex( idx ) );

  if( m_LastIndex == -1 )
    {
    m_FirstIndex = idx;
    m_LastIndex = idx;
    listArray[ idx ] = -1;
    }
  else
    {
    listArray[ idx ] = m_FirstIndex;
    m_FirstIndex = idx;
    }
  assert( this->HasIndex( idx ) );
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
bool ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::HasIndex( const IndexType & idx, const LinkedListArrayType & listArray ) const
{
  IndexType current = m_FirstIndex;
  while( current != -1 )
    {
    if( current == idx )
      {
      return true;
      }
    current = listArray[ current ];
    }
  return false;
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::TakeIndexesFrom( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *node, LinkedListArrayType & listArray )
{
  assert( node != this );

  if( node->m_FirstIndex != -1 )
    {
    listArray[ node->m_LastIndex ] = m_FirstIndex;
    m_FirstIndex = node->m_FirstIndex;
    node->m_FirstIndex = -1;
    node->m_LastIndex = -1;
    }
}





template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::Merge( ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray> *node, LinkedListArrayType & listArray )
{
  assert( node != this );
  // assert( this->GetPixel() <= node->GetPixel() );
  // merge the index list
  this->TakeIndexesFrom( node, listArray );
  // and add each child from node to the current child list
  for( ChildrenListIteratorType it=node->GetChildren().begin(); it!=node->GetChildren().end(); it++ )
    {
    assert( (*it)->GetParent() == node );
    this->AddChild( *it );
    }
  // clear the child list to be sure they will not be accidentally used
  // so node will have no children and no index after this method
  node->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::Flatten(LinkedListArrayType & listArray) 
{
  for( ChildrenListIteratorType it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
    assert( (*it)->GetParent() == this );
    // assert( this->GetPixel() < (*it)->GetPixel() );
    // merge the children of the children
    (*it)->Flatten( listArray );
    // and merge this children
    this->Merge( *it, listArray );
    delete *it;
    }
  // clear the child list 
  this->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
typename ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>::Self *
ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::Clone() 
{
/*  // create a new node to clone this one
  Self * node = new Self();
  // copy the ivars
  node->SetAttribute( m_Attribute );
  // node->SetParent( NULL );
  node->SetPixel( m_Pixel );
  IndexListType  m_IndexList;
  
  for( ChildrenListIteratorType it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
    assert( (*it)->GetParent() == this );
    assert( this->GetPixel() < (*it)->GetPixel() );
    // merge the children of the children
    (*it)->Clone();
    // and merge this children
    this->Merge( *it );
    }
  // clear the child list 
  this->GetChildren().clear();*/
}

template <typename TPixel, typename TIndex, typename TValue, typename TLinkedListArray>
const void ComponentTreeNode<TPixel, TIndex, TValue, TLinkedListArray>
::Print( int indent ) const
{
  assert( this != NULL );
/*  for( int i=0; i<indent; i++)
    {
    std::cout << "  ";
    }
  std::cout << "+ " << static_cast<typename NumericTraits<PixelType>::PrintType>(this->GetPixel())
            << ":" << this->m_Attribute << " ";
  for( typename IndexListType::const_iterator it=this->GetIndexes().begin(); it!=this->GetIndexes().end(); it++ )
    {
    std::cout << *it << " ";
    }
  std::cout << std::endl;
  for( typename ChildrenListType::const_iterator it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
    (*it)->Print( indent+1 );
    }*/
  
}


} // namespace itk

#endif
