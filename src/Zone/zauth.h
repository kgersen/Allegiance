/*-------------------------------------------------------------------------
  zauth.h
  
  Interfaces for zone authentication / authorization
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _ZAUTH_H_
#define _ZAUTH_H_

#include <zoneauth.h>

#define OBLIVION_TOKEN_GROUP "Allegiance"
#define OBLIVION_ZONE_KEY "Curt loves DPlay."

class IZoneAuthServer : public IObject
{
public:
  virtual HRESULT DecryptTicket(LPBYTE pbTicket, DWORD cbTicket) = 0;
  virtual bool    HasToken(LPSTR szToken, bool * pfValidNow) = 0;
  virtual LPCSTR  GetName() = 0;
  virtual LPCSTR  GetAuthServer() = 0;
  virtual unsigned long GetAccountID() = 0;
};

TRef<IZoneAuthServer> CreateZoneAuthServer();
  
class IZoneAuthClient : public IObject
{
public:
  virtual HRESULT  IsAuthenticated(DWORD msTimeout) = 0;
  virtual HRESULT  Authenticate(LPSTR szName, LPSTR szPW, bool fPWChanged, BOOL fRememberPW, DWORD msTimeout) = 0;
  virtual void     GetDefaultLogonInfo(OUT char * szName, OUT char * szPW, OUT BOOL * pfRememberPW) = 0;
  virtual HRESULT  GetTicket(OUT LPBYTE * ppBuffer, OUT DWORD * pcbBuffer, OUT LPSTR szLogin, IN OUT DWORD * pcbLogin) = 0;
  virtual ZString & GetAuthServer() = 0;
  virtual void     SetAuthServer(ZString strAuthServer) = 0;
  virtual bool     HasInterface(REFIID iid) = 0;
};

TRef<IZoneAuthClient> CreateZoneAuthClient();

#endif
