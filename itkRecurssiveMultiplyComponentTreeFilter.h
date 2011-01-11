/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRecurssiveMultiplyComponentTreeFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRecurssiveMultiplyComponentTreeFilter_h
#define __itkRecurssiveMultiplyComponentTreeFilter_h

#include "itkRecurssiveMathComponentTreeFilter.h"

namespace itk {
/** \class RecurssiveMultiplyComponentTreeFilter
 * \brief Multiply the attribute of the equivalent node of several component tree
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage,class TAttributeAccessor=typename Functor::AttributeComponentTreeNodeAccessor< typename TImage::NodeType > >
class ITK_EXPORT RecurssiveMultiplyComponentTreeFilter : 
    public RecurssiveMathComponentTreeFilter<TImage,
      typename std::multiplies< typename TAttributeAccessor::AttributeType > >
{
public:
  /** Standard class typedefs. */
  typedef RecurssiveMultiplyComponentTreeFilter                  Self;
  typedef RecurssiveMathComponentTreeFilter<TImage,
                typename std::multiplies< typename TAttributeAccessor::AttributeType > >
                                                  Superclass;
  typedef SmartPointer<Self>                      Pointer;
  typedef SmartPointer<const Self>                ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(RecurssiveMultiplyComponentTreeFilter, 
               InPlaceComponentTreeFilter);

protected:
  RecurssiveMultiplyComponentTreeFilter() {};

private:
  RecurssiveMultiplyComponentTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class
} // end namespace itk

#endif
