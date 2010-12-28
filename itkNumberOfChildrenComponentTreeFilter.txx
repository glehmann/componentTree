/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumberOfChildrenComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumberOfChildrenComponentTreeFilter_txx
#define __itkNumberOfChildrenComponentTreeFilter_txx

#include "itkNumberOfChildrenComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
NumberOfChildrenComponentTreeFilter<TInputImage, TAttributeAccessor>
::NumberOfChildrenComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfChildrenComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, 1);
  this->SetComponentNumberOfChildren( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfChildrenComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentNumberOfChildren( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  AttributeType nbOfChildren = 0;
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentNumberOfChildren( *it );
    nbOfChildren++;
    }
    
  accessor( node, nbOfChildren );
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfChildrenComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
