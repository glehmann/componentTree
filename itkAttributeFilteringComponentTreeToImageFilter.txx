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

template <class TInputImage, class TOutputImage, class TAttribute, class TCompare>
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TAttribute, TCompare>
::AttributeFilteringComponentTreeToImageFilter()
{
	m_Threshold = itk::NumericTraits<TAttribute>::Zero;
}


template<class TInputImage, class TOutputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TAttribute, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // this->GetOutput()->GetRoot()->MergeChildren();
  this->ThresholdComponents( this->GetInput()->GetRoot() );

}


template<class TInputImage, class TOutputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TAttribute, TCompare>
::ThresholdComponents( NodeType* node )
{
  assert(node != NULL);
  OutputImagePixelType v = static_cast<OutputImagePixelType>( node->GetPixel() );

  // write the pixels of the current node to ouput image
  const typename NodeType::IndexListType * indexList = & node->GetIndexes();
  OutputImageType* output = this->GetOutput();
  for( typename NodeType::IndexListType::const_iterator it=indexList->begin(); it!=indexList->end(); it++ )
    {
    // std::cout << *it << ": " << v << std::endl;
    output->SetPixel( *it, v );
    }

  TCompare compare;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    if( compare( (*it)->m_Attribute, m_Threshold ) )
      {
      // write this subtree to the output image with the pixel value of the current node
      this->WriteNodes( *it, v );
      }
    else
      {
      this->ThresholdComponents( *it );
      }
    }
}


template<class TInputImage, class TOutputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TAttribute, TCompare>
::WriteNodes( const NodeType* node, OutputImagePixelType & v )
{
  assert(node != NULL);
  const typename NodeType::IndexListType * indexList = & node->GetIndexes();
  OutputImageType* output = this->GetOutput();
  for( typename NodeType::IndexListType::const_iterator it=indexList->begin(); it!=indexList->end(); it++ )
    {
    output->SetPixel( *it, v );
    }
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->WriteNodes( *it, v );
    }
}


template<class TInputImage, class TOutputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeToImageFilter<TInputImage, TOutputImage, TAttribute, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
