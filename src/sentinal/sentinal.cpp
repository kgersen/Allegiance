//-------------------------------------------------------------------------
// sentinal\sentinal.cpp
//
// Sentinal server monitoring service
//
// Owner: Mark Snyder (MarkSn)
//
// Copyright (C) 1998 Mark E. Snyder, Microsoft Corp.
//-------------------------------------------------------------------------
#define INITGUID

#define __MODULE__ "sentinal"

#include "zlib.h"
#include <windows.h>
#include <windowsx.h>
#include <dplobby.h>
#include <stdlib.h>
//#include <commctrl.h>
#include <memory.h>
#include "srvdbg.h"
#include "Sentinal.h"
#include "sentmsg.h"
#include "config.h"
#include "dplaychk.h"

DEFINE_GUID(FEDSRV_GUID, 
0x81662310, 0xfcb4, 0x11d0, 0xa8, 0x8a, 0x0, 0x60, 0x97, 0xb5, 0x8f, 0xbf);

#define _INTERNET_PAGING

#ifdef _INTERNET_PAGING
#include <wininet.h>
#endif //_INTERNET_PAGING

// !!! The following enum must match the indices of the items in rgCfg in
// !!! LoadSettings() !!!
typedef enum
    {
    karg_fTimeout, 
    karg_fWantInt3,       
    karg_dwPingInterval,
    karg_szPagerNotificationURL,                  
    karg_szMonitorServers,              
    } SENTINAL_ARGS;

// instance of class that handles registry config parms
ConfigManager* g_pCfgMgr;

const char g_szSvcName[] = "Sentinal";
SERVICE_STATUS_HANDLE g_ssh = NULL;
HINSTANCE     g_hInst           = NULL;
void StartServerTerminateThread();
DWORD WINAPI ServerTerminateThread(DWORD dwUnused);
void SetSvcStatus(DWORD state, DWORD exitcode);
DWORD SendAPage(char* sz, ...);

#ifdef _INTERNET_PAGING

HINTERNET g_hinetSession;
typedef  HINTERNET (WINAPI* PFNINTERNETOPENURL)(HINTERNET, LPCSTR, LPCSTR, DWORD, DWORD, DWORD);
typedef  BOOL (WINAPI* PFNINTERNETGETLASTRESPONSEINFO)(LPDWORD, LPSTR, LPDWORD);
typedef  BOOL (WINAPI* PFNINTERNETCLOSEHANDLE)(HINTERNET);
typedef  HINTERNET (WINAPI* PFNINTERNETOPEN)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);

// prototypes
PFNINTERNETOPENURL             pfnInternetOpenUrl;
PFNINTERNETGETLASTRESPONSEINFO pfnInternetGetLastResponseInfo;
PFNINTERNETCLOSEHANDLE         pfnInternetCloseHandle;
PFNINTERNETOPEN                pfnInternetOpen;

#endif //_INTERNET_PAGING

HINSTANCE g_hMessage;
char g_szComputerName[MAX_COMPUTERNAME_LENGTH + 1];
char g_szServerStatus[16];

HANDLE g_hEventShutdown;

DWORD PingThreadProc(LPVOID lpv)
{
    HANDLE hEventShutdown = (HANDLE)lpv;
    DWORD dwPingInterval = 15000;
    char szServers[MAX_PATH + 1];
    char *psz = NULL;
    int nServers = 0;
    g_pCfgMgr->GetConfigDWORD(kCompId_Sentinal, karg_dwPingInterval, (DWORD*)&dwPingInterval);
    g_pCfgMgr->GetConfigString(kCompId_Sentinal, karg_szMonitorServers, szServers, sizeof(szServers)-1);
    for (psz = szServers; *psz; psz++)
        if (*psz==';' || *psz==',')
        {
            *psz = '\0';
            nServers++;
        }
    *(psz+1) = '\0';
    if (nServers > 16)
        nServers = 16;
    memset(g_szServerStatus, 'U', sizeof(g_szServerStatus));
    
    // Initialize COM library
    HRESULT hr = CoInitialize(NULL);

    BOOL bFirstTime = TRUE;
    while (WAIT_TIMEOUT == WaitForSingleObject(hEventShutdown, bFirstTime ? 10000 : dwPingInterval))
	{
        bFirstTime = FALSE;
        int nServer = 0;
        for(psz = szServers; *psz && nServer<nServers; psz += strlen(psz)+1, nServer++)
		{
            // *** HERE IS WHERE YOU CHECK IF THE SERVER IS STILL UP ***

            CDplayServerChecker dsc;
            dsc.SetServer(psz);
            BOOL rc = dsc.ServerConnectionAlive();
            if (!rc)
            {
                if (g_szServerStatus[nServer]=='U')
                {
                    g_szServerStatus[nServer] = 'D';
                    // send an NT event
                    SRVDBG_ReportEvent(SRVDBG_ERROR_TYPE, SENTINAL_MONITORED_SERVER_DOWN, psz, dsc.m_szMsg);

#ifdef _INTERNET_PAGING
                    // send a pager message
                    SendAPage("Monitored Server Failure", psz, dsc.m_szMsg);
#endif _INTERNET_PAGING
                }
            }            
            else
                g_szServerStatus[nServer] = 'U';

        }
	}

	return(0);
}

