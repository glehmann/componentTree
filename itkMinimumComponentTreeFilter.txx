/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimumComponentTreeFilter_txx
#define __itkMinimumComponentTreeFilter_txx

#include "itkMinimumComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
MinimumComponentTreeFilter<TInputImage, TAttributeAccessor>
::MinimumComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
MinimumComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentMinimum( this->GetOutput()->GetRoot() );
  // TODO: how to generate progress ??

}


template<class TInputImage, class TAttributeAccessor>
void
MinimumComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentMinimum( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  AttributeType mi = accessor( node );
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentMinimum( *it );
    mi = std::min( mi, accessor(*it) );
    }

  accessor( node, mi );
}


template<class TInputImage, class TAttributeAccessor>
void
MinimumComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
