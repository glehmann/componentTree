/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkKeepNLobesComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkKeepNLobesComponentTreeFilter_txx
#define __itkKeepNLobesComponentTreeFilter_txx

#include "itkKeepNLobesComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage, class TAttributeAccessor>
KeepNLobesComponentTreeFilter<TImage, TAttributeAccessor>
::KeepNLobesComponentTreeFilter()
{
  m_NumberOfLobes = 1;
  m_ReverseOrdering = false;
  m_RemoveByGroup = false;
  m_AddNewLeavesToQueue = true;
}


template<class TImage, class TAttributeAccessor>
void
KeepNLobesComponentTreeFilter<TImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);

  AttributeAccessorType accessor;

  if( !m_ReverseOrdering )
    {
    // setup the priority queue
    HierarchicalQueueType queue;

    // put all the leaves in the queue
    this->PutLeavesInQueue( queue, this->GetOutput()->GetRoot() );
    AttributeType lastAttribute = accessor( this->GetOutput()->GetRoot() );

    // now drop the smallest leaves until the number of leaves is the desired number
    while( !queue.Empty() && ( queue.Size() > m_NumberOfLobes || ( m_RemoveByGroup && lastAttribute == queue.FrontKey() ) ) )
      {
      NodeType * node = queue.FrontValue();
      lastAttribute = queue.FrontKey();
      queue.Pop();
      NodeType * parent = node->GetParent();
    
      // merge the node in its parent
      this->GetOutput()->NodeMerge( parent, node );
      parent->RemoveChild( node );
      delete node;

      if( m_AddNewLeavesToQueue )
        {
        // and add the parent to the queue if it is now a leaf
        // also, take care to never push the root to the queue !
        if( parent->IsLeaf() && !parent->IsRoot() )
          {
            queue.Push( accessor( parent ), parent );
          }
        }
      else
        {
        // merge the parents if they are leaves
        NodeType * n = parent;
        while( n->IsLeaf() && !n->IsRoot() )
          {
          NodeType * p = n->GetParent();
          this->GetOutput()->NodeMerge( p, n );
          p->RemoveChild( n );
          delete n;
          n = p;
          }
        }

      }
    }
  else
    {
    // setup the priority queue
    ReverseHierarchicalQueueType queue;

    // put all the leaves in the queue
    this->PutLeavesInQueue( queue, this->GetOutput()->GetRoot() );
    AttributeType lastAttribute = accessor( this->GetOutput()->GetRoot() );

    // now drop the smallest leaves untill the number of leaves is the desired number
    while( !queue.Empty() && ( queue.Size() > m_NumberOfLobes || ( m_RemoveByGroup && lastAttribute == queue.FrontKey() ) ) )
      {
      NodeType * node = queue.FrontValue();
      lastAttribute = queue.FrontKey();
      queue.Pop();
      NodeType * parent = node->GetParent();
    
      // merge the node in its parent
      this->GetOutput()->NodeMerge( parent, node );
      parent->RemoveChild( node );
      delete node;

      if( m_AddNewLeavesToQueue )
        {
        // and add the parent to the queue if it is now a leaf
        // also, take care to never push the root to the queue !
        if( parent->IsLeaf() && !parent->IsRoot() )
          {
            queue.Push( accessor( parent ), parent );
          }
        }
      else
        {
        // merge the parents if they are leaves
        NodeType * n = parent;
        while( n->IsLeaf() && !n->IsRoot() )
          {
          NodeType * p = n->GetParent();
          this->GetOutput()->NodeMerge( p, n );
          p->RemoveChild( n );
          delete n;
          n = p;
          }
        }

      }
    }
    
  // TODO: how to generate progress ??
}


template<class TImage, class TAttributeAccessor>
void
KeepNLobesComponentTreeFilter<TImage, TAttributeAccessor>
::PutLeavesInQueue( HierarchicalQueueType & queue, NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  if( node->IsLeaf() )
    {
    queue.Push( accessor( node ), node );
    }
  else
    {
    const typename NodeType::ChildrenListType & childrenList = node->GetChildren();
    for( typename NodeType::ChildrenListType::const_iterator it=childrenList.begin(); it!=childrenList.end(); it++ )
      {
      this->PutLeavesInQueue( queue, *it );
      }
    }
}


template<class TImage, class TAttributeAccessor>
void
KeepNLobesComponentTreeFilter<TImage, TAttributeAccessor>
::PutLeavesInQueue( ReverseHierarchicalQueueType & queue, NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  if( node->IsLeaf() )
    {
    queue.Push( accessor( node ), node );
    }
  else
    {
    const typename NodeType::ChildrenListType & childrenList = node->GetChildren();
    for( typename NodeType::ChildrenListType::const_iterator it=childrenList.begin(); it!=childrenList.end(); it++ )
      {
      this->PutLeavesInQueue( queue, *it );
      }
    }
}




template<class TImage, class TAttributeAccessor>
void
KeepNLobesComponentTreeFilter<TImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NumberOfLobes: " << m_NumberOfLobes << std::endl;
  os << indent << "ReverseOrdering: " << m_ReverseOrdering << std::endl;
  os << indent << "RemoveByGroup: " << m_RemoveByGroup << std::endl;
  os << indent << "AddNewLeavesToQueue: " << m_AddNewLeavesToQueue << std::endl;
}
  
}// end namespace itk
#endif
