#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class NetworkNameSpace {
    class Address : public sockaddr_in {
    public:
        friend bool operator==(const Address& a1, const Address& a2)
        {
            return memcmp(&a1, &a2, sizeof(Address)) == 0;
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    class NetworkImpl;
    class SocketImpl : public Socket {
    public:
        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<NetworkImpl> m_pnetwork;
        TRef<SocketSite>  m_psite;
        SOCKET            m_socket;
        WSAEVENT          m_event;
        Address           m_addrUDP;

        char*             m_pout;
        DWORD             m_sizeOut;
        DWORD             m_indexOut;

        char*             m_pin;
        char*             m_pinMessage;
        char*             m_pinCurrent;
        char*             m_pinEnd;

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        SocketImpl(
            NetworkImpl*   pnetwork, 
            WSAEVENT       event,
            SOCKET         socket
        ) :
            m_pnetwork(pnetwork),
            m_event(event),
            m_socket(socket)
        {
            m_addrUDP.sin_family = 0;

            DWORD sizeIn = 512;
            m_pin        = (char*)malloc(sizeIn);
            m_pinMessage = m_pin;
            m_pinCurrent = m_pin;
            m_pinEnd     = m_pin + sizeIn;

            m_sizeOut    = 512;
            m_indexOut   = 0;
            m_pout       = (char*)malloc(m_sizeOut);
        }

        ~SocketImpl()
        {
            if (m_pnetwork != NULL) {
                Disconnect();
            }

            free(m_pin);
            free(m_pout);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void Disconnect()
        {
            m_pnetwork->CloseSocket(this);
            m_pnetwork = NULL;
            m_psite->Disconnect(this);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void ReadTCPSocket()
        {
            //
            // Read the socket
            //

            int cbRead = 
                recv(
                    m_socket, 
                    m_pinCurrent, 
                    m_pinEnd - m_pinCurrent,
                    0
                );

            if (cbRead == SOCKET_ERROR) {
                int error = WSAGetLastError();
                ZAssert(
                    error == WSAECONNRESET
                );

                Disconnect();
                return;
            }

            //
            // Handle the message
            //

            if (m_addrUDP.sin_port == 0) {
                //
                // First message is the port of the UDP socket
                //

                m_addrUDP.sin_port = *(WORD*)m_pin;
                m_pnetwork->m_mapSockets.Set(m_addrUDP, this);
            } else if (cbRead == 0) {
                //
                // Client disconnected
                //

                Disconnect();
            } else {
                m_pinCurrent += cbRead;

                //
                // Parse the messages
                //

                while (m_pinMessage < m_pinCurrent) {
                    DWORD length = *(DWORD*)(m_pinMessage);

                    if ((DWORD)(m_pinCurrent - m_pinMessage) >= length) {
                        m_psite->Receive(this, m_pinMessage + sizeof(DWORD), length - sizeof(DWORD));
                        m_pinMessage += length;
                    } else {
                        //
                        // Received part of a message
                        //

                        if (length > (DWORD)(m_pinEnd - m_pinMessage)) {
                            //
                            // The rest of message isn't going to fit.
                            // Move the message to the front of the buffer.
                            // 

                            int delta = m_pinMessage - m_pin;
                            memcpy(m_pin, m_pinMessage, m_pinCurrent - m_pinMessage);
                            m_pinCurrent -= delta;
                            m_pinMessage  = m_pin;

                            //
                            // Make sure the buffer is big enough to hold the whole thing
                            //

                            DWORD inSize = (DWORD)(m_pinEnd - m_pin);
                            if (length > inSize) {
                                do {
                                    inSize *= 2;
                                } while (length > inSize);

                                DWORD delta = (DWORD)(m_pinCurrent - m_pin);
                                m_pin = (char*)realloc(m_pin, inSize);
                                m_pinMessage = m_pin;
                                m_pinCurrent = m_pin + delta;
                                m_pinEnd     = m_pin + inSize;
                            }
                        }

                        return;
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // Socket members
        //
        //////////////////////////////////////////////////////////////////////////////

        bool Flush()
        {
            int cbSent = send(m_socket, m_pout, m_indexOut, 0);

            if (cbSent == SOCKET_ERROR) {
                int error = WSAGetLastError();
                ZAssert(
                       error == WSAECONNRESET
                    || error == WSAENOTCONN
                );
                Disconnect();
                return false;
            } else {
                ZAssert((DWORD)cbSent == m_indexOut);
                m_indexOut = 0;
                return true;
            }
        }

        void* AllocateMessage(DWORD length)
        {
            length += sizeof(DWORD);

            if (m_indexOut + length > m_sizeOut) {
                if (!Flush()) {
                    return NULL;
                }

                if (length > m_sizeOut) {
                    do {
                        m_sizeOut *= 2;
                    } while (length > m_sizeOut);

                    m_pout = (char*)realloc(m_pout, m_sizeOut);
                }
            }

            void* pout = m_pout + m_indexOut + sizeof(DWORD);
            (*(DWORD*)(m_pout + m_indexOut)) = length;
            m_indexOut += length;

            return (void*)pout;
        }

        bool Send(const void* pbuf, DWORD length, bool bGuaranteed)
        {
            if (bGuaranteed) {
                void* pvoid = AllocateMessage(length);
                if (pvoid) {
                    memcpy(pvoid, pbuf, length);
                    return true;
                }

                return false;
            } else {
                return m_pnetwork->SendUDP(m_addrUDP, pbuf, length);
            }
        }

        bool Send(const ZString& str, bool bGuaranteed)
        {
            return Send(str, str.GetLength() + 1, bGuaranteed);
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    class NetworkImpl : public Network {
    public:
        typedef TVector<WSAEVENT,    DefaultEquals> EventVector;
        typedef TVector<SocketImpl*, DefaultEquals> SocketVector;
        typedef TMap<Address, SocketImpl*>          SocketMap;

        TRef<NetworkSite> m_psite;
        SOCKET            m_socketListen;
        SOCKET            m_socketUDP;
        Address           m_addrUDP;
        EventVector       m_vecEvents;
        SocketVector      m_vecSockets;
        SocketMap         m_mapSockets;
        char*             m_pin;
        int               m_sizeIn;

        NetworkImpl(NetworkSite* psite, int port) :
            m_psite(psite)
        {
            //
            // Allocate the input buffer
            //

            m_sizeIn   = 512;
            m_pin      = (char*)malloc(m_sizeIn);

            //
            // Start WinSock
            //

            WSADATA wsaData;
            ZVerify(WSAStartup(0x202, &wsaData) != SOCKET_ERROR);

            //
            // Listen for UDP messages
            //

            m_socketUDP = CreateListenSocket(SOCK_DGRAM, port);

            //
            // Get the UDP address
            //

            int cbUDP = sizeof(m_addrUDP);
            ZVerify(getsockname(m_socketUDP, (sockaddr*)&m_addrUDP, &cbUDP) != SOCKET_ERROR);

            //
            // If this is a server create the Listen thread
            //

            if (m_psite) {
                m_socketListen = CreateListenSocket(SOCK_STREAM, port);
            }
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        ~NetworkImpl()
        {
            WSACleanup();
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        SOCKET CreateListenSocket(int type, int port)
        {
            //
            // Create the socket
            //

	        SOCKET socketNew = socket(AF_INET, type, 0);
            ZAssert(socketNew != INVALID_SOCKET);

            //
            // Bind it
            //

            sockaddr_in addrLocal;

            addrLocal.sin_family      = AF_INET;
            addrLocal.sin_addr.s_addr = INADDR_ANY; 
            addrLocal.sin_port        = htons(port);

            ZVerify(bind(
                socketNew, 
                (struct sockaddr*)&addrLocal, 
                sizeof(addrLocal)
            ) != SOCKET_ERROR);

            //
            // Listen
            //

            if (type == SOCK_STREAM) {
                ZVerify(listen(socketNew, SOMAXCONN) != SOCKET_ERROR);
            }

            //
            // Create the Listen event
            //

            WSAEVENT event = WSACreateEvent();
            ZAssert(event != WSA_INVALID_EVENT);

            ZVerify(WSAEventSelect(
                socketNew, 
                event, 
                (type == SOCK_DGRAM) ? FD_READ : FD_ACCEPT
            ) != SOCKET_ERROR);

            m_vecEvents.PushEnd(event);
            m_vecSockets.PushEnd(NULL);

            return socketNew;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void CloseSocket(SocketImpl* psocket)
        {
            WSACloseEvent(psocket->m_event);
            psocket->m_event = NULL;

            closesocket(psocket->m_socket);
            psocket->m_socket = NULL;

            m_mapSockets.Remove(psocket);
            m_vecSockets.Remove(psocket);
            m_vecEvents.Remove(psocket->m_event);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<SocketImpl> CreateSocket(SOCKET socket)
        {
            //
            // Create an event
            //

            WSAEVENT event = WSACreateEvent();
            ZAssert(event != WSA_INVALID_EVENT);

            ZVerify(WSAEventSelect(socket, event, FD_READ) != SOCKET_ERROR);

            m_vecEvents.PushEnd(event);

            //
            // Create a Socket object
            //

            TRef<SocketImpl> psocket = new SocketImpl(this, event, socket);

            m_vecSockets.PushEnd(psocket);

            return psocket;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void HandleConnection()
        {
            //
            // Clear the event
            //

            WSANETWORKEVENTS nwe;

            ZVerify(WSAEnumNetworkEvents(
                m_socketListen,
                m_vecEvents[1],
                &nwe
            ) != SOCKET_ERROR);
            ZAssert(nwe.lNetworkEvents == FD_ACCEPT);

            //
            // Accept the connection
            //

            Address addrFrom;
            int cbFrom = sizeof(addrFrom);

            SOCKET socket = accept(m_socketListen, (sockaddr*)&addrFrom, &cbFrom);
            ZAssert(socket != INVALID_SOCKET);

            //
            // Create the Socket object
            //

            TRef<SocketImpl> psocket = CreateSocket(socket);

            //
            // Fill in the UDP address, except for the port
            //

            psocket->m_addrUDP.sin_family = AF_INET;
            psocket->m_addrUDP.sin_port   = 0;
            psocket->m_addrUDP.sin_addr   = addrFrom.sin_addr;
            memset(&(psocket->m_addrUDP.sin_zero), 0, 8);

            //
            // Tell the NetworkSite
            //

            psocket->m_psite = m_psite->Connect(psocket);
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void HandleUDPMessage()
        {
            //
            // Clear the event
            //

            WSANETWORKEVENTS nwe;

            ZVerify(WSAEnumNetworkEvents(
                m_socketUDP,
                m_vecEvents[0],
                &nwe
            ) != SOCKET_ERROR);
            //ZAssert(nwe.lNetworkEvents == FD_READ);

            //
            // Read the message
            //

            Address addrFrom;
            int cbFrom = sizeof(addrFrom);
            char buf[cbUDPMax];

            int length = recvfrom(m_socketUDP, buf, cbUDPMax, 0, (sockaddr*)&addrFrom, &cbFrom);
            ZAssert(length != SOCKET_ERROR);
            ZAssert(length != 0);

            //
            // Route the message
            //

            SocketImpl* psocket;
            if (m_mapSockets.Find(addrFrom, psocket)) {
                psocket->m_psite->Receive(psocket, buf, length);
            }
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void HandleTCPMessage(int index)
        {
            //
            // Clear the event
            //

            WSANETWORKEVENTS nwe;

            ZVerify(WSAEnumNetworkEvents(
                m_vecSockets[index]->m_socket,
                m_vecEvents[index],
                &nwe
            ) != SOCKET_ERROR);
            //ZAssert(nwe.lNetworkEvents == FD_READ);

            //
            // Get the socket pointer
            //

            SocketImpl* psocket = m_vecSockets[index];

            //
            // Read the message
            //

            psocket->ReadTCPSocket();
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        void Receive()
        {
            //
            // Flush all of the out going messages
            //

            int count = m_vecSockets.GetCount();
            for(int index = 2; index < count; index++) {
                m_vecSockets[index]->Flush();
            }

            //
            // Loop while there are incoming messages.
            //

            while (true) {
                //
                // Wait for an incomming message
                //

                DWORD index =
                    WSAWaitForMultipleEvents(
                        m_vecEvents.GetCount(),
                        &m_vecEvents[0],
                        false,
                        0,
                        false
                    );

                //
                // Was there a time out?
                //

                if (index == WSA_WAIT_TIMEOUT) {
                    return;
                } 
                
                //
                // Adjust the event index
                //

                index = index - WSA_WAIT_EVENT_0;

                //
                // Route the event
                //

                if (index == 0) {
                    //
                    // UDP message
                    //

                    HandleUDPMessage();
                } else if (m_psite && index == 1) {
                    //
                    // Connection
                    //

                    HandleConnection();
                } else {
                    //
                    // TCP message
                    //
                
                    HandleTCPMessage(index);
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // Send a UDP message
        //
        //////////////////////////////////////////////////////////////////////////////

        bool SendUDP(const Address& addr, const void* pbuf, DWORD length)
        {
            if (length > cbUDPMax) {
                return false;
            }

            int cbSent =
                sendto(
                    m_socketUDP, 
                    (char *)pbuf, 
                    length,
                    0,
                    (sockaddr*)&addr,
                    sizeof(addr)
                );

            ZAssert(cbSent != SOCKET_ERROR);
            ZAssert((DWORD)cbSent == length);

            return true;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // 
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<Socket> ConnectToServer(const ZString& str, WORD port, SocketSite* psite)
        {
            //
            // Create a socket
            //

	        SOCKET socketNew = socket(AF_INET, SOCK_STREAM, 0);
            ZAssert(socketNew >= 0);

            //
            // Connect
            //

    	    unsigned int addr;
	        struct hostent *hp;

	        if (isalpha(str[0])) {   
                /* server address is a name */
		        hp = gethostbyname(str);
	        } else { 
                /* Convert nnn.nnn address to a usable one */
		        addr = inet_addr(str);
		        hp = gethostbyaddr((char *)&addr, 4, AF_INET);
	        }

	        if (hp == NULL ) {
                ZError("invalid host name");
		        WSACleanup();
                return NULL;
	        }

	        Address addrServer;
	        memset(&addrServer, 0, sizeof(addrServer));
	        memcpy(&(addrServer.sin_addr), hp->h_addr, hp->h_length);
	        addrServer.sin_family = hp->h_addrtype;
	        addrServer.sin_port = htons(port);
            int cbAddr = sizeof(addrServer);

         	if (connect(socketNew, (sockaddr*)&addrServer, cbAddr) == SOCKET_ERROR) {
                int error = WSAGetLastError();
                return NULL;
            }

            //
            // Create a Socket object
            //

            TRef<SocketImpl> psocket = CreateSocket(socketNew);
            psocket->m_addrUDP = addrServer;
            psocket->m_psite   = psite;

            //
            // Tell the server what the UDP address is
            //

            ZAssert(sizeof(m_addrUDP.sin_port) == sizeof(WORD));
            psocket->Send(&m_addrUDP.sin_port, sizeof(WORD), true);

            return psocket;
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    /*
    class GroupSocketImpl : public GroupSocket {
    private:
        TList<TRef<Socket> > m_list;

    public:
        void AddSocket(Socket* psocket)
        {
            m_list->PushFront(psocket);
        }

        void RemoveSocket(Socket* psocket)
        {
            m_list->Remove(psocket);
        }
    }
    */
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

TRef<Network> CreateNetwork(NetworkSite* psite, int port)
{
    return new NetworkNameSpace::NetworkImpl(psite, port);
}

