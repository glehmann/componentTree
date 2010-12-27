/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCompactnessComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCompactnessComponentTreeFilter_txx
#define __itkCompactnessComponentTreeFilter_txx

#include "itkCompactnessComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TAttributeAccessor>
CompactnessComponentTreeFilter<TInputImage, TAttributeAccessor>
::CompactnessComponentTreeFilter()
{
}


template<class TInputImage, class TAttributeAccessor>
void
CompactnessComponentTreeFilter<TInputImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  m_PhysicalPixelSize = 1;
  for( int i=0; i<ImageDimension; i++ )
    {
    m_PhysicalPixelSize *= this->GetInput()->GetSpacing()[i];
    }

  this->SetCompactness( this->GetOutput()->GetRoot() );
  delete m_Progress;
  m_Progress = NULL;
  
//   assert( this->IsMonotone( false, false ) );
}


template<class TInputImage, class TAttributeAccessor>
typename CompactnessComponentTreeFilter<TInputImage, TAttributeAccessor>::ChildData
CompactnessComponentTreeFilter<TInputImage, TAttributeAccessor>
::SetCompactness( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  double sum = 0;
  VectorType cog;
  cog.Fill( 0 );
  MatrixType cm;
  cm.Fill( 0 );

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    ChildData ret = this->SetCompactness( *it );
    sum += ret.sum;
    cog += ret.cog;
    cm += ret.cm;
    }

  const PixelType & p = node->GetPixel();
  for( typename NodeType::IndexType current=node->GetFirstIndex();
     current != NodeType::EndIndex;
     current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    IndexType idx = this->GetOutput()->ComputeIndex( current );
    sum += p;
    PointType physicalPosition;
    this->GetOutput()->TransformIndexToPhysicalPoint(idx, physicalPosition);
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      cog[i] += physicalPosition[i] * p; 
      cm[i][i] += p * physicalPosition[i] * physicalPosition[i];
      for(unsigned int j=i+1; j<ImageDimension; j++)
        {
        double weight = p * physicalPosition[i] * physicalPosition[j];
        cm[i][j] += weight;
        cm[j][i] += weight;
        }
      }
    m_Progress->CompletedPixel();
    }

  double compactness = 0.0;
  if( sum != 0.0 )
    {
    MatrixType centralMoments = cm;
    VectorType centerOfGravity = cog;

    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] /= sum;
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] /= sum;
        }
      }
  
    // Center the second order moments
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] -= centerOfGravity[i] * centerOfGravity[j];
        }
      }
//    std::cout << m_CentralMoments << std::endl;
  
    // Compute principal moments and axes
    vnl_symmetric_eigensystem<double> eigen( centralMoments.GetVnlMatrix() );
    const vnl_diag_matrix<double> & pm = eigen.D;
  
    compactness = 1.0;
    if( pm(ImageDimension-1, ImageDimension-1) != 0 )
      {
      compactness = vcl_sqrt( pm(0, 0) / pm(ImageDimension-1, ImageDimension-1) );
      }
    }

  // constrain the value
  if( compactness > 1.0 )
    {
    compactness = 1.0;
//     itkWarningMacro( "compactness greater than 1.0" );
    }
  else if( compactness < 0.0 )
    {
    compactness = 0.0;
//     itkWarningMacro( "compactness less than 0.0" );
    }

  accessor( node, static_cast< AttributeType >( compactness ) );

  return ChildData( sum, cog, cm );

}


template<class TInputImage, class TAttributeAccessor>
void
CompactnessComponentTreeFilter<TInputImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
