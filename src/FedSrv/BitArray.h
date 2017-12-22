/*-------------------------------------------------------------------------
 * BitArray.h
 * 
 * Class for bit matching. 
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _BIT_ARRAY_
#define _BIT_ARRAY_

class CBitArray
{
public:  
  CBitArray(int nBits);
  CBitArray(int nBitsMin, int nBitsMax); // minimum is inclusive, maximum is exclusive
  ~CBitArray();
  bool FIsSet(int x, int y);
  bool Set(int x, int y, bool b);
  void ClearRow(int x);
  void ClearColumn(int y);
  void CBitArray::Dump(int min, int max);

private:
  unsigned int * m_pargBits;
  int m_nBits;
  int m_nMin;
  int m_cElemsPerRow; // how many m_pargBits's we need per row
};


#endif // _BIT_ARRAY_

