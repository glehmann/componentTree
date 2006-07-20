/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComponentTree.txx,v $
  Language:  C++
  Date:      $Date: 2006/05/10 20:27:16 $
  Version:   $Revision: 1.97 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkComponentTree_txx
#define _itkComponentTree_txx

#include "itkComponentTree.h"
#include "itkProcessObject.h"

namespace itk
{

/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
ComponentTree<TPixel, VImageDimension, TValue>
::ComponentTree()
{
  this->Initialize();
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
//   os << indent << "PixelContainer: " << std::endl;
//   m_Buffer->Print(os, indent.GetNextIndent());

// m_Origin and m_Spacing are printed in the Superclass
}


/**
 *
 */
template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::Initialize()
{
  m_Root = NULL;
}


template<class TPixel, unsigned int VImageDimension, class TValue>
void 
ComponentTree<TPixel, VImageDimension, TValue>
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft( data );

  if ( data )
    {
    // Attempt to cast data to an Image
    const Self * imgData;

    try
      {
      imgData = dynamic_cast<const Self *>( data );
      }
    catch( ... )
      {
      return;
      }


    if ( imgData )
      {
      // Now copy anything remaining that is needed
      this->SetRoot( const_cast< NodeType * >
                                  (imgData->GetRoot().GetPointer() ) );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::Image::Graft() cannot cast "
                         << typeid(data).name() << " to "
                         << typeid(const Self *).name() );
      }
    }
}} // end namespace itk

#endif
