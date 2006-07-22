/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeFilteringComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeFilteringComponentTreeFilter_txx
#define __itkAttributeFilteringComponentTreeFilter_txx

#include "itkAttributeFilteringComponentTreeFilter.h"


namespace itk {

template <class TInputImage, class TAttribute, class TCompare>
AttributeFilteringComponentTreeFilter<TInputImage, TAttribute, TCompare>
::AttributeFilteringComponentTreeFilter()
{
	m_Threshold = itk::NumericTraits<TAttribute>::Zero;
}


template<class TInputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAttribute, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // this->GetOutput()->GetRoot()->MergeChildren();
  this->ThresholdComponents( this->GetOutput()->GetRoot() );

}


template<class TInputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAttribute, TCompare>
::ThresholdComponents( NodeType* node )
{
	assert(node != NULL);
	TCompare compare;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
	for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
	  if( compare( (*it)->GetAttribute(), m_Threshold ) )
	    {
		  (*it)->MergeChildren();
		  node->Merge( *it );
		  node->RemoveChild( *it );
      }
    else
      {
			this->ThresholdComponents( *it );
      }
	  }
}


template<class TInputImage, class TAttribute, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAttribute, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif