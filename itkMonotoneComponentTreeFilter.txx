/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMonotoneComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMonotoneComponentTreeFilter_txx
#define __itkMonotoneComponentTreeFilter_txx

#include "itkMonotoneComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TAccessor>
MonotoneComponentTreeFilter<TInputImage, TAccessor>
::MonotoneComponentTreeFilter()
{
  m_RemoveNodes = false;
  m_ReverseOrdering = false;
  m_StrictlyMonotone = false;
}


template<class TInputImage, class TAccessor>
void
MonotoneComponentTreeFilter<TInputImage, TAccessor>
::GenerateData()
{
  if( m_StrictlyMonotone && !m_RemoveNodes )
    {
    itkWarningMacro( "MonotoneComponentTreeFilter can't make the attributes strictly monotones when it can't remove nodes. You should either set RemoveNodes to true, or set StrictlyMonotone to false." );
    }

  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  // TODO: how to generate progress ??
  
  this->RootFiltering( this->GetOutput()->GetRoot() );

}


template<class TInputImage, class TAccessor>
void
MonotoneComponentTreeFilter<TInputImage, TAccessor>
::RootFiltering( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( this->Compare( accessor(node), accessor(*it) ) )
      {
      if( m_RemoveNodes )
        {
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
        accessor( *it, accessor(node) );
        this->RootFiltering( *it );
        it++;
        }
      }
    else
      {
      this->RootFiltering( *it );
      it++;
      }
    }
}


template<class TInputImage, class TAccessor>
void
MonotoneComponentTreeFilter<TInputImage, TAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "ReverseOrdering: " 
     << static_cast<typename NumericTraits< bool >::PrintType>( m_ReverseOrdering ) << std::endl;
  os << indent << "RemoveNodes: " 
     << static_cast<typename NumericTraits< bool >::PrintType>( m_RemoveNodes ) << std::endl;
  os << indent << "StrictlyMonotone: " 
     << static_cast<typename NumericTraits< bool >::PrintType>( m_StrictlyMonotone ) << std::endl;
}
  
}// end namespace itk
#endif
