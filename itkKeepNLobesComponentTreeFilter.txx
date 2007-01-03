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

template <class TImage>
KeepNLobesComponentTreeFilter<TImage>
::KeepNLobesComponentTreeFilter()
{
  m_NumberOfLobes = 1;
}


template<class TImage>
void
KeepNLobesComponentTreeFilter<TImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);

  // setup the priority queue
  m_Queue = new PriorityQueueType();

  // put all the leaves in the queue
  this->PutLeavesInQueue( this->GetOutput()->GetRoot() );

  // now drop the smallest leaves untill the number of leaves is the desired number
  while( m_Queue->Size() > m_NumberOfLobes && !m_Queue->Empty() )
    {
    NodeType * node = m_Queue->FrontValue();
    m_Queue->Pop();
    NodeType * parent = node->GetParent();
    
    // merge the node in its parent
    this->GetOutput()->NodeMerge( parent, node );
    parent->RemoveChild( node );
    delete node;

    // and add the parent to the queue if it is now a leaf
    // also, take care to never push the root to the queue !
    if( parent->IsLeaf() && !parent->IsRoot() )
      {
      m_Queue->Push( parent->GetAttribute(), parent );
      }
    }

  // TODO: how to generate progress ??
  delete m_Queue;
  m_Queue = NULL;

}


template<class TImage>
void
KeepNLobesComponentTreeFilter<TImage>
::PutLeavesInQueue( NodeType* node )
{
  assert(node != NULL);
  assert(m_Queue != NULL);

  if( node->IsLeaf() )
    {
    m_Queue->Push( node->GetAttribute(), node );
    }
  else
    {
    const typename NodeType::ChildrenListType & childrenList = node->GetChildren();
    for( typename NodeType::ChildrenListType::const_iterator it=childrenList.begin(); it!=childrenList.end(); it++ )
      {
      this->PutLeavesInQueue( *it );
      }
    }
}




template<class TImage>
void
KeepNLobesComponentTreeFilter<TImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NumberOfLobes: " << m_NumberOfLobes << std::endl;
}
  
}// end namespace itk
#endif
