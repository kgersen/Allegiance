#include "pch.h"
#include "Training.h"

// AGC and AllSrv Includes -- Imago remove 6/21/09, they were for TM7
#include <agc.h>
#include <AllSrvModuleIDL.h>

#include "AdminSessionSecure.h"
#include "AdminSessionSecureHost.h"

//////////////////////////////////////////////////////////////////////////////
//
// Training Screen
//
//////////////////////////////////////////////////////////////////////////////

TRef<ModifiableNumber>  g_pnumberMissionNumber;

class TrainingScreen :
    public Screen, 
    public EventTargetContainer<TrainingScreen>,
    public TrekClientEventSink,
    public IItemEvent::Sink
{
private:
    TRef<Pane>                          m_ppane;
    TRef<ButtonPane>                    m_pbuttonBack;
    TRef<ButtonPane>                    m_pbuttonTrainMission1;
    TRef<ButtonPane>                    m_pbuttonTrainMission2;
    TRef<ButtonPane>                    m_pbuttonTrainMission3;
    TRef<ButtonPane>                    m_pbuttonTrainMission4;
    TRef<ButtonPane>                    m_pbuttonTrainMission5;
    TRef<ButtonPane>                    m_pbuttonTrainMission6;
    TRef<ButtonPane>                    m_pbuttonTrainMission7;
	TRef<ButtonPane>                    m_pbuttonTrainMission8; //TheBored 06-JUL-07: nanite mission
    TRef<ButtonPane>                    m_pbuttonTrain;

	// Imago remove 6/22/09 (for Live TM7)
    static  IAdminGamesPtr              m_spAdminGamesPtr;
    static  IAdminGamePtr               m_spAdminGamePtr;

    static  int                         m_iMissionNext;

public:
    int                                 m_civID;
// Imago remove 6/2/09 (for Live TM7)
    class TrainLiveDialogPopup : 
        public IPopup, 
        public EventTargetContainer<TrainLiveDialogPopup>,
        public TrekClientEventSink
    {
    private:
        TRef<Pane>              m_ppane;
        TRef<ButtonPane>        m_pbuttonOk;
        TRef<ButtonPane>        m_pbuttonCancel;
        TRef<ButtonPane>        m_pbuttonCancel2;
        TRef<ComboPane>         m_pcomboCiv;

        TrainingScreen*         m_pparent;

    public:

        TrainLiveDialogPopup (TRef<INameSpace> pns, TrainingScreen* pparent)
        {
            m_pparent = pparent;

            // find the dialog stuff in the namespace
            CastTo (m_ppane,            pns->FindMember("TrainLiveDialog"));
            CastTo (m_pbuttonOk,        pns->FindMember("okButtonPane"));
            CastTo (m_pbuttonCancel,    pns->FindMember("cancelButtonPane"));
            CastTo (m_pbuttonCancel2,   pns->FindMember("cancelButtonPane2"));
            CastTo (m_pcomboCiv,        pns->FindMember("civComboPane"));

            // handle the ok and cancel buttons
			// mdvalley: Pointers, Names.
            AddEventTarget(&TrainingScreen::TrainLiveDialogPopup::OnButtonOk, m_pbuttonOk->GetEventSource());
            AddEventTarget(&TrainingScreen::TrainLiveDialogPopup::OnButtonCancel, m_pbuttonCancel->GetEventSource());
            AddEventTarget(&TrainingScreen::TrainLiveDialogPopup::OnButtonCancel, m_pbuttonCancel2->GetEventSource());

            // fill in the combo pane of civs
            m_pcomboCiv->AddItem("Iron Coalition", 25);
            m_pcomboCiv->AddItem("Bios", 35);
            m_pcomboCiv->AddItem("Gigacorp", 18);
            //m_pcomboCiv->AddItem("Belters", 101);
        }

        virtual void OnClose()
        {
            IPopup::OnClose();
            debugf("train live popup closed\n");
        }

        Pane* GetPane()
        {
            return m_ppane;
        }

        void CloseThePopup (void)
        {
            if (m_ppopupOwner) {
                m_ppopupOwner->ClosePopup(this);
            } else if (m_pcontainer) {
                m_pcontainer->ClosePopup(this);
            }
            GetWindow()->RestoreCursor();
        }
        
        bool OnButtonOk()
        {
            CloseThePopup ();

            // now 
            m_pparent->m_civID = m_pcomboCiv->GetSelection ();
            m_pparent->DoTrainLive ();
            return true;
        }

        bool OnButtonCancel()
        {
            CloseThePopup ();

            return true;
        }
    };
    
    TRef<TrainLiveDialogPopup>          m_pTrainLivePopup;


public:
    TrainingScreen(Modeler* pmodeler)
    {
        // terminate existing training game if there was one
        KillStandaloneGame ();  //no more standalone training game Imago 6/22/09

        //
        // exports
        //

        TRef<INameSpace> pnsTrainingData = pmodeler->CreateNameSpace("trainingdata", pmodeler->GetNameSpace("gamepanes"));

        pnsTrainingData->AddMember("missionNumber", g_pnumberMissionNumber = new ModifiableNumber(m_iMissionNext));

        // Load the members from MDL

        TRef<INameSpace> pns = pmodeler->GetNameSpace("TrainingScreen");

        CastTo(m_ppane,                     pns->FindMember("screen"                    ));
        CastTo(m_pbuttonBack,               pns->FindMember("backButtonPane"            ));
        CastTo(m_pbuttonTrainMission1,      pns->FindMember("trainMission1ButtonPane"   ));
        CastTo(m_pbuttonTrainMission2,      pns->FindMember("trainMission2ButtonPane"   ));
        CastTo(m_pbuttonTrainMission3,      pns->FindMember("trainMission3ButtonPane"   ));
        CastTo(m_pbuttonTrainMission4,      pns->FindMember("trainMission4ButtonPane"   ));
        CastTo(m_pbuttonTrainMission5,      pns->FindMember("trainMission5ButtonPane"   ));
        CastTo(m_pbuttonTrainMission6,      pns->FindMember("trainMission6ButtonPane"   ));
        CastTo(m_pbuttonTrainMission7,      pns->FindMember("trainMission7ButtonPane"   ));
		CastTo(m_pbuttonTrainMission8,      pns->FindMember("trainMission8ButtonPane"   )); //TheBored 06-JUL-07: nanite mission
        CastTo(m_pbuttonTrain,              pns->FindMember("trainButtonPane"           ));

        m_pTrainLivePopup = new TrainLiveDialogPopup(pns, this); //imago 6/22/09

        pmodeler->UnloadNameSpace("TrainingScreen");

        //
        // buttons
        //

		// mdvalley: Pointers, Classes, and bears, oh my!
        AddEventTarget(&TrainingScreen::OnButtonBack, m_pbuttonBack->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrain->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission1, m_pbuttonTrainMission1->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission2, m_pbuttonTrainMission2->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission3, m_pbuttonTrainMission3->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission4, m_pbuttonTrainMission4->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission5, m_pbuttonTrainMission5->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission6, m_pbuttonTrainMission6->GetEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrainMission7, m_pbuttonTrainMission7->GetEventSource());
		AddEventTarget(&TrainingScreen::OnButtonTrainMission8, m_pbuttonTrainMission8->GetEventSource()); //TheBored 06-JUL-07: nanite mission

        //
        // buttons
        //

        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission1->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission2->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission3->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission4->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission5->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission6->GetDoubleClickEventSource());
        AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission7->GetDoubleClickEventSource());
		AddEventTarget(&TrainingScreen::OnButtonTrain, m_pbuttonTrainMission8->GetDoubleClickEventSource()); //TheBored 06-JUL-07: nanite mission

        // hilite the current mission
        switch (m_iMissionNext)
        {
            // here we will determine which button to hilight
            case Training::c_TM_1_Introduction:
                m_pbuttonTrainMission1->SetChecked (true);
                break;
            case Training::c_TM_2_Basic_Flight:
                m_pbuttonTrainMission2->SetChecked (true);
                break;
            case Training::c_TM_3_Basic_Weaponry:
                m_pbuttonTrainMission3->SetChecked (true);
                break;
            case Training::c_TM_4_Enemy_Engagement:
                m_pbuttonTrainMission4->SetChecked (true);
                break;
            case Training::c_TM_5_Command_View:
                m_pbuttonTrainMission5->SetChecked (true);
                break;
            case Training::c_TM_6_Practice_Arena:
                m_pbuttonTrainMission6->SetChecked (true);
                break;
			//TheBored 06-JUL-07: nanite mission
			case Training::c_TM_8_Nanite:
                m_pbuttonTrainMission8->SetChecked (true);
                break;
			//End TB
            case Training::c_TM_7_Live:
            default:
                m_pbuttonTrainMission7->SetChecked (true);
                break;
        }
        m_pbuttonTrain->SetChecked (true);
