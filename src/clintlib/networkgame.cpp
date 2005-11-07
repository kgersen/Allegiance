/****************************************************************************
* Copyright (C) 1995-1996 Microsoft Corporation.  All Rights Reserved.
*
* File:         NetworkGame.cpp
*
* Author:       Curt Carpenter
*
* Description:  DPlay interface for networking stuff
*      
******************************************************************************/

#include "pch.h"
#include <dplobby.h>

#define CURRENT_DPLAY_VER 318

//
// Variables global to this module.
//
DPlayWrapper mDPlayWrap;
BOOL mfMultiplex = FALSE;


//
// NetworkGame
//
// Basic constructor for a network game.
NetworkGame::NetworkGame()
{
    mpDirectPlay = NULL;
    mhPlayerEvent = NULL;
    mdpId = 0;

    mcMessagesSent = 0;
    mcMessagesReceived = 0;

    mcPlayers = 0;

#ifdef RECEIVETHREAD  
    mhReceiveThread = NULL;
    mhKillReceiveEvent = NULL;
#endif
}

// ~NetworkGame
//  
// Destructor.  Releases all DirectPlay state.
//
NetworkGame::~NetworkGame() 
{
    debugf("NWG:~NetworkGame()\n");

    this->ShutdownConnection();
}


/****************************************************************************\
*
*    FUNCTION:  EnumSessionsCallback2
*
*    PURPOSE:  Called by IDirectPlay#::EnumSessions
*
*\***************************************************************************/
BOOL FAR PASCAL EnumSessionsCallback2(LPCDPSESSIONDESC2 lpThisSD,
                                      LPDWORD lpdwTimeOut,
                                      DWORD dwFlags,
                                      LPVOID lpContext)
{
  // lpContext is a pointer for where to stick the instance guid, which is all we want
  if (lpThisSD) // will be null if we timed out
    *(GUID*)lpContext = lpThisSD->guidInstance;
  return FALSE;
}


/****************************************************************************\
*
*    FUNCTION:  SetupConnection
*
*    PURPOSE:  sets up all the dplay stuff and puts us in a listen state
*
*\***************************************************************************/
HRESULT NetworkGame::SetupConnection(const char * szFedServer,
                                    const char * szCharName,
                                    const char * szCharPW,
                                    bool fCreateNew,
                                    FILETIME * pftLastArtUpdate)
{
    HRESULT hr = S_OK;
    char szPlayerName[30] = "/"; // we're going to start copying past the slash
    DWORD dwcbPlayerName = sizeof(szPlayerName) - 1;
    DWORD dwAddressSize = 0;
#ifdef RECEIVETHREAD  
    DWORD idReceiveThread   = 0; // id of receive thread
#endif


    //
    // Create an IDirectPlay3 session to the server.
    //
    hr = mDPlayWrap.AllocDirectPlaySession(szFedServer, &mpDirectPlay);
    if (SUCCEEDED(hr))
    {
        // Use character name / user name for player name
        GetUserName(szPlayerName + 1, &dwcbPlayerName);

        //
        // Create a player
        //
        DPNAME dpName;
        dpName.dwSize = sizeof(dpName);
        dpName.dwFlags = 0;
        dpName.lpszShortNameA = *(char**)&szCharName; // make compiler happy about const-ness
        dpName.lpszLongNameA = szPlayerName;
        hr = mpDirectPlay->CreatePlayer(&mdpId,
                                        &dpName,
                                        mhPlayerEvent,
                                        NULL,
                                        0,
                                        0);
        if (SUCCEEDED(hr))
        {
            //
            // Player on this machine always in first slot
            //
            mPlayers[0].dpid = mdpId;
            mPlayers[0].FormalName[0] = 0;
            strcpy(mPlayers[0].FriendlyName, szPlayerName);
            mcPlayers++;

            DPCAPS dpcaps;
            dpcaps.dwSize = sizeof(dpcaps);
            hr = mpDirectPlay->GetCaps(&dpcaps, 0);

            FM.Init(mpDirectPlay, mdpId);

            {
            }
        } else
            debugf("NWG: Failed to create user\n");
    } else
        debugf("NWG: Failed to obtain dplay session\n");

    if (FAILED(hr))
        this->ShutdownConnection();

    return(hr);
}


