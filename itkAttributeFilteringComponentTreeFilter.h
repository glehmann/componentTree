/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeFilteringComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeFilteringComponentTreeFilter_h
#define __itkAttributeFilteringComponentTreeFilter_h

#include "itkInPlaceComponentTreeFilter.h"

namespace itk {
/** \class AttributeFilteringComponentTreeFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TAttibuteAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT AttributeFilteringComponentTreeFilter : 
    public InPlaceComponentTreeFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef AttributeFilteringComponentTreeFilter Self;
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
  itkTypeMacro(AttributeFilteringComponentTreeFilter, 
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

  itkSetMacro(Lambda, AttributeType);
  itkGetMacro(Lambda, AttributeType);
  

  static const int MAXIMUM=0;
  static const int MINIMUM=1;
  static const int DIRECT=2;
  static const int SUBTRACT=3;
  

  itkSetMacro(ReverseOrdering, bool);
  itkGetConstReferenceMacro(ReverseOrdering, bool);
  itkBooleanMacro(ReverseOrdering);

  static int GetFilteringTypeFromName( const std::string & s )
    {
    if( s == "Maximum" )
      {
      return MAXIMUM;
      }
    else if( s == "Minimum" )
      {
      return MINIMUM;
      }
    else if( s == "Direct" )
      {
      return DIRECT;
      }
    else if( s == "Subtract" )
      {
      return SUBTRACT;
      }
    // can't recognize the namespace
    itkGenericExceptionMacro( << "Unknown filtering type." );
    }

  static std::string GetNameFromFilteringType( const int & a )
    {
    switch( a )
      {
      case MAXIMUM:
        return "Maximum";
        break;
      case MINIMUM:
        return "Minimum";
        break;
      case DIRECT:
        return "Direct";
        break;
      case SUBTRACT:
        return "Subtract";
        break;
      }
    // can't recognize the namespace
    itkGenericExceptionMacro( << "Unknown filtering type." );
    }

//   itkSetMacro(FilteringType, int);
  itkGetMacro(FilteringType, int);
  
  void SetFilteringType( std::string type )
    {
    this->SetFilteringType( GetFilteringTypeFromName( type ) );
    }

  void SetFilteringType( const int & type )
    {
    if( m_FilteringType != type )
      {
      GetNameFromFilteringType( type ); // to validate the filtering type
      m_FilteringType = type;
      this->Modified();
      }
    }


protected:
  AttributeFilteringComponentTreeFilter();
  ~AttributeFilteringComponentTreeFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void MaximumFiltering( NodeType* );
  
  bool MinimumFiltering( NodeType* );
  
  void DirectFiltering( NodeType* );
  
  void SubtractFiltering( NodeType*, const PixelType & );
  
  inline bool Compare( const AttributeType & a1, const AttributeType & a2 )
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

private:
  AttributeFilteringComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  AttributeType m_Lambda;
  
  int m_FilteringType;
  
  bool m_ReverseOrdering;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAttributeFilteringComponentTreeFilter.txx"
#endif

#endif


