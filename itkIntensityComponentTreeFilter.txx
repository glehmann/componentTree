/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIntensityComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIntensityComponentTreeFilter_txx
#define __itkIntensityComponentTreeFilter_txx

#include "itkIntensityComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TSize>
IntensityComponentTreeFilter<TInputImage, TSize>
::IntensityComponentTreeFilter()
{
}


template<class TInputImage, class TSize>
void
IntensityComponentTreeFilter<TInputImage, TSize>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentIntensity( this->GetOutput()->GetRoot() );

  // TODO: how to generate progress ??

}


template<class TInputImage, class TSize>
void
IntensityComponentTreeFilter<TInputImage, TSize>
::SetComponentIntensity( NodeType* node )
{
  assert(node != NULL);
  IntensityType intensity = NumericTraits<IntensityType>::Zero;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentIntensity( *it );
    intensity = std::max( intensity, (*it)->m_Attribute );
    }
  if( node->IsRoot() )
    {
    node->m_Attribute = NumericTraits<IntensityType>::max();
    }
  else
    {
    node->m_Attribute = intensity + ( node->GetPixel() - node->GetParent()->GetPixel() );
    }
}


template<class TInputImage, class TSize>
void
IntensityComponentTreeFilter<TInputImage, TSize>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
