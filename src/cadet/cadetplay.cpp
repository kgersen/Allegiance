#include "pch.h"
//#include "dplay.h"
//#include "dplobby.h"

// {BF062CD6-D0EE-11d2-99B6-00E0291AB536}
static const GUID CLSID_CADET = 
    { 0xbf062cd6, 0xd0ee, 0x11d2, { 0x99, 0xb6, 0x0, 0xe0, 0x29, 0x1a, 0xb5, 0x36 } };

static const GUID GUID_NULL = 
    { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x0, 0x00, 0x00, 0x00, 0x00, 0x00 } };

static const DPCOMPORTADDRESS cDPCPA = 
    {
        #ifdef DREAMCAST
	        1,
            CBR_115200,
        #else
            4,
        	CBR_115200,
        #endif
	    ONESTOPBIT,
    	NOPARITY,
	    DPCPA_RTSFLOW,
    };

BOOL FAR PASCAL GetFirstSession(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext) 
{
	LPDPSESSIONDESC2 lpdpsd = (LPDPSESSIONDESC2)lpContext;
	
	if (dwFlags==DPESC_TIMEDOUT)
    	{ // No sessions found
		debugf("Session enumeration timed out\r\n");
		return (FALSE);
    	}

	memcpy(lpdpsd, lpThisSD, sizeof(DPSESSIONDESC2));

    #ifdef DREAMCAST
        int length = wcslen(lpThisSD->lpszSessionName) + 1;
        lpdpsd->lpszSessionName = new WCHAR[length];
        memcpy(lpdpsd->lpszSessionName, lpThisSD->lpszSessionName, length * 2);
    #else
        int length = strlen(lpThisSD->lpszSessionNameA) + 1;
        lpdpsd->lpszSessionNameA = new char[length];
        memcpy(lpdpsd->lpszSessionNameA, lpThisSD->lpszSessionNameA, length);
    #endif

	return (FALSE);
};

static TCHAR szPhone1[] = TEXT("101");
static TCHAR szPhone2[] = TEXT("202");

class CadetPlayImpl : public CadetPlay
{
private:
    DPID                m_dpid;
    TRef<IDirectPlay3>  m_pDirectPlay;
    TRef<IDirectPlayLobby2> m_pDirectPlayLobby;
    TRef<CadetPlaySite> m_pCadetPlaySite;
    CadetMode           m_cm;
    Time                m_timeLastUpdate;
    
public:
    CadetPlayImpl():
        m_cm(cmSinglePlayer)
    {
        m_timeLastUpdate = Time::Now();
    }

    void SetCadetPlaySite(CadetPlaySite* psite)
    {
        m_pCadetPlaySite = psite;
    }
  
    void SetCadetMode(CadetMode cm)
    {
        m_cm = cm;
    }
    
    virtual CadetMode GetCadetMode()
    {
        return m_cm;
    }
    
    TRef<IDirectPlay3> CreateDirectPlay()
    {
//!!!dc#ifdef DREAMCAST
//    	TRef<IUnknown>  pdplay;
//#else
    	TRef<IDirectPlay>  pdplay;
//#endif
    	TRef<IDirectPlay3> pdplay3;
        GUID guidNull = GUID_NULL;

    	if (FAILED(DirectPlayCreate(&guidNull, &pdplay, NULL)))
            return NULL;
            
        if (FAILED(pdplay->QueryInterface(IID_IDirectPlay3,(LPVOID*)&pdplay3)))
            return NULL;

		return pdplay3;
	}

    TRef<IDirectPlayLobby2> CreateDirectPlayLobby()
    {
    	TRef<IDirectPlayLobby>  plobby;
    	TRef<IDirectPlayLobby2> plobby2;
        if (FAILED(DirectPlayLobbyCreate(NULL,&plobby,NULL,NULL,0)))
            return NULL;
#ifdef UNICODE
        if (FAILED(plobby->QueryInterface(IID_IDirectPlayLobby2,(LPVOID*)&plobby2)))
            return NULL;
#else
        if (FAILED(plobby->QueryInterface(IID_IDirectPlayLobby2A,(LPVOID*)&plobby2)))
            return NULL;
#endif	

		return plobby2;
	}	


