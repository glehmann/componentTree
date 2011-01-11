/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRecurssiveMathComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRecurssiveMathComponentTreeFilter_txx
#define __itkRecurssiveMathComponentTreeFilter_txx

#include "itkRecurssiveMathComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
RecurssiveMathComponentTreeFilter<TInputImage, TAttributeAccessor>
::RecurssiveMathComponentTreeFilter()
{
}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
RecurssiveMathComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->GenerateAttributeValue( this->GetOutput()->GetRoot() );
  // TODO: how to generate progress ??

}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
RecurssiveMathComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateAttributeValue( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;
  MathFunctorType compute;

  AttributeType mi = accessor( node );
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->GenerateAttributeValue( *it );
    mi = compute( mi, accessor(*it) );
    }

  accessor( node, mi );
}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
RecurssiveMathComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
