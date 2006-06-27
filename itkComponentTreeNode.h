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
#include <itkObject.h>

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
 * - TValue = Element type stored in the node
 *
 * \ingroup DataRepresentation 
 */
template <typename TPixel, typename TIndex, typename TValue>
class ComponentTreeNode : public Object
{

public:

  /** Standard typedefs */
  typedef Object                    Superclass;
  typedef ComponentTreeNode<TPixel, TIndex, TValue>      Self;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef std::vector<Pointer>      ChildrenListType;

  typedef TIndex                     IndexType;
  typedef typename std::list<IndexType>      IndexListType;
  typedef TPixel                     PixelType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
 
  /** Run-time type information (and related methods). */ 
  itkTypeMacro( ComponentTreeNode, Object );

  /** Get the value of the node */
  const TValue& Get() const;

  /** Set the current value of the node */
  TValue Set(const TValue data);

  /** Get the child node */
  ComponentTreeNode<TPixel, TIndex, TValue>* GetChild( int number ) const;

  /** Get the parent node */
  ComponentTreeNode<TPixel, TIndex, TValue>* GetParent( ) const;

  /** Return true if the node has children */
  bool HasChildren( ) const;

  /** Return true if the node has a parent */
  bool HasParent( ) const;

  /** Set the parent of the node */
  void SetParent( ComponentTreeNode<TPixel, TIndex, TValue>* n );

  /** Return the number of children */
  int CountChildren( ) const;

  /** Return the depth of the tree */
  int Depth( ) const;

  /** Remove a node from the node */
  bool Remove( ComponentTreeNode<TPixel, TIndex, TValue> *n );

  /** Get the number of children given a name and depth */
  unsigned int GetNumberOfChildren(unsigned int depth=0, char * name=NULL ) const;

  /** Replace a given child by a new one */
  bool ReplaceChild( ComponentTreeNode<TPixel, TIndex, TValue> *oldChild, ComponentTreeNode<TPixel, TIndex, TValue> *newChild );

  /** Return the child position given a node */
  int ChildPosition( const ComponentTreeNode<TPixel, TIndex, TValue> *node ) const;
  /** Return the child position given a value */
  int ChildPosition( TValue node ) const;

  /** Add a child to the node */
  void AddChild( ComponentTreeNode<TPixel, TIndex, TValue> *node );

  /** Add a child to the node and specify the number in the children list */
  virtual void AddChild( int number, ComponentTreeNode<TPixel, TIndex, TValue> *node );

  /** Get the children list */
  virtual ChildrenListType* GetChildren( unsigned int depth=0, char * name=NULL) const;
  
  /** Get the internal list of children */
  virtual ChildrenListType& GetChildrenList() {return m_Children;}

  /** Set the data of the node */
  //virtual void SetData(TValue data) {m_Data = data;}

  /** Get the pixel value */
  itkGetConstReferenceMacro(Pixel, PixelType);
  itkSetMacro(Pixel, PixelType);

  /** Get the index list */
  IndexListType& GetIndexList()
    { return m_IndexList; }

protected:

  ComponentTreeNode();
  virtual ~ComponentTreeNode();
  TValue m_Data;
  Self* m_Parent;
  ChildrenListType m_Children;

  PixelType m_Pixel;
  IndexListType  m_IndexList;

private:
  ComponentTreeNode(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkComponentTreeNode.txx"
#endif

#endif
