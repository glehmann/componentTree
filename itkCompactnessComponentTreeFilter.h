/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCompactnessComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCompactnessComponentTreeFilter_h
#define __itkCompactnessComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"
#include "itkProgressReporter.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace itk {
/** \class CompactnessComponentTreeFilter
 * \brief Compute the compactness of each node and store it as attribute
 *
 * The compactness is computed as the size of the smallest principal moment
 * divided by the size of the greated principal moment.
 * Its value is 1.0 for a perfect ball.
 *
 * The pixels values are not used to compute the moment of the nodes.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType >  >
class ITK_EXPORT CompactnessComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef CompactnessComponentTreeFilter Self;
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
  typedef typename ImageType::PointType       PointType;

  typedef TAttibuteAccessor AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeType   AttributeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  typedef Matrix< double, ImageDimension, ImageDimension >   MatrixType;
  typedef Vector< double, ImageDimension > VectorType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(CompactnessComponentTreeFilter, 
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
  CompactnessComponentTreeFilter();
  ~CompactnessComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  class ChildData
    {
    public:
      ChildData( const double & _sum, const VectorType _cog, const MatrixType & _cm )
        {
        this->sum = _sum;
        this->cog = _cog;
        this->cm = _cm;
        }
      double sum;
      VectorType cog;
      MatrixType cm;
    };

  ChildData SetCompactness( NodeType* );

private:
  CompactnessComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ProgressReporter * m_Progress;
  double m_PhysicalPixelSize;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCompactnessComponentTreeFilter.txx"
#endif

#endif


