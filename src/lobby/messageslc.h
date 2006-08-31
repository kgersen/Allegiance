/*-------------------------------------------------------------------------
 * \src\Lobby\MessagesLC.h
 * 
 * Interface between Lobby and Allegiance
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _MESSAGES_LC_H_ 
#define _MESSAGES_LC_H_ 

#include "MessageCore.h"
#define LOBBYVER 9

 /*
  *************************************************
                 MESSAGES START HERE  

         Messages range for this file is 251 - 300

  *************************************************

  Message prefixes:
    L_  lobby to client
    C_  client to lobby
    LC_ both ways
*/

// We've changed the logon message but need to let older clients connect and 
// auto-update.  We should remove this post-beta.  
DEFINE_FEDMSG(C, LOGON_LOBBY_OLD, 251) // if the lobby is in club mode, everyone has to have a valid ticket, otherwise the ticket is ignored
  FM_VAR_ITEM(ZoneTicket);    // Encrypted
  short verLobby;    
  int   crcFileList; 
  DWORD dwTime;
END_FEDMSG

DEFINE_FEDMSG(C, LOGOFF_LOBBY, 252) // note: we don't care whether they joined a game or quit altogether
END_FEDMSG

// When a client receives ART_SERVER_INFO, it logs off
DEFINE_FEDMSG(L, AUTO_UPDATE_INFO, 253)
  FM_VAR_ITEM(FTPSite); // FTP Server
  FM_VAR_ITEM(FTPInitialDirectory);
  FM_VAR_ITEM(FTPAccount); 
  FM_VAR_ITEM(FTPPassword); 
  int         crcFileList; // correct version of server's filelist
  unsigned    nFileListSize;
END_FEDMSG

DEFINE_FEDMSG(C, CREATE_MISSION_REQ, 254) // client should lock ui until it gets back an ack or nack
END_FEDMSG

DEFINE_FEDMSG(L, CREATE_MISSION_ACK, 255) 
  DWORD dwCookie; // when the client gets the lobby mission info for this guy, he's free to join. Maybe make that more explicit?
END_FEDMSG

DEFINE_FEDMSG(L, CREATE_MISSION_NACK, 256) // the mission could not be created, because there are no available servers
END_FEDMSG

DEFINE_FEDMSG(C, JOIN_GAME_REQ, 257) // successful request is responded w/ a JOIN_MISSION
  DWORD dwCookie;
END_FEDMSG

DEFINE_FEDMSG(L, JOIN_GAME_NACK, 259)
END_FEDMSG

DEFINE_FEDMSG(L, JOIN_MISSION, 260)
  DWORD dwCookie; // client should ignore this message unless it's the cookie for the most recent join/create request 
  char szServer[16];
  DWORD dwPort;			// mdvalley: Lobby passes the server's port to the client
  GUID guidInstance;
END_FEDMSG    

DEFINE_FEDMSG(L, LOGON_ACK, 261) 
  DWORD dwTimeOffset;
END_FEDMSG

DEFINE_FEDMSG(L, LOGON_NACK, 262)  // tells client that theyt can't join the lobby.
  FM_VAR_ITEM(Reason);
  bool fRetry;
END_FEDMSG

DEFINE_FEDMSG(C, FIND_PLAYER, 263)
  FM_VAR_ITEM(szCharacterName); 
END_FEDMSG

DEFINE_FEDMSG(L, FOUND_PLAYER, 264)
  DWORD         dwCookie;
END_FEDMSG

//
// If LOGON_LOBBY ever changes, existing clients will not be able to AutoUpdate to this lobby's version.
// unless we pull another LOGON_LOBBY_OLD trick.  
//

#define CL_LOGON_KEY ("%x:%s: Corrupt artwork file ") // just to confuse a hacker

DEFINE_FEDMSG(C, LOGON_LOBBY, 265) // if the lobby is in club mode, everyone has to have a valid ticket, otherwise the ticket is ignored
  FM_VAR_ITEM(ZoneTicket);    // Encrypted
  FM_VAR_ITEM(ASGS_Ticket);         // wlp 2006 - used to be CdKey
  short verLobby;    
  int   crcFileList; 
  DWORD dwTime;
  char  szName[c_cbName];
END_FEDMSG

#endif // _MESSAGES_LC_H_

