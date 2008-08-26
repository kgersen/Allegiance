/////////////////////////////////////////////////////////////////////////////
// SymGuardApp.cpp : Implementation of the CSymGuardApp class.

#include "pch.h"
#include <SymGuard.h>
#include "SymInfo.h"
#include "SymModule.h"
#include "SymGuardVersion.h"
#include "SymGuardApp.h"


/////////////////////////////////////////////////////////////////////////////
// CSymGuardApp

TC_OBJECT_EXTERN_IMPL(CSymGuardApp)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

HRESULT CSymGuardApp::FinalConstruct()
{
  debugf("CSymGuardApp::FinalConstruct(this=%08X)\n", this);

  // Create an instance of the Free-Threaded Marshaler
  RETURN_FAILED(::CoCreateFreeThreadedMarshaler(GetControllingUnknown(),
    &m_spUnkMarshaler.p));

  // Indicate success
  return S_OK;
}

void CSymGuardApp::FinalRelease()
{
  // Flush the module cache
  FlushAllModulesFromCache(NULL);

  debugf("CSymGuardApp::FinalRelease(this=%08X)\n", this);
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CSymGuardApp::FlushModuleFromCache(const ZString& strCacheKey)
{
  XLock lock(this);
  XModuleIt it = m_ModuleCache.find(strCacheKey);
  if (it == m_ModuleCache.end())
    return E_INVALIDARG;
  it->second->Release();
  m_ModuleCache.erase(it);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CSymGuardApp::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_ISymGuardApp,
  };

  for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ISymGuardApp Interface Methods

STDMETHODIMP CSymGuardApp::LoadModule(BSTR bstrSymbolPath, BSTR bstrModuleName,
  VARIANT* pvarImageBase, VARIANT* pvarImageSize, ISymModule** ppModule)
{
  // Initialize the [out] parameter
  CLEAROUT(ppModule, (ISymModule*)NULL);

  // Create a cache key for the specified module parameters
  ZString strKey;
  RETURN_FAILED(CSymModule::GetCacheKey(bstrSymbolPath, bstrModuleName,
    pvarImageBase, pvarImageSize, strKey));

  // Lookup the specified module in the cache
  CSymModule* pModule = NULL;
  {
    XLock lock(this);
    XModuleIt it = m_ModuleCache.find(strKey);
    if (it != m_ModuleCache.end())
    {
      pModule = it->second;
    }
    else
    {
      // Create a new instance of CSymModule
      CComObject<CSymModule>* pModuleTemp = NULL;
      RETURN_FAILED(pModuleTemp->CreateInstance(&pModuleTemp));

      // Cache the new object
      pModule = pModuleTemp;
      m_ModuleCache[strKey] = pModule;

      // AddRef the cached module object
      pModule->AddRef();
    }
  }

  // Initialize the module object, if needed
  assert(pModule);
  HRESULT hr = pModule->Init(bstrSymbolPath, bstrModuleName, pvarImageBase,
     pvarImageSize, this, strKey);
  if (FAILED(hr))
  {
    ZSucceeded(FlushModuleFromCache(strKey));
    return hr;
  }

  // Put a reference count on the object as an [out] parameter
  (*ppModule = pModule)->AddRef();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymGuardApp::FlushAllModulesFromCache(long* pcModulesFlushed)
{
  XLock lock(this);

  // Initialize the [out] parameter
  CLEAROUT_ALLOW_NULL(pcModulesFlushed, (long)m_ModuleCache.size());

  // Release each cached module object
  for (XModuleIt it = m_ModuleCache.begin(); it != m_ModuleCache.end(); ++it)
    it->second->Release();

  // Clear the module cache
  m_ModuleCache.clear();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymGuardApp::get_Version(ISymGuardVersion** ppVersion)
{
  // Initialize the [out] parameter
  CLEAROUT(ppVersion, (ISymGuardVersion*)NULL);

  // Create an instance of the version object
  CComObject<CSymGuardVersion>* pVersion = NULL;
  RETURN_FAILED(pVersion->CreateInstance(&pVersion));
  ISymGuardVersionPtr spVersion(pVersion);

  // Initialize the version object
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(_Module.GetModuleInstance(), szModule, sizeofArray(szModule));
  RETURN_FAILED(spVersion->put_FileName(CComBSTR(szModule)));

  // Detach the object to the [out] parameter
  *ppVersion = spVersion.Detach();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymGuardApp::get_DbgHelpVersion(ISymGuardVersion** ppVersion)
{
  // Initialize the [out] parameter
  CLEAROUT(ppVersion, (ISymGuardVersion*)NULL);

  // Call a Win32 Symbol Engine API, to ensure that it's been delay-loaded
  ::SymGetOptions();

  // Get the module handle of "DBGHELP.DLL"
  HMODULE hDbgHelp = ::GetModuleHandle(TEXT("DBGHELP.DLL"));
  if (!hDbgHelp)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Create an instance of the version object
  CComObject<CSymGuardVersion>* pVersion = NULL;
  RETURN_FAILED(pVersion->CreateInstance(&pVersion));
  ISymGuardVersionPtr spVersion(pVersion);

  // Initialize the version object
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(hDbgHelp, szModule, sizeofArray(szModule));
  RETURN_FAILED(spVersion->put_FileName(CComBSTR(szModule)));

  // Detach the object to the [out] parameter
  *ppVersion = spVersion.Detach();

  // Indicate success
  return S_OK;
}

