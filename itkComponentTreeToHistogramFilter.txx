/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkComponentTreeToHistogramFilter_txx
#define __itkComponentTreeToHistogramFilter_txx

#include "itkComponentTreeToHistogramFilter.h"

namespace itk
{
template< class TImage, class TAccessor >
ComponentTreeToHistogramFilter< TImage, TAccessor >
::ComponentTreeToHistogramFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->ProcessObject::SetNthOutput( 0, this->MakeOutput(0) );
  
  this->SetAutoMinimumMaximum(true);
  
  typename HistogramType::SizeType histogramSize;
  histogramSize.SetSize(1);
  histogramSize.Fill(128);
  this->SetHistogramSize(histogramSize);
  
  typename HistogramType::MeasurementVectorType featureImageMin;
  featureImageMin.SetSize(1);
  featureImageMin.Fill(itk::NumericTraits<AttributeType>::NonpositiveMin());
  this->SetHistogramBinMinimum( featureImageMin );

  typename HistogramType::MeasurementVectorType featureImageMax;
  featureImageMax.SetSize(1);
  featureImageMax.Fill(itk::NumericTraits<AttributeType>::max());
  this->SetHistogramBinMaximum( featureImageMax );
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::SetInput(const ImageType *image)
{
  this->ProcessObject::SetNthInput( 0, const_cast< ImageType * >( image ) );
}

template< class TImage, class TAccessor >
const TImage *
ComponentTreeToHistogramFilter< TImage, TAccessor >
::GetInput() const
{
  if ( this->GetNumberOfInputs() < 1 )
    {
    return 0;
    }

  return static_cast< const ImageType * >( this->ProcessObject::GetInput(0) );
}

template< class TImage, class TAccessor >
DataObject::Pointer
ComponentTreeToHistogramFilter< TImage, TAccessor >
::MakeOutput( unsigned int itkNotUsed(idx) )
{
  typename HistogramType::Pointer output = HistogramType::New();
  return static_cast< DataObject * >( output );
}

template< class TImage, class TAccessor >
const typename ComponentTreeToHistogramFilter< TImage, TAccessor >::HistogramType *
ComponentTreeToHistogramFilter< TImage, TAccessor >
::GetOutput() const
{
  const HistogramType *output =
    static_cast< const HistogramType * >( this->ProcessObject::GetOutput(0) );

  return output;
}

template< class TImage, class TAccessor >
typename ComponentTreeToHistogramFilter< TImage, TAccessor >::HistogramType *
ComponentTreeToHistogramFilter< TImage, TAccessor >
::GetOutput()
{
  HistogramType *output =
    static_cast< HistogramType * >( this->ProcessObject::GetOutput(0) );

  return output;
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::GraftOutput(DataObject *graft)
{
  DataObject *output =
    const_cast< HistogramType * >( this->GetOutput() );

  // Call Histogram to copy meta-information, and the container
  output->Graft(graft);
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::GenerateData()
{
  ProgressReporter progress(this, 0, 1);
//  this->AllocateOutputs();
  

  typename HistogramType::MeasurementVectorType featureImageMin;
  featureImageMin.SetSize(1);
  typename HistogramType::MeasurementVectorType featureImageMax;
  featureImageMax.SetSize(1);
  if( this->GetAutoMinimumMaximum() )
    {
    m_Minimum = itk::NumericTraits<AttributeType>::NonpositiveMin();
    m_Maximum = itk::NumericTraits<AttributeType>::max();
    this->ComputeRange( this->GetInput()->GetRoot() );
    featureImageMin.Fill(m_Minimum);
    featureImageMax.Fill(m_Maximum);
    }
  else
    {
    featureImageMin = this->GetHistogramBinMinimum();
    featureImageMax = this->GetHistogramBinMaximum();
    }

  typename HistogramType::Pointer histogram = this->GetOutput();
  histogram->SetMeasurementVectorSize(1);
  histogram->SetClipBinsAtEnds(false);
  histogram->Initialize( this->GetHistogramSize(), featureImageMin, featureImageMax);

  this->ComputeHistogram( this->GetInput()->GetRoot() );
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::ComputeRange( const NodeType * node )
{
  AttributeAccessorType accessor;
  m_Minimum = std::min( m_Minimum, accessor( node ) );
  m_Maximum = std::max( m_Maximum, accessor( node ) );
  
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->ComputeRange( *it );
    }
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::ComputeHistogram( const NodeType * node )
{
  AttributeAccessorType accessor;
  typename HistogramType::MeasurementVectorType mv;
  mv.SetSize(1);
  mv[0] = accessor( node );
  this->GetOutput()->IncreaseFrequencyOfMeasurement( mv, 1 );

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    this->ComputeHistogram( *it );
    }
  
}

template< class TImage, class TAccessor >
void
ComponentTreeToHistogramFilter< TImage, TAccessor >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
   // m_HistogramBinMinimum
   os << indent << "HistogramBinMinimum: " << this->GetHistogramBinMinimum() << std::endl;
   // m_HistogramBinMaximum
   os << indent << "HistogramBinMaximum: " << this->GetHistogramBinMaximum() << std::endl;
//   // m_MarginalScale
//   os << indent << "MarginalScale: " << this->GetMarginalScaleInput() << std::endl;
  // m_AutoMinimumMaximum
  os << indent << "AutoMinimumMaximum: " << this->GetAutoMinimumMaximum() << std::endl;
  // m_HistogramSize
  os << indent << "HistogramSize: " << this->GetHistogramSize() << std::endl;
}
} // end of namespace itk

#endif
