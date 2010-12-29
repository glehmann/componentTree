/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGranulometryComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGranulometryComponentTreeFilter_txx
#define __itkGranulometryComponentTreeFilter_txx

#include "itkGranulometryComponentTreeFilter.h"
#include "itkProgressReporter.h"
#include <map>


namespace itk {

template <class TImage>
GranulometryComponentTreeFilter<TImage>
::GranulometryComponentTreeFilter()
{
  m_ReverseOrdering = false;
}


template<class TImage>
void
GranulometryComponentTreeFilter<TImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);

  if( !m_ReverseOrdering )
    {
    // setup the priority queue
    HierarchicalQueueType queue;

    // put all the leaves in the queue
    this->PutLeavesInQueue( queue, this->GetOutput()->GetRoot() );

    // the data structure to store the result of the granulometric analysis
    typename std::map< AttributeType, double > MapType;
    MapType result;

    // now drop the smallest leaves untill the number of leaves is the desired number
    while( !queue.Empty() )
      {
      NodeType * node = queue.FrontValue();
      queue.Pop();
      NodeType * parent = node->GetParent();
      
      // count how much difference it will make to remove that node
      long nodeSize = this->GetOutput()->NodeCountIndexes( node );
      double nodeDifference = nodeSize * std::abs( (double)node->GetPixel() - (double)parent->GetPixel() );
      
      result[ node->GetAttribute() ] += nodeDifference;

      // merge the node in its parent
      this->GetOutput()->NodeMerge( parent, node );
      parent->RemoveChild( node );
      delete node;

      // and add the parent to the queue if it is now a leaf
      // also, take care to never push the root to the queue !
      if( parent->IsLeaf() && !parent->IsRoot() )
        {
        queue.Push( parent->GetAttribute(), parent );
        }
      }
    }
  else
    {
/*    // setup the priority queue
    ReverseHierarchicalQueueType queue;

    // put all the leaves in the queue
    this->PutLeavesInQueue( queue, this->GetOutput()->GetRoot() );

    // now drop the smallest leaves untill the number of leaves is the desired number
    while( queue.Size() > m_NumberOfLobes && !queue.Empty() )
      {
      NodeType * node = queue.FrontValue();
      queue.Pop();
      NodeType * parent = node->GetParent();
    
      // merge the node in its parent
      this->GetOutput()->NodeMerge( parent, node );
      parent->RemoveChild( node );
      delete node;

      // and add the parent to the queue if it is now a leaf
      // also, take care to never push the root to the queue !
      if( parent->IsLeaf() && !parent->IsRoot() )
        {
        queue.Push( parent->GetAttribute(), parent );
        }
      }*/
    }
    
  // TODO: how to generate progress ??
}


template<class TImage>
void
GranulometryComponentTreeFilter<TImage>
::PutLeavesInQueue( HierarchicalQueueType & queue, NodeType* node )
{
  assert(node != NULL);

  if( node->IsLeaf() )
    {
    queue.Push( node->GetAttribute(), node );
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


template<class TImage>
void
GranulometryComponentTreeFilter<TImage>
::PutLeavesInQueue( ReverseHierarchicalQueueType & queue, NodeType* node )
{
  assert(node != NULL);

  if( node->IsLeaf() )
    {
    queue.Push( node->GetAttribute(), node );
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




template<class TImage>
void
GranulometryComponentTreeFilter<TImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
