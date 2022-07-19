#include "pch.h"
#include "AdminSessionClass.h"
#include "AdminSessionSecure.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminSessionClass


/////////////////////////////////////////////////////////////////////////////
// Implementation

bool CAdminSessionClass::IsEqualBSTR(BSTR bstr1, BSTR bstr2)
{
  // Equal if both are empty
  if (!BSTRLen(bstr1) && !BSTRLen(bstr2))
    return true;
  else if (!BSTRLen(bstr2))
    return false;

  // Compare strings
  return 0 == wcscmp(bstr1, bstr2);    
}


/////////////////////////////////////////////////////////////////////////////
// IClassFactory Interface Methods

STDMETHODIMP CAdminSessionClass::CreateInstance(IUnknown* pUnkOuter,
  REFIID riid, void** ppvObject)
{
  #if !defined(_ALLEGIANCE_PROD_) //changed to production only Imago 6/10

    // Perform default processing
    return CComClassFactory::CreateInstance(pUnkOuter, riid, ppvObject);

  #else // !defined(ALLSRV_STANDALONE)

    // Completely disallow normal object creation in standalone server
    return E_INVALIDARG;

  #endif // !defined(ALLSRV_STANDALONE)
}


/////////////////////////////////////////////////////////////////////////////
// IAdminSessionClass Interface Methods

STDMETHODIMP CAdminSessionClass::CreateSession(IAdminSessionHost* pHost,
  IAdminSession** ppSession)
{
  // pHost must be valid
  if (!pHost)
    return E_INVALIDARG;

  // Create a decoy cookie based on the current FILETIME
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);

  // Get the IUnknown from the admin session host
  IUnknownPtr spUnk;
  HRESULT hr = pHost->GetIdentity(ft.dwLowDateTime, &spUnk);
  if (hr != (ft.dwLowDateTime & 0x7FFFFFFF))
    return E_INVALIDARG;

  // IUnknown must support IStream
  IStreamPtr spStm(spUnk);
  if (NULL == spStm)
    return E_INVALIDARG;

  // Create a decoded copy of the specified stream
  IStreamPtr spStmOut;
  RETURN_FAILED(CAdminSessionSecure<
    &CLSID_AdminInterfaces>::CreateDecryptedStream(spStm, &spStmOut));

  // Rewind the decoded stream
  LARGE_INTEGER li = {0};
  RETURN_FAILED(spStmOut->Seek(li, STREAM_SEEK_SET, NULL));

  // Create a version info object for the host
  IAGCVersionInfoPtr spVersionHost;
  RETURN_FAILED(spVersionHost.CreateInstance(CLSID_AGCVersionInfo));
  
  // Initialize the version info object from the decoded stream
  IPersistStreamPtr spPersist(spVersionHost);
  if (NULL == spPersist)
    return E_UNEXPECTED;
  RETURN_FAILED(spPersist->Load(spStmOut));

  // Create a version info object for this module
  IAGCVersionInfoPtr spVersion;
  RETURN_FAILED(spVersion.CreateInstance(CLSID_AGCVersionInfo));
  RETURN_FAILED(spVersion->put_FileName(NULL));

  // Compare exactly for several fields of the version info object

  // CompanyName
  {
    CComBSTR bstr, bstrHost;
    RETURN_FAILED(spVersion->get_CompanyName(&bstr));
    RETURN_FAILED(spVersionHost->get_CompanyName(&bstrHost));
    if (!IsEqualBSTR(bstr, bstrHost))
      return E_INVALIDARG;
  }
  // LegalCopyright
  {
    CComBSTR bstr, bstrHost;
    RETURN_FAILED(spVersion->get_LegalCopyright(&bstr));
    RETURN_FAILED(spVersionHost->get_LegalCopyright(&bstrHost));
    if (!IsEqualBSTR(bstr, bstrHost))
      return E_INVALIDARG;
  }

  // Now, we can FINALLY create the sesion by calling the base class
  return CComClassFactory::CreateInstance(NULL, IID_IAdminSession,
    (void**)ppSession);
}

