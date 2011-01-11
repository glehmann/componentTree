/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRecurssiveMaximumComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRecurssiveMaximumComponentTreeFilter_txx
#define __itkRecurssiveMaximumComponentTreeFilter_txx

#include "itkRecurssiveMaximumComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
RecurssiveMaximumComponentTreeFilter<TInputImage, TAttributeAccessor>
::RecurssiveMaximumComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
RecurssiveMaximumComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentMaximum( this->GetOutput()->GetRoot() );
  // TODO: how to generate progress ??

}


template<class TInputImage, class TAttributeAccessor>
void
RecurssiveMaximumComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentMaximum( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  AttributeType mi = accessor( node );
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentMaximum( *it );
    mi = std::max( mi, accessor(*it) );
    }

  accessor( node, mi );
}


template<class TInputImage, class TAttributeAccessor>
void
RecurssiveMaximumComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