LPSTR NetworkGame::GetPlayerName(int i) 
{
    if ((i < 0) || (i >= mcPlayers)) 
        return NULL;
    return(&mPlayers[i].FriendlyName[0]);
}


HRESULT NetworkGame::SendMessages(DPID dpidTo, bool fGuaranteed)
{
    FM.SendMessages(dpidTo, fGuaranteed, NULL);
    return S_OK;
}

DPID NetworkGame::DpId()
{
    return(mdpId);
}
 

#ifdef RECEIVETHREAD  
/****************************************************************************\
*
*    FUNCTION:  ReceiveThread
*
*    PURPOSE:  Waits for messages in its own thread
*
*\***************************************************************************/
DWORD WINAPI ReceiveThread(LPVOID pNWG_This)
{
  //
  // BUGBUG: This won't work any more.  If we want to implement this, we
  // need access to the BasePlayerData so that we can call the callback.
  // If we decide to implement this, it should probably be in the
  // BasePlayerData class instead of here.
  //
  NetworkGame * pNetworkGame = (NetworkGame * ) pNWG_This;
  HANDLE eventHandles[2];
  BOOL fSysMsg;

  eventHandles[0] = pNetworkGame->mhPlayerEvent;
  eventHandles[1] = pNetworkGame->mhKillReceiveEvent;

  // loop waiting for player events. If the kill event is signaled
  // the thread will exit
  while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
  {
    // receive any messages in the queue
    pNetworkGame->ReceiveMessages(&fSysMsg);
  }

  ExitThread(0);

  return (0);
}
#endif // RECEIVETHREAD  


/****************************************************************************\
*
*    FUNCTION:  ReceiveMessages
*
*    PURPOSE:   Reads messages queued by DPlay
*
*    Returns:   S_OK if message needs processing, 
*               S_FALSE message doesn't need processing,
*               or failure code (including no more messages)
*
*\***************************************************************************/
HRESULT NetworkGame::ReceiveMessages(BOOL * pfSystemMsg)
{
    DPID    idFrom;
    HRESULT hr;

    *pfSystemMsg = FALSE;
    hr = S_FALSE;

    if (0 != mdpId)
    {
        idFrom = 0;

        hr = FM.ReceiveMessages(mdpId, &idFrom);
        if (SUCCEEDED(hr))
        {
            if (FM.PacketSize() >= sizeof(DPMSG_GENERIC))
            {
                mcMessagesReceived++;

                if (idFrom == DPID_SYSMSG)
                    *pfSystemMsg = TRUE;
            } else
                hr = S_FALSE;
        }
    }

    return (hr);
}


/****************************************************************************\
*
*    FUNCTION:  ShutdownConnection
*
*    PURPOSE:  Shut everything down and go home
*
*\***************************************************************************/
void NetworkGame::ShutdownConnection()
{
#ifdef RECEIVETHREAD  
  if (mhReceiveThread)
  {
    // wake up receive thread and wait for it to quit
    SetEvent(mhKillReceiveEvent);
    WaitForSingleObject(mhReceiveThread, INFINITE);

    CloseHandle(mhReceiveThread);
    mhReceiveThread = NULL;
  }

  if (mhKillReceiveEvent)
  {
    CloseHandle(mhKillReceiveEvent);
    mhKillReceiveEvent = NULL;
  }
#endif // RECEIVETHREAD  

  mDPlayWrap.FreeDirectPlaySession(mpDirectPlay);
  mpDirectPlay = NULL;

#ifdef RECEIVETHREAD  
  if (mhPlayerEvent)
  {
    CloseHandle(mhPlayerEvent);
    mhPlayerEvent = NULL;
  }
#endif // RECEIVETHREAD  
}

