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
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TCompare>
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::AttributeFilteringComponentTreeFilter()
{
  m_Threshold = itk::NumericTraits< AttributeType >::Zero;
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->ThresholdComponents( this->GetOutput()->GetRoot() );

  // TODO: how to generate progress ??

}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::ThresholdComponents( NodeType* node )
{
  assert(node != NULL);
  CompareType compare;
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( compare( (*it)->GetAttribute(), m_Threshold ) )
      {
      this->GetOutput()->NodeFlatten( *it );
      this->GetOutput()->NodeMerge( node, *it );
      // must store the iterator, because once the element
      // erased, it is invalidated
      typename NodeType::ChildrenListType::iterator toRemove = it;
      it++;
      childrenList->erase( toRemove );
      delete *toRemove;
      }
    else
      {
      this->ThresholdComponents( *it );
      it++;
      }
    }
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Threshold: " 
     << static_cast<typename NumericTraits< AttributeType >::PrintType>( m_Threshold ) << std::endl;
}
  
}// end namespace itk
#endif
