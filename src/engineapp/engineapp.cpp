//////////////////////////////////////////////////////////////////////////////
//
// Engine Application
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class ComponentPoint : public PointValue {
public:
    Number* GetX() { return Number::Cast(GetChild(0)); }
    Number* GetY() { return Number::Cast(GetChild(1)); }

    void SetX(Number* pvalue) { SetChild(0, pvalue); }
    void SetY(Number* pvalue) { SetChild(1, pvalue); }

    ComponentPoint(Number* px, Number* py) :
        PointValue(px, py)
    {
    }

    void Evaluate()
    {
        GetValueInternal() = 
            Point(
                GetX()->GetValue(), 
                GetY()->GetValue()
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Engine Window
//
//////////////////////////////////////////////////////////////////////////////

class EngineAppWindow :
    public EffectWindow, 
    public IIntegerEventSink,
    public ISubmenuEventSink,
    public IMenuCommandSink
{
protected:
    TRef<Camera>                m_pcamera;
    TRef<IImagePopupContainer>  m_ppopupContainer;
    TRef<IIntegerEventSink>     m_pintegerEventSink;
    TRef<ISubmenuEventSink>     m_psubmenuEventSink;
    TRef<IMenuCommandSink>      m_pmenuCommandSink;
    TRef<IMenu>                 m_pmenu;

    TRef<Geo>                   m_ptrailGeo;
    TRef<ModifiableVectorValue> m_pvectorPosition;
    TRef<ModifiableVectorValue> m_pvectorRight;
    float                       m_timePause;
    float                       m_timePauseStart;
    float                       m_timeLast;
    bool                        m_bPaused;

public:
    EngineAppWindow(EngineApp* papp, const ZString& strCommandLine) :
        EffectWindow(
            papp,
            strCommandLine, 
            false,
            WinRect(0, 0, 640, 480)
        )
    {
        //GetEngine()->SetDebugFullscreen(true);
        SetClearColor(Color(0.25, 0.0, 0.5));
        SetUI(false);

        //////////////////////////////////////////////////////////////////////////////
		//
		// Menus
		//
        //////////////////////////////////////////////////////////////////////////////
        /*
		m_pintegerEventSink = IIntegerEventSink::CreateDelegate(this);
		m_psubmenuEventSink = ISubmenuEventSink::CreateDelegate(this);
        m_pmenuCommandSink  = IMenuCommandSink::CreateDelegate(this);
        m_ppopupContainer   = CreateImagePopupContainer(GetEngine(), GetScreenRectValue());

        Window::AddKeyboardInputFilter(m_ppopupContainer);
        */
        //////////////////////////////////////////////////////////////////////////////
        //
        // The Camera
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<Image>          pimageCamera;
        TRef<IKeyboardInput> pkeyboardInput;

        m_pcamera =
            CreateMotionCamera(
                Vector(0, 0, 0),
                GetTime(),
                pimageCamera,
                pkeyboardInput
            );

        AddKeyboardInputFilter(pkeyboardInput);
        m_pcamera->SetZClip(1, 20000);
        m_pcamera->SetFOV(RadiansFromDegrees(65));
        //m_pcamera->SetPerspective(1.0f);

        //////////////////////////////////////////////////////////////////////////////
        //
        // The viewport
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<Viewport> pviewport =
            new Viewport(
                m_pcamera,
                GetRenderRectValue()
            );

        //////////////////////////////////////////////////////////////////////////////
        //
        // An Animated Image
        //
        //////////////////////////////////////////////////////////////////////////////

        /*
        TRef<Image> pimageWiggle =
            new TransformImage(
                new PaneImage(
                    GetEngine(),
                    SurfaceType2D(),
                    false,
                    new BorderPane(
                        0,
                        Color(1, 0, 0),
                        new EdgePane(
                            new ImagePane(
                                new TransformImage(
                                    GetModeler()->LoadImage("crimson.bmp", false),
                                    new TranslateTransform2(
                                        new ComponentPoint(
                                            Multiply(Sin(GetTime()), new Number(20.0f)),
                                            new Number(0.0f)
                                        )
                                    )
                                )
                            )
                        )
                    )
                ),
                new TranslateTransform2(
                    new PointValue(Point(50, 50))
                )
            );
        */

        //////////////////////////////////////////////////////////////////////////////
        //
        // Object Trail
        //
        //////////////////////////////////////////////////////////////////////////////

        /*
        m_pvectorPosition = new ModifiableVectorValue(Vector(0, 0, 0));
        m_pvectorRight    = new ModifiableVectorValue(Vector(0, 0, 0));
        m_ptrailGeo       = 
            CreateTrailGeo(
                Color(1, 0, 0), 
                m_pvectorPosition,
                m_pvectorRight,
                GetTime()
            );

        m_bPaused        = false;
        m_timePause      = 0;
        m_timePauseStart = 0;
        m_timeLast       = 0;
        */

        //////////////////////////////////////////////////////////////////////////////
        //
        // Posters
        //
        //////////////////////////////////////////////////////////////////////////////

        /*
        TRef<PosterImage> pposterImage;

        {
            bool bAnimation;
            TRef<NameSpace> pns = GetModeler()->LoadXFile("globe.x", new Number(0.0f), bAnimation);

            TRef<Geo> pgeo = Geo::Cast(pns->GetValue());

            pposterImage =
                CreatePosterImage(
                    new Viewport(
                        m_pcamera,
                        GetRenderRectValue()
                    )
                );

            pposterImage->AddPosterGeo(pgeo);
        }
        */

        //////////////////////////////////////////////////////////////////////////////
        //
        // The Image
        //
        //////////////////////////////////////////////////////////////////////////////
        /*
        SetImage(
            //Image::GetEmpty()
            new GroupImage(
                pimage,
                //m_ppopupContainer,
                //GetModeler()->LoadImage("crimson.bmp", true),
                pimageCamera,
                //pimageWiggle,
                new GeoImage(
                    GroupGeo::Create(
                        //CreateThing(),
                        //OpenThingMDL("tetra.mdl", Vector(0, 0, 0)),
                        //OpenThingMDL("fig04.mdl", Vector(0, 0, 0)),
                        //Geo::GetIcosahedron(),
                        m_ptrailGeo//,
                        //DebrisGeo::Create(1000)
                    ),
                    pviewport,
                    true
                ),
                StarImage::Create(pviewport, 1000)
                //pposterImage
            )
        );
        */

        //////////////////////////////////////////////////////////////////////////////
        //
        // Load an MDL File
        //
        //////////////////////////////////////////////////////////////////////////////

        TRef<INameSpace> pns   = GetModeler()->GetNameSpace("test");
        TRef<Image>      pimage; CastTo(pimage, pns->FindMember("image"));

        {
            TRef<ZFile> pfile = new ZWriteFile("testt.mdl");
            if (pfile->IsValid()) {
                pns->WriteToTextFile(0, pfile);
            }
        }

        {
            TRef<ZFile> pfile = new ZWriteFile("testb.mdl");
            if (pfile->IsValid()) {
                pns->WriteToBinaryFile(pfile);
            }
        }

        {
            TRef<INameSpace> pns   = GetModeler()->GetNameSpace("testb");
            TRef<Image>     pimage; CastTo(pimage, pns->FindMember("image"));

            SetImage(pimage);

            TRef<INameSpace> pnsNew = 
                GetModeler()->CreateNameSpace(
                    "testb2", 
                    GetModeler()->GetNameSpace("model")
                );

            pnsNew->AddMember("image", (Value*)pimage);

            TRef<ZFile> pfile = new ZWriteFile("testb2.mdl");
            if (pfile->IsValid()) {
                pnsNew->WriteToBinaryFile(pfile);
            }
        }

        /*
        TRef<INameSpace> pns = GetModeler()->GetNameSpace("tetra");

        {
            TRef<ZFile> pfile = new ZWriteFile("tetrat.mdl");
            if (pfile->IsValid()) {
                pns->WriteToTextFile(0, pfile);
            }
        }

        {
            TRef<ZFile> pfile = new ZWriteFile("tetrab.mdl");
            if (pfile->IsValid()) {
                pns->WriteToBinaryFile(pfile);
            }
        }

        {
            TRef<INameSpace> pns = GetModeler()->GetNameSpace("tetrab");
            TRef<Geo>        pgeo; CastTo(pgeo, pns->FindMember("object"));

            SetImage(
                new GroupImage(
                    pimageCamera,
                    new GeoImage(
                        GroupGeo::Create(
                            pgeo,
                            DebrisGeo::Create(1000)
                        ),
                        pviewport,
                        true
                    )
                )
            );
        }
        */

    }

    ~EngineAppWindow()
    {
    }

    void OnClose()
    {
        SetMouseInput(NULL);
        EngineWindow::OnClose();
    }

    TRef<Geo> CreateThing()
    {
        TRef<ThingGeo> pthing = ThingGeo::Create(GetModeler(), GetTime());

        TVector<Vertex> pvertex(3, 3);

        pvertex.Set(0, Vertex(-0.1f, 0.0f,  0.0, 1, 1, 1));
        pvertex.Set(1, Vertex( 0.1f, 0.1f, -2.0, 1, 1, 1));
        pvertex.Set(2, Vertex( 0.1f,-0.1f, -2.0, 1, 1, 1));

        TVector<WORD> pindex(3, 3);
        
        pindex.Set(0, 0);
        pindex.Set(1, 1);
        pindex.Set(2, 2);

        pthing->Load(
            1, 
            Geo::CreateMesh(pvertex, pindex),
            NULL, 
            0
        );

        return pthing;
    }

    TRef<Geo>  OpenThingMDL(const ZString& strFilename, const Vector& vec)
    {
        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strFilename);

        if (pns) {
            TRef<ThingGeo> pthing = ThingGeo::Create(GetModeler(), GetTime());
            pthing->LoadMDL(0, pns, NULL, 0);
            pthing->SetPosition(vec);
            return pthing;
        }

        return NULL;
    }

    bool OnKey(const KeyState& ks)
    {
        if (Window::OnKey(ks)) {
			return true;
		}

        if (ks.bDown) {
			switch(ks.vk)  {
                case 'P':
                    if (!m_bPaused) {
                        m_bPaused        = true;
                        m_timePauseStart = m_timeLast;
                    }
                    break;

				case VK_ESCAPE:
					//
					// The Menu
					//

					m_pmenu =
						CreateMenu(
							GetEngine(),
							GetEngine()->GetBackBuffer()->GetFont(),
                            m_pmenuCommandSink
						);

                    m_pmenu->AddMenuItem(2, "submenu 2", NULL, 0, m_psubmenuEventSink);
                    m_pmenu->AddMenuItem(0, "res up"  );
                    m_pmenu->AddMenuItem(1, "res down");

                    Point point(10, 10);
                    m_ppopupContainer->OpenPopup(m_pmenu, Rect(point, point));

					return true;
			}
        } else {
			switch(ks.vk)  {
                case 'P':
                    m_bPaused    = false;
                    m_timePause += m_timeLast - m_timePauseStart;
                    break;
            }
        }

		return false;
    }

    TRef<IPopup> GetSubMenu(IMenuItem* pitem)
    {
        TRef<IMenu> pmenu =
            CreateMenu(
                GetEngine(),
                GetEngine()->GetBackBuffer()->GetFont(),
                m_pmenuCommandSink
            );

        switch(pitem->GetID()) {
            case 2:
                pmenu->AddMenuItem( 3, "submenu 3", NULL, 0, m_psubmenuEventSink);
                pmenu->AddMenuItem(21, "item 21");
                pmenu->AddMenuItem(22, "item 22");
                pmenu->AddMenuItem(23, "item 23");
                break;

            case 3:
                pmenu->AddMenuItem(31, "item 31");
                pmenu->AddMenuItem(32, "item 32");
                break;
        }

        return pmenu;
    }

    bool OnEvent(IIntegerEventSource* pevent, int value)
    {
        return true;
    }

    void OnMenuCommand(IMenuItem* pitem)
    {
        switch (pitem->GetID()) {
            case 0:
                ChangeGameRectSize(true);
                break;

            case 1:
                ChangeGameRectSize(false);
                break;
        }
    }

    bool IsValid()
    {
        return EngineWindow::IsValid();
    }

    void EvaluateFrame(Time timeNow, float dtime)
    {
        float time = (GetTime()->GetValue() - m_timePause) * 0.1f;
        m_timeLast = time;

        if (!m_bPaused) {
            if (m_ptrailGeo) {
                float width  = 1;
                float angle  = time * 2;
                float d      = time * 4;
                float z      = time / pi;
                float radius = 30 * sin(z);

                m_pvectorPosition->SetValue(
                    Vector(
                        0,
                        radius * cos(d),
                        radius * sin(d)
                    )
                );

                m_pvectorRight->SetValue(
                    Vector(
                        width * cos(angle),
                        width * sin(angle),
                        0
                    )
                );
            }
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Engine Application
//
//////////////////////////////////////////////////////////////////////////////

class EngineAppApp : public EngineApp {
protected:
   TRef<EngineAppWindow> m_pwindow;

public:
    HRESULT Initialize(const ZString& strCommandLine)
    {
        if (SUCCEEDED(EngineApp::Initialize(strCommandLine))) {
            m_pwindow = new EngineAppWindow(this, strCommandLine);

            return S_OK;
        }

        return E_FAIL;
    }

    HRESULT Terminate()
    {
        m_pwindow = NULL;

        return S_OK;
    }
} g_app;
