#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// GameStarting Screen
//
//////////////////////////////////////////////////////////////////////////////

class GameStartingScreen :
    public Screen,
    public EventTargetContainer<GameStartingScreen>
{
private:
    TRef<Pane>       m_ppane;
    TRef<ButtonPane> m_pbuttonQuit;

public:
    GameStartingScreen(Modeler* pmodeler)
    {
        TRef<INameSpace> pns = pmodeler->GetNameSpace("gameStartingscreen");
        CastTo(m_ppane, pns->FindMember("screen"));

        CastTo(m_pbuttonQuit, pns->FindMember("quitButtonPane"));
		// mdvalley: Just one pointer/class this file.
        AddEventTarget(&GameStartingScreen::OnButtonQuit, m_pbuttonQuit->GetEventSource());
        pmodeler->UnloadNameSpace(pns);
    }

    bool OnButtonQuit()
    {
        GetWindow()->screen(ScreenIDTeamScreen);
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

TRef<Screen> CreateGameStartingScreen(Modeler* pmodeler)
{
    return new GameStartingScreen(pmodeler);
}
