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
  m_LastChild = m_Children.before_begin();
  m_LastIndex = m_Indexes.before_begin();
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
  ChildrenListIteratorType pos = m_Children.begin();
  ChildrenListIteratorType previous = m_Children.before_begin();
  while( pos != m_Children.end() )
  	{
    if ( *pos == n ) 
      {
      if( pos == m_LastChild )
      	{
      	m_LastChild = previous;
      	}
      m_Children.erase(pos);
      n->SetParent(NULL);
      return true;
      }
    previous = pos;
    pos++;
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
  if( m_Children.empty() )
  	{
    m_Children.push_front(node);
    m_LastChild = m_Children.begin();
    }
  else
  	{
    m_Children.push_front(node);
  	}

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

  if( !node->GetChildren().empty() )
    {
	  m_Children.splice_after( m_LastChild, node->GetChildren().before_begin(), node->m_LastChild );
    m_LastChild = node->m_LastChild;
    node->m_LastChild = node->GetChildren().before_begin();
    }
     
  assert( node->GetChildren().empty() );
//  assert( this->HasChild( *(node->m_LastChild) ) );
}




/** Return the number of indexes */
template <typename TPixel, typename TIndex, typename TValue>
int 
ComponentTreeNode<TPixel, TIndex, TValue>
::CountIndexes( ) const 
{
  int size = this->GetIndexes().size();
  for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
    {
    size += (*it)->CountIndexes();
    }
  return size;
}
 
/** Remove an index */
template <typename TPixel, typename TIndex, typename TValue>
bool 
ComponentTreeNode<TPixel, TIndex, TValue>
::RemoveIndex( const IndexType & idx ) 
{
  ChildrenListIteratorType pos = m_Indexes.begin();
  ChildrenListIteratorType previous = m_Indexes.before_begin();
  while( pos != m_Indexes.end() )
  	{
    if ( *pos == idx ) 
      {
      if( pos == m_LastChild )
      	{
      	m_LastChild = previous;
      	}
      m_Indexes.erase(pos);
      return true;
      }
    previous = pos;
    pos++;
    }
  return false;
}

/** Add an index */
template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::AddIndex( const IndexType & idx ) 
{
  assert( !this->HasIndex( idx ) );

  if( m_Indexes.empty() )
    {
    m_Indexes.push_front( idx );
    m_LastIndex = m_Indexes.begin();
    }
  else
  	{
    m_Indexes.push_front( idx );
  	}

  assert( this->HasIndex( idx ) );
}

template <typename TPixel, typename TIndex, typename TValue>
bool ComponentTreeNode<TPixel, TIndex, TValue>
::HasIndex( const IndexType & idx ) const
{
  return std::find(m_Indexes.begin(), m_Indexes.end(), idx ) != m_Indexes.end();
}

template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::TakeIndexesFrom( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
  assert( node != this );

//	std::cout << "m_Indexes.size(): " << m_Indexes.size() << std::endl;
//	std::cout << "node->GetIndexes().size(): " << node->GetIndexes().size() << std::endl;

  if( !node->GetIndexes().empty() )
    {
	  m_Indexes.splice_after( m_LastIndex, node->GetIndexes().before_begin(), node->m_LastIndex );
	  m_LastIndex = node->m_LastIndex;
    node->m_LastIndex = node->GetIndexes().before_begin();
    }

//	std::cout << "m_Indexes.size(): " << m_Indexes.size() << std::endl;
//	std::cout << "node->GetIndexes().size(): " << node->GetIndexes().size() << std::endl;
  assert( node->GetIndexes().empty() );
//  assert( this->HasIndex( *(node->m_LastIndex) ) );
}





template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::Merge( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
  assert( node != this );
  // assert( this->GetPixel() <= node->GetPixel() );
  // merge the index list
  this->TakeIndexesFrom( node );
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

template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::Flatten() 
{
  for( ChildrenListIteratorType it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
    assert( (*it)->GetParent() == this );
    // assert( this->GetPixel() < (*it)->GetPixel() );
    // merge the children of the children
    (*it)->Flatten();
    // and merge this children
    this->Merge( *it );
    delete *it;
    }
  // clear the child list 
  this->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue>
typename ComponentTreeNode<TPixel, TIndex, TValue>::Self *
ComponentTreeNode<TPixel, TIndex, TValue>
::Clone() 
{
  // create a new node to clone this one
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
  this->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue>
const void ComponentTreeNode<TPixel, TIndex, TValue>
::Print( int indent ) const
{
  assert( this != NULL );
  for( int i=0; i<indent; i++)
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
    }
  
}


} // namespace itk

#endif