	HRESULT InitializeModemConnection() 
    {
    	DPCOMPOUNDADDRESSELEMENT dpcae[2];
	    DWORD dwConnBuffSize = 0;
        BYTE* pbuf = NULL;
	    HRESULT hr = S_OK;
	
    	// Load service provider chunk
    	dpcae[0].guidDataType=DPAID_ServiceProvider;
    	dpcae[0].dwDataSize=sizeof(GUID);
    	dpcae[0].lpData=(LPVOID)&DPSPGUID_MODEM;
        // and phone chunk
        dpcae[0].lpData = (GetCadetMode()==cmJoinModemNum1) ? (LPVOID)szPhone1 : (LPVOID)szPhone2;
#ifdef UNICODE
	    dpcae[1].guidDataType=DPAID_PhoneW;
#else
		dpcae[1].guidDataType=DPAID_Phone;
#endif
		dpcae[1].dwDataSize=sizeof(TCHAR)*(4);

        if (DPERR_BUFFERTOOSMALL != 
                (hr = m_pDirectPlayLobby->CreateCompoundAddress(
                    dpcae,2,NULL, &dwConnBuffSize)))
            {
            ZAssert(FAILED(hr));
            return hr;
            }

       	if (NULL == (pbuf = (BYTE*)LocalAlloc(0,dwConnBuffSize)))
    		return E_FAIL;

    	if (FAILED(hr = m_pDirectPlayLobby->CreateCompoundAddress(
    	            dpcae, 2, pbuf, &dwConnBuffSize)))
    		goto quit;

    	hr = m_pDirectPlay->InitializeConnection(pbuf,0);

    quit:
	    if (pbuf)
    		LocalFree(pbuf);
    	return hr;
    }


    HRESULT InitializeSerialConnection() 
    {
    	DPCOMPOUNDADDRESSELEMENT dpcae[2];
	    DWORD dwConnBuffSize = 0;
        BYTE* pbuf = NULL;
	    HRESULT hr = S_OK;

    	// Load service provider chunk
    	dpcae[0].guidDataType=DPAID_ServiceProvider;
    	dpcae[0].dwDataSize=sizeof(GUID);
    	dpcae[0].lpData=(LPVOID)&DPSPGUID_SERIAL;
        // and comport chunk
    	dpcae[1].guidDataType=DPAID_ComPort;
		dpcae[1].dwDataSize=sizeof(DPCOMPORTADDRESS);
    	dpcae[1].lpData=(LPVOID)&cDPCPA;

        if (DPERR_BUFFERTOOSMALL != 
                (hr = m_pDirectPlayLobby->CreateCompoundAddress(
                    dpcae,2,NULL, &dwConnBuffSize)))
            {
            ZAssert(FAILED(hr));
            return hr;
            }

       	if (NULL == (pbuf = (BYTE*)LocalAlloc(0,dwConnBuffSize)))
    		return E_FAIL;

    	if (FAILED(hr = m_pDirectPlayLobby->CreateCompoundAddress(
    	            dpcae, 2, pbuf, &dwConnBuffSize)))
    		goto quit;

    	hr = m_pDirectPlay->InitializeConnection(pbuf,0);

    quit:
	    if (pbuf)
    		LocalFree(pbuf);
    	return hr;
    }

    HRESULT HostSession()
    {
        HRESULT hr = S_OK;

        m_pDirectPlay = CreateDirectPlay();
        m_pDirectPlayLobby = CreateDirectPlayLobby();
        if (!m_pDirectPlay || !m_pDirectPlayLobby)
            return E_FAIL;

        if (GetCadetMode() == cmHostSerial)
            {
            if (FAILED(hr = InitializeSerialConnection()))
                return hr;
            }
        else
            {
            ZAssert(GetCadetMode() == cmHostModem);
            if (FAILED(hr = InitializeModemConnection()))
                return hr;
            }
            
	    DPSESSIONDESC2	sessionDesc;
    	memset(&sessionDesc, 0, sizeof(DPSESSIONDESC2));
		sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
		sessionDesc.dwFlags = DPSESSION_MIGRATEHOST | DPSESSION_KEEPALIVE;
		sessionDesc.guidApplication = CLSID_CADET;
		sessionDesc.dwMaxPlayers = 2;
		sessionDesc.lpszSessionNameA = "Cadet Default Session";

		if (FAILED(hr = m_pDirectPlay->Open(&sessionDesc, DPOPEN_CREATE)))
			return hr;

		if (FAILED(hr = m_pDirectPlay->CreatePlayer(&m_dpid, NULL, NULL, NULL, 0, 0)))
			return hr;

        return hr;
    }
    
