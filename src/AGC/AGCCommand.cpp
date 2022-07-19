/////////////////////////////////////////////////////////////////////////////
// AGCCommand.cpp : Implementation of the CAGCCommand class.
//

#include "pch.h"
#include "AGCCommand.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCCommand

TC_OBJECT_EXTERN_IMPL(CAGCCommand)


/////////////////////////////////////////////////////////////////////////////
// IAGCCommand Interface Methods

STDMETHODIMP CAGCCommand::get_Target(BSTR* pbstrTarget)
{
  CLEAROUT(pbstrTarget, (BSTR)NULL);
  XLock lock(this);
  *pbstrTarget = m_bstrTarget.Copy();
  return S_OK;
}

STDMETHODIMP CAGCCommand::get_Verb(BSTR* pbstrVerb)
{
  CLEAROUT(pbstrVerb, (BSTR)NULL);
  XLock lock(this);
  *pbstrVerb = m_bstrVerb.Copy();
  return S_OK;
}

STDMETHODIMP CAGCCommand::get_Text(BSTR* pbstrText)
{
  CLEAROUT(pbstrText, (BSTR)NULL);
  XLock lock(this);
  int cch = m_bstrVerb.Length() + m_bstrTarget.Length() + 2;
  LPOLESTR psz = (LPOLESTR)_alloca(sizeof(OLECHAR) * cch);
  swprintf(psz, L"%ls %ls", LPOLESTR(m_bstrVerb), LPOLESTR(m_bstrTarget));
  *pbstrText = CComBSTR(psz).Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCCommandPrivate Interface Methods

STDMETHODIMP_(void) CAGCCommand::Init(LPCSTR pszTarget, LPCSTR pszVerb)
{
  XLock lock(this);
  m_bstrTarget = pszTarget;
  m_bstrVerb   = pszVerb;
  m_bDirty     = true;
}


/////////////////////////////////////////////////////////////////////////////
// IPersist Interface Methods

STDMETHODIMP CAGCCommand::GetClassID(CLSID* pClassID)
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

STDMETHODIMP CAGCCommand::IsDirty()
{
  // Return dirty flag
  XLock lock(this);
  return m_bDirty ? S_OK : S_FALSE;
}

STDMETHODIMP CAGCCommand::Load(LPSTREAM pStm)
{
  XLock lock(this);

  // Read each string from the stream
  m_bstrTarget.Empty();
  m_bstrVerb.Empty();
  RETURN_FAILED(m_bstrTarget.ReadFromStream(pStm));
  RETURN_FAILED(m_bstrVerb.ReadFromStream(pStm));

  // Set the dirty flag
  m_bDirty = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCCommand::Save(LPSTREAM pStm, BOOL fClearDirty)
{
  XLock lock(this);

  // Write each string to the stream
  RETURN_FAILED(m_bstrTarget.WriteToStream(pStm));
  RETURN_FAILED(m_bstrVerb.WriteToStream(pStm));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCCommand::GetSizeMax(ULARGE_INTEGER* pCbSize)
{
  XLock lock(this);
  return TCGetPersistStreamSize(GetUnknown(), pCbSize);
}

STDMETHODIMP CAGCCommand::InitNew( void)
{
  XLock lock(this);

  // Initialize the strings
  m_bstrTarget.Empty();
  m_bstrVerb.Empty();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistPropertyBag Interface Methods

STDMETHODIMP CAGCCommand::Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog)
{
  XLock lock(this);

  // Load each string
  VARIANT varTarget, varVerb;
  RETURN_FAILED(pPropBag->Read(L"Target", &varTarget, pErrorLog));
  RETURN_FAILED(pPropBag->Read(L"Verb"  , &varVerb  , pErrorLog));

  // Attach the variants to the strings
  // Note: Do NOT call VariantClear since ownership is given to the strings
  m_bstrTarget.Empty();
  m_bstrTarget.Attach(V_BSTR(&varTarget));
  m_bstrVerb.Empty();
  m_bstrVerb.Attach(V_BSTR(&varVerb));

  // Set the dirty flag
  m_bDirty = true;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCCommand::Save(IPropertyBag* pPropBag, BOOL fClearDirty, BOOL)
{
  XLock lock(this);

  // Save each string
  RETURN_FAILED(pPropBag->Write(L"Target", &CComVariant(m_bstrTarget)));
  RETURN_FAILED(pPropBag->Write(L"Verb"  , &CComVariant(m_bstrVerb  )));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}