#ifdef _INTERNET_PAGING
DWORD SendAPage(char* sz, ...) //const char *sz, ...)
{
	LPTSTR  rgszStrings[5];
	va_list vl;
	va_start(vl, sz);
	rgszStrings[0] = sz;
	rgszStrings[1] = va_arg(vl, LPTSTR);
	rgszStrings[2] = va_arg(vl, LPTSTR);
	rgszStrings[3] = va_arg(vl, LPTSTR);
	rgszStrings[4] = va_arg(vl, LPTSTR);
    va_end(vl);

    char szPagerNotificationURL[MAX_PATH + 1];
    g_pCfgMgr->GetConfigString(kCompId_Sentinal, karg_szPagerNotificationURL, szPagerNotificationURL, sizeof(szPagerNotificationURL)-1);

    char* szFormat = szPagerNotificationURL;
    //for (char* szFormat = szPagerNotificationURL; *szFormat; szFormat += lstrlen(szFormat) + 1)
	{
		if (pfnInternetOpenUrl && pfnInternetGetLastResponseInfo && pfnInternetCloseHandle)
		{
        	char  rgchBuf[1024];
			DWORD cbBuf = sizeof(rgchBuf);
		
			DWORD cch = FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY, szFormat, 0, 0, rgchBuf, sizeof(rgchBuf) - 1, rgszStrings);
		
			// open a session
            HINTERNET hinetHTTPSession = (*pfnInternetOpenUrl)(g_hinetSession, rgchBuf, NULL, 0, INTERNET_FLAG_RELOAD, 0);
			if (NULL == hinetHTTPSession)
            {
                char szMsg[256];
                wsprintf(szMsg, "%08x (%d)", GetLastError(), GetLastError());
                SRVDBG_ReportEvent(SRVDBG_ERROR_TYPE, SENTINAL_INTERNETOPENURL, szMsg);
                return 1;
            }

        	DWORD dwErr;
            BOOL rc = (*pfnInternetGetLastResponseInfo)(&dwErr, rgchBuf, &cbBuf);
            if (!rc)
            {
                char szMsg[256];
                wsprintf(szMsg, "%08x (%d)", GetLastError(), GetLastError());
                SRVDBG_ReportEvent(SRVDBG_ERROR_TYPE, SENTINAL_INTERNETGETLASTRESPONSEINFO, szMsg);
            }

            (*pfnInternetCloseHandle)(hinetHTTPSession);
		}
	}       

	return(0);
}
#endif //_INTERNET_PAGING


/*-------------------------------------------------------------------------
 * LoadSettings
 *-------------------------------------------------------------------------
 * Purpose:
 *    Load static server settings
 */