    HRESULT JoinSession(ImissionIGC* pmission)
    {
        HRESULT hr = S_OK;

        m_pDirectPlay = CreateDirectPlay();
        m_pDirectPlayLobby = CreateDirectPlayLobby();
        if (!m_pDirectPlay || !m_pDirectPlayLobby)
            return E_FAIL;

        if (GetCadetMode() == cmJoinSerial)
            {
            if (FAILED(hr = InitializeSerialConnection()))
                return hr;
            }
        else
            {
            ZAssert(GetCadetMode() == cmJoinModemNum1 || GetCadetMode() == cmJoinModemNum2);
            if (FAILED(hr = InitializeModemConnection()))
                return hr;
            }

    	DPSESSIONDESC2	sessionDescFilter;
		memset(&sessionDescFilter, 0, sizeof(DPSESSIONDESC2));
		sessionDescFilter.dwSize = sizeof(DPSESSIONDESC2);
		sessionDescFilter.guidApplication = CLSID_CADET;

    	DPSESSIONDESC2	sessionDescFirst;
		memset(&sessionDescFirst, 0, sizeof(DPSESSIONDESC2));
    	
        if (FAILED(hr = m_pDirectPlay->EnumSessions(&sessionDescFilter, 0, GetFirstSession, (LPVOID)&sessionDescFirst, DPENUMSESSIONS_ALL)))
            return hr;        

        if (sessionDescFirst.dwSize == 0)
            return E_FAIL;
            
		if (FAILED(hr = m_pDirectPlay->Open(&sessionDescFirst, DPOPEN_JOIN)))
			return hr;

		if (FAILED(hr = m_pDirectPlay->CreatePlayer(&m_dpid, NULL, NULL, NULL, 0, 0)))
			return hr;

        for (ShipLinkIGC*   l = pmission->GetShips()->first(); (l != NULL); l = l->next())
            {
            IshipIGC* pship = l->data();
            ZAssert(pship->GetObjectID() < 100);
            SendNewShipInstanceMessage(pship);
            }

        LogonMessage logonMsg;
        logonMsg.msgID = LogonMessageID;
        logonMsg.cbMsg = sizeof(LogonMessage);
        SendMessage(&logonMsg, true);

        return hr;
    }

    HRESULT ReceiveMessages()
    {
        HRESULT hr = S_OK;
        DWORD   dwMsgBufferSize = 0;
        DPID dpidFrom, dpidTo;
        BYTE rgbuf[1024];
        ULONG cbPacket = 1024;
        
        while (SUCCEEDED(hr))
            {
            dpidFrom = 0;
            dpidTo = 0;
            if (SUCCEEDED(hr = m_pDirectPlay->Receive(&dpidFrom, &dpidTo, DPRECEIVE_ALL, rgbuf, &cbPacket)))
                {
                // check for system message
                if (dpidFrom == DPID_SYSMSG)
                    ;//m_pCadetPlaySite->OnSysMessage((LPDPMSG_GENERIC)rgbuf);
                else
                    {
					CadetMessage* pcm = (CadetMessage*)rgbuf;
                    if (m_pCadetPlaySite)
                        m_pCadetPlaySite->OnAppMessage(pcm);
                    }
                }
            }

        return S_OK;
    }

    void SendMessage(CadetMessage* pmsg, bool fGuaranteed)
    {
        m_pDirectPlay->Send(m_dpid, DPID_ALLPLAYERS, fGuaranteed ? DPSEND_GUARANTEED : 0, pmsg, pmsg->cbMsg);
    }


