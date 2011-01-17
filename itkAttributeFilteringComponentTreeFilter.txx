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

template <class TInputImage, class TAccessor>
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::AttributeFilteringComponentTreeFilter()
{
  m_Lambda = itk::NumericTraits< AttributeType >::Zero;
  m_FilteringType = DIRECT;
  m_ReverseOrdering = false;
}


template<class TInputImage, class TAccessor>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
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


template<class TInputImage, class TAccessor>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::MaximumFiltering( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( this->Compare( accessor(*it), m_Lambda ) )
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


template<class TInputImage, class TAccessor>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::DirectFiltering( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  while( it!=childrenList->end() )
    {
    if( this->Compare( accessor(*it), m_Lambda ) )
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


template<class TInputImage, class TAccessor>
bool
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::MinimumFiltering( NodeType* node )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

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
    
    return nodeCanBeMerged && this->Compare( accessor(node), m_Lambda );
}


template<class TInputImage, class TAccessor>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::SubtractFiltering( NodeType* node, const PixelType & sub )
{
  assert(node != NULL);

  AttributeAccessorType accessor;

  typename NodeType::ChildrenListType * childrenList = & node->GetChildren();
  typename NodeType::ChildrenListType::iterator it=childrenList->begin();
  
  typename NodeType::ChildrenListType tempNodeList;
  
  while( it!=childrenList->end() )
    {
    if( this->Compare( accessor(*it), m_Lambda ) )
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


template<class TInputImage, class TAccessor>
void
AttributeFilteringComponentTreeFilter<TInputImage, TAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Lambda: " 
     << static_cast<typename NumericTraits< AttributeType >::PrintType>( m_Lambda ) << std::endl;
  os << indent << "FilteringType: "  << GetNameFromFilteringType(m_FilteringType) << " (" << m_FilteringType << ")" << std::endl;
  os << indent << "ReverseOrdering: " 
     << static_cast<typename NumericTraits< bool >::PrintType>( m_ReverseOrdering ) << std::endl;
}
  
}// end namespace itk
#endif
