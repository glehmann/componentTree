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

namespace itk
{
/** \class ComponentTree
 *  \brief Templated n-dimensional image class.
 *
 * \ingroup ImageObjects */
template <class TPixel, unsigned int VImageDimension, class TValue>
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
  typedef TValue ValueType;

  /** Node type */
  typedef ComponentTreeNode< PixelType, IndexType, ValueType > NodeType;
  typedef SmartPointer< NodeType > NodePointer;

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

  /** Get the root node */
  itkGetMacro(Root, NodePointer);

protected:
  ComponentTree();
  void PrintSelf(std::ostream& os, Indent indent) const;
  virtual ~ComponentTree() {};
private:
  ComponentTree(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The root node */
  NodePointer m_Root;
};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
# include "itkComponentTree.txx"
#endif

#endif
