/////////////////////////////////////////////////////////////////////////////
// TCPropBagOnRegKey.cpp | Implementation of the CTCPropBagOnRegKey
// class, which implements the CLSID_TCPropBagOnRegKey component object.
//

#include "pch.h"
#include "TCPropBagOnRegKey.h"

#include <..\TCAtl\ObjectMap.h>


/////////////////////////////////////////////////////////////////////////////
// CTCPropBagOnRegKey

TC_OBJECT_EXTERN_IMPL(CTCPropBagOnRegKey)


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

const _bstr_t CTCPropBagOnRegKey::m_szVariantType    (OLESTR(".vt"));
const _bstr_t CTCPropBagOnRegKey::m_szServer         (OLESTR(".server"));
const _bstr_t CTCPropBagOnRegKey::m_szProgID         (OLESTR(".progid"));
const _bstr_t CTCPropBagOnRegKey::m_szCLSID          (OLESTR(".clsid"));
const _bstr_t CTCPropBagOnRegKey::m_szLowerBound     (OLESTR(".lBound"));
const _bstr_t CTCPropBagOnRegKey::m_szElementCount   (OLESTR(".elements"));
const _bstr_t CTCPropBagOnRegKey::m_szElementFmt     (OLESTR("%d"));
const USHORT  CTCPropBagOnRegKey::m_wChangeTypeFlags =
  VARIANT_NOVALUEPROP | VARIANT_ALPHABOOL | VARIANT_LOCALBOOL;


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CTCPropBagOnRegKey::CTCPropBagOnRegKey() :
  m_pParent(NULL),
  m_bOwnKey(false)
{
}

