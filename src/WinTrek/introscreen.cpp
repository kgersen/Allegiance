#include "pch.h"
#include "regkey.h"
#include "training.h"

extern bool CheckNetworkDevices(ZString& strDriverURL);

//////////////////////////////////////////////////////////////////////////////
//
// Intro Screen
//
//////////////////////////////////////////////////////////////////////////////

class IntroScreen :
    public Screen,
    public TrekClientEventSink,
    public EventTargetContainer<IntroScreen>,
    public LogonSite,
    public PasswordDialogSink
{
private:
    TRef<Modeler>       m_pmodeler;
    TRef<Pane>          m_ppane;

    TRef<ButtonPane>    m_pbuttonPlayLan;
    TRef<ButtonPane>    m_pbuttonPlayInt;
#ifdef USEAZ
    TRef<ButtonPane>    m_pbuttonZoneClub;
#endif
    //TRef<ButtonPane> m_pbuttonTraining;
    TRef<ButtonPane>    m_pbuttonTrainingBig;
    TRef<ButtonPane>    m_pbuttonZoneWeb;
    TRef<ButtonPane>    m_pbuttonOptions;
    TRef<ButtonPane>    m_pbuttonIntro;
    TRef<ButtonPane>    m_pbuttonCredits;
    //TRef<ButtonPane> m_pbuttonQuickstart;
    TRef<ButtonPane>    m_pbuttonExit;
    TRef<ButtonPane>    m_pbuttonHelp;
    TRef<IMessageBox>   m_pMsgBox;
    GUID                m_guidSession;
    ZString             m_strCharacterName;
    ZString             m_strPassword;
    MissionInfo*        m_pmissionJoining;

    enum
    {
        hoverNone,
        hoverPlayLan,
        hoverPlayInt,
#ifdef USEAZ
        hoverZoneClub,
#endif        
        hoverTrain,
        hoverZoneWeb,
        hoverOptions,
        hoverIntro,
        hoverCredits,
        hoverQuickstart,
        hoverExit,
        hoverHelp
    };

    TRef<ModifiableNumber>  m_pnumberCurrentHover;

    friend class CreditsPopup;
    class CreditsPopup : public IPopup, public EventTargetContainer<CreditsPopup> 
    {
    private:
        TRef<Pane> m_ppane;
        TRef<ButtonPane> m_pbuttonClose;
        
        IntroScreen* m_pparent;

        TRef<IKeyboardInput> m_pkeyboardInputOldFocus;

    public:
        
        CreditsPopup(TRef<INameSpace> pns, IntroScreen* pparent, Number* ptime)
        {
            m_pparent = pparent;

            TRef<WrapNumber> pcreditsTime;

            CastTo(pcreditsTime,   pns->FindMember("creditsTime"));
            CastTo(m_ppane,        pns->FindMember("screen"));
            CastTo(m_pbuttonClose, pns->FindMember("closeButtonPane"));

            pcreditsTime->SetWrappedValue(Subtract(ptime, new Number(ptime->GetValue())));

            AddEventTarget(OnButtonClose, m_pbuttonClose->GetEventSource());
        }

        //
        // IPopup methods
        //

        virtual void OnClose()
        {
            if (m_pkeyboardInputOldFocus)
                GetWindow()->SetFocus(m_pkeyboardInputOldFocus);

            m_pkeyboardInputOldFocus = NULL;

            IPopup::OnClose();
        }

        virtual void SetContainer(IPopupContainer* pcontainer)
        {
            // initialize the check boxes
            
            m_pkeyboardInputOldFocus = GetWindow()->GetFocus();
            
            IPopup::SetContainer(pcontainer);
        }

        Pane* GetPane()
        {
            return m_ppane;
        }

        bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
        {
            // we need to make sure we get OnChar calls to pass on to the edit box
            fForceTranslate = true;
            return false;
        }

        bool OnChar(IInputProvider* pprovider, const KeyState& ks)
        {
            return true;
        }

        bool OnButtonClose()
        {
            
            m_pcontainer->ClosePopup(this);
            
            return true;
        }
    };


    friend class FindServerPopup;
    class FindServerPopup : public IPopup, public EventTargetContainer<FindServerPopup>, public IItemEvent::Sink
    {
    private:
        TRef<Pane> m_ppane;
        //TRef<ButtonPane>    m_pbuttonFind;
        TRef<ButtonPane>    m_pbuttonJoin;
        TRef<ButtonPane>    m_pbuttonCancel;
        //TRef<EditPane>      m_peditPane;
        TRef<ListPane>      m_plistPane;

        TRef<IItemEvent::Source>   m_peventServerList;
        TRef<TEvent<ItemID>::Sink> m_psinkServerList;

        IntroScreen* m_pparent;
        //TRef<IKeyboardInput> m_pkeyboardInputOldFocus;
        bool m_bDoBackgroundPolling;
        TRef<LANServerInfo> m_pserverSearching;

        TRef<IMessageBox> m_pmsgBox;

        typedef TListListWrapper<TRef<LANServerInfo> > ServerList;
        TRef<ServerList> m_plistServers;

        class ServerItemPainter : public ItemPainter
        {
            int m_nWidth;
            int m_nGameNameX;

        public:

            ServerItemPainter(int nGameNameX, int nWidth)
                : m_nWidth(nWidth), m_nGameNameX(nGameNameX) {};

            int GetXSize()
            {
                return m_nWidth;
            }

            int GetYSize()
            {
                return 14;
            }

            void Paint(ItemID pitemArg, Surface* psurface, bool bSelected, bool bFocus)
            {
                LANServerInfo* serverInfo = (LANServerInfo*)pitemArg;
                const int nPlayerWidth = 40;
                
                if (bSelected) {
                    psurface->FillRect(
                        WinRect(0, 0, GetXSize(), GetYSize()),
                        Color(1, 0, 0)
                    );
                }

                TRef<IEngineFont> pfont = TrekResources::SmallFont();
                Color             color = Color::White();

                WinRect rectClipOld = psurface->GetClipRect();
                psurface->SetClipRect(WinRect(WinPoint(0, 0), WinPoint(GetXSize() - nPlayerWidth, GetYSize()))); // clip name to fit in row
                psurface->DrawString(pfont, color, WinPoint(2, 0), serverInfo->strGameName);
                psurface->RestoreClipRect(rectClipOld);

                if (serverInfo->nMaxPlayers != 0)
                {
                    char cbPlayers[20];
                    wsprintf(cbPlayers, "(%d/%d)", serverInfo->nNumPlayers, serverInfo->nMaxPlayers);
                    psurface->DrawString(pfont, color, WinPoint(GetXSize() - nPlayerWidth + 2, 0), cbPlayers);
                }
            }
        };

        static bool GameNameCompare(ItemID pitem1, ItemID pitem2)
        {
            LANServerInfo* serverInfo1 = (LANServerInfo*)pitem1;
            LANServerInfo* serverInfo2 = (LANServerInfo*)pitem2;

            return _stricmp(serverInfo1->strGameName, serverInfo2->strGameName) > 0;
        }

    public:
        
        FindServerPopup(TRef<INameSpace> pns, IntroScreen* pparent)
        {
            m_bDoBackgroundPolling = false;
            m_pparent = pparent;
            Value* pGameNameX;
            Value* pListXSize;

            CastTo(m_ppane,                 pns->FindMember("ServerDialog"));

            //CastTo(m_pbuttonFind,           pns->FindMember("serverFindButtonPane"));
            CastTo(m_pbuttonJoin,           pns->FindMember("serverJoinButtonPane"));
            CastTo(m_pbuttonCancel,         pns->FindMember("serverCancelButtonPane"));
            //CastTo(m_peditPane,      (Pane*)pns->FindMember("serverEditPane"));
            CastTo(m_plistPane,      (Pane*)pns->FindMember("serverListPane"));
            CastTo(pGameNameX,              pns->FindMember("serverListGameNameX"));
            CastTo(pListXSize,              pns->FindMember("serverListWidth"));

            //AddEventTarget(OnButtonFind, m_pbuttonFind->GetEventSource());
            AddEventTarget(OnButtonJoin, m_pbuttonJoin->GetEventSource());
            AddEventTarget(OnButtonCancel, m_pbuttonCancel->GetEventSource());
            AddEventTarget(OnButtonJoin, m_plistPane->GetDoubleClickEventSource());

            m_peventServerList = m_plistPane->GetSelectionEventSource();
            m_peventServerList->AddSink(m_psinkServerList = new IItemEvent::Delegate(this));
            m_plistPane->NeedLayout();
            m_plistPane->SetItemPainter(new ServerItemPainter(GetNumber(pGameNameX), GetNumber(pListXSize)));
            m_plistServers = new ServerList();
            m_plistPane->SetList(new SortedList<ItemIDCompareFunction>(m_plistServers, GameNameCompare));

            m_pbuttonJoin->SetEnabled(false);

            m_pserverSearching = new LANServerInfo(GUID_NULL, "Searching...", 0, 0);

            AddEventTarget(PollForServers, GetWindow(), 1.0f);
        }

        ~FindServerPopup()
        {
            m_peventServerList->RemoveSink(m_psinkServerList);
        }

        //
        // IPopup methods
        //

        Pane* GetPane()
        {
            return m_ppane;
        }

        virtual void OnClose()
        {
            //if (m_pkeyboardInputOldFocus)
            //    GetWindow()->SetFocus(m_pkeyboardInputOldFocus);

            //m_pkeyboardInputOldFocus = NULL;
            m_bDoBackgroundPolling = false;

            IPopup::OnClose();
        }

        virtual void SetContainer(IPopupContainer* pcontainer)
        {
            //m_pkeyboardInputOldFocus = GetWindow()->GetFocus();
            //GetWindow()->SetFocus(m_peditPane);
            m_bDoBackgroundPolling = true;

            FindGames("");
            IPopup::SetContainer(pcontainer);
        }

        bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
        {
            // we need to make sure we get OnChar calls to pass on to the edit box
            fForceTranslate = true;
            return false;
        }
        /*
        bool OnChar(IInputProvider* pprovider, const KeyState& ks)
        {
            if (ks.vk == 13)
            {
                OnButtonFind();
                return true;
            }
            else
                return ((IKeyboardInput*)m_peditPane)->OnChar(pprovider, ks);
        }
        
        bool OnButtonFind()
        {
            FindGames(m_peditPane->GetString());

            return true;
        }
        */
        bool PollForServers()
        {
            if (m_bDoBackgroundPolling)
                FindGames(NULL);

            return true;
        }

        void FindGames(const char* szServerName)
        {
			// WLP 2005 - turned off the every second search to keep the mouse responding
			// added next line to turn off polling
            //m_bDoBackgroundPolling = false ;	// TE: Commented now that enumeration is done asynchronously
            TList<TRef<LANServerInfo> > listResults;

            trekClient.FindStandaloneServersByName(szServerName, listResults);


            // WLP 2005 - DPLAY8 upgrade addition code
            // DPLAY8 EnumHosts returns duplicates in the list
            // so we have to strip them out
            // How this works =
            // destroy all dupes to the first entry
            // destroy all dupes to the second entry - etc.
            // just keep at it until the end
            
		    TList<TRef<LANServerInfo> >::Iterator iterNew(listResults);   // Walking list 
		    for( int i = 1 ; i < listResults.GetCount() ; i++ ) // Walk the entire list 
		    {
		    iterNew.First();     // set on the first one
            for ( int j = 1 ; j < i ; j++, iterNew.Next() ) ;               // find the starting position
			GUID matchGUID = iterNew.Value()->guidSession ;               // this is the one to check for dupes
		
			iterNew.Next();             // bump to the next one to start comparing
            while (!iterNew.End())                                   // walk from here to the end checking dupes
                {
				    if (memcmp(&(iterNew.Value()->guidSession),      // compare GUID for dupes
				            &matchGUID, sizeof(GUID)) == 0)
	    	        iterNew.Remove();
			        else iterNew.Next();
		        }
		    }

            // WLP - 2005 end of add for DPLAY8 upgrade

            // add to or update the the server list with the new results
            // Note: we have to maintain some degree of consistancy to 
            // make sure scrolling an selection work, so unfortunately we
            // can't just copy.

            ServerList::Iterator iterOld(*m_plistServers);
            while (!iterOld.End())
            {
                bool bFound = false;
                ZString strOldName = iterOld.Value()->strGameName;

                for (TList<TRef<LANServerInfo> >::Iterator iterNew(listResults); 
                    !iterNew.End(); iterNew.Next())
                {
                    ZString strNewName = iterNew.Value()->strGameName;
                    if (memcmp(&(iterNew.Value()->guidSession), 
                            &(iterOld.Value()->guidSession), sizeof(GUID)) == 0
                        && iterNew.Value()->nMaxPlayers > 1
                        && iterNew.Value()->nMaxPlayers < 1000)
                    {
                        // found it - update it with the new info
                        bFound = true;
                        iterOld.Value()->strGameName = iterNew.Value()->strGameName;
                        iterNew.Remove();
                        break;
                    }
                }

                // didn't find it in the new list, so nuke it.
                if (!bFound)
                    iterOld.Remove();
                else
                    iterOld.Next();
            }

            // for anything else that's new, just add it to the end.
            for (TList<TRef<LANServerInfo> >::Iterator iterNew(listResults); 
                !iterNew.End(); iterNew.Next())
            {
                // don't show single player games or zone server games
                if (iterNew.Value()->nMaxPlayers > 1
                    && iterNew.Value()->nMaxPlayers < 1000)
                {
                    ZString strNewName = iterNew.Value()->strGameName;
                    m_plistServers->PushEnd(iterNew.Value());
                }
            }

            if (m_plistServers->IsEmpty())
            {
                // put "Searching..." in the list.
                m_plistServers->PushEnd(m_pserverSearching);
            }
        }

        bool OnButtonJoin()
        {
            //GetWindow()->SetFocus(m_pkeyboardInputOldFocus);
            //m_pkeyboardInputOldFocus = NULL;
            m_bDoBackgroundPolling = false;

            if (m_ppopupOwner) {
                m_ppopupOwner->ClosePopup(this);
            } else {
                m_pcontainer->ClosePopup(this);
            }

            LANServerInfo* serverInfo = (LANServerInfo*)(m_plistPane->GetSelection());
            
            
            if (serverInfo != NULL && serverInfo != m_pserverSearching)
                m_pparent->OnPickServer(serverInfo->guidSession);

            return true;
        }

        bool OnButtonCancel()
        {
            //GetWindow()->SetFocus(m_pkeyboardInputOldFocus);
            //m_pkeyboardInputOldFocus = NULL;
            m_bDoBackgroundPolling = false;

            if (m_ppopupOwner) {
                m_ppopupOwner->ClosePopup(this);
            } else {
                m_pcontainer->ClosePopup(this);
            }

            return true;
        }

        bool OnEvent(IItemEvent::Source *pevent, ItemID pitem)
        {
            LANServerInfo* serverInfo = (LANServerInfo*)pitem;

            m_pbuttonJoin->SetEnabled(serverInfo != NULL && serverInfo != m_pserverSearching);

            return true;
        }
    };    

    TRef<CreditsPopup> m_pcreditsPopup;
    TRef<FindServerPopup> m_pfindServerPopup;

    class CloseTrainWarnSink : public IIntegerEventSink {
    public:
        IntroScreen* m_pScreen;
        TRef<ButtonPane> m_pDontAskMeAgain;

        CloseTrainWarnSink(IntroScreen* pScreen, ButtonPane* pDontAskMeAgain) 
            : m_pScreen (pScreen), m_pDontAskMeAgain (pDontAskMeAgain) {}

        bool OnEvent(IIntegerEventSource* pevent, int value)
        {
            if (value == IDOK) {
                m_pScreen->OnButtonTraining ();
            }
            
            if (m_pDontAskMeAgain->GetChecked()) {
                // here we mark things as if a training mission has been launched
                HKEY    hKey;
                DWORD   dwHasRunTraining = 1;
                if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_WRITE, &hKey)) 
                {
                    RegSetValueEx (hKey, "HasTrained", NULL, REG_DWORD, (const BYTE*) &dwHasRunTraining, sizeof (dwHasRunTraining));
                    RegCloseKey (hKey);
                }
            }
            return false;
        }
    };

    class URLMessageSink : public IIntegerEventSink {
    public:
        ZString m_strURL;

        URLMessageSink(ZString strURL) :
            m_strURL(strURL)
        {
        }

        bool OnEvent(IIntegerEventSource* pevent, int value)
        {
            if (value == IDOK) {
                GetWindow()->ShowWebPage(m_strURL);
            }
            return false;
        }
    };

    bool CheckNetworkDrivers()
    {
        ZString strDriverURL;

        if (!CheckNetworkDevices(strDriverURL))
        {
            TRef<IMessageBox> pMsgBox = CreateMessageBox("The drivers of the "
                "network card you are using are known not to work with "
                "Microsoft Allegiance.  Please visit " + strDriverURL 
                + " to download updated drivers.  Press <OK> to go there now.",
                NULL, true, true);

            pMsgBox->GetEventSource()->AddSink(new URLMessageSink(strDriverURL));
            GetWindow()->GetPopupContainer()->OpenPopup(pMsgBox, false);

            return false;
        }        
        else
            return true;
    }

