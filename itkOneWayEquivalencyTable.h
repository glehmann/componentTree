/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOneWayEquivalencyTable.h,v $
  Language:  C++
  Date:      $Date: 2004/03/03 19:12:29 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOneWayEquivalencyTable_h
#define __itkOneWayEquivalencyTable_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactory.h"
#include "itkDataObject.h"
#include "itkProcessObject.h"
#include "itk_hash_map.h"

namespace itk
{
/** \class OneWayEquivalencyTable
 * \brief Hash table to manage integral label equivalencies that are order dependent.
 *
 * OneWayEquivalencyTable is a variation on itk::EquivalencyTable that
 * preserves the order of equivalencies.  For example, the entries { 5
 * = 4 } and {4 = 5} are not equivalent in this table, yet are the
 * treated as the same entry in EquivalencyTable.  Because of the
 * one-way logic of the table, reflexive equivalencies will result in
 * cycling from recursive lookups or flattening of the table.  The
 * responsibility is on the user for preventing recursive cycling.
 *
 * \par
 * See itk::EquivalencyTable for more information
 * \ingroup WatershedSegmentation
 *
 * \sa EquivalencyTable  */
template < typename TValueType=unsigned long, unsigned long FlattenPeriod=0, typename THash=hash<TValueType> >
class ITKCommon_EXPORT OneWayEquivalencyTable : public DataObject
{
public:
  /**  Standard typedefs and smart pointer declarations.   */
  typedef OneWayEquivalencyTable Self;
  typedef DataObject Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);
  itkTypeMacro(OneWayEquivalencyTable, DataObject);

  /** Define the container type for this table */
  typedef hash_map<TValueType, TValueType, THash > HashTableType;
  typedef typename HashTableType::iterator Iterator;
  typedef typename HashTableType::const_iterator ConstIterator;
  typedef typename HashTableType::value_type ValueType;
  
  /** ``Flattens'' the equivalency table by eliminating all redundant
   * and recursive equivalencies.  I.e. the set { 2=1; 3=2; 4=3 } is
   * converted to {4=1; 3=1; 2=1}.  */
  void Flatten();

  /** Insert an equivalency into the table.  A return value of TRUE
  * indicates that the equivalency did not previously exist in the
  * table and was successfully added.  A FALSE return value indicates
  * that the equivalency was not added to the table because a conflict
  * with an existing entry occurred (most likely, the equivalency was
  * already recorded directly or indirectly).
  */
  bool Add(TValueType a, TValueType b);

  /** Lookup an equivalency in the table.  If no entry is found in the
   * table, the method returns its the value of the argument.  Does
   * not recursively descent through equivalencies.  */
  TValueType Lookup(const TValueType a) const
  {
    ConstIterator result = m_HashMap.find(a);
    if ( result == m_HashMap.end() ) return a;
    else return (*result).second;
  }
  
  /** Lookup an equivalency in the table by recursing through all
   * successive equivalencies.  For example, if the follow entries
   * exist in the table {8=7, 7=6, 6=5}, then RecursiveLookup(8)
   * returns 5.  */  
  TValueType RecursiveLookup(const TValueType a) const;

  /** Returns TRUE if the label is found in the table and FALSE is the
   * label is not found in the table.  */
  bool IsEntry(const TValueType a) const
  {
    if ( m_HashMap.find(a) == m_HashMap.end() ) return false;
    else return true;
  }
  
  /**  Erases the entry with key a.   */
  void Erase(const TValueType a)
  {  m_HashMap.erase(a); }

  /** Erases all the entries in the table.   */
  void Clear()
  {      m_HashMap.clear();    }

  /** Returns TRUE if the table is empty, FALSE if it is not empty.   */
  bool Empty() const
  {      return m_HashMap.empty();    }
  
  /** Returns an iterator pointing to the first element of the (unordered)
      table.    */
  Iterator Begin() { return m_HashMap.begin(); }

  /** Returns and iterator pointing to one position past the last
   * element of the (unordered) table.  */  
  Iterator End()   { return m_HashMap.end();   }

  /** Convenience method for debugging.   */
  //  void PrintHashTable();
  
protected:
  OneWayEquivalencyTable()
    {
    m_Count = 0;
    }
  virtual ~OneWayEquivalencyTable() {}
  OneWayEquivalencyTable(const Self&); // purposely not implemented
  void operator=(const Self&); // purposely not implemented
  void PrintSelf(std::ostream& os, Indent indent) const;

  HashTableType m_HashMap;

  unsigned long m_Count;
};

}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOneWayEquivalencyTable.txx"
#endif

#endif

