#include "pch.h"
#include "terrain.h"


//////////////////////////////////////////////////////////////////////////////
//
// TextPanelImage
//
//////////////////////////////////////////////////////////////////////////////

class TextPanelImage : public Image
{
private:
    TRef<Surface>   m_psurface;
    Point           m_pointImgCenter;
    WinPoint        m_pointTextOffset;
    WinPoint        m_pointNextLetter;
    int             m_nTabStop;
    int             m_iNextLetter;
    ZString         m_str;
    
public:

    TextPanelImage(const Point& pointImgCenter, const WinPoint& sizeImg, const WinPoint& pointTextOffset, int nTabStop):
        m_pointTextOffset(pointTextOffset),
        m_pointImgCenter(pointImgCenter),
        m_iNextLetter(0),
        m_nTabStop(nTabStop)
    {
        m_psurface = GetEngine()->CreateSurface(WinRect(0,0,sizeImg.X(),sizeImg.Y()), SurfaceType2D(), NULL);
        m_psurface->FillSurface(Color(0,.8f,0));
        m_psurface->SetTextColor(Color(0,0,1));
    }

    void SetText(const ZString str)
    {
        m_psurface->FillSurface(Color(0,.8f,0));
        m_iNextLetter = 0;
        m_pointNextLetter = m_pointTextOffset;
        m_str = str;
    }

    void Update(float dtime)
    {
        if (m_iNextLetter < m_str.GetLength())
            {
            char c = m_str[m_iNextLetter];
            ZString strOut(&c,1);
            if (c == '\r')
                {
                m_pointNextLetter += WinPoint(
                                -(m_pointNextLetter.X()-m_pointTextOffset.X()),
                                m_psurface->GetFont()->GetTextExtent('X').Y());
                }
            else if (c == '\t')
                {
                m_pointNextLetter.SetX(m_pointTextOffset.X() + m_nTabStop);
                }
            else
                {
                m_psurface->SetTextColor(Color(.1f,.1f,.1f));
                m_psurface->DrawString(m_pointNextLetter, strOut);
                m_pointNextLetter += WinPoint(m_psurface->GetFont()->GetTextExtent(strOut).X(),0);
                }
            m_iNextLetter++;
            }
    }
    
    void Render(Context* pcontext)
    {
        pcontext->SetShadeMode(ShadeModeFlat);
        pcontext->SetLinearFilter(false, true);
        pcontext->SetColor(Color(1,1,1,.25));
        pcontext->SetBlendMode(BlendModeSourceAlpha);
        pcontext->DrawImage3D(m_psurface, true, m_pointImgCenter);
    }

};


//////////////////////////////////////////////////////////////////////////////
//
// MenuImage
//
//////////////////////////////////////////////////////////////////////////////

class MenuImage : public Image
{
private:
    TRef<Surface>   m_psurface;
    int             m_nItems;
    int             m_iSelItem;
    WinPoint        m_ptOffset;    
    WinPoint        m_ptItemSize;    
    float           m_fDelay;
public:

    MenuImage(const ZString& strImage, const WinPoint& ptOffset, int nItems):
        m_ptOffset(ptOffset),
        m_nItems(nItems),
        m_iSelItem(0),
        m_fDelay(0.0f)
    {
        m_psurface = GetModeler()->LoadSurface(strImage, false);
        WinRect rect = m_psurface->GetRect();
        m_ptItemSize.SetX(rect.XSize());
        m_ptItemSize.SetY(rect.YSize()/(nItems*2));
    }


    void Update(bool bUp, bool bDown, float dtime)
    {
        m_fDelay = m_fDelay + dtime;
        if (m_fDelay > .5f && bUp)
            {
            m_iSelItem = (m_iSelItem == 0) ? m_nItems-1 : m_iSelItem-1;
            m_fDelay = 0.0f;
            }
        else if (m_fDelay > .5f && bDown)
            {
            m_iSelItem = (m_iSelItem == m_nItems-1) ? 0 : m_iSelItem+1;
            m_fDelay = 0.0f;
            }
    }
    
    void Render(Context* pcontext)
    {
        for (int iItem = 0; iItem < m_nItems; iItem++)
            {
            if (iItem == m_iSelItem)
                pcontext->DrawImage(m_psurface, 
                                Rect(0, (float)m_ptItemSize.Y()*(m_nItems+iItem), (float)m_ptItemSize.X(), (float)m_ptItemSize.Y()*(m_nItems+iItem+1)), 
                                false, 
                                Point((float)m_ptOffset.X(), (float)(m_ptOffset.Y() + m_ptItemSize.Y()*(m_nItems - iItem)))
                                );                                
            else
                pcontext->DrawImage(m_psurface, 
                                Rect(0, (float)m_ptItemSize.Y()*iItem, (float)m_ptItemSize.X(), (float)m_ptItemSize.Y()*(iItem+1)), 
                                false, 
                                Point((float)m_ptOffset.X(), (float)(m_ptOffset.Y() + m_ptItemSize.Y()*(m_nItems - iItem)))
                                );
            }
    }

