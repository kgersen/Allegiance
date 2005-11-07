/////////////////////////////////////////////////////////////////////////////
// SymModuleImpl.cpp : Implementation of the CSymModuleImpl class.

#include "pch.h"
#include "SymModuleImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CSymModuleImpl


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

TCAutoCriticalSection CSymModuleImpl::s_cs;
CSymModuleImpl::XModules CSymModuleImpl::s_ModuleCache;


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CSymModuleImpl::CSymModuleImpl() :
  m_nMinutes(30),
  m_dwImageBase(0),
  m_bInDestructionLock(false)
{
  // Initialize
  UnloadModule();
}

HRESULT CSymModuleImpl::FinalConstruct()
{
  // Indicate success
  return S_OK;
}

void CSymModuleImpl::FinalRelease()
{
  // Remove our interface pointer from the static table
  {
    XStaticLock lock(&s_cs);
    XModuleIt it = s_ModuleCache.find(m_strCacheKey);
    if (it != s_ModuleCache.end())
      s_ModuleCache.erase(it);
  }

  // Unlock the additional static lock
  if (m_bInDestructionLock)
  {
    s_cs.Unlock();
    m_bInDestructionLock = false;
  }

  // Unload the current module, if any
  UnloadModule();

  // Terminate the symbol engine
  ::SymCleanup(GetSymHandle());
}

HRESULT CSymModuleImpl::Init(BSTR bstrSymbolPath, BSTR bstrModuleName,
  DWORD dwImageBase, DWORD dwImageSize, const ZString& strCacheKey)
{
  // Save the specified cache key
  m_strCacheKey = strCacheKey;

  // Set the symbol engine options
  DWORD dwOptions = ::SymSetOptions(SYMOPT_LOAD_LINES);

  // Initialize the symbol engine
  USES_CONVERSION;
  PSTR pszSymbolPath = BSTRLen(bstrSymbolPath) ?
    OLE2A(bstrSymbolPath) : NULL;
  BOOL bInit = ::SymInitialize(GetSymHandle(), pszSymbolPath, false);
  if (!bInit)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Load the specified symbol file
  PSTR pszModuleName = OLE2A(bstrModuleName);
  ::SetLastError(0);
  DWORD dwLoad = ::SymLoadModule(GetSymHandle(), NULL, pszModuleName, NULL,
    dwImageBase, dwImageSize);
  if (!dwLoad)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Save the module base address
  m_dwImageBase = dwImageBase;

  // Get the module information
  assert(sizeof(m_im) == m_im.SizeOfStruct);
  assert(0 == m_im.BaseOfImage);
  BOOL bGet = ::SymGetModuleInfo(GetSymHandle(), dwLoad, &m_im);
  if (!bGet)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Indicate success
  return S_OK;
}

