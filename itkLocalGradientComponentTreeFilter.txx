/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLocalGradientComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLocalGradientComponentTreeFilter_txx
#define __itkLocalGradientComponentTreeFilter_txx

#include "itkLocalGradientComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::LocalGradientComponentTreeFilter()
{
  m_UseZeroLeaves = false;
}


template<class TInputImage, class TAttributeAccessor>
void
LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_PhysicalPixelSize = 1;
  for( int i=0; i<ImageDimension; i++ )
    {
    m_PhysicalPixelSize *= this->GetInput()->GetSpacing()[i];
    }

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());

  if( m_UseZeroLeaves )
    {
    this->SetComponentLocalGradient2( this->GetOutput()->GetRoot() );
    }
  else
    {
    this->SetComponentLocalGradient( this->GetOutput()->GetRoot() );
    }

  delete m_Progress;
  m_Progress = NULL;

}


template<class TInputImage, class TAttributeAccessor>
typename LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>::IntensitySize
LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentLocalGradient( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;

  PixelType li = NumericTraits<PixelType>::Zero;
  unsigned long size = 0;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    IntensitySize res = this->SetComponentLocalGradient( *it );
    li = std::max( li, res.intensity );
    size += res.size;
    }

  if( node->IsRoot() )
    {
    accessor( node, NumericTraits<AttributeType>::Zero );
    }
  else
    {
    for( typename NodeType::IndexType current=node->GetFirstIndex();
      current != NodeType::EndIndex;
      current = this->GetInput()->GetLinkedListArray()[ current ] )
      {
      size++;
      m_Progress->CompletedPixel();
      }

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
    accessor( node, static_cast< AttributeType >( li / vcl_pow( size * m_PhysicalPixelSize, 1.0/ImageDimension ) ) );
    }
  return IntensitySize( li, size );
}


template<class TInputImage, class TAttributeAccessor>
typename LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>::IntensitySize
LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetComponentLocalGradient2( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;
  PixelType & pixelNode = node->GetPixel();

  PixelType li = NumericTraits<PixelType>::Zero;
  unsigned long size = 0;
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    IntensitySize res = this->SetComponentLocalGradient2( *it );
    size += res.size;

    PixelType & pixelChild = (*it)->GetPixel();
    if( pixelChild > pixelNode )
      {
      li = std::max( li, static_cast< PixelType >( res.intensity + ( pixelChild - pixelNode ) ) );
      }
    else
      {
      li = std::max( li, static_cast< PixelType >( res.intensity + ( pixelNode - pixelChild ) ) );
      }
    }

  for( typename NodeType::IndexType current=node->GetFirstIndex();
    current != NodeType::EndIndex;
    current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    size++;
    m_Progress->CompletedPixel();
    }

  accessor( node, static_cast< AttributeType >( li / vcl_pow( size * m_PhysicalPixelSize, 1.0/ImageDimension ) ) );
  return IntensitySize( li, size );
}


template<class TInputImage, class TAttributeAccessor>
void
LocalGradientComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "UseZeroLeaves: " << m_UseZeroLeaves << std::endl;
}
  
}// end namespace itk
#endif
