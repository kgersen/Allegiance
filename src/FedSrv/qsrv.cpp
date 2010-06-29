#include "pch.h"

//Imago 6/10 QueryServer

#if 1 // #ifdef LITTLEENDIAN
#if 0 //defined(_M_IX86)
#define _ZEnd32( pData )     \
    __asm {                 \
            mov eax, *pData \
            bswap eax       \
            mov *pData, eax \
          }

#else
#define _ZEnd32( pData )   \
{                         \
    char *c;              \
    char temp;            \
                          \
    c = (char *) pData;   \
    temp = c[0];          \
    c[0] = c[3];          \
    c[3] = temp;          \
    temp = c[1];          \
    c[1] = c[2];          \
    c[2] = temp;          \
}
#endif

#define _ZEnd16( pData )   \
{                         \
    char *c;              \
    char temp;            \
                          \
    c = (char *) pData;   \
    temp = c[0];          \
    c[0] = c[1];          \
    c[1] = temp;          \
}

#else  // not LITTLEENDIAN

#define _ZEnd32(pData)
#define _ZEnd16(pData)

#endif // not LITTLEENDIAN

#define ZEnd32(pData) _ZEnd32(pData)
#define ZEnd16(pData) _ZEnd16(pData)

const int kBufferSize = 8192;
        
SOCKET SetUpListener(const char* pcAddress, int nPort);
void QServerCnxn(SOCKET ListeningSocket);
bool UNSPackets(SOCKET sd);

SOCKET SetUpListener(const char* pcAddress, int nPort)
{
	SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd != INVALID_SOCKET) {
		sockaddr_in sinInterface;
		sinInterface.sin_family = AF_INET;
        sinInterface.sin_port = nPort;
		u_long nInterfaceAddr = inet_addr(pcAddress);
		if (nInterfaceAddr == INADDR_NONE) {
			hostent *pHost = gethostbyname(pcAddress);
			memcpy(&sinInterface.sin_addr, pHost->h_addr, pHost->h_length);
			sinInterface.sin_family = pHost->h_addrtype;
		} else {
            sinInterface.sin_addr.s_addr = nInterfaceAddr;
        }
        if (bind(sd, (sockaddr*)&sinInterface, 
                sizeof(sockaddr_in)) != SOCKET_ERROR) {
            listen(sd, SOMAXCONN);
            return sd;
        }
        else {
            //shhhh
        }
    }

    return INVALID_SOCKET;
}


