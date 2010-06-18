/***********************************************************************
 Imago 6/10 Query Winsock Crap
***********************************************************************/

#if !defined(WS_UTIL_H)
#define WS_UTIL_H

extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix,
        int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);

#endif // !defined (WS_UTIL_H)

