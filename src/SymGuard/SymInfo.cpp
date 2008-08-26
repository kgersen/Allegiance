/////////////////////////////////////////////////////////////////////////////
// SymInfo.cpp : Implementation of the CSymInfo class.

#include "pch.h"
#include "SymGuard.h"
#include "SymInfo.h"


/////////////////////////////////////////////////////////////////////////////
// CSymInfo

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CSymInfo)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CSymInfo::CSymInfo() :
  m_dwAddress(0),
  m_dwDisplacement(0)
{
  ZeroMemory(&m_il, sizeof(m_il));
}

HRESULT CSymInfo::FinalConstruct()
{
  debugf("CSymInfo::FinalConstruct(this=%08X)\n", this);

  // Create an instance of the Free-Threaded Marshaler
  RETURN_FAILED(::CoCreateFreeThreadedMarshaler(GetControllingUnknown(),
    &m_spUnkMarshaler.p));

  // Indicate success
  return S_OK;
}

void CSymInfo::FinalRelease()
{
  // Release the Free-Threaded Marshaler
  m_spUnkMarshaler = NULL;

  debugf("CSymInfo::FinalRelease(this=%08X)\n", this);
}

HRESULT CSymInfo::Init(HANDLE hProcessSym, DWORD dwAddr, ISymModule* pModule)
{
  // Lookup the symbol information for the specified address
  DWORD cbMaxNameLength = _MAX_PATH * 2;
  DWORD cb = cbMaxNameLength + sizeof(IMAGEHLP_SYMBOL);
  IMAGEHLP_SYMBOL* pSym = (IMAGEHLP_SYMBOL*)_alloca(cb);
  ZeroMemory(pSym, cb);
  pSym->SizeOfStruct  = sizeof(*pSym);
  pSym->MaxNameLength = cbMaxNameLength;
  if (!::SymGetSymFromAddr(hProcessSym, dwAddr, &m_dwDisplacement, pSym))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Save the specified address
  m_dwAddress = dwAddr;

  // Save the decorated name
  m_bstrDecoratedName = pSym->Name;

  // Get the undecorated name
  char szUn[8 * 1024];
  if (::SymUnDName(pSym, szUn, sizeofArray(szUn)))
    m_bstrName = szUn;

  // Get the line number information
  m_il.SizeOfStruct = sizeof(m_il);
  DWORD dwDisplacement = 0;
  if (::SymGetLineFromAddr(hProcessSym, dwAddr, &dwDisplacement, &m_il))
  {
    assert(dwDisplacement == (m_dwAddress - m_il.Address));

    // Get the BuildPathBase and SourcePathBase properties from the caller
    CComBSTR bstrBuildPathBase, bstrSourcePathBase;
    ZSucceeded(pModule->get_BuildPathBase(&bstrBuildPathBase));
    ZSucceeded(pModule->get_SourcePathBase(&bstrSourcePathBase));

    // Append a back-whack if SourcePathBase is non-empty and doesn't have one
    if (bstrSourcePathBase.Length())
      if (OLESTR('\\') != bstrSourcePathBase[bstrSourcePathBase.Length() - 1])
        bstrSourcePathBase += OLESTR("\\");

    // Resolve the relative path
    USES_CONVERSION;
    LPCTSTR pszFrom = bstrBuildPathBase.Length() ? OLE2CT(bstrBuildPathBase) : NULL;
    LPCTSTR pszPath = m_il.FileName              ?   A2CT(m_il.FileName)     : NULL;
    int     cchDest = _tcslen(pszPath) + _MAX_PATH;
    LPTSTR  pszDest = (LPTSTR)_alloca(cchDest * sizeof(TCHAR));
    m_bstrSourceFileName = bstrSourcePathBase;
    if (TCMakeRelativePath(pszPath, pszFrom, pszDest, cchDest, false))
      m_bstrSourceFileName += pszDest;
    else
      m_bstrSourceFileName += pszPath;
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CSymInfo::InterfaceSupportsErrorInfo(REFIID riid)
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
// ISymInfo Interface Methods

STDMETHODIMP CSymInfo::get_Address(VARIANT* pvar)
{
  // Initialize the [out] parameter
  INIT_VARIANT_OUT(pvar);

  // Copy the value
  V_VT(pvar) = VT_UI4;
  V_UI4(pvar) = m_dwAddress;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_Name(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  *pbstr = m_bstrName.Copy();
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_DecoratedName(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  *pbstr = m_bstrDecoratedName.Copy();
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_SourceFileName(BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Create a BSTR
  *pbstr = m_bstrSourceFileName.Copy();
  if (NULL == *pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_LineNumber(long* pn)
{
  // Initialize the [out] parameter
  CLEAROUT(pn, static_cast<long>(m_il.LineNumber));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_BytesFromSymbol(long* pcb)
{
  // Initialize the [out] parameter
  CLEAROUT(pcb, static_cast<long>(m_dwDisplacement));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CSymInfo::get_BytesFromLine(long* pcb)
{
  // Initialize the [out] parameter
  CLEAROUT(pcb, static_cast<long>(m_dwAddress - m_il.Address));

  // Indicate success
  return S_OK;
}

 