#ifndef __SymModule_h__
#define __SymModule_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// SymModule.h: Definition of the SymModule class
//

#include <SymGuard.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CSymGuardApp;


/////////////////////////////////////////////////////////////////////////////
// CSymModule
//
class CSymModule : 
  public IDispatchImpl<ISymModule, &IID_ISymModule, &LIBID_SymGuardLib>, 
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CSymModule, &CLSID_SymModule>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_SymModule)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CSymModule)
    IMPLEMENTED_CATEGORY(CATID_SymGuard)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CSymModule)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISymModule)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_spUnkMarshaler.p)
  END_COM_MAP()

// Construction / Destruction
public:
  CSymModule();
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Init(BSTR bstrSymbolPath, BSTR bstrModuleName,
    VARIANT* pvarImageBase, VARIANT* pvarImageSize, CSymGuardApp* pApp,
    const ZString& strCacheKey);
  void UnloadModule();

// Attributes
public:
  static HRESULT GetCacheKey(BSTR bstrSymbolPath, BSTR bstrModuleName,
    VARIANT* pvarImageBase, VARIANT* pvarImageSize, ZString& strKey);

// Implementation
protected:
  HANDLE GetSymHandle() const;
  static HRESULT VariantHexToDWORD(VARIANT* pvar, DWORD* pdw);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ISymModule Interface Methods
public:
  STDMETHODIMP GetSymbolFromAddress(VARIANT* pvarAddress,
    ISymInfo** ppSymInfo);
  STDMETHODIMP FlushModuleFromCache();
  STDMETHODIMP put_Timeout(long nMinutes);
  STDMETHODIMP get_Timeout(long* pnMinutes);
  STDMETHODIMP put_BuildPathBase(BSTR bstr);
  STDMETHODIMP get_BuildPathBase(BSTR* pbstr);
  STDMETHODIMP put_SourcePathBase(BSTR bstr);
  STDMETHODIMP get_SourcePathBase(BSTR* pbstr);
  STDMETHODIMP get_SymbolPath(BSTR* pbstr);
  STDMETHODIMP get_ModuleName(BSTR* pbstr);
  STDMETHODIMP get_ImageBase(VARIANT* pvar);
  STDMETHODIMP get_ImageSize(VARIANT* pvar);
  STDMETHODIMP get_TimeDateStamp(DATE* pd);
  STDMETHODIMP get_CheckSum(VARIANT* pvar);
  STDMETHODIMP get_SymbolCount(long* pc);
  STDMETHODIMP get_SymbolType(BSTR* pbstr);
  STDMETHODIMP get_ImageName(BSTR* pbstr);
  STDMETHODIMP get_LoadedImageName(BSTR* pbstr);

// Types
protected:
  typedef TCObjectLock<CSymModule> XLock;

// Data Members
public:
	CComPtr<IUnknown> m_spUnkMarshaler;
protected:
  CComBSTR          m_bstrBuildPathBase;
  CComBSTR          m_bstrSourcePathBase;
  IMAGEHLP_MODULE   m_im;
  long              m_nMinutes;
  DWORD             m_dwImageBase;
  ZString           m_strCacheKey;
  CSymGuardApp*     m_pApp;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__SymModule_h__
