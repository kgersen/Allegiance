#ifndef _DPLAYCHK_H
#define _DPLAYCHK_H

class CDplayServerChecker
{
protected:
    LPSTR m_pszServer;
    BOOL  m_fDPInit;
    IDirectPlay3A *m_pDirectPlay3A;
    IDirectPlayLobby *m_pDirectPlayLobbyA;
public:
    LPSTR m_szMsg;

    CDplayServerChecker();
    virtual ~CDplayServerChecker();

    void SetServer(LPSTR szServer);
    void ClearServer();
    BOOL ServerConnectionAlive();
};


#endif
