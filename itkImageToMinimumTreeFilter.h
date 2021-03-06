/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToMinimumTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToMinimumTreeFilter_h
#define __itkImageToMinimumTreeFilter_h

#include "itkImageToComponentTreeFilter.h"

namespace itk {
/** \class ImageToMinimumTreeFilter
 * \brief Convert an image to a min tree
 *
 * A min tree is a component tree with the maximum pixel value in the root node and the minimum pixel values
 * in the leaves.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters ImageToMaximumTreeFilter
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT ImageToMinimumTreeFilter : 
    public ImageToComponentTreeFilter<TInputImage, TOutputImage, std::less<typename TInputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ImageToMinimumTreeFilter Self;
  typedef ImageToComponentTreeFilter<TInputImage, TOutputImage, std::less<typename TInputImage::PixelType> > Superclass;
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
  typedef typename OutputImageType::NodeType       NodeType;
  typedef typename OutputImageType::AttributeType  AttributeType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ImageToMinimumTreeFilter, 
               ImageToComponentTreeFilter);

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
  ImageToMinimumTreeFilter() {};
  ~ImageToMinimumTreeFilter() {};  

private:
  ImageToMinimumTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
} ; // end of class

} // end namespace itk
  

#endif


