/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRoundnessComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRoundnessComponentTreeFilter_h
#define __itkRoundnessComponentTreeFilter_h

#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkShapeLabelObjectAccessors.h"
#include "itkShapeComponentTreeFilter.h"

namespace itk {
/** \class RoundnessComponentTreeFilter
 * \brief Compute the roundness of the connected component and assign it to the attribute value
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT RoundnessComponentTreeFilter : 
    public ShapeComponentTreeFilter< TImage, Functor::RoundnessLabelObjectAccessor< ShapeLabelObject< unsigned long, TImage::ImageDimension > >, TAttibuteAccessor >
{
public:
  /** Standard class typedefs. */
  typedef RoundnessComponentTreeFilter Self;
  typedef ShapeComponentTreeFilter< TImage, Functor::RoundnessLabelObjectAccessor< ShapeLabelObject< unsigned long, TImage::ImageDimension > >, TAttibuteAccessor >
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
  itkTypeMacro(RoundnessComponentTreeFilter, 
               ShapeComponentTreeFilter);

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
  RoundnessComponentTreeFilter() {};
  ~RoundnessComponentTreeFilter() {};

private:
  RoundnessComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk

#endif


