/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGradientComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientComponentTreeFilter_txx
#define __itkGradientComponentTreeFilter_txx

#include "itkGradientComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
GradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::GradientComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
GradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetComponentGradient( this->GetOutput()->GetRoot() );

  AttributeAccessorType accessor;
  accessor( this->GetOutput()->GetRoot(), 0 );

  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TAttributeAccessor>
unsigned long
GradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentGradient( NodeType* node )
{
  assert(node != NULL);
  
  AttributeAccessorType accessor;

  unsigned long size = 0;

  // need to store locally the size of the children
  typedef typename std::deque< unsigned long > SizesType;
  SizesType sizes;

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    unsigned long s = this->SetComponentGradient( *it );
    size += s;
    sizes.push_back( s );
    }
    
  // compute the number of indexes of this node
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    size++;
    m_Progress->CompletedPixel();
    }

  double localValue = static_cast< double >( node->GetPixel() );
  double rootSize = vcl_pow( static_cast< double >( size ), 1.0/ImageDimension );

  // set the gradient for all the children
  typename NodeType::ChildrenListType::const_iterator it;
  typename SizesType::const_iterator sIt = sizes.begin();
  for( it=childrenList->begin();
    it!=childrenList->end(), sIt != sizes.end();
    it++, sIt++ )
    {
    double v = static_cast< double >( (*it)->GetPixel() );
    double s = *sIt;

    double sDiff = rootSize - vcl_pow( s, 1.0/ImageDimension );
    double res = vcl_abs( v - localValue ) / sDiff;

    accessor( *it, static_cast< AttributeType >( res ) );
    }

  assert( size > 0 );

  return size;
}


template<class TInputImage, class TAttributeAccessor>
void
GradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
