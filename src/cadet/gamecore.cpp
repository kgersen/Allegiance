//////////////////////////////////////////////////////////////////////////////
//
// GameCore
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "main.h"
#include "regkey.h"

TRef<EffectWindow> g_pwindow;

char g_szArtReg[] = ALLEGIANCE_REGISTRY_KEY_ROOT;

//////////////////////////////////////////////////////////////////////////////
//
// GameWindowImpl
//
//////////////////////////////////////////////////////////////////////////////

class GameWindow : public EffectWindow
{
protected:
    TRef<GameContext> m_pgameContext;
    Time m_timeLastFrame;
    
public:
    GameWindow(EffectApp* papp, const ZString& strCommandLine) :
        EffectWindow(papp, strCommandLine, "",
        #ifdef DREAMCAST
            true, 
        #else
            false,
        #endif
            WinRect(0, 0, 640, 480),
            WinPoint(640, 480),
            NULL),
        m_timeLastFrame(Time::Now())
    {
        g_pwindow = this;

        SetUI(false);
        //SetFullscreen(true);
        SetShowFPS(true);
        Joystick::Initialize(GetModuleHandle(NULL),GetHWND());
        Sound::Initialize(GetHWND());

        #ifdef DREAMCAST
            GetModeler()->SetArtPath("\\cd-rom\\");
            //GetModeler()->SetArtPath("\\pc\\cadet\\");
        #else
            {
            PathString pathStr; // = ZString::GetProfileString("Federation", "ArtPath");

            HKEY hKey;
            DWORD dwType;
            char  szValue[MAX_PATH];
            DWORD cbValue = MAX_PATH;
            extern char g_szArtReg[];

            if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER, g_szArtReg, 0, KEY_READ, &hKey))
                {
                // Get the art path from the registry
                if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ArtPath", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
                    pathStr = szValue;
                ::RegCloseKey(hKey);
                }

            if (pathStr.IsEmpty()) 
                {
                char    path[MAX_PATH + 16];
                GetModuleFileName(NULL, path, MAX_PATH);
                char*   p = strrchr(path, '\\');
                if (!p)
                    p = path;
                else
                    p++;
                strcpy(p, "artwork");
                pathStr = path;
                }

            GetModeler()->SetArtPath(pathStr);
            }
        #endif

        m_pgameContext = GameContext::Create();
        m_pgameContext->Initialize();
        SetImage(m_pgameContext->GetImage());
    }

    ~GameWindow()
    {
    }

    bool IsValid()
    {
        return EngineWindow::IsValid();
    }

    void OnClose()
    {
        m_pgameContext->Terminate();
        m_pgameContext = NULL;
        EngineWindow::OnClose();
    }

    void EvaluateFrame(Time time)
    {
        float dtime = (float)(time - m_timeLastFrame);
        m_pgameContext->Update(time, dtime);
        m_timeLastFrame = time;
    }                                                       


};


//////////////////////////////////////////////////////////////////////////////
//
// GameApp
//
//////////////////////////////////////////////////////////////////////////////


class GameApp: public EffectApp {
protected:

public:
    HRESULT Initialize(const ZString& strCommandLine)
    {
        EffectApp::Initialize(strCommandLine);
        TRef<GameWindow> pwindow = new GameWindow(this, strCommandLine);
        ZAssert(pwindow == g_pwindow);
		return S_OK;
    }

    void Terminate()
    {
        g_pwindow = NULL;
        EffectApp::Terminate();
   }
} g_app;


//////////////////////////////////////////////////////////////////////////////
//
// Globals
//
//////////////////////////////////////////////////////////////////////////////

TRef<Engine> GetEngine()
{
    return g_pwindow->GetEngine();
}

TRef<Modeler> GetModeler()
{
    return g_pwindow->GetModeler();
}

TRef<EffectWindow> GetWindow()
{
    return g_pwindow;
}

