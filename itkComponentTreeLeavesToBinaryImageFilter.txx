/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeLeavesToBinaryImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeLeavesToBinaryImageFilter_txx
#define __itkComponentTreeLeavesToBinaryImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkComponentTreeLeavesToBinaryImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <algorithm>
#include <list>


namespace itk {

template <class TInputImage, class TOutputImage>
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
::ComponentTreeLeavesToBinaryImageFilter()
{
  m_ForegroundValue = NumericTraits< OutputImagePixelType >::max();
  m_BackgroundValue = NumericTraits< OutputImagePixelType >::Zero;
}

template <class TInputImage, class TOutputImage>
void 
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
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
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->WriteNodes( this->GetInput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
    
  
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
::WriteNodes( const NodeType* node )
{
  assert(node != NULL);

  OutputImageType* output = this->GetOutput();

  if( node->IsLeaf() )
    {
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    // std::cout << *it << ": " << v << std::endl;
    output->SetPixel( output->ComputeIndex( current ), m_ForegroundValue );
    m_Progress->CompletedPixel();
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
      this->WriteNodes( *it );
      }
    }
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeLeavesToBinaryImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
  
}// end namespace itk
#endif
