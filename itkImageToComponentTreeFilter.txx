/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToComponentTreeFilter_txx
#define __itkImageToComponentTreeFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageToComponentTreeFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include <algorithm>
#include <list>
#include <map>
#include "itkConnectedComponentAlgorithm.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkProgressReporter.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TCompare>
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::ImageToComponentTreeFilter()
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage, class TCompare>
void 
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  if ( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template <class TInputImage, class TOutputImage, class TCompare>
void 
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage, class TCompare>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  OutputImageType * output = this->GetOutput();

  // instantiate the comparator
  TCompare compare;
  
  // setup the progress reporter
  ProgressReporter progress(this, 0, this->GetInput()->GetRequestedRegion().GetNumberOfPixels()*3);

  // sort the pixel by gray level

  // iterator for the marker image
  typedef ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorType;
  InputIteratorType inputIt(this->GetInput(), this->GetInput()->GetRequestedRegion());

  // create a map to count how many pixels we have for each value, so we can optimize
  // the vector size of the really usefull map
  typedef std::map <InputImagePixelType, unsigned long, TCompare> PixelCountMapType;
  PixelCountMapType pixelCountMap;

  for( inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt )
    {
    // store index of current pixel value
    pixelCountMap[inputIt.Get()]++;
    progress.CompletedPixel();
    }

  // create map to store     pixel value -> [pos1, pos2 .. posn]
  typedef std::vector< typename InputImageType::OffsetValueType > IndexListType;
  typedef std::map <InputImagePixelType, IndexListType, TCompare> PixelMapType;
  PixelMapType pixelMap;

  // change the vector sizes to exactly fit the number of indexes needed
  for ( typename PixelCountMapType::iterator pixelCountMapIt=pixelCountMap.begin(); pixelCountMapIt!=pixelCountMap.end(); ++pixelCountMapIt )
    {
    pixelMap[pixelCountMapIt->first].reserve( pixelCountMapIt->second );
    }

  // and now fill the map
  for( inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt )
    {
    // store index of current pixel value
    pixelMap[inputIt.Get()].push_back( output->ComputeOffset( inputIt.GetIndex() ) );
    progress.CompletedPixel();
    }

  // we need, to construct the full build tree, to know to which node a pixel
  // belong
  typedef Image< NodeType*, ImageDimension > NodeImageType;
  typename NodeImageType::Pointer nodeImage = NodeImageType::New();
  nodeImage->SetRegions( this->GetInput()->GetRequestedRegion() );
  nodeImage->Allocate();
  nodeImage->FillBuffer( NULL );
  
  // setup an iterator to visit neighbor nodes
  typedef ShapedNeighborhoodIterator< NodeImageType > NodeImageIteratorType;
  typename NodeImageIteratorType::RadiusType radius;
  radius.Fill( 1 );
  NodeImageIteratorType nIt(radius, nodeImage, nodeImage->GetRequestedRegion());
  setConnectivity( &nIt, m_FullyConnected );
  ConstantBoundaryCondition< NodeImageType > cbc;
/*  typename NodeType::Pointer outNode = NodeType::New();
  NodeType* outside = outNode.GetPointer();
  cbc.SetConstant( outside );*/
  cbc.SetConstant( NULL );
  nIt.OverrideBoundaryCondition( &cbc );
  nIt.GoToBegin();

  // setup an iterator to visit neighbor nodes
  typedef ConstShapedNeighborhoodIterator< InputImageType > InputImageIteratorType;
  InputImageIteratorType iIt(radius, this->GetInput(), this->GetInput()->GetRequestedRegion());
  setConnectivity( &iIt, m_FullyConnected );
  iIt.GoToBegin();

  // a list to store the node merged with other nodes, and that we'll have to
  // delete later. They are not destructed immediately because they keep a pointer
  // on the reference node
  typedef typename std::vector< NodeType * > NodePointerList;
  NodePointerList tempNodeList;

  NodeType* n = NULL;

  // iterate over pixel values, from high to low
  while( !pixelMap.empty() )
    {
    typename PixelMapType::iterator pixelMapIt=pixelMap.begin();
    InputImagePixelType pixelValue = pixelMapIt->first;
    IndexListType* indexes = &(pixelMapIt->second);

    // iterate over pixel indexes, and build the tree !
    for ( typename IndexListType::iterator idxIt = indexes->begin(); idxIt != indexes->end(); ++idxIt )
      {
      // std::cout << "idx: " << idx << std::endl;
      IndexType idx = output->ComputeIndex( *idxIt );

      // shift output and mask iterators to new location
      nIt += idx - nIt.GetIndex();
      iIt += idx - iIt.GetIndex();
      
      InputImagePixelType p = iIt.GetCenterPixel();
      n = NULL;
      
      typename NodeImageIteratorType::Iterator nnIt;
      typename InputImageIteratorType::ConstIterator niIt;
      
      // search the neighbors which can get this pixel
      for ( nnIt = nIt.Begin(),  niIt = iIt.Begin(); nnIt != nIt.End() /*&& niIt != iIt.End()*/; nnIt++, niIt++)
        {
        NodeType* nn = this->GetReference( nnIt.Get() );
        InputImagePixelType np = niIt.Get();

        if( nn != NULL &&  n != nn && np == p )
          {
          if( n == NULL )
            {
            nn->GetIndexes().push_back( output->ComputeOffset( iIt.GetIndex() ) );
            n = nn;
            }
          else
            {
            // we have found an equivalent node
            // they must be merged
            this->LightMerge( n, nn );
            tempNodeList.push_back( nn );
            }
          }
        }

      // if no node has been found, create a new one
      if( n == NULL )
        {
        n = new NodeType();
        n->SetPixel( p );
        n->GetIndexes().push_back( output->ComputeOffset( iIt.GetIndex() ) );
        }

      nIt.SetCenterPixel( n );

      // search the neighbors with an higher value, to set the current node as parent of
      // the deepest parent of the neighbor
      for( nnIt = nIt.Begin(); nnIt != nIt.End(); nnIt++ )
        {
        NodeType* nn = this->GetReference( nnIt.Get() );

        if( nn != NULL &&  compare ( nn->GetPixel(), pixelValue ) )
          {
          // find nn deepest current parent
          nn = this->GetAncestor( nn );

          // and if n and nn are different, set n as parent of nn
          if( n->GetPixel() != nn->GetPixel() )
            {
            assert( compare( nn->GetPixel(), n->GetPixel() ) || nn->GetPixel() == n->GetPixel() );
            assert( !n->HasChild( nn ) );
            n->AddChild( nn );
            }
          else if( nn != n )
            {
            this->LightMerge( n, nn );
            tempNodeList.push_back( nn );
            }
          }
        }

      progress.CompletedPixel();

      }

    // release the memory used by the vector once processed
    pixelMap.erase( pixelMapIt );
    }

  // clean the tempList
  for( typename NodePointerList::iterator it=tempNodeList.begin(); it!=tempNodeList.end(); it++ )
    {
    delete (*it);
    }
  tempNodeList.clear();

  // Now set the real parent of the child, and no more the ancestor, or the reference.
  this->SetChildrenParent( n );

//   n->print();
//   std::cout << "tempNodeList: " << tempNodeList.size() << std::endl;
//   std::cout << "chilren: " << n->CountChildren() << std::endl;
//   std::cout << "pixels: " << n->CountPixels() << std::endl;
//   std::cout << "p: " << n->GetPixel()+0.0 << std::endl;

  assert( n->CountPixels() == this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() );

  // to be sure that root parent is NULL
  n->SetParent( NULL );

  // keep a pointer on the root node
  output->SetRoot( n );

}


