/////////////////////////////////////////////////////////////////////////////
// Pig$$Object$$.cpp : Implementation of the CPig$$Object$$ class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>

#include "Pig$$Object$$.h"


/////////////////////////////////////////////////////////////////////////////
// CPig$$Object$$

TC_OBJECT_EXTERN(CPig$$Object$$)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPig$$Object$$::CPig$$Object$$()
{
}

HRESULT CPig$$Object$$::FinalConstruct()
{
  // Indicate success
  return S_OK;
}

void CPig$$Object$$::FinalRelease()
{
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPig$$Object$$::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPig$$Object$$,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPig$$Object$$ Interface Methods

