/*-------------------------------------------------------------------------
 * e:\fed\src\fedsrv\FedSrv.H
 * 
 * Common stuff for server source files
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _FEDSRV_H_
#define _FEDSRV_H_

#include "..\AGC\AGCSite.h"

#undef new // because we override it

const int c_cbArtFilePath = 128;
const int c_cPlayersMax   = 500; // max players allowed on a server
const int c_cDronesMax    = 500; // max drones  allowed on a server
const int c_cShipsMax     = c_cPlayersMax + c_cDronesMax;

#if defined(ALLSRV_STANDALONE)
  const short c_cMaxPlayersPerGame = 200; // KGJV-; changed from 64 to 200. 
#else // defined(ALLSRV_STANDALONE)
  const short c_cMaxPlayersPerGame = 0x7FFF;
#endif // defined(ALLSRV_STANDALONE)

HRESULT pascal FedSrv_Init();
HRESULT pascal FedSrv_Terminate(void);
HRESULT pascal FedSrv_Pause();
HRESULT pascal FedSrv_Continue();

const bool c_fCheatsOff = false;
const bool c_fCheatsOn  = true;

const int  c_cShipsPerSectorMax = 110;
const int  c_cMissionsMax       = 10; // arbitrary number of how many missions there can be
const int  c_cMemberListRelloc  = 8; // size of member reallocs, *** keep it a power of two ***

class CAdminServer; // forward

/*********************
      Globals 
 ********************/

// *** App related stuff ***

// !!! The following enum must match the indices of the items in rgCfg in
// !!! LoadSettings() !!!
typedef enum
{
    karg_fTimeout, 
    karg_fWantInt3,       
    karg_nUpdatesPerSecond,
    karg_fDebugBreak,
    karg_ZoneAuth,
} FEDSRV_ARGS;

struct ClusterGroups 
{
  CFMGroup * pgrpClusterFlying;
  CFMGroup * pgrpClusterDocked;
};

struct  MapDescription
{
    short   mapID;
    char    szFileName[MAX_PATH + 1];
};


class MapFileSite : public IMissionSite, public IIgcSite
{
};

class FedSrvSiteBase :
  public IMissionSite,
  public IIgcSite,
#ifndef ALLSRV_STANDALONE
  public IFedMessagingSite, public ISQLSite, public ISQLSite2
#else // !ALLSRV_STANDALONE
  public IFedMessagingSite
