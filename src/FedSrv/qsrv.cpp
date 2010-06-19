#include "pch.h"

//Imago 6/10 QueryServer

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
            printf("%s\n",WSAGetLastErrorMessage("bind() failed"));
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
       printf("%s\n",(PCC)WSAGetLastErrorMessage("UNS kilo delivery failed"));
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
	ZString zQRequest;
	zQRequest.SetEmpty();
	char acReadBuffer[kBufferSize] = {'\0'};
    int nReadBytes;
    do {
        nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
        if (nReadBytes > 0) {
			zQRequest += acReadBuffer;
		} else if (nReadBytes == SOCKET_ERROR) {
            return false;
        }
    } while (nReadBytes != 0);

	//ZString zQResponse = GetQResponse(zQRequest); TODO

    printf("Connection closed by peer.\nSent: %s",(PCC)zQRequest);
    return true;
}

 /* TODO
   int nSentBytes = 0;
            while (nSentBytes < nReadBytes) {
                int nTemp = send(sd, acReadBuffer + nSentBytes,
                        nReadBytes - nSentBytes, 0);
                if (nTemp > 0) {
                   printf("Sent %i bytes back to client.\n",nTemp);
                    nSentBytes += nTemp;
                }
                else if (nTemp == SOCKET_ERROR) {
                    return false;
                }
                else {
                    printf("Peer unexpectedly dropped connection!\n");
                    return true;
                }
            }
        }
		*/