    int GetSelection()
    {
        return m_iSelItem;
    }
    
};

//////////////////////////////////////////////////////////////////////////////
//
// WheelGeo
//
//////////////////////////////////////////////////////////////////////////////

class WheelGeo : public WrapGeo
{
private:
    TRef<IntegerEventSourceImpl> m_peventSource;
    TRef<GroupGeo> m_pgroupGeo;
    TRef<RotateTransform> m_protTransform;                        
    float m_radius;
    float m_angleSep;
    short m_cItems;
    short m_iItemSel;
    float m_angleSel;
    float m_angleCur;
  
public:

    WheelGeo(float angleDisk, float angleSep, float radius, float dz) :
        WrapGeo(Geo::GetEmpty()),
        m_radius(radius),
        m_angleSep(angleSep),
        m_cItems(0),
        m_iItemSel(0),
        m_angleSel(0),
        m_angleCur(0)
    {        
        m_pgroupGeo = GroupGeo::Create();
        m_protTransform = new RotateTransform(Vector(0,1,0), 0);
        m_peventSource = new IntegerEventSourceImpl();
            
        SetGeo( 
            new TransformGeo(
                new TransformGeo(
                    new TransformGeo(
                        m_pgroupGeo,
                        m_protTransform
                        ),
                    new RotateTransform(Vector(1,0,0), angleDisk)
                    ),
                new TranslateTransform(Vector(0, radius*sin(angleDisk), -(dz+radius*cos(angleDisk))))
                )
            );

    }

    IIntegerEventSource* GetEventSource()
    {
        return m_peventSource;
    }
    
    void AddItem(Geo* pgeo)
    {
        m_pgroupGeo->AddGeo(
            new TransformGeo(
                new TransformGeo(
                    pgeo,
                    new TranslateTransform(Vector(0, 0, m_radius))
                    ),
                new RotateTransform(Vector(0,1,0), m_angleSep * m_cItems++)
                )
            );
    }

    short GetSelItem()
    {
        return m_iItemSel;
    }

    void SetSelItem(short iItem)
    {
        m_iItemSel = iItem;
        m_angleSel = - m_angleSep * iItem;
        m_peventSource->Trigger(iItem);
    }
    
    void Update(bool bLeft, bool bRight, float dtime)
    {
        if (m_angleSel != m_angleCur)
            {
            float angleMove = m_angleSel - m_angleCur;
            float angleMax = m_angleSep * dtime*4.0f;
            
            if (angleMove > angleMax)
                angleMove = angleMax;
            else if (angleMove < -angleMax)
                angleMove = -angleMax;

            m_angleCur = m_angleCur + angleMove;
            m_protTransform->SetAngle(m_angleCur);
            }
        else if (bLeft && m_iItemSel > 0)
            SetSelItem(m_iItemSel-1);
        else if (bRight && m_iItemSel < m_cItems-1)
            SetSelItem(m_iItemSel+1);
    }

};
    


//////////////////////////////////////////////////////////////////////////////
//
// MissionAction
//
//////////////////////////////////////////////////////////////////////////////

class MissionAction : public IObject
{
public:
    virtual void Trigger() = 0;
};

class CreateDroneAction : public MissionAction
{
private:
    TRef<CadetGameContext> m_pcontext;
    
public:
    CreateDroneAction(CadetGameContext* pcontext) :
        m_pcontext(pcontext)
    {
    }
    
