/*==========================================================================
 *
 *  Copyright (C) 1996,1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:  WinTrek.h
 *
 *  Purpose:Main header file
 *
 *  Author: J. Eugene Ball and Rick Rashid
 *
 *  Note:   This file derived in part on code from the game "Trek" written
 *        for the Xerox Alto by J. Eugene Ball and Rick Rashid, Copyright (c) 1978
 *        and used here by permission of the authors.
 *
 *
 ***************************************************************************/

#ifndef _WINTREK_
#define _WINTREK_

#include "treki.h"

extern bool g_bActivity; //Imago: AFK from Activity flag

const float s_fMaxFOV = RadiansFromDegrees(50.0f);
const float s_fDefaultFOV = RadiansFromDegrees(50.0f);
const float s_fMinFOV = RadiansFromDegrees(5.0f);

class TrekWindowImpl;
class AutoDownloadProgressDialogPopup; // MGC: move

/*!!!
class BillboardType {
public:
    TRef<AnimatedImage> m_pimage;
    float               m_lifespan;

    BillboardType() 
    {
    } 

    BillboardType(Modeler* pmodeler, const char* textureName, float lifespan, float sa, float ea) :
        m_lifespan(lifespan)
    {
        TRef<Surface> psurface = pmodeler->LoadSurface(textureName, true);

        if (psurface) {
            m_pimage = new AnimatedImage(new Number(0.0f), psurface);
        }
    }

    BillboardType(const BillboardType& type) :
        m_pimage(type.m_pimage),
        m_lifespan(type.m_lifespan)
    {
    }

    void Purge(void)
    {
        m_pimage = NULL;
    }
};
*/

//////////////////////////////////////////////////////////////////////////////
// WinTrek Client
const c_numGrooveLevels = 3;

