/////////////////////////////////////////////////////////////////////////////
// Pig$$Object$$.h : Declaration of the CPig$$Object$$ class

#ifndef __Pig$$Object$$_h__
#define __Pig$$Object$$_h__

#if _MSC_VER >= 1000
  #pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"       // main symbols
#include "PigEngine.h"
#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// CPig$$Object$$
class ATL_NO_VTABLE CPig$$Object$$ : 
  public IDispatchImpl<IPig$$Object$$, &IID_IPig$$Object$$, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPig$$Object$$, &CLSID_Pig$$Object$$>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_Pig$$Object$$)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPig$$Object$$)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPig$$Object$$)
    COM_INTERFACE_ENTRY(IPig$$Object$$)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPig$$Object$$();
  HRESULT FinalConstruct();
  void FinalRelease();

// Implementation
protected:

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// IPig$$Object$$ Interface Methods
public:

// Types
protected:
  typedef TCObjectLock<CPig$$Object$$> XLock;

// Data Members
protected:
};


/////////////////////////////////////////////////////////////////////////////

#endif //__Pig$$Object$$_h__