#endif // !ALLSRV_STANDALONE
{
public:
  FedSrvSiteBase() :
    m_pfsMission(NULL)
  {}
// IIgcSite  
  void ClusterUpdateEvent(IclusterIGC* c);
  TRef<ThingSite>   CreateThingSite(ImodelIGC* pModel);
  TRef<ClusterSite>   CreateClusterSite(IclusterIGC* pCluster);
  void KillAsteroidEvent(IasteroidIGC* pasteroid, bool explodeF);
  void DrainAsteroidEvent(IasteroidIGC* pasteroid);
  void KillProbeEvent(IprobeIGC* pprobe);
  void KillMissileEvent(ImissileIGC* pmissile, const Vector& position);
  void KillTreasureEvent(ItreasureIGC* ptreasure);
  void KillBuildingEffectEvent(IbuildingEffectIGC* pbe);
  void KillMineEvent(ImineIGC* pmine);
  void KillShipEvent(Time timeCollision, IshipIGC* pship, ImodelIGC* plauncher, float amount, const Vector& p1, const Vector& p2);

  void DamageShipEvent(Time now, IshipIGC* ship, ImodelIGC* launcher, DamageTypeID type, float amount, float leakage, const Vector& p1, const Vector& p2);
  void DamageStationEvent(IstationIGC* station, ImodelIGC* launcher, DamageTypeID type, float amount);

  void KillStationEvent(IstationIGC* pstation, ImodelIGC* plauncher, float amount);
  bool DockWithStationEvent(IshipIGC* pship, IstationIGC* pstation);
  void CaptureStationEvent(IshipIGC* pship, IstationIGC* pstation);
  void WarpBombEvent(IwarpIGC* pwarp, ImissileIGC* pmissile);
  bool RescueShipEvent(IshipIGC* pshipRescued, IshipIGC* pshipRescuer);
  bool LandOnCarrierEvent(IshipIGC* pshipCarrier, IshipIGC* pship, float tCollision);
  void ChangeStation(IshipIGC* pship, IstationIGC* pstationOld, IstationIGC* pstationNew);
  void * GetDroneFromShip(IshipIGC * pship);
  bool ContinueRipcord(IshipIGC* pship, ImodelIGC* pmodel);
  bool UseRipcord(IshipIGC* pship, ImodelIGC* pmodel);
  void RequestRipcord(IshipIGC* pship, IclusterIGC* pcluster);
  void HitWarpEvent(IshipIGC* pship, IwarpIGC* pwarp);
  bool HitTreasureEvent(Time now, IshipIGC* ship, ItreasureIGC* treasure);
  void PaydayEvent(IsideIGC* pside, float money);
  void DevelopmentCompleted(IbucketIGC*  b, IdevelopmentIGC*   d, Time now);
  void HullTypeCompleted(IsideIGC* pside, IhullTypeIGC* pht);
  void PartTypeCompleted(IsideIGC* pside, IpartTypeIGC* ppt);
  void StationTypeCompleted(IbucketIGC * pbucket, IstationIGC* pstation, IstationTypeIGC * pstationtype, Time now);
  void DroneTypeCompleted(IbucketIGC*  b, IstationIGC* pstation, IdroneTypeIGC*   dt, Time now);
  void BuildStation(IasteroidIGC*    pasteroid,
                    IsideIGC*        pside,
                    IstationTypeIGC* pstationtype, Time now);
  void SideDevelopmentTechChange(IsideIGC*    pside);
  void SideGlobalAttributeChange(IsideIGC*    pside);
  void ChangeCluster(IshipIGC* pship,
                     IclusterIGC* pclusterOld,
                     IclusterIGC* pclusterNew);
  void LoadoutChangeEvent(IshipIGC* pship, IpartIGC* ppart, LoadoutChange lc);
  void CreateSideEvent(IsideIGC * pIsideIGC);
  void DestroySideEvent(IsideIGC * pIsideIGC);
  void TerminateModelEvent(ImodelIGC* model);
  void TerminateMissionEvent(ImissionIGC* pMission) {}
  void CommandChangedEvent(Command i, IshipIGC * pship, ImodelIGC* ptarget, CommandID cid);

  void        CreateBuildingEffect(Time           now,
                                   IasteroidIGC*  pasteroid,
                                   IshipIGC*      pshipBuilder);
  void        LayExpendable(Time                   now,
                            IexpendableTypeIGC*    pet,
                            IshipIGC*              pshipLayer);
  void SendChat(IshipIGC*   pshipSender,
                ChatTarget  ctRecipient,
                ObjectID    oidRecipient,
                SoundID     soVoiceOver,
                const char* pszText,
                CommandID   cid       = c_cidNone,
                ObjectType  otTarget  = NA,
                ObjectType  oidTarget = NA,
                ImodelIGC*  pmodelTarget = NULL,
                bool        bObjectModel = false);

// IMissionSite
  void Create(CFSMission * pfsMission) {m_pfsMission = pfsMission; }
  void Destroy(CFSMission * pfsMission);

// IFedMessagingSite
  virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) ;
  virtual int     OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType) ;
  virtual HRESULT OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) ;
  virtual HRESULT OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) ;
  virtual HRESULT OnSessionLost(FedMessaging * pthis) ;
  virtual HRESULT OnSysMessage(FedMessaging * pthis);
  virtual void    OnMessageSent(FedMessaging * pthis, CFMRecipient * precip, const void * pv, DWORD cb, FMGuaranteed fmg);
#ifndef NO_MSG_CRC
  virtual void    OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg);
#endif

// Also for ISQLSite
  virtual int  OnMessageBox(const char * strText, const char * strCaption, UINT nType);

// ISQLSite2
  virtual void OnSQLErrorRecord(SSERRORINFO * perror, OLECHAR * postrError);
  virtual void OnOLEDBErrorRecord(BSTR bstrDescription, GUID guid, DWORD dwHelpContext, 
                                  BSTR bstrHelpFile,    BSTR bstrSource );

//Not part of IIgcSite
  void UpgradeShip(IshipIGC*  pship, const IstationIGC* pstation);
  void UpgradeDrones(IsideIGC* pside);

  void RespawnFlag(SideID sidFlag);
  
protected:
  CFSMission * m_pfsMission;
};


typedef CAGCSite<FedSrvSiteBase> FedSrvSite;

class FedSrvLobbySite : public IFedMessagingSite
{
public:
  FedSrvLobbySite() 
  {}
// IFedMessagingSite
  virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) ;
  virtual int     OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType) ;
  virtual HRESULT OnSessionLost(FedMessaging * pthis);
#ifndef NO_MSG_CRC
  virtual void    OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg);
#endif
};


struct FedTechBit
{
public:
  FedTechBit() {ZeroMemory(this, sizeof(*this));}
  char szName[CbTechBitName];
  int bitnum;
};

struct Global
{ 
  Global() :
    ssh(0),
    hInst(0),
    trekCore(NULL),
    dpidEveryone(0),
    idReceiveThread(0),
    hReceiveThread(0),
    hKillReceiveEvent(0),
    hPlayerEvent(0),
    fSecure(false),
    fTimeOut(false),
    pServerCounters(NULL),
    pargPoster(NULL),
    cbPosters(0),
    cMsgsToOthers(0),
    cBytesToOthers(0),
    timeNow(Time::Now()),
    fm(&siteFedSrv),
    fmLobby(&siteFSLobby),
    dwServerGITCookie(0),
    fProtocol(false),
    fPaused(false),
    fDoublePrecision(true)
#if !defined(ALLSRV_STANDALONE)
    ,
    csqlSilentThreads(0),
    csqlNotifyThreads(0),
    sql(&siteFedSrv)
#endif    
  {
    ZeroMemory(rgTechs, sizeof(rgTechs));
  }

