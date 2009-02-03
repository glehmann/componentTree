/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeFilteringComponentTreeToImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeFilteringComponentTreeToImageFilter_h
#define __itkAttributeFilteringComponentTreeToImageFilter_h

#include "itkInPlaceComponentTreeFilter.h"
#include "itkProgressReporter.h"

namespace itk {
/** \class AttributeFilteringComponentTreeToImageFilter
 * \brief TODO
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage,class TCompare=std::less< typename TInputImage::AttributeType > >
class ITK_EXPORT AttributeFilteringComponentTreeToImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef AttributeFilteringComponentTreeToImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TCompare CompareType;
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename InputImageType::NodeType        NodeType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::AttributeType   AttributeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AttributeFilteringComponentTreeToImageFilter, 
               ImageToImageFilter);

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

  itkSetMacro(Lambda, AttributeType);
  itkGetMacro(Lambda, AttributeType);

protected:
  AttributeFilteringComponentTreeToImageFilter();
  ~AttributeFilteringComponentTreeToImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void LambdaComponents( const NodeType* );

  void WriteNodes( const NodeType*, OutputImagePixelType & p );

private:
  AttributeFilteringComponentTreeToImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  AttributeType m_Lambda;

  ProgressReporter * m_Progress;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAttributeFilteringComponentTreeToImageFilter.txx"
#endif

#endif


