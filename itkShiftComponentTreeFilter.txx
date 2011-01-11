/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShiftComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShiftComponentTreeFilter_txx
#define __itkShiftComponentTreeFilter_txx

#include "itkShiftComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
ShiftComponentTreeFilter<TInputImage, TAttributeAccessor>
::ShiftComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
ShiftComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetAttributeValue( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
//   assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
void
ShiftComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetAttributeValue( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetAttributeValue( *it );
    accessor( *it, accessor( node ) );
    }
}

}// end namespace itk
#endif