  // Explicit initialization
  void Init()
  {
    // Explicitly initialize the ZString objects
    strLobbyServer.SetEmpty();
    strAuthServer.SetEmpty();
    strArtPath.SetEmpty();
#if !defined(ALLSRV_STANDALONE)
    strSQLConfig.Empty();
#endif    
  }

  SERVICE_STATUS_HANDLE ssh;

  HINSTANCE           hInst;
  SWMRG               swmrg;
  FedSrvSite          siteFedSrv;
  FedSrvLobbySite     siteFSLobby;
  DPID                dpidEveryone;     // People in the mission lobby and every mission
  DPID                dpidLobby;        // people in mission lobby
  ImissionIGC *       trekCore;         // gateway to everything that is IGC

  // *** Network layer stuff ***
  DWORD               idReceiveThread;
  HANDLE              hReceiveThread;
  HANDLE              hKillReceiveEvent;
  HANDLE              hPlayerEvent;
  FedMessaging        fm;               // all missions share a session for now
  FedMessaging        fmLobby;
  bool                fSecure;
  bool                fTimeOut;
  bool                fWantInt3;
  ZString             strArtPath;
  char                m_szToken[24]; // sizeof(_ZONETICKET_TOKEN.szToken)
  int                 nUpdatesPerSecond;
  TRef<IZoneAuthServer> pzas;

  // *** Game state stuff ***
  Time                timeNow;             // cached value (for very short duration) of timeGetTime()

  // *** Perfmon counter stuff ***
  CPerfShare          perfshare;
  SERVER_COUNTERS *   pServerCounters;

  // Keep list of everything not in IGC, so no database access on each client connect
  FMD_S_POSTER *      pargPoster;
  CB                  cbPosters;

  int                 cMsgsToOthers;  // temp stuff for tracking amount of data sent during logon to players
  int                 cBytesToOthers; // other than the player logging on

  DWORD               dwServerGITCookie;
  ZString             strLobbyServer;
  ZString             strAuthServer;
  ZString             strLocalAddress;
  char                rgTechs[cTechs][CbTechBitName+1];

  RankInfo*           vRankInfo;
  short               cRankInfo;
  StaticMapInfo*      vStaticMapInfo;
  short               cStaticMapInfo;

  bool                fProtocol;
  bool                fPaused;
  bool                fDoublePrecision;
  //  CAdminSponsor<CAdminGames>  sponsorGames;
#if !defined(ALLSRV_STANDALONE)
  CSQLCore            sql;
  DWORD               csqlSilentThreads;
  DWORD               csqlNotifyThreads;
  CComBSTR            strSQLConfig;
#endif
};


// ******* Functions/Declarations that need prototypes *********
class CFSShip;
class CFSPlayer;
class CFSDrone;

struct DataEnumPlayers
{
  int indent;
  CFSShip * pfsShip;
};

HRESULT pascal FedSrv_Init(); // needed by AllSrvModule.cpp
HRESULT pascal FedSrv_Terminate(void);
void DisconnectFromLobby();
void ChangeStationOwners(IstationIGC * pstation, CFSMission * pfsMission, IsideIGC* iSideNew);
void ExportObj(IbaseIGC * pIGC, ObjectType ot, FMD_S_EXPORT ** ppfmExport);
void CheatDPGroups(CFSShip * pfsShip);
BOOL FAR PASCAL EnumPlayersCallback(DPID dpId, DWORD dwPlayerType, 
        LPCDPNAME lpName, DWORD dwFlags, LPVOID lpContext);
void SendPlayerInfo(CFSPlayer * pfsPlayer, CFSShip * pfsShip, CFSMission* pfsMission, bool bSend = true);
bool LeaveShip(CFSShip* pfsShip, IshipIGC* pshipParent);
CFMGroup * GetGroupSectorDocked(IclusterIGC * pCluster);
CFMGroup * GetGroupSectorFlying(IclusterIGC * pCluster);

CFSDrone * CreateStockDrone(IdroneTypeIGC * pdronetype, IsideIGC* pSide, IbaseIGC* pbase = NULL);

void SetCharStats(int characterID, CFSPlayer* pfsPlayer, IsideIGC * pSide, PlayerScoreObject & pso, CFSMission* pfsMission);
void RecordSquadGame(const SideListIGC*  psides, IsideIGC* psideWon);

void GetInvitations(int nInvitationID, MissionID missionID);

//-KGJV - AZ uses StartDefaultGames now
//#if defined(ALLSRV_STANDALONE)
void StartDefaultGames();
//#endif

ItreasureIGC*  CreateTreasure(Time now, IshipIGC* pship, DataTreasureIGC* pdt, const Vector& position, float dv);

IshipIGC*   PickNewLeader(const ShipListIGC*    pships,
                          IshipIGC*             pshipIgnore,
                          int                   maxVotes);
void SetSessionDetails();
HRESULT ConnectToLobby(const char * pszLobby_OverrideCFG /* NULL == do not override */);
  
#define TrapHack(x) assert(x)

#define wm_fs_pause WM_APP // wparam/lparam not used

#endif // _FEDSRV_H_
