/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalizeComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNormalizeComponentTreeFilter_txx
#define __itkNormalizeComponentTreeFilter_txx

#include "itkNormalizeComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
NormalizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::NormalizeComponentTreeFilter()
{
  m_Maximum = 0.0;
}


template<class TInputImage, class TAttributeAccessor>
void
NormalizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  m_Maximum = itk::NumericTraits<double>::NonpositiveMin();
  this->Compute( this->GetOutput()->GetRoot() );
  
  this->Scale( this->GetOutput()->GetRoot() );
  
  delete m_Progress;
  m_Progress = NULL;
  
//   assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
void
NormalizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::Compute( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;
  m_Maximum = std::max( m_Maximum, static_cast<double>( accessor( node ) ) );

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->Compute( *it );
    }
}


template<class TInputImage, class TAttributeAccessor>
void
NormalizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::Scale( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;
  accessor( node, static_cast<AttributeType>( accessor( node ) / m_Maximum ) );

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->Scale( *it );
    }
}


template<class TInputImage, class TAttributeAccessor>
void
NormalizeComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
  os << indent << "Maximum: " << m_Maximum << std::endl;
}
  
}// end namespace itk
#endif
