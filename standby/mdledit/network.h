#ifndef _network_h_
#define _network_h_

//////////////////////////////////////////////////////////////////////////////
//
// Network
//
//////////////////////////////////////////////////////////////////////////////

const int cbUDPMax = 512;

class Socket : public IObject {
public:
    virtual bool  Flush()                                                       = 0;
    virtual void* AllocateMessage(DWORD length)                                 = 0;
    virtual bool  Send(const void* pbuf, DWORD length, bool bGuaranteed = true) = 0;
    virtual bool  Send(const ZString& str, bool bGuaranteed = true)             = 0;
};

class SocketSite : public IObject {
public:
    virtual void Receive(Socket* psocket, const void* pbuf, DWORD length) = 0;
    virtual void Disconnect(Socket* psocket) = 0;
};

class NetworkSite : public IObject {
public:
    virtual TRef<SocketSite> Connect(Socket* psocket) = 0;
};

class Network : public IObject {
public:
    virtual TRef<Socket> ConnectToServer(const ZString& str, WORD port, SocketSite* psite) = 0;
    virtual void Receive() = 0;
};

/*
class GroupSocket : public Socket {
public:
    virtual void AddSocket(Socket* psocket)    = 0;
    virtual void RemoveSocket(Socket* psocket) = 0;
};
*/

#endif
