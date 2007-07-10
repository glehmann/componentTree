/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVolumeLevellingComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVolumeLevellingComponentTreeFilter_txx
#define __itkVolumeLevellingComponentTreeFilter_txx

#include "itkVolumeLevellingComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
VolumeLevellingComponentTreeFilter<TInputImage, TAttributeAccessor>
::VolumeLevellingComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
VolumeLevellingComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  m_PhysicalPixelSize = 1;
  for( int i=0; i<ImageDimension; i++ )
    {
    m_PhysicalPixelSize *= this->GetInput()->GetSpacing()[i];
    }

  this->SetVolumeLevelling( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
//   assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
typename VolumeLevellingComponentTreeFilter<TInputImage, TAttributeAccessor>::SumSize
VolumeLevellingComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetVolumeLevelling( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  unsigned long size = 0;
  for( typename NodeType::IndexType current=node->GetFirstIndex();
     current != NodeType::EndIndex;
     current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    size++;
    m_Progress->CompletedPixel();
    }

  double sum = size * node->GetPixel();
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    SumSize ret = this->SetVolumeLevelling( *it );
    sum += ret.sum;
    size += ret.size;
    }

  accessor( node, static_cast< AttributeType >( sum - size * m_PhysicalPixelSize * node->GetPixel() ) );

  return SumSize( sum, size );

}


template<class TInputImage, class TAttributeAccessor>
void
VolumeLevellingComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
