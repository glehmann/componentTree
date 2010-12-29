/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeComponentTreeFilter_txx
#define __itkShapeComponentTreeFilter_txx

#include "itkShapeComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TLabelObjectAccessor, class TAttributeAccessor>
ShapeComponentTreeFilter<TInputImage, TLabelObjectAccessor, TAttributeAccessor>
::ShapeComponentTreeFilter()
{
  m_ShapeLabelMapFilter = NULL;
  m_LabelMap = NULL;
}


template<class TInputImage, class TLabelObjectAccessor, class TAttributeAccessor>
void
ShapeComponentTreeFilter<TInputImage, TLabelObjectAccessor, TAttributeAccessor>
::GenerateData()
{

  // create the filter which will be use to compute all the values of interest
  m_ShapeLabelMapFilter = ShapeLabelMapFilterType::New();
  // make sure it will run inplace
  m_ShapeLabelMapFilter->SetInPlace( true );
  // provide it an input label map
  m_LabelMap = LabelMapType::New();
  m_LabelMap->SetRegions( this->GetInput()->GetLargestPossibleRegion() );
  m_LabelMap->SetSpacing( this->GetInput()->GetSpacing() );
  m_LabelMap->SetOrigin( this->GetInput()->GetOrigin() );
  m_LabelMap->SetDirection( this->GetInput()->GetDirection() );
  m_LabelMap->Allocate();
  m_ShapeLabelMapFilter->SetInput( m_LabelMap );
  
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  this->SetAttribute( this->GetOutput()->GetRoot() );

  AttributeAccessorType accessor;
  accessor( this->GetOutput()->GetRoot(), 0 );

  delete m_Progress;
  m_Progress = NULL;

  // we don't need that filter anymore - its input and output will be destroyed
  // with it
  m_ShapeLabelMapFilter = NULL;
  m_LabelMap = NULL;
}


template<class TInputImage, class TLabelObjectAccessor, class TAttributeAccessor>
typename TLabelObjectAccessor::LabelObjectType::Pointer
ShapeComponentTreeFilter<TInputImage, TLabelObjectAccessor, TAttributeAccessor>
::SetAttribute( NodeType* node )
{
  assert(node != NULL);
  AttributeAccessorType accessor;
  LabelObjectAccessorType labelObjectAccessor;
  
  typename LabelObjectType::Pointer labelObject = LabelObjectType::New();
  labelObject->SetLabel( 1 );

  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    typename LabelObjectType::Pointer childLabelObject = this->SetAttribute( *it );
    
    // put all the lines from the child into the label object of the current node
    typedef typename LabelObjectType::LineContainerType  LineContainerType;
    typedef typename LineContainerType::const_iterator   LineContainerIterator;
    const LineContainerType & lineContainer = childLabelObject->GetLineContainer();
    LineContainerIterator lit = lineContainer.begin();
    while ( lit != lineContainer.end() )
      {
      labelObject->AddLine(*lit);
      lit++;
      }
    }
    
  // compute the number of indexes of this node
  for( typename NodeType::IndexType current=node->GetFirstIndex();
       current != NodeType::EndIndex;
       current = this->GetInput()->GetLinkedListArray()[ current ] )
    {
    labelObject->AddIndex( this->GetOutput()->ComputeIndex( current ) );
    m_Progress->CompletedPixel();
    }

  // make sure to have the lines well organized
  labelObject->Optimize();

  // then put that object in the input of the ShapeLabelMapFilter
  m_LabelMap->ClearLabels();
  m_LabelMap->AddLabelObject( labelObject );
  m_ShapeLabelMapFilter->Update();
  
  accessor( node, labelObjectAccessor( labelObject ) );
  
  return labelObject;
}


template<class TInputImage, class TLabelObjectAccessor, class TAttributeAccessor>
void
ShapeComponentTreeFilter<TInputImage, TLabelObjectAccessor, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
