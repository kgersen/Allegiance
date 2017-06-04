#include "pch.h"
#include <TCLib.h>

#include "ExcepInfo.h"


/////////////////////////////////////////////////////////////////////////////
// Operations

void TCEXCEPINFO::Release(EXCEPINFO* pei)
{
  SysFreeString(pei->bstrDescription);
  SysFreeString(pei->bstrHelpFile);
  SysFreeString(pei->bstrSource);
  pei->bstrDescription = NULL;
  pei->bstrHelpFile = NULL;
  pei->bstrSource = NULL;
}

HRESULT TCEXCEPINFO::CreateErrorInfo(EXCEPINFO* pei, IErrorInfo** ppei)
{
  // Return S_FALSE if pei is NULL
  if (!pei)
    return S_FALSE;

  // If the EXCEPINFO is deferred, now is the time
  if (pei->pfnDeferredFillIn)
    RETURN_FAILED((*pei->pfnDeferredFillIn)(pei));

  // Create error info object
  ICreateErrorInfoPtr spcei;
  RETURN_FAILED(::CreateErrorInfo(&spcei));

  // Set the fields of the error info object
  RETURN_FAILED(spcei->SetDescription(pei->bstrDescription));
  RETURN_FAILED(spcei->SetGUID(GUID_NULL));
  RETURN_FAILED(spcei->SetHelpContext(pei->dwHelpContext));
  RETURN_FAILED(spcei->SetHelpFile(pei->bstrHelpFile));
  RETURN_FAILED(spcei->SetSource(pei->bstrSource));

  // QI for IErrorInfo
  return spcei->QueryInterface(IID_IErrorInfo, (void**)ppei);
}

HRESULT TCEXCEPINFO::SetErrorInfo(EXCEPINFO* pei)
{
  IErrorInfoPtr spei;
  RETURN_FAILED(CreateErrorInfo(pei, &spei));
  return ::SetErrorInfo(NULL, spei);
}


