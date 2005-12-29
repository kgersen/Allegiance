#define INITGUID
#include <windows.h>
#include <windowsx.h>
//#include <dplobby.h>
#include "resource.h"

// {81662310-FCB4-11d0-A88A-006097B58FBF}
DEFINE_GUID(FEDSRV_GUID, 
0x81662310, 0xfcb4, 0x11d0, 0xa8, 0x8a, 0x0, 0x60, 0x97, 0xb5, 0x8f, 0xbf);

#define WM_MOUSEINTRAY WM_USER + 1

HICON hIconUp, hIconDown, hIconInit, hIconBoth;
HINSTANCE hInst;
const int c_cSrvMax = 10;
int g_cSrv = 0; // how many servers we're monitoring
int g_iSrvDlg = 0; // what the current server in the dialog is
int g_iSrvCur = 0; // what the current server is that we're querying
const int c_cbServer = 32;
char rgszServer[c_cSrvMax][c_cbServer];
int secInterval = 60; // I'm sorry, but I'm not going to have separate intervals for each server
const char * const SzAppName = "FSMon";
HWND hWnd;
NOTIFYICONDATA nid;
IDirectPlay3A * pDirectPlay3A[c_cSrvMax];
IDirectPlayLobby * pDirectPlayLobbyA;
bool fDPInit[c_cSrvMax];

char * const gszMsgInit = "Initializing...";
char * gszMsg = gszMsgInit;
enum State {FS_UP, FS_DOWN, FS_INIT, FS_BOTH};
State gState[c_cSrvMax];
DWORD gdwCurrentPlayers[c_cSrvMax];
char g_rgszDesc[c_cSrvMax][1000];


void CheckConnection();


/****************************************************************************\
*
*    FUNCTION: DlgProcSettings
*
*    PURPOSE:  Handles Settings dialog box
*
*\***************************************************************************/
BOOL CALLBACK DlgProcSettings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) 
  {
    case WM_INITDIALOG:
    {
      SetDlgItemText(hDlg, IDC_EDIT_SERVER,   rgszServer[g_iSrvDlg]);
      SetDlgItemInt( hDlg, IDC_EDIT_INTERVAL, secInterval, FALSE);

      return 0; // set focus on first control
    }
    case WM_COMMAND:                         // message: received a command
    {
      switch (LOWORD(wParam))
      {
        case IDOK:
        {
          GetDlgItemText(hDlg, IDC_EDIT_SERVER, rgszServer[g_iSrvDlg], sizeof(rgszServer[0]));
          secInterval = GetDlgItemInt( hDlg, IDC_EDIT_INTERVAL, NULL, FALSE);
          if (secInterval < 10)
            secInterval = 10;
          if (secInterval < 3 * g_cSrv)
            secInterval = 3 * g_cSrv;
        
          // Force reconnection
          if (fDPInit[g_iSrvDlg]) 
          {
            if (pDirectPlay3A[g_iSrvDlg])
            {
              pDirectPlay3A[g_iSrvDlg]->Release();
              pDirectPlay3A[g_iSrvDlg] = NULL;
            }
            fDPInit[g_iSrvDlg] = false;
          }

          EndDialog(hDlg, TRUE);
          PostMessage(hWnd, WM_TIMER, 0, 0);
          KillTimer(hWnd, 1);
          SetTimer(hWnd, 1, 1000 * secInterval, NULL);      
          lstrcpy(g_rgszDesc[g_iSrvDlg], gszMsgInit); 
          return (TRUE);
        }

        case IDCANCEL:
        {
          EndDialog(hDlg, TRUE);
          return (TRUE);
	      }

        case ID_BTN_UNLOAD:
        {
          if (IDYES == MessageBox(hDlg, "Are you sure you want to cancel all server monitoring and kill FSMon?",
                                  "FSMon", MB_YESNO | MB_ICONQUESTION))
          {
            EndDialog(hDlg, TRUE);
            PostQuitMessage(0);
            return (TRUE);
          }
        }
      }
    }
    break;
  } 
  return (FALSE);
}


