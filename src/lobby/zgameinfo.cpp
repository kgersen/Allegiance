#include "pch.h"

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


UdpSocket g_Broadcast;

void ZGameServerInfoMsgEndian(ZGameServerInfoMsg* msg)
{
    ZEnd32(&msg->protocolSignature);
    ZEnd32(&msg->protocolVersion);        /* Protocol version */
    ZEnd16(&msg->numEntries);
}

void ZGameInstanceInfoMsgEndian(ZGameInstanceInfoMsg* msg)
{
    ZEnd32(&msg->gameAddr);
    ZEnd16(&msg->gamePort);
    ZEnd16(&msg->serviceType);
    // order is a byte
    // game state is a byte
    ZEnd32(&msg->gameVersion);
    ZEnd32(&msg->numPlayers);
    ZEnd32(&msg->numGamesServed);
    ZEnd16(&msg->numTables);
    ZEnd16(&msg->numTablesInUse);
    ZEnd16(&msg->blobsize);
    ZEnd32(&msg->numSysops);
    ZEnd32(&msg->numNotPlaying);
    ZEnd32(&msg->maxPopulation);

}


void ZGameInstanceInfoMsgZ5Endian(ZGameInstanceInfoMsgZ5* msg)
{
    ZEnd32(&msg->gameAddr);
    ZEnd16(&msg->gamePort);
    ZEnd16(&msg->serviceType);
    // order is a byte
    // game state is a byte
    ZEnd32(&msg->gameVersion);
    ZEnd32(&msg->numPlayers);
    ZEnd32(&msg->numGamesServed);
    ZEnd16(&msg->numTables);
    ZEnd16(&msg->numTablesInUse);
    ZEnd16(&msg->blobsize);
    ZEnd32(&msg->numSysops);
    ZEnd32(&msg->numNotPlaying);
    ZEnd32(&msg->maxPopulation);

}


void ZGameInstanceInfoMsgZ3Endian(ZGameInstanceInfoMsgZ3* msg)
{
    ZEnd32(&msg->gameAddr);
    ZEnd16(&msg->gamePort);
    // game state is a byte
    ZEnd32(&msg->gameVersion);
    ZEnd32(&msg->numPlayers);
    ZEnd32(&msg->numGamesServed);
    ZEnd16(&msg->serviceType);
}



void ZGameServerInfoMsgZ2Endian(ZGameServerInfoMsgZ2* msg)
{
    ZEnd32(&msg->protocolSignature);
    ZEnd32(&msg->protocolVersion);        /* Protocol version */
    ZEnd32(&msg->gameVersion);
    ZEnd32(&msg->gameID);
    ZEnd32(&msg->gameAddr);
    ZEnd16(&msg->gamePort);
    ZEnd16(&msg->gameState);
    ZEnd32(&msg->numPlayers);
    ZEnd32(&msg->numGamesServed);
}



int ZGameInfoInit(uint16 port)
{

    if (SOCKET_ERROR==g_Broadcast.BindExcludePort(port)) {

        return -1;
    }

    return 0;

}

int ZGameInfoClose()
{

    if (SOCKET_ERROR==g_Broadcast.Close()) {

        return -1;
    }

    return 0;
}

int ZGameInfoSendTo(uint32 addr, uint16 port, ZGameServerInfoMsg* msg, uint16 size )
{
    int ret = 0;

    ret = g_Broadcast.SendTo(msg, size, port, addr);
    return ret;
};

