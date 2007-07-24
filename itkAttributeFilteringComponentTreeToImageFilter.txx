/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeFilteringComponentTreeToImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeFilteringComponentTreeToImageFilter_txx
#define __itkAttributeFilteringComponentTreeToImageFilter_txx

#include "itkAttributeFilteringComponentTreeToImageFilter.h"


namespace itk {

template <class TInputImage, class TOutputImage, class TCompare>
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TCompare>
::AttributeFilteringComponentTreeToImageFilter()
{
  m_Lambda = itk::NumericTraits< AttributeType >::Zero;
}


template<class TInputImage, class TOutputImage, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->LambdaComponents( this->GetInput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
}


template<class TInputImage, class TOutputImage, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TCompare>
::LambdaComponents( const NodeType* node )
{
  assert(node != NULL);
  OutputImagePixelType v = static_cast<OutputImagePixelType>( node->GetPixel() );

  // write the pixels of the current node to ouput image
  OutputImageType* output = this->GetOutput();
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    // std::cout << *it << ": " << v << std::endl;
    output->SetPixel( output->ComputeIndex( current ), v );
    m_Progress->CompletedPixel();
    }
  

  CompareType compare;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    if( compare( (*it)->GetAttribute(), m_Lambda ) )
      {
      // write this subtree to the output image with the pixel value of the current node
      this->WriteNodes( *it, v );
      }
    else
      {
      this->LambdaComponents( *it );
      }
    }
}


template<class TInputImage, class TOutputImage, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TCompare>
::WriteNodes( const NodeType* node, OutputImagePixelType & v )
{
  assert(node != NULL);

  OutputImageType* output = this->GetOutput();

  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    // std::cout << *it << ": " << v << std::endl;
    output->SetPixel( output->ComputeIndex( current ), v );
    m_Progress->CompletedPixel();
    }
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->WriteNodes( *it, v );
    }
}


template<class TInputImage, class TOutputImage, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Lambda: " 
     << static_cast<typename NumericTraits< AttributeType >::PrintType>( m_Lambda ) << std::endl;
}
  
}// end namespace itk
#endif
