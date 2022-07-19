#ifndef __AdminEventLoggerHook_h__
#define __AdminEventLoggerHook_h__
//#if !defined(ALLSRV_STANDALONE) //Imago enabled 6/10

/////////////////////////////////////////////////////////////////////////////
// AdminEventLoggerHook.h : Declaration of the CAdminEventLoggerHook


/////////////////////////////////////////////////////////////////////////////
// CAdminEventLoggerHook
class ATL_NO_VTABLE CAdminEventLoggerHook : 
  public IAGCEventLoggerHook,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminEventLoggerHook)
    COM_INTERFACE_ENTRY(IAGCEventLoggerHook)
  END_COM_MAP()

// Implementation
private:
  static void EventLogged(CQLogEvent* pquery);

// IAGCEventLoggerHook Interface Methods
public:
  STDMETHODIMP LogEvent(IAGCEvent* pEvent, VARIANT_BOOL bSynchronous);
};


/////////////////////////////////////////////////////////////////////////////

//#endif // !defined(ALLSRV_STANDALONE)
#endif //__AdminEventLoggerHook_h__