template<class TInputImage, class TOutputImage, class TCompare>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::LightMerge( NodeType* node1, NodeType* node2)
{
  assert( node1 != node2 );
  // assert( this->GetPixel() <= node->GetPixel() );
  // merge the index list
  typename NodeType::IndexListType & node1Indexes = node1->GetIndexes();
  node1Indexes.splice( node1Indexes.begin(), node2->GetIndexes() );
  // merge the children
  typename NodeType::ChildrenListType & node1Children = node1->GetChildren();
  node1Children.splice( node1Children.begin(), node2->GetChildren() );
  // set the node1 as parent of node2, to indicate that node1 is the reference
  // for node2
  node2->SetParent( node1 );

  assert( node2->GetIndexes().empty() );
  assert( node2->GetChildren().empty() );
  assert( !node1->GetIndexes().empty() );
}


template<class TInputImage, class TOutputImage, class TCompare>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::SetChildrenParent( NodeType* node )
{
  assert( node != NULL );
  for( typename NodeType::ChildrenListType::iterator it=node->GetChildren().begin(); it!=node->GetChildren().end(); it++ )
    {
    (*it)->SetParent( node );
    this->SetChildrenParent( *it );
    }
}


template<class TInputImage, class TOutputImage, class TCompare>
typename ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>::NodeType *
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::GetAncestor( NodeType* node )
{
  assert( node != NULL );
  NodeType * equivNode = this->GetReference( node );
  if( equivNode->GetParent() == NULL )
    {
    // we got the root node
    return equivNode;
    }

  NodeType * ancestor = this->GetAncestor( equivNode->GetParent() );
  // path compression: we keep a pointer to the ancestor so when we
  // will have to search it again, the path will be a lot shorter
  equivNode->SetParent( ancestor );
  return ancestor;
}


template<class TInputImage, class TOutputImage, class TCompare>
typename ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>::NodeType *
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::GetReference( NodeType* node )
{
  if( node == NULL )
    {
    return NULL;
    }

  if( !node->GetIndexes().empty() )
    {
    // we got the reference node
    return node;
    }

  // index list is empty, so we need to find the reference node
  NodeType * ref = this->GetReference( node->GetParent() );
  // store the reference node, to avoid searching it next time
  // TODO: it is not needed to perform a path compression here.
  // It is possible to have only one reference node, by choosing
  // which node will be kept as reference node during the merge
  node->SetParent( ref );
  return ref;
}


template<class TInputImage, class TOutputImage, class TCompare>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
