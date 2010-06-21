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

const int kBufferSize = 1024;
        
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
		printf("%s\n",(PCC)WSAGetLastErrorMessage("Query server message delivery failed"));
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

 bool UNSPackets(SOCKET sd)
{

	char acSendBuffer[kBufferSize] = {'\0'};
    int nReadBytes;

	// $> qstat.exe -P -R -bfs 192.168.0.252:25000 192.168.0.252:25001 ...
	//TODO hook up real data!
	memcpy(acSendBuffer,"AllegQstatN cz    MapName",25);
	memcpy(acSendBuffer+26,"1zczcz     iMago",16);
	memcpy(acSendBuffer+26+17,"2zczcz     ImAgo",16);
	memcpy(acSendBuffer+26+17+17,"3zczcz     ImaGo",16);

	int nTemp = send(sd,acSendBuffer,kBufferSize, 0);
	if (nTemp > 0) {
		printf("Sent: %s to qstat\n",2);
	}
	else if (nTemp == SOCKET_ERROR) {
		return false;
	}
	else {
		printf("qstat unexpectedly dropped connection!\n");
		return true;
	}

    return true;
}

