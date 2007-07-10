/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSizeComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSizeComponentTreeFilter_txx
#define __itkSizeComponentTreeFilter_txx

#include "itkSizeComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
SizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::SizeComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
SizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentSize( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
void
SizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentSize( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  AttributeType size = 0;
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentSize( *it );
    size += accessor(*it);
    }
    
  // compute the number of indexes of this node
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    size++;
    m_Progress->CompletedPixel();
    }

  accessor( node, size );
  // GetAttribute() is broken, but why ??

  assert( size > 0 );
  // assert( node->GetAttribute() == size );
}


template<class TInputImage, class TAttributeAccessor>
void
SizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