void CSymModuleImpl::UnloadModule()
{
  // Initialize the IMAGEHELP_MODULE structure
  ::ZeroMemory(&m_im, sizeof(m_im));
  m_im.SizeOfStruct = sizeof(m_im);

  // Unload the current module, if any
  if (m_dwImageBase)
  {
    ::SymUnloadModule(GetSymHandle(), m_dwImageBase);
    m_dwImageBase = 0;
  }

  // Clear the cache key
  m_strCacheKey.SetEmpty();
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CSymModuleImpl::CreateModule(BSTR bstrSymbolPath, BSTR bstrModuleName,
  VARIANT* pvarImageBase, VARIANT* pvarImageSize, ISymModule** ppSymModule)
{
  // Validate the specified parameters
  if (!BSTRLen(bstrModuleName))
    return E_INVALIDARG;
  DWORD dwImageBase, dwImageSize;
  RETURN_FAILED(VariantHexToDWORD(pvarImageBase, &dwImageBase));
  RETURN_FAILED(VariantHexToDWORD(pvarImageSize, &dwImageSize));
  CLEAROUT(ppSymModule, (ISymModule*)NULL);

  // Create a key string
  int cchSymbolPath = BSTRLen(bstrSymbolPath);
  int cchModuleName = BSTRLen(bstrModuleName);
  int cch = cchSymbolPath + 1 + cchModuleName + 1 + 8 + 1 + 8;
  LPOLESTR pszKey = reinterpret_cast<LPOLESTR>(_alloca((cch + 1) * sizeof(OLECHAR)));
  swprintf(pszKey, OLESTR("%ls:%ls:%08X:%08X"),
    cchSymbolPath ? bstrSymbolPath : OLESTR(""),
    cchModuleName ? bstrModuleName : OLESTR(""), dwImageBase, dwImageSize);
  _wcsupr(pszKey);

  // Lock the statics for the remainder of the scope
  XStaticLock lock(&s_cs);

  // Find a module already associated with the key
  USES_CONVERSION;
  ZString strCacheKey(OLE2CT(pszKey));
  XModuleIt it = s_ModuleCache.find(strCacheKey);
  if (s_ModuleCache.end() != it)
  {
    (*ppSymModule = it->second)->AddRef();
    return S_OK;
  }

  // Create a new instance of CSymModuleImpl
  CComObject<CSymModuleImpl>* pModule = NULL;
  RETURN_FAILED(pModule->CreateInstance(&pModule));
  HRESULT hr = pModule->Init(bstrSymbolPath, bstrModuleName,
    dwImageBase, dwImageSize, strCacheKey);
  if (FAILED(hr))
  {
    delete pModule;
    return hr;
  }

  // Put a reference count on the new object
  ZSucceeded(hr = pModule->QueryInterface(IID_ISymModule, (void**)ppSymModule));

  // Add the new instance to the cache (without a refcount)
  s_ModuleCache[pModule->m_strCacheKey] = *ppSymModule;

  // Return the last result
  return hr;
}

HRESULT CSymModuleImpl::VariantHexToDWORD(VARIANT* pvar, DWORD* pdw)
{
  assert(pvar);
  assert(pdw);

  HRESULT hr = S_OK;
  VARTYPE vt = V_VT(pvar);
  ULONG   ul = 0;
  if (VT_ERROR == vt && DISP_E_PARAMNOTFOUND == V_ERROR(pvar))
  {
    ul = 0;
    hr = S_FALSE;
  }
  else if (VT_EMPTY == vt)
  {
    ul = 0;
    hr = S_FALSE;
  }
  else if (VT_BSTR == vt)
  {
    // Skip past whitespace
    LPCOLESTR psz = V_BSTR(pvar);
    while (iswspace(*psz))
      ++psz;

    // Skip past "0x" or "&H"
    if (0 == _wcsnicmp(psz, OLESTR("0x"), 2))       // JScript/ECMAScript/C/C++
      psz += 2;
    else if (0 == _wcsnicmp(psz, OLESTR("&H"), 2))  // VBScript/VBA/VB
      psz += 2;

    // Convert as base16
    ul = wcstoul(psz, NULL, 16);
    if (!ul && ERANGE == errno)
      return E_INVALIDARG;
  }
  else if (VT_I1 == vt || VT_UI1 == vt)
  {
    hr = ::VarUI4FromUI1(V_UI1(pvar), &ul);
  }
  else if (VT_I2 == vt || VT_UI2 == vt)
  {
    hr = ::VarUI4FromUI2(V_UI2(pvar), &ul);
  }
  else if (VT_I4 == vt || VT_UI4 == vt)
  {
    ul = V_UI4(pvar);
  }

  // Return the last result
  *pdw = ul;
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

ULONG CSymModuleImpl::InternalRelease()
{
  // Lock the statics for the remainder of the scope
  XStaticLock lock(&s_cs);

  // Perform default processing
  ULONG ul = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();

  // If we're about to be deleted, add an additional lock until FinalRelease
  if (0 == ul)
  {
    m_bInDestructionLock = true;
    s_cs.Lock();
  }

  // Return the remaining refcount
  return ul;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline HANDLE CSymModuleImpl::GetSymHandle() const
{
  return
    reinterpret_cast<HANDLE>(
      static_cast<ISymModule*>(
        const_cast<CSymModuleImpl*>(this)));
}


/////////////////////////////////////////////////////////////////////////////
// IDispatch Interface Methods

STDMETHODIMP CSymModuleImpl::GetTypeInfoCount(UINT*)
{
  // Fail since this class should not be delegated to on IDispatch methods
  ZError("CSymModuleImpl::GetTypeInfoCount() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::GetTypeInfo(UINT, LCID, ITypeInfo**)
{
  // Fail since this class should not be delegated to on IDispatch methods
  ZError("CSymModuleImpl::GetTypeInfo() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*)
{
  // Fail since this class should not be delegated to on IDispatch methods
  ZError("CSymModuleImpl::GetIDsOfNames() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*,
  VARIANT*, EXCEPINFO*, UINT*)
{
  // Fail since this class should not be delegated to on IDispatch methods
  ZError("CSymModuleImpl::Invoke() should never be delegated to!");
  return E_NOTIMPL;
}


/////////////////////////////////////////////////////////////////////////////
// ISymModule Interface Methods

STDMETHODIMP CSymModuleImpl::FlushModuleFromCache()
{
  // TODO: Remove from the cache

  // Indicate sucess
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::put_Timeout(long nMinutes)
{
  // Flush immediately if zero was specified
  if (0 == nMinutes)
    return FlushModuleFromCache();

  // Save the specified value
  XLock lock(this);
  m_nMinutes = nMinutes;

  // TODO: Update timeout thread

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_Timeout(long* pnMinutes)
{
  // Initialize the [out] parameter
  XLock lock(this);
  CLEAROUT(pnMinutes, m_nMinutes);

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::GetSymbolFromAddress(VARIANT* pvarAddress,
  ISymInfo** ppSymInfo)
{
  // Fail since this class should not be delegated to on this method
  ZError("CSymModuleImpl::GetSymbolFromAddress() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::put_BuildPathBase(BSTR bstr)
{
  // Fail since this class should not be delegated to on this method
  ZError("CSymModuleImpl::put_BuildPathBase() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::get_BuildPathBase(BSTR* pbstr)
{
  // Fail since this class should not be delegated to on this method
  ZError("CSymModuleImpl::get_BuildPathBase() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::put_SourcePathBase(BSTR bstr)
{
  // Fail since this class should not be delegated to on this method
  ZError("CSymModuleImpl::put_SourcePathBase() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::get_SourcePathBase(BSTR* pbstr)
{
  // Fail since this class should not be delegated to on this method
  ZError("CSymModuleImpl::get_SourcePathBase() should never be delegated to!");
  return E_NOTIMPL;
}

STDMETHODIMP CSymModuleImpl::get_SymbolPath(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Get the symbol search path
  char szPath[8 * 1024];
  if (!::SymGetSearchPath(GetSymHandle(), szPath, sizeofArray(szPath)))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Create a BSTR
  USES_CONVERSION;
  *pbstr = ::SysAllocString(A2COLE(szPath));
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_ModuleName(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  USES_CONVERSION;
  *pbstr = ::SysAllocString(A2COLE(m_im.ModuleName));
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_ImageBase(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.BaseOfImage;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_ImageSize(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.ImageSize;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_TimeDateStamp(DATE* pd)
{
  // Initialize the [out] parameter
  CLEAROUT(pd, (DATE)0.f);

  // Convert the time value to a usable form
  time_t timer = static_cast<time_t>(m_im.TimeDateStamp);
  tm* theTime = gmtime(&timer);

  // Initialize a SYSTEMTIME structure
  SYSTEMTIME st;
  ZeroMemory(&st, sizeof(st));
  st.wYear      = theTime->tm_year + 1900;
  st.wMonth     = theTime->tm_mon  +    1;
  st.wDayOfWeek = theTime->tm_wday;
  st.wDay       = theTime->tm_mday;
  st.wHour      = theTime->tm_hour;
  st.wMinute    = theTime->tm_min;
  st.wSecond    = theTime->tm_sec;

  // Convert the time to local time
  SYSTEMTIME stLocal;
  TCSystemTimeToTzSpecificLocalTime(NULL, &st, &stLocal);

  // Convert the time to Variant time
  if (!::SystemTimeToVariantTime(&stLocal, pd))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_CheckSum(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.CheckSum;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_SymbolCount(long* pc)
{
  // Initialize the [out] parameter
  CLEAROUT(pc, static_cast<long>(m_im.NumSyms));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_SymbolType(BSTR* pbstr)
{
  // Define a macro to simplify mapping of type to name
  #define CSymModuleImpl_SymType(type)                                      \
    case type:                                                              \
      pszType = OLESTR(#type);                                              \
      break;

  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  LPOLESTR pszType = NULL;
  switch (m_im.SymType)
  {
    CSymModuleImpl_SymType(SymNone)
    CSymModuleImpl_SymType(SymCoff)
    CSymModuleImpl_SymType(SymCv)
    CSymModuleImpl_SymType(SymPdb)
    CSymModuleImpl_SymType(SymExport)
    CSymModuleImpl_SymType(SymDeferred)
    CSymModuleImpl_SymType(SymSym)
  }

  // Create a BSTR
  if (pszType)
  {
    *pbstr = ::SysAllocString(pszType);
    if (NULL == *pbstr)
      return E_OUTOFMEMORY;
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_ImageName(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  USES_CONVERSION;
  *pbstr = ::SysAllocString(A2COLE(m_im.ImageName));
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModuleImpl::get_LoadedImageName(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  USES_CONVERSION;
  *pbstr = ::SysAllocString(A2COLE(m_im.LoadedImageName));
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

