#ifndef __ComboValueTranslator_h__
#define __ComboValueTranslator_h__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// ComboValueTranslator.h : Declaration of the IComboValueTranslator
// interface.
//


/////////////////////////////////////////////////////////////////////////////
//
//
class IComboValueTranslator
{
public:
  virtual int GetDefaultComboIndex() = 0;
  virtual int ComboIndexFromValue(int nValue) = 0;
  virtual int ValueFromComboIndex(int nIndex) = 0;
};


/////////////////////////////////////////////////////////////////////////////
//
//
template <class T>
class IComboValueTranslatorImpl : public IComboValueTranslator
{
public:
  virtual int GetDefaultComboIndex()
  {
    T* pThis = static_cast<T*>(this);
    int cElements;
    int iDefault;
    pThis->ProvideArray(&cElements, &iDefault);
    return iDefault;
  }
  virtual int ComboIndexFromValue(int nValue)
  {
    T* pThis = static_cast<T*>(this);
    int cElements;
    int iDefault;
    const static int* pArray = pThis->ProvideArray(&cElements, &iDefault);
    for (int i = 0; i < cElements; ++i)
      if (pArray[i] == nValue)
        return i;
    return iDefault;    
  }
  virtual int ValueFromComboIndex(int nIndex)
  {
    T* pThis = static_cast<T*>(this);
    int cElements;
    int iDefault;
    const static int* pArray = pThis->ProvideArray(&cElements, &iDefault);
    return pArray[nIndex];
  }

public:
  const static int* ProvideArray(int* pcElements, int* piDefault);
};


/////////////////////////////////////////////////////////////////////////////
//
//
#define BEGIN_ComboTranslator(NAME, DEFAULT)                                \
  class NAME : public IComboValueTranslatorImpl<CTeamCountTranslator>       \
  {                                                                         \
  public:                                                                   \
    const static int* ProvideArray(int* pcElements, int* piDefault)         \
    {                                                                       \
      const static int s_iDefault = DEFAULT;                                \
      const static int s_array[] =                                          \
      {


/////////////////////////////////////////////////////////////////////////////
//
//
#define END_ComboTranslator(NAME)                                           \
      };                                                                    \
      const static int s_cElements = sizeof(s_array) / sizeof(s_array[0]);  \
                                                                            \
      *pcElements = s_cElements;                                            \
      *piDefault = s_iDefault;                                              \
    }                                                                       \
  } s_##NAME##ComboTranslator;


/////////////////////////////////////////////////////////////////////////////

#endif // !__ComboValueTranslator_h__
