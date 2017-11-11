/*-------------------------------------------------------------------------
 * \src\Club\ClubMessages.h
 * 
 * Interface between AllClub and Allegiance
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _MESSAGES_ALLCLUB_H_ 
#define _MESSAGES_ALLCLUB_H_ 

#include <Squad.h>
#include <Member.h>
#include "messagecore.h"

#define ALLCLUBVER 10

#ifndef CivID
typedef short CivID;
#endif

#ifndef RankID
typedef short RankID;
#endif

 /*
  *************************************************
                 MESSAGES START HERE  

         Messages range for this file is 321 - 400

  *************************************************

  Message prefixes:
    S_  AllClub to client
    C_  client to AllClub
    CS_ both ways
*/

DEFINE_FEDMSG(C, LOGON_CLUB, 321) 
  FM_VAR_ITEM(CharacterName); 
  FM_VAR_ITEM(ZoneTicket);    // Encrypted
  short verClub;    
END_FEDMSG

DEFINE_FEDMSG(C, LOGOFF_CLUB, 322) // note: we don't care whether they joined a game or quit altogether
END_FEDMSG

DEFINE_FEDMSG(S, LOGON_CLUB_ACK, 323) 
  int nMemberID;
END_FEDMSG

DEFINE_FEDMSG(S, LOGON_CLUB_NACK, 324)  // tells client that they can't join the Club server
  FM_VAR_ITEM(Reason);
  bool fRetry;
END_FEDMSG                                  

/****************************************************************************
  REQ_CHARACTER_INFO

  This message is sent when a zone character requests info about someone.
  Server responds with CHARACTER_INFO_GENERAL
  along with CHARACTER_INFO_BY_CIV messages.
*****************************************************************************/
DEFINE_FEDMSG(C, REQ_CHARACTER_INFO, 327)
  int nMemberID; // if -1, current member is used
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_BY_CIV

  This message is sent as a response to REQ_CHARACTER_INFO.  All info
  is specific to a particular civ (or a sum of all civs).
*****************************************************************************/
DEFINE_FEDMSG(S, CHARACTER_INFO_BY_CIV, 329)
  FM_VAR_ITEM(szLastPlayed);     
  CivID             civid;   // -1 means all civs
  short             nRank;
  float             fScore;
  short             cBaseCaptures;
  short             cBaseKills;
  short             nRating;
  short             cMinutesPlayed;
  short             cKills;
  short             cDeaths;
  short             cWins;
  short             cLosses;
  int               nPromotionAt;
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_GENERAL

  This message is sent as a response to REQ_CHARACTER_INFO.  
  It contains info that is not civ specific except medals.
*****************************************************************************/
DEFINE_FEDMSG(S, CHARACTER_INFO_GENERAL, 330)
  FM_VAR_ITEM(szName);     
  FM_VAR_ITEM(szDescription);     
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_MEDAL

  This message is sent as a response to REQ_CHARACTER_INFO.  
  It contains info that is civ specific also related to medals.
*****************************************************************************/
DEFINE_FEDMSG(S, CHARACTER_INFO_MEDAL, 331)
  FM_VAR_ITEM(szName);     
  FM_VAR_ITEM(szDescription);
  FM_VAR_ITEM(szSpecificInfo);
  FM_VAR_ITEM(szBitmap);     
  int         nSortRank;
  CivID       civid;
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_EDIT_DESCRIPTION

  Client wants to change his description.
*****************************************************************************/
DEFINE_FEDMSG(C, CHARACTER_INFO_EDIT_DESCRIPTION, 332)
  FM_VAR_ITEM(szDescription);
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_SQUAD

  This message is sent as a response to REQ_CHARACTER_INFO.  
  Several CHARACTER_INFO_SQUAD messages comprise all the squads the character
  belong to.
*****************************************************************************/
DEFINE_FEDMSG(S, CHARACTER_INFO_SQUAD, 334)
  FM_VAR_ITEM(szSquadName);
  int         nSquadID;
END_FEDMSG

/****************************************************************************
  CHARACTER_INFO_METAL

  This message is sent as a response to REQ_CHARACTER_INFO.  
  Several CHARACTER_INFO_METAL messages comprise all the metals the character
  has earned.
*****************************************************************************/
DEFINE_FEDMSG(S, CHARACTER_INFO_METAL, 335)
  FM_VAR_ITEM(szMetalName);
  FM_VAR_ITEM(szDescription);
  FM_VAR_ITEM(szSquadName);
  int         nRanking;   // not to be confused with player ranking
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_TEXT_MESSAGE, 336)  // general text message sent to user at squads screen, usually an error
  FM_VAR_ITEM(szMsg);
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_NEXT_PAGE_ALL, 337)
  SQUAD_SORT_COLUMN column;
  int               nSquadID;  // -1 means top
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_NEXT_PAGE_MY, 338) 
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_NEXT_PAGE_DUDEX, 339) 
  int               nMemberID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_PAGE_FIND, 340)  // find squad based on a partial name
  FM_VAR_ITEM(szPartialName);
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_DUDEX_LIST, 341)
//  This message is sent when an interactive user presses on the My or DudeX tab.
//  Server Responds with several SQUAD_INFO messages.  The squads sent are the ones associated with DudeX.
  int nMemberID; // Same Member ID as Interactive User for "My" tab
END_FEDMSG


