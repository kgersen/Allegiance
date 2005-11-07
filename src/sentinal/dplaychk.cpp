#include <windows.h>
#include <windowsx.h>
#include <dplobby.h>
#include "dplaychk.h"

// {81662310-FCB4-11d0-A88A-006097B58FBF}
DEFINE_GUID(FEDSRV_GUID, 
0x81662310, 0xfcb4, 0x11d0, 0xa8, 0x8a, 0x0, 0x60, 0x97, 0xb5, 0x8f, 0xbf);

CDplayServerChecker::CDplayServerChecker()
    : m_pszServer(NULL), m_fDPInit(FALSE), m_pDirectPlay3A(NULL), m_szMsg(NULL),
    m_pDirectPlayLobbyA(NULL)
{
}

CDplayServerChecker::~CDplayServerChecker()
{
    if (!m_fDPInit) 
    {
        if (m_pDirectPlay3A)
        {
            m_pDirectPlay3A->Release();
            m_pDirectPlay3A = NULL;
        }
    }
}

void CDplayServerChecker::ClearServer()
{
    // delete the server id string
    if (m_pszServer)
    {
        delete m_pszServer;
        m_pszServer = NULL;
    }

    // free the server connection
    if (!m_fDPInit) 
    {
        if (m_pDirectPlay3A)
        {
            m_pDirectPlay3A->Release();
            m_pDirectPlay3A = NULL;
        }
    }
}

void CDplayServerChecker::SetServer(LPSTR szServer)
{
    // if we had a server before, clear it
    ClearServer();

    m_pszServer = new char[strlen(szServer)+1];
    lstrcpy(m_pszServer, szServer);
}

//---------------------------------------------------------------------------
//
//    FUNCTION:  EnumSessionsCallback2
//
//    PURPOSE:  Called by IDirectPlay3A::EnumSessions. 
//
//---------------------------------------------------------------------------
BOOL FAR PASCAL EnumSessionsCallback2(LPCDPSESSIONDESC2 lpThisSD,
                                      LPDWORD lpdwTimeOut,
                                      DWORD dwFlags,
                                      LPVOID lpContext)
{
    // We're not actually going to open this session, so let's just remember that we have one
    if (dwFlags & DPESC_TIMEDOUT)
		return FALSE;
  
    if (lpThisSD) // will be null if we timed out
        *(BOOL *)lpContext = TRUE;

    return FALSE;
}

BOOL CDplayServerChecker::ServerConnectionAlive()
{
    HRESULT hr = E_FAIL;
    BOOL rc = FALSE;

    if (!m_fDPInit)
    {
        // setup dplay stuff
        hr = CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IDirectPlay3A, (void**)&m_pDirectPlay3A);
        if (FAILED(hr))
        {
            m_szMsg = "DPlay5 is not properly installed. (IID_IDirectPlay3A)";
            goto CLEANUP;
        }

        // Create an IDirectPlayLobby interface, just so I can create an address. 
        hr = DirectPlayLobbyCreate(NULL, &m_pDirectPlayLobbyA, NULL, NULL, 0);
        if (FAILED(hr))
        {
            m_szMsg = "DPlay5 is not properly installed. (DirectPlayLobbyCreate)";
            goto CLEANUP;
        }
    
        void * pvAddress = NULL; // dplay address of server
        // Create an IDirectPlay3 interface
  
        DWORD dwAddressSize;
  
        // Find out how big the address buffer needs to be--intentional fail 1st time
        hr = m_pDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, m_pszServer, 
                                    lstrlen(m_pszServer) + 1,  pvAddress, &dwAddressSize);
        if (DPERR_BUFFERTOOSMALL != hr)
        {
            m_szMsg = "DPlay5 is not properly installed. (CreateAddress)";
            goto CLEANUP;
        }

        pvAddress = GlobalAllocPtr(GMEM_MOVEABLE, dwAddressSize);
        hr = m_pDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, m_pszServer, 
                                    lstrlen(m_pszServer) + 1, pvAddress, &dwAddressSize);
        m_pDirectPlayLobbyA->Release();
        m_pDirectPlayLobbyA = NULL;
        if (FAILED(hr))
        {
            m_szMsg = "DPlay5 is not properly installed. (CreateAddress)";
            goto CLEANUP;
        }

        hr = m_pDirectPlay3A->InitializeConnection(pvAddress, 0);
        GlobalFreePtr(pvAddress);
        if (FAILED(hr))
        {
            m_szMsg = "Can't initialize connection to specified machine";
            goto CLEANUP;
        }
        m_fDPInit = true;
    }

    {
        // try and find the session
        bool fFoundOne = false;
        DPSESSIONDESC2 sessionDesc;
        ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
        sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
        sessionDesc.guidApplication = FEDSRV_GUID;
        hr = m_pDirectPlay3A->EnumSessions(&sessionDesc, 2000, EnumSessionsCallback2, &fFoundOne, 0); //DPENUMSESSIONS_ASYNC);
        if (FAILED(hr) || !fFoundOne)
        {
            m_szMsg = "Down for the count";
            goto CLEANUP;
        }
    }

    rc = TRUE;

CLEANUP:  
    // save connection, so we can use it over and over
    if (!m_fDPInit) 
    {
        if (m_pDirectPlay3A)
        {
            m_pDirectPlay3A->Release();
            m_pDirectPlay3A = NULL;
        }
    }
    return rc;
}

