#ifndef __SymModuleImpl_h__
#define __SymModuleImpl_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// SymModuleImpl.h: Definition of the CSymModuleImpl class
//

#include <SymGuard.h>


/////////////////////////////////////////////////////////////////////////////
// CSymModuleImpl
//
class CSymModuleImpl :
  public ISymModule,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CSymModuleImpl)
    COM_INTERFACE_ENTRY(ISymModule)
  END_COM_MAP()

// Construction / Destruction
public:
  CSymModuleImpl();
  HRESULT FinalConstruct();
  void FinalRelease();
protected:
  HRESULT Init(BSTR bstrSymbolPath, BSTR bstrModuleName,
    DWORD dwImageBase, DWORD dwImageSize, const ZString& strCacheKey);
  void UnloadModule();

// Operations
public:
  static HRESULT CreateModule(BSTR bstrSymbolPath, BSTR bstrModuleName,
    VARIANT* pvarImageBase, VARIANT* pvarImageSize, ISymModule** ppSymModule);
  static HRESULT VariantHexToDWORD(VARIANT* pvar, DWORD* pdw);

// Overrides
public:
  // CComObjectRootEx Overrides
  ULONG InternalRelease();

// Implementation
protected:
  HANDLE GetSymHandle() const;

// IDispatch Interface Methods
public:
  STDMETHODIMP GetTypeInfoCount(UINT* pctinfo);
  STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
  STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
    LCID lcid, DISPID* rgDispId);
  STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
    WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult,
    EXCEPINFO* pExcepInfo, UINT* puArgErr);

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
  typedef TCObjectLock<CSymModuleImpl>        XLock;
  typedef TCObjectLock<TCAutoCriticalSection> XStaticLock;
  typedef std::map<ZString, ISymModule*>      XModules;
  typedef XModules::iterator                  XModuleIt;

// Data Members
protected:
  IMAGEHLP_MODULE m_im;
  long            m_nMinutes;
  DWORD           m_dwImageBase;
  ZString         m_strCacheKey;
  bool            m_bInDestructionLock;
protected:
  static TCAutoCriticalSection s_cs;
  static XModules              s_ModuleCache;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__SymModuleImpl_h__
