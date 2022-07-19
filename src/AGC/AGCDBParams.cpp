/////////////////////////////////////////////////////////////////////////////
// AGCDBParams.cpp : Implementation of the CAGCDBParams class.
//

#include "pch.h"
#include "AGCDBParams.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCDBParams

TC_OBJECT_EXTERN_IMPL(CAGCDBParams)


/////////////////////////////////////////////////////////////////////////////
// IAGCDBParams Interface Methods

STDMETHODIMP CAGCDBParams::put_ConnectionString(BSTR bstr)
{
  XLock lock(this);
  m_bstrConnectionString = bstr;
  m_bDirty = true;
  return S_OK;
}

STDMETHODIMP CAGCDBParams::get_ConnectionString(BSTR* pbstr)
{
  CLEAROUT(pbstr, (BSTR)NULL);
  XLock lock(this);
  *pbstr = m_bstrConnectionString.Copy();
  return S_OK;
}

STDMETHODIMP CAGCDBParams::put_TableName(BSTR bstr)
{
  XLock lock(this);
  m_bstrTableName = bstr;
  m_bDirty = true;
  return S_OK;
}

STDMETHODIMP CAGCDBParams::get_TableName(BSTR* pbstr)
{
  CLEAROUT(pbstr, (BSTR)NULL);
  XLock lock(this);
  *pbstr = m_bstrTableName.Copy();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersist Interface Methods

STDMETHODIMP CAGCDBParams::GetClassID(CLSID* pClassID)
{
  __try
  {
    *pClassID = GetObjectCLSID();
  }
  __except(1)
  {
    return E_POINTER;
  }
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistStreamInit Interface Methods

STDMETHODIMP CAGCDBParams::IsDirty()
{
  // Return dirty flag
  XLock lock(this);
  return m_bDirty ? S_OK : S_FALSE;
}

STDMETHODIMP CAGCDBParams::Load(LPSTREAM pStm)
{
  XLock lock(this);

  // Read each string from the stream
  m_bstrConnectionString.Empty();
  m_bstrTableName.Empty();
  RETURN_FAILED(m_bstrConnectionString.ReadFromStream(pStm));
  RETURN_FAILED(m_bstrTableName.ReadFromStream(pStm));

  // Set the dirty flag
  m_bDirty = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCDBParams::Save(LPSTREAM pStm, BOOL fClearDirty)
{
  XLock lock(this);

  // Write each string to the stream
  RETURN_FAILED(m_bstrConnectionString.WriteToStream(pStm));
  RETURN_FAILED(m_bstrTableName.WriteToStream(pStm));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCDBParams::GetSizeMax(ULARGE_INTEGER* pCbSize)
{
  XLock lock(this);
  return TCGetPersistStreamSize(GetUnknown(), pCbSize);
}

STDMETHODIMP CAGCDBParams::InitNew( void)
{
  XLock lock(this);

  // Initialize the strings
  m_bstrConnectionString.Empty();
  m_bstrTableName.Empty();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistPropertyBag Interface Methods

STDMETHODIMP CAGCDBParams::Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog)
{
  // Load each string
  VARIANT varConnectionString, varTableName;
  RETURN_FAILED(pPropBag->Read(L"ConnectionString", &varConnectionString, pErrorLog));
  RETURN_FAILED(pPropBag->Read(L"TableName"       , &varTableName       , pErrorLog));

  // Attach the variants to the strings
  // Note: Do NOT call VariantClear since ownership is given to the strings
  XLock lock(this);
  m_bstrConnectionString.Empty();
  m_bstrConnectionString.Attach(V_BSTR(&varConnectionString));
  m_bstrTableName.Empty();
  m_bstrTableName.Attach(V_BSTR(&varTableName));

  // Set the dirty flag
  m_bDirty = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCDBParams::Save(IPropertyBag* pPropBag, BOOL fClearDirty, BOOL)
{
  XLock lock(this);

  // Save each string
  RETURN_FAILED(pPropBag->Write(L"ConnectionString", &CComVariant(m_bstrConnectionString)));
  RETURN_FAILED(pPropBag->Write(L"TableName"       , &CComVariant(m_bstrTableName       )));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}


