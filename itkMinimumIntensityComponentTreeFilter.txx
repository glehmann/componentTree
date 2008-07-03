/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumIntensityComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimumIntensityComponentTreeFilter_txx
#define __itkMinimumIntensityComponentTreeFilter_txx

#include "itkMinimumIntensityComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
MinimumIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::MinimumIntensityComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
MinimumIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentMaximumIntensity( this->GetOutput()->GetRoot() );
  // TODO: how to generate progress ??

}


template<class TInputImage, class TAttributeAccessor>
void
MinimumIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentMaximumIntensity( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  AttributeType mi = NumericTraits<AttributeType>::Zero;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentMaximumIntensity( *it );
    mi = std::min( mi, accessor(*it) );
    }

  mi = std::min( mi, static_cast< AttributeType >( node->GetPixel() ) );

  accessor( node, mi );
}


template<class TInputImage, class TAttributeAccessor>
void
MinimumIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
