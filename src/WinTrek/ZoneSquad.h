/*-------------------------------------------------------------------------
 * wintrek\ZoneSquad.h
 * 
 * Interface for IZoneSquad and CZonePlayer 
 *
 * These two classes are in the same file because are closely related.
 * When you add a player to a squad, a double link is created.  That is,
 * the squad points to the player and the player points to the squad.
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



class IZonePlayer;
class IZoneSquad;
class CMembership;


typedef TListListWrapper<TRef<IZonePlayer> >    XZonePlayers;
typedef TListListWrapper<TRef<IZoneSquad> >     XSquads;
typedef TListListWrapper<TRef<CMembership> >    XMemberships;


typedef XZonePlayers::Iterator                  XZonePlayersIt;
typedef XSquads::Iterator                       XSquadsIt;
typedef XMemberships::Iterator                  XMembershipsIt;




class IZoneSquad:
  public IObject,
  public ISquadBase
{
public:

  virtual char *GetName() = 0;
  virtual void  SetName(PCC szName) = 0;

  virtual int   GetID() = 0;                         // True ID determined by Zone database servers
  virtual void  SetID(int nID) = 0;

  virtual int   GetCreationID() = 0;                 // Creation ID is valid on client only
  virtual void  SetCreationID(int nCreationID) = 0;

  virtual char *GetDescription() = 0;
  virtual void  SetDescription(PCC szDescription) = 0;

  virtual char *GetURL() = 0;
  virtual void  SetURL(PCC szURL) = 0;

  virtual char *GetInceptionDate() = 0;
  virtual void  SetInceptionDate(char *szInceptionDate) = 0;

  virtual TRef<IZonePlayer>  GetOwner() = 0;

  virtual char const * GetRank() = 0;

  virtual int   GetRanking(SQUAD_SORT_COLUMN column)          = 0;
  virtual void  SetRanking(SQUAD_SORT_COLUMN column, int nRanking) = 0;

  virtual int   GetWins()             = 0;
  virtual void  SetWins(int nWins)    = 0;

  virtual int   GetLosses()           = 0;
  virtual void  SetLosses(int nLosses) = 0;

  virtual CivID GetCivID()            = 0;
  virtual void  SetCivID(int id)      = 0;

  virtual short GetScore()              = 0;
  virtual void  SetScore(short nScore)  = 0;

  virtual void  SetDetailsReceived() = 0;
  virtual bool  WereDetailsReceived() = 0; // do we know everything about this squad, or are some of its details yet to be received from server?

  virtual void  AddJoinRequest(IZonePlayer & pPlayer) = 0;
  virtual void  RejectPlayer(IZonePlayer & pPlayer) = 0; // reject player's join request
  virtual void  CancelJoinRequest(IZonePlayer & pPlayer) = 0;

  // Note: consider using SquadScreen::AddPlayerToSquad() instead of AddPlayer
  // to ensure proper screen refresh
  virtual void  AddPlayer(IZonePlayer & player, IMemberBase::DetailedStatus detailedstatus) = 0;
  virtual void  SetStatus(IZonePlayer & player, IMemberBase::DetailedStatus detailedstatus) = 0; // sets status (adds player as needed)
  virtual void  BootPlayer(IZonePlayer & pPlayer) = 0; 
  virtual void  QuitPlayer(IZonePlayer & player) = 0; 

  virtual void  SetEmpty() = 0;  // Clears all membership

  virtual void  SetOwnershipLog(char *szLog) = 0;
  virtual TList<ZString> * GetOwnershipLog() = 0;

  virtual void  AddPlayersToList(XZonePlayers & list) = 0;
};


TRef<IZoneSquad> CreateZoneSquad();


class IZonePlayer:
  public IObject,
  public IMemberBase
{
public:

  virtual void              Randomize() = 0; // fill in random data

  virtual char *            GetName()       = 0;
  virtual void              SetName(PCC szName) = 0;       

  virtual int               GetID() = 0;
  virtual void              SetID(int nID) = 0;

  virtual char *            GetLastPlayedDate(IZoneSquad & squad) = 0;
  virtual void              SetLastPlayedDate(IZoneSquad & squad, PCC szLastPlayedDate) = 0;       

  virtual DetailedStatus    GetStatus(IZoneSquad & squad) = 0;

  virtual Rank              GetRank()       = 0;
  virtual void              SetRank(Rank rank) = 0;

  virtual Position          GetPosition(IZoneSquad & squad) = 0;   
  virtual void              SetPosition(IZoneSquad & squad, Position pos) = 0; // setting to leader should only be done from IZoneSquad::SetOwner()

  virtual float             GetHours(IZoneSquad & squad) = 0;   
  virtual void              SetHours(IZoneSquad & squad, float fHours) = 0;   

  virtual bool              IsMemberOf(IZoneSquad & squad) = 0; // is this player a member of pSquad
  virtual bool              IsPendingFor(IZoneSquad & squad) = 0;
};

TRef<IZonePlayer> CreateZonePlayer(const char *szName, int nID);
