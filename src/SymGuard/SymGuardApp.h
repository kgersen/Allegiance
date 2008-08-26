#ifndef __SymGuardApp_h__
#define __SymGuardApp_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// SymGuardApp.h: Definition of the SymGuardApp class
//

#include <SymGuard.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CSymModule;


/////////////////////////////////////////////////////////////////////////////
// CSymGuardApp
//
class CSymGuardApp : 
  public IDispatchImpl<ISymGuardApp, &IID_ISymGuardApp, &LIBID_SymGuardLib>, 
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CSymGuardApp,&CLSID_SymGuardApp>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_SymGuardApp)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CSymGuardApp)
    IMPLEMENTED_CATEGORY(CATID_SymGuard)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CSymGuardApp)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISymGuardApp)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_spUnkMarshaler.p)
  END_COM_MAP()

// Construction / Destruction
public:
  HRESULT FinalConstruct();
  void FinalRelease();

// Operations
public:
  HRESULT FlushModuleFromCache(const ZString& strCacheKey);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ISymGuardApp Interface Methods
public:
  STDMETHODIMP LoadModule(BSTR bstrSymbolPath, BSTR bstrModuleName,
    VARIANT* pvarImageBase, VARIANT* pvarImageSize, ISymModule** ppModule);
  STDMETHODIMP FlushAllModulesFromCache(long* pcModulesFlushed);
  STDMETHODIMP get_Version(ISymGuardVersion** ppVersion);
  STDMETHODIMP get_DbgHelpVersion(ISymGuardVersion** ppVersion);

// Types
protected:
  typedef TCObjectLock<CSymGuardApp>     XLock;
  typedef std::map<ZString, CSymModule*> XModules;
  typedef XModules::iterator             XModuleIt;

// Data Members
public:
  CComPtr<IUnknown> m_spUnkMarshaler;
protected:
  XModules          m_ModuleCache;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__SymGuardApp_h__