/****************************************************************************\
*
*    FUNCTION:  HandleSysMsg
*
*    PURPOSE:  Handle system message
*
*\***************************************************************************/
void NetworkGame::HandleSysMsg(DPID idTo, DPID idFrom)
{
    // The body of each case is there so you can set a breakpoint and examine
    // the contents of the message received.
  LPDPMSG_GENERIC pMsg = (LPDPMSG_GENERIC) FM.BuffIn();
  debugf("NWG: HandleSysMsg from %lu to %lu, dwType %lu\n", idFrom, idTo, pMsg->dwType);
  switch (pMsg->dwType)
  {
    case DPSYS_CREATEPLAYERORGROUP:
    {
      // should never get this
    }
    break;

    case DPSYS_DESTROYPLAYERORGROUP:
    {
      LPDPMSG_DESTROYPLAYERORGROUP lp = (LPDPMSG_DESTROYPLAYERORGROUP) pMsg;
    }
    break;

    case DPSYS_ADDPLAYERTOGROUP:
    {
      LPDPMSG_ADDPLAYERTOGROUP lp = (LPDPMSG_ADDPLAYERTOGROUP) pMsg;
    }
    break;

    case DPSYS_DELETEPLAYERFROMGROUP:
    {
      LPDPMSG_DELETEPLAYERFROMGROUP lp = (LPDPMSG_DELETEPLAYERFROMGROUP) pMsg;
    }
    break;

    case DPSYS_SESSIONLOST:
    {
      LPDPMSG_SESSIONLOST lp = (LPDPMSG_SESSIONLOST) pMsg;
    }
    break;

    case DPSYS_HOST:
    {
      LPDPMSG_HOST lp = (LPDPMSG_HOST) pMsg;
    }
    break;

    case DPSYS_SETPLAYERORGROUPDATA:
    {
      LPDPMSG_SETPLAYERORGROUPDATA lp = (LPDPMSG_SETPLAYERORGROUPDATA) pMsg;
      assert(false);
    }
    break;

    case DPSYS_SETPLAYERORGROUPNAME:
    {
      LPDPMSG_SETPLAYERORGROUPNAME lp = (LPDPMSG_SETPLAYERORGROUPNAME) pMsg;
      assert(false);
    }
    break;

    case DPSYS_SECUREMESSAGE:
    {
      LPDPMSG_SECUREMESSAGE lp = (LPDPMSG_SECUREMESSAGE) pMsg;
      assert(false);
    }
    break;
  }
}


//
// We really want to use the same IDirectPlay pointer in all of
// our network game code (DPlay creates 3 threads per IDirectPlay).
// To do this, I've created these static functions to handle the
// initialization and termination of the one DPlay pointer.
//
DPlayWrapper::DPlayWrapper()
{
    mpDirectPlay = NULL;
    mpDirectPlayLobbyA = NULL;
}


DPlayWrapper::~DPlayWrapper()
{
    ZAssert(NULL == mpDirectPlay);
    if (NULL != mpDirectPlayLobbyA)
        mpDirectPlayLobbyA->Release();
}