public:

    IntroScreen(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
        
        trekClient.DisconnectClub();

        TRef<INameSpace> pnsIntroScreen = m_pmodeler->CreateNameSpace("introscreendata", m_pmodeler->GetNameSpace("gamepanes"));
        pnsIntroScreen->AddMember("hoverNone",       new Number(hoverNone        ));
        pnsIntroScreen->AddMember("hoverPlayLan",    new Number(hoverPlayLan        ));
        pnsIntroScreen->AddMember("hoverPlayInt",    new Number(hoverPlayInt)    );
#ifdef USEAZ
        pnsIntroScreen->AddMember("hoverZoneClub",   new Number(hoverZoneClub    ));
#endif
        pnsIntroScreen->AddMember("hoverZoneWeb",    new Number(hoverZoneWeb        ));
        pnsIntroScreen->AddMember("hoverTrain",      new Number(hoverTrain        ));
        pnsIntroScreen->AddMember("hoverOptions",    new Number(hoverOptions      ));
        pnsIntroScreen->AddMember("hoverIntro",      new Number(hoverIntro        ));
        pnsIntroScreen->AddMember("hoverCredits",    new Number(hoverCredits        ));
        pnsIntroScreen->AddMember("hoverQuickstart", new Number(hoverQuickstart  ));
        pnsIntroScreen->AddMember("hoverExit",       new Number(hoverExit        ));
        pnsIntroScreen->AddMember("hoverHelp",       new Number(hoverHelp        ));

        pnsIntroScreen->AddMember("CurrentHover", m_pnumberCurrentHover = new ModifiableNumber(hoverNone));

        TRef<INameSpace> pns = pmodeler->GetNameSpace("introscreen");
        CastTo(m_ppane, pns->FindMember("screen"));
        CastTo(m_pbuttonPlayLan,    pns->FindMember("playLanButtonPane"));
        CastTo(m_pbuttonPlayInt,    pns->FindMember("playIntButtonPane"));
#ifdef USEAZ
        CastTo(m_pbuttonZoneClub,   pns->FindMember("zoneClubButtonPane" ));
#endif
        //CastTo(m_pbuttonTraining,   pns->FindMember("trainButtonPane"));
        CastTo(m_pbuttonTrainingBig,pns->FindMember("trainBigButtonPane"));
        CastTo(m_pbuttonZoneWeb,    pns->FindMember("zoneWebButtonPane" ));
        CastTo(m_pbuttonOptions,     pns->FindMember("optionsButtonPane"));
        CastTo(m_pbuttonIntro,      pns->FindMember("introButtonPane"));
        CastTo(m_pbuttonCredits,    pns->FindMember("creditsButtonPane"));
        //CastTo(m_pbuttonQuickstart, pns->FindMember("quickstartButtonPane"));
        CastTo(m_pbuttonExit,       pns->FindMember("exitButtonPane" ));
        CastTo(m_pbuttonHelp,       pns->FindMember("helpButtonPane" ));
        

        //AddEventTarget(OnButtonGames,       m_pbuttonPlayLan->GetEventSource());
        //AddEventTarget(OnButtonTraining,    m_pbuttonTraining->GetEventSource());
        AddEventTarget(OnButtonTraining,    m_pbuttonTrainingBig->GetEventSource());
        AddEventTarget(OnButtonExit,        m_pbuttonExit->GetEventSource());
        AddEventTarget(OnButtonHelp,        m_pbuttonHelp->GetEventSource());
#ifdef USEAZ
        AddEventTarget(OnButtonZoneClub,    m_pbuttonZoneClub->GetEventSource());
#endif
        AddEventTarget(OnButtonInternet,    m_pbuttonPlayInt->GetEventSource());
        AddEventTarget(OnButtonLAN,         m_pbuttonPlayLan->GetEventSource());
        AddEventTarget(OnButtonZoneWeb,     m_pbuttonZoneWeb->GetEventSource());
        AddEventTarget(OnButtonOptions,     m_pbuttonOptions->GetEventSource());
        AddEventTarget(OnButtonCredits,     m_pbuttonCredits->GetEventSource());
        AddEventTarget(OnButtonIntro,       m_pbuttonIntro->GetEventSource());
        

        AddEventTarget(OnHoverPlayLan,      m_pbuttonPlayLan->GetMouseEnterEventSource());
        AddEventTarget(OnHoverPlayInt,      m_pbuttonPlayInt->GetMouseEnterEventSource());
#ifdef USEAZ
        AddEventTarget(OnHoverZoneClub,     m_pbuttonZoneClub->GetMouseEnterEventSource());
#endif
        //AddEventTarget(OnHoverTrain,        m_pbuttonTraining->GetMouseEnterEventSource());
        AddEventTarget(OnHoverTrain,        m_pbuttonTrainingBig->GetMouseEnterEventSource());
        AddEventTarget(OnHoverZoneWeb,      m_pbuttonZoneWeb->GetMouseEnterEventSource());
        AddEventTarget(OnHoverOptions,       m_pbuttonOptions->GetMouseEnterEventSource());
        AddEventTarget(OnHoverIntro,        m_pbuttonIntro->GetMouseEnterEventSource());
        AddEventTarget(OnHoverCredits,      m_pbuttonCredits->GetMouseEnterEventSource());
        //AddEventTarget(OnHoverQuickstart,   m_pbuttonQuickstart->GetMouseEnterEventSource());
        AddEventTarget(OnHoverExit,         m_pbuttonExit->GetMouseEnterEventSource());
        AddEventTarget(OnHoverHelp,         m_pbuttonHelp->GetMouseEnterEventSource());

        AddEventTarget(OnHoverNone,     m_pbuttonPlayLan->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonPlayInt->GetMouseLeaveEventSource());
#ifdef USEAZ
        AddEventTarget(OnHoverNone,     m_pbuttonZoneClub->GetMouseLeaveEventSource());
#endif
        //AddEventTarget(OnHoverNone,     m_pbuttonTraining->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonTrainingBig->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonZoneWeb->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonOptions->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonIntro->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonCredits->GetMouseLeaveEventSource());
        //AddEventTarget(OnHoverNone,     m_pbuttonQuickstart->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonHelp->GetMouseLeaveEventSource());
        AddEventTarget(OnHoverNone,     m_pbuttonExit->GetMouseLeaveEventSource());

        //m_pbuttonPlayLan->SetEnabled(false);
        //m_pbuttonPlayInt->SetEnabled(false);

        m_pfindServerPopup = new FindServerPopup(pns, this);
          
        pmodeler->UnloadNameSpace(pns);
        
        trekClient.Disconnect();
        trekClient.DisconnectLobby();
        if (g_bQuickstart || g_bReloaded)
            AddEventTarget(OnQuickstart, GetWindow(), 0.01f);

        // we only do this once per execution, and only if training is installed
        static  bool    bHaveVisited = false;
        if (Training::IsInstalled () && !bHaveVisited)
        {
            // check to see if training has been run before
            HKEY    hKey;
            DWORD   dwHasRunTraining = 0;
            DWORD   dwDataSize = sizeof (dwHasRunTraining);
            if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey)) 
            {
                RegQueryValueEx (hKey, "HasTrained", NULL, NULL, (LPBYTE) &dwHasRunTraining, &dwDataSize);
                RegCloseKey(hKey);
            }

            // first time through, show a dialog explaining that users
            // really should do the training first
            if (!dwHasRunTraining)
            {
                TRef<IMessageBox>   pMsgBox = 
                    CreateMessageBox(
                        "ALLEGIANCE is a team-based massively-multiplayer space combat game, in which your goal is to conquer the universe.\n\nTraining is strongly recommended before going online, and will take approximately 30 minutes. Press the OK button to begin training now.",
                        NULL,
                        true,
                        true
                    );
                TRef<Surface> psurface = GetModeler()->LoadSurface("btndontaskmeagainbmp", true);
                TRef<ButtonPane> pDontAskMeAgain
                    = CreateTrekButton(CreateButtonFacePane(psurface, ButtonNormalCheckBox), true, mouseclickSound);
                pDontAskMeAgain->SetOffset(WinPoint(183, 140));
                pMsgBox->GetPane()->InsertAtBottom(pDontAskMeAgain);
                pMsgBox->GetEventSource()->AddSink(new CloseTrainWarnSink(this, pDontAskMeAgain));
                GetWindow()->GetPopupContainer()->OpenPopup (pMsgBox, false);
            }
        }

        if (g_bAskForCDKey && !bHaveVisited && trekClient.GetCDKey().IsEmpty())
        {
            TRef<IPopup> ppopupCDKey = CreateCDKeyPopup();
            GetWindow()->GetPopupContainer()->OpenPopup(ppopupCDKey, false);
        }

        // stop us from asking again during this execution
        bHaveVisited = true;

        trekClient.FlushSessionLostMessage();
    }

    virtual ~IntroScreen() 
    {
        m_pmodeler->UnloadNameSpace("introscreendata");
    }


    bool OnQuickstart()
    {
        OnButtonZoneClub();
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Event handlers
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnPickServer(GUID guidSession)
    {
        assert (!trekClient.LoggedOn());
        m_guidSession = guidSession;

        //
        // pop up the call sign/login dialog
        //

        TRef<IPopup> plogonPopup = CreateLogonPopup(m_pmodeler, this, LogonLAN, 
            "Enter a call sign to use for this game.", trekClient.GetSavedCharacterName(), "", false);
        GetWindow()->GetPopupContainer()->OpenPopup(plogonPopup, false);

        return true;
    }

    bool OnButtonTraining()
    {
        if (Training::IsInstalled ())
        {
            // go train, because we have all the training files installed
            trekClient.SetIsLobbied(false);
            trekClient.SetIsZoneClub(false);
            GetWindow()->screen(ScreenIDTrainScreen);
        }
        else // wlp 2006 - go straight to lobby
        {
            // otherwise give the player the option to download the training files
            // if they do, then go to the download, and restart allegiance
            // otherwise don't do anything
            m_pMsgBox = CreateMessageBox ("TRAINING IS NOT INSTALLED.\n\nClick 'OK' to open the Allegiance website, then follow the directions there to download and install training.", NULL, false);
            TRef<Surface>       psurfaceOKButton = GetModeler ()->LoadImage ("btnokbmp", true)->GetSurface();
            TRef<Surface>       psurfaceAbortButton = GetModeler ()->LoadImage ("btnabortbmp", true)->GetSurface();
            TRef<ButtonPane>    pOKButton = CreateButton ( CreateButtonFacePane(psurfaceOKButton, ButtonNormal, 0, psurfaceOKButton->GetSize().X()), false, 0, 1.0);
            TRef<ButtonPane>    pAbortButton = CreateButton ( CreateButtonFacePane(psurfaceAbortButton, ButtonNormal, 0, psurfaceAbortButton->GetSize().X()), false, 0, 1.0);
            pOKButton->SetOffset (WinPoint(100, 170));
            pAbortButton->SetOffset (WinPoint(300, 170));
            m_pMsgBox->GetPane ()->InsertAtBottom (pOKButton);
            m_pMsgBox->GetPane ()->InsertAtBottom (pAbortButton);
            AddEventTarget(OnButtonBailTraining, pAbortButton->GetEventSource());
            AddEventTarget(OnButtonDownloadTraining, pOKButton->GetEventSource());
            GetWindow()->GetPopupContainer()->OpenPopup (m_pMsgBox, false);
        }
        return true;
    }

    bool    OnButtonBailTraining ()
    {
        GetWindow ()->GetPopupContainer ()->ClosePopup (m_pMsgBox);
        return true;
    }

    bool    OnButtonDownloadTraining ()
    {
        // close the dialog window
        GetWindow ()->GetPopupContainer ()->ClosePopup (m_pMsgBox);

        // start to find the config file
        char    config_file_name[MAX_PATH];

        // get the app build number
        ZVersionInfo vi;
        ZString strBuild(vi.GetFileBuildNumber());

        // start with a default name
        lstrcpy(config_file_name, "Allegiance");
        lstrcat(config_file_name, PCC(strBuild));
        lstrcat(config_file_name, ".cfg");

        // then look to see if there is one in the registry we should use instead
        HKEY hKey;
        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
        {
            DWORD   cbValue = MAX_PATH;
            char    reg_config_file_name[MAX_PATH] = {0};
            ::RegQueryValueEx (hKey, "CfgFile", NULL, NULL, (LPBYTE)&reg_config_file_name, &cbValue);
            // if it didn't succeed, we'll just use the default above
            if (lstrlen (reg_config_file_name) > 0)
              lstrcpy (config_file_name, reg_config_file_name);
        }

        // then construct the full path name of the config file
        PathString  pathEXE(PathString::GetCurrentDirectory());
        PathString  pathConfig(pathEXE + PathString(PathString(config_file_name).GetFilename()));

        // load the config data
        CfgInfo     cfgInfo;
        cfgInfo.Load (pathConfig);
        if (cfgInfo.strTrainingURL)
        {
            // if everything is OK, launch the web page
            GetWindow ()->ShowWebPage (cfgInfo.strTrainingURL);

            // ... and close our application
            GetWindow()->PostMessage(WM_CLOSE);
        }
        else
        {
            // if not, we alert the user...
            TRef<IMessageBox>   pMsgBox = CreateMessageBox ("CAN'T DOWNLOAD TRAINING FILES.\n\nCan't find the download address.", NULL, true);
            GetWindow()->GetPopupContainer()->OpenPopup (pMsgBox, false);
        }
        return true;
    }

    void OnLogon(const ZString& strName, const ZString& strPassword, BOOL fRememberPW)
    {
        // remember the character name for next time
        trekClient.SaveCharacterName(strName);
        m_strCharacterName = strName;
        m_strPassword = "";

        // pop up the Connecting... dialog.   
        GetWindow()->SetWaitCursor();
        TRef<IMessageBox> pmsgBox = CreateMessageBox("Connecting...", NULL, false);
        GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);

        // pause to let the "connecting..." box draw itself
        AddEventTarget(OnTryLogon, GetWindow(), 0.1f);
    }

    bool OnTryLogon()
    {
        trekClient.SetIsLobbied(false);
        trekClient.SetIsZoneClub(false);

        BaseClient::ConnectInfo ci;
        ci.guidSession = m_guidSession;
        strcpy(ci.szName, m_strCharacterName);        
        assert(strlen(m_strPassword) < c_cbGamePassword);

        trekClient.ConnectToServer(ci, NA, Time::Now(), m_strPassword, true);

        return false;
    }

    void OnLogonGameServer()
    {
        // wait for a join message.
        GetWindow()->SetWaitCursor();
        TRef<IMessageBox> pmsgBox = 
            CreateMessageBox("Joining mission....", NULL, false, false);
        GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
    }

    void OnLogonGameServerFailed(bool bRetry, const char* szReason)
    {
        if (bRetry)
        {
            // pop up the call sign/login dialog
            TRef<IPopup> plogonPopup = CreateLogonPopup(m_pmodeler, this, LogonLAN, 
                szReason, trekClient.GetSavedCharacterName(), "", false);
            GetWindow()->GetPopupContainer()->OpenPopup(plogonPopup, false);
        }
        else
        {
            // tell the user why the logon failed
            TRef<IMessageBox> pmsgBox = CreateMessageBox(szReason);
            GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
        }
    }

    void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo)
    {
        if (pMissionInfo && pPlayerInfo == trekClient.MyPlayerInfo())
        {
            // go to the team lobby...
            GetWindow()->screen(ScreenIDTeamScreen);
        }
    }

    void OnDelRequest(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, DelPositionReqReason reason)
    {
        if (pPlayerInfo == trekClient.MyPlayerInfo())
        {
            m_pmissionJoining = pMissionInfo;

            // rejected !!  Bad password?
            if (!GetWindow()->GetPopupContainer()->IsEmpty())
                GetWindow()->GetPopupContainer()->ClosePopup(NULL);
            GetWindow()->RestoreCursor();

            if (reason == DPR_BadPassword)
            {
                TRef<IPopup> ppopupPassword = CreatePasswordPopup(this, m_strPassword);
                GetWindow()->GetPopupContainer()->OpenPopup(ppopupPassword, false);
            }
            else if (reason == DPR_NoMission)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("The mission you were trying to join has ended.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_LobbyLocked)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("The lobby for this mission is locked.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_OutOfLives)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("You have run out of lives in this mission.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_Banned)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("You have been banned from this mission.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_GameFull)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("This server has reached its maximum number of players.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_PrivateGame)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("The game you tried to join is private.  Your login id does not have access to it.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_DuplicateLogin)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("Someone else is already using that name in this game.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else if (reason == DPR_ServerPaused)
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("The server for this game is shutting down and is not accepting new users.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
            else
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("You have not been accepted into the game.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
                trekClient.Disconnect();
            }
        }
    }

    virtual void OnCancelPassword()
    {
        trekClient.Disconnect();
    }

    virtual void OnPassword(ZString strPassword)
    {
        // try the new password
        m_strPassword = strPassword;

        // pop up the Connecting... dialog.   
        GetWindow()->SetWaitCursor();
        TRef<IMessageBox> pmsgBox = CreateMessageBox("Connecting...", NULL, false);
        GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);

        // pause to let the "connecting..." box draw itself
        AddEventTarget(OnTryLogon, GetWindow(), 0.1f);
    }

    void OnAbort()
    {
        // user aborted logging on; we don't need to do anything
    };

    bool OnButtonExit()
    {
        GetWindow()->StartClose();

        return true;
    }

    bool OnButtonHelp()
    {
        GetWindow()->OnHelp (true);
        return true;
    }

    bool OnButtonZoneClub()
    {
        if (CheckNetworkDrivers())
        {
            trekClient.SetIsZoneClub(true);
            GetWindow()->screen(ScreenIDZoneClubScreen);
        }
        return true;
    }

    bool OnButtonInternet()
    {
        if (CheckNetworkDrivers())
        {
            trekClient.SetIsZoneClub(false);
            GetWindow()->screen(ScreenIDZoneClubScreen);
        }
        return true;
    }

    bool OnButtonLAN()
    {
        if (CheckNetworkDrivers())
        {
            GetWindow()->GetPopupContainer()->OpenPopup(m_pfindServerPopup, false);
        }
        return true;
    }

    bool OnButtonZoneWeb()
    {
        GetWindow()->ShowWebPage();
        return true;
    }

    bool OnButtonOptions()
    {
        GetWindow()->ShowOptionsMenu();
        return true;
    }

    bool OnButtonCredits()
    {
        TRef<INameSpace> pnsCredits = GetModeler()->GetNameSpace("creditspane");
        m_pcreditsPopup = new CreditsPopup(pnsCredits, this, GetWindow()->GetTime());
        GetWindow()->GetPopupContainer()->OpenPopup(m_pcreditsPopup, false);
        return true;
    }

    bool OnButtonIntro()
    {
        GetWindow()->screen(ScreenIDSplashScreen);
        return true;
    }

    bool OnHoverPlayLan()
    {
        m_pnumberCurrentHover->SetValue(hoverPlayLan);
        return true;
    }

    bool OnHoverPlayInt()
    {
        m_pnumberCurrentHover->SetValue(hoverPlayInt);
        return true;
    }

#ifdef USEAZ
    bool OnHoverZoneClub()
    {
        m_pnumberCurrentHover->SetValue(hoverZoneClub);
        return true;
    }
#endif

    bool OnHoverTrain()
    {
        m_pnumberCurrentHover->SetValue(hoverTrain);
        return true;
    }

    bool OnHoverZoneWeb()
    {
        m_pnumberCurrentHover->SetValue(hoverZoneWeb);
        return true;
    }

    bool OnHoverOptions()
    {
        m_pnumberCurrentHover->SetValue(hoverOptions);
        return true;
    }

    bool OnHoverIntro()
    {
        m_pnumberCurrentHover->SetValue(hoverIntro);
        return true;
    }

    bool OnHoverCredits()
    {
        m_pnumberCurrentHover->SetValue(hoverCredits);
        return true;
    }

    bool OnHoverQuickstart()
    {
        m_pnumberCurrentHover->SetValue(hoverQuickstart);
        return true;
    }

    bool OnHoverHelp()
    {
        m_pnumberCurrentHover->SetValue(hoverHelp);
        return true;
    }

    bool OnHoverExit()
    {
        m_pnumberCurrentHover->SetValue(hoverExit);
        return true;
    }
    
    bool OnHoverNone()
    {
        m_pnumberCurrentHover->SetValue(hoverNone);
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateIntroScreen(Modeler* pmodeler)
{
    return new IntroScreen(pmodeler);
}