#ifdef _DEBUG
		if (bStartTraining) {
			g_pnumberMissionNumber->SetValue(6);
			OnButtonTrain();
		}
#endif
    }

    ~TrainingScreen()
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // UI Events
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnEvent(IItemEvent::Source *pevent, ItemID pitem)
    {
        return true;
    }

    bool OnButtonBack()
    {
        GetWindow()->screen(ScreenIDIntroScreen);
        return false;
    }

    bool OnButtonTrain()
    {
        // here we mark the fact that a training mission has been launched
        HKEY    hKey;
        DWORD   dwHasRunTraining = 1;
        if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_WRITE, &hKey)) 
        {
            RegSetValueEx (hKey, "HasTrained", NULL, REG_DWORD, (const BYTE*) &dwHasRunTraining, sizeof (dwHasRunTraining));
            RegCloseKey (hKey);
        }

        // check to see which mission this is, and advance to the next mission
        int iMission = static_cast<int> (g_pnumberMissionNumber->GetValue ());
        if (iMission == m_iMissionNext)
        {
            m_iMissionNext++;
            if (m_iMissionNext > Training::c_TM_8_Nanite) //TB: Pretty sure this just doesnt let you go past the last mish, in this case that is the new one.
                m_iMissionNext = Training::c_TM_8_Nanite;
        }

        // check to see which mission this is
        switch (iMission)
        {
            case Training::c_TM_1_Introduction:
                GetWindow ()->screen (ScreenIDPostTrainSlideshow);
                break;
            case Training::c_TM_2_Basic_Flight:
            case Training::c_TM_3_Basic_Weaponry:
            case Training::c_TM_4_Enemy_Engagement:
            case Training::c_TM_5_Command_View:
            case Training::c_TM_6_Practice_Arena:
			case Training::c_TM_8_Nanite: //TheBored 06-JUL-07: nanite mission
                // note that the training slideshow can get the mission number from the global
                // value when subsequently launching.
                GetWindow ()->screen (ScreenIDTrainSlideshow);
                break;
            case Training::c_TM_7_Live:
                TrainLive ();  // imago remove 6/22/09
                break;
        }
        return true;
    }
	//TheBored 06-JUL-07: nanite mission (Each OnButtonTrainMission#() was edited to add #8)
    bool OnButtonTrainMission1()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_1_Introduction);
        m_pbuttonTrainMission1->SetChecked (true);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission2()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_2_Basic_Flight);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (true);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission3()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_3_Basic_Weaponry);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (true);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission4()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_4_Enemy_Engagement);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (true);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission5()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_5_Command_View);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (true);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission6()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_6_Practice_Arena);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (true);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }

    bool OnButtonTrainMission7()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_7_Live);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (true);
		m_pbuttonTrainMission8->SetChecked (false);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }
	//TheBored 06-JUL-07: #8 added
	bool OnButtonTrainMission8()
    {
        g_pnumberMissionNumber->SetValue (Training::c_TM_8_Nanite);
        m_pbuttonTrainMission1->SetChecked (false);
        m_pbuttonTrainMission2->SetChecked (false);
        m_pbuttonTrainMission3->SetChecked (false);
        m_pbuttonTrainMission4->SetChecked (false);
        m_pbuttonTrainMission5->SetChecked (false);
        m_pbuttonTrainMission6->SetChecked (false);
        m_pbuttonTrainMission7->SetChecked (false);
		m_pbuttonTrainMission8->SetChecked (true);
        m_pbuttonTrain->SetEnabled (true);
        m_pbuttonTrain->SetChecked (true);
        return true;
    }
	//End TB

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }

	/* Imago remved - we're not using it anymore and it's breaking the new vs express edition (gotta pay up for atl/mfc) */
    //////////////////////////////////////////////////////////////////////////////
    //
    // Game Creation Methods
    //
    //////////////////////////////////////////////////////////////////////////////
    #define TRAINING_MISSION_7_GAME_NAME    "Training Mission 7"

    //////////////////////////////////////////////////////////////////////////////
    static
    bool    Error (HRESULT hr)
    {
        if (FAILED (hr))
        {
            // get the error codes
            IErrorInfoPtr spErr;
            ::GetErrorInfo(0, &spErr);

            // convert that to an english message
            _com_error e (hr, spErr, true);
            _bstr_t strError (e.Description().length() ? e.Description() : _bstr_t (e.ErrorMessage()));

            // debug just bail so I can trace the problem
            // assert (0);

            // put up a dialog about the error
            TRef<IMessageBox>   pMsgBox = CreateMessageBox (ZString ("Unable to create a game. The home server may not be properly installed. Install the home server from your Allegiance CD.\n\nReason for failure: ") + (LPCTSTR)strError, NULL, true);
            GetWindow()->GetPopupContainer()->OpenPopup (pMsgBox, false);
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    void    TrainLive (void)
    {
        // Make sure that they have entered a CD Key, since the server will boot 
        // them if they have not.  
        if (g_bAskForCDKey && trekClient.GetCDKey ().IsEmpty () )
        {
            TRef<IPopup> ppopupCDKey = CreateCDKeyPopup ();
            GetWindow ()->GetPopupContainer ()->OpenPopup (ppopupCDKey, false);
        }
        else
        {
            // open the TrainLiveDialogPopup
            GetWindow ()->GetPopupContainer ()->OpenPopup (m_pTrainLivePopup, false);
            // this then goes to DoTrainLive if the user clicks OK
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    bool    DoTrainLive (void)
    {
        // pop up the "creating game..." dialog.   
        GetWindow()->SetWaitCursor ();
        TRef<IMessageBox> pMsgBox = CreateMessageBox ("Creating game...", NULL, false);
        GetWindow ()->GetPopupContainer ()->OpenPopup (pMsgBox, false);

        // pause to let the "creating game..." box draw itself
		// mdvalley: Pointer, name, repeat.
        AddEventTarget(&TrainingScreen::OnTrainLive, GetWindow(), 0.1f);

        // this then goes to OnTrainLive

        // return false, I don't know why
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    bool OnTrainLive (void)
    {
        // close the "creating game..." popup (it won't really close until after we get through this function)
        while (!GetWindow()->GetPopupContainer()->IsEmpty())
            GetWindow()->GetPopupContainer()->ClosePopup(NULL);
        GetWindow()->RestoreCursor();

        // create the game, and logon to it if successful
        CreateStandaloneGame ();

        // return false, I don't know why
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    void    CheckForExistingGames (void)
    {
        // try to get a game pointer from the games list
        m_spAdminGamesPtr->get_Item (& (_variant_t) (0L), &m_spAdminGamePtr);

        // if we got a valid game
        if (m_spAdminGamePtr)
        {
            // put up a message about the existing game
            TRef<IMessageBox> pMsgBox = CreateMessageBox ("The training mission game could not be created because there is already a server running on this machine with an active game.");
            GetWindow ()->GetPopupContainer ()->OpenPopup (pMsgBox, false);

            // and we terminate the entire login process here
            m_spAdminGamePtr = 0;
            m_spAdminGamesPtr = 0;
        }
        else
        {
            // create the game we want
            CreateTrainingMissionGame ();
        }
    }
	
    //////////////////////////////////////////////////////////////////////////////
    
    bool    TerminateExistingGames (void)
    {
        // kill every game that exists
        while (m_spAdminGamePtr)
        {
            m_spAdminGamePtr->Kill ();
            m_spAdminGamesPtr->get_Item (& (_variant_t) (0L), &m_spAdminGamePtr);
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    HRESULT CreateTrainingMissionGame (void)
    {
        HRESULT                 hr;

        assert (m_spAdminGamesPtr);

        // Create a GameParameters object
        IAGCGameParametersPtr spParams;
        if (!Error (hr = spParams.CreateInstance (CLSID_AGCGameParameters)))
        {
            // set up game parameters associated with the training game
            spParams->put_MinPlayers (1);
            spParams->put_TotalMaxPlayers (1);
            spParams->put_CivIDs (0, m_civID);  // default is iron league, but user may pick a different one
            spParams->put_CivIDs (1, 35);       // bios
            spParams->put_GameName (_bstr_t (TRAINING_MISSION_7_GAME_NAME));
            spParams->put_LockTeamSettings (true);

            // Create a new game
            if (!Error (hr = m_spAdminGamesPtr->Add (spParams)))
            {
                // get the game
                if (!Error (hr = m_spAdminGamesPtr->get_Item (& (_variant_t) (0L), &m_spAdminGamePtr)))
                {
                    // set up the teams
                    m_spAdminGamePtr->SetTeamName (0, _bstr_t ("Training Academy"));
                    m_spAdminGamePtr->SetTeamName (1, _bstr_t ("Bios"));

                    // pop up the Connecting... dialog.   
                    GetWindow()->SetWaitCursor ();
                    TRef<IMessageBox> pMsgBox = CreateMessageBox ("Connecting...", NULL, false);
                    GetWindow ()->GetPopupContainer ()->OpenPopup (pMsgBox, false);

                    // pause to let the "connecting..." box draw itself
					// mdvalley: repeat
                    AddEventTarget(&TrainingScreen::OnTryLogon, GetWindow(), 0.1f);

                    // this then goes to OnTryLogon
                }
            }
        }

        // terminate the games admin pointer
        m_spAdminGamesPtr = 0;

        return hr;
    }

    //////////////////////////////////////////////////////////////////////////////
    HRESULT CreateStandaloneGame (void)
    {
        HRESULT                 hr;

        // Create the AdminSession class object
        IAdminSessionClassPtr   spClass;
        if (!Error (hr = CoGetClassObject (__uuidof(AdminSession), CLSCTX_LOCAL_SERVER, NULL, __uuidof(spClass), (void**)&spClass)))
        {
            // Create an instance of the host object for an IAdminSessionHost interface
            AdminSessionSecureHost    xHost;

            // Create an AdminSession object
            IAdminSessionPtr spSession;
            hr = spClass->CreateSession (&xHost, &spSession);
            ::CoDisconnectObject(&xHost, 0);
            if (!Error (hr))
            {
                // Get the Server property from the session object
                IAdminServerPtr spServer;
                if (!Error (hr = spSession->get_Server (&spServer)))
                {
                    // Get the Games property from the server object
                    if (!Error (hr = spServer->get_Games (&m_spAdminGamesPtr)))
                    {
                        // check to see if there are existing games
                        // and proceed only if there aren't
                        CheckForExistingGames ();
                    }
                }
            }
        }
        return hr;
    }

    //////////////////////////////////////////////////////////////////////////////
    static
    HRESULT KillStandaloneGame (void)
    {
        HRESULT hr = S_OK;
        m_spAdminGamesPtr = 0;
        if (m_spAdminGamePtr)
        {
            if (CoIsHandlerConnected (m_spAdminGamePtr))
                hr = m_spAdminGamePtr->Kill ();
            m_spAdminGamePtr = 0;
        }
        return hr;
    }

    //////////////////////////////////////////////////////////////////////////////
    bool OnTryLogon (void)
    {
        TList<TRef<LANServerInfo> > listResults;

        // try ten times
        int iTryCount = 10;
        while (iTryCount-- && (listResults.GetCount () == 0))
        {
            // initiate a query for the local server
            trekClient.FindStandaloneServersByName ("127.0.0.1", listResults); //Imago REVIEW this hardly works now-a-days 6/10

            // now, for 6 seconds, check to see if we found it every half second
            for (int i = 0; (i < 12) && (listResults.GetCount () == 0); i++)
            {
                Sleep (500);
                trekClient.FindStandaloneServersByName (NULL, listResults);
            }
        }

        // if we found a server...
        if (listResults.GetCount () == 1)
        {
            // there should be only one game on the server
            TList<TRef<LANServerInfo> >::Iterator iterNew(listResults);
            if (iterNew.Value ()->strGameName == TRAINING_MISSION_7_GAME_NAME)
            {
                // set ourselves to not be in the lobby or the zone
                trekClient.SetIsLobbied(false);
                trekClient.SetIsZoneClub(false);

                // set up the connection info
                BaseClient::ConnectInfo     ci;
                ci.guidSession = iterNew.Value ()->guidSession;
                strcpy (ci.szName, "Cadet");

                // we try five times
                iTryCount = 5;
                while (iTryCount-- && !trekClient.m_fm.IsConnected ())
                {
                    // close any popups
                    while (!GetWindow()->GetPopupContainer()->IsEmpty())
                        GetWindow()->GetPopupContainer()->ClosePopup(NULL);
                    GetWindow()->RestoreCursor();
                    
                    // connect to the server
                    trekClient.ConnectToServer (ci, NA, Time::Now(), "", true);
                }
            }
        }

        // if we didn't get connected
        if (!trekClient.m_fm.IsConnected ())
        {
            // close any popups
            while (!GetWindow()->GetPopupContainer()->IsEmpty())
                GetWindow()->GetPopupContainer()->ClosePopup(NULL);
            GetWindow()->RestoreCursor();

            // give some indication of what happened
            TRef<IMessageBox> pmsgBox = CreateMessageBox("Connection to standalone server failed.");
            GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);

            // terminate the game so they can try again
            KillStandaloneGame ();

            // report the number of servers found as a diagnostic
            #ifdef _DEBUG
            char    szBuffer[256];
            sprintf (szBuffer, "Found %d servers.\n", listResults.GetCount ());
            debugf (szBuffer);
            #endif
        }
        else
        {
            // tell training that this is the live mission
            Training::StartMission (Training::c_TM_7_Live);
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    void OnLogonGameServer()
    {
        // wait for a join message.
        GetWindow()->SetWaitCursor();
        TRef<IMessageBox> pmsgBox = 
            CreateMessageBox("Joining mission....", NULL, false, false);
        GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
    }

    //////////////////////////////////////////////////////////////////////////////
    void OnLogonGameServerFailed(bool bRetry, const char* szReason)
    {
        // tell the user why the logon failed
        TRef<IMessageBox> pmsgBox = CreateMessageBox(szReason);
        GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
    }

    //////////////////////////////////////////////////////////////////////////////
    void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo)
    {
        if ((Training::GetTrainingMissionID () == Training::c_TM_7_Live) && pMissionInfo && (pPlayerInfo == trekClient.MyPlayerInfo()))
        {
            debugf ("Player was added, waiting for the game to start\n");

            // start the game
            Error (m_spAdminGamePtr->StartGame ());

            // next will be OnEnterMission if this succeeds
        }
    }

    //////////////////////////////////////////////////////////////////////////////

    void OnEnterMission (void)
    {
        debugf ("Game has started!\n");

        // set the cluster
        GetWindow ()->SetCluster (trekClient.GetShip ()->GetCluster ());

        // switch to the combat screen
        GetWindow ()->screen (ScreenIDCombat);

        // set the station so we don't launch in a lifepod
        IstationIGC*    pstation = trekClient.GetShip ()->GetStation();
        if (pstation)
            trekClient.ReplaceLoadout (pstation);
    }

    //////////////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////////
//
// static initializer
//
//////////////////////////////////////////////////////////////////////////////
int             TrainingScreen::m_iMissionNext = Training::c_TM_1_Introduction;
//imago removed live training mission 6/22/09
IAdminGamesPtr  TrainingScreen::m_spAdminGamesPtr = 0;
IAdminGamePtr   TrainingScreen::m_spAdminGamePtr = 0;

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateTrainingScreen(Modeler* pmodeler)
{
    return new TrainingScreen(pmodeler);
}

// destructor
 //imago removed 6/22/09 - unremoved 7/10
void    KillTrainingStandaloneGame (void)
{
    TrainingScreen::KillStandaloneGame ();
}