void LoadSettings()
{
    g_pCfgMgr = new ConfigManager;
    ZAssert(g_pCfgMgr);  // all new's assumed to succeed...

    ZAssert(g_pCfgMgr->Init());

    static const ConfigInfo rgCfg[] =
    {
        // Name                           Type                Max Length              Required?   Default, if not there
		{ "fTimeout",	                  kConfigType_DWORD,  sizeof(DWORD),          TRUE,       0 },
		{ "fWantInt3",                    kConfigType_DWORD,  sizeof(DWORD),          TRUE,       0 },
		{ "dwPingInterval",               kConfigType_DWORD,  sizeof(DWORD),          FALSE,      0 },  
        { "szPagerNotificationURL",       kConfigType_SZ,     MAX_PATH + 1,           TRUE,      (DWORD) "" },
        { "szMonitorServers",             kConfigType_SZ,     MAX_PATH + 1,           TRUE,      (DWORD) "" },
        // !!!! If you add or delete items here, you MUST update the enum SENTINAL_ARGS in CONFIG.H to match
        // !!!! the appropriate index in this array !!!
	};

    g_pCfgMgr->LoadConfig(kCompId_Sentinal, rgCfg, sizeof(rgCfg) / sizeof(rgCfg[0]));
}


/*-------------------------------------------------------------------------
 * Sentinal_Init
 *-------------------------------------------------------------------------
 * Purpose:
 *    Performs startup tasks and initialization for this service.
 * 
 * Side Effects:
 *    Lots
 */
HRESULT pascal Sentinal_Init()
{
    int iCon = 0;
    HRESULT hr = S_OK;

    SRVDBG_Init("Sentinal", NULL);
    SRVDBG_Info("Initialize");

    LoadSettings();

    // Initialize COM library
    hr = CoInitialize(NULL);

    DWORD cbBuf = sizeof(g_szComputerName);
	GetComputerName(g_szComputerName, &cbBuf);

#ifdef _INTERNET_PAGING
	// are we going to be doing paging?
    char szPagerNotificationURL[MAX_PATH + 1];
    g_pCfgMgr->GetConfigString(kCompId_Sentinal, karg_szPagerNotificationURL, szPagerNotificationURL, sizeof(szPagerNotificationURL)-1);
	if (szPagerNotificationURL[0])
	{
		HINSTANCE hinstWininet          = LoadLibrary("wininet.dll");
		pfnInternetOpen                 = (PFNINTERNETOPEN)GetProcAddress(hinstWininet, "InternetOpenA");
		pfnInternetGetLastResponseInfo  = (PFNINTERNETGETLASTRESPONSEINFO)GetProcAddress(hinstWininet, "InternetGetLastResponseInfoA");
		pfnInternetOpenUrl              = (PFNINTERNETOPENURL)GetProcAddress(hinstWininet, "InternetOpenUrlA");
		pfnInternetCloseHandle          = (PFNINTERNETCLOSEHANDLE)GetProcAddress(hinstWininet, "InternetCloseHandle");

		if (pfnInternetOpen)
		{
			g_hinetSession = (*pfnInternetOpen)((char*)g_szSvcName, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (NULL == g_hinetSession)
			{
				SRVDBG_Info("InternetOpen");
			}
		}
	}
#endif //_INTERNET_PAGING

    HANDLE hEventShutdown = CreateEvent(NULL, // no security
                               FALSE, // auto reset
                               FALSE, // initial event reset
                               NULL); // no name

    DWORD idPingThread = 0;
    HANDLE hThreadPing = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PingThreadProc, hEventShutdown, 0, &idPingThread);
    
    hr = (hThreadPing && hEventShutdown) ? S_OK : E_FAIL;
    //ZSucceeded(hr);

    return hr;
}


/*-------------------------------------------------------------------------
 * ShutDown
 *-------------------------------------------------------------------------
 * Purpose:
 *    Cleanup before exiting
 * 
 * Side Effects:
 *    Everything that was allocated is deallocated and no longer valid
 */
HRESULT pascal Sentinal_Terminate(void)
{
    SRVDBG_Info("Terminate");
    SRVDBG_Terminate();

    return(S_OK);
}

////////////////////////////////////////////////////////////////////////
// SetServiceStatus - more useful version of this function than the NT
// one.
//
//

