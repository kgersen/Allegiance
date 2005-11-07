// AllegianceController.h : Declaration of the CAllegianceController

#ifndef __ALLEGIANCECONTROLLER_H_
#define __ALLEGIANCECONTROLLER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAllegianceController
class ATL_NO_VTABLE CAllegianceController : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAllegianceController, &CLSID_AllegianceController>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAllegianceController, &IID_IAllegianceController, &LIBID_FEDSRVLib>
{
public:
	CAllegianceController()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ALLEGIANCECONTROLLER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAllegianceController)
	COM_INTERFACE_ENTRY(IAllegianceController)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAllegianceController
public:
	STDMETHOD(KillService)();
	STDMETHOD(QueryService)();
	STDMETHOD(RemoveService)();
	STDMETHOD(InstallService)(BSTR bstrUser, BSTR bstrPassword);
	STDMETHOD(StopService)();
	STDMETHOD(StartService)();
};

#endif //__ALLEGIANCECONTROLLER_H_
