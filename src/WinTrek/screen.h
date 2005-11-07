#ifndef _screens_h_
#define _screens_h_

//////////////////////////////////////////////////////////////////////////////
//
// Screen
//
//////////////////////////////////////////////////////////////////////////////

class Screen : public IObject {
public:
    virtual Image*   GetImage() { return NULL; }
    virtual Pane*    GetPane()  { return NULL; }
    virtual WinPoint GetSize()  { return WinPoint(800, 600); }
    virtual void     OnFrame()  {}
};

//////////////////////////////////////////////////////////////////////////////
//
// Screen Constructors
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateTeamScreen(Modeler* pmodeler);
TRef<Screen> CreateGameScreen(Modeler* pmodeler);
TRef<Screen> CreateIntroScreen(Modeler* pmodeler);
TRef<Screen> CreateNewGameScreen(Modeler* pmodeler);
TRef<Screen> CreateGameOverScreen(Modeler* pmodeler);
TRef<Screen> CreateLeaderBoardScreen(Modeler* pmodeler, ZString strCharacter);
TRef<Screen> CreateZoneEventsScreen(Modeler* pmodeler);
TRef<Screen> CreateZoneClubScreen(Modeler* pmodeler, Number * ptime);
TRef<Screen> CreateSquadsScreen(Modeler* pmodeler, const char * szNameDudeX, int idZoneDudeX, const char * szSquad);
TRef<Screen> CreateCharInfoScreen(Modeler* pmodeler, int idZone); // if id is NA, current player is used
TRef<Screen> CreateHangarScreen(Modeler* pmodeler, const ZString& strNamespace);
TRef<Screen> CreateTrainingScreen(Modeler* pmodeler);
TRef<Screen> CreateTrainingSlideshow (Modeler* pmodeler, const ZString& strNamespace, int iMissionIndex);
TRef<Screen> CreatePostTrainingSlideshow (Modeler* pmodeler, const ZString& strNamespace);
TRef<Screen> CreateGameStartingScreen(Modeler* pmodeler);
TRef<Screen> CreateVideoScreen(Modeler* pmodeler, bool bIntroOnly);

TRef<IPopup> CreateMissionParametersPopup(Modeler* pmodeler);

extern bool g_bQuickstart;
extern bool g_bReloaded;   // as in after AutoUpdate
extern int  g_civStart;

extern char g_szCharName[c_cbName];
extern char g_szCharPW[c_cbName];

void LaunchFromHangar();

#endif
