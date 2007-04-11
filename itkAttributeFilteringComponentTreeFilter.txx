/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeFilteringComponentTreeFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeFilteringComponentTreeFilter_txx
#define __itkAttributeFilteringComponentTreeFilter_txx

#include "itkAttributeFilteringComponentTreeFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TInputImage, class TCompare>
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::AttributeFilteringComponentTreeFilter()
{
  m_Threshold = itk::NumericTraits< AttributeType >::Zero;
  m_FilteringType = MAXIMUM;
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels());
  // TODO: how to generate progress ??
  
  if( m_FilteringType == MAXIMUM )
  	{
    this->MaximumFiltering( this->GetOutput()->GetRoot() );
    }
  else if( m_FilteringType == MINIMUM )
  	{
    this->MinimumFiltering( this->GetOutput()->GetRoot() );
    }
  else if( m_FilteringType == DIRECT )
  	{
    this->DirectFiltering( this->GetOutput()->GetRoot() );
    }
  else if( m_FilteringType == SUBTRACT )
  	{
    this->SubtractFiltering( this->GetOutput()->GetRoot(), 0 );
    }
  else
  	{
  	itkExceptionMacro( << "Unknown filtering type" );
  	}

}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::MaximumFiltering( NodeType* node )
{
  assert(node != NULL);
  CompareType compare;
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( compare( (*it)->GetAttribute(), m_Threshold ) )
      {
      this->GetOutput()->NodeFlatten( *it );
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
      this->MaximumFiltering( *it );
      it++;
      }
    }
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::DirectFiltering( NodeType* node )
{
  assert(node != NULL);
  CompareType compare;
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( compare( (*it)->GetAttribute(), m_Threshold ) )
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
      this->DirectFiltering( *it );
      it++;
      }
    }
}


template<class TInputImage, class TCompare>
bool
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::MinimumFiltering( NodeType* node )
{
  assert(node != NULL);
  CompareType compare;
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  
  bool nodeCanBeMerged = true;
  
  while( it!=childrenList->end() )
    {
    if( this->MinimumFiltering( *it ) )
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
      nodeCanBeMerged = false;
      it++;
      }
    }
    
    return nodeCanBeMerged && compare( node->GetAttribute(), m_Threshold );
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::SubtractFiltering( NodeType* node, const PixelType & sub )
{
  assert(node != NULL);
  
  CompareType compare;
  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  
  typename NodeType::ChildrenListType tempNodeList;
  
  while( it!=childrenList->end() )
    {
    if( compare( (*it)->GetAttribute(), m_Threshold ) )
      {
      this->SubtractFiltering( *it, sub + (*it)->GetPixel() - node->GetPixel() );
      
      // don't merge now to avoid putting the nodes in the list and avoid processing
      // their children several times
      // this->GetOutput()->NodeMerge( node, *it );
      tempNodeList.push_front( *it );
      
      // must store the iterator, because once the element
      // erased, it is invalidated
      typename NodeType::ChildrenListType::iterator toRemove = it;
      it++;
      childrenList->erase( toRemove );
      }
    else
      {
      this->SubtractFiltering( *it, sub );
      it++;
      }
    }
    
  for( it=tempNodeList.begin(); it!=tempNodeList.end(); it++ )
    {
    this->GetOutput()->NodeMerge( node, *it );
    delete *it;
    }
  
  node->SetPixel( node->GetPixel() - sub );
  
}


template<class TInputImage, class TCompare>
void
AttributeFilteringComponentTreeFilter<TInputImage, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Threshold: " 
     << static_cast<typename NumericTraits< AttributeType >::PrintType>( m_Threshold ) << std::endl;
}
  
}// end namespace itk
#endif
