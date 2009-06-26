#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// MenuCommandSink
//
//////////////////////////////////////////////////////////////////////////////

void EngineWindow::MenuCommandSink::OnMenuCommand(IMenuItem* pitem)
{
    m_pwindow->OnEngineWindowMenuCommand(pitem);
}

//////////////////////////////////////////////////////////////////////////////
//
// Static members
//
//////////////////////////////////////////////////////////////////////////////

EngineWindow::ModeData EngineWindow::s_pmodes[] =
    {
        ModeData(WinPoint(320, 200), false),
        ModeData(WinPoint(480, 360), false)
        //ModeData(WinPoint(320, 200), true),
        //ModeData(WinPoint(480, 360), true)
    };

int EngineWindow::s_countModes = 2;

//////////////////////////////////////////////////////////////////////////////
//
// Keyboard Input Filter
//
//////////////////////////////////////////////////////////////////////////////

class EngineWindowKeyboardInput : public IKeyboardInput {
private:
    EngineWindow* m_pwindow;

public:
    EngineWindowKeyboardInput(EngineWindow* pwindow) :
        m_pwindow(pwindow)
    {
    }

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown && ks.bAlt) {
            switch(ks.vk) {
                case VK_F4:
                    m_pwindow->StartClose();
                    return true;

                case VK_RETURN:
                    m_pwindow->SetFullscreen(!m_pwindow->GetFullscreen());
                    return true;

                case 'F':
                    m_pwindow->ToggleShowFPS();
                    return true;

                #ifdef ICAP
                    case 'P':
                        m_pwindow->ToggleProfiling(-1);
                        return true;

                    case 'O':
                        m_pwindow->ToggleProfiling(1);
                        return true;
                #endif

                #ifdef _DEBUG
                    case VK_F9:
                        if (ks.bShift) {
                            ZError("Forced Assert");
                        }
                        return true;

                    case VK_F10:
                        if (ks.bShift) {
                            *(DWORD*)NULL = 0;
                        }
                        return true;
                #endif
            }
        }

        return false;
    }

    bool OnChar(IInputProvider* pprovider, const KeyState& ks)
    {
        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

EngineWindow::EngineWindow(	EngineApp *			papp,
							const ZString&		strCommandLine,
							const ZString&		strTitle,
							bool				bStartFullscreen,
							const WinRect&		rect,
							const WinPoint&		sizeMin,
							HMENU				hmenu ) :
				Window(NULL, rect, strTitle, ZString(), 0, hmenu),
				m_pengine(papp->GetEngine()),
				m_pmodeler(papp->GetModeler()),
				m_sizeWindowed(rect.Size()),
				m_offsetWindowed(rect.Min()),
				m_bSizeable(true),
				m_bMinimized(false),
				m_bMovingWindow(false),
				m_pimageCursor(Image::GetEmpty()),
				m_bHideCursor(false),
				m_bCaptured(false),
				m_bHit(false),
				m_bInvalid(true),
				m_bActive(true),
				m_bShowCursor(true),
				m_bMouseEnabled(true),
				m_bRestore(false),
				m_bMouseInside(false),
				m_bMoveOnHide(true),
				m_bWindowStateMinimised(false),
				m_bWindowStateRestored(false),
				m_pEngineApp(papp)
{
    //
    // Button Event Sink
    //

    m_pbuttonEventSink = new ButtonEvent::Delegate(this);

    //
    // mouse position
    //

    m_ppointMouse = new ModifiablePointValue(Point(0, 0));

    // Create the input engine
    //

    m_pinputEngine = CreateInputEngine(GetHWND());
    m_pinputEngine->SetFocus(true);

    //
    // Should we start fullscreen?
	CD3DDevice9 * pDev = CD3DDevice9::Get();
	m_bStartFullScreen = pDev->GetDeviceSetupParams()->bRunWindowed ? false : true;
    ParseCommandLine( strCommandLine, m_bStartFullScreen );

    // Get the mouse
    //

    m_pmouse = m_pinputEngine->GetMouse();
    m_pmouse->SetEnabled(m_bStartFullScreen);
    papp->SetMouse(m_pmouse);

    m_pmouse->GetEventSource()->AddSink(m_peventSink = new ButtonEvent::Delegate(this));

    //
    // Make the minimum window size
    //

    SetMinimumClientSize(sizeMin);

    // Filter all keyboard input
    m_pkeyboardInput = new EngineWindowKeyboardInput(this);
    AddKeyboardInputFilter(m_pkeyboardInput);

    // Initialize performance counters
    m_pszLabel[0]   = '\0';
    m_bFPS          = false;
    m_indexFPS      = 0;
    m_frameCount    = 0;
    m_frameTotal    = 0;
    m_timeLastFrame =
    m_timeLast      =
    m_timeCurrent   = 
    m_timeStart     = Time::Now();
    m_timeLastClick = 0;

    // menu
    m_pmenuCommandSink  = new MenuCommandSink(this);

    // Start the callback
    EnableIdleFunction();

	// Create the D3D device.
	HWND hWindow = GetHWND();

	CLogFile devLog( "DeviceCreation.log" );
	pDev->Initialise( &devLog );
	if( pDev->CreateD3D9( &devLog ) != D3D_OK )
	{
		_ASSERT( false );
	}
	if( pDev->CreateDevice( hWindow, &devLog ) != D3D_OK )
	{
		_ASSERT( false );
	}
	devLog.OutputString( "Device creation finished.\n" );

	// Initialise the various engine components.
	CVRAMManager::Get()->Initialise( );
	CVBIBManager::Get()->Initialise( );
	CVertexGenerator::Get()->Initialise( );
}

EngineWindow::~EngineWindow()
{
}

void EngineWindow::PostWindowCreationInit()
{
    // Tell the engine we are the window
    GetEngine()->SetFocusWindow(this, m_bStartFullScreen);

    //
    // These rects track the size of the window
    //

    m_prectValueScreen     = new ModifiableRectValue(GetClientRect());
    m_prectValueRender     = new ModifiableRectValue(Rect(0, 0, 640, 480));
    m_pwrapRectValueRender = new WrapRectValue(m_prectValueScreen);
    m_modeIndex            = s_countModes;

    //
    // Intialize all the Image stuff
 /*   m_pgroupImage =
        new GroupImage(
            CreateUndetectableImage(
                m_ptransformImageCursor = new TransformImage(
                    Image::GetEmpty(),
                    m_ptranslateTransform = new TranslateTransform2(
                        m_ppointMouse
                    )
                )
            ),
            m_pwrapImage = new WrapImage(Image::GetEmpty())
        );*/

	m_ptranslateTransform	= new TranslateTransform2( m_ppointMouse );
	m_ptransformImageCursor = new TransformImage( Image::GetEmpty(), m_ptranslateTransform );
	m_pwrapImage			= new WrapImage(Image::GetEmpty());
    m_pgroupImage			= new GroupImage( CreateUndetectableImage( m_ptransformImageCursor ), m_pwrapImage );

    //
    // Setup the popup container
    m_ppopupContainer = m_pEngineApp->GetPopupContainer();
    IPopupContainerPrivate* ppcp; CastTo(ppcp, m_ppopupContainer);
    ppcp->Initialize(m_pengine, GetScreenRectValue());

	m_pfontFPS = GetModeler()->GetNameSpace("model")->FindFont("defaultFont");
}

void EngineWindow::StartClose()
{
    PostMessage(WM_CLOSE);
}

bool EngineWindow::IsValid()
{
    return m_pengine->IsValid();
}

void EngineWindow::OnClose()
{
    RemoveKeyboardInputFilter(m_pkeyboardInput);

    m_pgroupImage           = NULL;
    m_pwrapImage            = NULL;
    m_ptransformImageCursor = NULL;
    m_ptranslateTransform   = NULL;

    m_pmodeler->Terminate();
    m_pmodeler = NULL;

    m_pengine->Terminate();

    Window::OnClose();
}

// TBD: Switch these flags off.
bool g_bMDLLog    = false;
bool g_bWindowLog = true;

void EngineWindow::ParseCommandLine(const ZString& strCommandLine, bool& bStartFullscreen)
{
    PCC pcc = strCommandLine;
    CommandLineToken token(pcc, strCommandLine.GetLength());

    while (token.MoreTokens()) {
        ZString str;

        if (token.IsMinus(str)) {
// BUILD_DX9
            if (str == "mdllog") {
                g_bMDLLog = true;
            } else if (str == "windowlog") {
                g_bWindowLog = true;
            }
//#else
//            if (str == "windowed") {
//                bStartFullscreen = false;
//            } else if (str == "fullscreen") {
//                bStartFullscreen = true;
//            } else if (str == "mdllog") {
//                g_bMDLLog = true;
//            } else if (str == "windowlog") {
//                g_bWindowLog = true;
//            }
// BUILD_DX9
        } else {
            token.IsString(str);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////
// InitialiseTime()
//
//////////////////////////////////////////////////////////////////////////////
void EngineWindow::InitialiseTime()
{
    // get time
    TRef<INameSpace> pnsModel = m_pmodeler->GetNameSpace("model");
    CastTo(m_pnumberTime, pnsModel->FindMember("time"));
}

//////////////////////////////////////////////////////////////////////////////
//
// Private Attribute Functions
//
//////////////////////////////////////////////////////////////////////////////

void EngineWindow::UpdateSurfacePointer()
{
	//Imago 6/26/09

    CD3DDevice9 * pDev = CD3DDevice9::Get();

    if (!m_pengine->IsFullscreen()) 
	{
		WinPoint size = GetClientRect().Size();

		if (size.X() == 0) {
		    size.SetX(1);
		}

		if (size.Y() == 0) {
		    size.SetY(1);
		}
		
		m_psurface = m_pengine->CreateDummySurface( size, NULL );
		ZAssert(m_psurface != NULL && m_psurface->IsValid());
		if(pDev->IsDeviceValid())
			DDCall(pDev->ResetDevice(true,size.X(),size.Y()));

	} else {

		//NYI: Expose these so we can step up / down full screen resolutions again
		int x = pDev->GetDeviceSetupParams()->pFullScreenResArray->iWidth;
		int y = pDev->GetDeviceSetupParams()->pFullScreenResArray->iHeight;

		m_psurface = m_pengine->CreateDummySurface(WinPoint(x,y), NULL );
		ZAssert(m_psurface != NULL && m_psurface->IsValid());
		if(pDev->IsDeviceValid())
			DDCall(pDev->ResetDevice(false,x,y));
	}
}

/* OLD ROUTINES FOR ABOVE FUNCTION COMBINED

	CD3DDevice9 * pDev = CD3DDevice9::Get();
	if( !m_pengine->IsFullscreen() )
	{
		m_psurface = m_pengine->CreateDummySurface( GetClientRect().Size(), NULL );
		
		if( pDev->IsDeviceValid() == true )
		{
			pDev->ResetDevice(	pDev->IsWindowed(), 
								GetClientRect().Size().x,
								GetClientRect().Size().y );
		}
	}
	else
	{
		m_psurface = m_pengine->CreateDummySurface( m_pengine->GetFullscreenSize(), NULL );

		if( pDev->IsDeviceValid() == true )
		{
			pDev->ResetDevice(	pDev->IsWindowed(), 
								m_pengine->GetFullscreenSize().x,
								m_pengine->GetFullscreenSize().y );
		}
	}
	*/


/*	if( (!m_pengine->IsFullscreen())
        || (!m_pengine->GetUsing3DAcceleration())
    ) {
        WinPoint size = GetClientRect().Size();

        if (size.X() == 0) {
            size.SetX(1);
        }

        if (size.Y() == 0) {
            size.SetY(1);
        }

        if (
               m_pengine->PrimaryHas3DAcceleration()
            && m_pengine->GetAllow3DAcceleration()
        ) {
            m_psurface =
                m_pengine->CreateSurface(
                    size,
                    SurfaceType2D() | SurfaceType3D() | SurfaceTypeZBuffer() | SurfaceTypeVideo(),
                    NULL
                );

            if (m_psurface != NULL && m_psurface->IsValid()) {
                return;
            }
        }

        m_psurface =
            m_pengine->CreateSurface(
                size,
                SurfaceType2D() | SurfaceType3D() | SurfaceTypeZBuffer(),
                NULL
            );
    }*/

void EngineWindow::UpdateWindowStyle()
{
    if (m_pengine->IsFullscreen()) {
        SetHasMinimize(false);
        SetHasMaximize(false);
        SetHasSysMenu(false);
        Window::SetSizeable(false);
        SetTopMost(true);

        //
        // Size the window to cover the entire desktop
        // Win32 doesn't recognize the style change unless we resize the window
        //

        WinRect rect = GetRect();
        SetClientRect(
            WinRect(
                rect.Min(),
                rect.Max() + WinPoint(1, 1)
            )
        );

        SetClientRect(rect);
    } else {
        SetHasMinimize(true);
        SetHasMaximize(true);
        SetHasSysMenu(true);
        Window::SetSizeable(m_bSizeable);
        SetTopMost(false);
        //SetTopMost(true);  Imago 6/25/09 - removed this undocumented change

        //
        // Win32 doesn't recognize the style change unless we resize the window
        //
        
        WinPoint size = m_sizeWindowed;

        m_bMovingWindow = true;
        SetClientSize(size + WinPoint(1, 1));
        SetClientSize(size);
        SetPosition(m_offsetWindowed);
        m_bMovingWindow = false;
    }

    //
    // Enable DInput if we are fullscreen
    //

    m_pmouse->SetEnabled(m_bActive && m_pengine->IsFullscreen());
}

void EngineWindow::UpdateRectValues()
{
    if (g_bWindowLog) {
        ZDebugOutput("EngineWindow::UpdateRectValues()\n");
    }

    //
    // The screen rect
    //

    if (m_pengine->IsFullscreen()) {
        WinRect 
            rect(
                WinPoint(0, 0),
                m_pengine->GetFullscreenSize()
            );

        if (g_bWindowLog) {
            ZDebugOutput("  Fullscreen: " + GetString(0, rect) + "\n");
        }

        m_prectValueScreen->SetValue(rect);
        m_pmouse->SetClipRect(rect);
    } else {
        WinRect rect(WinPoint(0, 0), m_sizeWindowed);

        if (g_bWindowLog) {
            ZDebugOutput("  Windowed: " + GetString(0, rect) + "\n");
        }

        m_prectValueScreen->SetValue(rect);
        m_pmouse->SetClipRect(rect);
    }

    //
    // The render rect
    //

    if (m_pengine->IsFullscreen() && m_modeIndex < s_countModes) {
        WinPoint size = GetFullscreenSize();

        float dx = ((float)size.X() - s_pmodes[m_modeIndex].m_size.X()) / 2.0f;
        float dy = ((float)size.Y() - s_pmodes[m_modeIndex].m_size.Y()) / 2.0f;

        m_prectValueRender->SetValue(
            Rect(
                dx,
                dy,
                (float)size.X() - dx,
                (float)size.Y() - dy
            )
        );

        m_pwrapRectValueRender->SetWrappedValue(m_prectValueRender);
    } else {
        m_pwrapRectValueRender->SetWrappedValue(m_prectValueScreen);
    }
}

void EngineWindow::SetMoveOnHide(bool bMoveOnHide)
{
    m_bMoveOnHide = bMoveOnHide;
}

void EngineWindow::UpdateCursor()
{
    bool bGameCursor;
    
    if (m_pengine->IsFullscreen()) {
        bGameCursor = true;
    } else if (m_bCaptured) {
        bGameCursor = m_prectValueScreen->GetValue().Inside(m_ppointMouse->GetValue());
    } else {
        bGameCursor = m_bMouseInside;
    }

    if (bGameCursor) {
        bool bTimeOut = 
               m_bHideCursor
            && m_timeCurrent - m_timeLastMouseMove > 2.0f;

        if (
               m_pimageCursor != NULL 
            && m_bShowCursor
            && (!bTimeOut)
        ) {
            m_ptransformImageCursor->SetImage(m_pimageCursor);
            s_cursorIsHidden = false;
        } else {
            /*
            if (m_bMoveOnHide) {
                HandleMouseMessage(0, Point(0, 0));
            }
            */
            m_ptransformImageCursor->SetImage(Image::GetEmpty());
            s_cursorIsHidden = true;
        }
        ShowMouse(false);
    } else {
        m_ptransformImageCursor->SetImage(Image::GetEmpty());
        ShowMouse(m_bShowCursor);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Public methods
//
//////////////////////////////////////////////////////////////////////////////

RectValue* EngineWindow::GetScreenRectValue()
{
    return m_prectValueScreen;
}

RectValue* EngineWindow::GetRenderRectValue()
{
    return m_pwrapRectValueRender;
}

void EngineWindow::SetFullscreen(bool bFullscreen)
{
    if (m_pengine->IsFullscreen() != bFullscreen) {
        //
        // Make sure we don't recurse
        //

        m_bMovingWindow = true;

        //
        // Release the backbuffer
        //

//        m_psurface = NULL;

        //
        // Switch modes
        //

        m_pengine->SetFullscreen(bFullscreen);

        //
        // Tell the caption
        //

        if (m_pcaption) {
            m_pcaption->SetFullscreen(bFullscreen);
        }

        //
        // Enable DirectInput mouse?
        //

        m_pmouse->SetEnabled(m_pengine->IsFullscreen() && m_bActive);
        m_pmouse->SetPosition(m_prectValueScreen->GetValue().Center());

        //
        // Done, start listening to window sizing notifications
        //

        m_bMovingWindow = false;
    }
}

bool EngineWindow::OnWindowPosChanging(WINDOWPOS* pwp)
{
	char szBuffer[256];
	sprintf( szBuffer, "%d  %d,   %d %d\n", pwp->x, pwp->y, 
		m_bWindowStateMinimised, m_bWindowStateRestored );
	OutputDebugString( szBuffer );

    if (GetFullscreen()) {
        pwp->x = 0;
        pwp->y = 0;
    } 
	else 
	{
		// For some reason, when restoring a minimised window, it gets a position of
		// -32000, -32000.
		pwp->x = max( 0, pwp->x );
		pwp->y = max( 0, pwp->y );
        if (!m_bMovingWindow) 
		{
            return Window::OnWindowPosChanging(pwp);
        }
    }

    return true;
}

void EngineWindow::Invalidate()
{
    m_bInvalid = true;
}

void EngineWindow::RectChanged()
{
    if (
           (!m_bMovingWindow)
        && (!m_pengine->IsFullscreen())
    ) {
        WinPoint size = GetClientRect().Size();

        if (
               (size           != WinPoint(0, 0))
            && (m_sizeWindowed != size          )
        ) {
            m_sizeWindowed = size;
            Invalidate();
        }

        if (m_offsetWindowed != GetRect().Min()) {
            m_offsetWindowed = GetRect().Min();
        }
    }
}

void EngineWindow::SetSizeable(bool bSizeable)
{
    if (m_bSizeable != bSizeable) {
        m_bSizeable = bSizeable;

        if (m_pitemHigherResolution) {
            m_pitemHigherResolution->SetEnabled(m_bSizeable);
            m_pitemLowerResolution->SetEnabled(m_bSizeable);
        }

        Invalidate();
    }
}

WinPoint EngineWindow::GetSize()
{
    if (m_pengine->IsFullscreen()) {
        return GetFullscreenSize();
    } else {
        return GetWindowedSize();
    }
}

WinPoint EngineWindow::GetWindowedSize()
{
    return m_sizeWindowed;
}

WinPoint EngineWindow::GetFullscreenSize()
{
    return m_pengine->GetFullscreenSize();
}

void EngineWindow::SetWindowedSize(const WinPoint& size)
{
    if (g_bWindowLog) {
        ZDebugOutput("EngineWindow::SetWindowedSize(" + GetString(size) + ")\n");
    }

    if (m_sizeWindowed != size) {
        m_sizeWindowed = size;

        if (!m_pengine->IsFullscreen()) {
            Invalidate();
        }
    }

    if (g_bWindowLog) {
        ZDebugOutput("EngineWindow::SetWindowedSize() exiting\n");
    }
}

void EngineWindow::Set3DAccelerationImportant(bool b3DAccelerationImportant)
{
    m_pengine->Set3DAccelerationImportant(b3DAccelerationImportant);
}

void EngineWindow::SetFullscreenSize(const WinPoint& size)
{
    m_pengine->SetFullscreenSize(size);
}

void EngineWindow::ChangeFullscreenSize(bool bLarger)
{
    if (m_pengine->IsFullscreen() && m_bSizeable) 
	{
/*        WinPoint size = GetFullscreenSize();

        if (size == WinPoint(640, 480)) 
		{
            if (bLarger) 
			{
                if (m_modeIndex < s_countModes) 
				{
                    m_modeIndex++;
                } 
				else 
				{
                    m_pengine->ChangeFullscreenSize(bLarger);
                }
            } 
			else 
			{
                if (m_modeIndex > 0) 
				{
                    m_modeIndex--;
                }
            }
        } 
		else 
		{
            m_pengine->ChangeFullscreenSize(bLarger);
        }

        Invalidate();

        RenderSizeChanged(	( size == WinPoint ( 640, 480 ) ) && 
							( m_modeIndex < s_countModes ) );*/

		CD3DDevice9 * pDev = CD3DDevice9::Get();
		if( bLarger == true )
		{
			pDev->GetDeviceSetupParams()->iCurrentRes ++;
			pDev->GetDeviceSetupParams()->iCurrentRes %= pDev->GetDeviceSetupParams()->iNumRes;
		}
		else
		{
			pDev->GetDeviceSetupParams()->iCurrentRes --;
			pDev->GetDeviceSetupParams()->iCurrentRes %= pDev->GetDeviceSetupParams()->iNumRes;
		}
		_ASSERT( ( pDev->GetDeviceSetupParams()->iCurrentRes >= 0 ) &&
				( pDev->GetDeviceSetupParams()->iCurrentRes < pDev->GetDeviceSetupParams()->iNumRes ) );
        Invalidate();
        RenderSizeChanged( false );
    }
}

void EngineWindow::SetImage(Image* pimage)
{
    m_pwrapImage->SetImage(pimage);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

#define idmHigherResolution    1
#define idmLowerResolution     2
#define idmAllow3DAcceleration 3
#define idmAllowSecondary      4
#define idmBrightnessUp        5
#define idmBrightnessDown      6

TRef<IPopup> EngineWindow::GetEngineMenu(IEngineFont* pfont)
{
    TRef<IMenu> pmenu =
        CreateMenu(
            GetModeler(),
            pfont,
            m_pmenuCommandSink
        );

                                 pmenu->AddMenuItem(idmBrightnessUp       , "Brightness Up"                                   , 'U');
                                 pmenu->AddMenuItem(idmBrightnessDown     , "Brightness Down"                                 , 'D');
                                 pmenu->AddMenuItem(0                     , "------------------------------------------------"     );
                                 pmenu->AddMenuItem(0                     , "The following options are only valid when flying"     );
                                 pmenu->AddMenuItem(0                     , "------------------------------------------------"     );
    m_pitemHigherResolution    = pmenu->AddMenuItem(idmHigherResolution   , "Higher Resolution"                               , 'H');
    m_pitemLowerResolution     = pmenu->AddMenuItem(idmLowerResolution    , "Lower Resolution"                                , 'L');

								 pmenu->AddMenuItem(0                     , "------------------------------------------------"     );
                                 pmenu->AddMenuItem(0                     , "Current device state"                                 );
                                 pmenu->AddMenuItem(0                     , "------------------------------------------------"     );
    m_pitemDevice              = pmenu->AddMenuItem(0                     , GetDeviceString()                                      );
//    m_pitemRenderer            = pmenu->AddMenuItem(0                     , GetRendererString()                                    );
    m_pitemResolution          = pmenu->AddMenuItem(0                     , GetResolutionString()                                  );
    m_pitemRendering           = pmenu->AddMenuItem(0                     , GetRenderingString()                                   );
    m_pitemBPP                 = pmenu->AddMenuItem(0                     , GetPixelFormatString()                                 ); // KGJV 32B

    return pmenu;
}

ZString EngineWindow::GetResolutionString()
{
    Point size = GetScreenRectValue()->GetValue().Size();

    return "Resolution: " + ZString(size.X()) + " x " + ZString(size.Y());
}

ZString EngineWindow::GetRenderingString()
{
    Point size = GetRenderRectValue()->GetValue().Size();

    return "Rendering: " + ZString(size.X()) + " x " + ZString(size.Y());
}

// KGJV 32B
ZString EngineWindow::GetPixelFormatString()
{
    return GetEngine()->GetPixelFormatName();
}
ZString EngineWindow::GetRendererString()
{
    return 
          (
               GetEngine()->GetUsing3DAcceleration() 
            && (
                   m_psurface == NULL 
                || m_psurface->GetSurfaceType().Test(SurfaceTypeVideo())
               )
          )
        ? "Using hardware rendering"
        : "Using software rendering";
}

ZString EngineWindow::GetDeviceString()
{
    return "Device: " + GetEngine()->GetDeviceName();
}

ZString EngineWindow::GetAllow3DAccelerationString()
{
    return 
          GetEngine()->GetAllow3DAcceleration()
        ? "Use 3D acceleration when needed"
        : "Never use 3D acceleration";
}

ZString EngineWindow::GetAllowSecondaryString()
{
    return 
          GetEngine()->GetAllowSecondary()
        ? "Use secondary device for 3D acceleration when needed"
        : "Never use secondary device";
}

void EngineWindow::UpdateMenuStrings()
{
    if (m_pitemDevice) {
        m_pitemDevice             ->SetString(GetDeviceString()             );
//        m_pitemRenderer           ->SetString(GetRendererString()           );
        m_pitemResolution         ->SetString(GetResolutionString()         );
        m_pitemRendering          ->SetString(GetRenderingString()          );
        m_pitemBPP                ->SetString(GetPixelFormatString()        );
    }
}

void EngineWindow::OnEngineWindowMenuCommand(IMenuItem* pitem)
{
    switch (pitem->GetID()) 
	{
/*       case idmAllowSecondary:
            GetEngine()->SetAllowSecondary(
                !GetEngine()->GetAllowSecondary()
            );
            break;

        case idmAllow3DAcceleration:
            GetEngine()->SetAllow3DAcceleration(
                !GetEngine()->GetAllow3DAcceleration()
            );
            break;

		// DISABLE THE higher/lower resolution option - to be reinstated at some point.
		case idmHigherResolution:
            ChangeFullscreenSize(true);
            break;

        case idmLowerResolution:
            ChangeFullscreenSize(false);
            break;*/

        case idmBrightnessUp:
            GetEngine()->SetGammaLevel(
                GetEngine()->GetGammaLevel() * 1.01f
            );
			break;

        case idmBrightnessDown:
            GetEngine()->SetGammaLevel(
                GetEngine()->GetGammaLevel() / 1.01f
            );
            break;
    }
}

ZString EngineWindow::GetFPSString(float fps, float mspf, Context* pcontext)
{
    return ZString();
}

void EngineWindow::UpdatePerformanceCounters(Context* pcontext, Time timeCurrent)
{
    m_frameTotal++;
    if (m_bFPS) {
        if (m_frameCount == -1) {
//            pcontext->ResetPerformanceCounters();
			CD3DDevice9::Get()->ResetPerformanceCounters();
            m_frameCount    = 0;
            m_timeLastFrame = timeCurrent;
        }

        m_frameCount++;
		if (m_timeCurrent - m_timeLastFrame > 1.0) 
		{
			float fOneOverFrameCount = 1.0f / m_frameCount;
			float fNumPrims = (float)CD3DDevice9::Get()->GetPerformanceCounter( CD3DDevice9::eD9S_NumPrimsRendered );
			float fPrimsPerFrame = fNumPrims * fOneOverFrameCount;
			float fPrimsPerSecond = fNumPrims / (timeCurrent - m_timeLastFrame);
			float fDPsPerFrame = (float) CD3DDevice9::Get()->GetPerformanceCounter( CD3DDevice9::eD9S_NumDrawPrims );
			float fStateChangesPerFrame = (float) CD3DDevice9::Get()->GetPerformanceCounter( CD3DDevice9::eD9S_NumStateChanges );
			float fTextureChangesPerFrame = (float) CD3DDevice9::Get()->GetPerformanceCounter( CD3DDevice9::eD9S_NumTextureChanges );
			float fShaderChangesPerFrame = (float) CD3DDevice9::Get()->GetPerformanceCounter( CD3DDevice9::eD9S_NumShaderChanges );
			fDPsPerFrame *= fOneOverFrameCount;
			fStateChangesPerFrame *= fOneOverFrameCount;
			fTextureChangesPerFrame *= fOneOverFrameCount;
			fShaderChangesPerFrame *= fOneOverFrameCount;

			double fps  = m_frameCount / (timeCurrent - m_timeLastFrame);
			double mspf = 1000.0 * (timeCurrent - m_timeLastFrame) / m_frameCount;

			if (m_indexFPS == 0) 
			{
				// Old performance string:
				// mspf, fps, tmem/maxtmem, vmem/maxvmem, tris/frame, tris/sec, points/frame, gamma
				//
				// New performance string:
				// fps, available tmem, num prims, num dp calls,
				// num state changes, num tex changes, num shader changes.

				m_strPerformance1 =
#ifdef ICAP
					ZString("ICAP: ")
#elif defined(_DEBUG)
					ZString("Debug: ")
#else
					ZString("Retail: ")
#endif
//					+ ZString(CD3DDevice9::GetDeviceSetupString())
					+ " mspf: " + ZString(MakeInt(mspf))
					+ " fps: " + ZString(MakeInt(fps))
//					+ " tmem: (" + ZString(CD3DDevice9::GetPerformanceCounter( CD3DDevice9::eD9S_CurrentTexMem ) )
//					+ "of " + ZString(CD3DDevice9::GetPerformanceCounter( CD3DDevice9::eD9S_InitialTexMem ) )
//					+ ") ppf: " + ZString(MakeInt(fPrimsPerFrame))
					+ " ppf: " + ZString(MakeInt(fPrimsPerFrame))
					+ " pps: " + ZString(MakeInt(fPrimsPerSecond))
					+ " #dp: " + ZString(MakeInt(fDPsPerFrame))
					+ " #sc: " + ZString(MakeInt(fStateChangesPerFrame))
					+ " #tx: " + ZString(MakeInt(fTextureChangesPerFrame))
					+ " #shc: " + ZString(MakeInt(fShaderChangesPerFrame));
				
				m_strPerformance2 = GetFPSString((float)fps, (float)mspf, pcontext);
			} 
			else 
			{
				m_strPerformance1 = ZString(MakeInt(mspf)) + "/" + ZString(MakeInt(fps));
			}

            m_frameCount    = 0;
            m_timeLastFrame = timeCurrent;
			CD3DDevice9::Get()->ResetPerformanceCounters();
        }
    }
}

void EngineWindow::OutputPerformanceCounters()
{
   if (m_bFPS) {
        ZString strOut = m_strPerformance1 + " " + m_strPerformance2 + "\r\n";
        OutputDebugStringA(strOut);
    }
}

void EngineWindow::RenderPerformanceCounters(Surface* psurface)
{
    if (m_bFPS) {
		
		// Disable AA.
		CD3DDevice9::Get()->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

        int ysize = m_pfontFPS->GetHeight();
        Color color(1, 0, 0);

        psurface->DrawString(m_pfontFPS, color, WinPoint(1, 1 + 0 * ysize), m_strPerformance1);

        if (m_indexFPS == 0) {
            psurface->DrawString(m_pfontFPS, color, WinPoint(1, 1 + 1 * ysize), m_strPerformance2);
            psurface->DrawString(m_pfontFPS, color, WinPoint(1, 1 + 2 * ysize), "Frame " + ZString(m_frameTotal));
        }
		
		// Reenable AA.
		CD3DDevice9::Get()->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	}

    #ifdef ICAP
        if (IsProfiling()) {
            psurface->DrawString(m_pfontFPS, Color::White(), WinPoint(1, 1), "Profiling");
        }
    #endif
}

void EngineWindow::SetHideCursorTimer(bool bHideCursor)
{
    m_bHideCursor = bHideCursor;
}

void EngineWindow::UpdateFrame()
{
    m_timeCurrent = Time::Now();
    m_pnumberTime->SetValue(m_timeCurrent - m_timeStart);
    EvaluateFrame(m_timeCurrent);
    m_pgroupImage->Update();
}


// 205 - main background texture index.
static int iDebugTexCount = 1;

bool EngineWindow::RenderFrame()
{
    PrivateEngine* pengine; CastTo(pengine, m_pengine);
    TRef<Surface> psurface;

	HRESULT hr = CD3DDevice9::Get()->BeginScene();
	_ASSERT( hr == D3D_OK );
	_ASSERT( m_psurface != NULL );

	TRef<Context> pcontext = m_psurface->GetContext();
	if( pcontext ) 
	{
//		pcontext->BeginRendering();

		ZStartTrace("---------Begin Frame---------------");

		const Rect& rect = m_pwrapRectValueRender->GetValue();
		pcontext->Clip(rect);

		m_pgroupImage->Render( pcontext );
		UpdatePerformanceCounters(pcontext, m_timeCurrent);
		m_psurface->ReleaseContext(pcontext);
		RenderPerformanceCounters(m_psurface);
	}
	hr = CD3DDevice9::Get()->EndScene();

	if( hr == D3D_OK )
	{
		return true;
	}
    return false;
}

void EngineWindow::OnPaint(HDC hdc, const WinRect& rect)
{
    /* !!!
    if (m_psurface) {
        UpdateFrame();
        if (RenderFrame()) {
            //hdc = GetDC();
            m_psurface->BitBltToDC(hdc);
            //ReleaseDC(hdc);
            return;
        }
    }
    */

 //   ZVerify(::FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)));
}

bool EngineWindow::ShouldDrawFrame()
{
    if (m_pengine->IsFullscreen()) {
        return true;
    } else {
        return !m_bMinimized;
    }
}

void EngineWindow::DoIdle()
{
    //
    // Update the input values
    //

    UpdateInput();

    //
    // Switch fullscreen state if requested
    //

    if (m_bRestore) {
        m_bRestore = false;
        SetFullscreen(false);
    }
    
    //
    // Is the device ready
    //

    bool bChanges = false;
    if (m_pengine->IsDeviceReady(bChanges)) 
	{
        if (bChanges || m_bInvalid) 
		{
            m_bInvalid = false;

            UpdateWindowStyle();
            UpdateRectValues();
            UpdateMenuStrings();
            UpdateSurfacePointer();
        }

        //
        // Evaluation
        //

        UpdateFrame();

        //
        // Rendering
        if( ShouldDrawFrame() ) 
		{
			CD3DDevice9::Get()->ClearScreen( );

			UpdateCursor();

			// Render, and if successful, display.
			if( RenderFrame() ) 
			{
				CD3DDevice9::Get()->RenderFinished( );
				return;
			}
        }
    } else {
        //
        // We don't have access to the device just update the frame
        //

        UpdateFrame();
    }

    //
    // we are flipping, but
    //      we are minimized
    //      we couldn't get the surface
    //   or we couldn't get the context
    // so sleep for a while so we don't eat up too much processor time
    //

    ::Sleep(30);
}

void EngineWindow::SetShowFPS(bool bFPS, const char* pszLabel)
{
    if (pszLabel)
        strncpy_s(m_pszLabel, 20, pszLabel, sizeof(m_pszLabel) - 1);
    else
        m_pszLabel[0] = '\0';

    m_bFPS       = bFPS;
    m_indexFPS   = 0;
    m_frameCount = -1;
}

void EngineWindow::ToggleShowFPS()
{
    if (m_bFPS) {
        if (m_indexFPS == 0) {
            m_indexFPS = 1;
        } else {
            m_bFPS     = false;
            m_indexFPS = 0;
        }
    } else {
        m_bFPS = true;
    }
}

bool EngineWindow::OnActivate(UINT nState, bool bMinimized)
{
    m_bMinimized = bMinimized;
    return Window::OnActivate(nState, bMinimized);
}

bool EngineWindow::OnActivateApp(bool bActive)
{
    if (g_bWindowLog) {
        ZDebugOutput(
              ZString("OnActivateApp: was ")
            + (m_bActive ? "active" : "inactive")
            + ", becoming "
            + (bActive ? "active" : "inactive")
            + "\n"
        );
    }

    if (m_bActive != bActive) {
        m_bActive = bActive;
        if (m_pengine) {
            m_pmouse->SetEnabled(m_bActive && m_pengine->IsFullscreen());
        }
        m_pinputEngine->SetFocus(m_bActive);
		
		if( m_bActive == true )
		{
			SetActiveWindow( GetHWND() );
		}
    }

    if (g_bWindowLog) {
        ZDebugOutput("OnActivateApp exiting\n");
    }

    return Window::OnActivateApp(bActive);
}

void EngineWindow::SetCursorImage(Image* pimage)
{
    if (m_pimageCursor != pimage) {
        m_pimageCursor = pimage;
    }
}

Image* EngineWindow::GetCursorImage(void) const
{
    return m_pimageCursor;
}

bool EngineWindow::OnSysCommand(UINT uCmdType, const WinPoint &point)
{
    switch (uCmdType) {
        case SC_KEYMENU:
            //
            // don't let the ALT key open the system menu
            //
            return true;

        case SC_MAXIMIZE:
			m_bWindowStateMinimised = false;
			m_bWindowStateRestored = false;
            SetFullscreen(true);
            return true;

		case SC_MINIMIZE:
			m_bWindowStateMinimised = true;
			break;

		case SC_RESTORE:
			if( m_bWindowStateMinimised )
			{
				m_bWindowStateMinimised = false;
				m_bWindowStateRestored = true;
			}
			break;

        case SC_CLOSE:
            StartClose();
            return true;
    }

    return Window::OnSysCommand(uCmdType, point);
}

//////////////////////////////////////////////////////////////////////////////
//
// IInputProvider
//
//////////////////////////////////////////////////////////////////////////////

bool EngineWindow::IsDoubleClick()
{
    return (m_timeCurrent < (m_timeLastClick + 0.25f));
}

void EngineWindow::SetCursorPos(const Point& point)
{
    if (m_pengine->IsFullscreen()) {
        m_pmouse->SetPosition(point);
        //HandleMouseMessage(WM_MOUSEMOVE, point);
    } else {
        Window::SetCursorPos(point);
    }
}

void EngineWindow::ShowCursor(bool bShow)
{
    m_bShowCursor = bShow;
}

//////////////////////////////////////////////////////////////////////////////
//
// Mouse stuff
//
//////////////////////////////////////////////////////////////////////////////

int     EngineWindow::s_forceHitTestCount = 0;
bool    EngineWindow::s_cursorIsHidden = false;

void EngineWindow::DoHitTest()
{
    if (!s_cursorIsHidden)
    {
        Window::DoHitTest();
        s_forceHitTestCount = 2;
    }
}

void EngineWindow::SetMouseEnabled(bool bEnable)
{
    m_bMouseEnabled = bEnable;
}

void EngineWindow::HandleMouseMessage(UINT message, const Point& point)
{
    if (m_pgroupImage != NULL) {
        //
        // Make sure these objects don't go away until we are done
        //

        TRef<Image>  pimage = m_pgroupImage;
        TRef<Window> pthis  = this;

        //
        // Handle mouse move messages
        //

        switch (message) {
            case 0: // 0 == WM_MOUSEENTER
                //pimage->MouseEnter(this, point);
                m_bMouseInside = true;
                break;

            case WM_MOUSELEAVE:
                //pimage->MouseLeave(this);
                m_bMouseInside = false;
                break;

            case WM_MOUSEMOVE:
                m_timeLastMouseMove = m_timeCurrent;
                m_timeLastClick     = 0;
                m_ppointMouse->SetValue(point);

                if (m_pengine->IsFullscreen()) {
                    m_ptransformImageCursor->SetImage(m_pimageCursor ? m_pimageCursor : Image::GetEmpty());
                }
                break;
        }
    
        //
        // Is the mouse hitting the image?
        //

        MouseResult mouseResult;

        while (true) {
            mouseResult = pimage->HitTest(this, point, m_bCaptured);

            if (!mouseResult.Test(MouseResultRelease())) {
                break;
            } 

            pimage->RemoveCapture();
            ReleaseMouse();
            m_bCaptured = false;
        }

        bool bHit = m_bMouseInside && mouseResult.Test(MouseResultHit());

        //
        // Call MouseMove, MouseLeave or MouseEnter
        //

        if (m_bHit != bHit) {
            if (m_bHit) {
                pimage->MouseLeave(this);
            }

            m_bHit = bHit;

            if (m_bHit) {
                pimage->MouseEnter(this, point);
            }
        } else if (m_bHit) {
            pimage->MouseMove(this, point, false, true);
        }

        //
        // Handle button messages
        //

        if (m_bMouseEnabled) {    
            switch (message) {
                case WM_LBUTTONDOWN: 
                    mouseResult = pimage->Button(this, point, 0, m_bCaptured, m_bHit, true );
                    m_timeLastClick = m_timeCurrent;
                    break;

                case WM_LBUTTONUP:   
                    mouseResult = pimage->Button(this, point, 0, m_bCaptured, m_bHit, false);
                    break;

                case WM_RBUTTONDOWN: 
                    mouseResult = pimage->Button(this, point, 1, m_bCaptured, m_bHit, true );
                    break;

                case WM_RBUTTONUP:   
                    mouseResult = pimage->Button(this, point, 1, m_bCaptured, m_bHit, false);
                    break;

                case WM_MBUTTONDOWN: 
                    mouseResult = pimage->Button(this, point, 2, m_bCaptured, m_bHit, true );
                    break;

                case WM_MBUTTONUP:   
                    mouseResult = pimage->Button(this, point, 2, m_bCaptured, m_bHit, false);
                    break;
            }
        }

        if (mouseResult.Test(MouseResultRelease())) {
            pimage->RemoveCapture();
            ReleaseMouse();
            m_bCaptured = false;
        } else if (mouseResult.Test(MouseResultCapture())) {
            CaptureMouse();
            m_bCaptured = true;
        }
    }
}

bool EngineWindow::OnMouseMessage(UINT message, UINT nFlags, const WinPoint& point)
{
    if (!m_pengine->IsFullscreen()) {
        HandleMouseMessage(message, Point::Cast(point));
    }
    
    return true;
}

bool EngineWindow::OnEvent(ButtonEvent::Source* pevent, ButtonEventData be)
{
    //
    // button state change
    //

    if (be.GetButton() == 0) {
        if (be.IsDown()) {
            HandleMouseMessage(WM_LBUTTONDOWN, m_pmouse->GetPosition());
        } else {
            HandleMouseMessage(WM_LBUTTONUP,   m_pmouse->GetPosition());
        }
    } else if (be.GetButton() == 1) {
        if (be.IsDown()) {
            HandleMouseMessage(WM_RBUTTONDOWN, m_pmouse->GetPosition());
        } else {
            HandleMouseMessage(WM_RBUTTONUP,   m_pmouse->GetPosition());
        }
    } else if (be.GetButton() == 2) {
        if (be.IsDown()) {
            HandleMouseMessage(WM_MBUTTONDOWN, m_pmouse->GetPosition());
        } else {
            HandleMouseMessage(WM_MBUTTONUP,   m_pmouse->GetPosition());
        }
    }

    return true;
}

void EngineWindow::UpdateInput()
{
    m_pinputEngine->Update();

    //
    // Update the mouse position
    //

    if (m_pengine->IsFullscreen()) {
        if (m_ppointMouse->GetValue() != m_pmouse->GetPosition() || (s_forceHitTestCount >> 0)) {
            if (s_forceHitTestCount > 0) {
                s_forceHitTestCount--;
            }
            HandleMouseMessage(WM_MOUSEMOVE, m_pmouse->GetPosition());
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// IScreenSiteMethods
//
//////////////////////////////////////////////////////////////////////////////

void EngineWindow::SetCaption(ICaption* pcaption)
{
    m_pcaption = pcaption;
    if (m_pcaption) {
        m_pcaption->SetFullscreen(GetFullscreen());
    }
}

void EngineWindow::OnCaptionMinimize()
{
    PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void EngineWindow::OnCaptionMaximize()
{
    PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void EngineWindow::OnCaptionFullscreen()
{
    SetFullscreen(true);
}

void EngineWindow::OnCaptionRestore()
{
    if (GetFullscreen()) {
        m_bRestore = true;
    } else {
        PostMessage(WM_SYSCOMMAND, SC_RESTORE);
    }
}

void EngineWindow::OnCaptionClose()
{
    StartClose();
}
