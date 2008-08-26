/*-------------------------------------------------------------------------
  ClubApp.cpp
  
  Implementation of the Club 
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include <conio.h>

#include <zreg.h>

// KGJV changed & moved in regkey.h (see pch.h)
//#define HKLM_AllClub "SYSTEM\\CurrentControlSet\\Services\\AllClub" // Keep in sync with HKLM_FedSrv found in ClubQueries.h

CSQLSiteImpl g_SQLSite;  // needs to be global cause SQL stuff is globally initialized

int CSQLSiteImpl::OnMessageBox(const char * strText, const char * strCaption, UINT nType)
{
  char szMsg[256]; 
  sprintf(szMsg, "%0.55s : %0.190s", strCaption, strText);

  if (g_pClubApp)
    g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, szMsg);
  else
    printf(szMsg);

  return 0;
}

ALLOC_MSG_LIST;

CClubApp * g_pClubApp = NULL;

CClubApp::CClubApp(IClubSite * plas) :
  m_plas(plas),
  m_fmClients(&m_siteClient)
  //$ ASYNCCLUB
  // ,m_sql(this)
{
  m_plas->LogEvent(EVENTLOG_INFORMATION_TYPE, "Creating AllClub");
  //g_pStaticData = CreateStaticData(); KGJV - obsolete
  assert(m_plas);
  g_pClubApp = this;

  HKEY  hk;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, HKLM_AllClub, 0, "", REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hk, NULL) == ERROR_SUCCESS)
  {
    _Module.ReadFromRegistry(hk, true,  "AuthServer",       m_szAuthServer,     (DWORD) "pointweb01");
    _Module.ReadFromRegistry(hk, true,  "Token",            m_szToken,          (DWORD) "OBLI_PBILL");
/*
    _Module.ReadFromRegistry(hk, false, "SQLThreadsNotify", &m_dwThreadsNotify, (DWORD) 5);
    _Module.ReadFromRegistry(hk, false, "SQLThreadsSilent", &m_dwThreadsSilent, (DWORD) 1);
    if (FAILED(LoadRegString(hk, "SQLConfig", m_bstrSQLConfig)))
      m_bstrSQLConfig.Empty(); // default for all developers
*/
  }
  else
  {
    assert(0); // if we can't access the registry, we got real problems
  }
  m_pzas = CreateZoneAuthServer();
  
}

CClubApp::~CClubApp()
{
  delete [] m_vRankInfo;
  m_cRankInfo = 0;
  m_pzas = NULL;

  m_plas->LogEvent(EVENTLOG_INFORMATION_TYPE, "Shutting down AllClub");
  //if(g_pStaticData) KGJV - obsolete
  //  delete g_pStaticData;
  m_perfshare.FreeCounters(m_pCounters);
}



HRESULT CClubApp::Init()
{
  HRESULT hr = E_FAIL;

  m_cRankInfo = 0;
  m_vRankInfo = NULL;

  m_plas->LogEvent(EVENTLOG_INFORMATION_TYPE, "Initializing AllClub");
  ZVerify(m_perfshare.Initialize());
  m_pCounters = (CLUB_COUNTERS *)m_perfshare.AllocateCounters(
                      "AllClub", "0",    // if there are ever multiple lobbies running, change this
                      sizeof(CLUB_COUNTERS));
  ZeroMemory(m_pCounters, sizeof(CLUB_COUNTERS));

/*
  //$ ASYNCCLUB
  hr = m_sql.Init(m_bstrSQLConfig, _Module.dwThreadID, m_dwThreadsSilent, m_dwThreadsNotify);
  if (FAILED(hr))
  {
    return hr;
  }
*/

  // TODO: Make keep-alives an option
  if (FAILED(hr = m_fmClients.HostSession(FEDCLUB_GUID, true, 0, true)))
    m_plas->LogEvent(EVENTLOG_ERROR_TYPE, "Could not host dplay session. Check for correct dplay installation");

  return hr;
}

