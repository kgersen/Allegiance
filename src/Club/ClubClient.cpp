/*-------------------------------------------------------------------------
  ClubClient.cpp
  
  Per client stuff, including site for Clients session
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "ClubQueries.h"

const DWORD CFLClient::c_dwID = 19680815;

const char * szNotMemberMessage = "Your request could not be completed because you are not currently signed up as an Allegiance Zone Member.";

namespace 
{
  // The most play time we want to ever show is 9999 hours, or 599940 minutes
  int MaxMinutes(int minutes)
  {
    const int cMinutesMax = 599940;
    if (minutes < cMinutesMax)
      return minutes;

    return cMinutesMax;
  }


  // remove trailing whitespaces from a string
  void RemoveTrailingSpaces(char* sz)
  {
    for (int nIndex = strlen(sz); nIndex > 0 && sz[nIndex - 1] == ' '; --nIndex)
    {
      sz[nIndex - 1] = '\0';    
    }
  };

  // map character name to character ID (and fix capitialization of character name)
  int FindCharacterIDAndCapitalization(char* szName)
  {
    strncpy((char*)CharIDAndCapitalization_Name, szName, c_cbName - 1);
    CharID_CharacterName[c_cbName - 1] = '\0';
    SQL_GO(CharIDAndCapitalization);
    if (SQL_SUCCEEDED(SQL_GETROW(CharIDAndCapitalization)))
    {
      strcpy(szName, (char*)CharIDAndCapitalization_ProperName);
      return CharIDAndCapitalization_CharacterId;
    }
    else
    {
      return -1;
    }
  }

  // map character ID to character name
  const char* FindCharacterName(int id)
  {
    CharInfoGeneral_CharacterID = id;
    SQL_GO(CharInfoGeneral);
    if (SQL_SUCCEEDED(SQL_GETROW(CharInfoGeneral)))
    {
      return (const char*)CharInfoGeneral_Name;
    }
    else
    {
      return "";
    }
  }

  // is there a player in the results list with the given name?
  bool HasPlayerNamed(const char* szName, LeaderBoardEntry* vResult, int cResults)
  {
    for (int i = 0; i < cResults; i++)
    {
      if (_strnicmp(szName, vResult[i].CharacterName, c_cbName) == 0)
        return true;
    }
    return false;
  }

  // is there a player in the results list with the given character id?
  bool HasCharacterID(int charID, LeaderBoardEntry* vResult, int cResults)
  {
    // HACK: always return true for id -1
    if (charID == -1)
      return true;

    for (int i = 0; i < cResults; i++)
    {
      if (charID == vResult[i].idCharacter)
        return true;
    }
    return false;
  }

  // create an entry for the given character - you can pass in either name or id, but
  // the other one must be "" or -1 respectively.  
  void ForgeResultEntry(LeaderBoardEntry& entry, const char* szName, int id, int nPosition)
  {
    if (szName[0] != '\0')
    {
      assert(id = -1);
      strncpy(entry.CharacterName, szName, c_cbName);
      id = FindCharacterIDAndCapitalization(entry.CharacterName);
    }
    else
    {
      strncpy(entry.CharacterName, FindCharacterName(id), c_cbName);
    }
    RemoveTrailingSpaces(entry.CharacterName);
    entry.idCharacter = id;
    entry.nPosition = nPosition;
    entry.fTotalScore = 0;
    entry.nMinutesPlayed = 0;
    entry.cTotalBaseKills = 0;
    entry.cTotalBaseCaptures = 0;
    entry.cTotalKills = 0;
    entry.cTotalDeaths = 0;
    entry.rank = 0;
    entry.rating = 0;
    entry.cTotalWins = 0;
    entry.cTotalLosses = 0;
    entry.cTotalGamesPlayed = 0;
    entry.cTotalCommanderWins = 0;
  }

  /*
  int GetResultsByXXX(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by XXX in this civ
    GetTopXXX_CivIDIn = civID;
    GetTopXXX_CivIDIn2 = civID;
    SQL_GO(GetTopXXX);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopXXX)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopXXX_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopXXX_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopXXX_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopXXX_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopXXX_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopXXX_BaseCaptures;
      topPlayer.cTotalKills = GetTopXXX_Kills;
      topPlayer.cTotalDeaths = GetTopXXX_Deaths;
      topPlayer.rank = GetTopXXX_Rank;
      topPlayer.rating = GetTopXXX_Rating;
      topPlayer.cTotalWins = GetTopXXX_Wins;
      topPlayer.cTotalLosses = GetTopXXX_Losses;
      topPlayer.cTotalCommanderWins = GetTopXXX_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopXXX_GamesPlayed;
    }

    // get the players near the character in question
    GetNearXXX_CharacterIDIn = characterId;
    GetNearXXX_CivIDIn = civID;
    strcpy((char*)GetNearXXX_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearXXX);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearXXX)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearXXX_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearXXX_CharacterID;
      vResults[cResults].nPosition = GetNearXXX_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearXXX_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearXXX_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearXXX_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearXXX_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearXXX_Kills;
      vResults[cResults].cTotalDeaths = GetNearXXX_Deaths;
      vResults[cResults].rank = GetNearXXX_Rank;
      vResults[cResults].rating = GetNearXXX_Rating;
      vResults[cResults].cTotalWins = GetNearXXX_Wins;
      vResults[cResults].cTotalLosses = GetNearXXX_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearXXX_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearXXX_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }
  */

  int GetResultsByName(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Name in this civ
    GetTopName_CivIDIn = civID;
    GetTopName_CivIDIn2 = civID;
    SQL_GO(GetTopName);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopName)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopName_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopName_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopName_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopName_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopName_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopName_BaseCaptures;
      topPlayer.cTotalKills = GetTopName_Kills;
      topPlayer.cTotalDeaths = GetTopName_Deaths;
      topPlayer.rank = GetTopName_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopName_Rating);
      topPlayer.cTotalWins = GetTopName_Wins;
      topPlayer.cTotalLosses = GetTopName_Losses;
      topPlayer.cTotalCommanderWins = GetTopName_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopName_GamesPlayed;
    }

    // get the players near the character in question
    GetNearName_CharacterIDIn = characterId;
    GetNearName_CivIDIn = civID;
    strcpy((char*)GetNearName_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearName);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearName)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearName_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearName_CharacterID;
      vResults[cResults].nPosition = GetNearName_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearName_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearName_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearName_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearName_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearName_Kills;
      vResults[cResults].cTotalDeaths = GetNearName_Deaths;
      vResults[cResults].rank = GetNearName_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearName_Rating);
      vResults[cResults].cTotalWins = GetNearName_Wins;
      vResults[cResults].cTotalLosses = GetNearName_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearName_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearName_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByRank(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Rank in this civ
    GetTopRank_CivIDIn = civID;
    GetTopRank_CivIDIn2 = civID;
    SQL_GO(GetTopRank);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopRank)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopRank_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopRank_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopRank_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopRank_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopRank_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopRank_BaseCaptures;
      topPlayer.cTotalKills = GetTopRank_Kills;
      topPlayer.cTotalDeaths = GetTopRank_Deaths;
      topPlayer.rank = GetTopRank_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopRank_Rating);
      topPlayer.cTotalWins = GetTopRank_Wins;
      topPlayer.cTotalLosses = GetTopRank_Losses;
      topPlayer.cTotalCommanderWins = GetTopRank_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopRank_GamesPlayed;
    }

    // get the players near the character in question
    GetNearRank_CharacterIDIn = characterId;
    GetNearRank_CivIDIn = civID;
    strcpy((char*)GetNearRank_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearRank);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearRank)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearRank_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearRank_CharacterID;
      vResults[cResults].nPosition = GetNearRank_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearRank_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearRank_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearRank_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearRank_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearRank_Kills;
      vResults[cResults].cTotalDeaths = GetNearRank_Deaths;
      vResults[cResults].rank = GetNearRank_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearRank_Rating);
      vResults[cResults].cTotalWins = GetNearRank_Wins;
      vResults[cResults].cTotalLosses = GetNearRank_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearRank_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearRank_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByRating(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Rating in this civ
    GetTopRating_CivIDIn = civID;
    GetTopRating_CivIDIn2 = civID;
    SQL_GO(GetTopRating);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopRating)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopRating_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopRating_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopRating_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopRating_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopRating_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopRating_BaseCaptures;
      topPlayer.cTotalKills = GetTopRating_Kills;
      topPlayer.cTotalDeaths = GetTopRating_Deaths;
      topPlayer.rank = GetTopRating_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopRating_Rating);
      topPlayer.cTotalWins = GetTopRating_Wins;
      topPlayer.cTotalLosses = GetTopRating_Losses;
      topPlayer.cTotalCommanderWins = GetTopRating_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopRating_GamesPlayed;
    }

    // get the players near the character in question
    GetNearRating_CharacterIDIn = characterId;
    GetNearRating_CivIDIn = civID;
    strcpy((char*)GetNearRating_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearRating);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearRating)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearRating_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearRating_CharacterID;
      vResults[cResults].nPosition = GetNearRating_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearRating_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearRating_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearRating_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearRating_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearRating_Kills;
      vResults[cResults].cTotalDeaths = GetNearRating_Deaths;
      vResults[cResults].rank = GetNearRating_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearRating_Rating);
      vResults[cResults].cTotalWins = GetNearRating_Wins;
      vResults[cResults].cTotalLosses = GetNearRating_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearRating_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearRating_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByScore(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Score in this civ
    GetTopScore_CivIDIn = civID;
    GetTopScore_CivIDIn2 = civID;
    SQL_GO(GetTopScore);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopScore)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopScore_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopScore_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopScore_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopScore_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopScore_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopScore_BaseCaptures;
      topPlayer.cTotalKills = GetTopScore_Kills;
      topPlayer.cTotalDeaths = GetTopScore_Deaths;
      topPlayer.rank = GetTopScore_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopScore_Rating);
      topPlayer.cTotalWins = GetTopScore_Wins;
      topPlayer.cTotalLosses = GetTopScore_Losses;
      topPlayer.cTotalCommanderWins = GetTopScore_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopScore_GamesPlayed;
    }

    // get the players near the character in question
    GetNearScore_CharacterIDIn = characterId;
    GetNearScore_CivIDIn = civID;
    strcpy((char*)GetNearScore_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearScore);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearScore)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearScore_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearScore_CharacterID;
      vResults[cResults].nPosition = GetNearScore_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearScore_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearScore_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearScore_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearScore_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearScore_Kills;
      vResults[cResults].cTotalDeaths = GetNearScore_Deaths;
      vResults[cResults].rank = GetNearScore_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearScore_Rating);
      vResults[cResults].cTotalWins = GetNearScore_Wins;
      vResults[cResults].cTotalLosses = GetNearScore_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearScore_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearScore_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByTimePlayed(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by TimePlayed in this civ
    GetTopTimePlayed_CivIDIn = civID;
    GetTopTimePlayed_CivIDIn2 = civID;
    SQL_GO(GetTopTimePlayed);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopTimePlayed)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopTimePlayed_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopTimePlayed_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopTimePlayed_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopTimePlayed_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopTimePlayed_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopTimePlayed_BaseCaptures;
      topPlayer.cTotalKills = GetTopTimePlayed_Kills;
      topPlayer.cTotalDeaths = GetTopTimePlayed_Deaths;
      topPlayer.rank = GetTopTimePlayed_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopTimePlayed_Rating);
      topPlayer.cTotalWins = GetTopTimePlayed_Wins;
      topPlayer.cTotalLosses = GetTopTimePlayed_Losses;
      topPlayer.cTotalCommanderWins = GetTopTimePlayed_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopTimePlayed_GamesPlayed;
    }

    // get the players near the character in question
    GetNearTimePlayed_CharacterIDIn = characterId;
    GetNearTimePlayed_CivIDIn = civID;
    strcpy((char*)GetNearTimePlayed_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearTimePlayed);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearTimePlayed)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearTimePlayed_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearTimePlayed_CharacterID;
      vResults[cResults].nPosition = GetNearTimePlayed_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearTimePlayed_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearTimePlayed_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearTimePlayed_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearTimePlayed_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearTimePlayed_Kills;
      vResults[cResults].cTotalDeaths = GetNearTimePlayed_Deaths;
      vResults[cResults].rank = GetNearTimePlayed_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearTimePlayed_Rating);
      vResults[cResults].cTotalWins = GetNearTimePlayed_Wins;
      vResults[cResults].cTotalLosses = GetNearTimePlayed_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearTimePlayed_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearTimePlayed_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByBaseKills(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by BaseKills in this civ
    GetTopBaseKills_CivIDIn = civID;
    GetTopBaseKills_CivIDIn2 = civID;
    SQL_GO(GetTopBaseKills);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopBaseKills)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopBaseKills_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopBaseKills_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopBaseKills_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopBaseKills_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopBaseKills_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopBaseKills_BaseCaptures;
      topPlayer.cTotalKills = GetTopBaseKills_Kills;
      topPlayer.cTotalDeaths = GetTopBaseKills_Deaths;
      topPlayer.rank = GetTopBaseKills_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopBaseKills_Rating);
      topPlayer.cTotalWins = GetTopBaseKills_Wins;
      topPlayer.cTotalLosses = GetTopBaseKills_Losses;
      topPlayer.cTotalCommanderWins = GetTopBaseKills_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopBaseKills_GamesPlayed;
    }

    // get the players near the character in question
    GetNearBaseKills_CharacterIDIn = characterId;
    GetNearBaseKills_CivIDIn = civID;
    strcpy((char*)GetNearBaseKills_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearBaseKills);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearBaseKills)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearBaseKills_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearBaseKills_CharacterID;
      vResults[cResults].nPosition = GetNearBaseKills_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearBaseKills_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearBaseKills_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearBaseKills_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearBaseKills_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearBaseKills_Kills;
      vResults[cResults].cTotalDeaths = GetNearBaseKills_Deaths;
      vResults[cResults].rank = GetNearBaseKills_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearBaseKills_Rating);
      vResults[cResults].cTotalWins = GetNearBaseKills_Wins;
      vResults[cResults].cTotalLosses = GetNearBaseKills_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearBaseKills_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearBaseKills_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByBaseCaptures(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by BaseCaptures in this civ
    GetTopBaseCaptures_CivIDIn = civID;
    GetTopBaseCaptures_CivIDIn2 = civID;
    SQL_GO(GetTopBaseCaptures);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopBaseCaptures)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopBaseCaptures_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopBaseCaptures_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopBaseCaptures_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopBaseCaptures_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopBaseCaptures_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopBaseCaptures_BaseCaptures;
      topPlayer.cTotalKills = GetTopBaseCaptures_Kills;
      topPlayer.cTotalDeaths = GetTopBaseCaptures_Deaths;
      topPlayer.rank = GetTopBaseCaptures_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopBaseCaptures_Rating);
      topPlayer.cTotalWins = GetTopBaseCaptures_Wins;
      topPlayer.cTotalLosses = GetTopBaseCaptures_Losses;
      topPlayer.cTotalCommanderWins = GetTopBaseCaptures_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopBaseCaptures_GamesPlayed;
    }

    // get the players near the character in question
    GetNearBaseCaptures_CharacterIDIn = characterId;
    GetNearBaseCaptures_CivIDIn = civID;
    strcpy((char*)GetNearBaseCaptures_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearBaseCaptures);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearBaseCaptures)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearBaseCaptures_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearBaseCaptures_CharacterID;
      vResults[cResults].nPosition = GetNearBaseCaptures_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearBaseCaptures_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearBaseCaptures_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearBaseCaptures_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearBaseCaptures_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearBaseCaptures_Kills;
      vResults[cResults].cTotalDeaths = GetNearBaseCaptures_Deaths;
      vResults[cResults].rank = GetNearBaseCaptures_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearBaseCaptures_Rating);
      vResults[cResults].cTotalWins = GetNearBaseCaptures_Wins;
      vResults[cResults].cTotalLosses = GetNearBaseCaptures_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearBaseCaptures_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearBaseCaptures_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByKills(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Kills in this civ
    GetTopKills_CivIDIn = civID;
    GetTopKills_CivIDIn2 = civID;
    SQL_GO(GetTopKills);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopKills)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopKills_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopKills_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopKills_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopKills_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopKills_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopKills_BaseCaptures;
      topPlayer.cTotalKills = GetTopKills_Kills;
      topPlayer.cTotalDeaths = GetTopKills_Deaths;
      topPlayer.rank = GetTopKills_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopKills_Rating);
      topPlayer.cTotalWins = GetTopKills_Wins;
      topPlayer.cTotalLosses = GetTopKills_Losses;
      topPlayer.cTotalCommanderWins = GetTopKills_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopKills_GamesPlayed;
    }

    // get the players near the character in question
    GetNearKills_CharacterIDIn = characterId;
    GetNearKills_CivIDIn = civID;
    strcpy((char*)GetNearKills_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearKills);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearKills)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearKills_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearKills_CharacterID;
      vResults[cResults].nPosition = GetNearKills_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearKills_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearKills_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearKills_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearKills_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearKills_Kills;
      vResults[cResults].cTotalDeaths = GetNearKills_Deaths;
      vResults[cResults].rank = GetNearKills_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearKills_Rating);
      vResults[cResults].cTotalWins = GetNearKills_Wins;
      vResults[cResults].cTotalLosses = GetNearKills_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearKills_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearKills_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByDeaths(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Deaths in this civ
    GetTopDeaths_CivIDIn = civID;
    GetTopDeaths_CivIDIn2 = civID;
    SQL_GO(GetTopDeaths);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopDeaths)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopDeaths_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopDeaths_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopDeaths_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopDeaths_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopDeaths_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopDeaths_BaseCaptures;
      topPlayer.cTotalKills = GetTopDeaths_Kills;
      topPlayer.cTotalDeaths = GetTopDeaths_Deaths;
      topPlayer.rank = GetTopDeaths_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopDeaths_Rating);
      topPlayer.cTotalWins = GetTopDeaths_Wins;
      topPlayer.cTotalLosses = GetTopDeaths_Losses;
      topPlayer.cTotalCommanderWins = GetTopDeaths_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopDeaths_GamesPlayed;
    }

    // get the players near the character in question
    GetNearDeaths_CharacterIDIn = characterId;
    GetNearDeaths_CivIDIn = civID;
    strcpy((char*)GetNearDeaths_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearDeaths);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearDeaths)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearDeaths_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearDeaths_CharacterID;
      vResults[cResults].nPosition = GetNearDeaths_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearDeaths_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearDeaths_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearDeaths_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearDeaths_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearDeaths_Kills;
      vResults[cResults].cTotalDeaths = GetNearDeaths_Deaths;
      vResults[cResults].rank = GetNearDeaths_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearDeaths_Rating);
      vResults[cResults].cTotalWins = GetNearDeaths_Wins;
      vResults[cResults].cTotalLosses = GetNearDeaths_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearDeaths_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearDeaths_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByGamesPlayed(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by GamesPlayed in this civ
    GetTopGamesPlayed_CivIDIn = civID;
    GetTopGamesPlayed_CivIDIn2 = civID;
    SQL_GO(GetTopGamesPlayed);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopGamesPlayed)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopGamesPlayed_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopGamesPlayed_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopGamesPlayed_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopGamesPlayed_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopGamesPlayed_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopGamesPlayed_BaseCaptures;
      topPlayer.cTotalKills = GetTopGamesPlayed_Kills;
      topPlayer.cTotalDeaths = GetTopGamesPlayed_Deaths;
      topPlayer.rank = GetTopGamesPlayed_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopGamesPlayed_Rating);
      topPlayer.cTotalWins = GetTopGamesPlayed_Wins;
      topPlayer.cTotalLosses = GetTopGamesPlayed_Losses;
      topPlayer.cTotalCommanderWins = GetTopGamesPlayed_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopGamesPlayed_GamesPlayed;
    }

    // get the players near the character in question
    GetNearGamesPlayed_CharacterIDIn = characterId;
    GetNearGamesPlayed_CivIDIn = civID;
    strcpy((char*)GetNearGamesPlayed_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearGamesPlayed);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearGamesPlayed)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearGamesPlayed_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearGamesPlayed_CharacterID;
      vResults[cResults].nPosition = GetNearGamesPlayed_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearGamesPlayed_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearGamesPlayed_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearGamesPlayed_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearGamesPlayed_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearGamesPlayed_Kills;
      vResults[cResults].cTotalDeaths = GetNearGamesPlayed_Deaths;
      vResults[cResults].rank = GetNearGamesPlayed_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearGamesPlayed_Rating);
      vResults[cResults].cTotalWins = GetNearGamesPlayed_Wins;
      vResults[cResults].cTotalLosses = GetNearGamesPlayed_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearGamesPlayed_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearGamesPlayed_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByWins(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Wins in this civ
    GetTopWins_CivIDIn = civID;
    GetTopWins_CivIDIn2 = civID;
    SQL_GO(GetTopWins);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopWins)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopWins_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopWins_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopWins_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopWins_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopWins_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopWins_BaseCaptures;
      topPlayer.cTotalKills = GetTopWins_Kills;
      topPlayer.cTotalDeaths = GetTopWins_Deaths;
      topPlayer.rank = GetTopWins_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopWins_Rating);
      topPlayer.cTotalWins = GetTopWins_Wins;
      topPlayer.cTotalLosses = GetTopWins_Losses;
      topPlayer.cTotalCommanderWins = GetTopWins_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopWins_GamesPlayed;
    }

    // get the players near the character in question
    GetNearWins_CharacterIDIn = characterId;
    GetNearWins_CivIDIn = civID;
    strcpy((char*)GetNearWins_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearWins);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearWins)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearWins_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearWins_CharacterID;
      vResults[cResults].nPosition = GetNearWins_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearWins_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearWins_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearWins_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearWins_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearWins_Kills;
      vResults[cResults].cTotalDeaths = GetNearWins_Deaths;
      vResults[cResults].rank = GetNearWins_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearWins_Rating);
      vResults[cResults].cTotalWins = GetNearWins_Wins;
      vResults[cResults].cTotalLosses = GetNearWins_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearWins_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearWins_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByLosses(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by Losses in this civ
    GetTopLosses_CivIDIn = civID;
    GetTopLosses_CivIDIn2 = civID;
    SQL_GO(GetTopLosses);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopLosses)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopLosses_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopLosses_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopLosses_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopLosses_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopLosses_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopLosses_BaseCaptures;
      topPlayer.cTotalKills = GetTopLosses_Kills;
      topPlayer.cTotalDeaths = GetTopLosses_Deaths;
      topPlayer.rank = GetTopLosses_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopLosses_Rating);
      topPlayer.cTotalWins = GetTopLosses_Wins;
      topPlayer.cTotalLosses = GetTopLosses_Losses;
      topPlayer.cTotalCommanderWins = GetTopLosses_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopLosses_GamesPlayed;
    }

    // get the players near the character in question
    GetNearLosses_CharacterIDIn = characterId;
    GetNearLosses_CivIDIn = civID;
    strcpy((char*)GetNearLosses_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearLosses);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearLosses)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearLosses_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearLosses_CharacterID;
      vResults[cResults].nPosition = GetNearLosses_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearLosses_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearLosses_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearLosses_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearLosses_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearLosses_Kills;
      vResults[cResults].cTotalDeaths = GetNearLosses_Deaths;
      vResults[cResults].rank = GetNearLosses_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearLosses_Rating);
      vResults[cResults].cTotalWins = GetNearLosses_Wins;
      vResults[cResults].cTotalLosses = GetNearLosses_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearLosses_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearLosses_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResultsByCommanderWins(const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    // get the top player by CommanderWins in this civ
    GetTopCommanderWins_CivIDIn = civID;
    GetTopCommanderWins_CivIDIn2 = civID;
    SQL_GO(GetTopCommanderWins);
    if (!SQL_SUCCEEDED(SQL_GETROW(GetTopCommanderWins)))
    {
      return 0;
    }
    else
    {
      strcpy(topPlayer.CharacterName, (char*)GetTopCommanderWins_CharacterName);
      RemoveTrailingSpaces(topPlayer.CharacterName);
      topPlayer.idCharacter = GetTopCommanderWins_CharacterID;
      topPlayer.nPosition = 1;
      topPlayer.fTotalScore = GetTopCommanderWins_Score;
      topPlayer.nMinutesPlayed = MaxMinutes(GetTopCommanderWins_MinutesPlayed);
      topPlayer.cTotalBaseKills = GetTopCommanderWins_BaseKills;
      topPlayer.cTotalBaseCaptures = GetTopCommanderWins_BaseCaptures;
      topPlayer.cTotalKills = GetTopCommanderWins_Kills;
      topPlayer.cTotalDeaths = GetTopCommanderWins_Deaths;
      topPlayer.rank = GetTopCommanderWins_Rank;
      topPlayer.rating = RATING_EXT2DISP(GetTopCommanderWins_Rating);
      topPlayer.cTotalWins = GetTopCommanderWins_Wins;
      topPlayer.cTotalLosses = GetTopCommanderWins_Losses;
      topPlayer.cTotalCommanderWins = GetTopCommanderWins_CommanderWins;
      topPlayer.cTotalGamesPlayed = GetTopCommanderWins_GamesPlayed;
    }

    // get the players near the character in question
    GetNearCommanderWins_CharacterIDIn = characterId;
    GetNearCommanderWins_CivIDIn = civID;
    strcpy((char*)GetNearCommanderWins_CharacterNameIn, szName);

    int cResults = 0;
    SQL_GO(GetNearCommanderWins);
    while (SQL_SUCCEEDED(SQL_GETROW(GetNearCommanderWins)) && cResults < nMaxResults)
    {
      strcpy(vResults[cResults].CharacterName, (char*)GetNearCommanderWins_CharacterName);
      RemoveTrailingSpaces(vResults[cResults].CharacterName);
      vResults[cResults].idCharacter = GetNearCommanderWins_CharacterID;
      vResults[cResults].nPosition = GetNearCommanderWins_FirstOrdinal + cResults;
      vResults[cResults].fTotalScore = GetNearCommanderWins_Score;
      vResults[cResults].nMinutesPlayed = MaxMinutes(GetNearCommanderWins_MinutesPlayed);
      vResults[cResults].cTotalBaseKills = GetNearCommanderWins_BaseKills;
      vResults[cResults].cTotalBaseCaptures = GetNearCommanderWins_BaseCaptures;
      vResults[cResults].cTotalKills = GetNearCommanderWins_Kills;
      vResults[cResults].cTotalDeaths = GetNearCommanderWins_Deaths;
      vResults[cResults].rank = GetNearCommanderWins_Rank;
      vResults[cResults].rating = RATING_EXT2DISP(GetNearCommanderWins_Rating);
      vResults[cResults].cTotalWins = GetNearCommanderWins_Wins;
      vResults[cResults].cTotalLosses = GetNearCommanderWins_Losses;
      vResults[cResults].cTotalCommanderWins = GetNearCommanderWins_CommanderWins;
      vResults[cResults].cTotalGamesPlayed = GetNearCommanderWins_GamesPlayed;

      cResults++;
      assert(cResults <= nMaxResults);
    }

    return cResults;
  }

  int GetResults(LeaderBoardSort sort, const char* szName, int characterId, CivID civID, 
      LeaderBoardEntry& topPlayer, LeaderBoardEntry* vResults, int nMaxResults)
  {
    switch (sort)
    {
    default:
      assert(false);
      return 0;

    case lbSortName:
      return GetResultsByName(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortRank:
      return GetResultsByRank(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortRating:
      return GetResultsByRating(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortScore:
      return GetResultsByScore(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortTimePlayed:
      return GetResultsByTimePlayed(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortBaseKills:
      return GetResultsByBaseKills(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortBaseCaptures:
      return GetResultsByBaseCaptures(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortKills:
      return GetResultsByKills(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortGamesPlayed:
      return GetResultsByGamesPlayed(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortDeaths:
      return GetResultsByDeaths(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortWins:
      return GetResultsByWins(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortLosses:
      return GetResultsByLosses(szName, characterId, civID, topPlayer, vResults, nMaxResults);

    case lbSortCommanderWins:
      return GetResultsByCommanderWins(szName, characterId, civID, topPlayer, vResults, nMaxResults);
    }
  }

  void CheckRankInfo()
  {
    if (!g_pClubApp->GetRankInfo())
    {
      // Load the ranking info
      SQL_GO(GetRankCount);
      SQL_GETROW(GetRankCount);

      RankInfo* vRankInfo = NULL;

      if (GetRankCount_Count > 0)
      {
        vRankInfo = new RankInfo[GetRankCount_Count];

        SQL_GO(GetRanks);
        for (int i = 0; i < GetRankCount_Count; i++)
        {
          ZVerify(SQL_SUCCEEDED(SQL_GETROW(GetRanks)));
          vRankInfo[i].civ = GetRanks_CivID;
          vRankInfo[i].rank = GetRanks_Rank;
          vRankInfo[i].requiredRanking = GetRanks_Requirement;
          strcpy(vRankInfo[i].RankName, (char*)GetRanks_Name);
        }
      }

      g_pClubApp->SetRankInfo(vRankInfo, GetRankCount_Count);
    }
  }

  int GetNextPromotion(CivID civID, float fScore)
  {
    // search through the rank table to find the first rank with a score requirement greater than 
    // the current score.
    for (int nRankIndex = 0; nRankIndex < g_pClubApp->GetRankInfoCount(); ++nRankIndex)
    {
      RankInfo& rankinfo = g_pClubApp->GetRankInfo()[nRankIndex];

      if (rankinfo.civ == civID && rankinfo.requiredRanking > fScore)
        return rankinfo.requiredRanking;
    }

    return 0;
  }

};



//////////////////////////////////////////////////////////////////////////////////////////
//
// SendSquadPage()
//
//////////////////////////////////////////////////////////////////////////////////////////
void SendSquadPage(FedMessaging * pthis, CFMConnection & cnxnFrom, SQUAD_SORT_COLUMN column, int nSquadID)
{
    // let's not crash the server
    if (column <= SSC_UNKNOWN || column >= SSC_MAX-1) // TODO: remove -1 once Hours sort works
        return;

    // setup query
    SquadGetNear_INPUT_Column      = column;
    SquadGetNear_INPUT_SquadID     = nSquadID;

    int i = 0;

    SQL_GO(SquadGetNear);
    while(SQL_SUCCEEDED(SQL_GETROW(SquadGetNear)))
    {
        {
            BEGIN_PFM_CREATE(*pthis, pfmInfo, S, SQUAD_INFO)
                    FM_VAR_PARM(SquadGetNear_Name, CB_ZTS)
            END_PFM_CREATE
            pfmInfo->bSearchResult= false;
            pfmInfo->nSquadID =     SquadGetNear_SquadID;
            pfmInfo->nRanking =     SquadGetNear_Ranking + i++;
            pfmInfo->column =       (char)column;
            pfmInfo->nScore =       SquadGetNear_Score;
            pfmInfo->cWins =        SquadGetNear_Wins;
            pfmInfo->cLosses =      SquadGetNear_Losses;
//            pfmInfo->fHours =       0.0f; // TODO
        }
    }
    pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
}





//////////////////////////////////////////////////////////////////////////////////////////
//
// SendSquadPageDudeX()
//
//
//////////////////////////////////////////////////////////////////////////////////////////
void SendSquadPageDudeX(FedMessaging * pthis, CFMConnection & cnxnFrom, int nCharacterID, bool bMyTab)
{
    // setup query
    SquadGetNearDudeX_INPUT_CharacterID = nCharacterID;

    SQL_GO(SquadGetNearDudeX);
    while(SQL_SUCCEEDED(SQL_GETROW(SquadGetNearDudeX)))
    {
        {
            BEGIN_PFM_CREATE(*pthis, pfmInfo, S, SQUAD_INFO_DUDEX)
                    FM_VAR_PARM(SquadGetNearDudeX_Name, CB_ZTS)
            END_PFM_CREATE
            pfmInfo->nSquadID =     SquadGetNearDudeX_SquadID;
            pfmInfo->nScore =       SquadGetNearDudeX_Score;
            pfmInfo->cWins =        SquadGetNearDudeX_Wins;
            pfmInfo->cLosses =      SquadGetNearDudeX_Losses;
            pfmInfo->bMyTab =       bMyTab;
//            pfmInfo->fHours =       0.0f; // TODO
        }
    }
    pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// SendSquadPage_FromFind():    
//
//    szName: partial name to search for
//
//////////////////////////////////////////////////////////////////////////////////////////
void SendSquadPage_FromFind(FedMessaging * pthis, CFMConnection & cnxnFrom, char * szName)
{
    if (!szName)
      return;

    if (strlen(szName) > c_cbNameDB) // keep hackers from crashing server
      return;

    // setup query
    lstrcpy((char*)SquadGetNearFromFind_INPUT_Name, szName);

    int i = 0;

    SQL_GO(SquadGetNearFromFind);
    while(SQL_SUCCEEDED(SQL_GETROW(SquadGetNearFromFind)))
    {
        {
            RemoveTrailingSpaces((char*)SquadGetNearFromFind_Name);

            BEGIN_PFM_CREATE(*pthis, pfmInfo, S, SQUAD_INFO)
                    FM_VAR_PARM(SquadGetNearFromFind_Name, CB_ZTS)
            END_PFM_CREATE
            pfmInfo->bSearchResult= true;
            pfmInfo->nSquadID =     SquadGetNearFromFind_SquadID;
            pfmInfo->nRanking =     SquadGetNearFromFind_Ranking + i++;
            pfmInfo->column =       1;
            pfmInfo->nScore =       SquadGetNearFromFind_Score;
            pfmInfo->cWins =        SquadGetNearFromFind_Wins;
            pfmInfo->cLosses =      SquadGetNearFromFind_Losses;
//            pfmInfo->fHours =       0.0f; // TODO
        }
    }
    pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// SendSquadDetails(): sends most the details about a squad to the client
//
//////////////////////////////////////////////////////////////////////////////////////////
void SendSquadDetails(FedMessaging * pthis, CFMConnection & cnxnFrom, int nSquadID)
{
  char szDate[9];
  SquadDetails_INPUT_SquadID = nSquadID;
  SquadDetailsPlayers_INPUT_SquadID = nSquadID;
  SquadDetails_CivID = -1;

  SQL_GO(SquadDetails);
  if(SQL_SUCCEEDED(SQL_GETROW(SquadDetails)))
  {
    {
        wsprintf(szDate, "%2d/%2d/%02d", SquadDetails_InceptionDate.month, 
                                         SquadDetails_InceptionDate.day, 
                                         SquadDetails_InceptionDate.year%100);
        //int SquadDetails_Rank = 0; // temp

        BEGIN_PFM_CREATE(*pthis, pfmInfo, S, SQUAD_DETAILS)
                FM_VAR_PARM(SquadDetails_Description, CB_ZTS)
                FM_VAR_PARM(SquadDetails_URL, CB_ZTS)
                FM_VAR_PARM(szDate, CB_ZTS)
        END_PFM_CREATE
//        pfmInfo->sqRank = (ISquadBase::Rank) SquadDetails_Rank;
        pfmInfo->nSquadID = SquadDetails_INPUT_SquadID;
        pfmInfo->civid = SquadDetails_CivID;
    }

    SquadDetailsPlayers_Status2 = 0; // set to zero incase result of null
    SquadDetailsPlayers_Status = 1;

    SQL_GO(SquadDetailsPlayers);
    while(SQL_SUCCEEDED(SQL_GETROW(SquadDetailsPlayers)))
    {
        {
            wsprintf(szDate, "%2d/%2d/%02d", SquadDetailsPlayers_Granted.month, 
                                            SquadDetailsPlayers_Granted.day, 
                                            SquadDetailsPlayers_Granted.year%100);

            BEGIN_PFM_CREATE(*pthis, pfmInfo, S, SQUAD_DETAILS_PLAYER)
                    FM_VAR_PARM(SquadDetailsPlayers_MemberName, CB_ZTS)
                    FM_VAR_PARM(szDate, CB_ZTS)
            END_PFM_CREATE
            pfmInfo->nMemberID      =                                SquadDetailsPlayers_MemberID;
            pfmInfo->sqRank         = (IMemberBase::Rank)            SquadDetailsPlayers_Rank;

            if (SquadDetailsPlayers_Status == 0) // 0 == leader
            {
                pfmInfo->detailedstatus = IMemberBase::DSTAT_LEADER;
            }
            else
            if (SquadDetailsPlayers_Status == 2) // 2 == pending
            {
                pfmInfo->detailedstatus = IMemberBase::DSTAT_PENDING;
            }
            else // (SquadDetailsPlayers_Status == 1) 
            {
                if (SquadDetailsPlayers_Status2 == 1)
                    pfmInfo->detailedstatus = IMemberBase::DSTAT_ASL;
                else
                    pfmInfo->detailedstatus = IMemberBase::DSTAT_MEMBER;
            }

            SquadDetailsPlayers_Status2 = 0; // set to zero incase result of null
            SquadDetailsPlayers_Status = 1;
        }
    }

    pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
  }
  else
  {
     // let client know that the squad has ve 86-ed; this sometimes happens
     // if our database is not in sync with theirs.  The SquadDetails should
     // have updated our database (that is deleted this squad from it).
     BEGIN_PFM_CREATE(*pthis, pfmDeleted, S, SQUAD_DELETED)
     END_PFM_CREATE
     pfmDeleted->nSquadID = nSquadID;
     pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
  }
}



//////////////////////////////////////////////////////////////////////////////////////////
//
// SendSquadTextMessage(): sends a text message to the user at the squads screen.
//                         this msg, which is usually an error, is displayed on the user's
//                         end as a dialog text box.  
//
//////////////////////////////////////////////////////////////////////////////////////////
void SendSquadTextMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, const char* format, ...)
{
    const size_t size = 256;
    char         bfr[size];

    va_list vl;
    va_start(vl, format);
    _vsnprintf(bfr, size, format, vl);
    va_end(vl);

    BEGIN_PFM_CREATE(*pthis, pfmSquadTextMessage, S, SQUAD_TEXT_MESSAGE)
      FM_VAR_PARM(bfr, CB_ZTS)
    END_PFM_CREATE
    pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
}


HRESULT IClubClientSite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  CFLClient * pClient = CFLClient::FromConnection(cnxnFrom);
  assert(pClient);
  char szBuffer[128];

  debugf("Client: %s from <%s> at time %u\n", g_rgszMsgNames[pfm->fmid], cnxnFrom.GetName(), Time::Now());
  
  switch (pfm->fmid)
  {

    case FM_C_LOGON_CLUB: 
    {
      CASTPFM(pfmLogon, C, LOGON_CLUB, pfm);
      bool fValid = false; // whether we have a valid logon
      bool fRetry = false; // whether or not the client should retry the login
      char * szReason = NULL; // when fValid==false

      //
      // TODO: If a new person logs on and they are not in the database, we need to add a record for them (or maybe report error).
      //

      // verify messaging version
      if(pfmLogon->verClub != ALLCLUBVER)
      {
          // tell client that his version is wrong
          sprintf(szBuffer, "AllClub Server's 'ClubMessaging' version (%i) is different than Client's version (%i)", ALLCLUBVER, pfmLogon->verClub);
          szReason = szBuffer;
      }
      else
      {
          LPBYTE pZoneTicket = (LPBYTE) FM_VAR_REF(pfmLogon, ZoneTicket);
          if (pZoneTicket) // it's all in the Zone Ticket
          {
              TRef<IZoneAuthServer> pzas = g_pClubApp->GetZoneAuthServer();

              HRESULT hr = pzas->DecryptTicket(pZoneTicket, pfmLogon->cbZoneTicket);
              switch (hr)
              {
                  case ZT_NO_ERROR:
                  {
                    if (lstrcmp(g_pClubApp->GetAuthServer(), pzas->GetAuthServer())) // you MUST use the auth server that we expect
                    {
                        szReason = "Your account authentication did not go through the expected server.";
                    }
                    else
                    {
                        bool fValidNow = false;
                        pClient->SetClubMember(pzas->HasToken(g_pClubApp->GetToken(), &fValidNow) && fValidNow); // Note: relies on left to right eval
                        // Bell & whistle: Give them a message if their subscription has expired, that they won't be able to change anything
                        fValid = true;
                        pClient->SetZoneID(pzas->GetAccountID());
                    }
                  }
                  break;

                  case ZT_E_BUFFER_TOO_SMALL:
                      g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "Club: Need to increase size of token buffer.");
                      szReason = "Internal error.";
                      break;

                  case ZT_E_AUTH_INVALID_TICKET:
                      g_pClubApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, "Club: Invalid ticket received.");
                      szReason = "Could not validate Zone ID.";
                      break;

                  default:
                      g_pClubApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, "Unexpected error from zoneauth Decrypt.");
                      szReason = "Internal error.";
              }
          }
          else
          {
#ifdef MARKCU
              // let mark run his little squads app on his machine
              strncpy((char*)CharID_CharacterName, FM_VAR_REF(pfmLogon, CharacterName), sizeof(CharID_CharacterName));
              SQL_GO(CharID);
              if(!SQL_SUCCEEDED(SQL_GETROW(CharID)))
              {
                  assert(0);
              }
              pClient->SetZoneID(CharID_CharacterId);
              pClient->SetClubMember(true);
              fValid = true;
#else
              g_pClubApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, "Club: No ticket on logon.");
              szReason = "No login credentials found.";
#endif
          }
      }

      if (fValid)
      {
          // Allow client to logon
          pthis->SetDefaultRecipient(&cnxnFrom, FM_GUARANTEED);
          BEGIN_PFM_CREATE(*pthis, pfmLogonAck, S, LOGON_CLUB_ACK)
          END_PFM_CREATE

          pfmLogonAck->nMemberID = pClient->GetZoneID();

          debugf("Zone ID: %d\n", pfmLogonAck->nMemberID);

          // Send them the ranking info
          CheckRankInfo();
          BEGIN_PFM_CREATE(*pthis, pfmRankInfo, S, CLUB_RANK_INFO)
            FM_VAR_PARM(g_pClubApp->GetRankInfo(), sizeof(RankInfo) * g_pClubApp->GetRankInfoCount())
          END_PFM_CREATE
          pfmRankInfo->cRanks = g_pClubApp->GetRankInfoCount();
      }
      else
      {
          BEGIN_PFM_CREATE(*pthis, pfmLogonNack, S, LOGON_CLUB_NACK)
            FM_VAR_PARM(szReason, CB_ZTS)
          END_PFM_CREATE
          pfmLogonNack->fRetry = fRetry;
      }
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

    case FM_C_LOGOFF_CLUB:
    {
      // TODO: mark 'em so we know they weren't dropped
    }
    break;

    //  This message is sent when a zone character requests info about himself.
    //  Server responds with CHARACTER_INFO_GENERAL
    //  along with CHARACTER_INFO_BY_CIV messages.
    //
    case FM_C_REQ_CHARACTER_INFO:
    {
        CASTPFM(pfmReq, C, REQ_CHARACTER_INFO, pfm);

        if (pfmReq->nMemberID == -1)
            CharInfoGeneral_CharacterID = pClient->GetZoneID();
        else
            CharInfoGeneral_CharacterID = pfmReq->nMemberID;

        //
        // First, we send civ-independent info
        //
        debugf("Sending Character Info for %d.\n", CharInfoGeneral_CharacterID);

        // do query
        SQL_GO(CharInfoGeneral);
        if(SQL_SUCCEEDED(SQL_GETROW(CharInfoGeneral)))
        {
            pthis->SetDefaultRecipient(&cnxnFrom, FM_GUARANTEED);

            RemoveTrailingSpaces((char*) CharInfoGeneral_Name);
            RemoveTrailingSpaces((char*) CharInfoGeneral_Description);
    
            BEGIN_PFM_CREATE(*pthis, pfmCharInfoGeneral, S, CHARACTER_INFO_GENERAL)
                FM_VAR_PARM(CharInfoGeneral_Name, CB_ZTS)
                FM_VAR_PARM(CharInfoGeneral_Description, CB_ZTS)
            END_PFM_CREATE
        }
        else
        {
            // review: consider uncommenting this:
//            g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "Database is messsed up; failed to get info for Player with ID=%d ", CharInfoGeneral_CharacterID);
        }

        //
        // Next, send civ specific info
        //

        //  This message is sent when a zone character requests info about himself 
        //  specific to a CIV.

        //int cCivs = g_pStaticData->GetCivCount(); KGJV - obsolete
        CharInfoByCiv_CharacterID = CharInfoGeneral_CharacterID;  // setup input for CharInfoByCiv queries

        SQL_GO(CharInfoByCiv);
        while(SQL_SUCCEEDED(SQL_GETROW(CharInfoByCiv)))
        {
            // utilize query output in the response fedmsg
            {
                char szDate[11];
                wsprintf(szDate, "%2d/%2d/%4d", CharInfoByCiv_LastPlayed.month, 
                                                CharInfoByCiv_LastPlayed.day, 
                                                CharInfoByCiv_LastPlayed.year);
                BEGIN_PFM_CREATE(*pthis, pfmCharInfoByCiv, S, CHARACTER_INFO_BY_CIV)
                    FM_VAR_PARM(szDate, CB_ZTS)
                END_PFM_CREATE

                pfmCharInfoByCiv->civid = CharInfoByCiv_Civid;
                pfmCharInfoByCiv->nRank = CharInfoByCiv_Rank;
                pfmCharInfoByCiv->fScore = CharInfoByCiv_Score;
                pfmCharInfoByCiv->cMinutesPlayed = MaxMinutes(CharInfoByCiv_MinutesPlayed);
                pfmCharInfoByCiv->cBaseKills = CharInfoByCiv_BaseKills;
                pfmCharInfoByCiv->cBaseCaptures = CharInfoByCiv_BaseCaptures;
                pfmCharInfoByCiv->nRating = RATING_EXT2DISP(CharInfoByCiv_Rating);
                pfmCharInfoByCiv->cKills = CharInfoByCiv_Kills;
                pfmCharInfoByCiv->cDeaths = CharInfoByCiv_Deaths;
                pfmCharInfoByCiv->cWins = CharInfoByCiv_Wins;
                pfmCharInfoByCiv->cLosses = CharInfoByCiv_Losses;
                pfmCharInfoByCiv->nPromotionAt = GetNextPromotion(
                  pfmCharInfoByCiv->civid, 
                  pfmCharInfoByCiv->fScore
                  ); 
            }
        }

        //
        // Send medal info for this character
        //
        MedalInfo_CharacterID = CharInfoGeneral_CharacterID;

        SQL_GO(CharMedal);
        while (SQL_SUCCEEDED(SQL_GETROW(CharMedal)))
        {
            RemoveTrailingSpaces((char*)MedalInfo_Name);
            RemoveTrailingSpaces((char*)MedalInfo_Description);
            RemoveTrailingSpaces((char*)MedalInfo_SpecificInfo);
            RemoveTrailingSpaces((char*)MedalInfo_Bitmap);

            {
            BEGIN_PFM_CREATE(*pthis, pfmMedal, S, CHARACTER_INFO_MEDAL)
                FM_VAR_PARM(MedalInfo_Name, CB_ZTS)
                FM_VAR_PARM(MedalInfo_Description, CB_ZTS)
                FM_VAR_PARM(MedalInfo_SpecificInfo, CB_ZTS)
                FM_VAR_PARM(MedalInfo_Bitmap, CB_ZTS)
            END_PFM_CREATE
            pfmMedal->civid = MedalInfo_Civid;
            }
        }

        //
        // Send out queued messages to character
        //
        pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;
    
    case FM_C_CHARACTER_INFO_EDIT_DESCRIPTION:
    {
      CASTPFM(pfmEdit, C, CHARACTER_INFO_EDIT_DESCRIPTION, pfm);
      char * szDesc = FM_VAR_REF(pfmEdit, szDescription);
      if (szDesc)
        lstrcpy((char *) UpdateDescription_Description, szDesc);
      else
        lstrcpy((char *) UpdateDescription_Description, "");

      UpdateDescription_CharacterId = pClient->GetZoneID();
      SQL_GO(UpdateDescription);
    }
    break;

    case FM_C_SQUAD_CREATE:
    {
      CASTPFM(pfmCreate, C, SQUAD_CREATE, pfm);

      char * szName  = FM_VAR_REF(pfmCreate, szName); 
      char * szDesc  = FM_VAR_REF(pfmCreate, szDescription);
      char * szURL   = FM_VAR_REF(pfmCreate, szURL);

      //
      // setup input for query
      //

      if(szName)
        strncpy((char *) CreateSquad_Name, szName, min(sizeof(CreateSquad_Name)-1, c_cbNameDB));
      else
      *((char *)CreateSquad_Name) = 0;

      if (szDesc)
        strncpy((char *) CreateSquad_Description, szDesc, sizeof(CreateSquad_Description)-1);
      else
      *((char *)CreateSquad_Description) = 0;

      if(szURL)
        strncpy((char *) CreateSquad_URL, szURL, sizeof(CreateSquad_URL)-1);
      else
      *((char *)CreateSquad_URL) = 0;

      CreateSquad_CivID = pfmCreate->civid;
      CreateSquad_Owner = pClient->GetZoneID();
      CreateSquad_ErrorCode = -1;
                                                
      if (pClient->GetClubMember())
      {
        //
        // do query
        //
        SQL_GO(CreateSquad);
        if (!SQL_SUCCEEDED(SQL_GETROW(CreateSquad)))
        {
          lstrcpy((char *) CreateSquad_ErrorMsg, "Server Error; try again later.");
          g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "Error while trying to create squad.");
        }

        if (CreateSquad_ErrorCode != 0)
        {
            //
            // Scan the CreateSquad_ErrorMsg string, replace the word "Team" with "Squad";
            // then send the message to the user
            //
            char szErrorMsg[c_cbErrorMsg + 20];

            UTL::SearchAndReplace(szErrorMsg, (char*)CreateSquad_ErrorMsg, "Squad", "Team");

            SendSquadTextMessage(pthis, cnxnFrom, "Cannot create %s Squad.\n%s", szName, szErrorMsg);
        }
      }
      else
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
      }

      BEGIN_PFM_CREATE(*pthis, pfmAck, S, SQUAD_CREATE_ACK)
      END_PFM_CREATE
      pfmAck->nClientSquadID = pfmCreate->nClientSquadID;
      pfmAck->nSquadID = CreateSquad_SquadID;
      pfmAck->bSuccessful = (CreateSquad_ErrorCode == 0);

      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

    case FM_C_SQUAD_EDIT:
    {
      CASTPFM(pfmEdit, C, SQUAD_EDIT, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmEdit->nSquadID);  // update client with the truth
         break;
      }

      char * szDesc  = FM_VAR_REF(pfmEdit, szDescription);
      char * szURL   = FM_VAR_REF(pfmEdit, szURL);

      if(szDesc)
        strncpy((char *) EditSquad_Description, szDesc, sizeof(EditSquad_Description)-1);
      else
          *((char *)EditSquad_Description) = 0;

      if(szURL)
        strncpy((char *) EditSquad_URL, szURL, sizeof(EditSquad_URL)-1);
      else
          *((char *)EditSquad_URL) = 0;

      EditSquad_SquadID = pfmEdit->nSquadID;
      EditSquad_CivID = pfmEdit->civid;
      EditSquad_ErrorCode = 0;

      //
      // do query
      //
      SQL_GO(EditSquad);
      if (!SQL_SUCCEEDED(SQL_GETROW(EditSquad)))
      {
        EditSquad_ErrorCode = -1;
        lstrcpy((char *) EditSquad_ErrorMsg, "Server Error; try again later.");
        g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "Error while trying to edit squad: id = %d", EditSquad_SquadID);
      }

      if (EditSquad_ErrorCode != 0)
      {
        SendSquadTextMessage(pthis, cnxnFrom, "Server cannot edit squad (%d);  Reason (%d) %s ", EditSquad_SquadID, EditSquad_ErrorCode, EditSquad_ErrorMsg);
      }

      SendSquadDetails(pthis, cnxnFrom, pfmEdit->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_NEXT_PAGE_ALL:
    {
      CASTPFM(pfmPage, C, SQUAD_NEXT_PAGE_ALL, pfm);

      SendSquadPage(pthis, cnxnFrom, pfmPage->column, pfmPage->nSquadID);
    }
    break;

    case FM_C_SQUAD_NEXT_PAGE_DUDEX:
    {
      CASTPFM(pfmPage, C, SQUAD_NEXT_PAGE_DUDEX, pfm);

      SendSquadPageDudeX(pthis, cnxnFrom, pfmPage->nMemberID, false);
    }
    break;

    case FM_C_SQUAD_NEXT_PAGE_MY:
    {
      SendSquadPageDudeX(pthis, cnxnFrom, pClient->GetZoneID(), true);
    }
    break;

    case FM_C_SQUAD_DETAILS_REQ:
    {
      CASTPFM(pfmReq, C, SQUAD_DETAILS_REQ, pfm);

      SendSquadDetails(pthis, cnxnFrom, pfmReq->nSquadID);
    }
    break;

    case FM_C_SQUAD_PAGE_FIND:
    {
      CASTPFM(pfmPage, C, SQUAD_PAGE_FIND, pfm);

      SendSquadPage_FromFind(pthis, cnxnFrom, FM_VAR_REF(pfmPage, szPartialName));
    }
    break;

    case FM_C_SQUAD_LOG_REQ:
    {
      CASTPFM(pfmLog, C, SQUAD_LOG_REQ, pfm);
      GetOwnershipLog_SquadID = pfmLog->nSquadID;

      // do query
      SQL_GO(GetOwnershipLog);
      if (SQL_SUCCEEDED(SQL_GETROW(GetOwnershipLog)))
      {
          // Send SQL response
          BEGIN_PFM_CREATE(*pthis, pfmAck, S, SQUAD_LOG_INFO)
                FM_VAR_PARM(GetOwnershipLog_Output, CB_ZTS)
          END_PFM_CREATE
          pfmAck->nSquadID = GetOwnershipLog_SquadID;

          pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      }
    }
    break;

    case FM_C_SQUAD_MAKE_JOIN_REQUEST:
    {
      CASTPFM(pfmSquad, C, SQUAD_MAKE_JOIN_REQUEST, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      MakeJoinRequest_MemberID = pClient->GetZoneID();
      MakeJoinRequest_SquadID  = pfmSquad->nSquadID;
      MakeJoinRequest_ErrorCode = 0;

      // do query
      SQL_GO(MakeJoinRequest);
      if (SQL_SUCCEEDED(SQL_GETROW(MakeJoinRequest)))
      {
          if (MakeJoinRequest_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_CANCEL_JOIN_REQUEST:
    {
      CASTPFM(pfmSquad, C, SQUAD_CANCEL_JOIN_REQUEST, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      CancelJoinRequest_MemberID = pClient->GetZoneID();
      CancelJoinRequest_SquadID  = pfmSquad->nSquadID;
      CancelJoinRequest_ErrorCode = 0;

      // do query
      SQL_GO(CancelJoinRequest);
      if (SQL_SUCCEEDED(SQL_GETROW(CancelJoinRequest)))
      {
          if (CancelJoinRequest_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_REJECT_JOIN_REQUEST:
    {
      CASTPFM(pfmSquad, C, SQUAD_REJECT_JOIN_REQUEST, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      RejectJoinRequest_MemberID = pfmSquad->nMemberID;
      RejectJoinRequest_SquadID  = pfmSquad->nSquadID;
      RejectJoinRequest_ErrorCode = 0;

      // do query
      SQL_GO(RejectJoinRequest);
      if (SQL_SUCCEEDED(SQL_GETROW(RejectJoinRequest)))
      {
          if (RejectJoinRequest_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_ACCEPT_JOIN_REQUEST:
    {
      CASTPFM(pfmSquad, C, SQUAD_ACCEPT_JOIN_REQUEST, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      AcceptJoinRequest_MemberID = pfmSquad->nMemberID;
      AcceptJoinRequest_SquadID  = pfmSquad->nSquadID;
      AcceptJoinRequest_ErrorCode = 0;

      // do query
      SQL_GO(AcceptJoinRequest);
      if (SQL_SUCCEEDED(SQL_GETROW(AcceptJoinRequest)))
      {
          if (AcceptJoinRequest_ErrorCode != 0)
          {
              SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
    }
    break;

    case FM_C_SQUAD_BOOT_MEMBER:
    {
      CASTPFM(pfmSquad, C, SQUAD_BOOT_MEMBER, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      BootMember_MemberID = pfmSquad->nMemberID;
      BootMember_SquadID  = pfmSquad->nSquadID;
      BootMember_ErrorCode = 0;

      // do query
      SQL_GO(BootMember);
      if (SQL_SUCCEEDED(SQL_GETROW(BootMember)))
      {
          if (BootMember_ErrorCode != 0)
          {
              SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
          else
          {
              if (BootMember_Status == -1)
              {
                 BEGIN_PFM_CREATE(*pthis, pfmDeleted, S, SQUAD_DELETED)
                 END_PFM_CREATE
                 pfmDeleted->nSquadID = BootMember_SquadID;
                 pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
              }
              else SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update incase of new owner
          }
      }
      else SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_QUIT:
    {
      CASTPFM(pfmSquad, C, SQUAD_QUIT, pfm);

      /*
      Let's let people at least be able to quit; so if a leader forgot to pay his dues, the
      squad can continue without him.

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }
      */

      QuitSquad_MemberID = pClient->GetZoneID();
      QuitSquad_SquadID  = pfmSquad->nSquadID;
      QuitSquad_ErrorCode = 0;

      // do query
      SQL_GO(QuitSquad);
      if (SQL_SUCCEEDED(SQL_GETROW(QuitSquad)))
      {
          if (QuitSquad_ErrorCode != 0)
          {
              SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
          else
          {
              if (QuitSquad_Status == -1)
              {
                 BEGIN_PFM_CREATE(*pthis, pfmDeleted, S, SQUAD_DELETED)
                 END_PFM_CREATE
                 pfmDeleted->nSquadID = QuitSquad_SquadID;
                 pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
              }
              else SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update incase of new owner

          }
      }
      else SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;
    
    case FM_C_SQUAD_PROMOTE_TO_ASL:
    {
      CASTPFM(pfmSquad, C, SQUAD_PROMOTE_TO_ASL, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      PromoteToASL_MemberID = pfmSquad->nMemberID;
      PromoteToASL_SquadID  = pfmSquad->nSquadID;
      PromoteToASL_ErrorCode = 0;

      // do query
      SQL_GO(PromoteToASL);
      if (SQL_SUCCEEDED(SQL_GETROW(PromoteToASL)))
      {
          if (PromoteToASL_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_DEMOTE_TO_MEMBER:
    {
      CASTPFM(pfmSquad, C, SQUAD_DEMOTE_TO_MEMBER, pfm);

      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }

      DemoteToMember_MemberID = pfmSquad->nMemberID;
      DemoteToMember_SquadID  = pfmSquad->nSquadID;
      DemoteToMember_ErrorCode = 0;

      // do query
      SQL_GO(DemoteToMember);
      if (SQL_SUCCEEDED(SQL_GETROW(DemoteToMember)))
      {
          if (DemoteToMember_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
          else
          pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_SQUAD_TRANSFER_CONTROL:
    {
      CASTPFM(pfmSquad, C, SQUAD_TRANSFER_CONTROL, pfm);

      /*
      At least allow him to transfer control, so that if a leader doesn't pay, he can at least step down as leader.
      if (!pClient->GetClubMember())
      {
         SendSquadTextMessage(pthis, cnxnFrom, szNotMemberMessage);
         SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
         break;
      }
      */

      TransferControl_MemberID = pfmSquad->nMemberID;
      TransferControl_SquadID  = pfmSquad->nSquadID;
      TransferControl_ErrorCode = 0;

      // do query
      SQL_GO(TransferControl);
      if (SQL_SUCCEEDED(SQL_GETROW(TransferControl)))
      {
          if (TransferControl_ErrorCode != 0)
          {
              SendSquadTextMessage(pthis, cnxnFrom, "Your request could not be completed; try again later.");
          }
      }
      SendSquadDetails(pthis, cnxnFrom, pfmSquad->nSquadID);  // update client with the truth
    }
    break;

    case FM_C_LEADER_BOARD_QUERY:
    {
      CASTPFM(pfmLeaderBoardQuery, C, LEADER_BOARD_QUERY, pfm);
      const int nMaxResultSize = 84 + 1;
      LeaderBoardEntry vResults[nMaxResultSize];
      LeaderBoardEntry topPlayer;
      int nResultCount = 0;

      // force the name to be at least somewhat valid
      char szBasis[c_cbName];
      strncpy(szBasis, pfmLeaderBoardQuery->szBasis, c_cbName);
      szBasis[c_cbName - 1] = '\0';
      RemoveTrailingSpaces(szBasis);

      // do the query
      int characterId = (pfmLeaderBoardQuery->idBasis == -2) ? -1 : pfmLeaderBoardQuery->idBasis;
      nResultCount = GetResults(pfmLeaderBoardQuery->sort,
        szBasis, characterId, pfmLeaderBoardQuery->civid, 
        topPlayer, vResults, nMaxResultSize - 1);

      // if this was a client trying to pull up the initial info on the leader 
      // board but we did not find any info, just pull up the top players.
      // REVIEW: it's inefficient (2 queries), but saves having to add this
      // special case in 12 places.
      if (nResultCount == 0 && pfmLeaderBoardQuery->idBasis == -2)
      {
        szBasis[0] = '\0';
        nResultCount = GetResults(pfmLeaderBoardQuery->sort, "", -1, 
            pfmLeaderBoardQuery->civid, topPlayer, vResults, nMaxResultSize - 1);
      }

      // compose a response
      if (0 == nResultCount)
      {
        BEGIN_PFM_CREATE(*pthis, pfmLeaderBoardQueryFail, S, LEADER_BOARD_QUERY_FAIL)
        END_PFM_CREATE
        strncpy(pfmLeaderBoardQueryFail->szBasis, szBasis, c_cbName);
      }
      else
      {
        assert(nResultCount < nMaxResultSize);

        // if they were seaching by name while sorted by name, they have at 
        // least a partial match
        if ((szBasis[0] != '\0') 
            && (pfmLeaderBoardQuery->sort == lbSortName))
        {
        }
        // if they were searching and the player name was not in the results, forge one
        // on the end of the list.
        else if ((szBasis[0] != '\0')
            ? !HasPlayerNamed(szBasis, vResults, nResultCount)
            : !HasCharacterID(characterId, vResults, nResultCount))
        {
            ForgeResultEntry(vResults[nResultCount], szBasis, 
                characterId, vResults[nResultCount-1].nPosition + 1);
            nResultCount++;
        }

        // send the result list
        BEGIN_PFM_CREATE(*pthis, pfmLeaderBoardList, S, LEADER_BOARD_LIST)
          FM_VAR_PARM(vResults, nResultCount * sizeof(LeaderBoardEntry))
        END_PFM_CREATE
        pfmLeaderBoardList->cEntries = nResultCount;
        pfmLeaderBoardList->entryNumber1 = topPlayer;
      }

      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

//    default:
        // TODO: perhaps we should disconnect this client.
        //g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "Client sent unknown message type.  Type = %d  ", pfm->fmid);
  }

  return S_OK;
}


HRESULT IClubClientSite::OnSysMessage(FedMessaging * pthis) 
{
  return S_OK;
}


void IClubClientSite::OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) 
{
  debugf("ACK!! A guaranteed message didn't make it through to recipient %s.\n", prcp->GetName());
}


HRESULT IClubClientSite::OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  CFLClient * pClient = new CFLClient(&cnxn);
  debugf("Player %s has connected.\n", cnxn.GetName());
  g_pClubApp->GetCounters()->cLogins++;
  return S_OK;
}


HRESULT IClubClientSite::OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  debugf("Player %s has left.\n", cnxn.GetName());
  g_pClubApp->GetCounters()->cLogoffs++;
  delete CFLClient::FromConnection(cnxn);
  return S_OK;
}


HRESULT IClubClientSite::OnSessionLost(FedMessaging * pthis) 
{
  g_pClubApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, "ACK!! Clients session was lost!");
  return S_OK;
}


int IClubClientSite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  debugf("IClubClientSite::OnMessageBox: "); 
  return g_pClubApp->OnMessageBox(strText, strCaption, nType);
}


