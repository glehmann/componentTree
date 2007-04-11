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

template <class TInputImage>
VolumeLevellingComponentTreeFilter<TInputImage>
::VolumeLevellingComponentTreeFilter()
{
}


template<class TInputImage>
void
VolumeLevellingComponentTreeFilter<TInputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetVolumeLevelling( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
  assert( this->IsMonotone( false, false ) );
}


template<class TInputImage>
typename VolumeLevellingComponentTreeFilter<TInputImage>::SumSize
VolumeLevellingComponentTreeFilter<TInputImage>
::SetVolumeLevelling( NodeType* node )
{
  assert(node != NULL);

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

  node->SetAttribute( static_cast< AttributeType >( sum - size * node->GetPixel() ) );

  return SumSize( sum, size );

}


template<class TInputImage>
void
VolumeLevellingComponentTreeFilter<TInputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