    void HandleIGCMessage(ImissionIGC* pmission, CadetMessage* pmsg)
    {
        switch (pmsg->msgID)
            {
            case NewShipInstanceMessageID:
                {
                NewShipInstanceMessage* pnewShipMsg = (NewShipInstanceMessage*)pmsg;
                DWORD cbExport = pnewShipMsg->cbMsg - sizeof(NewShipInstanceMessage);
                BYTE* pExport = ((BYTE*)pnewShipMsg) + sizeof(NewShipInstanceMessage);
                ((DataShipIGC*)pExport)->shipID += 100;
                IshipIGC* pship = (IshipIGC*)pmission->CreateObject(Time::Now(),
                                        OT_ship, pExport, cbExport);
                ZAssert(pship);
                pship->SetCluster(pmission->GetCluster(0));
                pship->Release();
                break;
                }
            case ShipUpdateMessageID:
                {
                ShipUpdateMessage* pshipUpdateMsg = (ShipUpdateMessage*)pmsg;
                pshipUpdateMsg->shipUpdate.shipID += 100;
                IshipIGC* pship = pmission->GetShip(pshipUpdateMsg->shipUpdate.shipID);
                if (pship)
                    pship->ProcessShipUpdate(Time::Now(), pshipUpdateMsg->shipUpdate);
                break;
                }
            case LogonMessageID:
                {
                for (ShipLinkIGC*   l = pmission->GetShips()->first(); (l != NULL); l = l->next())
                    {
                    IshipIGC* pship = l->data();
                    if (pship->GetObjectID() < 100)
                        SendNewShipInstanceMessage(pship);
                    }
                break;
                }
            case FireMissileMessageID:
                {
                    FireMissileMessage* pfireMissileMsg = (FireMissileMessage*)pmsg;

                    DataMissileIGC  dm;
                    pfireMissileMsg->launcherID += 100;
                    dm.pLauncher = pmission->GetShip(pfireMissileMsg->launcherID);
                    if (dm.pLauncher)
                    {
                        dm.position = pfireMissileMsg->position;
                        dm.velocity = pfireMissileMsg->velocity;
                        dm.forward = pfireMissileMsg->forward;
                        dm.pmissiletype = (ImissileTypeIGC*)(pmission->GetExpendableType(pfireMissileMsg->missiletypeID));
                        dm.pTarget = pmission->GetModel(pfireMissileMsg->targetType,
                                                              pfireMissileMsg->targetID);

                        dm.pCluster = pmission->GetCluster(pfireMissileMsg->clusterID);
                        dm.lock = pfireMissileMsg->lock;

                        dm.missileID = pmission->GenerateNewMissileID();

                        Time timeFired = Time::Now();

                        ImissileIGC*    m = (ImissileIGC*)(pmission->CreateObject(timeFired, OT_missile,
                                                                                    &dm, sizeof(dm)));

                        if (m)
                            m->Release();

/*                        //The player immediately adjusts their own count on launching the missile so don't update
                        //their count.
                        if (pfireMissileMsg->launcherID != m_ship->GetObjectID())
                        {
                            ImagazineIGC*   pmagazine = (ImagazineIGC*)(dm.pLauncher->GetMountedPart(ET_Magazine, 0));
                            if (pmagazine)
                            {
                                short   amount = pmagazine->GetAmount() - 1;
                                if (amount == 0)
                                {
                                    //Nothing left ... nuke it (which may also cause it to be released & deleted).
                                    pmagazine->Terminate();
                                }
                                else
                                {
                                    assert (amount > 0);
                                    pmagazine->SetAmount(amount);
                                }
                            }
                        }
*/                        
                    }
                }
                break;
                
            }
    }

    void SendShipUpdateMessages(ImissionIGC* pmission)
    {
        if (Time::Now() - m_timeLastUpdate > .25f) //send updates every 1/4 sec
            {
            for (ShipLinkIGC*   l = pmission->GetShips()->first(); (l != NULL); l = l->next())
                {
                IshipIGC* pship = l->data();
                if (pship->GetObjectID() < 100)
                    SendShipUpdateMessage(pship);
                }
            m_timeLastUpdate = Time::Now();                
            }                
    }

    void SendShipUpdateMessage(IshipIGC* pship)
    {
        ShipUpdateMessage msg;
        msg.msgID = ShipUpdateMessageID;
        msg.cbMsg = sizeof(ShipUpdateMessage);
        pship->ExportShipUpdate(&msg.shipUpdate);
        SendMessage(&msg, false);
    }

    void SendNewShipInstanceMessage(IshipIGC* pship)
    {
        BYTE buf[4096];
        NewShipInstanceMessage* pmsg = (NewShipInstanceMessage*)&buf;
        pmsg->msgID = NewShipInstanceMessageID;

        void* pExport = (void*)(pmsg+1);
        int cbExport = pship->Export(pExport);
        
        pmsg->cbMsg = sizeof(NewShipInstanceMessage) + cbExport;

        SendMessage(pmsg, true);
    }

    void SendFireMissileMessage(DataMissileIGC* pdataMissile)
    {
        FireMissileMessage msg;
        msg.msgID = FireMissileMessageID;
        msg.cbMsg = sizeof(FireMissileMessage);

        msg.position = pdataMissile->position;
        msg.velocity = pdataMissile->velocity;
        msg.forward = pdataMissile->forward;

        msg.launcherID = pdataMissile->pLauncher->GetObjectID();
        if (NULL != pdataMissile->pTarget)
        {
            msg.targetType = pdataMissile->pTarget->GetObjectType();
            msg.targetID = pdataMissile->pTarget->GetObjectID();
        }
        else
        {
            msg.targetType = OT_invalid;
            msg.targetID = NA;
        }
        msg.clusterID = pdataMissile->pCluster->GetObjectID();
        msg.lock = pdataMissile->lock;
        msg.missiletypeID = pdataMissile->pmissiletype->GetObjectID();
        msg.timeFired = Time::Now();

        SendMessage(&msg, true);
    }

};

TRef<CadetPlay> CadetPlay::Create()
{
    return new CadetPlayImpl();
}
