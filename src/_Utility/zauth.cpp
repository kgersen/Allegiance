/*-------------------------------------------------------------------------
  zauth.cpp
  
  implementation of IZoneAuthClient and IZoneAuthServer
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "zauth.h"

const int cTokensMax = 500;

class CZoneAuthServer : public IZoneAuthServer
{
public:
  CZoneAuthServer() :
    m_pzt(NULL),
    m_fInit(false)
  {
  }

  HRESULT Init()
  {
    assert(!m_fInit);
    HRESULT hr = CoCreateInstance( CLSID_ZoneAuthDecrypt, NULL, CLSCTX_INPROC_SERVER, 
                                    IID_IZoneAuthDecrypt, (LPVOID*)&m_pzad);
    if (SUCCEEDED(hr))
      hr = CoCreateInstance(CLSID_ZoneAuth, NULL, CLSCTX_INPROC_SERVER, IID_IZoneAuth, (LPVOID*)&m_pZoneAuth);
    ZSucceeded(hr);

    m_ztkey.cbSize = sizeof(m_ztkey);
    m_ztkey.nVersion = ZONETICKET_CURRENT_VERSION;
    if (SUCCEEDED(hr))
      hr = m_pzad->GenerateKey(OBLIVION_ZONE_KEY, &m_ztkey);
    ZSucceeded(hr);

    m_fInit = SUCCEEDED(hr);
    return hr;
  }


/*-------------------------------------------------------------------------
 * CZoneAuthServer.DecryptTicket
 *-------------------------------------------------------------------------
  Purpose:
      Crack the ticket the client passes in
 */
  HRESULT DecryptTicket(LPBYTE pbTicket, DWORD cbTicket)
  {
    assert(m_fInit);
    DWORD cbZT = sizeof(m_rgchZT);
    m_pzt = (ZONETICKET *) m_rgchZT;
    HRESULT hr = m_pzad->DecryptTicket(&m_ztkey, pbTicket, cbTicket, m_pzt, &cbZT);
    assert(IMPLIES(FAILED(hr), ZT_E_AUTH_INVALID_TICKET)); // this is the only failure we should be getting
    if (SUCCEEDED(hr))
    {
      assert (ZONETICKET_CURRENT_VERSION == m_pzt->nVersion);
      assert(sizeof(*m_pzt) <= m_pzt->cbSize);
    }
    return hr;
  }


 /*-------------------------------------------------------------------------
  * HasToken
  *-------------------------------------------------------------------------
    Purpose:
        Find out whether a certain token set has a desired token, 
        AND whether it's valid now

    Returns just whether the token EXISTS. You should not consider the account active
        unless they have the token AND it's valid now. Use the combination in order 
        to give a useful message
  */
  bool HasToken(LPSTR szToken, bool * pfValidNow)
  {
    assert(m_fInit);
    assert(szToken);

    if (pfValidNow)
      *pfValidNow = false;

    if (!m_pzt)
      return false;

    if (!*szToken) // If they just want a blank token, then everyone has that.
    {
      *pfValidNow = true;
      return true;
    }

    for(DWORD i = 0; i < m_pzt->nTokens; i++)
      if (!lstrcmpi(szToken, m_pzt->token_list[i].szToken))
      {
        time_t timeNow;
        time(&timeNow);
        double dSec = difftime(timeNow, m_pzt->tTicketIssued);
        *pfValidNow = // do I need this??? dSec > m_pzt->token_list[i].secTokenStart &&
                      dSec < m_pzt->token_list[i].secTokenEnd;
        return true;
      }

    return false;
  }

  LPCSTR GetName()
  {
    assert(m_fInit);
    return m_pzt ? m_pzt->szUserName : NULL;
  }

  LPCSTR GetAuthServer()
  {
    assert(m_fInit);
    return m_pzt ? m_pzt->szZoneServer : NULL;
  }

  unsigned long GetAccountID()
  {
    assert(m_fInit);
    return m_pzt ? m_pzt->nAccountID : NULL;
  }

private:
  TRef<IZoneAuthDecrypt> m_pzad;
  ZONETICKET_KEY         m_ztkey;
  TRef<IZoneAuth>        m_pZoneAuth;
  BYTE                   m_rgchZT[sizeof(ZONETICKET) + cTokensMax * sizeof(ZONETICKET_TOKEN)];
  ZONETICKET *           m_pzt;
  bool                   m_fInit;
};


class CZoneAuthClient : public IZoneAuthClient
{
public:
  CZoneAuthClient() :
    m_strAuthServer("auth.zone.com"),
    m_fInit(false)
  {
  }

  HRESULT Init()
  {
    assert(!m_fInit);
    HRESULT hr  = CoInitialize(NULL); // we ALREADY DID THIS, but on RBonny's machine, something has uninitialized us, so try again
    if (SUCCEEDED(hr))
      hr = CoCreateInstance( CLSID_ZoneAuth, NULL, CLSCTX_INPROC_SERVER, IID_IZoneAuth, (LPVOID*)&m_pZoneAuth);
    ZSucceeded(hr);
    m_fInit = SUCCEEDED(hr);
    return hr;
  }

