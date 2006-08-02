/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSizeComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSizeComponentTreeFilter_txx
#define __itkSizeComponentTreeFilter_txx

#include "itkSizeComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TSize>
SizeComponentTreeFilter<TInputImage, TSize>
::SizeComponentTreeFilter()
{
}


template<class TInputImage, class TSize>
void
SizeComponentTreeFilter<TInputImage, TSize>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  this->SetComponentSize( this->GetOutput()->GetRoot() );

  // TODO: how to generate progress ??

}


template<class TInputImage, class TSize>
void
SizeComponentTreeFilter<TInputImage, TSize>
::SetComponentSize( NodeType* node )
{
  assert(node != NULL);
  SizeType size = 0;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentSize( *it );
    size += (*it)->m_Attribute;
    }
  size += node->GetIndexes().size();
  node->m_Attribute = size;
  assert( size > 0 );
  // GetAttribute() is broken, but why ??
  // assert( node->GetAttribute() == size );
}


template<class TInputImage, class TSize>
void
SizeComponentTreeFilter<TInputImage, TSize>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
