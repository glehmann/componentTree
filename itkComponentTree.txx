/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTree.txx,v $
  Language:  C++
  Date:      $Date: 2006/05/10 20:27:16 $
  Version:   $Revision: 1.97 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkComponentTree_txx
#define _itkComponentTree_txx

#include "itkComponentTree.h"
#include "itkProcessObject.h"

namespace itk
{

/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
ComponentTree<TPixel, VImageDimension, TValue>
::ComponentTree()
{
  this->Initialize();
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
//   os << indent << "PixelContainer: " << std::endl;
//   m_Buffer->Print(os, indent.GetNextIndent());

// m_Origin and m_Spacing are printed in the Superclass
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::Initialize()
{
  m_Root = NULL;
  m_LinkedListArray.clear();
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::Allocate()
{
  m_LinkedListArray.resize( this->GetLargestPossibleRegion().GetNumberOfPixels() );
}


template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft( data );

  if ( data )
    {
    // Attempt to cast data to an Image
    const Self * imgData;

    try
      {
      imgData = dynamic_cast<const Self *>( data );
      }
    catch( ... )
      {
      return;
      }


    if ( imgData )
      {
      // Now copy anything remaining that is needed
      this->SetRoot( const_cast< NodeType * >
                                  (imgData->GetRoot() ) );
      this->m_LinkedListArray = imgData->m_LinkedListArray;
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::Image::Graft() cannot cast "
                         << typeid(data).name() << " to "
                         << typeid(const Self *).name() );
      }
    }
}


/** Return the number of indexes */
template<class TPixel, unsigned int VImageDimension, class TValue>
int 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeCountIndexes( const NodeType * node ) const 
{
  assert( node != NULL );

  int size = 0;
  OffsetValueType current = node->GetFirstIndex();
  while( current != -1 )
    {
    size++;
    current = m_LinkedListArray[ current ];
    }

  for( typename NodeType::ChildrenListType::const_iterator it=node->GetChildren().begin();
       it!=node->GetChildren().end();
       it++ )
    {
    size += this->NodeCountIndexes( *it );
    }

  return size;
}
 
/** Remove an index */
template<class TPixel, unsigned int VImageDimension, class TValue>
bool 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeRemoveIndex( NodeType * node, const OffsetValueType & idx ) 
{
  assert( node != NULL );

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
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeAddIndex( NodeType * node, const OffsetValueType & idx ) 
{
  assert( node != NULL );
  assert( !this->NodeHasIndex( node, idx ) );

  if( node->GetLastIndex() == NodeType::EndIndex )
    {
    node->SetFirstIndex( idx );
    node->SetLastIndex( idx );
    m_LinkedListArray[ idx ] = NodeType::EndIndex;
    }
  else
    {
    m_LinkedListArray[ idx ] = node->GetFirstIndex();
    node->SetFirstIndex( idx );
    }
  assert( this->NodeHasIndex( node, idx ) );
}

template<class TPixel, unsigned int VImageDimension, class TValue>
bool 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeHasIndex( const NodeType * node, const OffsetValueType & idx ) const
{
  assert( node != NULL );

  OffsetValueType current = node->GetFirstIndex();
  while( current != -1 )
    {
    if( current == idx )
      {
      return true;
      }
    current = m_LinkedListArray[ current ];
    }
  return false;
}

template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeTakeIndexesFrom( NodeType * node, NodeType * obsolatedNode )
{
  assert( node != NULL );
  assert( obsolatedNode != NULL );
  assert( node != obsolatedNode );

  if( obsolatedNode->GetFirstIndex() != NodeType::EndIndex )
    {
    m_LinkedListArray[ obsolatedNode->GetLastIndex() ] = node->GetFirstIndex();
    node->SetFirstIndex( obsolatedNode->GetFirstIndex() );
    obsolatedNode->SetFirstIndex( NodeType::EndIndex );
    obsolatedNode->SetLastIndex( NodeType::EndIndex );
    }
}





template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeMerge( NodeType * node, NodeType * obsolatedNode )
{
  assert( node != NULL );
  assert( obsolatedNode != NULL );
  assert( node != obsolatedNode );

  // merge the index list
  this->NodeTakeIndexesFrom( node, obsolatedNode );
  
  // and add each child from node to the current child list
  for( typename NodeType::ChildrenListType::iterator it=obsolatedNode->GetChildren().begin();
       it!=obsolatedNode->GetChildren().end();
       it++ )
    {
    assert( (*it)->GetParent() == obsolatedNode );
    node->AddChild( *it );
    }
  // clear the child list to be sure they will not be accidentally used
  // so node will have no children and no index after this method
  obsolatedNode->GetChildren().clear();
}

template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::NodeFlatten( NodeType * node ) 
{
  assert( node != NULL );

  for( typename NodeType::ChildrenListType::iterator it=node->GetChildren().begin();
       it!=node->GetChildren().end();
       it++ )
    {
    assert( (*it)->GetParent() == node );
    // assert( this->GetPixel() < (*it)->GetPixel() );
    // merge the children of the children
    this->NodeFlatten( *it );
    // and merge this children
    this->NodeMerge( node, *it );
    delete *it;
    }
  // clear the child list 
  node->GetChildren().clear();
}
} // end namespace itk

#endif
