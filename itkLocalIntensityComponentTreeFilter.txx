/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLocalIntensityComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLocalIntensityComponentTreeFilter_txx
#define __itkLocalIntensityComponentTreeFilter_txx

#include "itkLocalIntensityComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::LocalIntensityComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentIntensity( this->GetOutput()->GetRoot() );

  // TODO: how to generate progress ??

}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntensity( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  AttributeType intensity = NumericTraits<AttributeType>::Zero;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentIntensity( *it );
    intensity = std::max( intensity, accessor(*it) );
    }
  if( node->IsRoot() )
    {
    accessor( node, NumericTraits<AttributeType>::max() );
    }
  else
    {
    accessor( node, static_cast< AttributeType >( intensity + ( node->GetPixel() - node->GetParent()->GetPixel() ) ) );
    }
}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
