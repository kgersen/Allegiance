/////////////////////////////////////////////////////////////////////////////
// TCMarshalByValue.cpp | Implementation of the CTCMarshalByValue class,
// which implements the CLSID_TCMarshalByValue component object.<nl><nl>
// This object can be used as a COM aggregate that implements IMarshal in
// terms of the outer object's IPersistStream or IPersistStreamInit
// implementation.
//

#include "pch.h"
#include "TCMarshalByValue.h"


/////////////////////////////////////////////////////////////////////////////
// CTCMarshalByValue

TC_OBJECT_EXTERN_IMPL(CTCMarshalByValue)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CTCMarshalByValue::CTCMarshalByValue()
  : m_dwEndian(0),
  m_dwEndianOriginal(0xFF669900),
  m_dwEndianInverted(0x009966FF)
{
}

#if defined(_DEBUG) && defined(CTCMarshalByValue_DEBUG)

  HRESULT CTCMarshalByValue::FinalConstruct()
  {
    _TRACE0("CTCMarshalByValue::FinalConstruct()\n");
    return S_OK;
  }

  void CTCMarshalByValue::FinalRelease()
  {
    _TRACE0("CTCMarshalByValue::FinalRelease()\n");
  }

#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// IMarshal Interface Methods

STDMETHODIMP CTCMarshalByValue::GetUnmarshalClass(REFIID riid, void* pv,
  DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, CLSID* pCid)
{
  UNUSED(riid);
  UNUSED(pv);
  UNUSED(dwDestContext);
  UNUSED(pvDestContext);
  UNUSED(mshlflags);

  // Delegate to the IPersistStream::GetClassID of the controlling unknown
  IPersistStreamPtr spps;
  HRESULT hr = GetOuterPersistStream(&spps);
  if (SUCCEEDED(hr))
    hr = spps->GetClassID(pCid);

  // Return the last HRESULT
  return hr;
}

STDMETHODIMP CTCMarshalByValue::GetMarshalSizeMax(REFIID riid, void* pv,
  DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, DWORD* pSize)
{
  UNUSED(riid);
  UNUSED(pv);
  UNUSED(dwDestContext);
  UNUSED(pvDestContext);
  UNUSED(mshlflags);

  // Initialize the [out] parameter
  *pSize = 0;

  // Delegate to the IPersistStream::GetSizeMax of the controlling unknown
  IPersistStreamPtr pps;
  HRESULT hr = GetOuterPersistStream(&pps);
  if (SUCCEEDED(hr))
  {
    // Include the size of an endian indicator DWORD value
    ULARGE_INTEGER uli;
    if (SUCCEEDED(hr = pps->GetSizeMax(&uli)) || (E_NOTIMPL == hr
      && SUCCEEDED(hr = TCGetPersistStreamSize(pps, &uli))))
        *pSize = uli.LowPart + sizeof(m_dwEndian);
  }

  // Display a trace message
  #if defined(_DEBUG) && defined(CTCMarshalByValue_DEBUG)
    _TRACE1("CTCMarshalByValue::GetMarshalSizeMax(): returning 0x%08X\n", *pSize);
  #endif

  // Return the last HRESULT
  return hr;
}

STDMETHODIMP CTCMarshalByValue::MarshalInterface(IStream* pStm, REFIID riid,
  void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags)
{
  UNUSED(riid);
  UNUSED(pv);
  UNUSED(dwDestContext);
  UNUSED(pvDestContext);
  UNUSED(mshlflags);

  // Write an endian indicator DWORD value to the stream
  DWORD dwEndian = m_dwEndianOriginal;
  HRESULT hr = pStm->Write(&dwEndian, sizeof(dwEndian), NULL);
  if (SUCCEEDED(hr))
  {
    // Delegate to the IPersistStream::Save of the controlling unknown
    IPersistStreamPtr pps;
    if (SUCCEEDED(hr = GetOuterPersistStream(&pps)))
      hr = pps->Save(pStm, FALSE);
  }

  // Return the last HRESULT
  return hr;
}

STDMETHODIMP CTCMarshalByValue::UnmarshalInterface(IStream* pStm,
  REFIID riid, void** ppv)
{
  // Read the endian indicator DWORD value from the stream
  CLock lock(this);
  HRESULT hr = pStm->Read(&m_dwEndian, sizeof(m_dwEndian), NULL);
  if (SUCCEEDED(hr))
  {
    // Unlock the object
    lock.Unlock();

    // Delegate to the IPersistStream::Load of the controlling unknown
    IPersistStreamPtr pps;
    if (SUCCEEDED(hr = GetOuterPersistStream(&pps)))
      if (SUCCEEDED(hr = pps->Load(pStm)))
        hr = pps->QueryInterface(riid, ppv);
  }

  // Return the last HRESULT
  return hr;
}

STDMETHODIMP CTCMarshalByValue::ReleaseMarshalData(IStream* pStm)
{
  // Marshaling by value does not cause us to acquire any resources
  return S_OK;
}

STDMETHODIMP CTCMarshalByValue::DisconnectObject(DWORD dwReserved)
{
  // Marshaling by value has no notion of connected-ness
  return S_OK;
}

