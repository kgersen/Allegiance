#pragma once

class ISquadBase // inheritted by IZoneSquad
{

public:

  enum Rank
  {
    RANK_START = 0,
    RANK_COTTON = 0,
    RANK_COPPER,
    RANK_BRONZE,
    RANK_SILVER,
    RANK_GOLD,
    RANK_PLATINUM,
    RANK_DOUBLE_PLATINUM,
    RANK_DIAMOND,
    RANK_DOUBLE_DIAMOND,
    RANK_TRIPLE_DIAMOND,
    RANK_MAX,
    RANK_UNKNOWN,
  };

  // mdvalley: 2005 needs the int
  static int IsRankValid(Rank rank)
  {
      return rank >= RANK_COTTON && rank < RANK_MAX;
  }

  static const char * Rank2String (Rank rank)
  {
    static const char * s_szRank[] = {
      "Cotton",
      "Copper",
      "Bronze",
      "Silver",
      "Gold",
      "Platinum",
      "Double Platinum",
      "Diamond",
      "Double Diamond",
      "Triple Diamond",
      "Max",
      "Unknown"
    };
    return s_szRank[rank];
  }
};


enum SQUAD_SORT_COLUMN
{
    SSC_UNKNOWN = 0,
    SSC_NAME,
    SSC_SCORE,
    SSC_WINS,
    SSC_LOSSES,
    SSC_HOURS,
    SSC_MAX,
};
