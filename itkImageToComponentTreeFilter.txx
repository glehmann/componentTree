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
#include "itkOneWayEquivalencyTable.h"

namespace itk {

template <class TInputImage, class TOutputImage>
ImageToComponentTreeFilter<TInputImage, TOutputImage>
::ImageToComponentTreeFilter()
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage>
void 
ImageToComponentTreeFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  if ( !input )
    { return; }

  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
ImageToComponentTreeFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // sort the pixel by gray level

  // iterator for the marker image
  typedef ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorType;
  InputIteratorType inputIt(this->GetInput(), this->GetInput()->GetRequestedRegion());

  // create map to store     pixel value -> [pos1, pos2 .. posn]
  typedef std::list<IndexType> IndexListType;
  typedef std::map <InputImagePixelType, IndexListType> PixelMapType;
  PixelMapType pixelMap;

  for ( inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt )
    {
    // store index of current pixel value
    pixelMap[inputIt.Get()].push_back( inputIt.GetIndex() );
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

  // a list to temporary store the node's SmartPointers
  typedef typename std::list< NodePointer > NodePointerList;
  NodePointerList tempNodeList;

  NodeType* n = NULL;
  typedef OneWayEquivalencyTable< NodeType*, typename Function::PointerHash< NodeType* > > EquivType;
  typename EquivType::Pointer equiv = EquivType::New();

  // iterate over pixel values, from high to low
  for ( typename PixelMapType::reverse_iterator pixelMapIt=pixelMap.rbegin(); pixelMapIt!=pixelMap.rend(); ++pixelMapIt )
    {
    InputImagePixelType pixelValue = pixelMapIt->first;
    IndexListType* indexes = &(pixelMapIt->second);

    std::cout << "pixelValue: " << pixelValue+0.0 << std::endl;

    // iterate over pixel indexes, and build the tree !
    for ( typename IndexListType::iterator idxIt = indexes->begin(); idxIt != indexes->end(); ++idxIt )
      {
		  // std::cout << "*idxIt: " << *idxIt << std::endl;
	
      // shift output and mask iterators to new location
      nIt += *idxIt - nIt.GetIndex();
      iIt += *idxIt - iIt.GetIndex();
      
      InputImagePixelType p = iIt.GetCenterPixel();
      n = NULL;
      
      typename NodeImageIteratorType::Iterator nnIt;
      typename InputImageIteratorType::ConstIterator niIt;
      
      // search the neighbors which can get this pixel
      for ( nnIt = nIt.Begin(),  niIt = iIt.Begin(); nnIt != nIt.End() /*&& niIt != iIt.End()*/; nnIt++, niIt++)
        {
        NodeType* nn = equiv->RecursiveLookup( nnIt.Get() );
        InputImagePixelType np = niIt.Get();

        if( nn != NULL &&  n != nn && np == p )
          {
          if( n == NULL )
            {
            nn->GetIndexList().push_back( iIt.GetIndex() );
            n = nn;
            }
          else
            {
			      // std::cout << nn << " -> " << n << "   " << *idxIt << "   " << equiv->RecursiveLookup( nn ) << std::endl;
            // we have found an equivalent node
            // they must be merged
            n->Merge( nn );
            // store the equivalency to avoid resetting all the nodes in temp image with the new
            // one.
            equiv->Add(nn, n);
            }
          }
        }

      // if no node has been found, create a new one
      if( n == NULL )
        {

        // std::cout << iIt.GetIndex() << std::endl;

        NodePointer newNode = NodeType::New();
        tempNodeList.push_back( newNode );
        n = newNode.GetPointer();
        n->SetPixel( p );
        n->GetIndexList().push_back( iIt.GetIndex() );
        }

      nIt.SetCenterPixel( n );

      // search the neighbors with an higher value, to set the current node as parent of
      // the deepest parent of the neighbor
      for( nnIt = nIt.Begin(); nnIt != nIt.End(); nnIt++ )
        {
        NodeType* nn = equiv->RecursiveLookup( nnIt.Get() );

        if( nn != NULL &&  nn->GetPixel() > pixelValue )
          {
          // find nn deepest current parent
          while( nn->GetParent() != NULL )
            {
            // std::cout << "nn: " << nn << std::endl;
            assert(equiv->RecursiveLookup( nn->GetParent() )->GetPixel() < nn->GetPixel() );
            nn = equiv->RecursiveLookup( nn->GetParent() ); 
            // std::cout << "  " << nn->GetPixel() << std::endl;
            }

          // and if n and nn are different, set n as parent of nn
          if( n->GetPixel() != nn->GetPixel() )
            {
            assert( n->GetParent() == NULL );
            assert( n->GetParent() != nn );
            assert( nn->GetPixel() >= n->GetPixel() );
            n->AddChild( nn );
            }
          else if( nn != n )
            {
			      // std::cout << nn << " => " << n << "   " << *idxIt << "   " << equiv->RecursiveLookup( nn ) << std::endl;
	          n->Merge( nn );
	          equiv->Add(nn, n);
	          // equiv->Add(n, NULL);
            }
          }
        }

      }
    }

  assert( n->CountPixels() == this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() );

  std::cout << "tempNodeList: " << tempNodeList.size() << std::endl;
  std::cout << "chilren: " << n->CountChildren() << std::endl;
  std::cout << "pixels: " << n->CountPixels() << std::endl;
  std::cout << "p: " << n->GetPixel()+0.0 << std::endl;
  n->print();

  this->GetOutput()->SetRoot( n );

/* for( int i=0; i<n->CountChildren() ; i++ )
  {
  std::cout <<   n->GetChildrenList()[i]->GetPixel() << std::endl;
  }*/
}


template<class TInputImage, class TOutputImage>
void
ImageToComponentTreeFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif