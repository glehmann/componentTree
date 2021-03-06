/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTree.h,v $
  Language:  C++
  Date:      $Date: 2006/04/20 14:54:09 $
  Version:   $Revision: 1.136 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTree_h
#define __itkComponentTree_h

#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "itkFixedArray.h"
#include "itkWeakPointer.h"
#include "itkComponentTreeNode.h"
#include <list>

namespace itk
{
/** \class ComponentTree
 *  \brief Connected component tree image representation
 *
 * This is the main container class used to store an image as a tree of connected components.
 *
 * The tree is made of a set of nodes. Each node stores the set of pixels of a connected component
 * at a specific pixel value, and a set of children nodes which have greater pixel values.
 * The pixels of a node and the pixels of all its children form a connected component.
 *
 * The tree has a single root node accessible with GetRoot().
 *
 * For storage efficiency, the list of pixels is not directly stored in the nodes. It is kept in a single
 * array structure in the component tree: LinkedListArray. This array has the same number of elements than
 * the corresponding itk::Image. Each element is the offset, as produced by itk::ImageBase methods, to the
 * next element in the pixel list of the node, or -1 if this is the end of the list.
 * The first and last index of the list are stored in each node.
 * This allow constant time merging of two nodes, which is not possible if the container is a std::vector
 * and use less memory than std::list where each index requires the storage of two additional pointers.
 *
 * As a consequence of the indices management, some methods which may seems best suited to be implemented
 * in the node class are implemented in this class, for example NodeMerge().
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ComponentTreeNode ImageToMaximumTreeFilter ImageToMinimumTreeFilter
 * \ingroup ImageObjects
 */
template <class TPixel, unsigned int VImageDimension, class TAttribute>
class ITK_EXPORT ComponentTree : public ImageBase<VImageDimension>
{
public:
  /** Standard class typedefs */
  typedef ComponentTree               Self;
  typedef ImageBase<VImageDimension>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComponentTree, ImageBase);

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef TPixel PixelType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType  IndexType;

  /** Offset typedef support. An offset is used to access pixel values. */
  typedef typename Superclass::OffsetType OffsetType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType  SizeType;

  /** Direction typedef support. A matrix of direction cosines. */
  typedef typename Superclass::DirectionType  DirectionType;

  /** Region typedef support. A region is used to specify a subset of an image. */
  typedef typename Superclass::RegionType  RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename Superclass::PointType PointType;

  /** Offset typedef (relative position between indices) */
  typedef typename Superclass::OffsetValueType OffsetValueType;

  /** the type of data associated with each node */
  typedef TAttribute AttributeType;

  /** linked list array type */
  typedef std::vector< OffsetValueType > LinkedListArrayType;

  /** Node type */
  typedef ComponentTreeNode< PixelType, OffsetValueType, AttributeType > NodeType;

  /** Convenience methods to set the LargestPossibleRegion,
   *  BufferedRegion and RequestedRegion. Allocate must still be called.
   */
  void SetRegions(RegionType region)
    {
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  void SetRegions(SizeType size)
    {
    RegionType region; region.SetSize(size);
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  /** Get the root node. Throw an exception is no root is set. */
  NodeType * GetRoot();
  const NodeType * GetRoot() const;

  /** Set the root node */
  itkSetObjectMacro(Root, NodeType);

  /** Restore the data object to its initial state. This means releasing
   * memory. */
  virtual void Initialize();

  /**  */
  void Allocate();

  virtual void Graft(const DataObject *data);

//  itkGetMacro(LinkedListArray, LinkedListArrayType);
//  itkGetConstMacro(LinkedListArray, LinkedListArrayType);
  const LinkedListArrayType & GetLinkedListArray() const
    {
    return m_LinkedListArray;
    }

  LinkedListArrayType & GetLinkedListArray()
    {
    return m_LinkedListArray;
    }


  //methods to manipulate the nodes
  // those methods are here because they require the access to the linked list array
  
  /** Merge nodes */
  void NodeMerge( NodeType *node, NodeType *obsoletedNode );

  /** Merge node */
  void NodeFlatten( NodeType *node );

  void NodeAddIndex( NodeType *node, const OffsetValueType & idx );

  void NodeAddIndex( NodeType *node, const IndexType & idx );

  bool NodeRemoveIndex( NodeType *node, const OffsetValueType & idx );

  bool NodeRemoveIndex( NodeType *node, const IndexType & idx );

  bool NodeHasIndex( const NodeType *node, const OffsetValueType & idx ) const;

  bool NodeHasIndex( const NodeType *node, const IndexType & idx ) const;

  void NodeTakeIndexesFrom( NodeType * node, NodeType *obsoletedNode );

  /** Return the number of index  in the node and its children */
  unsigned long NodeCountIndexes( const NodeType *node ) const;

  const PixelType & GetPixel( const IndexType & idx ) const;

  const PixelType & GetPixel( const OffsetValueType & idx ) const;

  const NodeType * GetNode( const IndexType & idx ) const;

  const NodeType * GetNode( const OffsetValueType & idx ) const;

  NodeType * GetNode( const IndexType & idx );

  NodeType * GetNode( const OffsetValueType & idx );

  const NodeType * GetNode( const NodeType *node, const IndexType & idx ) const;

  const NodeType * GetNode( const NodeType *node, const OffsetValueType & idx ) const;

  NodeType * GetNode( NodeType *node, const IndexType & idx );

  NodeType * GetNode( NodeType *node, const OffsetValueType & idx );

protected:
  ComponentTree();
  void PrintSelf(std::ostream& os, Indent indent) const;
  virtual ~ComponentTree()
    {
    if( m_Root )
      {
      delete m_Root;
      }
    }

private:
  ComponentTree(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The root node */
  NodeType * m_Root;

  LinkedListArrayType m_LinkedListArray;
};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
# include "itkComponentTree.txx"
#endif

#endif
