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
#include <ext/slist>
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
  typedef ComponentTreeNode<TPixel, TIndex, TAttribute>      Self;
  typedef __gnu_cxx::slist<Self *>      ChildrenListType;
  typedef typename ChildrenListType::iterator ChildrenListIteratorType;

  typedef TIndex                     IndexType;
  typedef typename __gnu_cxx::slist<IndexType>      IndexListType;
  typedef typename IndexListType::iterator IndexListIteratorType;
  typedef TPixel                     PixelType;
  typedef TAttribute AttributeType;

  /** get/set the attribute */
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

  /** Set the parent of the node */
  inline void SetParent( Self* n )
    {
    assert( n != this );
    // assert( node->GetPixel() < this->GetPixel() );
    m_Parent = n;
    }

  /** Return the number of children */
  int CountChildren( ) const;

  /** Return the depth of the tree */
  int Depth( ) const;

  /** Remove a node from the node */
  bool RemoveChild( Self *node );

  /** Add a child to the node */
  void AddChild( Self *node );

  /** return true if node is in the children list */
  bool HasChild( Self *node ) const;

  void TakeChildrenFrom( Self * node );

  /** Merge node */
  void Merge( Self *node );

  /** Merge node */
  void Flatten();

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
  inline const PixelType& GetPixel() const
    {
    return m_Pixel;
    }

  inline PixelType& GetPixel()
    {
    return m_Pixel;
    }

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

  void AddIndex( const IndexType & idx );

  bool RemoveIndex( const IndexType & idx );

  bool HasIndex( const IndexType & idx ) const;

  void TakeIndexesFrom( Self * node );

  /** Return the number of children */
  int CountIndexes( ) const;

  /** a convenient method to print the tree on std::out.
   * To be used only on small trees !
   */ 
  const void Print( int indent=0 ) const;

  /** return a clone of the current tree */
  Self * Clone();

  inline ComponentTreeNode();

  inline ~ComponentTreeNode();

  inline bool IsLeaf() const
    {
    return m_Children.empty();
    }

  inline bool IsRoot() const
    {
    return m_Parent == NULL;
    }
    
  

  /** the attribute */
  // TODO: make it go back to protected - once found why Get/SetAttribute() are broken
  TAttribute m_Attribute;

protected:
  /** the pixel value associated with the node */
  PixelType m_Pixel;
  /** a pointer to the parent node */
  Self* m_Parent;
  /** the list of children */
  ChildrenListType m_Children;
  ChildrenListIteratorType m_LastChild;
  /** the list of indexs of the node */
  IndexListType  m_Indexes;
  IndexListIteratorType  m_LastIndex;

private:
  ComponentTreeNode(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkComponentTreeNode.txx"
#endif

#endif
