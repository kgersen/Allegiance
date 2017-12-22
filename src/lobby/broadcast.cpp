#include "pch.h"
#include "broadcast.h"

#define DebugPrint

UdpSocket::UdpSocket() 
{
    m_sock=INVALID_SOCKET;
}


UdpSocket::~UdpSocket()
{
    if (m_sock!=INVALID_SOCKET)
    {
        Close();
    }
}

int UdpSocket::Bind(unsigned short port, unsigned long addr )
{
    int result = 0;

    //If called a second time reintialize
    if (m_sock != INVALID_SOCKET)
        closesocket(m_sock);

    m_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (m_sock==INVALID_SOCKET)
    {
        result=WSAGetLastError();
        DebugPrint("Failed to create socket error<%d>\n",result);
        return result;
    }

    SOCKADDR_IN sin;
    ZeroMemory(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr = addr;
    sin.sin_port = htons(port);

    if ( bind(m_sock,(SOCKADDR *)&sin,sizeof(sin)) == SOCKET_ERROR )
    {
        result=WSAGetLastError();
        DebugPrint("Failed to bind error<%d>\n",result);
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
    }
    
    return result;
};


int UdpSocket::BindExcludePort(unsigned short port, unsigned long addr )
{
    int result;
    unsigned short i;

    //If called a second time reintialize
    if (m_sock != INVALID_SOCKET)
        closesocket(m_sock);

    m_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (m_sock==INVALID_SOCKET)
    {
        result=WSAGetLastError();
        DebugPrint("Failed to create socket error<%d>\n",result);
        return result;
    }


    SOCKADDR_IN sin;
    ZeroMemory(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr = addr;

    // bind to a port other than the one passed in
    for (i=10000;i<0xFFFF;i++) {
        if (i==port)
            continue;

        sin.sin_port = htons(i);
//        DebugPrint("Trying to bind on port<%d>\n",i);
        if (result=bind(m_sock,(SOCKADDR *)&sin,sizeof(sin)))
        {
            result=WSAGetLastError();
            if (result != WSAEADDRINUSE) {
                break;
            }
        }
        else
            break;
    }

    if (result) 
    {
        DebugPrint("Failed to bind error<%d>\n",result);
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
    }

    
    return result;
}

int UdpSocket::Close() 
{
    int ret = closesocket(m_sock);
    m_sock=INVALID_SOCKET;
    return ret;
}

int UdpSocket::EnableBroadcast()
{
    //Create a socket for broadcasting 
    //should set buffer max to 512
    int result = 0;

    BOOL istrue=TRUE;
    if (setsockopt(m_sock,SOL_SOCKET,
            SO_BROADCAST,(const char *)&istrue,sizeof(istrue))) {
        result = WSAGetLastError();
        DebugPrint("Failed to setsockopt SO_BROADCAST error<%d>\n",result);
    }
    return result;
}

int UdpSocket::SendTo(void* pBuffer, int size, unsigned short port, unsigned long addr) 
{
    SOCKADDR_IN sin;
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr = addr;
    sin.sin_port = htons(port);

    return sendto(m_sock,(char *)pBuffer, size,
            (addr == INADDR_BROADCAST) ? MSG_DONTROUTE : 0 ,
            (SOCKADDR *)&sin, sizeof(sin));
        
};

int UdpSocket::Recv(void* pBuffer,int size) 
{
    return recvfrom(m_sock,(char *)pBuffer,size,0,NULL,NULL);                    
};

#if 0

Broadcast::Broadcast() 
{
    m_sock=INVALID_SOCKET;
};


Broadcast::~Broadcast()
{
    if (m_sock!=INVALID_SOCKET)
    {
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
    }
};

int Broadcast::Bind(unsigned short port)
{
    SOCKADDR_IN addr;
    int result;
    unsigned short i;
    BOOL istrue=TRUE;

    //If called a second time reintialize
    if (m_sock != INVALID_SOCKET)
        closesocket(m_sock);

    m_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    if (m_sock==INVALID_SOCKET)
    {
        result=WSAGetLastError();
        DebugPrint("Failed to create socket error<%d>\n",result);
        return result;
    }


    //Create a socket for broadcasting 
    //and that doesn't linge on close
    //should set buffer max to 512
/*
    if (setsockopt(m_sock,SOL_SOCKET,
            SO_BROADCAST,(const char *)&istrue,sizeof(istrue))) {
        result = WSAGetLastError();
        DebugPrint("Failed to setsockopt SO_BROADCAST error<%d>\n",result);
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
        return result;
    }
*/
    //remember that TCP/IP and UDP ports are seperate
    //and so even with TCP/IP ports all used we shouldn't
    //have a problem binding to a port. So no retry on port
    ZeroMemory(&addr,sizeof(SOCKADDR_IN));
    ZeroMemory(&m_addr,sizeof(SOCKADDR_IN));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    //For broadcast address we don't want to bind to the port we want to send to
    for (i=10000;i<0xFFFF;i++) {
        if (i==port)
            continue;

        addr.sin_port = htons(i);
        DebugPrint("Tring to bind on port<%d>\n",i);
        if (result=bind(m_sock,(SOCKADDR *)&addr,sizeof(SOCKADDR_IN)))
        {
            result=WSAGetLastError();
            if (result!= WSAEADDRINUSE) {
                DebugPrint("Failed to bind error<%d>\n",result);
                closesocket(m_sock);
                m_sock=INVALID_SOCKET;
                return result;
            }
        }
        else
            break;
    }

    if (result) {
        DebugPrint("Failed to bind to any address\n");
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
    };    
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_BROADCAST;
    CopyMemory(&m_addr,&addr,sizeof(SOCKADDR_IN));

    //We are good to send
    return NULL;
};

int Broadcast::Close() 
{
    int ret = closesocket(m_sock);
    m_sock=INVALID_SOCKET;
    return ret;
}

//Change semantics of send because UDP sends
//are all or nothing. So we are not returning size    
int Broadcast::Send(void* pBuffer,int size) 
{
    ASSERT( m_sock != INVALID_SOCKET );
    return sendto(m_sock,(char *)pBuffer,size,
            MSG_DONTROUTE,(SOCKADDR *)&m_addr,sizeof(SOCKADDR_IN));
        
};


int Listen::Bind(unsigned long addr, unsigned short port)
{
    int result;
    BOOL istrue=TRUE;

    //If called a second time reintialize
    if (m_sock != INVALID_SOCKET)
        closesocket(m_sock);

    m_sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    if (m_sock==INVALID_SOCKET)
    {
        result=WSAGetLastError();
        DebugPrint("Failed to create socket error<%d>\n",result);
        return result;
    }


    //Create a socket for broadcasting 
    //and that doesn't linge on close
    //should set buffer max to 512
/*
    if (setsockopt(m_sock,SOL_SOCKET,
            SO_BROADCAST,(const char *)&istrue,sizeof(istrue))) {
        result = WSAGetLastError();
        DebugPrint("Failed to setsockopt SO_BROADCAST error<%d>\n",result);
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
        return result;
    }
*/

    ZeroMemory(&m_addr,sizeof(SOCKADDR_IN));
    m_addr.sin_family=AF_INET;
    m_addr.sin_addr.s_addr = addr;
    m_addr.sin_port = htons(port);

    if ( bind(m_sock,(SOCKADDR *)&m_addr,sizeof(SOCKADDR_IN)) == SOCKET_ERROR )
    {
        result=WSAGetLastError();
        DebugPrint("Failed to bind error<%d>\n",result);
        closesocket(m_sock);
        m_sock=INVALID_SOCKET;
        return result;
    }
    
    return 0;
};


//Change semantics of send because UDP sends
//are all or nothing. So we are not returning size    
int Listen::Recv(void* pBuffer,int size) 
{
    return recvfrom(m_sock,(char *)pBuffer,size,0,NULL,NULL);                    
};

    
#endif