void CTCPropBagOnRegKey::FinalRelease()
{
  ClosePrevious();
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

_bstr_t CTCPropBagOnRegKey::_GetSubkey()
{
  // Practice safe threading
  XLock lock(this);

  // Get the parent's subkey, if any
  _bstr_t str;
  if (m_pParent)
  {
    str = m_pParent->_GetSubkey();
    if (str.length())
      str += L"\\";
  }

  // Concatenate this object's subkey
  return str += m_strSubkey;
}

_bstr_t CTCPropBagOnRegKey::_GetServer(const _bstr_t& strName)
{
  // Open the subkey with the specified name
  CRegKey subkey;
  if (!strName.length())
    subkey = m_key;
  else if (!m_key.Exists(strName) || !subkey.Open(m_key, strName))
    return _bstr_t();

  // Read the Server of the object saved on the specified subkey
  _bstr_t strServer;
  if (!subkey._GetString(m_szServer, strServer))
    strServer.Empty();
  return strServer;
}

HRESULT CTCPropBagOnRegKey::_SetServer(const _bstr_t& strName,
  const _bstr_t& strServer)
{
  // Open the subkey with the specified name
  CRegKey subkey;
  if (!strName.length())
    subkey = m_key;
  else if (!m_key.Exists(strName) || !subkey.Open(m_key, strName))
    return HRESULT_FROM_WIN32(GetLastError());

  // Write the Server of the object saved on the specified subkey
  subkey.WriteString(m_szServer, strServer);

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_GetObjectCLSID(const _bstr_t& strName,
  CLSID& clsid)
{
  // Read the CLSID of the object saved on the specified subkey
  CRegKey subkey;
  _bstr_t strObjectID;
  return _GetObjectCLSID(strName, clsid, subkey, strObjectID, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CTCPropBagOnRegKey::_CreateObject(const _bstr_t& strName,
  IUnknown** ppunkObj, IErrorLog* pErrorLog, IUnknown* punkOuter)
{
  // Get the Server of the object saved on the specified subkey
  _bstr_t strServer(GetServer(strName));
  if (!strServer.length())
    return CreateLocalObject(strName, ppunkObj, pErrorLog, punkOuter);
  return CreateRemoteObject(strServer, strName, ppunkObj, pErrorLog);
}

HRESULT CTCPropBagOnRegKey::_CreateLocalObject(const _bstr_t& strName,
  IUnknown** ppunkObj, IErrorLog* pErrorLog, IUnknown* punkOuter)
{
  // Read the CLSID of the object saved on the specified subkey
  CLSID clsid;
  CRegKey subkey;
  _bstr_t strObjectID;
  HRESULT hr = GetObjectCLSID(strName, clsid, subkey, strObjectID,
    pErrorLog);
  if (FAILED(hr))
    return hr;

  // Create the object
  DWORD dwCtx = CLSCTX_ALL | CLSCTX_REMOTE_SERVER;
  IPersistPropertyBagPtr pppb;
  void** ppv = reinterpret_cast<void**>(&pppb);
  hr = CoCreateInstance(clsid, punkOuter, dwCtx, __uuidof(pppb), ppv);
  if (E_INVALIDARG == hr)
  {
    dwCtx &= ~CLSCTX_REMOTE_SERVER;
    hr = CoCreateInstance(clsid, punkOuter, dwCtx, __uuidof(pppb), ppv);
  }
  if (FAILED(hr))
  {
    // Use local resources
    MCLibRes res;

    // Format a description string
    _bstr_t strDesc;
    strDesc.Format(IDS_FMT_FAIL_CREATE_CLSID, strObjectID);

    // Log the error
    USES_CONVERSION;
    return LogError("CreateObject", strDesc, hr, T2COLE(strName), pErrorLog);
  }

  // Load the object's persistent properties
  CComObjectStack<CTCPropBagOnRegKey> bag;
  bag.Init(subkey, strName, this);
  hr = pppb->Load(&bag, pErrorLog);
  if (FAILED(hr))
    return hr;

  // Detach the IUnknown* and copy to the [out] parameter, don't Release it
  *ppunkObj = pppb.Detach();

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_CreateRemoteObject(const _bstr_t& strServer,
  const _bstr_t& strName, IUnknown** ppunkObj, IErrorLog* pErrorLog)
{
  // If DCOM is not installed, take appropriate action
  MCOle32_dll& libOle32 = GetOle32Lib();
  if (!libOle32.Exists_CoCreateInstanceEx())
    return !strServer.length() ?
      CreateLocalObject(strName, ppunkObj, pErrorLog) : E_UNEXPECTED;

  // Read the CLSID of the object saved on the specified subkey
  CLSID clsid;
  CRegKey subkey;
  _bstr_t strObjectID;
  HRESULT hr = GetObjectCLSID(strName, clsid, subkey, strObjectID,
    pErrorLog);
  if (FAILED(hr))
    return hr;

  // Create the object
  USES_CONVERSION;
  _bstr_t bstrServer(strServer);
  COSERVERINFO csi = {0, bstrServer, NULL, 0};
  MULTI_QI mqi = {&IID_IPersistPropertyBag, NULL, 0};
  DWORD dwCtx = CLSCTX_ALL | CLSCTX_REMOTE_SERVER;
  hr = libOle32.CoCreateInstanceEx(clsid, NULL, dwCtx, &csi, 1, &mqi);
  if (FAILED(hr))
  {
    // Use local resources
    MCLibRes res;

    // Format a description string
    _bstr_t strDesc;
    strDesc.Format(IDS_FMT_FAIL_CREATE_CLSID, strObjectID);

    // Log the error
    USES_CONVERSION;
    return LogError("CreateObject", strDesc, hr, T2COLE(strName), pErrorLog);
  }

  // Attach the interface pointer to the smart pointer
  IPersistPropertyBagPtr pppb((IPersistPropertyBag*)mqi.pItf, false);
  
  // Load the object's persistent properties
  CComObjectStack<CTCPropBagOnRegKey> bag;
  bag.Init(subkey, strName, this);
  hr = pppb->Load(&bag, pErrorLog);
  if (FAILED(hr))
    return hr;

  // Detach the IUnknown* and copy to the [out] parameter, don't Release it
  *ppunkObj = pppb.Detach();

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_LoadObject(const _bstr_t& strName,
  IUnknown* punkObj, IErrorLog* pErrorLog)
{
  // Validate the specified parameters
  if (IsBadReadPtr(punkObj))
    return E_POINTER;
  if (NULL != pErrorLog && IsBadReadPtr(pErrorLog))
    return E_POINTER;

  // Open the subkey with the specified name
  CRegKey subkey;
  if (!strName.length())
    subkey = m_key;
  else if (!m_key.Exists(strName) || !subkey.Open(m_key, strName))
    return HRESULT_FROM_WIN32(GetLastError());

  // QueryInterface for IPersistPropertyBag
  IPersistPropertyBagPtr pppb;
  HRESULT hr = punkObj->QueryInterface(__uuidof(pppb), (void**)&pppb);
  if (FAILED(hr))
    return hr;

  // Load the object's persistent properties
  CComObjectStack<CTCPropBagOnRegKey> bag;
  bag.Init(subkey, strName, this);
  hr = pppb->Load(&bag, pErrorLog);

  // Return the last HRESULT
  return hr;
}

HRESULT CTCPropBagOnRegKey::_SaveObject(const _bstr_t& strName,
  IUnknown* punkObj, BOOL bClearDirty, BOOL bSaveAllProperties)
{
  // Validate the specified parameters
  if (IsBadReadPtr(punkObj))
    return E_POINTER;

  // Create a subkey with the specified name
  CRegKey subkey;
  if (!strName.length())
    subkey = m_key;
  else
  {
    m_key.DeleteValue(strName);
    m_key.RecurseDeleteKey(strName);
    if (!subkey.Open(m_key, strName))
      return HRESULT_FROM_WIN32(GetLastError());
  }

  // QueryInterface for IPersistPropertyBag
  IPersistPropertyBagPtr pppb;
  HRESULT hr = punkObj->QueryInterface(__uuidof(pppb), (void**)&pppb);
  if (FAILED(hr))
    return hr;

  // Get the object's CLSID
  CLSID clsid;
  hr = pppb->GetClassID(&clsid);
  if (FAILED(hr))
    return hr;

  // Attempt first to convert the object's CLSID to a ProgID
  LPOLESTR pszProgID = NULL;
  if (SUCCEEDED(ProgIDFromCLSID(clsid, &pszProgID)))
  {
    subkey.WriteString(m_szProgID, pszProgID);
    CoTaskMemFree(pszProgID);
  }
  else
  {
    // Convert the object's CLSID to a string
    OLECHAR szClassID[64];
    StringFromGUID2(clsid, szClassID, sizeofArray(szClassID));
    subkey.WriteString(m_szCLSID, szClassID);
  }

  // Write the variant type value
  subkey.WriteDWord(m_szVariantType, DWORD(VarTypeFromUnknown(punkObj)));

  // Save the persistent properties of the object
  CComObjectStack<CTCPropBagOnRegKey> bag;
  bag.Init(subkey, strName, this);
  hr = pppb->Save(&bag, bClearDirty, bSaveAllProperties);

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

MCOle32_dll& CTCPropBagOnRegKey::_GetOle32Lib()
{
  static MCOle32_dll libOle32;
  return libOle32;
}

HRESULT CTCPropBagOnRegKey::_GetObjectCLSID(const _bstr_t& strName,
  CLSID& clsid, CRegKey& subkey, _bstr_t& strObjectID, IErrorLog* pErrorLog)
{
  USES_CONVERSION;

  // Clear the specified clsid, subkey, and strObjectID parameters
  clsid = CLSID_NULL;
  subkey.Close();
  strObjectID.Empty();

  // Open the subkey with the specified name
  if (!strName.length())
    subkey = m_key;
  else if (!m_key.Exists(strName) || !subkey.Open(m_key, strName))
    return HRESULT_FROM_WIN32(GetLastError());

  // Use local resources for string loading
  MCLibRes res;

  // Read the object's CLSID string value
  HRESULT hr;
  _bstr_t strDesc;
  if (subkey.GetString(m_szCLSID, strObjectID))
  {
    // Convert the object's CLSID string to a CLSID
    LPOLESTR pszObjectID = const_cast<LPOLESTR>(T2COLE(strObjectID));
    hr = CLSIDFromString(pszObjectID, &clsid);
    if (SUCCEEDED(hr))
      return hr;

    // Format an error description string   
    strDesc.Format(IDS_FMT_FAIL_CONVERT_CLSID, strObjectID);
  }
  else if (subkey.GetString(m_szProgID, strObjectID))
  {
    // Convert the object's ProgID string to a CLSID
    hr = CLSIDFromProgID(T2COLE(strObjectID), &clsid);
    if (SUCCEEDED(hr))
      return hr;

    // Format an error description string
    strDesc.Format(IDS_FMT_FAIL_CONVERT_CLSID, strObjectID);
  }
  else
  {
    // Save the last error
    hr = HRESULT_FROM_WIN32(GetLastError());

    // Format an error description string
    strDesc.LoadString(IDS_FAIL_READ_CLSID);
  }

  // Log the error and return
  return LogError("GetObjectCLSID", strDesc, hr, T2COLE(strName), pErrorLog);
}

HRESULT CTCPropBagOnRegKey::_ReadVariantFromValue(CRegKey& key,
  const _bstr_t& strPropName, DWORD nType, int cbData, CComVariant& v,
  IErrorLog* pErrorLog)
{
  // Read the value from the registry based on the value type
  switch (nType)
  {
    case REG_DWORD:
    {
      key.GetDWord(strPropName, (DWORD&)V_UI4(&v));
      v.vt = VT_UI4;
      break;
    }

    case REG_SZ:
    {
      _bstr_t strValue;
      key.GetString(strPropName, strValue);
      v = strValue;
      break;
    }

    case REG_BINARY:
    {
      BYTE* pData = new BYTE[cbData];
      key.GetBinary(strPropName, pData, cbData);
      CMemFile file(pData, cbData);
      CArchive ar(&file, CArchive::load, 0);
      ar >> v;
      ar.Close();
      delete [] file.Detach();
      break;
    }

    default:
    {
      // Use local resources
      MCLibRes res;

      // Format a description string
      _bstr_t strDesc;
      strDesc.Format(IDS_FMT_UNSUPPORTED_REGTYPE, nType);

      // Log the error
      USES_CONVERSION;
      return LogError("ReadVariantFromValue", strDesc, E_FAIL,
        T2COLE(strPropName), pErrorLog);
    }
  }

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_LogError(const _bstr_t& strFn,
  const _bstr_t& strDesc, HRESULT hr, LPCOLESTR pszPropName,
  IErrorLog* pErrorLog)
{
  // Create the source string
  _bstr_t strSrc, strSubkey(GetSubkey());
  if (!strSubkey.length())
    strSrc.Format(_T("CTCPropBagOnRegKey::_%s(\"%ls\")"), strFn,
      pszPropName);
  else
    strSrc.Format(_T("CTCPropBagOnRegKey::_%s(\"%s\\%ls\")"), strFn,
      strSubkey, pszPropName);

  // Output a trace message under _DEBUG builds
  TRACE2("%s: %s\n", strSrc, strDesc);

  // Log an error, if an IErrorLog interface is specified
  if (NULL != pErrorLog)
  {
    // Allocate BSTR's from the formatted strings
    BSTR bstrSrc = strSrc.AllocSysString();
    BSTR bstrDesc = strDesc.AllocSysString();

    // Create an EXCEPINFO structure and log the error
    EXCEPINFO ei = {0, 0, bstrSrc, bstrDesc, NULL, 0, NULL, NULL, hr};
    HRESULT hrAdd = pErrorLog->AddError(pszPropName, &ei);

    // Free the allocated BSTR's
    SysFreeString(bstrDesc);
    SysFreeString(bstrSrc);

    // Handle an unsuccessful IErrorLog::AddError call
    if (FAILED(hrAdd))
    {
      TRACE1("%s: pErrorLog->AddError() failed!\n", strSrc);
      AfxThrowOleException(hrAdd);
    }
  }

  // Indicate failure
  return hr;
}

VARTYPE CTCPropBagOnRegKey::_GetSubkeyVarType(CRegKey& key,
  const _bstr_t& strPropName)
{
  ASSERT(key.Exists(strPropName));

  // Open the subkey with the specified name
  CRegKey subkey;
  if (!subkey.Open(key, strPropName))
    return VT_EMPTY;

  // Read the subkey's variant type value
  DWORD dwVT;
  if (!subkey.GetDWord(m_szVariantType, dwVT))
    return VT_EMPTY;

  // Return the subkey's variant type value
  return VARTYPE(dwVT);
}

HRESULT CTCPropBagOnRegKey::_ReadSafeArray(CRegKey& key,
  const _bstr_t& strPropName, VARIANT* pVar, IErrorLog* pErrorLog)
{
  // Open the subkey with the specified name
  CRegKey subkey;
  VERIFY(subkey.Open(key, strPropName));

  // Read the variant type of the registry value
  VARTYPE vt = GetSubkeyVarType(key, strPropName);
  ASSERT(vt & VT_ARRAY);

  // Remove the VT_ARRAY bit flag from the variant type
  vt &= ~VT_ARRAY;

  // Check for supported variant types
  switch (vt)
  {
    case VT_BOOL:
    case VT_I1:
    case VT_I2:
    case VT_I4:
    case VT_UI1:
    case VT_UI2:
    case VT_UI4:
    case VT_ERROR:
    case VT_R4:
    case VT_R8:
    case VT_DECIMAL:
    case VT_CY:
    case VT_DATE:
    case VT_BSTR:
    case VT_UNKNOWN:
    case VT_DISPATCH:
    case VT_VARIANT:
      break;
    default:
    {
      // Use local resources
      MCLibRes res;

      // Format a description string
      _bstr_t strDesc;
      strDesc.Format(IDS_FMT_UNSUP_ARRAY_VARTYPE, strPropName, vt, vt);

      // Log the error
      USES_CONVERSION;
      return LogError("ReadSafeArray", strDesc, E_UNEXPECTED,
        T2COLE(strPropName), pErrorLog);
    }
  }

  // Read the element count of the safe array
  DWORD dwElements = 0;
  if (!subkey.GetDWord(m_szElementCount, dwElements))
  {
    // Use local resources
    MCLibRes res;

    // Format a description string
    _bstr_t strDesc;
    strDesc.Format(IDS_FMT_VALUE_NOT_EXIST, m_szElementCount, strPropName);

    // Log the error
    USES_CONVERSION;
    return LogError("ReadSafeArray", strDesc, E_UNEXPECTED,
      T2COLE(strPropName), pErrorLog);
  }

  // Read the lower bound of the safe array, defaults to zero
  LONG lLBound = 0;
  subkey.GetDWord(m_szLowerBound, (DWORD&)lLBound);

  // Read each array element into a temporary array
  _bstr_t strText;
  CComVariant var;
  CArray<CComVariant, CComVariant&> arrayTemp;
  for (DWORD i = 0; i < dwElements; i++)
  {
    // Prepare the variant
    V_VT(&var) = vt;

    // Format the value name
    strText.Format(m_szElementFmt, i);

    // Read the variant
    HRESULT hr = ReadVariant(subkey, strText, var, pErrorLog);
    if (FAILED(hr))
      return hr;

    // Add the variant to the temporary array
    arrayTemp.Add(var);
  }

  // Create a safe array and copy the temporary array elements
  SAFEARRAY* psa = SafeArrayCreateVector(vt, lLBound, dwElements);
  if (NULL == psa)
  {
    // Use local resources
    MCLibRes res;

    // Format a description string
    _bstr_t strDesc(LPCSTR(IDS_FAIL_SAFEARRAY_CREATE));

    // Log the error
    USES_CONVERSION;
    return LogError("ReadSafeArray", strDesc, E_OUTOFMEMORY,
      T2COLE(strPropName), pErrorLog);
  }

  // Copy the temporary array elements into the safe array
  for (long iElement = 0; iElement < arrayTemp.GetSize(); iElement++)
  {
    CComVariant& v = arrayTemp[iElement];
    void* pvData;
    switch (vt)
    {
      case VT_VARIANT:
        pvData = &v;
        break;
      case VT_UNKNOWN:
      case VT_DISPATCH:
      case VT_BSTR:
        pvData = V_BSTR(&v);
        break;
      default:
        pvData = &V_NONE(&v);
    }

    HRESULT hr = SafeArrayPutElement(psa, &iElement, pvData);
    if (FAILED(hr))
    {
      // Use local resources
      MCLibRes res;

      // Format a description string
      _bstr_t strDesc(LPCSTR(IDS_FAIL_SAFEARRAY_PUT));

      // Log the error
      USES_CONVERSION;
      return LogError("ReadSafeArray", strDesc, hr, T2COLE(strPropName),
        pErrorLog);
    }
  }

  // Put the safe array into the specified variant
  V_VT(pVar) = vt | VT_ARRAY;
  V_ARRAY(pVar) = psa;

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_WriteSafeArray(CRegKey& key,
  const _bstr_t& strPropName, VARIANT* pVar)
{
  ASSERT(V_ISARRAY(pVar));
  ASSERT(lstrlen(strPropName));

  // Get the SAFEARRAY pointer from the variant
  SAFEARRAY* psa = V_ARRAY(pVar);
  if (IsBadReadPtr(psa))
    return E_POINTER;

  // Only support 1-dimensional arrays (currently)
  if (1 != SafeArrayGetDim(psa))
    return E_INVALIDARG;

  // Get the element size of the safe array
  UINT cbElement = SafeArrayGetElemsize(psa);

  // Get the safe array type from the variant
  VARTYPE vt = V_VT(pVar) & ~VT_ARRAY;

  // Check for supported types and validate the element size
  switch (vt)
  {
    case VT_BOOL:
      if (sizeof(V_BOOL(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_I1:
      if (sizeof(V_I1(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_I2:
      if (sizeof(V_I2(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_I4:
      if (sizeof(V_I4(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_UI1:
      if (sizeof(V_UI1(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_UI2:
      if (sizeof(V_UI2(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_UI4:
      if (sizeof(V_UI4(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_ERROR:
      if (sizeof(V_ERROR(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_R4:
      if (sizeof(V_R4(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_R8:
      if (sizeof(V_R8(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_DECIMAL:
      if (sizeof(V_DECIMAL(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_CY:
      if (sizeof(V_CY(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_DATE:
      if (sizeof(V_DATE(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_BSTR:
      if (sizeof(V_BSTR(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_UNKNOWN:
      if (sizeof(V_UNKNOWN(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_DISPATCH:
      if (sizeof(V_DISPATCH(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    case VT_VARIANT:
      if (sizeof(V_VARIANTREF(pVar)) != cbElement)
        return E_UNEXPECTED;
      break;
    default:
      return E_UNEXPECTED;
  }

  // Get the upper and lower bounds of the safe array
  HRESULT hr;
  LONG lUBound = 0, lLBound = 0;
  if (FAILED(hr = SafeArrayGetUBound(psa, 1, &lUBound)))
    return hr;
  if (FAILED(hr = SafeArrayGetLBound(psa, 1, &lLBound)))
    return hr;
  UINT nElements = lUBound - lLBound + 1;

  // Create a subkey with the specified name
  key.DeleteValue(strPropName);
  key.RecurseDeleteKey(strPropName);
  CRegKey subkey;
  if (!subkey.Open(key, strPropName))
    return HRESULT_FROM_WIN32(GetLastError());

  // Get access to the safe array data
  BYTE* pElement = NULL;
  if (FAILED(hr = SafeArrayAccessData(psa, (void**)&pElement)))
    return hr;

  // Write the variant type value
  subkey.WriteDWord(m_szVariantType, DWORD(V_VT(pVar)));

  // Write the element count value
  subkey.WriteDWord(m_szElementCount, DWORD(nElements));

  // Write the lower bound value, if not 0
  if (lLBound)
    subkey.WriteDWord(m_szLowerBound, DWORD(lLBound));

  // Special handling for arrays of variants 
  _bstr_t strText;
  if (VT_VARIANT == vt)
  {
    // Write each variant array element to the registry
    for (UINT i = 0; i < nElements; i++, pElement += cbElement)
    {
      // Format the value name
      strText.Format(m_szElementFmt, i);

      // Write the variant array element to the registry subkey
      if (FAILED(hr = WriteVariant(subkey, strText, (VARIANT*)pElement)))
      {
        TRACE1("CTCPropBagOnRegKey::_WriteSafeArray(\"%s\", pVar): ",
          strPropName);
        TRACE2("WriteVariant(subkey, \"%s\", &var) returned 0x%08X\n",
          strText, hr);
      }
    }
  }
  else
  {
    // Write each array element to the registry
    VARIANT var;
    V_VT(&var) = vt;
    for (UINT i = 0; i < nElements; i++, pElement += cbElement)
    {
      // Copy the array element to the data portion of the VARIANT
      memcpy(&V_NONE(&var), pElement, cbElement);

      // Format the value name
      strText.Format(m_szElementFmt, i);

      // Write the variant to the registry subkey
      if (FAILED(hr = WriteVariant(subkey, strText, &var)))
      {
        TRACE1("CTCPropBagOnRegKey::_WriteSafeArray(\"%s\", pVar): ",
          strPropName);
        TRACE2("WriteVariant(subkey, \"%s\", &var) returned 0x%08X\n",
          strText, hr);
      }
    }
  }

  // Release access to the safe array data
  VERIFY(SUCCEEDED(SafeArrayUnaccessData(psa)));

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_ReadVariant(CRegKey& key,
  const _bstr_t& strValueName, VARIANT* pVar, IErrorLog* pErrorLog)
{
  // Create an empty CComVariant object
  CComVariant v;

  // Read the specified property from the registry
  DWORD nType = 0;
  int cbData = 0;
  if (key.QueryValue(strValueName, nType, cbData))  // Name is a value?
  {
    // Read property in registry value as a VARIANT
    HRESULT hr = _ReadVariantFromValue(key, strValueName, nType, cbData, v,
      pErrorLog);
    if (FAILED(hr))
      return hr;
  }
  else if (key.Exists(strValueName))                // Name is a subkey?
  {
    // Read property on subkey as specified by subkey's VariantType value
    HRESULT hr = E_FAIL;
    if (IsSubkeySafeArray(key, strValueName))
      hr = ReadSafeArray(key, strValueName, pVar, pErrorLog);
    else if (IsSubkeyObject(key, strValueName))
    {
      // Read property on subkey as an object
      CComObjectStack<CTCPropBagOnRegKey> bag;
      bag.Init(key, _bstr_t(), this);
      V_VT(&v) = VT_UNKNOWN;
      hr = bag._CreateObject(strValueName, &V_UNKNOWN(&v), pErrorLog);
    }
    if (FAILED(hr))
      return hr;
  }
  else
  {
    // Specified name does not exist
    TRACE1("CTCPropBagOnRegKey::_ReadVariant: Property \"%s\" does not exist\n",
      strValueName);
    return E_INVALIDARG;
  }

  // Coerce the value to the specified variant type
  switch (V_VT(pVar))
  {
    // Leave value as-is if no VARIANT type was specified
    case VT_EMPTY:
      VariantCopy(pVar, v);
      break;

    // Coerce supported VARIANT types
    case VT_BOOL:
    case VT_I1:
    case VT_I2:
    case VT_I4:
    case VT_UI1:
    case VT_UI2:
    case VT_UI4:
    case VT_ERROR:
    case VT_R4:
    case VT_R8:
    case VT_DECIMAL:
    case VT_CY:
    case VT_DATE:
    case VT_BSTR:
    case VT_UNKNOWN:
    case VT_DISPATCH:
    case VT_ARRAY | VT_BOOL:
    case VT_ARRAY | VT_I1:
    case VT_ARRAY | VT_I2:
    case VT_ARRAY | VT_I4:
    case VT_ARRAY | VT_UI1:
    case VT_ARRAY | VT_UI2:
    case VT_ARRAY | VT_UI4:
    case VT_ARRAY | VT_ERROR:
    case VT_ARRAY | VT_R4:
    case VT_ARRAY | VT_R8:
    case VT_ARRAY | VT_DECIMAL:
    case VT_ARRAY | VT_CY:
    case VT_ARRAY | VT_DATE:
    case VT_ARRAY | VT_BSTR:
    case VT_ARRAY | VT_UNKNOWN:
    case VT_ARRAY | VT_DISPATCH:
    case VT_ARRAY | VT_VARIANT:
    {
      // Coerce the value to the specified VARIANT type
      VariantChangeTypeEx(pVar, v, GetThreadLocale(), m_wChangeTypeFlags,
        V_VT(pVar));
      break;
    }

    // All other VARIANT types are unsupported
    default:
    {
      // Use local resources
      MCLibRes res;

      // Format a description string
      _bstr_t strDesc;
      strDesc.Format(IDS_FMT_UNSUPPORTED_VT, UINT(V_VT(pVar)),
        UINT(V_VT(pVar)));

      // Log the error
      USES_CONVERSION;
      return LogError("ReadVariant", strDesc, E_FAIL, T2COLE(strValueName),
        pErrorLog);
    }
  }

  // Indicate success
  return S_OK;
}

HRESULT CTCPropBagOnRegKey::_WriteVariant(CRegKey& key,
  const _bstr_t& strValueName, VARIANT* pVar)
{
  // Check for an array
  if (V_ISARRAY(pVar))
    return WriteSafeArray(key, strValueName, pVar);

  // Write the value to the registry based on the VARIANT type
  switch (V_VT(pVar))
  {
    // Empty variant is written as nothing
    case VT_EMPTY:
      key.DeleteValue(strValueName);
      key.RecurseDeleteKey(strValueName);
      break;

    // Integer types are written as a REG_DWORD value
    case VT_I1:
    case VT_I2:
    case VT_I4:
    case VT_UI1:
    case VT_UI2:
    case VT_ERROR:
    {
      // Coerce the value to a VT_UI4
      VariantChangeTypeEx(pVar, pVar, GetThreadLocale(), m_wChangeTypeFlags,
        VT_UI4);
      // Fall thru to next case
    }
    case VT_UI4:
    {
      // Write the REG_DWORD value to the registry
      key.RecurseDeleteKey(strValueName);
      key.WriteDWord(strValueName, V_UI4(pVar));
      break;
    }

    // BOOL's, float types and strings are written as a REG_SZ value
    case VT_R4:
    case VT_R8:
    case VT_CY:
    case VT_DATE:
    case VT_DECIMAL:
    case VT_BOOL:
    {
      // Coerce the value to a VT_BSTR
      VariantChangeTypeEx(pVar, pVar, GetThreadLocale(), m_wChangeTypeFlags,
        VT_BSTR);
      // Fall thru to next case
    }
    case VT_BSTR:
    {
      // Write the REG_SZ value to the registry
      key.RecurseDeleteKey(strValueName);
      key.WriteString(strValueName, V_BSTR(pVar));
      break;
    }

    // Objects written as REG_BINARY, if they don't support IPersistPropertyBag
    case VT_UNKNOWN:
    case VT_DISPATCH:
    {
      // Attempt first to save the object property using IPersistPropertyBag
      key.DeleteValue(strValueName);
      key.RecurseDeleteKey(strValueName);

      CComObjectStack<CTCPropBagOnRegKey> bag;
      bag.Init(key, _bstr_t(), this);
      HRESULT hr = bag.SaveObject(strValueName, V_UNKNOWN(pVar), FALSE, TRUE);
      if (FAILED(hr))
      {
        TRACE1("CTCPropBagOnRegKey::_WriteVariant: Saving object property \"%s\" as a binary value\n",
          strValueName);

        // Create a CArchive on a CMemFile
        CMemFile file;
        CArchive ar(&file, CArchive::store, 0);

        // Archive the variant to the CArchive and close it
        CComVariant v(pVar);
        ar << v;
        ar.Close();

        // Write the REG_BINARY value to the registry
        int cbData = file.GetLength();
        BYTE* pData = file.Detach();
        key.RecurseDeleteKey(strValueName);
        key.WriteBinary(strValueName, pData, cbData);
        file.Attach(pData, cbData);
      }
      break;
    }

    default:
      TRACE1("CTCPropBagOnRegKey::_WriteVariant(\"%ls\"): ", strValueName);
      TRACE2("Unsupported variant type 0x%02X (%d)\n", UINT(V_VT(pVar)),
        UINT(V_VT(pVar)));
      return E_FAIL;
  }

  // Indicate success
  return S_OK;
}

VARTYPE CTCPropBagOnRegKey::_VarTypeFromUnknown(IUnknown* punk)
{
  try
  {
    VARTYPE vt = VT_UNKNOWN;
    IDispatch* pdisp = NULL;
    if (SUCCEEDED(punk->QueryInterface(IID_IDispatch, (void**)&pdisp)))
    {
      if (pdisp == punk)
        vt = VT_DISPATCH;
      pdisp->Release();
    }
    return vt;
  }
  catch (...)
  {
    return VT_EMPTY;
  }
}

HKEY CTCPropBagOnRegKey::RootKeyFromString(BSTR bstrRegKey, DWORD* cchEaten)
{
  // Define a static lookup table
  struct XRootKeys
  {
    LPCOLESTR pszName;
    DWORD     cchName;
    HKEY      hkey;
  };
  const static XRootKeys s_table[] =
  {
    {L"HKCR\\"                 ,  5, HKEY_CLASSES_ROOT    },
    {L"HKCU\\"                 ,  5, HKEY_CURRENT_USER    },
    {L"HKLM\\"                 ,  5, HKEY_LOCAL_MACHINE   },
    {L"HKCC\\"                 ,  5, HKEY_CURRENT_CONFIG  },
    {L"HKPD\\"                 ,  5, HKEY_PERFORMANCE_DATA},
    {L"HKDD\\"                 ,  5, HKEY_DYN_DATA        },

    {L"HKEY_CLASSES_ROOT\\"    , 18, HKEY_CLASSES_ROOT    },
    {L"HKEY_CURRENT_USER\\"    , 18, HKEY_CURRENT_USER    },
    {L"HKEY_LOCAL_MACHINE\\"   , 19, HKEY_LOCAL_MACHINE   },
    {L"HKEY_CURRENT_CONFIG\\"  , 20, HKEY_CURRENT_CONFIG  },
    {L"HKEY_PERFORMANCE_DATA\\", 22, HKEY_PERFORMANCE_DATA},
    {L"HKEY_DYN_DATA\\"        , 14, HKEY_DYN_DATA        },
  };
  const static long s_cEntries = sizeofArray(s_table);

  // Search for an allowed root key name
  if (BSTRLen(bstrRegKey))
  {
    for (long i = 0; i < s_cEntries; ++i)
    {
      if (!wcsnicmp(s_table[i].pszName, bstrRegKey, s_table[i].cchName))
      {
        *cchName = s_table[i].cchName;
        return s_table[i].hkey;
      }
    }
  }

  // Could not find the specified string
  *cchName = 0;
  return NULL;
}


LONG CTCPropBagOnRegKey::KeyExists(HKEY hkey, const _bstr_t& strKeyName)
{
	HKEY hkeyTemp;
	LONG lr = IsWinNT() ?
    RegOpenKeyExW(hkey, strKeyName, 0, KEY_READ, &hkeyTemp) :
    RegOpenKeyExA(hkey, strKeyName, 0, KEY_READ, &hkeyTemp);
	if (ERROR_SUCCESS == lr)
  {
		RegCloseKey(hkeyTemp);
    return true;
  }
	return false;
}


LONG CTCPropBagOnRegKey::QueryString(HKEY hkey, const _bstr_t& strValueName,
  _bstr_t& strOut)
{
  bool bIsWinNT = IsWinNT();
  DWORD cbData;
  LONG lr = bIsWinNT ?
    RegQueryValueExW(hkey, strValueName, NULL, NULL, NULL, &cbData) :
    RegQueryValueExA(hkey, strValueName, NULL, NULL, NULL, &cbData);

  // Allocate a buffer
  LP
}


/////////////////////////////////////////////////////////////////////////////
// ITCPropBagOnRegKey Interface Methods

STDMETHODIMP CTCPropBagOnRegKey::CreateKey(BSTR bstrRegKey,
  VARIANT_BOOL bReadOnly)
{
  // Interpret the first part of the specified registry key string
  DWORD cchEaten;
  HKEY hkeyRoot = RootKeyFromString(bstrRegKey, &cchEaten);
  if (!hkeyRoot)
  {
    ZError("CTCPropBagOnRegKey::CreateKey(): Invalid registry key name specified.");
    return E_INVALIDARG;
  }

  // Increment the string past the root key string
  bstrRegKey += cchEaten;

  // Determine the specified access permission
  REGSAM regsam = bReadOnly ? KEY_READ : KEY_ALL_ACCESS;

  // Attempt to create/open the specified subkey of the specified key
  long lr;
  HKEY hkeyOpen = NULL;
  DWORD dw;
  if (IsWinNT())  // Take advantage of Unicode BSTR's under WinNT
  {
    lr = RegCreateKeyExW(hkeyRoot, bstrRegKey, 0, REG_NONE,
      REG_OPTION_NON_VOLATILE, regsam, NULL, &hkeyOpen, &dw);
  }
  else
  {
    USES_CONVERSION;
    lr = RegCreateKeyEx(hkeyRoot, OLE2CT(bstrRegKey), 0, REG_NONE,
      REG_OPTION_NON_VOLATILE, regsam, NULL, &hkeyOpen, &dw);
  }
  if (ERROR_SUCCESS != lr)
    return HRESULT_FROM_WIN32(lr);

  // Save the new open registry key
  XLock lock(this);
  ClosePrevious();
  m_key.Attach(hkeyOpen);
  m_bOwnKey = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCPropBagOnRegKey::OpenKey(BSTR bstrRegKey,
  VARIANT_BOOL bReadOnly)
{
  // Interpret the first part of the specified registry key string
  DWORD cchEaten;
  HKEY hkeyRoot = RootKeyFromString(bstrRegKey, &cchEaten);
  if (!hkeyRoot)
  {
    ZError("CTCPropBagOnRegKey::CreateKey(): Invalid registry key name specified.");
    return E_INVALIDARG;
  }

  // Increment the string past the root key string
  bstrRegKey += cchEaten;

  // Determine the specified access permission
  REGSAM regsam = bReadOnly ? KEY_READ : KEY_ALL_ACCESS;

  // Attempt to create/open the specified subkey of the specified key
  long lr;
  HKEY hkeyOpen = NULL;
  DWORD dw;
  if (IsWinNT())  // Take advantage of Unicode BSTR's under WinNT
  {
    lr = RegOpenKeyExW(hkeyRoot, bstrRegKey, 0, regsam, &hkeyOpen);
  }
  else
  {
    USES_CONVERSION;
    lr = RegOpenKeyEx(hkeyRoot, OLE2CT(bstrRegKey), 0, regsam, &hkeyOpen);
  }
  if (ERROR_SUCCESS != lr)
    return HRESULT_FROM_WIN32(lr);

  // Save the new open registry key
  XLock lock(this);
  ClosePrevious();
  m_key.Attach(hkeyOpen);
  m_bOwnKey = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCPropBagOnRegKey::CreateObject(IUnknown** ppunkObj)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->CreateObject(_bstr_t(), ppunkObj);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::CreateObject()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::CreateLocalObject(IUnknown** ppunkObj)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->CreateLocalObject(_bstr_t(), ppunkObj);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::CreateLocalObject()",
    RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::CreateRemoteObject(BSTR bstrServer,
  IUnknown** ppunkObj)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->CreateRemoteObject(_bstr_t(bstrServer), _bstr_t(),
      ppunkObj);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::CreateRemoteObject()",
    RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::LoadObject(IUnknown* punkObj)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->LoadObject(_bstr_t(), punkObj);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::LoadObject()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::SaveObject(IUnknown* punkObj,
  VARIANT_BOOL bClearDirty, VARIANT_BOOL bSaveAllProperties)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->SaveObject(_bstr_t(), punkObj);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::SaveObject()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::put_Server(BSTR bstrServer)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return m_pBag->SetServer(_bstr_t(), _bstr_t(bstrServer));
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::put_Server()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::get_Server(BSTR* pbstrServer)
{
  try
  {
    // Initialize the [out] parameter
    CLEAROUT(pbstrServer, (BSTR)NULL);

    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    *pbstrServer = m_pBag->GetServer(_bstr_t()).AllocSysString();

    // Indicate success
    return S_OK;
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::get_Server()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::get_ObjectCLSID(BSTR* pbstrCLSID)
{
  try
  {
    // Initialize the [out] parameter
    CLEAROUT(pbstrCLSID, (BSTR)NULL);

    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    CLSID clsid;
    RETURN_FAILED(m_pBag->GetObjectCLSID(_bstr_t(), clsid));

    // Convert the CLSID to a string
    *pbstrCLSID = BSTRFromGUID(clsid).copy();

    // Indicate success
    return S_OK;
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::get_ObjectCLSID()",
    RPC_E_SERVERFAULT)
}


/////////////////////////////////////////////////////////////////////////////
// IPropertyBag Interface Methods

STDMETHODIMP CTCPropBagOnRegKey::Read(LPCOLESTR pszPropName,
  VARIANT* pVar, IErrorLog* pErrorLog)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return ((IPropertyBag*)m_pBag)->Read(pszPropName, pVar, pErrorLog);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::Read()", RPC_E_SERVERFAULT)
}

STDMETHODIMP CTCPropBagOnRegKey::Write(LPCOLESTR pszPropName,
  VARIANT* pVar)
{
  try
  {
    // Lock the object
    XLock lock(this);

    // Ensure that an internal property bag object has been created
    if (!m_pBag)
      return E_UNEXPECTED;

    // Forward the call to the internal property bag object
    return ((IPropertyBag*)m_pBag)->Write(pszPropName, pVar);
  }
  TC_CATCH_ALL("CTCPropBagOnRegKey::Write()", RPC_E_SERVERFAULT)
}