class  WinTrekClient :
    public BaseClient,
    public IIntegerEventSink
{
    private:
        TRef<AnimatedImage> LoadExplosionImage(const ZString& str);
        bool OnEvent(IIntegerEventSource* pevent, int value);

    public:
        WinTrekClient(void);
        ~WinTrekClient(void);

        virtual void Initialize(Time timeNow);
        virtual void Terminate();
        ImissionIGC*    ResetStaticData (void);

        bool                    bInitTrekThrottle;
        //bool                    bInitTrekJoyStick; moved to baseclient
        bool                    joyThrottle;
        float                   trekThrottle;
        float                   trekJoyStick[3];
        float                   fOldJoyThrottle;
        virtual void            OverrideThrottle (float fDesiredThrottle);

        // igc site implementation

    protected:
        bool                    m_bDisconnected;
        TRef<TrekWindow>        m_pwindow;
        TRef<Engine>            m_pengine;
        TRef<Modeler>           m_pmodeler;
        TRef<AnimatedImage>     m_pimageFlare;
        float                   m_offset;
        TRef<Geo>               m_pgeoFlares[3];
        SoundID                 m_idAmbient;
        TRef<ISoundInstance>    m_psoundAmbient;
        TRef<ISoundInstance>    m_psoundMissileWarning;

        int                     m_nGrooveLevel;
        Time                    m_vtimeGrooveDrops[c_numGrooveLevels];

        SideID                  m_sideidLastWinner;
        bool                    m_bWonLastGame;
        bool                    m_bLostLastGame;
        ZString                 m_strGameOverMessage;
        int                     m_nNumEndgamePlayers;
        PlayerEndgameInfo*      m_vplayerEndgameInfo;
        int                     m_nNumEndgameSides;
        SideEndgameInfo         m_vsideEndgameInfo[c_cSidesMax];
        bool                    m_bEndgameEjectPods;
        bool                    m_bGameCounted;
        bool                    m_bScoresCounted;

        ZString                 m_strDisconnectReason;

        bool                    m_bFilterChatsToAll;
        bool                    m_bFilterQuickComms;
        bool                    m_bFilterLobbyChats;


        //
        // Explosions
        //

        TVector<TRef<AnimatedImage> > m_vpimageExplosion[8];
        TRef<Image>                   m_pimageShockWave;

        TRef<AnimatedImage> LoadExplosion(const ZString& str);
        
    public:
        virtual void      DevelopmentCompleted(IbucketIGC*  b, IdevelopmentIGC*   d, Time now);
        virtual void      StationTypeCompleted(IbucketIGC*  b, IstationIGC* pstation, IstationTypeIGC*   st, Time now);
        virtual void      DroneTypeCompleted(IbucketIGC*  b, IstationIGC* pstation, IdroneTypeIGC*   dt, Time now);
        virtual void      BuildStation(IasteroidIGC* pasteroid, IsideIGC* pside, IstationTypeIGC* pstationtype, Time now);
        virtual void      LayExpendable(Time now, IexpendableTypeIGC* pet, IshipIGC* pshipLayer);
        virtual TRef<ThingSite>   CreateThingSite(ImodelIGC* pModel);
        virtual TRef<ClusterSite> CreateClusterSite(IclusterIGC* pCluster);
        virtual void      PlaySoundEffect(SoundID soundID, ImodelIGC* model = NULL);
        virtual void      PlaySoundEffect(SoundID soundID, ImodelIGC* model, const Vector& vectOffset);
        virtual void      PlayNotificationSound(SoundID soundID, ImodelIGC* model);
        virtual void      PlayFFEffect(ForceEffectID effectID, ImodelIGC* model = NULL, LONG lDirection = 0);
        virtual void      PlayVisualEffect(VisualEffectID effectID, ImodelIGC* model = NULL, float fIntensity = 1.0f);
        virtual void      TerminateModelEvent(ImodelIGC*    pmodel);
        virtual bool      DockWithStationEvent(IshipIGC* ship, IstationIGC* station);
        virtual void      KillShipEvent(Time now, IshipIGC* ship, ImodelIGC* launcher, float amount, const Vector& p1, const Vector& p2);
        virtual void      DamageShipEvent(Time now,
                                          IshipIGC * pShip,
                                          ImodelIGC * pLauncher,
                                          DamageTypeID type, 
                                          float flAmount,
                                          float flLeakage,
                                          const Vector& p1,
                                          const Vector& p2);
        virtual void      TargetKilled(ImodelIGC*    pmodel);
        virtual void      ShipWarped(IshipIGC* ship, SectorID sidOld, SectorID sidNew);
        virtual void      ActivateTeleportProbe(IprobeIGC* pprobe);
        virtual void      DestroyTeleportProbe(IprobeIGC* pprobe);
        virtual void      PostText(bool bCritical, const char* pszText, ...);
        virtual void      PostNotificationText(ImodelIGC* pmodel, bool bCritical, const char* pszText, ...);
        virtual void      OverrideCamera(ImodelIGC*    pmodel);
        virtual void      EjectPlayer(ImodelIGC*    pmodel);
        virtual void      HitWarpEvent(IshipIGC* ship, IwarpIGC* warp);
        virtual bool      HitTreasureEvent(Time now, IshipIGC* ship, ItreasureIGC* treasure);
        virtual void      PaydayEvent(IsideIGC* pside, float money);
        virtual bool      UseRipcord(IshipIGC* pship, ImodelIGC* pmodel);
        virtual bool      ContinueRipcord(IshipIGC* pship, ImodelIGC* pmodel);

        virtual void      SetViewCluster(IclusterIGC* pcluster, const Vector* pposition = NULL);
        virtual void      RequestViewCluster(IclusterIGC* pcluster, ImodelIGC* pmodelTarget = NULL);
        virtual void      ReceiveChat(IshipIGC*   pshipSender,
                                      ChatTarget  ctRecipient,
                                      ObjectID    oidRecipient,
                                      SoundID     voiceOver,
                                      const char* szText,
                                      CommandID   cid,
                                      ObjectType  otTarget,
                                      ObjectID    oidTarget,
                                      ImodelIGC*  pmodelTarget = NULL,
                                      bool        bObjectModel = false);

        virtual void      GetMoneyRequest(IshipIGC* pshipSender, Money amount, HullID hidFor);

        bool              FilterChatsToAll() { return m_bFilterChatsToAll; };
        void              FilterChatsToAll(bool bFilterChatsToAll) { m_bFilterChatsToAll = bFilterChatsToAll; };
        bool              FilterQuickComms() { return m_bFilterQuickComms; };
        void              FilterQuickComms(bool bFilterQuickComms) { m_bFilterQuickComms = bFilterQuickComms; };
        bool              FilterLobbyChats() { return m_bFilterLobbyChats; };
        void              FilterLobbyChats(bool bFilterLobbyChats) { m_bFilterLobbyChats = bFilterLobbyChats; };
/*
        virtual void FireMissile(IshipIGC* pship, ImagazineIGC* pmagazine,
                        Time timeFired, ImodelIGC* pTarget, float lock);
*/
        virtual HRESULT   ConnectToServer(BaseClient::ConnectInfo & ci, DWORD dwCookie, Time now, const char* szPassword, bool bStandalonePrivate);
        virtual HRESULT   ConnectToLobby(BaseClient::ConnectInfo * pci); // pci is NULL if relogging in
        virtual HRESULT   ConnectToClub(BaseClient::ConnectInfo * pci);

        virtual void      JoinMission(MissionInfo * pMission, const char* szMissionPassword);
        virtual void      CreateMissionReq();
        
// IFedMessagingSite
        virtual HRESULT   OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
#ifndef NO_MSG_CRC
        virtual void      OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg);
