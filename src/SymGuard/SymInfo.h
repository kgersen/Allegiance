#ifndef __SymInfo_h__
#define __SymInfo_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSymInfo.h: Definition of the CSymInfo class
//

#include <SymGuard.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CSymInfo
//
class CSymInfo : 
  public IDispatchImpl<ISymInfo, &IID_ISymInfo, &LIBID_SymGuardLib>, 
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CSymInfo, &CLSID_SymInfo>
{
// Declarations
public:
  //DECLARE_NOT_AGGREGATABLE(CSymInfo) 
  DECLARE_REGISTRY_RESOURCEID(IDR_SymInfo)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CSymInfo)
    IMPLEMENTED_CATEGORY(CATID_SymGuard)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CSymInfo)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISymInfo)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_spUnkMarshaler.p)
  END_COM_MAP()

// Construction / Destruction
public:
  CSymInfo();
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Init(HANDLE hProcessSym, DWORD dwAddr, ISymModule* pModule);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ISymInfo Interface Methods
public:
  STDMETHODIMP get_Address(VARIANT* pvar);
  STDMETHODIMP get_Name(BSTR* pbstr);
  STDMETHODIMP get_DecoratedName(BSTR* pbstr);
  STDMETHODIMP get_SourceFileName(BSTR* pbstr);
  STDMETHODIMP get_LineNumber(long* pn);
  STDMETHODIMP get_BytesFromSymbol(long* pcb);
  STDMETHODIMP get_BytesFromLine(long* pcb);

// Data Members
public:
	CComPtr<IUnknown> m_spUnkMarshaler;
protected:
  DWORD         m_dwAddress;
  DWORD         m_dwDisplacement;
  CComBSTR      m_bstrName;
  CComBSTR      m_bstrDecoratedName;
  CComBSTR      m_bstrSourceFileName;
  IMAGEHLP_LINE m_il;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__SymInfo_h__
