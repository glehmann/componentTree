/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeNode.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeNode_h
#define __itkComponentTreeNode_h

#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <itkLightObject.h>

namespace itk
{
/** \class ComponentTreeNode
 *  \brief ComponentTreeNode class
 * 
 * This class derives from the Object class.
 *
 * The class is templated over the type of the elements.
 *
 * Template parameters for class ComponentTreeNode:
 *
 * - TAttribute = Element type stored in the node
 *
 * \ingroup DataRepresentation 
 */
template <typename TPixel, typename TIndex, typename TAttribute>
class ComponentTreeNode
{

public:

  /** Standard typedefs */
  typedef Object                    Superclass;
  typedef ComponentTreeNode<TPixel, TIndex, TAttribute>      Self;
  typedef std::list<Self *>      ChildrenListType;

  typedef TIndex                     IndexType;
  typedef typename std::list<IndexType>      IndexListType;
  typedef TPixel                     PixelType;
  typedef TAttribute AttributeType;

  /** Run-time type information (and related methods). */ 
  itkTypeMacro( ComponentTreeNode, Object );

  /* Get/Set Attribute */
//   itkGetConstReferenceMacro(Attribute, AttributeType);
  const AttributeType & GetAttribute() const
    {
    m_Attribute;
    }

  AttributeType & GetAttribute()
    {
    m_Attribute;
    }

  void SetAttribute( const AttributeType & a )
    {
    m_Attribute = a;
    }
   
   
  /** Get the parent node */
  inline Self* GetParent() const
    {
    return m_Parent;
    }

  /** Return true if the node has a parent */
  bool HasParent( ) const;

  /** Set the parent of the node */
  inline void SetParent( Self* n )
    {
    assert( n != this );
    // assert( node->GetPixel() < this->GetPixel() );
    m_Parent = n;
    }

  /** Return the number of children */
  int CountChildren( ) const;

  /**  */
  int CountPixels( ) const;

  /** Return the depth of the tree */
  int Depth( ) const;

  /** Remove a node from the node */
  bool RemoveChild( Self *node );

  /** Add a child to the node */
  void AddChild( Self *node );

  /**  */
  bool HasChild( Self *node );

  /** Merge node */
  void Merge( Self *node );

  /** Merge node */
  void MergeChildren();

  /** Get the internal list of children */
  ChildrenListType& GetChildren()
    {
    return m_Children;
    }
  const ChildrenListType& GetChildren() const
    {
    return m_Children;
    }

  /** Get the pixel value */
//   itkGetConstReferenceMacro(Pixel, PixelType);
  inline const PixelType& GetPixel() const
    {
    return m_Pixel;
    }

  inline PixelType& GetPixel()
    {
    return m_Pixel;
    }
//  itkSetMacro(Pixel, PixelType);
  inline void SetPixel( const PixelType & p )
    {
    m_Pixel = p;
    }
   

  /** Get the index list */
  inline const IndexListType& GetIndexes() const
    {
    return m_Indexes;
    }

  inline IndexListType& GetIndexes()
    {
    return m_Indexes;
    }

//  itkGetMacro(IndexList, IndexListType&);

  const void print( int indent=0 ) const;

  Self * Clone();

  ComponentTreeNode();

  ~ComponentTreeNode();

  TAttribute m_Attribute;

protected:
  Self* m_Parent;
  ChildrenListType m_Children;

  PixelType m_Pixel;
  IndexListType  m_Indexes;

private:
  ComponentTreeNode(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkComponentTreeNode.txx"
#endif

#endif
