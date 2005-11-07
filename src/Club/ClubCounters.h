#ifndef _CLUBCOUNTERS_H_
#define _CLUBCOUNTERS_H_

typedef struct _CLUB_COUNTERS
{
    DWORD cLogins;
    DWORD cLogoffs;
    DWORD cPlayersDropped;
    DWORD cPlayers;
    DWORD cInboundQueueLength;
    DWORD cInboundQueueSize;
    DWORD cOutboundQueueLength;
    DWORD cOutboundQueueSize;
    DWORD timeInnerLoop;
} CLUB_COUNTERS;

#endif


