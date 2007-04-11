/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeLeavesToLabelImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeLeavesToLabelImageFilter_txx
#define __itkComponentTreeLeavesToLabelImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkComponentTreeLeavesToLabelImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <algorithm>
#include <list>


namespace itk {

template <class TInputImage, class TOutputImage>
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::ComponentTreeLeavesToLabelImageFilter()
{
  m_BackgroundValue = NumericTraits< OutputImagePixelType >::Zero;
}

template <class TInputImage, class TOutputImage>
void 
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  if ( !input )
    { return; }

  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  OutputImagePixelType firstLabel = NumericTraits< OutputImagePixelType >::Zero;
  if( firstLabel == m_BackgroundValue )
    {
    firstLabel++;
    }

  this->WriteNodes( this->GetInput()->GetRoot(), firstLabel );

  delete m_Progress;
  m_Progress = NULL;
  
}


template<class TInputImage, class TOutputImage>
typename ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>::OutputImagePixelType
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::WriteNodes( const NodeType* node, const OutputImagePixelType & label )
{
  assert(node != NULL);

  OutputImageType* output = this->GetOutput();
  OutputImagePixelType nextLabel = label;

  if( node->IsLeaf() )
    {
    for( typename NodeType::IndexType current=node->GetFirstIndex();
        current != NodeType::EndIndex;
        current = this->GetInput()->GetLinkedListArray()[ current ] )
      {
      // std::cout << *it << ": " << v << std::endl;
      output->SetPixel( output->ComputeIndex( current ), label );
      m_Progress->CompletedPixel();
      }

    nextLabel++;
    if( nextLabel == m_BackgroundValue )
      {
      nextLabel++;
      }

    }
  else
    {
    for( typename NodeType::IndexType current=node->GetFirstIndex();
        current != NodeType::EndIndex;
        current = this->GetInput()->GetLinkedListArray()[ current ] )
      {
      // std::cout << *it << ": " << v << std::endl;
      output->SetPixel( output->ComputeIndex( current ), m_BackgroundValue );
      m_Progress->CompletedPixel();
      }

    const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
    for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
      {
      nextLabel = this->WriteNodes( *it, nextLabel );
      }
    }

  return nextLabel;

}


template<class TInputImage, class TOutputImage>
void
ComponentTreeLeavesToLabelImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
  
}// end namespace itk
#endif