HRESULT DPlayWrapper::AllocDirectPlaySession(const CHAR * szServer,
                                            IDirectPlayX ** ppDirectPlay)
{
    BOOL fCreateSession;
    VOID * pvAddress;
    DWORD cbAddress;
    HRESULT hr;

    hr = S_OK;
    fCreateSession = FALSE;

    if (!CheckDPlayVersion())
        hr = E_FAIL;
    
    //
    // Create a lobby if we don't already have one.
    //
    if (SUCCEEDED(hr) && NULL == mpDirectPlayLobbyA)
    {
        hr = DirectPlayLobbyCreate(NULL,
                                    &mpDirectPlayLobbyA,
                                    NULL,
                                    NULL,
                                    0);
    }

    //
    // Create a directplay session if need be.
    //
    if (SUCCEEDED(hr) && (NULL == mpDirectPlay))
    {
        hr = CoCreateInstance(CLSID_DirectPlay,
                              NULL,
                              CLSCTX_INPROC_SERVER, 
                              IID_IDirectPlayX,
                              (VOID **) &mpDirectPlay);
        if (SUCCEEDED(hr))
            fCreateSession = TRUE;
    }

    if (TRUE == fCreateSession)
    {
        //
        // At this point I have a new directplay pointer and lobby pointer.
        //

        //
        // Need to find out how big of an address buffer we need, so
        // we intentionally fail the first time.
        //
        pvAddress = NULL;
        cbAddress = 0;
        hr = mpDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP,
                                                DPAID_INet,
                                                szServer, 
                                                strlen(szServer) + 1,
                                                pvAddress,
                                                &cbAddress);
        if (DPERR_BUFFERTOOSMALL == hr)
        {
            pvAddress = new BYTE[cbAddress];
            if (NULL != pvAddress)
            {
                hr = mpDirectPlayLobbyA->CreateAddress(DPSPGUID_TCPIP,
                                                        DPAID_INet,
                                                        szServer, 
                                                        strlen(szServer) + 1,
                                                        pvAddress,
                                                        &cbAddress);
            } else
                hr = E_OUTOFMEMORY;
        }

        if (SUCCEEDED(hr))
        {
            hr = mpDirectPlay->InitializeConnection(pvAddress, 0);

            if (SUCCEEDED(hr))
            {
                //
                // Find the session.
                //
                DPSESSIONDESC2 sessionDesc;
                GUID guidDPInstance, guidZero;

                ZeroMemory(&sessionDesc, sizeof(sessionDesc));
                sessionDesc.dwSize = sizeof(sessionDesc);
                sessionDesc.guidApplication = FEDSRV_GUID;
                ZeroMemory(&guidDPInstance, sizeof(guidDPInstance));
                ZeroMemory(&guidZero, sizeof(guidZero));

                hr = mpDirectPlay->EnumSessions(&sessionDesc,
                                                    5000,
                                                    EnumSessionsCallback2,
                                                    &guidDPInstance,
                                                    0);

                if (FAILED(hr) || (guidZero == guidDPInstance))
                {
                    //
                    // Maybe we hit a burp.  Try again.
                    //
                    hr = mpDirectPlay->EnumSessions(&sessionDesc,
                                                        5000,
                                                        EnumSessionsCallback2,
                                                        &guidDPInstance,
                                                        0);
                }

                if (guidZero != guidDPInstance)
                {
                    //
                    // Join the session.
                    //
                    sessionDesc.guidInstance = guidDPInstance;
                    hr = mpDirectPlay->SecureOpen(&sessionDesc,
                                                    DPOPEN_JOIN,
                                                    NULL,
                                                    NULL);
                } else
                    hr = E_FAIL;
            }
        }

        if (NULL != pvAddress)
            delete[] pvAddress;

        if (FAILED(hr))
        {
            mpDirectPlay->Release();
            mpDirectPlay = NULL;
        }
    }

    if (SUCCEEDED(hr))
    {
        *ppDirectPlay = mpDirectPlay;
        if (FALSE == mfMultiplex)
            mpDirectPlay = NULL;
        else
            mpDirectPlay->AddRef();
    } else
        *ppDirectPlay = NULL;

    return(hr);
}


VOID DPlayWrapper::FreeDirectPlaySession(IDirectPlayX * pDirectPlay)
{
    if (NULL != pDirectPlay)
    {
        if (FALSE == mfMultiplex)
        {
            //
            // Not multiplexing.  Shut the whole thing down.
            //
            pDirectPlay->Close();
            pDirectPlay->Release();
        } else if (1 == pDirectPlay->Release())
        {
            //
            // We own the last reference.  Shut down.
            //
            mpDirectPlay->Close();
            mpDirectPlay->Release();
            mpDirectPlay = NULL;
        }
    }
}



