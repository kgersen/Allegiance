/////////////////////////////////////////////////////////////////////////////
// SymModule.cpp : Implementation of the CSymModule class.

#include "pch.h"
#include "SymInfo.h"
#include "SymModule.h"
#include "SymGuardApp.h"


/////////////////////////////////////////////////////////////////////////////
// CSymModule

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CSymModule)


/////////////////////////////////////////////////////////////////////////////
// Construction

CSymModule::CSymModule() :
  m_nMinutes(30),
  m_dwImageBase(0),
  m_pApp(NULL)
{
  UnloadModule();
}

HRESULT CSymModule::FinalConstruct()
{
  debugf("CSymModule::FinalConstruct(this=%08X)\n", this);

  // Create an instance of the Free-Threaded Marshaler
  RETURN_FAILED(::CoCreateFreeThreadedMarshaler(GetControllingUnknown(),
    &m_spUnkMarshaler.p));

  // Indicate success
  return S_OK;
}

void CSymModule::FinalRelease()
{
  // Release the Free-Threaded Marshaler
  m_spUnkMarshaler = NULL;

  // Unload the module
  UnloadModule();

  debugf("CSymModule::FinalRelease(this=%08X)\n", this);
}

HRESULT CSymModule::Init(BSTR bstrSymbolPath, BSTR bstrModuleName,
  VARIANT* pvarImageBase, VARIANT* pvarImageSize, CSymGuardApp* pApp,
  const ZString& strCacheKey)
{
  // Do nothing if already initialized
  XLock lock(this);
  if (m_pApp)
    return S_OK;

  // Validate the specified parameters
  if (!BSTRLen(bstrModuleName))
    return E_INVALIDARG;
  DWORD dwImageBase, dwImageSize;
  RETURN_FAILED(VariantHexToDWORD(pvarImageBase, &dwImageBase));
  RETURN_FAILED(VariantHexToDWORD(pvarImageSize, &dwImageSize));

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

  // Save application pointer, cache key string, and the module base address
  m_pApp        = pApp;
  m_strCacheKey = strCacheKey;
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

void CSymModule::UnloadModule()
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

  // Terminate the symbol engine
  ::SymCleanup(GetSymHandle());
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

HRESULT CSymModule::GetCacheKey(BSTR bstrSymbolPath, BSTR bstrModuleName,
  VARIANT* pvarImageBase, VARIANT* pvarImageSize, ZString& strKey)
{
  // Initialize the [out] parameter
  strKey.SetEmpty();

  // Validate the specified parameters
  if (!BSTRLen(bstrModuleName))
    return E_INVALIDARG;
  DWORD dwImageBase, dwImageSize;
  RETURN_FAILED(VariantHexToDWORD(pvarImageBase, &dwImageBase));
  RETURN_FAILED(VariantHexToDWORD(pvarImageSize, &dwImageSize));

  // Create a key string
  int cchSymbolPath = BSTRLen(bstrSymbolPath);
  int cchModuleName = BSTRLen(bstrModuleName);
  int cch = cchSymbolPath + 1 + cchModuleName + 1 + 8 + 1 + 8;
  LPOLESTR pszKey = reinterpret_cast<LPOLESTR>(_alloca((cch + 1) * sizeof(OLECHAR)));
  swprintf(pszKey, OLESTR("%ls:%ls:%08X:%08X"),
    cchSymbolPath ? bstrSymbolPath : OLESTR(""),
    cchModuleName ? bstrModuleName : OLESTR(""), dwImageBase, dwImageSize);
  _wcsupr(pszKey);

  // Save the key string to the [out] parameter
  USES_CONVERSION;
  strKey = OLE2CT(pszKey);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CSymModule::VariantHexToDWORD(VARIANT* pvar, DWORD* pdw)
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
// Implementation

inline HANDLE CSymModule::GetSymHandle() const
{
  return
    reinterpret_cast<HANDLE>(
      static_cast<ISymModule*>(
        const_cast<CSymModule*>(this)));
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CSymModule::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_ISymModule,
  };

  for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ISymModule Interface Methods

STDMETHODIMP CSymModule::GetSymbolFromAddress(VARIANT* pvarAddress,
  ISymInfo** ppSymInfo)
{
  // Initialize the [out] parameter
  CLEAROUT(ppSymInfo, (ISymInfo*)NULL);  

  // Validate the specified address parameter
  DWORD dwAddr;
  RETURN_FAILED(VariantHexToDWORD(pvarAddress, &dwAddr));

  // Create a new instance of CSymInfo
  CComObject<CSymInfo>* pInfo = NULL;
  RETURN_FAILED(pInfo->CreateInstance(&pInfo));
  HRESULT hr = pInfo->Init(GetSymHandle(), dwAddr, this);
  if (FAILED(hr))
  {
    delete pInfo;
    return hr;
  }

  // Put a reference count on the new object
  ZSucceeded(hr = pInfo->QueryInterface(IID_ISymInfo, (void**)ppSymInfo));

  // Return the last result
  return hr;
}

STDMETHODIMP CSymModule::FlushModuleFromCache()
{
  // Flush ourself from the cache
  assert(m_pApp);
  ZSucceeded(m_pApp->FlushModuleFromCache(m_strCacheKey));

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::put_Timeout(long nMinutes)
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

STDMETHODIMP CSymModule::get_Timeout(long* pnMinutes)
{
  // Initialize the [out] parameter
  XLock lock(this);
  CLEAROUT(pnMinutes, m_nMinutes);

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::put_BuildPathBase(BSTR bstr)
{
  // Save a copy of the specified string
  XLock lock(this);
  m_bstrBuildPathBase = bstr;

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::get_BuildPathBase(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a copy of the string
  {
    XLock lock(this);
    *pbstr = m_bstrBuildPathBase.Copy();
  }
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::put_SourcePathBase(BSTR bstr)
{
  // Save a copy of the specified string
  XLock lock(this);
  m_bstrSourcePathBase = bstr;

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::get_SourcePathBase(BSTR* pbstr)
{
  // Create a copy of the string
  {
    XLock lock(this);
    *pbstr = m_bstrSourcePathBase.Copy();
  }
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate sucess
  return S_OK;
}

STDMETHODIMP CSymModule::get_SymbolPath(BSTR* pbstr)
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

STDMETHODIMP CSymModule::get_ModuleName(BSTR* pbstr)
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

STDMETHODIMP CSymModule::get_ImageBase(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.BaseOfImage;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModule::get_ImageSize(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.ImageSize;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModule::get_TimeDateStamp(DATE* pd)
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

STDMETHODIMP CSymModule::get_CheckSum(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_im.CheckSum;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModule::get_SymbolCount(long* pc)
{
  // Initialize the [out] parameter
  CLEAROUT(pc, static_cast<long>(m_im.NumSyms));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymModule::get_SymbolType(BSTR* pbstr)
{
  // Define a macro to simplify mapping of type to name
  #define CSymModule_SymType(type)                                          \
    case type:                                                              \
      pszType = OLESTR(#type);                                              \
      break;

  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  LPOLESTR pszType = NULL;
  switch (m_im.SymType)
  {
    CSymModule_SymType(SymNone)
    CSymModule_SymType(SymCoff)
    CSymModule_SymType(SymCv)
    CSymModule_SymType(SymPdb)
    CSymModule_SymType(SymExport)
    CSymModule_SymType(SymDeferred)
    CSymModule_SymType(SymSym)
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

STDMETHODIMP CSymModule::get_ImageName(BSTR* pbstr)
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

STDMETHODIMP CSymModule::get_LoadedImageName(BSTR* pbstr)
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