#endif
        virtual void      OnLogonAck(bool fValidated, bool bRetry, LPCSTR szFailureReason);
        virtual void      OnLogonLobbyAck(bool fValidated, bool bRetry, LPCSTR szFailureReason);
        virtual void      OnLogonClubAck(bool fValidated, bool bRetry, LPCSTR szFailureReason);
        virtual HRESULT   OnSessionLost(char * szReason, FedMessaging * pthis);
        void              FlushSessionLostMessage();
        virtual void      Disconnect();
        virtual void      OnQuitMission(QuitSideReason reason, const char* szMessageParam);
        virtual void      ChangeStation(IshipIGC* s, IstationIGC* pstationOld, IstationIGC* pstationNew);
        virtual void      ChangeCluster(IshipIGC* s, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew);
        virtual void      LoadoutChangeEvent(IshipIGC* pship, IpartIGC* ppart, LoadoutChange lc);
        virtual IpartIGC* GetCargoPart(void);
        virtual void      NextCargoPart(void);
        virtual bool      SelectCargoPartOfType(EquipmentType et, PartMask maxPartMask, IpartTypeIGC* pptNotThis);
        virtual Mount     GetSelectedCargoMount();

        virtual void      OnReload(IpartIGC* ppart, bool bConsumed);
        virtual void      Preload(const char* pszModelName, const char* pszTextureName);
        virtual void      SetCDKey(const ZString& strCDKey);
        virtual void      ChangeGameState(void) { GetWindow()->UpdateGameStateContainer(); }

        virtual IAutoUpdateSink * OnBeginAutoUpdate(IAutoUpdateSink * pSink, bool bConnectToLobby);
        virtual bool ShouldCheckFiles();

        virtual const char* GetArtPath()  { return GetModeler()->GetArtPath(); }

        const char*       GetNameLogonClubServer() { return m_szClubCharName; };
        const char*       GetNameLogonZoneServer() { return m_szLobbyCharName; };

        IObject*          LoadRadarIcon(const char* szName);

        
        void              UpdateAmbientSounds(DWORD dwElapsedTime);
        void              ResetSound();

        Mount       mountSelected;
        bool        fGroupFire;

        WarningMask    wmOld; 

        TRef<ISoundInstance>  StartSound(ISoundTemplate* ptemplate, ISoundPositionSource* psource = NULL) 
            { return GetWindow()->StartSound(ptemplate, psource); };
        TRef<ISoundInstance>  StartSound(SoundID soundId, ISoundPositionSource* psource = NULL)
            { return GetWindow()->StartSound(soundId, psource); };

        void              ForwardSquadMessage(FEDMESSAGE* pSquadMessage); // forward message to squads screen
        void              ForwardCharInfoMessage(FEDMESSAGE* pCharInfoMessage); // forward message to character info screen
        void              ForwardLeaderBoardMessage(FEDMESSAGE* pCharInfoMessage); // forward message to character info screen
        SideID            GetLastWinner() { return m_sideidLastWinner; };
        bool              GetWonLastGame() { return m_bWonLastGame; };
        bool              GetLostLastGame() { return m_bLostLastGame; };
        void              SetGameoverInfo(FMD_S_GAME_OVER* pfmGameOver);
        void              AddGameoverPlayers(PlayerEndgameInfo* vEndgamePlayerInfo, int nCount);
        ZString           GetGameoverMessage();
        PlayerEndgameInfo* GetEndgamePlayerInfo(int nIndex);
        int               GetNumEndgamePlayers();
        SideEndgameInfo*  GetSideEndgameInfo(SideID sideId);
        int               GetNumEndgameSides();
        Color             GetEndgameSideColor(SideID id);
        CivID             GetEndgameSideCiv(SideID id);
        bool              GetGameoverEjectPods() { return m_bEndgameEjectPods; };
        bool              GetGameCounted() { return m_bGameCounted; };
        bool              GetScoresCounted() { return m_bScoresCounted; };

        virtual void      SaveSquadMemberships(const char* szCharacterName);
        virtual void      RestoreSquadMemberships(const char* szCharacterName);

        virtual void      StartLockDown(const ZString& strReason, LockdownCriteria criteria);
        virtual void      EndLockDown(LockdownCriteria criteria);

        int               GetGrooveLevel();
        ZString           GetSavedCharacterName();
        void              SaveCharacterName(ZString strName);
        

    private:

    friend class ClusterSiteImpl;
    friend class ThingSiteImpl;

    AutoDownloadProgressDialogPopup * m_pAutoDownloadProgressDialogPopup; 
};

const int c_PopupX = 150;
const int c_PopupY = 580;  // 0 == bottom of screen


extern WinTrekClient  trekClient;

extern void debugClock(const char*  file, long line = 0);
#define     debugClk()  debugClock(__FILE__, __LINE__)

extern bool g_bAskForCDKey;
// wlp 2006 - add askforcallsign so we only ask if it's not passed in on commandline
extern bool g_bAskForCallSign;  // wlp 2006
extern TRef<ZWriteFile> g_pzfFrameDump;
typedef struct 
{
    int objects_drawn[OT_modelEnd+1];
    int objects_insector[OT_modelEnd+1];
} FrameDumpInfo;
extern FrameDumpInfo g_fdInfo;

TRef<IMessageBox> CreateMessageBox(
    const ZString& str, 
    ButtonPane*    pbuttonIn         = NULL, 
    bool           fAddDefaultButton = true,
    bool           fAddCancelButton  = false,
    float          paintDelay        = 0.0f
);
TRef<IPopup> CreateCDKeyPopup();

#endif
