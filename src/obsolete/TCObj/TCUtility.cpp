/////////////////////////////////////////////////////////////////////////////
// TCUtility.cpp | Implementation of the CTCUtility class.
//

#include "pch.h"
#include "TCUtility.h"
#include <..\TCLib\UtilImpl.h>


/////////////////////////////////////////////////////////////////////////////
// CTCUtility

TC_OBJECT_EXTERN_IMPL(CTCUtility)


/////////////////////////////////////////////////////////////////////////////
// ITCUtility Interface Methods

STDMETHODIMP CTCUtility::CreateObject(BSTR bstrProgID, BSTR bstrComputer,
  IUnknown** ppUnk)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::CreateObject(bstrProgID, bstrComputer, ppUnk);
}

STDMETHODIMP CTCUtility::ObjectReference(IUnknown* pUnk,
  BSTR* pbstrObjectReference)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::ObjectReference(pUnk, pbstrObjectReference);
}

STDMETHODIMP CTCUtility::Sleep(long nDurationMS)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::Sleep(nDurationMS);
}

STDMETHODIMP CTCUtility::GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
  long nTimeoutMS, IUnknown** ppUnk)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::GetObject(bstrMoniker, bAllowUI, nTimeoutMS, ppUnk);
}

