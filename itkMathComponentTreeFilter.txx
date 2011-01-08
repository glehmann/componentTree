/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMathComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMathComponentTreeFilter_txx
#define __itkMathComponentTreeFilter_txx

#include "itkMathComponentTreeFilter.h"


namespace itk {

template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
MathComponentTreeFilter<TInputImage, TMathFunctor, TAttributeAccessor>
::MathComponentTreeFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
MathComponentTreeFilter<TInputImage, TMathFunctor, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  m_Progress = new ProgressReporter(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  
  // build the list of root node to process
  NodeArrayType nodes;
  for( unsigned int i=1; i<this->GetNumberOfInputs(); i++ )
    {
    nodes.push_back( this->GetInput(i)->GetRoot() );
    }
  
  this->SetAttribute( this->GetOutput()->GetRoot(), nodes );
  delete m_Progress;
  m_Progress = NULL;

//   assert( this->IsMonotone() );
}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
MathComponentTreeFilter<TInputImage, TMathFunctor, TAttributeAccessor>
::SetAttribute( NodeType* node, const NodeArrayType & nodes )
{
  assert(node != NULL);
  assert( nodes.size() == this->GetNumberOfInputs() - 1 );
  std::cout << "=== SetAttribute() ===" << std::endl;
  node->Print();
    
  MathFunctorType compute;
  AttributeAccessorType accessor;
  AttributeType res = accessor( node );

  // build the list of children iterators for the nodes of the other inputs
  typedef std::vector< typename NodeType::ChildrenListType::const_iterator > ChildrenNodesIteratorsType;
  ChildrenNodesIteratorsType childrenNodesIterators;
  for( typename NodeArrayType::const_iterator nit = nodes.begin(); nit!=nodes.end(); nit++ )
  {
    const NodeType * otherNode = *nit;
    assert( otherNode != NULL );
    otherNode->Print();
    assert( otherNode->GetPixel() == node->GetPixel() );
    assert( otherNode->GetFirstIndex() == node->GetFirstIndex() );
    assert( otherNode->GetLastIndex() == node->GetLastIndex() );
    assert( otherNode->CountChildren() == node->CountChildren() );
    res = compute( res, otherNode->GetAttribute() );
    childrenNodesIterators.push_back( otherNode->GetChildren().begin() );
  }
  std::cout << res << std::endl;
  accessor( node, res );
  const typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  for( typename NodeType::ChildrenListType::const_iterator it=childrenList->begin(); it!=childrenList->end(); it++ )
    {
    // build the list of nodes for the other inputs
    NodeArrayType childrenNodes;
    for( typename ChildrenNodesIteratorsType::iterator nit = childrenNodesIterators.begin(); nit!=childrenNodesIterators.end(); nit++ )
      {
      typename NodeType::ChildrenListType::const_iterator & iit = *nit; 
      assert( iit != childrenList->end() );
      childrenNodes.push_back( *iit );
      iit++;
      }
    this->SetAttribute( *it, childrenNodes );
    }
}


template<class TInputImage, class TMathFunctor, class TAttributeAccessor>
void
MathComponentTreeFilter<TInputImage, TMathFunctor, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
  
}// end namespace itk
#endif
