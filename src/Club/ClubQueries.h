#ifndef _CLUBQUERIES_
#define _CLUBQUERIES_

#define HKLM_FedSrv		"SYSTEM\\CurrentControlSet\\Services\\AllClub"

const int c_cbErrorMsgDB        = 128;
const int c_cbErrorMsg          = c_cbErrorMsgDB + 1;

const int c_cbOwnershipLogDB    = 500;
const int c_cbOwnershipLog      = c_cbOwnershipLogDB + 1;


// ******************************************************************
// Old-style synchronous queries
// ******************************************************************

BEGIN_SQL_DEF(&g_SQLSite)

  //
  // Input:   Character Name
  //
  // Output:  Zone Club ID
  //
  DEF_STMT(CharID, 
           "SELECT CharacterID "
           "FROM CharacterInfo "
           "WHERE CharacterName=?")
    SQL_INT4_PARM(CharID_CharacterId,                   SQL_OUT_PARM)
    SQL_STR_PARM(CharID_CharacterName, c_cbNameDB,      SQL_IN_PARM)

  //
  // Input:   Zone Club ID
  //
  // Output:  Character's Name and Description
  //
  DEF_STMT(CharInfoGeneral, 
        "SELECT " 
        FEDSTR(CharacterName) ", "
        FEDSTR(Description) 
        " FROM CharacterInfo "
        "WHERE CharacterID = ?"
        )
  SQL_STR_PARM(CharInfoGeneral_Name,        c_cbNameDB,        SQL_OUT_PARM)
  SQL_STR_PARM(CharInfoGeneral_Description, c_cbDescriptionDB, SQL_OUT_PARM)
  SQL_INT4_PARM(CharInfoGeneral_CharacterID,                   SQL_IN_PARM)

  //
  // Input:   Zone Club ID, CivID 
  //
  // Output:  Character's Stats for Civ
  //
  DEF_STMT(CharInfoByCiv,
        "SELECT " FEDNUM(CivID) ", Rank, Score, MinutesPlayed, BaseKills, "
        "BaseCaptures, Rating, GamesPlayed, Kills, Deaths, Wins, Losses, LastPlayed "
        "FROM CharStats "
        "WHERE CharacterID=?"
        )
  SQL_INT2_PARM(CharInfoByCiv_Civid,               SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Rank,                SQL_OUT_PARM)
  SQL_FLT4_PARM(CharInfoByCiv_Score,               SQL_OUT_PARM)
  SQL_INT4_PARM(CharInfoByCiv_MinutesPlayed,       SQL_OUT_PARM)
  SQL_INT4_PARM(CharInfoByCiv_BaseKills,           SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_BaseCaptures,        SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Rating,              SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_GamesPlayed,         SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Kills,               SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Deaths,              SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Wins,                SQL_OUT_PARM)
  SQL_INT2_PARM(CharInfoByCiv_Losses,              SQL_OUT_PARM)
  SQL_DATE_PARM(CharInfoByCiv_LastPlayed,          SQL_OUT_PARM)
  //SQL_INT4_PARM(CharInfoByCiv_PromotionAt,         SQL_OUT_PARM)
  SQL_INT4_PARM(CharInfoByCiv_CharacterID,         SQL_IN_PARM)

  //
  // Input:   Zone Club ID
  // 
  // Output:  A list of medals that the character has.
  //
  DEF_STMT(CharMedal, "SELECT " FEDNUM(CivID) ", MedalName, MedalDescription, SpecificInfo, MedalBitmap FROM CharMedals C, Medals M WHERE M.MedalID=C.MedalID and CharacterID=? ORDER BY M.MedalSortOrder")
  SQL_INT2_PARM(MedalInfo_Civid,                           SQL_OUT_PARM)
  SQL_STR_PARM (MedalInfo_Name,         48,                SQL_OUT_PARM)
  SQL_STR_PARM (MedalInfo_Description,  256,               SQL_OUT_PARM)
  SQL_STR_PARM (MedalInfo_SpecificInfo, 48,                SQL_OUT_PARM)
  SQL_STR_PARM (MedalInfo_Bitmap,       12,                SQL_OUT_PARM)
  SQL_INT4_PARM(MedalInfo_CharacterID,                     SQL_IN_PARM)

  //
  // Updates character's description
  //
  DEF_STMT(UpdateDescription, "UPDATE CharacterInfo SET Description=? "
                              "WHERE CharacterID=?")
  SQL_STR_PARM(UpdateDescription_Description, c_cbDescriptionDB, SQL_IN_PARM)
  SQL_INT4_PARM(UpdateDescription_CharacterId,                   SQL_IN_PARM)

  //
  // Input:   Character Name
  //
  // Output:  Zone Club ID and the name properly capitalized
  //
  DEF_STMT(CharIDAndCapitalization, 
           "SELECT CharacterID, CharacterName "
           "FROM CharacterInfo "
           "WHERE CharacterName=?")
    SQL_INT4_PARM(CharIDAndCapitalization_CharacterId,                   SQL_OUT_PARM)
    SQL_STR_PARM (CharIDAndCapitalization_ProperName,   c_cbNameDB,      SQL_OUT_PARM)
    SQL_STR_PARM (CharIDAndCapitalization_Name,         c_cbNameDB,      SQL_IN_PARM)


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Name in this civ
  //
  DEF_STMT(GetTopName, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By CharacterName, CharacterID")
  SQL_INT4_PARM(GetTopName_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopName_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopName_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopName_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopName_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopName_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearName, "{ call GetNearNames(?, ?, ?) }")
  SQL_INT4_PARM(GetNearName_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearName_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearName_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearName_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearName_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearName_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearName_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearName_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearName_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Rank in this civ
  //
  DEF_STMT(GetTopRank, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Score DESC, CharacterID")
  SQL_INT4_PARM(GetTopRank_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopRank_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopRank_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopRank_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRank_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopRank_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearRank, "{ call GetNearScore(?, ?, ?) }")
  SQL_INT4_PARM(GetNearRank_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRank_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearRank_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearRank_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRank_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRank_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRank_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearRank_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearRank_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Rating in this civ
  //
  DEF_STMT(GetTopRating, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Rating DESC, CharacterID")
  SQL_INT4_PARM(GetTopRating_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopRating_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopRating_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopRating_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopRating_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopRating_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearRating, "{ call GetNearRating(?, ?, ?) }")
  SQL_INT4_PARM(GetNearRating_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRating_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearRating_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearRating_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRating_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearRating_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearRating_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearRating_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearRating_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Score in this civ
  //
  DEF_STMT(GetTopScore, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Score DESC, CharacterID")
  SQL_INT4_PARM(GetTopScore_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopScore_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopScore_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopScore_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopScore_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopScore_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearScore, "{ call GetNearScore(?, ?, ?) }")
  SQL_INT4_PARM(GetNearScore_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearScore_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearScore_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearScore_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearScore_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearScore_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearScore_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearScore_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearScore_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by TimePlayed in this civ
  //
  DEF_STMT(GetTopTimePlayed, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By MinutesPlayed DESC, CharacterID")
  SQL_INT4_PARM(GetTopTimePlayed_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopTimePlayed_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopTimePlayed_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopTimePlayed_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopTimePlayed_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopTimePlayed_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearTimePlayed, "{ call GetNearMinutesPlayed(?, ?, ?) }")
  SQL_INT4_PARM(GetNearTimePlayed_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearTimePlayed_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearTimePlayed_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearTimePlayed_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearTimePlayed_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearTimePlayed_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearTimePlayed_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearTimePlayed_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearTimePlayed_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by BaseKills in this civ
  //
  DEF_STMT(GetTopBaseKills, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By BaseKills DESC, CharacterID")
  SQL_INT4_PARM(GetTopBaseKills_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopBaseKills_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopBaseKills_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopBaseKills_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseKills_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopBaseKills_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearBaseKills, "{ call GetNearBaseKills(?, ?, ?) }")
  SQL_INT4_PARM(GetNearBaseKills_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseKills_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearBaseKills_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearBaseKills_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseKills_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseKills_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseKills_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearBaseKills_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearBaseKills_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by BaseCaptures in this civ
  //
  DEF_STMT(GetTopBaseCaptures, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By BaseCaptures DESC, CharacterID")
  SQL_INT4_PARM(GetTopBaseCaptures_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopBaseCaptures_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopBaseCaptures_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopBaseCaptures_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopBaseCaptures_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopBaseCaptures_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearBaseCaptures, "{ call GetNearBaseCaptures(?, ?, ?) }")
  SQL_INT4_PARM(GetNearBaseCaptures_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseCaptures_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearBaseCaptures_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearBaseCaptures_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseCaptures_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearBaseCaptures_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearBaseCaptures_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearBaseCaptures_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearBaseCaptures_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Kills in this civ
  //
  DEF_STMT(GetTopKills, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Kills DESC, CharacterID")
  SQL_INT4_PARM(GetTopKills_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopKills_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopKills_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopKills_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopKills_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopKills_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearKills, "{ call GetNearKills(?, ?, ?) }")
  SQL_INT4_PARM(GetNearKills_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearKills_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearKills_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearKills_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearKills_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearKills_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearKills_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearKills_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearKills_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by GamesPlayed in this civ
  //
  DEF_STMT(GetTopGamesPlayed, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By GamesPlayed DESC, CharacterID")
  SQL_INT4_PARM(GetTopGamesPlayed_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopGamesPlayed_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopGamesPlayed_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopGamesPlayed_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopGamesPlayed_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopGamesPlayed_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearGamesPlayed, "{ call GetNearGamesPlayed(?, ?, ?) }")
  SQL_INT4_PARM(GetNearGamesPlayed_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearGamesPlayed_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearGamesPlayed_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearGamesPlayed_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearGamesPlayed_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearGamesPlayed_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearGamesPlayed_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearGamesPlayed_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearGamesPlayed_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Deaths in this civ
  //
  DEF_STMT(GetTopDeaths, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Deaths DESC, CharacterID")
  SQL_INT4_PARM(GetTopDeaths_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopDeaths_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopDeaths_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopDeaths_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopDeaths_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopDeaths_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearDeaths, "{ call GetNearDeaths(?, ?, ?) }")
  SQL_INT4_PARM(GetNearDeaths_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearDeaths_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearDeaths_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearDeaths_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearDeaths_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearDeaths_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearDeaths_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearDeaths_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearDeaths_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Wins in this civ
  //
  DEF_STMT(GetTopWins, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Wins DESC, CharacterID")
  SQL_INT4_PARM(GetTopWins_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopWins_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopWins_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopWins_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopWins_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopWins_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearWins, "{ call GetNearWins(?, ?, ?) }")
  SQL_INT4_PARM(GetNearWins_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearWins_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearWins_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearWins_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearWins_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearWins_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearWins_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearWins_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearWins_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Commander Wins in this civ
  //
  DEF_STMT(GetTopCommanderWins, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By WinsCmd DESC, CharacterID")
  SQL_INT4_PARM(GetTopCommanderWins_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopCommanderWins_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopCommanderWins_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopCommanderWins_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopCommanderWins_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopCommanderWins_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearCommanderWins, "{ call GetNearWinsCmd(?, ?, ?) }")
  SQL_INT4_PARM(GetNearCommanderWins_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearCommanderWins_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearCommanderWins_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearCommanderWins_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearCommanderWins_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearCommanderWins_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearCommanderWins_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearCommanderWins_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearCommanderWins_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )


  //
  // Input:     CivID (x2) (-1 for overall)
  //
  // Output:    The top ranked player by Losses in this civ
  //
  DEF_STMT(GetTopLosses, "SELECT TOP 1 * FROM CharStats WHERE (CivID=? or (-1=? and CivID is null)) ORDER By Losses DESC, CharacterID")
  SQL_INT4_PARM(GetTopLosses_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetTopLosses_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetTopLosses_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetTopLosses_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetTopLosses_CivIDIn,                            SQL_IN_PARM )
  SQL_INT2_PARM(GetTopLosses_CivIDIn2,                           SQL_IN_PARM )

  //
  // Input:   character ID (-1 for top), civ id (-1 for overall), and character name (or "" to use the character ID)
  // 
  // Output:  The stats for thsi civ for this character and the characters ranked near him/her
  //
  DEF_STMT(GetNearLosses, "{ call GetNearLosses(?, ?, ?) }")
  SQL_INT4_PARM(GetNearLosses_FirstOrdinal,                       SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearLosses_CharacterID,                        SQL_OUT_PARM)
  SQL_STR_PARM (GetNearLosses_CharacterName,      c_cbNameDB,     SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_CivID,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Rank,                               SQL_OUT_PARM)
  SQL_FLT4_PARM(GetNearLosses_Score,                              SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearLosses_MinutesPlayed,                      SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_BaseKills,                          SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_BaseCaptures,                       SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Kills,                              SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Rating,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_GamesPlayed,                        SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Deaths,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Wins,                               SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_Losses,                             SQL_OUT_PARM)
  SQL_INT2_PARM(GetNearLosses_CommanderWins,                      SQL_OUT_PARM)
  SQL_INT4_PARM(GetNearLosses_CharacterIDIn,                      SQL_IN_PARM )
  SQL_INT2_PARM(GetNearLosses_CivIDIn,                            SQL_IN_PARM )
  SQL_STR_PARM (GetNearLosses_CharacterNameIn,    c_cbNameDB,     SQL_IN_PARM )




  DEF_STMT(GetRankCount, "Select Count(*) From Ranks") 
  SQL_INT2_PARM(GetRankCount_Count,                   SQL_OUT_PARM)  

  DEF_STMT(GetRanks, "SELECT Rank, " FEDNUM(CivID) ", Requirement, Name FROM Ranks ORDER BY CivID, Rank") 
  SQL_INT2_PARM(GetRanks_Rank,                    SQL_OUT_PARM)
  SQL_INT2_PARM(GetRanks_CivID,                   SQL_OUT_PARM)  
  SQL_INT4_PARM(GetRanks_Requirement,             SQL_OUT_PARM)  
  SQL_STR_PARM( GetRanks_Name,        c_cbNameDB, SQL_OUT_PARM)


  //
  // Create's a squad
  //
  // Input:    Squad name, Description, URL, OwnerID, civid
  //
  // Output:   TeamID, Error Code, Err Msg
  //
  DEF_STMT(CreateSquad,
        "{Call SquadCreate(?, ?, ?, ?, ?)}" 
        )
  SQL_INT4_PARM(CreateSquad_SquadID,                             SQL_OUT_PARM)
  SQL_INT4_PARM(CreateSquad_ErrorCode,                           SQL_OUT_PARM)
  SQL_STR_PARM (CreateSquad_ErrorMsg,    c_cbErrorMsgDB,         SQL_OUT_PARM)
  SQL_STR_PARM(CreateSquad_Name,         c_cbNameDB,             SQL_IN_PARM)
  SQL_STR_PARM(CreateSquad_Description,  c_cbSquadDescriptionDB, SQL_IN_PARM)
  SQL_STR_PARM(CreateSquad_URL,          c_cbURLDB,              SQL_IN_PARM)  
  SQL_INT4_PARM(CreateSquad_Owner,                               SQL_IN_PARM)
  SQL_INT2_PARM(CreateSquad_CivID,                               SQL_IN_PARM)


  //
  // Edit a squad
  //
  // Input:    squadID, Description, URL, civid
  //
  // Output:   TeamID, Error Code, Err Msg
  //
  DEF_STMT(EditSquad,
        "{Call SquadEdit(?, ?, ?, ?)}" 
        )
  SQL_INT4_PARM(EditSquad_ErrorCode,                            SQL_OUT_PARM)
  SQL_STR_PARM (EditSquad_ErrorMsg,     c_cbErrorMsgDB,         SQL_OUT_PARM)
  SQL_INT4_PARM(EditSquad_SquadID,                              SQL_IN_PARM)
  SQL_STR_PARM (EditSquad_Description,  c_cbSquadDescriptionDB, SQL_IN_PARM)
  SQL_STR_PARM (EditSquad_URL,          c_cbURLDB,              SQL_IN_PARM)
  SQL_INT2_PARM(EditSquad_CivID,                                SQL_IN_PARM)

  //
  // Get Squad Page
  //
  // Input:    column, SquadID
  //
  // Output:   list of squadID, ranking, civid, wins, losses, score, todo: hours
  //
  DEF_STMT(SquadGetNear,
        "{Call SquadGetNear(?, ?)}" 
        )
  SQL_INT4_PARM(SquadGetNear_Ranking,             SQL_OUT_PARM)
  SQL_INT4_PARM(SquadGetNear_SquadID,             SQL_OUT_PARM)
  SQL_STR_PARM (SquadGetNear_Name,     c_cbNameDB,SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNear_Score,               SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNear_Wins,                SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNear_Losses,              SQL_OUT_PARM) 
  SQL_STR_PARM (SquadGetNear_Log,     c_cbLogDB,  SQL_OUT_PARM)  // TODO: remove
  SQL_INT2_PARM(SquadGetNear_CivID,               SQL_OUT_PARM)  // TODO: remove 
  SQL_INT4_PARM(SquadGetNear_INPUT_Column,        SQL_IN_PARM)
  SQL_INT4_PARM(SquadGetNear_INPUT_SquadID,       SQL_IN_PARM)

  //
  // Get Squad Page DudeX (for DudeX and "My" tabs of squadsscreen)
  //
  // Input:    CharacterID (for DudeX Tab; -1 means any player)
  //
  // Output:   list of squadID, civid, wins, losses, score, todo: hours
  //
  DEF_STMT(SquadGetNearDudeX,
        "{Call SquadGetDudeXSquads(?)}" 
        )
  SQL_INT4_PARM(SquadGetNearDudeX_SquadID,       SQL_OUT_PARM)
  SQL_STR_PARM (SquadGetNearDudeX_Name,     c_cbNameDB,  SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearDudeX_Score,         SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearDudeX_Wins,          SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearDudeX_Losses,        SQL_OUT_PARM) 
  SQL_STR_PARM (SquadGetNearDudeX_Log,     c_cbOwnershipLogDB,  SQL_OUT_PARM)  // TODO: remove
  SQL_INT2_PARM(SquadGetNearDudeX_CivID,         SQL_OUT_PARM)  // TODO: remove 
  SQL_INT4_PARM(SquadGetNearDudeX_INPUT_CharacterID,   SQL_IN_PARM)

  //
  // Get Squad Page From Find
  //
  // Input:    column, SquadID, CharacterID (for DudeX Tab; -1 means any player)
  //
  // Output:   list of squadID, ranking, civid, wins, losses, score, todo: hours
  //
  DEF_STMT(SquadGetNearFromFind,
        "{Call SquadGetNearNames(?, ?)}" 
        )
  SQL_INT4_PARM(SquadGetNearFromFind_Ranking,       SQL_OUT_PARM)
  SQL_INT4_PARM(SquadGetNearFromFind_SquadID,       SQL_OUT_PARM)
  SQL_STR_PARM (SquadGetNearFromFind_Name,     c_cbNameDB,  SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearFromFind_Score,         SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearFromFind_Wins,          SQL_OUT_PARM)
  SQL_INT2_PARM(SquadGetNearFromFind_Losses,        SQL_OUT_PARM) 
  SQL_STR_PARM (SquadGetNearFromFind_Log,     c_cbOwnershipLogDB,  SQL_OUT_PARM)  // review: consider removing for perf
  SQL_INT2_PARM(SquadGetNearFromFind_CivID,         SQL_OUT_PARM) 
  SQL_INT4_PARM(SquadGetNearFromFind_INPUT_Dummy,   SQL_IN_PARM)
  SQL_STR_PARM (SquadGetNearFromFind_INPUT_Name,c_cbNameDB, SQL_IN_PARM)

  /*  SQL_FLT4_PARM(SquadGetNear_Hours,        SQL_OUT_PARM);  TODO:insert */\

  //
  // Get Squad Details 
  //
  // Input:    SquadID
  //
  // Output:   description, URL, inception date, ID, squad rank
  //
  DEF_STMT(SquadDetails,
        "{Call SquadDetails(?)}"      
        )
  SQL_STR_PARM (SquadDetails_Name,          c_cbZoneNameDB,         SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_LeaderName,    c_cbZoneNameDB,         SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_Description,  c_cbSquadDescriptionDB,  SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_FavoriteGame1,  4,                     SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_FavoriteGame2,  4,                     SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_FavoriteGame3,  4,                     SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_FavoriteGame4,  4,                     SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_FavoriteGame5,  4,                     SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_URL,          c_cbURLDB,               SQL_OUT_PARM)
  SQL_INT1_PARM(SquadDetails_Closed,                                SQL_OUT_PARM)
  SQL_INT1_PARM(SquadDetails_Award,                                 SQL_OUT_PARM)
  SQL_INT4_PARM(SquadDetails_SquadID,                               SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetails_EditRestrictions, 8,                   SQL_OUT_PARM)
  SQL_DATE_PARM(SquadDetails_InceptionDate,                         SQL_OUT_PARM) 
  SQL_INT2_PARM(SquadDetails_CivID,                                 SQL_OUT_PARM)
//  SQL_INT4_PARM(SquadDetails_CreatedByID,       SQL_OUT_PARM)
//  SQL_STR_PARM (SquadDetails_LastChangeDate,c_cbNameDB, SQL_OUT_PARM) 
//  SQL_INT4_PARM(SquadDetails_Rank,              SQL_OUT_PARM) TODO, do this
  SQL_INT4_PARM(SquadDetails_INPUT_SquadID,                         SQL_IN_PARM)

  //
  // Get Squad Details in terms of its players
  //
  // Input:    SquadID
  //
  // Output:   list of Member's Name, ID, date last played, rank, and status(pending, ASL, leader, etc)
  //
  DEF_STMT(SquadDetailsPlayers,
        "{Call SquadDetailsPlayers(?)}"      
        )
  SQL_INT4_PARM(SquadDetailsPlayers_MemberID,                   SQL_OUT_PARM)
  SQL_STR_PARM (SquadDetailsPlayers_MemberName,  c_cbNameDB,    SQL_OUT_PARM)
  SQL_INT4_PARM(SquadDetailsPlayers_Status,                     SQL_OUT_PARM) 
  SQL_INT4_PARM(SquadDetailsPlayers_Status2,                    SQL_OUT_PARM) 
  SQL_DATE_PARM(SquadDetailsPlayers_Granted,                    SQL_OUT_PARM) 
  SQL_INT4_PARM(SquadDetailsPlayers_Rank,                       SQL_OUT_PARM) 
  SQL_INT4_PARM(SquadDetailsPlayers_INPUT_SquadID,              SQL_IN_PARM)

  //
  // Input:     squadID
  //
  // Output:    ownership log
  //
  DEF_STMT(GetOwnershipLog, "select OwnershipLog from squads where SquadID=?")
  SQL_STR_PARM (GetOwnershipLog_Output,     c_cbOwnershipLogDB,   SQL_OUT_PARM)  
  SQL_INT4_PARM(GetOwnershipLog_SquadID,           SQL_IN_PARM)

  //
  // Make Join Request
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(MakeJoinRequest,
        "{Call SquadMakeJoinRequest(?,?)}"      
        )
  SQL_INT4_PARM(MakeJoinRequest_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(MakeJoinRequest_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(MakeJoinRequest_SquadID,           SQL_IN_PARM)


  //
  // CancelJoinRequest
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(CancelJoinRequest,
        "{Call SquadCancelJoinRequest(?,?)}"      
        )
  SQL_INT4_PARM(CancelJoinRequest_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(CancelJoinRequest_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(CancelJoinRequest_SquadID,           SQL_IN_PARM)

  //
  // RejectJoinRequest
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(RejectJoinRequest,
        "{Call SquadRejectJoinRequest(?,?)}"      
        )
  SQL_INT4_PARM(RejectJoinRequest_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(RejectJoinRequest_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(RejectJoinRequest_SquadID,           SQL_IN_PARM)

  //
  // AcceptJoinRequest
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(AcceptJoinRequest,
        "{Call SquadAcceptJoinRequest(?,?)}"      
        )
  SQL_INT4_PARM(AcceptJoinRequest_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(AcceptJoinRequest_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(AcceptJoinRequest_SquadID,           SQL_IN_PARM)

  //
  // BootMember
  //
  // Input:     memberID, squadID
  //
  // Output:    errorcode (0 means no error), status (-1 means team is destroyed)
  //
  DEF_STMT(BootMember,
        "{Call SquadBootMember(?,?)}"      
        )
  SQL_INT4_PARM(BootMember_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(BootMember_Status,            SQL_OUT_PARM) 
  SQL_INT4_PARM(BootMember_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(BootMember_SquadID,           SQL_IN_PARM)

  //
  // QuitSquad
  //
  // Input:     memberID, squadID
  //
  // Output:    errorcode (0 means no error), status (-1 means team is destroyed)
  //
  DEF_STMT(QuitSquad,
        "{Call SquadQuit(?,?)}"      
        )
  SQL_INT4_PARM(QuitSquad_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(QuitSquad_Status,            SQL_OUT_PARM) 
  SQL_INT4_PARM(QuitSquad_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(QuitSquad_SquadID,           SQL_IN_PARM)

  //
  // PromoteToASL
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(PromoteToASL,
        "{Call SquadPromoteToASL(?,?)}"      
        )
  SQL_INT4_PARM(PromoteToASL_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(PromoteToASL_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(PromoteToASL_SquadID,           SQL_IN_PARM)

  //
  // DemoteToMember
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(DemoteToMember,
        "{Call SquadDemoteToMember(?,?)}"      
        )
  SQL_INT4_PARM(DemoteToMember_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(DemoteToMember_MemberID,           SQL_IN_PARM)
  SQL_INT4_PARM(DemoteToMember_SquadID,           SQL_IN_PARM)

  //
  // TransferControl
  //
  // Input:     memberID, squadID
  //
  DEF_STMT(TransferControl,
        "{Call SquadTransferControl(?,?)}"      
        )
  SQL_INT4_PARM(TransferControl_ErrorCode,         SQL_OUT_PARM)
  SQL_INT4_PARM(TransferControl_MemberID,          SQL_IN_PARM)
  SQL_INT4_PARM(TransferControl_SquadID,           SQL_IN_PARM)

END_SQL_DEF

#endif
