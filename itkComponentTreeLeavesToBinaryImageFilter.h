/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTreeLeavesToBinaryImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkComponentTreeLeavesToBinaryImageFilter_h
#define __itkComponentTreeLeavesToBinaryImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkProgressReporter.h"

namespace itk {

/** \class ComponentTreeLeavesToBinaryImageFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT ComponentTreeLeavesToBinaryImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ComponentTreeLeavesToBinaryImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
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
  typedef typename InputImageType::AttributeType   AttributeType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ComponentTreeLeavesToBinaryImageFilter, 
               ImageToImageFilter);

  itkSetMacro(ForegroundValue, OutputImagePixelType);
  itkGetConstMacro(ForegroundValue, OutputImagePixelType);

  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

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

protected:
  ComponentTreeLeavesToBinaryImageFilter();
  ~ComponentTreeLeavesToBinaryImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** ComponentTreeLeavesToBinaryImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** ComponentTreeLeavesToBinaryImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
    void GenerateData();

  void WriteNodes( const NodeType* );
  

private:
  ComponentTreeLeavesToBinaryImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ProgressReporter * m_Progress;

  OutputImagePixelType m_ForegroundValue;
  OutputImagePixelType m_BackgroundValue;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkComponentTreeLeavesToBinaryImageFilter.txx"
#endif

#endif


