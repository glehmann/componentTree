/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeAttributeToImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeAttributeToImageFilter_txx
#define __itkComponentTreeAttributeToImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkComponentTreeAttributeToImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <algorithm>
#include <list>


namespace itk {

template <class TInputImage, class TOutputImage>
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
::ComponentTreeAttributeToImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void 
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
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
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
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
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
::WriteNodes( const NodeType* node )
{
  assert(node != NULL);
  OutputImagePixelType v = static_cast<OutputImagePixelType>( node->GetAttribute() );
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
    this->WriteNodes( *it );
    }
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeAttributeToImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
  
}// end namespace itk
#endif
