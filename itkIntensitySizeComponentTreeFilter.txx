/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIntensitySizeComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIntensitySizeComponentTreeFilter_txx
#define __itkIntensitySizeComponentTreeFilter_txx

#include "itkIntensitySizeComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TSize>
IntensitySizeComponentTreeFilter<TInputImage, TSize>
::IntensitySizeComponentTreeFilter()
{
}


template<class TInputImage, class TSize>
void
IntensitySizeComponentTreeFilter<TInputImage, TSize>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentIntensitySize( this->GetOutput()->GetRoot() );

  // TODO: how to generate progress ??

}


template<class TInputImage, class TSize>
typename IntensitySizeComponentTreeFilter<TInputImage, TSize>::IntensitySize
IntensitySizeComponentTreeFilter<TInputImage, TSize>
::SetComponentIntensitySize( NodeType* node )
{
  assert(node != NULL);

  PixelType intensity = NumericTraits<PixelType>::Zero;
  unsigned long size = node->GetIndexes().size();

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    IntensitySize is = this->SetComponentIntensitySize( *it );
    intensity = std::max( intensity, is.intensity );
    size += is.size;
    }

  if( node->GetParent() )
    {
    intensity = intensity + ( node->GetPixel() - node->GetParent()->GetPixel() );
    }
  else
    {
    intensity = NumericTraits<PixelType>::max();
    }

  node->m_Attribute = static_cast< AttributeType >( intensity * size );

  return IntensitySize( intensity, size );
}


template<class TInputImage, class TSize>
void
IntensitySizeComponentTreeFilter<TInputImage, TSize>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