  HRESULT  IsAuthenticated(DWORD msTimeout)
  {
    assert(m_fInit);
    if (!m_pZoneAuth)
    {
      return E_FAIL;
    }

    // TODO: Get the server name from somewhere
    HANDLE hFinished = CreateEvent(NULL, FALSE, FALSE, NULL);
    HRESULT hr = m_pZoneAuth->ReAuthenticate(m_strAuthServer, OBLIVION_TOKEN_GROUP, //$ CRASHGUARD
            FEDSRV_GUID, hFinished, &m_pzas);
    if (SUCCEEDED(hr))
    {
      DWORD dwObj = WaitForSingleObject(hFinished, INFINITE); // msTimeout);
      if (WAIT_OBJECT_0 == dwObj)
      {
        IZoneAuthSession::STATUS status;
        m_pzas->GetStatus(&status);
        if (IZoneAuthSession::STATUS_COMPLETED != status)
          hr = m_pzas->GetError();
      }
      else
      {
        m_pzas->Cancel();
        hr = ZT_E_AUTH_TIMEOUT; // it's not that the zone timed out, it's that we timed out wrt the specified timeout
      }
    }
    else
      m_pzas = NULL;

    CloseHandle(hFinished);
    if (ZT_NO_ERROR == hr) // hopefully the compiler will optimize this out as long as ZT_NO_ERROR == S_OK
      hr = S_OK;
    return hr;
  }
    
  HRESULT Authenticate(LPSTR szName, LPSTR szPW, bool fPWChanged, BOOL fRememberPW, DWORD msTimeout)
  {
    assert(m_fInit);
    assert (m_pZoneAuth);

  // TODO: Get the server name from somewhere
    HANDLE hFinished = CreateEvent(NULL, FALSE, FALSE, NULL);
    HRESULT hr = m_pZoneAuth->Authenticate(m_strAuthServer, OBLIVION_TOKEN_GROUP, 
            FEDSRV_GUID, szName, szPW, fPWChanged, fRememberPW, hFinished, &m_pzas);
    if (SUCCEEDED(hr))
    {
      // The cancel thing doesn't work without having to wait again, so that's pointless. Just wait now.
      DWORD dwObj = WaitForSingleObject(hFinished, INFINITE); //msTimeout);
      if (WAIT_OBJECT_0 == dwObj)
      {
        IZoneAuthSession::STATUS status;
        hr = m_pzas->GetStatus(&status);
        if (IZoneAuthSession::STATUS_COMPLETED != status)
          hr = m_pzas->GetError();
      }
      else
      {
        m_pzas->Cancel();
        DWORD dwObj = WaitForSingleObject(hFinished, INFINITE);
        hr = ZT_E_AUTH_TIMEOUT; // it's not that the zone timed out, it's that we timed out wrt the specified timeout
      }
    }
    else
      m_pzas = NULL;

    CloseHandle(hFinished);
    return hr;
  }
    
  void GetDefaultLogonInfo(OUT char * szName, OUT char * szPW, OUT BOOL * pfRememberPW)
  {
    assert(m_fInit);
    ZeroMemory(szName, c_cbName);
    ZeroMemory(szPW, c_cbName);
    HRESULT hr = m_pZoneAuth->GetLoginInfo(szName, c_cbName, szPW, c_cbName, pfRememberPW);
    if (ZT_E_AUTH_NO_LOGIN_INFO == hr)
    {
      *szName = 0;
      *szPW = 0;
      *pfRememberPW = false;
    }
    else
      ZSucceeded(hr);
  }
    
  HRESULT  GetTicket(OUT LPBYTE * ppBuffer, OUT DWORD * pcbBuffer, OUT LPSTR szLogin, IN OUT DWORD * pcbLogin)
  {
    assert(m_fInit);
    assert(m_pzas);
    assert(pcbBuffer);

    // Call GetTicket first just to get the buffer sizes
    char chStupid; // I have to pass a valid buffer just to find out that
                   // it's too small and find out the real size it needs to be. I reported this as a bug,
                   // x fixed it, then somebody else unfixed it.
    *pcbBuffer = sizeof(chStupid);
    HRESULT hr = m_pzas->GetTicket((BYTE*)&chStupid, pcbBuffer, NULL, NULL, NULL, NULL); //$ CRASHGUARD -- just find out the size 
    
    if (FAILED(hr) && ZT_E_BUFFER_TOO_SMALL != hr)
      return hr;
      
    *ppBuffer = static_cast<LPBYTE>(HeapAlloc(GetProcessHeap(), 0, *pcbBuffer));
    assert (*ppBuffer);

    ZSucceeded(m_pzas->GetTicket(*ppBuffer, pcbBuffer, szLogin, pcbLogin, NULL, NULL));
    return ZT_NO_ERROR;
  }

  ZString & GetAuthServer()
  {
    assert(m_fInit);
    return m_strAuthServer;
  }

  void SetAuthServer(ZString strAuthServer)
  {
    assert(m_fInit);
    m_strAuthServer = strAuthServer;
  }

  bool HasInterface(REFIID iid)
  {
    assert(m_pZoneAuth);

    TRef<IUnknown> punk = NULL;
    HRESULT hr = m_pZoneAuth->QueryInterface(iid, (void**) &punk);
    return SUCCEEDED(hr);
  }
  
private:
  ~CZoneAuthClient() {} //$ CRASHGUARD
  
  TRef<IZoneAuth>        m_pZoneAuth;
  TRef<IZoneAuthSession> m_pzas;
  ZString                m_strAuthServer;
  bool                   m_fInit;
};

TRef<IZoneAuthClient> CreateZoneAuthClient()
{
  TRef<CZoneAuthClient> pzac = new CZoneAuthClient();
  assert(pzac);
  if (pzac && FAILED(pzac->Init()))
    pzac = NULL;

  return pzac;
}

TRef<IZoneAuthServer> CreateZoneAuthServer()
{
  TRef<CZoneAuthServer> pzas = new CZoneAuthServer();
  assert(pzas);
  if (pzas && FAILED(pzas->Init()))
    pzas = NULL;

  return pzas;
}

