/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalizeComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNormalizeComponentTreeFilter_h
#define __itkNormalizeComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"
#include "itkProgressReporter.h"

namespace itk {
/** \class NormalizeComponentTreeFilter
 * \brief TODO
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT NormalizeComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NormalizeComponentTreeFilter Self;
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
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(NormalizeComponentTreeFilter, 
               InPlaceComponentTreeFilter);

  itkGetConstReferenceMacro(Maximum, double);

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
  NormalizeComponentTreeFilter();
  ~NormalizeComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();
  
  void Compute( NodeType* );
  void Scale( NodeType* );

private:
  NormalizeComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ProgressReporter * m_Progress;
  double             m_Maximum;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNormalizeComponentTreeFilter.txx"
#endif

#endif