DWORD WINAPI QueryHandler(void* sd_) 
{
    int nRetval = 0;
    SOCKET sd = (SOCKET)sd_;
	u_long iMode = 0;
	int ret = ioctlsocket(sd, FIONBIO, &iMode);
    if (!UNSPackets(sd)) {
		debugf("%s\n",(PCC)WSAGetLastErrorMessage("Query server message delivery failed"));
        nRetval = 3;
    }
	iMode = 1;
	ret = ioctlsocket(sd, FIONBIO, &iMode);
    if (ShutdownConnection(sd)) {
      //TODO limits
    }
    else {
        nRetval = 3;
    }

    return nRetval;
}


 void QServerCnxn(SOCKET QServerSocket)
{
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);
	int sd, arg;
	u_long iMode = 1;
	int ret = ioctlsocket(QServerSocket, FIONBIO, &iMode);
	SOCKET sds = accept(QServerSocket, (sockaddr*)&sinRemote, &nAddrSize);
	if (sds != INVALID_SOCKET) {
		DWORD nThreadID;
		CreateThread(0, 0, QueryHandler, (void*)sds, 0, &nThreadID); //TODO limits
	}
	else {
		return;
	}
}

 bool UNSPackets(SOCKET sd) {
	char acSendBuffer[kBufferSize] = {'\0'};
	char szBuffer[kBufferSize] = {'\0'};

	ZString szServer = "No games";	
	int iMostships = 0;
	int iTotalships = 0;
	int iTotallag = 1;
	int iMaxships = 0;
	byte iGametype = 0;
	int nPlayers = 0;
	bool bAutobalance = false;

	const ListFSMission * lMissions = CFSMission::GetMissions();
	for (LinkFSMission * plinkFSMis = lMissions->first(); plinkFSMis; plinkFSMis = plinkFSMis->next())
	{
		CFSMission * pfsMission = plinkFSMis->data();
		const ShipListIGC * plistShip = pfsMission->GetIGCMission()->GetShips();
		int iShips = plistShip->n();
		iTotalships += iShips;
		if (iShips > iMostships) {
			szServer = pfsMission->GetMissionDef()->misparms.szIGCStaticFile;
			bAutobalance = pfsMission->GetMissionDef()->misparms.iMaxImbalance;
			if (pfsMission->GetMissionDef()->misparms.IsDeathMatchGame()) {
				iGametype = 0;
			}
			if (pfsMission->GetMissionDef()->misparms.IsFlagsGame()) {
				iGametype = 8;
			}
			if (pfsMission->GetMissionDef()->misparms.IsConquestGame()) {
				iGametype = 6;
			}
		}
		iMaxships += pfsMission->GetMissionDef()->misparms.nTotalMaxPlayersPerGame;
		for (ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
		{
			if (ISPLAYER(plinkShip->data()) &&  !plinkShip->data()->IsGhost()) {
				CFSPlayer* pfsPlayer = ((CFSShip*)(plinkShip->data()->GetPrivateData()))->GetPlayer();
				DWORD lag = pfsPlayer->GetAverageLatency();
				iTotallag += lag;
			}
		}

	}
	int ilag = (iTotalships > 0) ? iTotallag / iTotalships : 1; //no you lag HPB
	int iver = MSGVER;

    int nBytes = 10;
	memcpy(acSendBuffer,"AllegQstat",nBytes);
	
	memcpy(acSendBuffer+nBytes,&ilag,sizeof(char));
	nBytes++;

	memcpy(acSendBuffer+nBytes,&iver,sizeof(char));
	nBytes++;

	memcpy(acSendBuffer+nBytes," ",sizeof(char));
	nBytes++;

	memcpy(acSendBuffer+nBytes,&iGametype,sizeof(char)); //TODO fix byte (gametype = bit &15 and imbal = bit &16)
	nBytes++;

	memcpy(acSendBuffer+nBytes,"    ",sizeof(char) * 4);
	nBytes += 4;

	memcpy(acSendBuffer+nBytes,(PCC)szServer,szServer.GetLength());
	nBytes += szServer.GetLength() + 1;
	int iServerBytes = nBytes;
	nBytes = 0;
	//ya, i'm lazy...
	lMissions = CFSMission::GetMissions();
	for (LinkFSMission * plinkFSMis = lMissions->first(); plinkFSMis; plinkFSMis = plinkFSMis->next())
	{
		CFSMission * pfsMission = plinkFSMis->data();
		const ShipListIGC * plistShip = pfsMission->GetIGCMission()->GetShips();

		for (ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
		{
			if (ISPLAYER(plinkShip->data()) &&  !plinkShip->data()->IsGhost()) {
				CFSPlayer* pfsPlayer = ((CFSShip*)(plinkShip->data()->GetPrivateData()))->GetPlayer();
				PlayerScoreObject pso = pfsPlayer->GetPlayerScoreObject();
				nPlayers++;

				memcpy(szBuffer+nBytes,&nPlayers,sizeof(char));
				nBytes++;

				if (pfsPlayer->GetIGCShip() && pfsPlayer->GetIGCShip()->GetHullType()) {
					ObjectID ship = pfsPlayer->GetIGCShip()->GetHullType()->GetObjectID();
					memcpy(szBuffer+nBytes,&ship,sizeof(char));
				} else {
					memcpy(szBuffer+nBytes,"0",sizeof(char));
				}
				nBytes++;

				DWORD lag = pfsPlayer->GetAverageLatency(); //TODO this should hook up to w0dkas PING
				memcpy(szBuffer+nBytes,&lag,sizeof(char));
				nBytes++;

				short kills = pso.GetKills();
				memcpy(szBuffer+nBytes,&kills,sizeof(char));
				nBytes++;

				if (pfsPlayer->GetSide()) { //TODO match up
					ObjectID team = pfsPlayer->GetSide()->GetObjectID();
					memcpy(szBuffer+nBytes,&team,sizeof(char));
				} else {
					memcpy(szBuffer+nBytes,"0",sizeof(char));
				}
				nBytes++;

				if (pfsPlayer->GetCluster()) {
					ObjectID cluster = pfsPlayer->GetCluster()->GetObjectID();
					memcpy(szBuffer+nBytes,&cluster,sizeof(char));
				} else {
					memcpy(szBuffer+nBytes,"0",sizeof(char));
				}
				nBytes++;

				memcpy(szBuffer+nBytes,"0",sizeof(char));
				nBytes++;

				unsigned long iScore = (long)pso.GetScore();
				ZEnd32(&iScore);			
				memcpy(szBuffer+nBytes,&iScore,sizeof(iScore));
				nBytes += 4;

				ZString name = pfsPlayer->GetName();
				memcpy(szBuffer+nBytes,(PCC)name,name.GetLength());
				nBytes += name.GetLength();
			}
		}
	}
	memcpy(acSendBuffer+iServerBytes,&szBuffer,nBytes);
	int nSent = 0;
	int nTemp = 0;
	do {
		nTemp = send(sd,acSendBuffer,kBufferSize, 0);
		if (nTemp == SOCKET_ERROR) {
			return false;
		}
		nSent += nTemp;
	} while (nSent < kBufferSize && nTemp != 0);

	if (nTemp == 0 && nSent != kBufferSize) {
		return false;
	}

    return true; //Payload delivered!
}