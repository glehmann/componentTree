/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIntensityVariationComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIntensityVariationComponentTreeFilter_txx
#define __itkIntensityVariationComponentTreeFilter_txx

#include "itkIntensityVariationComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
IntensityVariationComponentTreeFilter<TInputImage, TAttributeAccessor>
::IntensityVariationComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
IntensityVariationComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentIntensityVariation( this->GetOutput()->GetRoot() );

  AttributeAccessorType accessor;
  accessor( this->GetOutput()->GetRoot(), 0 );

  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
void
IntensityVariationComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntensityVariation( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  double localValue = static_cast< double >( node->GetPixel() );

  // set the gradient for all the children
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::const_iterator it;
  for( it=childrenList->begin();  it!=childrenList->end(); it++ )
    {
    double v = static_cast< double >( (*it)->GetPixel() );
    AttributeType res = static_cast< AttributeType >( vcl_abs( v - localValue ) );
    accessor( node, res );

    this->SetComponentIntensityVariation( *it );
    }
}


template<class TInputImage, class TAttributeAccessor>
void
IntensityVariationComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
