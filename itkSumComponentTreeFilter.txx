/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSumComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSumComponentTreeFilter_txx
#define __itkSumComponentTreeFilter_txx

#include "itkSumComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
SumComponentTreeFilter<TInputImage, TAttributeAccessor>
::SumComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
SumComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentIntegratedIntensity( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
//   assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
void
SumComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntegratedIntensity( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  unsigned long size = 0;
  for( typename NodeType::IndexType current=node->GetFirstIndex();
     current != NodeType::EndIndex;
     current = this->GetInput()->GetLinkedListArray()[ current ] )
  {
  size++;
  m_Progress->CompletedPixel();
  }

  AttributeType intensity = static_cast<AttributeType>( size * std::abs( node->GetPixel() ) );
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentIntegratedIntensity( *it );
    intensity += accessor( *it );
    }

  accessor( node, intensity );
}


template<class TInputImage, class TAttributeAccessor>
void
SumComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
