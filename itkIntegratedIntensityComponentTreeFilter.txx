/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIntegratedIntensityComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIntegratedIntensityComponentTreeFilter_txx
#define __itkIntegratedIntensityComponentTreeFilter_txx

#include "itkIntegratedIntensityComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
IntegratedIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::IntegratedIntensityComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
IntegratedIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentIntensitySize( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
  assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
void
IntegratedIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntensitySize( NodeType* node )
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
    this->SetComponentIntensitySize( *it );
    intensity += accessor( *it );
    }

  accessor( node, intensity );
}


template<class TInputImage, class TAttributeAccessor>
void
IntegratedIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