/****************************************************************************\
*
*    FUNCTION:  EnumSessionsCallback2
*
*    PURPOSE:  Called by IDirectPlay3A::EnumSessions. 
*
*\***************************************************************************/
BOOL FAR PASCAL EnumSessionsCallback2(LPCDPSESSIONDESC2 lpThisSD,
                                      LPDWORD lpdwTimeOut,
                                      DWORD dwFlags,
                                      LPVOID lpContext)
{
  // We're not actually going to open this session, so let's just remember that we have one
  if (dwFlags & DPESC_TIMEDOUT)
		return (FALSE);						
  
  if (lpThisSD) // will be null if we timed out
  {
    *(bool *)lpContext = true;
    gState[g_iSrvCur] = FS_UP;
    gdwCurrentPlayers[g_iSrvCur] = lpThisSD->dwCurrentPlayers - 1;
    wsprintf(g_rgszDesc[g_iSrvCur], "Players: %d", (int) (gdwCurrentPlayers[g_iSrvCur]));
  }
  return FALSE;
}


/****************************************************************************\
*
*    FUNCTION: CheckConnection
*
*    PURPOSE:  Check the connection and update the taskbar
*
*\***************************************************************************/
void CheckConnection()
{
  HRESULT hr = E_FAIL;

  if (!fDPInit[g_iSrvCur])
  {
    // setup dplay stuff
    hr = CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IDirectPlay3A, (void**)&pDirectPlay3A[g_iSrvCur]);
    if (FAILED(hr))
    {
      MessageBox(NULL, "DPlay5+ is not properly installed. (IID_IDirectPlay3A)", "FSMon", MB_OK);
      exit(1);
    }

    // Create an IDirectPlayLobby interface, just so I can create an address. 
    hr = DirectPlayLobbyCreate(NULL, &pDirectPlayLobbyA, NULL, NULL, 0);
    if (FAILED(hr))
    {
      MessageBox(NULL, "DPlay5 is not properly installed. (DirectPlayLobbyCreate)", "FSMon", MB_OK);
      exit(1);
    }
    
    void * pvAddress = NULL; // dplay address of server
    // Create an IDirectPlay3 interface
  
    DWORD dwAddressSize;
  
    // Find out how big the address buffer needs to be--intentional fail 1st time
    hr = pDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, rgszServer[g_iSrvCur], 
        lstrlen(rgszServer[g_iSrvCur]) + 1,  pvAddress, &dwAddressSize);
    if (DPERR_BUFFERTOOSMALL != hr)
    {
      MessageBox(NULL, "DPlay5 is not properly installed. (CreateAddress)", "FSMon", MB_OK);
      exit(1);
    }

    pvAddress = GlobalAllocPtr(GMEM_MOVEABLE, dwAddressSize);
    hr = pDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, rgszServer[g_iSrvCur], 
        lstrlen(rgszServer[g_iSrvCur]) + 1, pvAddress, &dwAddressSize);
    pDirectPlayLobbyA->Release();
    pDirectPlayLobbyA = NULL;
    if (FAILED(hr))
    {
      MessageBox(NULL, "DPlay5 is not properly installed. (CreateAddress)", "FSMon", MB_OK);
      exit(1);
    }
    hr = pDirectPlay3A[g_iSrvCur]->InitializeConnection(pvAddress, 0);
    GlobalFreePtr(pvAddress);
    if (FAILED(hr))
    {
      gszMsg = "Can't initialize connection to specified machine";
      wsprintf(g_rgszDesc[g_iSrvCur], "Can't initialize connection", (int) (gdwCurrentPlayers[g_iSrvCur]));
      goto CLEANUP;
    }
    fDPInit[g_iSrvCur] = true;
  }

  {
    // try and find the session
    bool fFoundOne = false;
    DPSESSIONDESC2 sessionDesc;
    ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
    sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
    sessionDesc.guidApplication = FEDSRV_GUID;
    hr = pDirectPlay3A[g_iSrvCur]->EnumSessions(&sessionDesc, 2000, EnumSessionsCallback2, &fFoundOne, 0); //DPENUMSESSIONS_ASYNC);
    if ((FAILED(hr) || !fFoundOne) && (gState[g_iSrvCur] != FS_DOWN))
    {
      gState[g_iSrvCur] = FS_DOWN;
      gdwCurrentPlayers[g_iSrvCur] = 0;
      wsprintf(g_rgszDesc[g_iSrvCur], "DOWN", (int) (gdwCurrentPlayers[g_iSrvCur]));
      goto CLEANUP;
    }
  }

