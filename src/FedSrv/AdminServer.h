
/*-------------------------------------------------------------------------
 * fedsrv\AdminServer.CPP
 * 
 * Declaration of CAdminServer
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
//noagc
// we revive this class but as a regular class not a ATL/COM class
// eventually we could build an API (rest/graphql) on it for remote control

#ifndef __SERVER_H_
#define __SERVER_H_

/////////////////////////////////////////////////////////////////////////////
// CAdminServer
//
class CAdminServer  
  /*public IDispatchImpl<IAdminServer, &IID_IAdminServer, &LIBID_ALLEGIANCESERVERLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CAdminServer, &CLSID_AdminServer>*/
{
//// Declarations
//public:
//  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINSERVER)
//  DECLARE_PROTECT_FINAL_CONSTRUCT()
//
//// Interface Map
//public:
//  BEGIN_COM_MAP(CAdminServer)
//    COM_INTERFACE_ENTRY(IAdminServer)
//    COM_INTERFACE_ENTRY(IDispatch)
//    COM_INTERFACE_ENTRY(ISupportErrorInfo)
//  END_COM_MAP()
//
//// Category Map
//public:
//  BEGIN_CATEGORY_MAP(CAdminServer)
//    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
//  END_CATEGORY_MAP()
//
//// ISupportsErrorInfo Interface Methods
//public:
//  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
//
//// IAdminServer Interface Methods
public:
	//long get_Games(IAdminGames** ppAdminGames);
	//long get_PlayerCount(long *pVal);
	//long get_MissionCount(long *pVal);
	//long SendMsg(BSTR  bstrMessage);
	//long get_Users(IAdminUsers** ppAdminUsers);
	//long get_LookupUser(AGCUniqueID id, IAdminUser** ppUser);
	//long get_FindUser(BSTR bstrName, IAdminUser** ppUser);
	char *get_MachineName(char ** pbstrMachine);
	//long get_PacketsIn(long * pVal);
	//long get_PlayersOnline(long * pVal);
	//long get_TimeInnerLoop(long * pVal);
	char * put_LobbyServer(char * bstrLobbyServer);
	char * get_LobbyServer(char ** pbstrLobbyServer);
	char * put_PublicLobby(bool bPublic);
	char * get_PublicLobby(bool * pbPublic);
	char * CreateDefaultGames();
};


/////////////////////////////////////////////////////////////////////////////

#endif //__SERVER_H_