void SetSvcStatus(
  DWORD state,
  DWORD exitcode
)
{
   if (g_ssh)
   {
       SERVICE_STATUS ss;
       static DWORD s_cp = 1;

       ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
       ss.dwCurrentState = state;
       ss.dwControlsAccepted = SERVICE_ACCEPT_STOP;  // todo: pause
       ss.dwWin32ExitCode = exitcode;
       ss.dwServiceSpecificExitCode = 0;

       switch (state)
       {
           case SERVICE_RUNNING:
           case SERVICE_STOPPED:
           case SERVICE_PAUSED:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 1000;
               break;

         case SERVICE_START_PENDING:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 30000;       // we take about 30 seconds to start
            break;

         case SERVICE_STOP_PENDING:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 30000;       // we take about 30 seconds to shutdown
            break;

           default:
               ss.dwCheckPoint = 0;
               ss.dwWaitHint = 0;
               break;
       }

       SetServiceStatus(g_ssh,&ss);
   }
}
       
void StartServerTerminateThread()
{
    DWORD dwId;
    HANDLE hthrPending = CreateThread(NULL, 8192, (LPTHREAD_START_ROUTINE) ServerTerminateThread, 0, 0, &dwId);
    CloseHandle(hthrPending);
}
       
DWORD WINAPI ServerTerminateThread(DWORD dwUnused)
{
    Sentinal_Terminate();
    SetSvcStatus(SERVICE_STOPPED,0);
    return 0;
}      

////////////////////////////////////////////////////////////////////////
// ServiceControl
//
// Service control handler which is registered with NT (using
// RegisterServiceCtrlHandler) as the callback for the service events.
// Right now the service does not support PAUSE/CONTINUE functionality,
// but this could be added here if required.
//
//
void WINAPI ServiceControl(DWORD dwCode)
{
    switch (dwCode)
    {
        case SERVICE_CONTROL_STOP:
            // If it can take us a long time to stop our service as we wait for operations to complete,
            // the SCM doesn't like the ServiceControl() function to take a long time, so we spin up another
            // thread to terminate us.
            SetSvcStatus(SERVICE_STOP_PENDING,0);
            StartServerTerminateThread();
            break;

        case SERVICE_CONTROL_PAUSE:
            break;

        case SERVICE_CONTROL_CONTINUE:
            break;

        case SERVICE_CONTROL_SHUTDOWN:
            // System is shutting down.  Memory and resources should *not*
            // be freed in this case due to limited time available for
            // shutdown.  Instead, only urgent non-volatile (i.e. disk)
            // structures should be flushed.
            break;

        case SERVICE_CONTROL_INTERROGATE:
            break;

    }

}

////////////////////////////////////////////////////////////////////////
// ServiceMain
//
// Pointer to this function is in the SERVICE_TABLE_ENTRY of the
// array passed to StartServiceCtrlDispatcher.

void WINAPI ServiceMain(
  DWORD dwArgc,
  LPSTR *lpszArgv)
{
    HRESULT f;

    g_ssh = RegisterServiceCtrlHandler(g_szSvcName,ServiceControl);

    if (!g_ssh)
    {
        SetSvcStatus(SERVICE_STOPPED,GetLastError());
        return;
    }

    SetSvcStatus(SERVICE_START_PENDING,NO_ERROR);

    f = Sentinal_Init();

    if (f==S_OK)
        SetSvcStatus(SERVICE_RUNNING,NO_ERROR);
    else
        SetSvcStatus(SERVICE_STOPPED,GetLastError());

}
           
////////////////////////////////////////////////////////////////////////
// InstallService - installs this service into the NT service manager
//
//

BOOL InstallService()
{
    SC_HANDLE schMgr;
    SC_HANDLE schSvc;
    char szPath[512];

    schMgr = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

    if (!schMgr)
    {
        printf("Unable to open SCManager.  Service not installed.\n");
        return FALSE;
    }

    GetModuleFileName(NULL,szPath,sizeof(szPath));

    schSvc = CreateService(schMgr,
                           g_szSvcName,
                           "MS Sentinal Server Monitor",
                           SERVICE_ALL_ACCESS,
                           SERVICE_WIN32_OWN_PROCESS,
                           SERVICE_AUTO_START,
                           SERVICE_ERROR_NORMAL,
                           szPath,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           NULL);

    if (!schSvc)
    {
        printf("Unable to create service [0x%08x].  Service not installed.\n", GetLastError());
        CloseServiceHandle(schMgr);
        return FALSE;
    }

    CloseServiceHandle(schSvc);
    CloseServiceHandle(schMgr);
    printf("%s service installed.\n",g_szSvcName);

    return TRUE;
}


