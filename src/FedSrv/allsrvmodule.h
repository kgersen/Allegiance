
/*-------------------------------------------------------------------------
 * fedsrv\AllSrvModule.H
 * 
 * This part of AllSrv contains Service-related functions and COM/ATL support.  
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



#ifndef _ALLSRV_MOD_
#define _ALLSRV_MOD_

#ifndef STRICT
  #define STRICT
#endif
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
#include <AGC.h>
#include <..\TCLib\AutoHandle.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CServiceModule : public CComModule
{
// Construction / Destruction
public:
  CServiceModule() :
    m_fCOMStarted(false),
		m_iPIDID(0)
  {
  }
  HRESULT      Init(HINSTANCE hInst);
  void         Term();
  HRESULT      InitAGC();
  void         TermAGC();

// Attributes
public:
  BOOL         IsInstalled(); // returns TRUE iff AllSrv was properly installed (either as Service or EXE)
  BOOL         IsInstalledAsService(); // returns TRUE iff AllSrv is installed as an NT service.
  BOOL         IsInServiceControlManager(); // TRUE iff AllSrv is an NT service (by checking the Service Control Manager)
  HRESULT      get_EventLog(IAGCEventLogger** ppEventLogger);

// Operations
public:
  HRESULT      RegisterServer(BOOL bReRegister, BOOL bRegTypeLib, BOOL bService, int argc, char * argv[]);
  HRESULT      UnregisterServer();

  void         RegisterCOMObjects();
  void         RevokeCOMObjects();

  VOID         RunAsExecutable();
  //imago - parent/child server tracking functions 6/23/08
#if defined(SRV_PARENT)  
  void         BreakChildren();
  DWORD		   GetOutgoingPort(void);													//remember the last port used to 
  void		   SetOutgoingPort(DWORD dwPort);											//handle game creation requests from lobby

  void		   AddPID(DWORD dwProcessId){ m_dwPIDs[m_iPIDID]=dwProcessId; m_iPIDID++; }
  
  void		   InitPIDs();																//also remember children pids for cleanup and
  int		   GetChildCount(){return m_iPIDID;}										//provide accurate count to the server list 
#endif 
  BOOL         InstallService(int argc, char * argv[]);
  BOOL         RemoveService(void);

  static void  WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

  void         SetCOMStarted(bool fCOMStarted) { m_fCOMStarted = fCOMStarted; }
  bool         WasCOMStarted() { return m_fCOMStarted; }

  void         StopAllsrv();

// Implementation
protected:
  static void  WINAPI StartServerTerminateThread();
  static void  WINAPI ServiceControl(DWORD dwCode);
  static void  SetSvcStatus(DWORD state, DWORD exitcode);
  static DWORD WINAPI ServerTerminateThread(DWORD dwUnused);
  static DWORD WINAPI MTAKeepAliveThunk(void* pvThis);
         void  MTAKeepAliveThread();

// Data Members
protected:
  bool          m_fCOMStarted;       // true if a COM call started the server
//  HANDLE        m_hEventStopRunningAsEXE; 
  TCHandle      m_shthMTA;           // thread handle of MTA keep-alive thread
  TCHandle      m_shevtMTAReady;     // event to sync MTA keep-alive thread
  TCHandle      m_shevtMTAExit;      // event to sync MTA keep-alive thread
  HRESULT       m_hrMTAKeepAlive;    // HRESULT from MTA keep-alive thread
  IAGCEventLoggerPtr m_spEventLogger;
  DWORD			m_dwMPort;				// our outgoing port last used to connect to g.dwLobby -Imago
  DWORD			m_dwPIDs[98];	     // simple array of process identifiers, we don't use key 0 for simplicity
  int			m_iPIDID;			// last used pid key, gets re-calculated each hand-off
};

extern CServiceModule _Module;
#include <atlcom.h>



void PrintSystemErrorMessage(LPCTSTR szBuf, DWORD dwErrorCode);
#if defined(SRV_PARENT)
void WINAPI MPServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
#else
void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
#endif

extern const GUID APPID_AllSrv;
extern const char *c_szAPPID_AllSrv; // string form of APPID_AllSrv
extern const CATID CATID_AllegianceAdmin;


#endif //_ALLSRV_MOD