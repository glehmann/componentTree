/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLeafComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLeafComponentTreeFilter_txx
#define __itkLeafComponentTreeFilter_txx

#include "itkLeafComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
LeafComponentTreeFilter<TInputImage, TAttributeAccessor>
::LeafComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
LeafComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentLeaf( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
void
LeafComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentLeaf( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentLeaf( *it );
    }
    
  accessor( node, node->IsLeaf() );
}


template<class TInputImage, class TAttributeAccessor>
void
LeafComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