////////////////////////////////////////////////////////////////////////
// RemoveService - uninstalls this service from the NT service manager
//
//

BOOL RemoveService(void)
{
    SC_HANDLE schMgr;
    SC_HANDLE schSvc;
    SERVICE_STATUS ss;

    schMgr = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

    if (!schMgr)
    {
        printf("Unable to open SCManager.  Service not removed.\n");
        return FALSE;
    }

    schSvc = OpenService(schMgr,g_szSvcName,SERVICE_ALL_ACCESS);

    if (!schSvc)
    {
        printf("Unable to open %s service.  Service not removed.\n",g_szSvcName);
        CloseServiceHandle(schMgr);
        return FALSE;
    }

    QueryServiceStatus(schSvc,&ss);

    if (ss.dwCurrentState!=SERVICE_STOPPED)
    {
        printf("Unable to remove %s service while it is running.\n",g_szSvcName);
        CloseServiceHandle(schSvc);
        CloseServiceHandle(schMgr);
        return FALSE;
    }

    if (DeleteService(schSvc))
        printf("%s service removed.\n",g_szSvcName);
    else
        printf("Unable to delete % service.\n",g_szSvcName);

    CloseServiceHandle(schSvc);
    CloseServiceHandle(schMgr);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
// RunAsExecutable - for debugging, this service can be run from the
//                   command-line.
//
//

VOID RunAsExecutable()
{
    HRESULT f;

    printf("Microsoft (R) Sentinal Server Monitoring Service\n");
    printf("Copyright (C) Microsoft Corp 1998.  All rights reserved.\n\n");

    printf("Running as an executable.\n");

    printf("Initializing...");

    f = Sentinal_Init();

    if (f==S_OK)
    {
        HANDLE hConsole;
        DWORD cbr;
        char c;

        printf("\rType 'Q' to exit.\n");

        hConsole = GetStdHandle(STD_INPUT_HANDLE);
        SetConsoleMode(hConsole,0);

        while (ReadConsole(hConsole,&c,1,&cbr,NULL) && (int)CharUpper((LPSTR)c)!='Q')
            ;

        Sentinal_Terminate();
    }
    else
    {
        printf("\rInitialization failed. (%d)\n",f);
    }
}



////////////////////////////////////////////////////////////////////////
// main
//
// This is the entry point to the process, whether run as a service, or
// as an executable.  Since the NT service manager does not send us
// command line flags, the default case is to run as a service.
// Additional supported mutually-exclusive functions are installation
// (with the -install command line switch) de-installation (with the
// -remove switch) and running as a non-service executable (with the
// -debug switch).
//
// This function parses the command line arguments and invokes the
// selected mode of operation.  When running as an executable, blocking
// keyboard input is used to prevent the program from exiting prematurely.
//

int __cdecl main(int argc, char *argv[])
{
    SERVICE_TABLE_ENTRY rgSTE[] =
    {
        { (char *)g_szSvcName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };
    bool fShowUsage = false;

    g_hInst = GetModuleHandle(NULL);

    if (argc>1)
    {
        if (*argv[1]=='-' || *argv[1]=='/')
        {
            if (!lstrcmpi("install",argv[1]+1))
                InstallService();
            else if (!lstrcmpi("remove",argv[1]+1))
                RemoveService();
            else if (!lstrcmpi("debug",argv[1]+1))
                RunAsExecutable();
            else
              fShowUsage = true;
        }
        else
          fShowUsage = true;

        if (fShowUsage)
        {
          printf("Microsoft (R) Sentinal Server Monitoring Service\n");
          printf("Copyright (C) Microsoft Corp 1998.  All rights reserved.\n\n");
          printf("usage: %s -install      install the service\n",argv[0]);
          printf("       %s -remove       remove the service\n",argv[0]);
          printf("       %s -debug        run the service as an EXE\n",argv[0]);
          printf("\nTo start this service when installed: net start %s\n",g_szSvcName);
        }
    }
    else //start as service
    {
#ifdef WIN95
      RunAsExecutable();
#else
      if (!StartServiceCtrlDispatcher(rgSTE))
        SRVDBG_Error("StartServiceCtrlDispatcher failed.");
#endif
    }
    return 0;
}

