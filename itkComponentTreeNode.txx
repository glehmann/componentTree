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
}

/** Destructor */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>
::~ComponentTreeNode() 
{
/*  if ( m_Parent )
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
  m_Attribute = 0; */
}

/** Return the parent node */
template <typename TPixel, typename TIndex, typename TValue>
ComponentTreeNode<TPixel, TIndex, TValue>* 
ComponentTreeNode<TPixel, TIndex, TValue>
::GetParent( ) const 
{
  return m_Parent;
}

/** Set the parent node */
template <typename TPixel, typename TIndex, typename TValue>
void 
ComponentTreeNode<TPixel, TIndex, TValue>
::SetParent( ComponentTreeNode<TPixel, TIndex, TValue>* node) 
{
	assert( node != this );
	// assert( node->GetPixel() < this->GetPixel() );
  m_Parent = node;
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
::CountPixels( ) const 
{
	int size = this->GetIndexes().size();
	for( typename ChildrenListType::const_iterator it=m_Children.begin(); it!=m_Children.end(); it++ )
	  {
		size += (*it)->CountPixels();
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
}

template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::Merge( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
	assert( node != this );
	// assert( this->GetPixel() <= node->GetPixel() );
	// merge the index list
  m_Indexes.splice( m_Indexes.begin(), node->GetIndexes() );
  // and add each child from node to the current child list
  for( typename ChildrenListType::iterator it=node->GetChildren().begin(); it!=node->GetChildren().end(); it++ )
    {
	  assert( (*it)->GetParent() == node );
	  this->AddChild( *it );
	  }
	// clear the child list to be sure they will not be accidentally used
	// so node will have no children and no index after this method
	node->GetChildrenList().clear();
}

template <typename TPixel, typename TIndex, typename TValue>
void ComponentTreeNode<TPixel, TIndex, TValue>
::MergeChildren() 
{
  for( typename ChildrenListType::iterator it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
	  assert( (*it)->GetParent() == this );
		// assert( this->GetPixel() < (*it)->GetPixel() );
	  // merge the children of the children
	  (*it)->MergeChildren();
	  // and merge this children
	  this->Merge( *it );
	  }
	// clear the child list 
	this->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue>
typename ComponentTreeNode<TPixel, TIndex, TValue>::Pointer
ComponentTreeNode<TPixel, TIndex, TValue>
::Clone() 
{
	// create a new node to clone this one
	Pointer node = Self::New();
	// copy the ivars
	node->SetAttribute( m_Attribute );
  // node->SetParent( NULL );
  node->SetPixel( m_Pixel );
  IndexListType  m_IndexList;
  
  for( typename ChildrenListType::iterator it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
	  assert( (*it)->GetParent() == this );
		assert( this->GetPixel() < (*it)->GetPixel() );
	  // merge the children of the children
	  (*it)->MergeChildren();
	  // and merge this children
	  this->Merge( *it );
	  }
	// clear the child list 
	this->GetChildren().clear();
}

template <typename TPixel, typename TIndex, typename TValue>
bool ComponentTreeNode<TPixel, TIndex, TValue>
::HasChild( ComponentTreeNode<TPixel, TIndex, TValue> *node ) 
{
	return std::find(m_Children.begin(), m_Children.end(), node ) != m_Children.end();
}

template <typename TPixel, typename TIndex, typename TValue>
const void ComponentTreeNode<TPixel, TIndex, TValue>
::print( int indent ) const
{
	for( int i=0; i<indent; i++)
	  {
    std::cout << "  ";
    }
  std::cout << "+ " << this->GetPixel()+0.0 << ":" << this->GetAttribute() << " ";
  for( typename IndexListType::const_iterator it=this->GetIndexes().begin(); it!=this->GetIndexes().end(); it++ )
    {
	  std::cout << *it << " ";
    }
  std::cout << std::endl;
  for( typename ChildrenListType::const_iterator it=this->GetChildren().begin(); it!=this->GetChildren().end(); it++ )
    {
	  (*it)->print( indent+1 );
	  }
  
}


} // namespace itk

#endif