CLEANUP:  
  // save connection, so we can use it over and over
  if (!fDPInit[g_iSrvCur]) 
  {
    if (pDirectPlay3A[g_iSrvCur])
    {
      pDirectPlay3A[g_iSrvCur]->Release();
      pDirectPlay3A[g_iSrvCur] = NULL;
    }
  }
}


void UpdateMessage()
{
  int cUp = 0, cDown = 0, cInit = 0;
  for (int iSrv = 0; iSrv < g_cSrv; iSrv++)
  {
    if (gState[iSrv] == FS_UP)
      cUp++;
    else if (gState[iSrv] == FS_DOWN)
      cDown++;
    else if (gState[iSrv] == FS_INIT)
      cInit++;
  }
  
  nid.hIcon = cInit > 0 ? hIconInit : 
              cUp   > 0 ? (cDown > 0 ? hIconBoth : hIconUp) : hIconDown;
  Shell_NotifyIcon(NIM_MODIFY, &nid);
}


/****************************************************************************\
*
*    FUNCTION: MainWndProc
*
*    PURPOSE:  Processes notification messages
*
*\***************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_MOUSEINTRAY:
    {
      if (WM_LBUTTONDBLCLK == (UINT) lParam)
      {
        if (IDYES == MessageBox(NULL, "Are you sure you want to cancel all server monitoring and kill FSMon?",
                                "FSMon", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL))
        {
          PostQuitMessage(0);
        }

        //g_iSrvDlg = wParam;
        //DialogBox(hInst, MAKEINTRESOURCE(IDD_DLG_PROPS), NULL, (DLGPROC) DlgProcSettings);
      }

      else if (WM_LBUTTONDOWN == (UINT) lParam)
      {
        char szMsg[5000];
        char szTitle[100];
        int cPlayers = 0;
        int cServersUp = 0;
        int cServersDown = 0;
        szMsg[0] = 0;
        for (int iSrv = 0; iSrv < g_cSrv; iSrv++)
        {
          lstrcat(szMsg, rgszServer[iSrv]); 
          lstrcat(szMsg, ": "); 
          lstrcat(szMsg, g_rgszDesc[iSrv]); 
          lstrcat(szMsg, "\n");
          cPlayers+=gdwCurrentPlayers[iSrv];
          if (gState[iSrv] == FS_UP)
            cServersUp++;
          if (gState[iSrv] == FS_DOWN)
            cServersDown++;
        }
        wsprintf(szTitle, "FSMon: %d players on %d servers. %d servers down.",
          cPlayers, cServersUp, cServersDown);
        MessageBox(NULL, szMsg, szTitle, MB_OK | MB_SYSTEMMODAL);
      }
      break;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }
    case WM_TIMER:
    {
      for (g_iSrvCur = 0; g_iSrvCur < g_cSrv; g_iSrvCur++)
      {
        CheckConnection();
        UpdateMessage();
      }
      
      break;
    }
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}



/****************************************************************************
*
*    FUNCTION: WinMain
*
*    PURPOSE: calls initialization function, processes message loop
*
*\***************************************************************************/

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
  MSG msg;
  WNDCLASSEX wcx;
  HRESULT hr = CoInitialize(NULL);

  wcx.cbSize = sizeof(wcx);
  wcx.style = 0;
  wcx.lpfnWndProc = MainWndProc;
  wcx.cbClsExtra = 0;
  wcx.cbWndExtra = 0;
  wcx.hInstance = hInst = hInstance;
  wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FSMON));
  wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcx.lpszMenuName = 0;
  wcx.lpszClassName = SzAppName;
  wcx.hIconSm = NULL;

  RegisterClassEx(&wcx);

  // Create an invisible window to receive notification messages
  hWnd = CreateWindow(SzAppName, "FedSrv Monitor", WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

  hIconUp    = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_FS_RUNNING),     IMAGE_ICON, 16, 16, 0);
  hIconDown  = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_FS_STOPPED),     IMAGE_ICON, 16, 16, 0);
  hIconBoth  = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_FSMON),          IMAGE_ICON, 16, 16, 0);
  hIconInit  = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_FS_CONNECTING),  IMAGE_ICON, 16, 16, 0);
    
  nid.cbSize = sizeof(nid); 
  nid.hWnd = hWnd; 
  nid.uID = 0; 
  nid.uFlags = NIF_ICON | NIF_MESSAGE;
  nid.uCallbackMessage = WM_MOUSEINTRAY; 
  nid.hIcon = hIconInit; 

  if (!Shell_NotifyIcon(NIM_ADD, &nid))
  {
      MessageBox(hWnd, "Couldn't put FSMon Icon into the taskbar\n\n\tAborting", 
          "FSMon: FATAL Error", MB_OK | MB_ICONEXCLAMATION);
      Shell_NotifyIcon(NIM_DELETE, &nid); // just in case it's somehow there anyway
      exit(1);
  }

  // Now let's see what other servers they want to monitor
  bool fMore = true;
  for (; *szCmdLine; fMore)
  {
    // skip any extraneous spaces
    while (*szCmdLine && *szCmdLine == ' ')
      szCmdLine++;

    // look for end of token
    char * szTokenStart = szCmdLine;
    while (*szCmdLine && *szCmdLine != ' ')
      szCmdLine++;

    fMore = !!*szCmdLine;
      
    if (fMore) 
      *szCmdLine++ = '\0'; // NULL terminate the string, and skip space

    if (szTokenStart != szCmdLine)
    {
      // make sure it's not a dupe
      int iSrv ;
      for (iSrv = 0; iSrv < g_cSrv && stricmp(rgszServer[iSrv], szTokenStart); iSrv++)
        ;
      if (iSrv == g_cSrv) // we have a winner
      {
        lstrcpy(rgszServer[g_cSrv], szTokenStart);
        gState[g_cSrv] = FS_INIT;
        gdwCurrentPlayers[g_cSrv] = 0;
        fDPInit[g_cSrv] = false;
        pDirectPlay3A[g_cSrv] = NULL;
        g_cSrv++;
      }
    }
  }

  if (secInterval < 3 * g_cSrv)
    secInterval = 3 * g_cSrv;
  
  for (g_iSrvCur = 0; g_iSrvCur < g_cSrv; g_iSrvCur++)
  {
    CheckConnection();
  }
  UpdateMessage();
  SetTimer(hWnd, 1, 1000 * secInterval, NULL);      
  
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  Shell_NotifyIcon(NIM_DELETE, &nid);

  for (g_iSrvCur = 0; g_iSrvCur < g_cSrv; g_iSrvCur++)
  {
    if (!fDPInit[g_iSrvCur]) 
    {
      if (pDirectPlay3A[g_iSrvCur])
      {
        pDirectPlay3A[g_iSrvCur]->Release();
        pDirectPlay3A[g_iSrvCur] = NULL;
      }
    }
  }

  return (msg.wParam);
}
