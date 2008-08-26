/*-------------------------------------------------------------------------
  clubqueries2.h
  
  New async query definitions for allclub
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _CLUBQUERIES2_
#define _CLUBQUERIES2_

#include "allegdb.h"

// ******************************************************************
// New-style asynchronous queries
// ******************************************************************

// Leaderboard by name
BEGIN_QUERY(GetNearNames, true, 
  TEXT("{call GetNearNames(?, ?, ?)}"))

  // IN
  CharacterID characterIDIn;
  CivID       civIDIn;
  char        szNameIn[c_cbName];

  // OUT
  long        ordinalFirst;
  CharacterID characterIDOut;
  char        szNameOut[c_cbName];
  CivID       civIDOut;
  RankID      rank;
  float       score;
  short       minPlayed;
  short       cBaseKills;
  short       cBaseCaptures;
  short       cKills;
  short       rating;
  short       cGamesPlayed;
  short       cDeaths;
  short       cWins;
  short       cWinsCmd;
  short       cLosses;
  short       cLossesCmd;

  BEGIN_COLUMN_MAP(CQGetNearNamesData)
    COLUMN_ENTRY_TYPE(1,  DBTYPE_I4,  ordinalFirst)
    COLUMN_ENTRY_TYPE(2,  DBTYPE_I4,  characterIDOut)
    COLUMN_ENTRY_TYPE(3,  DBTYPE_STR, szNameOut)
    COLUMN_ENTRY_TYPE(4,  DBTYPE_I2,  civIDOut)
    COLUMN_ENTRY_TYPE(5,  DBTYPE_I2,  rank)
    COLUMN_ENTRY_TYPE(6,  DBTYPE_R4,  score)
    COLUMN_ENTRY_TYPE(7,  DBTYPE_I2,  minPlayed)
    COLUMN_ENTRY_TYPE(8,  DBTYPE_I2,  cBaseKills)
    COLUMN_ENTRY_TYPE(9,  DBTYPE_I2,  cBaseCaptures)
    COLUMN_ENTRY_TYPE(10, DBTYPE_I2,  cKills)
    COLUMN_ENTRY_TYPE(11, DBTYPE_I2,  rating)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cGamesPlayed)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cDeaths)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cWins)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cWinsCmd)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cLosses)
    COLUMN_ENTRY_TYPE(12, DBTYPE_I2,  cLossesCmd)
  END_COLUMN_MAP()

  BEGIN_PARAM_MAP(CQGetNearNamesData)
    COLUMN_ENTRY_TYPE(1,  DBTYPE_I4,  characterIDIn)
    COLUMN_ENTRY_TYPE(2,  DBTYPE_I2,  civIDIn)
    COLUMN_ENTRY_TYPE(3,  DBTYPE_STR, szNameIn)
  END_PARAM_MAP()
END_QUERY(CQLogonStats, true)


#endif

