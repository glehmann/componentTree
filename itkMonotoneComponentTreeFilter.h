/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMonotoneComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMonotoneComponentTreeFilter_h
#define __itkMonotoneComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"

namespace itk {
/** \class MonotoneComponentTreeFilter
 * \brief Make the attribute values of a component tree monotones.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT MonotoneComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef MonotoneComponentTreeFilter Self;
  typedef InPlaceComponentTreeFilter<TImage> Superclass;
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
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(MonotoneComponentTreeFilter, 
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

  /** Set/Get whether the nodes which are breaking the monotone rule should be removed or not */
  itkSetMacro(RemoveNodes, bool);
  itkGetMacro(RemoveNodes, bool);
  itkBooleanMacro(RemoveNodes);

  /**
   * Set/Get whether the attributes are increasing (starting from the leaves) (ReverseOrdering=false)
   * or decreasing (ReverseOrdering=true).
   */
  itkSetMacro(ReverseOrdering, bool);
  itkGetConstReferenceMacro(ReverseOrdering, bool);
  itkBooleanMacro(ReverseOrdering);

  /** Set/Get whether the attribute nodes should be strictly monotones or not */
  itkSetMacro(StrictlyMonotone, bool);
  itkGetConstReferenceMacro(StrictlyMonotone, bool);
  itkBooleanMacro(StrictlyMonotone);


protected:
  MonotoneComponentTreeFilter();
  ~MonotoneComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void RootFiltering( NodeType* );

  inline bool Compare( const AttributeType & a1, const AttributeType & a2 )
    {
    if( m_StrictlyMonotone )
      {
      if( m_ReverseOrdering )
        {
        return a1 >= a2;
        }
      else
        {
        return a1 <= a2;
        }
      }
    else
      {
      if( m_ReverseOrdering )
        {
        return a1 > a2;
        }
      else
        {
        return a1 < a2;
        }
      }
    }

private:
  MonotoneComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ReverseOrdering;
  bool m_RemoveNodes;
  bool m_StrictlyMonotone;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMonotoneComponentTreeFilter.txx"
#endif

#endif


