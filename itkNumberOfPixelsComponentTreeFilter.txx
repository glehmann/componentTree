/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumberOfPixelsComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumberOfPixelsComponentTreeFilter_txx
#define __itkNumberOfPixelsComponentTreeFilter_txx

#include "itkNumberOfPixelsComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
NumberOfPixelsComponentTreeFilter<TInputImage, TAttributeAccessor>
::NumberOfPixelsComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfPixelsComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentNumberOfPixels( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfPixelsComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentNumberOfPixels( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  AttributeType nbOfPixels = 0;
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentNumberOfPixels( *it );
    nbOfPixels += accessor(*it);
    }
    
  // compute the number of indexes of this node
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    nbOfPixels++;
    m_Progress->CompletedPixel();
    }

  accessor( node, nbOfPixels );
  // GetAttribute() is broken, but why ??

  assert( nbOfPixels > 0 );
  // assert( node->GetAttribute() == nbOfPixels );
}


template<class TInputImage, class TAttributeAccessor>
void
NumberOfPixelsComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
