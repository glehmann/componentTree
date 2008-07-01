/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLocalIntensityComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLocalIntensityComponentTreeFilter_txx
#define __itkLocalIntensityComponentTreeFilter_txx

#include "itkLocalIntensityComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::LocalIntensityComponentTreeFilter()
{
  m_UseZeroLeaves = false;
}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*2);
  if( m_UseZeroLeaves )
    {
    this->SetComponentIntensity2( this->GetOutput()->GetRoot() );
    }
  else
    {
    this->SetComponentIntensity( this->GetOutput()->GetRoot() );
    }
  // TODO: how to generate progress ??

}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntensity( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  AttributeType li = NumericTraits<AttributeType>::Zero;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentIntensity( *it );
    li = std::max( li, accessor(*it) );
    }

  if( node->IsRoot() )
    {
    accessor( node, NumericTraits<AttributeType>::max() );
    }
  else
    {
    PixelType & pixelNode = node->GetPixel();
    PixelType & pixelParent = node->GetParent()->GetPixel();
    if( pixelNode > pixelParent )
      {
      li += node->GetPixel() - node->GetParent()->GetPixel();
      }
    else
      {
      li += node->GetParent()->GetPixel() - node->GetPixel();
      }
    accessor( node, static_cast< AttributeType >( li ) );
    }
}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentIntensity2( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;
  PixelType & pixelNode = node->GetPixel();

  AttributeType li = NumericTraits<AttributeType>::Zero;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->SetComponentIntensity2( *it );
    PixelType & pixelChild = (*it)->GetPixel();
    AttributeType & liChild = accessor(*it);
    if( pixelChild > pixelNode )
      {
      li = std::max( li, static_cast< AttributeType >( liChild + ( pixelChild - pixelNode ) ) );
      }
    else
      {
      li = std::max( li, static_cast< AttributeType >( liChild + ( pixelNode - pixelChild ) ) );
      }
    }
  accessor( node, li );
}


template<class TInputImage, class TAttributeAccessor>
void
LocalIntensityComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "UseZeroLeaves: " << m_UseZeroLeaves << std::endl;
}
  
}// end namespace itk
#endif
