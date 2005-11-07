/*===========================================================================*\
|
|  File:        NetworkGame.h
|
|  Description: 
|        Header file for NetworkGame.cpp
|       
|-----------------------------------------------------------------------------
|
|  Copyright (C) 1995-1996 Microsoft Corporation.  All Rights Reserved.
|
\*===========================================================================*/

#ifndef _CGREMOTE_H
#define _CGREMOTE_H

//** private data **

#define MAX_NAME_LENGTH 21
#define MAX_PLAYERS		500

DWORD WINAPI ReceiveThread(LPVOID pThreadParameter);

struct PLAYERSDESC 
{
  DPID dpid;
  char FriendlyName[MAX_NAME_LENGTH];
  char FormalName[MAX_NAME_LENGTH];
};


class NetworkGame
{
    private:
        IDirectPlayX * mpDirectPlay;
        HANDLE          mhPlayerEvent;
        DPID            mdpId;

        INT             mcMessagesSent;
        INT             mcMessagesReceived;

        INT             mcPlayers;
        PLAYERSDESC     mPlayers[MAX_PLAYERS];

#ifdef RECEIVETHREAD
        HANDLE          mhReceiveThread;
        HANDLE          mhKillReceiveEvent;
#endif

    public:
        FedMessaging    FM;

    private:
        void ShutdownConnection();
        void HandleSysMsg(DPID idTo, DPID idFrom);

    public:
        NetworkGame();
        ~NetworkGame();

        HRESULT SetupConnection(const char * szFedServer,
                                const char * szCharName,
                                const char * szCharPW,
                                bool fCreateNew,
                                FILETIME * pftLastArtUpdate = NULL);
        HRESULT ReceiveMessages(BOOL * pfSystemMsg);
        HRESULT SendMessages(DPID dpidTo, bool fGuaranteed);
        LPSTR GetPlayerName(int i);
        DPID DpId();
} ;


class DPlayWrapper
{
    protected:
        IDirectPlayX * mpDirectPlay;
        IDirectPlayLobbyX * mpDirectPlayLobbyA;
        bool CheckDPlayVersion();
        
    public:
        DPlayWrapper();
        ~DPlayWrapper();

        HRESULT AllocDirectPlaySession(const CHAR * szServer,
                                        IDirectPlayX ** ppDirectPlay);
        VOID FreeDirectPlaySession(IDirectPlayX * pDirectPlay);
} ;


#endif