int CClubApp::Run()
{
  MSG msg;
  DWORD dwWait = WAIT_TIMEOUT;

  m_plas->LogEvent(EVENTLOG_INFORMATION_TYPE, "Running AllClub");
  _putts("---------Press Q to exit---------");
   printf("Ready for clients.\n");
  CTempTimer timerIterations("between iterations", .25f);
  timerIterations.Start();
  CTempTimer timerReceiveClientsMessages("in clients ReceiveMessages()", .05f);
  CTempTimer timerPerfCounters("assembling perf info", .05f);

  Time timeLastQueueCheck = Time::Now();
  while (true)
  {
    timerIterations.Stop();
    timerIterations.Start();

    if (PeekMessage(&msg, 0, WM_QUIT, WM_QUIT, PM_REMOVE) ||
       (_kbhit() && toupper(_getch()) == 'Q'))
      return 0;

    SetNow();

    m_pCounters->timeInnerLoop = timerIterations.LastInterval();

    // receive any messages in the queue
    timerReceiveClientsMessages.Start();
    m_fmClients.ReceiveMessages();
    timerReceiveClientsMessages.Stop();

    if (GetNow() - timeLastQueueCheck >= 1.0f)
    {
      // count the fairly expensive stuff no more than once a second
      timerPerfCounters.Start();
      m_fmClients.GetSendQueue(&(m_pCounters->cOutboundQueueLength),
                               &(m_pCounters->cOutboundQueueSize));
      m_fmClients.GetReceiveQueue(&(m_pCounters->cInboundQueueLength),
                                  &(m_pCounters->cInboundQueueSize));

      m_pCounters->cPlayers = m_fmClients.GetCountConnections();
      timeLastQueueCheck = Time::Now();
      timerPerfCounters.Stop();
    }
    Sleep(1);
  }
  return 0;
}

int CClubApp::OnMessageBox(const char * strText, const char * strCaption, UINT nType)
{
  char sz[256];
  if (strCaption && *strCaption)
  {
    lstrcpy(sz, strCaption);
    lstrcat(sz, ": ");
  }
  lstrcat(sz, strText);
  return m_plas->LogEvent(EVENTLOG_ERROR_TYPE, strText);
}

bool CClubApp::OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule)
{
  m_plas->LogEvent(EVENTLOG_ERROR_TYPE, ZString("assertion failed: '")
            + psz
            + "' ("
            + pszFile
            + ":"
            + ZString(line)
            + ")\n"
  );
  return true;
}

void CClubApp::DebugOutput(const char *psz)
{
  ::OutputDebugString("AllClub: ");
  #ifdef _DEBUG
    Win32App::DebugOutput(psz);
  #endif
}


//$ ASYNCCLUB
/*
void CClubApp::OnSQLErrorRecord(SSERRORINFO * perror, OLECHAR * postrError) 
{
  // don't make the event an error event, because this may or may not be fatal. 
  // But we certainly want to see them all in any case.
  m_plas->LogEvent(EVENTLOG_WARNING_TYPE, "SQL Server Error (if deadlock or timeout, then it's not fatal):\n"
      "  Message:   %s\n"
      "  Procedure: %s\n"
      "  Native:    %d\n"
      "  Line:      %d\n"
      "  OleDB:     %s\n",
      perror->pwszMessage, perror->pwszProcedure, perror->lNative, perror->wLineNumber, postrError);
}


void CClubApp::OnOLEDBErrorRecord(BSTR bstrDescription, GUID guid, DWORD dwHelpContext, 
                                  BSTR bstrHelpFile,    BSTR bstrSource)
{
  // These are always fatal, because we have no idea what we can do about it.
  m_plas->LogEvent(EVENTLOG_WARNING_TYPE, "Fatal Database Error (OLDEB):\n"
    "  Desc:     %s\n"
    "  Guid:     {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n"
    "  HelpID:   %u\n"
    "  HelpFile: %s\n"
    "  Source:   %s\n",
    bstrDescription, 
    guid.Data1, guid.Data2, guid.Data3, 
    guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
    dwHelpContext, bstrHelpFile, bstrSource);
}
*/