DEFINE_FEDMSG(C, SQUAD_CREATE, 342)
  FM_VAR_ITEM(szName);
  FM_VAR_ITEM(szDescription);
  FM_VAR_ITEM(szURL);
  CivID       civid;
  int         nClientSquadID;
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_CREATE_ACK, 343)  // map client's squad ID to the true squad ID, determined by Zone database server
  int         nClientSquadID;
  int         nSquadID;     // true ID
  bool        bSuccessful;  // was create successful
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_EDIT, 344)
  FM_VAR_ITEM(szDescription); 
  FM_VAR_ITEM(szURL); 
  CivID       civid;
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_DETAILS_REQ, 345)
// Server Responds with SQUAD_DETAILS
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_LOG_REQ, 346)
//  Server Responds with a LOG_INFO
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_MAKE_JOIN_REQUEST, 348)
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_CANCEL_JOIN_REQUEST, 349)
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_REJECT_JOIN_REQUEST, 350)
  int nMemberID; 
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_ACCEPT_JOIN_REQUEST, 351)
  int nMemberID; 
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_BOOT_MEMBER, 352)
  int nMemberID; 
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_QUIT, 353)
  int nSquadID;                 
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_PROMOTE_TO_ASL, 354)
  int nMemberID; 
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_DEMOTE_TO_MEMBER, 355)
  int nMemberID; 
  int nSquadID;
END_FEDMSG

DEFINE_FEDMSG(C, SQUAD_TRANSFER_CONTROL, 356)
// Previous leader becomes an ASL
  int nMemberID; 
  int nSquadID;
END_FEDMSG


DEFINE_FEDMSG(S, SQUAD_INFO, 358)
  FM_VAR_ITEM(szName);
  bool  bSearchResult:1;  // true iff result of a search
  unsigned char column:3;         // ranking is always in terms of a column
  int   nSquadID;
  int   nRanking;
  short cWins;
  short cLosses;
  short nScore; 
//  float fHours; // hours that interactive user has played on this squad
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_INFO_DUDEX, 359)
  FM_VAR_ITEM(szName);
  bool  bMyTab;           // false for DudeX tab
  int   nSquadID;
  short cWins;
  short cLosses;
  short nScore; 
//  float fHours; // hours that interactive user has played on this squad
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_DETAILS, 361)
  FM_VAR_ITEM(szDescription);
  FM_VAR_ITEM(szURL);
  FM_VAR_ITEM(szInceptionDate);
  int   nSquadID;       // used to ensure robustness
  CivID civid;
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_DETAILS_PLAYER, 362) // these aways follow a SQUAD_DETAILS message
  FM_VAR_ITEM(szName);
  FM_VAR_ITEM(szLastPlayed);
  int                           nMemberID; 
  IMemberBase::Rank             sqRank;
  IMemberBase::DetailedStatus   detailedstatus;
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_LOG_INFO, 363)
  FM_VAR_ITEM(LogInfo);
  int   nSquadID;       // used to ensure robustness
END_FEDMSG

DEFINE_FEDMSG(S, SQUAD_DELETED, 364)
  int nSquadID;
END_FEDMSG

enum LeaderBoardSort
{
    lbSortName,
    lbSortRank,
    lbSortRating,
    lbSortScore,
    lbSortTimePlayed,
    lbSortBaseKills,
    lbSortBaseCaptures,
    lbSortKills,
    lbSortGamesPlayed,
    lbSortDeaths,
    lbSortWins,
    lbSortLosses,
    lbSortCommanderWins
};

DEFINE_FEDMSG(C, LEADER_BOARD_QUERY, 365)
    char            szBasis[c_cbName]; // the character to use as a refrence 
                                       // point for the location in the list,
                                       // or "" to use idBasis.
    int             idBasis;           // the id of the basis character, or -1 for the top of the list
    CivID           civid;             // the civ to retrieve scores from, or NA for any civ
    LeaderBoardSort sort;              // the sort to use
END_FEDMSG

struct LeaderBoardEntry
{
    char                        CharacterName[c_cbName];
    int                         idCharacter;

    int                         nPosition;

    float                       fTotalScore;
    int                         nMinutesPlayed;
    short                       cTotalBaseKills;
    short                       cTotalBaseCaptures;
    short                       cTotalKills;
    short                       cTotalDeaths;

    short                       rank;
    short                       rating;

    unsigned short              cTotalWins;
    unsigned short              cTotalLosses;
    unsigned short              cTotalGamesPlayed;
    unsigned short              cTotalCommanderWins;
};

DEFINE_FEDMSG(S, LEADER_BOARD_QUERY_FAIL, 366)
    char            szBasis[c_cbName]; // the character we failed to find
END_FEDMSG

DEFINE_FEDMSG(S, LEADER_BOARD_LIST, 367)
  FM_VAR_ITEM(ventryPlayers); // an array of LeaderBoardEntry structures (first entry is number 1)
  int cEntries;
  LeaderBoardEntry entryNumber1; // the highest or lowest ranked player
END_FEDMSG

#ifndef _MESSAGES_ 
// keep in sync with copy in Messages.h
struct RankInfo
{
    int     requiredRanking;
    CivID   civ;
    RankID  rank;
    char    RankName[c_cbName];
};
#endif //_MESSAGES_ 

DEFINE_FEDMSG(S, CLUB_RANK_INFO, 368)
  FM_VAR_ITEM(ranks); // an array of RankInfo sorted by civ, then rank
  short   cRanks;
END_FEDMSG

#endif // _MESSAGES_ALLCLUB_H_

