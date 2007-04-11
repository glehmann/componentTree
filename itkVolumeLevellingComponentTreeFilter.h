/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVolumeLevellingComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVolumeLevellingComponentTreeFilter_h
#define __itkVolumeLevellingComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"
#include "itkProgressReporter.h"

namespace itk {
/** \class VolumeLevellingComponentTreeFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage >
class ITK_EXPORT VolumeLevellingComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef VolumeLevellingComponentTreeFilter Self;
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
  typedef typename ImageType::AttributeType   AttributeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(VolumeLevellingComponentTreeFilter, 
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

protected:
  VolumeLevellingComponentTreeFilter();
  ~VolumeLevellingComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  class SumSize
    {
    public:
      SumSize( double sum, unsigned long size )
        {
        this->sum = sum;
        this->size = size;
        }
      double sum;
      unsigned long size;
    };

  SumSize SetVolumeLevelling( NodeType* );

private:
  VolumeLevellingComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ProgressReporter * m_Progress;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVolumeLevellingComponentTreeFilter.txx"
#endif

#endif


