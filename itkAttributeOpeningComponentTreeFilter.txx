/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeOpeningComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeOpeningComponentTreeFilter_txx
#define __itkAttributeOpeningComponentTreeFilter_txx

#include "itkAttributeOpeningComponentTreeFilter.h"


namespace itk {

template <class TInputImage, class TOutputImage>
AttributeOpeningComponentTreeFilter<TInputImage, TOutputImage>
::AttributeOpeningComponentTreeFilter()
{
	m_Threshold = 0;
}


template<class TInputImage, class TOutputImage>
void
AttributeOpeningComponentTreeFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // this->GetOutput()->GetRoot()->MergeChildren();
  this->ThresholdComponents( this->GetOutput()->GetRoot() );

}


template<class TInputImage, class TOutputImage>
void
AttributeOpeningComponentTreeFilter<TInputImage, TOutputImage>
::ThresholdComponents( NodeType* node )
{
	assert(node != NULL);
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildrenList();
	for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
	  if( (*it)->CountPixels() <= m_Threshold )
	    {
		  (*it)->MergeChildren();
		  node->Merge( *it );
		  node->Remove( *it );
      }
    else
      {
			this->ThresholdComponents( *it );
      }
	  }
}


template<class TInputImage, class TOutputImage>
void
AttributeOpeningComponentTreeFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
