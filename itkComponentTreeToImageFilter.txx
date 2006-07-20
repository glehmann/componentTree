/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeToImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeToImageFilter_txx
#define __itkComponentTreeToImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkComponentTreeToImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <algorithm>
#include <list>
#include "itkPreOrderTreeIterator.h"


namespace itk {

template <class TInputImage, class TOutputImage>
ComponentTreeToImageFilter<TInputImage, TOutputImage>
::ComponentTreeToImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void 
ComponentTreeToImageFilter<TInputImage, TOutputImage>
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
ComponentTreeToImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeToImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  this->WriteNodes( this->GetInput()->GetRoot() );
    
  
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeToImageFilter<TInputImage, TOutputImage>
::WriteNodes( const NodeType* node )
{
	assert(node != NULL);
	OutputImagePixelType v = node->GetPixel();
	const typename NodeType::IndexListType * indexList = & node->GetIndexList();
	OutputImageType* output = this->GetOutput();
  for( typename NodeType::IndexListType::const_iterator it=indexList->begin(); it!=indexList->end(); it++ )
    {
	  // std::cout << *it << ": " << v << std::endl;
	  output->SetPixel( *it, v );
    }
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildrenList();
	for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
		this->WriteNodes( *it );
	  }
}


template<class TInputImage, class TOutputImage>
void
ComponentTreeToImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
  
}// end namespace itk
#endif
