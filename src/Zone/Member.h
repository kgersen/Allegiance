

// munging macros to go to/from interal internal/external ratings
#define RATING_EXT2DISP(x) ((x) * 1000 / MAXSHORT)
#define RATING_EXT2INT(x) ((float) (x) / (float) MAXSHORT)
#define RATING_INT2EXT(x) (MAXSHORT * (x))

class IMemberBase // inheritted by IZonePlayer
{
public:

  enum Rank
  {
    RANK_UNKNOWN = 0,
    RANK_PRIVATE,
    RANK_SERGEANT,
    RANK_GENERAL,
  };

  static IsRankValid(Rank rank)
  {
      return rank >= RANK_PRIVATE && rank <= RANK_GENERAL;
  }

  static char * Rank2String (Rank rank)
  {
    static char* s_szRank[] = {
      "Unknown",
      "Private",
      "Sergant",
      "General",
    };
    return s_szRank[rank];
  }

  enum Position
  {
    POS_UNKNOWN = 0,
    POS_MEMBER,
    POS_ASL,
    POS_LEADER // if change this, you need to update enum DetailedStatus
  };

  static IsPositionValid(Position position)
  {
      return position >= POS_MEMBER && position <= POS_LEADER;
  }

  static char * Position2String (Position position)
  {
    static char* s_szPosition[] = {
      "Unknown",
      "Member",
      "ASL",
      "Leader",
    };
    return s_szPosition[position];
  }

  enum Status
  {
    STAT_NO_ASSOCIATION = 0,  
    STAT_BOOTED,
    STAT_REJECTED,
    STAT_QUIT,
    STAT_PENDING,
    STAT_ACTIVE, // if you add another need to update GetStatus -AND- maybe even GetDetailedStatus()
  };

  static IsStatusValid(Status status)
  {
      return status >= STAT_NO_ASSOCIATION && status <= STAT_ACTIVE;
  }

  static char * Status2String (Status status)
  {
    static char * s_szStatus[] = {
      "No Association",
      "Booted",
      "Rejected",
      "Retired",
      "Pending",
      "Active",
    };
    return s_szStatus[status];
  }

  enum DetailedStatus  // combo of position and status
  {
    DSTAT_NO_ASSOCIATION = 0,  // order is used for sorting in squad screen
    DSTAT_UNKNOWN,
    DSTAT_BOOTED,
    DSTAT_REJECTED,
    DSTAT_PENDING,
    DSTAT_MEMBER,
    DSTAT_ASL,
    DSTAT_LEADER, // if you add another need to update GetDetailedStatus() -AND- maybe even GetStatus()
  };

  static IsDetailedStatusValid(DetailedStatus status)
  {
      return status >= DSTAT_NO_ASSOCIATION && status <= DSTAT_LEADER;
  }

  static char * DetailedStatus2String (DetailedStatus status)
  {
    static char * s_szDetailedStatus[] = {
      "No Association",
      "Unknown",
      "Booted",
      "Rejected",
      "Pending",
      "Member",
      "ASL",
      "Leader",
    };
    return s_szDetailedStatus[status];
  }
};

