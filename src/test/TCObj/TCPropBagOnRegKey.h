#ifndef __TCPropBagOnRegKey_h__
#define __TCPropBagOnRegKey_h__

/////////////////////////////////////////////////////////////////////////////
// TCPropBagOnRegKey.h | Declaration of the CTCPropBagOnRegKey class,
// which implements the CLSID_TCPropBagOnRegKey component object.
//

#include "resource.h"       // main symbols
#include "SrcInc.h"
#include <TCObj.h>

#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// CTCPropBagOnRegKey

class ATL_NO_VTABLE CTCPropBagOnRegKey :
  public IDispatchImpl<ITCPropBagOnRegKey, &IID_ITCPropBagOnRegKey, &LIBID_TCObj>,
  public IPropertyBag,
  public CComCoClass<CTCPropBagOnRegKey, &CLSID_TCPropBagOnRegKey>,
  public CComObjectRoot
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCPropBagOnRegKey)

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCPropBagOnRegKey)
    IMPLEMENTED_CATEGORY(CATID_TCObj)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCPropBagOnRegKey)
    COM_INTERFACE_ENTRY(ITCPropBagOnRegKey)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IPropertyBag)
  END_COM_MAP()

// Construction / Destruction
public:
  CTCPropBagOnRegKey();
  void FinalRelease();
  void Init(HKEY hkey, const _bstr_t& strSubkey,
    CTCPropBagOnRegKey* pParent);
protected:
  void ClosePrevious();

// Attributes
public:
  _bstr_t _GetSubkey();
  _bstr_t _GetServer(const _bstr_t& strName);
  HRESULT _SetServer(const _bstr_t& strName, const _bstr_t& strServer);
  HRESULT _GetObjectCLSID(const _bstr_t& strName, CLSID& clsid);

// Operations
public:
  HRESULT _CreateObject(const _bstr_t& strName, IUnknown** ppunkObj,
    IErrorLog* pErrorLog = NULL, IUnknown* punkOuter = NULL);
  HRESULT _CreateLocalObject(const _bstr_t& strName, IUnknown** ppunkObj,
    IErrorLog* pErrorLog = NULL, IUnknown* punkOuter = NULL);
  HRESULT _CreateRemoteObject(const _bstr_t& strServer,
    const _bstr_t& strName, IUnknown** ppunkObj,
    IErrorLog* pErrorLog = NULL);
  HRESULT _LoadObject(const _bstr_t& strName, IUnknown* punkObj,
    IErrorLog* pErrorLog = NULL);
  HRESULT _SaveObject(const _bstr_t& strName, IUnknown* punkObj,
    BOOL bClearDirty = FALSE, BOOL bSaveAllProperties = TRUE);

// Implementation
public:
  HRESULT _GetObjectCLSID(const _bstr_t& strName, CLSID& clsid,
    CRegKey& subkey, _bstr_t& strObjectID, IErrorLog* pErrorLog);
  HRESULT _ReadVariantFromValue(CRegKey& key, const _bstr_t& strPropName,
    DWORD nType, int cbData, CComVariant& v, IErrorLog* pErrorLog);
  HRESULT _LogError(const _bstr_t& strFn, const _bstr_t& strDesc, HRESULT hr,
    LPCOLESTR pszPropName, IErrorLog* pErrorLog = NULL);
  VARTYPE _GetSubkeyVarType(CRegKey& key, const _bstr_t& strPropName);
  bool _IsSubkeySafeArray(CRegKey& key, const _bstr_t& strPropName);
  bool _IsSubkeyObject(CRegKey& key, const _bstr_t& strPropName);
  HRESULT _ReadSafeArray(CRegKey& key, const _bstr_t& strPropName,
    VARIANT* pVar, IErrorLog* pErrorLog);
  HRESULT _WriteSafeArray(CRegKey& key, const _bstr_t& strPropName,
    VARIANT* pVar);
  HRESULT _ReadVariant(CRegKey& key, const _bstr_t& strValueName,
    VARIANT* pVar, IErrorLog* pErrorLog);
  HRESULT _WriteVariant(CRegKey& key, const _bstr_t& strValueName,
    VARIANT* pVar);
  VARTYPE _VarTypeFromUnknown(IUnknown* punk);

  static bool KeyExists(HKEY hkey, const _bstr_t& strValueName);
  static LONG QueryString(HKEY hkey, const _bstr_t& strValueName,
    _bstr_t& strOut);


// ITCPropBagOnRegKey Interface Methods
public:
  STDMETHODIMP CreateKey(BSTR bstrSubKey, VARIANT_BOOL bReadOnly);
  STDMETHODIMP OpenKey(BSTR bstrSubKey, VARIANT_BOOL bReadOnly);
  STDMETHODIMP CreateObject(IUnknown** ppunkObj);
  STDMETHODIMP CreateLocalObject(IUnknown** ppunkObj);
  STDMETHODIMP CreateRemoteObject(BSTR bstrServer, IUnknown** ppunkObj);
  STDMETHODIMP LoadObject(IUnknown* punkObj);
  STDMETHODIMP SaveObject(IUnknown* punkObj, VARIANT_BOOL bClearDirty,
    VARIANT_BOOL bSaveAllProperties, VARIANT_BOOL bSaveCreationInfo);
  STDMETHODIMP put_Server(BSTR bstrServer);
  STDMETHODIMP get_Server(BSTR* pbstrServer);
  STDMETHODIMP get_ObjectCLSID(BSTR* pbstrCLSID);

// IPropertyBag Interface Methods
public:
  STDMETHODIMP Read(LPCOLESTR pszPropName, VARIANT* pVar,
    IErrorLog* pErrorLog);
  STDMETHODIMP Write(LPCOLESTR pszPropName, VARIANT* pVar);

// Types
protected:
  typedef TCObjectLock<CTCPropBagOnRegKey> XLock;

// Data Members
protected:

// Data Members
protected:
  _bstr_t             m_strSubkey;
  CTCPropBagOnRegKey* m_pParent;
  CRegKey             m_key;
  bool                m_bOwnKey;

// Constants
protected:
  const static _bstr_t m_szVariantType;
  const static _bstr_t m_szServer;
  const static _bstr_t m_szProgID;
  const static _bstr_t m_szCLSID;
  const static _bstr_t m_szLowerBound;
  const static _bstr_t m_szElementCount;
  const static _bstr_t m_szElementFmt;
  const static USHORT  m_wChangeTypeFlags;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

inline void CTCPropBagOnRegKey::Init(HKEY hkey, const _bstr_t& strSubkey,
  CTCPropBagOnRegKey* pParent)
{
  m_key.Attach(hkey);
  m_strSubkey = strSubkey;
  m_pParent = pParent;
  m_bOwnKey = false;
}

inline void CTCPropBagOnRegKey::ClosePrevious()
{
  // Detach from the current key if we don't own it
  XLock lock(this);
  if (!m_bOwnKey)
    m_key.Detach();
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCPropBagOnRegKey_h__