    void Trigger()
    {
        Drone* pdrone = m_pcontext->GetMissionManager()->CreateDrone(c_dtWingman, "loadoutFighterDrone", 2, "Fighter",
                     Vector(0,0,0), m_pcontext->GetShip()->GetCluster());
        pdrone->SetGoal(c_ctDestroy, m_pcontext->GetShip());
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// EnteredRegionHandler
//
//////////////////////////////////////////////////////////////////////////////

class EnteredRegionHandler : public MissionEventHandler
{
private:
    TRef<ImodelIGC>         m_pmodel;
    TRef<Region>            m_pregion;
    TRef<MissionAction>     m_paction;
    bool                    m_bOnceOnly;
    
public:
    EnteredRegionHandler(ImodelIGC* pmodel, Region* pregion, bool bOnceOnly, MissionAction* paction) :
        m_pmodel(pmodel),
        m_pregion(pregion),
        m_paction(paction),
        m_bOnceOnly(bOnceOnly)
    {
    }
    
    bool OnEnteredRegion(ImodelIGC* pmodel, Region* pregion)
    {
        if (pmodel == m_pmodel && pregion == m_pregion)
            {
            m_paction->Trigger();
            if (m_bOnceOnly)
                return false;
            }                   
        return true;            
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// CreateTestStage
//
//////////////////////////////////////////////////////////////////////////////

void CreateTestStage(CadetGameContext* pcontext)
{
    pcontext->GetMissionManager()->AdviseRegion(
            pcontext->GetMissionManager()->GetRegion("region1"),
            pcontext->GetShip()
            );

    pcontext->GetMissionManager()->AddMissionEventHandler(
                    new EnteredRegionHandler(
                            pcontext->GetShip(),
                            pcontext->GetMissionManager()->GetRegion("region1"),
                            true,
                            new CreateDroneAction(pcontext)
                            )
                    );                            
}                    

//////////////////////////////////////////////////////////////////////////////
//
// CadetSceneObjects
//
//////////////////////////////////////////////////////////////////////////////

class ICadetSceneObject : public IObject
{
public:
    virtual bool Update(Time time, float dtime) = 0;
};

template<class ClassType>
class TDelayedCallback : public ICadetSceneObject
{
private:
    typedef void (ClassType::*PFNMember)();

    float m_timeDelay;
    TRef<ClassType> m_pobj;
    PFNMember m_pfn;

public:
    TDelayedCallback(float timeDelay, ClassType* pobj, PFNMember pfn) :
        m_pobj(pobj),
        m_pfn(pfn),
        m_timeDelay(timeDelay)
    {
    }

    bool Update(Time time, float dtime)
    {
        m_timeDelay = m_timeDelay - dtime;
        if (m_timeDelay <= 0)
            {
            (m_pobj->*m_pfn)();
            return false;
            }
        return true;            
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// BaseCadetScene
//
//////////////////////////////////////////////////////////////////////////////

class BaseCadetScene : public GameScene, public IKeyboardInput
{
protected:
    TRef<Image> m_pimage;
    TRef<CadetGameContext> m_pgameContext;
    TList<TRef<ICadetSceneObject> > m_lstSceneObjects;
    
public:

    BaseCadetScene(CadetGameContext* pgameContext, Image* pimage, float dtimeDelay = 1) :
        m_pgameContext(pgameContext),
        m_pimage(pimage)
    {
    }

    TRef<Image> GetSceneImage()
    {
        return m_pimage;
    }

    virtual void Init()
    {
    }
    
    virtual void Term()
    {
    }
    
    virtual bool PreIgcUpdate(Time time, float dtime)
    {
        return false;
    }

    virtual void PostIgcUpdate(Time time, float dtime)
    {
    }

    void InitScene()
    {
        Init();
        GetWindow()->AddKeyboardInputFilter(this);
    }
    
    void TermScene()
    {
        Term();
        m_pimage = NULL;
        GetWindow()->RemoveKeyboardInputFilter(this);
        m_pgameContext = NULL;
    }

    void UpdateScene(Time time, float dtime)
    {
        TList<TRef<ICadetSceneObject> >::Iterator iter(m_lstSceneObjects);
        while (!iter.End())
            {
            if (iter.Value() != NULL)
                {
                if (!iter.Value()->Update(time, dtime))
                    m_lstSceneObjects.Replace(iter.Value(), NULL); // mark for deletion
                }
            iter.Next();
            }

        while (m_lstSceneObjects.Remove(NULL)) {}  // go through and delete
       
        if (PreIgcUpdate(time, dtime) && m_pgameContext)
            {
            AddRef(); // might terminate scene during IGC update
            m_pgameContext->GetMissionIGC()->Update(time);
            if (m_pgameContext) // this will tell us if we terminated
                PostIgcUpdate(time, dtime);
            Release();
            }
    }

    void AddSceneObject(ICadetSceneObject* psceneObject)
    {
        m_lstSceneObjects.PushEnd(psceneObject);
    }

    virtual bool   OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case 'S':
                    Sound::SetPlaySounds( !Sound::GetPlaySounds() );
                    return true;
                case 'Q':
                    GetWindow()->PostMessage(WM_QUIT);
                    return true;
                case 'O':
                    GetWindow()->OutputPerformanceCounters();
                    return true;
                }
            }
		return false;
	}


};


//////////////////////////////////////////////////////////////////////////////
//
// SplashScene
//
//////////////////////////////////////////////////////////////////////////////

class SplashScene : public BaseCadetScene
{
private:
    TRef<MenuImage> m_pmenuImage;
    
public:

    SplashScene(CadetGameContext* pgameContext) :
        BaseCadetScene(pgameContext, Image::GetEmpty())
    {
    }

    void Init()
    {
        /*!!!test
        Sound::DefineLoopingSoundEffect(flightMusicSound, "flightmusic");
        Sound::PlaySoundEffect(flightMusicSound);
        */

        m_pimage = new GroupImage(
                        m_pmenuImage = new MenuImage("cadetmenubmp", WinPoint(340,20), 3)
                        );
    }

    void Term()
    {
        //!!!test Sound::StopSoundEffect(flightMusicSound);
        GetModeler()->UnloadNameSpace("splashbmp");
        GetModeler()->UnloadNameSpace("cadetmenubmp");
    }
    
    bool PreIgcUpdate(Time time, float dtime)
    {
        DIJOYSTATE  dijs;
        Joystick::GetJoystick(&dijs);
        m_pmenuImage->Update(
                (bool)(dijs.rgbButtons[3]!=0), 
                (bool)(dijs.rgbButtons[4]!=0), 
                dtime);    
        if (dijs.rgbButtons[1] || dijs.rgbButtons[2])
            {
            CadetMode cm = (CadetMode)m_pmenuImage->GetSelection();
            m_pgameContext->GetCadetPlay()->SetCadetMode(cm);
            m_pgameContext->LoadMap((cm == cmSinglePlayer) ? "spclusters" : "clusters");
            m_pgameContext->SwitchToScene(CreateLoadoutScene(m_pgameContext));
            }
        return false;
    }
   

};

TRef<GameScene> CreateSplashScene(CadetGameContext* pgameContext)
{
    return new SplashScene(pgameContext);
}





//////////////////////////////////////////////////////////////////////////////
//
// LoadoutScene
//
//////////////////////////////////////////////////////////////////////////////

class LoadoutScene : public BaseCadetScene, public EventTargetContainer<LoadoutScene>
{
private:
    TRef<Camera>            m_pcamera;
    TRef<Viewport>          m_pviewport;
    TRef<WheelGeo>          m_pwheelGeo;
    TList<TRef<IshipIGC> >  m_rgShips;
    bool                    m_bEnableStart;
    
public:

    LoadoutScene(CadetGameContext* pgameContext) :
        BaseCadetScene(pgameContext, Image::GetEmpty()),
        m_bEnableStart(false)
    {
    }

    void AddShip(const ZString strLoadout)
    {
        SideID sideID = (m_pgameContext->GetCadetPlay()->GetCadetMode() == cmJoinSerial) ? 1 : 0;
        TRef<IshipIGC> pship = m_pgameContext->GetMissionManager()
                            ->CreateShip(strLoadout, sideID, "");

        m_rgShips.PushEnd(pship);
        m_pwheelGeo->AddItem(
            new FaceCameraGeo(
                new TransformGeo(
                    pship->GetThingSite()->GetThingGeo(),
                    new RotateTransform(Vector(-.2f,1,.2f), RadiansFromDegrees(210))
                    )
                )
            );
    }

    bool OnShipSel(int i)
    {
        /*ZString str;
        TRef<IhullTypeIGC> phullType = m_rgShips[i]->GetHullType();
        str += "Ship:\t" + ZString(phullType->GetDescription())
                + "\rMass:\t" + ZString((int)phullType->GetMass())
                + "\rLength:\t" + ZString((int)phullType->GetLength()) 
                + "\rSpeed:\t" + ZString((int)phullType->GetMaxSpeed()) 
                + "\rTurn Rate:\t" + ZString((int)phullType->GetMaxTurnRate(c_axisYaw))
                + "\rArmor:\t" + ZString(phullType->GetHitPoints());
        m_ptextPanelImage->SetText(str);*/
        return true;
    }

    void EnableStart()
    {
        m_bEnableStart = true;
    }

    TRef<Image> LoadImage24(const ZString& str)
    {
        HBITMAP hbitmap = GetModeler()->LoadBitmap(str);
        TRef<Surface> psurface24 = GetEngine()->CreateSurface(hbitmap);
        ZVerify(::DeleteObject(hbitmap));

        //
        // Convert the bitmap to 16bpp
        //

        TRef<Surface> psurface =
            GetEngine()->CreateSurface(
                psurface24->GetRect(),
                new PixelFormat(16, 0xf800, 0x07e0, 0x001f, 0x0000)
            );

        psurface->BitBlt(WinPoint(0, 0), psurface24);

        return new ConstantImage(psurface, str);
    }

    void Init()
    {
        //
        // Top and bottom images
        //

        //TRef<Image> pimageTop    = LoadImage24("loadouttop.bmp");
        //TRef<Image> pimageBottom = LoadImage24("loadoutbottom.bmp");

        //
        //
        //

        m_pcamera = new Camera();
        m_pcamera->SetPosition(Vector(0, 0, 0));
        m_pcamera->SetOrientation(Orientation(Vector(0, 0, -1), Vector(0, 1, 0)));
        m_pcamera->SetZClip(1, 10000);
        m_pcamera->SetFOV(RadiansFromDegrees(65));
        m_pviewport = new Viewport(m_pcamera, GetWindow()->GetRenderRectValue());

        TRef<INameSpace> pns = GetWindow()->GetModeler()->GetNameSpace("globe");
        TRef<Geo> pgeoGlobe = Geo::Cast(pns->FindMember("object"));

        m_pwheelGeo = new WheelGeo(
                                RadiansFromDegrees(30), 
                                RadiansFromDegrees(30), 
                                50, 50);
        m_pimage = 
            new GroupImage(
                //GetModeler()->LoadImage("loadoutbmp", true),
                //new TranslateImage(pimageTop, Point(0, 480 - pimageTop->GetBounds().GetRect().YSize())),
                //new TranslateImage(pimageBottom, Point(640 - pimageTop->GetBounds().GetRect().XSize(), 0)),
                new GeoImage(m_pwheelGeo, m_pviewport, true),
                new GeoImage(pgeoGlobe, m_pviewport, false)
            );

        AddEventTarget(OnShipSel, m_pwheelGeo->GetEventSource());
        AddSceneObject(new TDelayedCallback<LoadoutScene>(1.0f, this, EnableStart));

        AddShip("loadoutFighter");
        AddShip("loadoutBomber");
        AddShip("loadoutScarab");
        AddShip("loadoutFox");
    }

    void Term()
    {
        TList<TRef<IshipIGC> >::Iterator iter(m_rgShips);
        while (!iter.End())
            {
            iter.Value()->Terminate();
            iter.Next();
            }

        GetModeler()->UnloadNameSpace("loadoutbmp");
    }

   
    bool PreIgcUpdate(Time time, float dtime)
    {
        DIJOYSTATE  dijs;
        Joystick::GetJoystick(&dijs);
        m_pwheelGeo->Update(
                (bool)(dijs.rgbButtons[5]!=0 || dijs.rgbButtons[10]!=0), 
                (bool)(dijs.rgbButtons[6]!=0 || dijs.rgbButtons[9]!=0), 
                dtime);    

        if ((dijs.rgbButtons[1] || dijs.rgbButtons[2]) && m_bEnableStart)
            {
            TRef<IshipIGC> pship = m_rgShips[m_pwheelGeo->GetSelItem()];
            m_pgameContext->SetShip(pship);
            pship->SetPosition(Vector(.0001f,.0001f,.0001f));
            m_rgShips.Remove(pship);
            m_pgameContext->SwitchToScene(CreateFlightScene(m_pgameContext));
            return false;
            }

        return false;
    }
   


};

TRef<GameScene> CreateLoadoutScene(CadetGameContext* pgamecontext)
{
    return new LoadoutScene(pgamecontext);
}


//////////////////////////////////////////////////////////////////////////////
//
// FlightScene
//
//////////////////////////////////////////////////////////////////////////////

class FlightScene : public BaseCadetScene, public CadetPlaySite
{
private:

    TRef<TerrainTileData>   m_ptileDataRight;
    TRef<TerrainTileData>   m_ptileDataStraight;
    TRef<TerrainMap>        m_pmap;
    TRef<TerrainTile>       m_ptile;

    TRef<TextPanelImage>    m_ptextPanelImage;
    bool                    m_bEnableStart;
    bool                    m_bCreatedHelp;

    enum ViewMode { vmChase, vmCockpit };
    ViewMode                m_viewMode;

    ShipControlStateIGC     m_fireMode;
    
public:

    FlightScene(CadetGameContext* pgameContext) :
        BaseCadetScene(pgameContext, Image::GetEmpty()),
        m_bEnableStart(false),
        m_bCreatedHelp(false),
        m_viewMode(vmChase),
        m_fireMode(oneWeaponIGC)
    {
    }

    void EnableStart()
    {
        m_bEnableStart = true;
    }

    void CreateAttack()
    {

        
        int cAttackers = 0;
        SideID sideID = m_pgameContext->GetShip()->GetSide()->GetObjectID();
        const ModelListIGC* models = m_pgameContext->GetCluster()->GetModels();
        assert (models);
        for (ModelLinkIGC*  l = models->first(); (l != NULL); l = l->next())
            {
            ImodelIGC* pmodel = l->data();
            if (pmodel->GetObjectType() == OT_ship &&
                    pmodel->GetSide()->GetObjectID() != sideID)
                cAttackers++;
            }
        if (cAttackers < 7)
            {
            Drone* pdrone = m_pgameContext->GetMissionManager()->CreateDrone(c_dtWingman, "loadoutFighterDrone", 1, "Fighter",
                     Vector(-1000,0,0), m_pgameContext->GetShip()->GetCluster());
            }                     
        else if (!m_bCreatedHelp)
            {
            m_bCreatedHelp = true;
            Drone* pdrone = m_pgameContext->GetMissionManager()->CreateDrone(c_dtWingman, "loadoutBomber", 0, "Bomber",
                     Vector(-1000,0,0), m_pgameContext->GetShip()->GetCluster());
            }                     
        
        AddSceneObject(new TDelayedCallback<FlightScene>(15.0f, this, CreateAttack));
    }

    void CreateMiner()
    {
        int cFriendlies = 0;
        SideID sideID = m_pgameContext->GetShip()->GetSide()->GetObjectID();
        const ModelListIGC* models = m_pgameContext->GetCluster()->GetModels();
        assert (models);
        for (ModelLinkIGC*  l = models->first(); (l != NULL); l = l->next())
            {
            ImodelIGC* pmodel = l->data();
            if (pmodel->GetObjectType() == OT_ship &&
                    pmodel->GetSide()->GetObjectID() == sideID)
                cFriendlies++;
            }
        if (cFriendlies < 7)
            {
            Drone* pdrone = m_pgameContext->GetMissionManager()->CreateDrone(c_dtMining, "loadoutMinerDrone", 0, "Miner",
                     Vector(700,0,-500), m_pgameContext->GetShip()->GetCluster());
            }                     
        AddSceneObject(new TDelayedCallback<FlightScene>(35.0f, this, CreateMiner));
    }


    void Init()
    {
        m_pimage = m_pgameContext->GetFlightImage();
        m_pgameContext->GetCadetPlay()->SetCadetPlaySite(this);

        if (m_pgameContext->GetCadetPlay()->GetCadetMode() == cmHostSerial ||
                m_pgameContext->GetCadetPlay()->GetCadetMode() == cmHostModem)
            {                   
            HRESULT hr = m_pgameContext->GetCadetPlay()->HostSession();
            ZAssert(SUCCEEDED(hr));
            }
        else if (m_pgameContext->GetCadetPlay()->GetCadetMode() == cmSinglePlayer)
            {
            AddSceneObject(new TDelayedCallback<FlightScene>(15.0f, this, CreateMiner));
            AddSceneObject(new TDelayedCallback<FlightScene>(15.0f, this, CreateAttack));
            AddSceneObject(new TDelayedCallback<FlightScene>(1.0f, this, EnableStart));
            }
        else
            {
            HRESULT hr = m_pgameContext->GetCadetPlay()->JoinSession(
                            m_pgameContext->GetMissionIGC());
            ZAssert(SUCCEEDED(hr));
            }

        
        /*
        TRef<Geo> pgeo = GetModeler()->LoadGeo("canyon");
        m_pgameContext->GetFlightImage()->AddGeo(
                new TransformGeo(
                        pgeo,
                        new ScaleTransform(10)
                        )
                );
        */

        Sound::PlaySoundEffect(inSound);
    }

    void Term()
    {
        if (m_ptextPanelImage)
            m_pgameContext->GetFlightImage()->RemoveImage(m_ptextPanelImage);

        m_pgameContext->GetCadetPlay()->SetCadetPlaySite(NULL);

        Sound::StopSoundEffect(inSound);
        Sound::StopSoundEffect(outSound);
        Sound::StopSoundEffect(turnSound);
        Sound::StopSoundEffect(accelSound);
    }

    bool PreIgcUpdate(Time time, float dtime)
    {
        if (m_pgameContext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
            m_pgameContext->GetCadetPlay()->ReceiveMessages();

        if (m_ptextPanelImage)
            m_ptextPanelImage->Update(dtime);
        
        DIJOYSTATE  dijs;
        Joystick::GetJoystick(&dijs);

        ControlData     controls;
        controls.Reset();

        Time timeLastFrame = time - dtime;

        int buttonsM = m_pgameContext->GetShip()->GetStateM() & (noCollisionDetectionIGC |
                                                                    coastButtonIGC |
                                                                    cloakActiveIGC);
        if (dijs.rgbButtons[2] && m_bEnableStart) //Start
            {
            m_pgameContext->SwitchToScene(CreateLoadoutScene(m_pgameContext));
            return false;
            }

        if (dijs.rgbButtons[3])
            {
            if (m_viewMode != vmCockpit)
                {
                m_viewMode = vmCockpit;
                m_pgameContext->GetShip()->GetThingSite()->SetVisible(c_ucRenderNone);
                Sound::StopSoundEffect(outSound);
                Sound::PlaySoundEffect(inSound);
                }
            }
        else if (dijs.rgbButtons[4])
            {
            if (m_viewMode != vmChase)
                {
                m_viewMode = vmChase;
                m_pgameContext->GetShip()->GetThingSite()->SetVisible(c_ucRenderAll);
                Sound::StopSoundEffect(inSound);
                Sound::PlaySoundEffect(outSound);
                }
            }

        if (dijs.rgbButtons[5])
            m_fireMode = oneWeaponIGC;
        if (dijs.rgbButtons[6])
            m_fireMode = allWeaponsIGC;


        if (dijs.rgbButtons[0]) //B
            buttonsM |= m_fireMode;
        if (dijs.rgbButtons[1])
            buttonsM |= forwardButtonIGC; //A   
        if (dijs.rgbButtons[7])
            buttonsM |= afterburnerButtonIGC; //Y
        if (dijs.rgbButtons[8])
            buttonsM |= missileFireIGC; //X
        if (dijs.rgbButtons[9])
            controls.jsValues[c_axisRoll] = -1.0f;
        if (dijs.rgbButtons[10])
            controls.jsValues[c_axisRoll] = 1.0f;

        controls.jsValues[c_axisYaw] = -dijs.lX/100.0f;
        controls.jsValues[c_axisPitch] = -dijs.lY/100.0f;

        m_pgameContext->GetShip()->SetControls(controls);
        m_pgameContext->GetShip()->SetStateBits(timeLastFrame, buttonsMaskIGC | weaponsMaskIGC | selectedWeaponMaskIGC | missileFireIGC, buttonsM);

        //Play sound effects appropriate for the maneuvers of the player's ship.
        float   turn = (controls.jsValues[c_axisYaw] * controls.jsValues[c_axisYaw] +
                        controls.jsValues[c_axisPitch] * controls.jsValues[c_axisPitch] +
                        controls.jsValues[c_axisRoll] * controls.jsValues[c_axisRoll]);
        if (turn > 0.05f)
            Sound::PlaySoundEffect(turnSound);
        else
            Sound::StopSoundEffect(turnSound);

        float thrust = m_pgameContext->GetShip()->GetHullType()->GetThrust();
        if (m_pgameContext->GetShip()->GetEngineVector().LengthSquared() > 0.10f * thrust * thrust)
            Sound::PlaySoundEffect(accelSound);
        else
            Sound::StopSoundEffect(accelSound);

        return true;
    }

    void PostIgcUpdate(Time time, float dtime)
    {
        Orientation cameraOrientation = m_pgameContext->GetShip()->GetOrientation();
        Vector cameraPosition;
        if (m_viewMode == vmChase)
            {
            float vel2 = m_pgameContext->GetShip()->GetVelocity().LengthSquared();
            if (vel2 > 0.0f)
                    {
                    float maxVel = m_pgameContext->GetShip()->GetHullType()->GetMaxSpeed();
                    cameraPosition = m_pgameContext->GetShip()->GetPosition() 
                        - m_pgameContext->GetShip()->GetVelocity().Normalize() * (.4f * m_pgameContext->GetShip()->GetRadius() * (sqrt(vel2) / maxVel))
                        - cameraOrientation.GetForward() * (3.0f * m_pgameContext->GetShip()->GetRadius())
                        + cameraOrientation.GetUp() * (.7f * m_pgameContext->GetShip()->GetRadius());
                    }
            else
                    cameraPosition =  m_pgameContext->GetShip()->GetPosition() 
                        - cameraOrientation.GetForward() * (3.0f * m_pgameContext->GetShip()->GetRadius())
                        + cameraOrientation.GetUp() * (.7f * m_pgameContext->GetShip()->GetRadius());
            }
        else
            {
            cameraPosition = m_pgameContext->GetShip()->GetPosition();
            }

        m_pgameContext->GetFlightImage()->SetCameraOrientation(cameraOrientation);
        m_pgameContext->GetFlightImage()->SetCameraPosition(cameraPosition);

        if (m_pgameContext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
                m_pgameContext->GetCadetPlay()->SendShipUpdateMessages(
                            m_pgameContext->GetMissionIGC());
        
        //detect collisions with map
        static bool hitLast = false;
        if (m_pmap)
            {
            if (hitLast && !m_pmap->HitTest(m_pgameContext->GetShip()->GetPosition()))
                hitLast = false;
            else if (!hitLast && m_pmap->HitTest(m_pgameContext->GetShip()->GetPosition()))
                {
                hitLast = true;
                Sound::PlaySoundEffect(myHullHitSound);
                }
            }

    }

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case '0':
                    {
                    // load map
                    m_ptileDataStraight = TerrainTileData::Create(GetStraight());
                    m_ptileDataRight = TerrainTileData::Create(GetRight());
                    m_pmap = TerrainMap::Create();
                    m_ptile = m_pmap->SetRoot(m_pmap->CreateTile(m_ptileDataStraight));
                    m_pgameContext->GetFlightImage()->AddGeo(m_pmap->GetGeo());
                    if (m_pgameContext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
                        {
                        TerrainChangeMessage msg;
                        msg.msgID = TerrainChangeMessageID;
                        msg.cbMsg = sizeof(TerrainChangeMessage);
                        msg.tc = tcInit;
                        m_pgameContext->GetCadetPlay()->SendMessage(&msg);
                        }
                    return true;
                    }
                case '1':
                    {
                    m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataStraight));
                    if (m_pgameContext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
                        {
                        TerrainChangeMessage msg;
                        msg.msgID = TerrainChangeMessageID;
                        msg.cbMsg = sizeof(TerrainChangeMessage);
                        msg.tc = tcStraight;
                        m_pgameContext->GetCadetPlay()->SendMessage(&msg);
                        }
                    return true;
                    }
                case '2':
                    {
                    m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataRight));
                    if (m_pgameContext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
                        {
                        TerrainChangeMessage msg;
                        msg.msgID = TerrainChangeMessageID;
                        msg.cbMsg = sizeof(TerrainChangeMessage);
                        msg.tc = tcTurn;
                        m_pgameContext->GetCadetPlay()->SendMessage(&msg);
                        }
                    return true;
                    }
                case 'C':
                    {
                    ChatMessage chatMsg;
                    chatMsg.msgID = ChatMessageID;
                    chatMsg.cbMsg = sizeof(ChatMessage);
                    strcpy(chatMsg.szMsg, "This is a chat.");
                    m_pgameContext->GetCadetPlay()->SendMessage(&chatMsg);
                    return true;
                    }
                case 'M':
                    {
                    Drone* pdrone = m_pgameContext->GetMissionManager()->CreateDrone(c_dtMining, "loadoutMinerDrone", 2, "Miner",
                                 Vector::RandomPosition(1000), m_pgameContext->GetShip()->GetCluster());
                    return true;
                    }
                }
            }
		return BaseCadetScene::OnKey(pprovider, ks, fForceTranslate);
	}

    void ShowText(const ZString& str)
    {
        if (!m_ptextPanelImage)
            {
            m_ptextPanelImage = new TextPanelImage(Point(320,100), WinPoint(256,128), WinPoint(10,10), 80);
            m_pgameContext->GetFlightImage()->AddImage(m_ptextPanelImage);
            }
        m_ptextPanelImage->SetText(str);
    }

    void HideText()
    {
        if (m_ptextPanelImage)
            {
            m_pgameContext->GetFlightImage()->RemoveImage(m_ptextPanelImage);
            m_ptextPanelImage = NULL;
            }
    }

    void OnAppMessage(CadetMessage* pmsg)
    {
        switch (pmsg->msgID)
            {
            case ChatMessageID:
                {
                ChatMessage* pchatMsg = (ChatMessage*)pmsg;
                debugf(pchatMsg->szMsg);
                debugf("\r\n");
                break;
                }
            case TerrainChangeMessageID:
                {
                TerrainChangeMessage* pterrainMsg = (TerrainChangeMessage*)pmsg;
                switch (pterrainMsg->tc)
                    {
                    case tcInit:
                        {
                        m_ptileDataStraight = TerrainTileData::Create(GetStraight());
                        m_ptileDataRight = TerrainTileData::Create(GetRight());
                        m_pmap = TerrainMap::Create();
                        m_ptile = m_pmap->SetRoot(m_pmap->CreateTile(m_ptileDataStraight));
                        m_pgameContext->GetFlightImage()->AddGeo(m_pmap->GetGeo());
                        break;
                        }
                    case tcStraight:
                        {
                        m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataStraight));
                        break;
                        }
                    case tcTurn:
                        {
                        m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataRight));
                        break;
                        }
                    }
                break;
                }
            default:
                {
                 m_pgameContext->GetCadetPlay()->HandleIGCMessage(
                            m_pgameContext->GetMissionIGC(),
                            pmsg);
                }                            
            }    
    }

};

TRef<GameScene> CreateFlightScene(CadetGameContext* pgameContext)
{
    return new FlightScene(pgameContext);
}


