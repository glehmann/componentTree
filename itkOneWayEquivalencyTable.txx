/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOneWayEquivalencyTable.cxx,v $
  Language:  C++
  Date:      $Date: 2004/03/03 19:12:29 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkOneWayEquivalencyTable.h"

namespace itk
{
  
template <typename TValueType, typename THash>
bool 
OneWayEquivalencyTable <TValueType, THash>
::Add(TValueType a, TValueType b)
{
  //
  // Unlike EquivalencyTable, the order of the equivalence is important.
  //
  std::pair<Iterator, bool> result;
  if (a == b) return false;
  result = m_HashMap.insert( ValueType(a, b) );

  return result.second;
}
  
//void OneWayEquivalencyTable::PrintHashTable()
//{
//  ConstIterator it = this->Begin();
//  while (it != this->End() )
//    {
//      std::cout << (*it).first << " = " << (*it).second << std::endl;
//      it++;
//    }
//}

template <typename TValueType, typename THash>
void 
OneWayEquivalencyTable <TValueType, THash>
::Flatten()
{
  Iterator it = this->Begin();
  while ( it != this->End() )
    {
    (*it).second = this->RecursiveLookup((*it).first);
    it++;
    }
}

template <typename TValueType, typename THash>
TValueType 
OneWayEquivalencyTable <TValueType, THash>
::RecursiveLookup(const TValueType a) const
{
  TValueType ans = a;
  TValueType last_ans=a;

  ConstIterator it;
  ConstIterator hashEnd = m_HashMap.end();
  while ( (it = m_HashMap.find(ans)) != hashEnd )
    {
    ans = (*it).second;
    if (ans == a ) return last_ans; // about to cycle again.
    last_ans = ans;
    }

  return ans;
}

template <typename TValueType, typename THash>
void 
OneWayEquivalencyTable <TValueType, THash>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// end namespace itk
