/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkKeepNLobesComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkKeepNLobesComponentTreeFilter_h
#define __itkKeepNLobesComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"
#include "itkHierarchicalQueue.h"

namespace itk {

namespace Function {

  template< class TComponentTreeNode, class TCompare >
  class CompareComponentTreeNoneAttribute
  {
  public:
  inline bool operator()( TComponentTreeNode * n1, TComponentTreeNode * n2 )
    {
    TCompare compare;
    return compare( n1->GetAttribute(), n2->GetAttribute() );
    }
  };

}


/** \class KeepNLobesComponentTreeFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT KeepNLobesComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef KeepNLobesComponentTreeFilter Self;
  typedef InPlaceComponentTreeFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::NodeType        NodeType;
  typedef typename ImageType::IndexType       IndexType;

  typedef TAttibuteAccessor AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeType   AttributeType;
  
  typedef HierarchicalQueue< AttributeType, NodeType*, typename std::less< AttributeType > > HierarchicalQueueType;
  typedef HierarchicalQueue< AttributeType, NodeType*, typename std::greater< AttributeType > > ReverseHierarchicalQueueType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(KeepNLobesComponentTreeFilter, 
               InPlaceComponentTreeFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
  /** End concept checking */
#endif

  itkSetMacro(NumberOfLobes, unsigned long);
  itkGetConstReferenceMacro(NumberOfLobes, unsigned long);

  itkSetMacro(ReverseOrdering, bool);
  itkGetConstReferenceMacro(ReverseOrdering, bool);
  itkBooleanMacro(ReverseOrdering);


protected:
  KeepNLobesComponentTreeFilter();
  ~KeepNLobesComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void PutLeavesInQueue( HierarchicalQueueType & queue, NodeType* node );
  void PutLeavesInQueue( ReverseHierarchicalQueueType & queue, NodeType* node );


private:
  KeepNLobesComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned long m_NumberOfLobes;
  
  bool m_ReverseOrdering;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkKeepNLobesComponentTreeFilter.txx"
#endif

#endif


