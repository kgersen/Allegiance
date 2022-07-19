/////////////////////////////////////////////////////////////////////////////
// AGCGame.cpp : Implementation of CAGCGame
//

#include "pch.h"
#include "AGCGame.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCGame

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCGame)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCGame::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IAGCGame
  };
  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}


