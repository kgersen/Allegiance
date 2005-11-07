#include "pch.h"
#include "sounds.h"

//////////////////////////////////////////////////////////////////////////////
//
// Artwork
//
//////////////////////////////////////////////////////////////////////////////

#define AWF_EXPLOSION_00                        "dcexp20bmp"
#define AWF_EXPLOSION_01                        "dcexp22bmp"
#define AWF_EXPLOSION_02                        "dcexp23bmp"
#define AWF_EXPLOSION_03                        "dcexp24bmp"
#define AWF_SHOCKWAVE                           "fx18bmp"
#define AWF_SHIELD_FLARE                        "lens"
#define AWF_SHIELD_FLARE_TEXTURE                "dcfx09bmp"


//////////////////////////////////////////////////////////////////////////////
//
// Indicator Image
//
//////////////////////////////////////////////////////////////////////////////

class IndicatorImage : public WrapImage 
{

    TRef<Surface> m_psurfaceDirection;
    TRef<Surface> m_psurfaceCenter;
    TRef<Surface> m_psurfaceLead;
    TRef<CadetGameContext> m_pgameContext;
    
public:
    IndicatorImage(Modeler* pmodeler, Viewport* pviewport, CadetGameContext* pgameContext) :
        WrapImage(Image::GetEmpty(), pviewport),
        m_pgameContext(pgameContext)
    {
        m_psurfaceDirection   = pmodeler->LoadSurface("dcdirectionbmp", true);
        m_psurfaceCenter      = pmodeler->LoadSurface("dcdirectionnewbmp", true);
        m_psurfaceLead        = pmodeler->LoadSurface("dcleadbmp", true);

        m_psurfaceDirection->SetColorKey(Color(0, 0, 0));
        m_psurfaceCenter   ->SetColorKey(Color(0, 0, 0));
        m_psurfaceLead     ->SetColorKey(Color(0, 0, 0));
    }

    void SetViewport(Viewport* pvalue) { SetChild(1, pvalue); }

    Viewport*  GetViewport() { return Viewport::Cast(GetChild(1));  }
    Camera*    GetCamera()   { return GetViewport()->GetCamera();   }
    RectValue* GetViewRect() { return GetViewport()->GetViewRect(); }

    void RenderDirectionIndicator(Context* pcontext)
    {
        const Vector& myVelocity = m_pgameContext->GetShip()->GetVelocity();
        int speed = (int)(myVelocity.Length() + 0.5f);

        if (speed != 0)
        {
            Point pointNDC;

            if (GetCamera()->TransformDirectionToNDC(myVelocity, pointNDC)) {
                const Rect& rect       = GetViewRect()->GetValue();
                      Point pointImage = rect.TransformNDCToImage(pointNDC);

                if (rect.Inside(pointImage)) {
                    pcontext->DrawImage(m_psurfaceDirection, true, pointImage);
                }
            }
        }
    }

    void RenderCenterIndicator(Context* pcontext)
    {
        pcontext->DrawImage(m_psurfaceCenter, true, GetViewRect()->GetValue().Center());
    }

    void RenderLeadIndicator(Context* pcontext)
    {
        //ImodelIGC* targetModel = m_pgameContext->GetShip()->GetCommandTarget(c_cmdCurrent);
        ImodelIGC* targetModel = FindClosestTarget();
        
        if (
               targetModel
            && (targetModel->GetCluster() == m_pgameContext->GetCluster())
            && m_pgameContext->GetShip()->CanSee(targetModel)
        ) {
            IweaponIGC* w = (IweaponIGC*)m_pgameContext->GetShip()->GetMountedPart(ET_Weapon, 0);
            if (w) {
                Vector vecLeadPosition;

                float t = solveForLead(m_pgameContext->GetShip(), targetModel, w, &vecLeadPosition);
                if (t <= w->GetLifespan()) {
                    Point pointNDC;
                    if (GetCamera()->TransformDirectionToNDC(vecLeadPosition, pointNDC)) {
                        pcontext->DrawImage(
                            m_psurfaceLead,
                            true,
                            GetViewport()->TransformNDCToImage(pointNDC)
                        );
                    }
                }
            }
        }
    }

    void Render(Context* pcontext)
    {
        if (m_pgameContext->GetShip())
        {
            //
            // Rendering attributes
            //

            pcontext->SetShadeMode(ShadeModeFlat);
            pcontext->SetColor(Color(1, 1, 1));
            pcontext->SetLinearFilter(false, true);
            pcontext->Clip(GetViewRect()->GetValue());

            //
            // Draw the indicators
            //

            RenderCenterIndicator(pcontext);
            RenderDirectionIndicator(pcontext);
            RenderLeadIndicator(pcontext);
        }
    }

    ImodelIGC* FindClosestTarget()
    {
        if (!m_pgameContext->GetCluster())
            return NULL;

        SideID sideID = m_pgameContext->GetShip()->GetSide()->GetObjectID();
        const ModelListIGC* models = m_pgameContext->GetCluster()->GetModels();
        assert (models);
        TRef<ImodelIGC> pmodelClosest;
        float lengthSquaredClosest = 1;
        
        for (ModelLinkIGC*  l = models->first(); (l != NULL); l = l->next())
            {
            ImodelIGC*        pmodel = l->data();
            if (pmodel != m_pgameContext->GetShip() &&
                    pmodel->GetObjectType() == OT_ship &&
                    pmodel->GetSide()->GetObjectID() != sideID)
                {
                const Vector&   positionModel = pmodel->GetPosition();
                Point           pointNDC;
                if (GetCamera()->TransformLocalToNDC(positionModel, pointNDC))
                    {
                    float lengthSquared = pointNDC.LengthSquared();
                    if (lengthSquared < lengthSquaredClosest)
                        {
                        lengthSquaredClosest = lengthSquared;
                        pmodelClosest = pmodel;
                        }
                    }
                }
            }

        return pmodelClosest;        
    }

};


//////////////////////////////////////////////////////////////////////////////
//
// BillboardType
//
//////////////////////////////////////////////////////////////////////////////

class BillboardType {
public:
    TRef<AnimatedImage> m_pimage;
    float               m_lifespan;

    BillboardType() 
    {
    } 

    BillboardType(Modeler* pmodeler, const char* textureName, float lifespan, int nRows, int nCols) :
        m_lifespan(lifespan)
    {
        TRef<Surface> psurface = pmodeler->LoadSurface(textureName, true);

        if (psurface) {
            m_pimage = new AnimatedImage(new Number(0.0f), psurface, nRows, nCols);
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


//////////////////////////////////////////////////////////////////////////////
//
// Timed Billboard Geo
//
//////////////////////////////////////////////////////////////////////////////

class TimedBillboardGeo : public Geo, public IGeoCallback {
private:
    Vector          m_vecPosition;
    float           m_angle;
    float           m_lifespan;
    float           m_timeStart;
    float           m_scale;
    TRef<Image>     m_pimage;

    Number* GetTime() { return Number::Cast(GetChild(0)); }

public:
    TimedBillboardGeo(
        Number* ptime,
        const Vector& vecPosition,
        float scale,
        AnimatedImage* pimageArg,
        float lifespan
    ) :
        Geo(ptime),
        m_vecPosition(vecPosition),
        m_timeStart(ptime->GetValue()),
        m_lifespan(lifespan),
        m_scale(scale),
        m_angle(random(0, 2 * pi))
    {
        m_pimage =
             new AnimatedImage(
                 Divide(
                     Subtract(ptime, ptime->MakeConstant()),
                     new Number(lifespan)
                 ),
                 pimageArg
             );
    }

    void Evaluate()
    {
        if (GetTime()->GetValue() - m_timeStart > m_lifespan) {
            SetEmpty();
        }
    }

    void Render(Context* pcontext)
    {
        pcontext->DrawCallbackGeo(this);
    }

    void RenderCallback(Context* pcontext)
    {
        pcontext->SetBlendMode(BlendModeAdd);
        pcontext->SetColor(Color(1, 1, 1));
        pcontext->SetZWrite(false);

        pcontext->PreTranslate(m_vecPosition);
        pcontext->SetFaceCameraTransform();
        pcontext->PreRotate(Vector(0, 0, 1), m_angle);
        pcontext->PreScale(m_scale);

        // !!! animate alpha
        m_pimage->Update();
        pcontext->SetTexture(m_pimage->GetSurface());

        Geo::GetWhiteEmissiveSquare()->Render(pcontext);
    }

    ZString GetFunctionName()
    {
        return "TimedBillboardGeo";
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Forward Declarations
//
//////////////////////////////////////////////////////////////////////////////

class CadetFlightImageImpl;
TRef<ClusterSite> CreateClusterSite(Modeler* pmodeler, Number* ptime, Viewport* pviewport, CadetFlightImageImpl* pflightImg);
TRef<ThingSite> CreateThingSite(ImodelIGC* pModel, CadetFlightImageImpl* pflightImg);

class ThingSitePriv : public ThingSite
{
public:
    virtual void UpdateScreenPosition(
                    const           Point& pointScreenPosition,
                    float           fScreenRadius,
                    float           distanceToEdge,
                    unsigned char   ucRadarState
            ) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// CadetFlightImageImpl
//
//////////////////////////////////////////////////////////////////////////////

class CadetFlightImageImpl : public CadetFlightImage
{
private:

    // Context
    TRef<CadetGameContext> m_pcontext;
    TRef<IclusterIGC> m_pcluster;
    
    // camera and viewports
    TRef<Camera>            m_pcameraPosters;
    TRef<Camera>            m_pcamera;
    TRef<Viewport>          m_pviewport;
    TRef<Viewport>          m_pviewportPosters;
    
    // images
    TRef<GroupImage>        m_pgroupTopImages;
    TRef<GroupImage>        m_pgroupImage;
    TRef<GeoImage>          m_pimageEnvironment;
    TRef<GroupGeo>          m_pgroupGeo;
    TRef<WrapGeo>           m_pwrapGeoScene;
//    TRef<RadarImage>        m_pradarImage;
    
    // resources
    TMap<ZString, TRef<AnimatedImage> > m_mapAnimatedImages;
    BillboardType                       m_types[4];
    TRef<AnimatedImage>                 m_pimageFlare;
    TRef<Geo>                           m_pgeoFlares[3];
    TVector<TRef<AnimatedImage> >       m_pimageExplosions;
    TRef<Image>                         m_pimageShockWave;

public:

    CadetFlightImageImpl(CadetGameContext* pcontext) : 
        m_pcontext(pcontext)
    {
        TRef<Modeler> pmodeler = GetModeler();
        m_types[0] = BillboardType(pmodeler, AWF_EXPLOSION_00, 1.38f,  2, 8);
        m_types[1] = BillboardType(pmodeler, AWF_EXPLOSION_01, 1.15f,  3, 8);
        m_types[2] = BillboardType(pmodeler, AWF_EXPLOSION_02, 1.80f,  4, 8);
        m_types[3] = BillboardType(pmodeler , AWF_EXPLOSION_03, 1.03f, 2, 8);

        AddExplosionImage(AWF_EXPLOSION_00, 2, 8);
        AddExplosionImage(AWF_EXPLOSION_01, 3, 8);
        AddExplosionImage(AWF_EXPLOSION_02, 4, 8);
        AddExplosionImage(AWF_EXPLOSION_03, 2, 8);

        TRef<Surface> psurfaceFlare = pmodeler->LoadSurface(AWF_SHIELD_FLARE_TEXTURE, true);
        ZAssert(psurfaceFlare);
        m_pimageFlare = new AnimatedImage(GetWindow()->GetTime(), psurfaceFlare);

         TRef<INameSpace> pns = pmodeler->GetNameSpace(AWF_SHIELD_FLARE);
         if (pns) {
             CastTo(m_pgeoFlares[0], pns->FindMember("lens30"));
             CastTo(m_pgeoFlares[1], pns->FindMember("lens60"));
             CastTo(m_pgeoFlares[2], pns->FindMember("lens90"));
         }

        m_pimageShockWave = pmodeler->LoadImage(AWF_SHOCKWAVE, true);


        //
        // The Camera
        //
        m_pcamera = new Camera();
        m_pcamera->SetPosition(Vector(0, 0, 5));
        m_pcamera->SetZClip(1, 10000);
        m_pcamera->SetFOV(RadiansFromDegrees(65));
        m_pviewport = new Viewport(m_pcamera, GetWindow()->GetRenderRectValue());
    
        m_pcameraPosters = new Camera();
        m_pcameraPosters->SetZClip(5, 20000);
        m_pcameraPosters->SetFOV(RadiansFromDegrees(50));
        m_pviewportPosters = new Viewport(m_pcameraPosters, GetWindow()->GetRenderRectValue());

        m_pgroupGeo = GroupGeo::Create();
        m_pgroupGeo->AddGeo(DebrisGeo::Create(1000));
        m_pwrapGeoScene = new WrapGeo(Geo::GetEmpty());
        m_pgroupGeo->AddGeo(m_pwrapGeoScene);
        
        m_pimageEnvironment = new GeoImage(Geo::GetEmpty(), m_pviewportPosters, false);

        //
        // Create a group
        //

        m_pgroupImage = new GroupImage();
        m_pgroupTopImages = new GroupImage();
        m_pgroupImage->AddImage(m_pgroupTopImages);
        m_pgroupImage->AddImage(new IndicatorImage(GetModeler(), m_pviewport, m_pcontext));
//        m_pgroupImage->AddImage( m_pradarImage = RadarImage::Create(GetModeler(), m_pviewport, m_pcontext));
//        m_pradarImage->SetRadarLOD(RadarImage::radarLOD2);
        m_pgroupImage->AddImage(CreateMuzzleFlareImage(GetModeler(), GetWindow()->GetTime()));
        m_pgroupImage->AddImage(CreateLensFlareImage(GetModeler(), m_pviewportPosters));
        m_pgroupImage->AddImage( new GeoImage(m_pgroupGeo, m_pviewport, true));
        m_pgroupImage->AddImage(StarImage::Create(m_pviewportPosters, 500));
        m_pgroupImage->AddImage(m_pimageEnvironment);

        SetImage(m_pgroupImage);  

        ThingGeo::SetShowTrails(false);
    }

    TRef<IshipIGC> GetShip()
    {
        return m_pcontext->GetShip();
    }

    TRef<IclusterIGC> GetCluster()
    {
        return m_pcluster;
    }

    void SetCluster(IclusterIGC* pcluster)
    {
        m_pcluster = pcluster;
        m_pwrapGeoScene->SetGeo(pcluster->GetClusterSite()->GetGroupScene());
    }
    
    void SetCameraPosition(const Vector& vec)
    {
        m_pcamera->SetPosition(vec);
    }
    
    void SetCameraOrientation(const Orientation& orientation)
    {
        m_pcamera->SetOrientation(orientation);
        m_pcameraPosters->SetOrientation(orientation);
    }

    void AddImage(Image* pimage)
    {
        m_pgroupTopImages->AddImage(pimage);
    }

    void RemoveImage(Image* pimage)
    {
        m_pgroupTopImages->RemoveImage(pimage);
    }

    void AddGeo(Geo* pgeo)
    {
        m_pgroupGeo->AddGeo(pgeo);
    }

    void RemoveGeo(Geo* pgeo)
    {
        m_pgroupGeo->RemoveGeo(pgeo);
    }
    
    //////////////////////////////////////////////////
    // Callbacks for ClusterSite and ThingSite

    void AddExplosionImage(const ZString& str, int nRows = 0, int nCols = 0)
    {
        TRef<Surface> psurface = GetModeler()->LoadSurface(str, true);
        ZAssert(psurface);
        if (psurface) 
            {
            m_pimageExplosions.PushEnd(new AnimatedImage(new Number(0.0f), psurface, nRows, nCols));
            GetModeler()->UnloadNameSpace(str);            
            }
    }

    TRef<AnimatedImage> LoadAnimatedImage(Number* ptime, const ZString& str)
    {
        TRef<AnimatedImage> pimage;

        if (m_mapAnimatedImages.Find(str, pimage)) {
            return new AnimatedImage(ptime, pimage);
        } else {
            TRef<Surface> psurface = GetModeler()->LoadSurface(str, true);

            if (psurface == NULL) {
                return NULL;
            }

            pimage = new AnimatedImage(ptime, psurface);

            m_mapAnimatedImages.Set(str, pimage);

            return pimage;
        }
    }

    TVector<TRef<AnimatedImage> > GetExplosionImages()
    {
        return m_pimageExplosions;
    }

    TRef<Image> GetShockWaveImage()
    {
        return m_pimageShockWave;
    }

    TRef<AnimatedImage> GetFlareImage()
    {
        return m_pimageFlare;
    }

    void SetPosterGeo(Geo* pgeo)
    {
        m_pimageEnvironment->SetGeo(pgeo);
    }

    BillboardType* GetBillboardType(int iType)
    {
        return &m_types[iType];
    }

    void AddMuzzleFlare(const Vector& vecEmissionPoint, float duration)
    {
    }

    TRef<Geo> GetGeoFlares(int iFlare)
    {
        return m_pgeoFlares[iFlare];
    }

    void Render(Context* pcontext)
    {
        //Update();
        WrapImage::Render(pcontext);
    }
    
    void Update()
    {

        if (!m_pcontext->GetCluster())
            return;

        const ModelListIGC* models = m_pcontext->GetCluster()->GetModels();
        assert (models);
    
        for (ModelLinkIGC*  l = models->first(); (l != NULL); l = l->next())
            {
            ImodelIGC*        pmodel = l->data();
            if (pmodel->GetObjectType() == OT_ship)
                {
                IshipIGC* pship = (IshipIGC*)pmodel;
                if (pship->GetStateM() & miningMaskIGC) 
                    pship->GetThingSite()->ActivateBolt();
                else
                    pship->GetThingSite()->DeactivateBolt();
                }
            }                    

  /*
            const ModelListIGC* models = m_pcontext->GetCluster()->GetPickableModels();
            assert (models);

            Rect rectImage(0,0,640,480);
            double   angleToPixels = double(rectImage.Size().X()) / double(m_pcamera->GetFOV());

            Point pointCenter = rectImage.Center();
            rectImage.Offset(-pointCenter);
            static const float c_flBorderSide = 10.0f;
            rectImage.Expand(-c_flBorderSide);

           
            const Vector&   positionCamera = m_pcamera->GetPosition();

            for (ModelLinkIGC*  l = models->first(); (l != NULL); l = l->next())
            {
                ImodelIGC*        pmodel = l->data();
                if (((pmodel != m_pcontext->GetShip()) || pmodel->GetVisibleF()))
                {
                    ObjectType        type   = pmodel->GetObjectType();
                    ThingSitePriv*    psite; CastTo(psite, pmodel->GetThingSite());
                    ThingGeo*         pthing = psite->GetThingGeo();

                    unsigned char     renderMask = c_ucRenderAll;

                    //
                    // Update screen positions for ships
                    //
                    {
                        float           fScreenRadius;
                        float           distanceToEdge;

                        Point           pointScreenPosition;
                        unsigned char   ucRadarState;

                        const Vector& positionModel = pmodel->GetPosition();

                        {
                            Point pointNDC;
                            if (m_pcamera->TransformLocalToNDC(positionModel, pointNDC))
                            {
                                //
                                // Blip is in front of ship
                                //
                                pointScreenPosition = pointNDC * (rectImage.XSize() * 0.5f);

                                ucRadarState = rectImage.Inside(pointScreenPosition) ? c_ucRadarOnScreenLarge : c_ucRadarOffScreen;
                            }
                            else
                                ucRadarState = c_ucRadarOffScreen;
                        }

                        if (ucRadarState == c_ucRadarOffScreen)
                        {
                            fScreenRadius = 14.0f;
                            distanceToEdge = 0.0f;

                            //
                            // Direction from eye to object
                            //

                            Vector vecDirection = m_pcamera->TransformLocalToEye(positionModel);
                            vecDirection.SetNormalize();
     
                            //
                            // Get the components
                            //

                            float x = vecDirection.X();
                            float y = vecDirection.Y();
                    
                            if (x == 0.0f)
                            {
                                y = (y <= 0.0f) ? rectImage.YMin() : rectImage.YMax();
                            }
                            else if (y == 0.0f)
                            {
                                x = (x <= 0.0f) ? rectImage.XMin() : rectImage.XMax();
                            }
                            else
                            {
                                float   tX = ((x > 0.0f) ? rectImage.XMax() : rectImage.XMin()) / x;
                                float   tY = ((y > 0.0f) ? rectImage.YMax() : rectImage.YMin()) / y;
                                float   tMin = (tX < tY) ? tX : tY;

                                x *= tMin;
                                y *= tMin;
                            }

                            pointScreenPosition.SetX(x);
                            pointScreenPosition.SetY(y);

                        }
                        else
                        {
                            double  sinAlpha = psite->GetRadius() / (positionModel - positionCamera).Length();
                            fScreenRadius = (sinAlpha < 1.0)
                                            ? float(angleToPixels * asin(sinAlpha))      //hack ... 480/(50*pi/180) * 2asin(r/d)
                                            : 640.0f;

                            distanceToEdge = (pointScreenPosition.X() >= 0.0f)
                                             ? (rectImage.XMax() - pointScreenPosition.X())
                                             : (pointScreenPosition.X() - rectImage.XMin());
                            {
                                float   d = (pointScreenPosition.Y() >= 0.0f)
                                            ? (rectImage.YMax() - pointScreenPosition.Y())
                                            : (pointScreenPosition.Y() - rectImage.YMin());
                                if (d < distanceToEdge)
                                    distanceToEdge = d;
                            }

                            if (fScreenRadius < 14.0f)
                            {
                                if ((pmodel != m_pcontext->GetShip()) &&
                                    ((fScreenRadius < 1.0f) ||
                                     CommandCamera(m_cm)))
                                {
                                    ucRadarState = c_ucRadarOnScreenSmall;
                                    renderMask = c_ucRenderTrail;
                                }

                                fScreenRadius = 14.0f;
                            }
                        }

                        psite->UpdateScreenPosition(
                            pointScreenPosition,
                            fScreenRadius,
                            distanceToEdge,
                            ucRadarState
                        );
                    }

                    if (type == OT_ship)
                    {
                        IshipIGC* pship; CastTo(pship, pmodel);

                        IafterburnerIGC*    afterburner = (IafterburnerIGC*)(pship->GetMountedPart(ET_Afterburner, 0));
                        float power = afterburner ? afterburner->GetPower() : 0.0f;

                        ThingGeo* pthing = psite->GetThingGeo();
                        if (power != 0.0f) {
                            pthing->SetAfterburnerThrust(pship->GetOrientation().GetForward() * (-power));
                        } else {
                            pthing->SetAfterburnerThrust(Vector::GetZero());
                        }

                        pthing->SetThrust(0.5f * (pship->GetControls().jsValues[3] + 1));

                        if (pship->GetStateM() & miningMaskIGC) {
                            psite->ActivateBolt();
                        } else {
                            psite->DeactivateBolt();
                        }
                    }

                    //Is the model visible?
                    if (pmodel != m_pcontext->GetShip())
                    {
                        bool    visibleF = true;

                        if (!(visibleF && m_pcontext->GetShip()->CanSee(pmodel)))
                        {
                            visibleF = false;
                        }

                        pmodel->SetVisibleF(visibleF);
                        pmodel->SetRender(visibleF ? renderMask : c_ucRenderNone);
                    }
                }
            }
*/

    }

    //////////////////////////////////////////////////
    // IIgcSite

    TRef<ThingSite> CreateThingSite(ImodelIGC* pModel)
    {
        return ::CreateThingSite(pModel, this);
    }

    TRef<ClusterSite> CreateClusterSite(IclusterIGC* pCluster)
    {
        return ::CreateClusterSite(GetModeler(), 
                        GetWindow()->GetTime(),  
                        m_pviewportPosters, this);
    }

    void PlaySoundEffect(SoundID soundID, ImodelIGC* model = NULL)
    {
        if ((model == NULL) || (model == m_pcontext->GetShip()))
            Sound::PlaySoundEffect(soundID);
    }

    void DamageStationEvent(IstationIGC* pStation,
                                        ImodelIGC* pLauncher,
                                        float flAmount,
                                        float flLeakage)
    {
        if ((NULL != pStation->GetCluster()) &&
            (pStation->GetCluster() == GetCluster()))
        {
            if (pStation->GetShieldFraction() < 0.0f)
                PlaySoundEffect(otherHullHitSound);
            else
                PlaySoundEffect(otherShieldHitSound);
        }
    }
  
    void KillStationEvent(IstationIGC* pStation,
                                  ImodelIGC* pLauncher,
                                  float flAmount,
                                  float flLeakage)
    {
        pStation->GetCluster()->GetClusterSite()->AddExplosion(pStation);
        pStation->Terminate();
    }

    void DamageShipEvent(Time now, IshipIGC* pship, ImodelIGC* plauncher, float amount, float leakage, const Vector& p1, const Vector& p2)
    {
        if (pship == GetShip())
            {
            if (plauncher)
                {
                if (leakage > 0.0f)
                    Sound::PlaySoundEffect(myHullHitSound);
                else
                    Sound::PlaySoundEffect(myShieldHitSound);
                }
            }
        else if ((pship->GetCluster() == GetShip()->GetCluster()) && plauncher)
            {
            if (leakage > 0.0f)
                Sound::PlaySoundEffect(otherHullHitSound);
            else
                Sound::PlaySoundEffect(otherShieldHitSound);
            }
    }
    
    void KillShipEvent(Time now, IshipIGC* pShip, ImodelIGC* launcher, float amount, const Vector& p1, const Vector& p2)
    {
        pShip->GetCluster()->GetClusterSite()->AddExplosion(pShip);
        if (pShip == m_pcontext->GetShip())
            {
            m_pcontext->SetShip(NULL); // this will terminate the ship
            m_pcontext->SwitchToScene(CreateLoadoutScene(m_pcontext));
            }
        else
            {
            pShip->Terminate();
            }
    }

    void FireMissile(IshipIGC* pShip,
                                ImagazineIGC* pMagazine,
                                Time timeFired,
                                ImodelIGC* pTarget,
                                float flLock)
    {
        PlaySoundEffect(jumpSound);

        IclusterIGC*    pCluster = pShip->GetCluster();
        assert (pCluster);

        const Vector&       myVelocity      = pShip->GetVelocity();
        const Orientation&  myOrientation   = pShip->GetOrientation();
        static const Vector offset(0, 0, -1.0f);
        Vector              myPosition      = pShip->GetPosition() +
                                              offset * myOrientation;

        DataMissileIGC   dataMissile;
        dataMissile.pmissiletype = pMagazine->GetMissileType();

        dataMissile.forward = myOrientation.GetForward();
        //Permute the "forward" direction slightly by a random amount
        {
            float d = dataMissile.pmissiletype->GetDispersion();
            if (d != 0.0f)
            {
                float   r = random(0.0f, d);
                float   a = random(0.0f, 2.0f * pi);
                dataMissile.forward += (r * cos(a)) * myOrientation.GetRight();
                dataMissile.forward += (r * sin(a)) * myOrientation.GetUp();

                dataMissile.forward.SetNormalize();
            }
        }

        Time    lastUpdate = pShip->GetLastUpdate();

        dataMissile.position = myPosition + myVelocity * (timeFired - lastUpdate);
        dataMissile.velocity = dataMissile.pmissiletype->GetInitialSpeed() * dataMissile.forward + myVelocity;
        dataMissile.pLauncher = pShip;
        dataMissile.pTarget = pTarget;
        dataMissile.pCluster = pCluster;
        dataMissile.lock = flLock;

        dataMissile.missileID = m_pcontext->GetMissionIGC()->GenerateNewMissileID();
        if (m_pcontext->GetCadetPlay()->GetCadetMode() != cmSinglePlayer)
            m_pcontext->GetCadetPlay()->SendFireMissileMessage(&dataMissile);

        ImissileIGC*  m = (ImissileIGC*)(m_pcontext->GetMissionIGC()->CreateObject(timeFired,
                                                                  OT_missile,
                                                                  &dataMissile,
                                                                  sizeof(dataMissile)));
        assert (m != NULL);
        m->Release();

        short   amount = pMagazine->GetAmount() - 1;
        if (0 == amount)
        {
            //
            // Nothing left ... nuke it (which may also cause it to be
            // released & deleted).
            //
            pMagazine->Terminate();
        }
        else
        {
            assert (amount > 0);
            pMagazine->SetAmount(amount);
        }
    }

    void TerminateModelEvent(ImodelIGC* pModel)
    {
        ObjectType    type = pModel->GetObjectType();
        if (type == OT_ship)
            g_drones.DeleteShip((IshipIGC *)pModel);
    }

    void KillAsteroidEvent(IasteroidIGC* pasteroid, bool explodeF) 
    {
    }
    
    void KillProbeEvent(IprobeIGC* pprobe) 
    {
    }
    
    void KillMineEvent(ImineIGC* pmine) 
    {
    }
    
    void KillMunitionEvent(ImunitionIGC* pmunition) 
    {
    }
    
    bool DockWithStationEvent(IshipIGC* ship, IstationIGC* station)
    {
        return false;
    }

    void HitWarpEvent(IshipIGC* ship, IwarpIGC* warp)
    {

    }
    
    void HitTreasureEvent(Time now, IshipIGC* ship, ItreasureIGC* treasure)
    {
    }
   
};

TRef<CadetFlightImage> CadetFlightImage::Create(CadetGameContext* pcontext)
{
    return new CadetFlightImageImpl(pcontext);
}

//////////////////////////////////////////////////////////////////////////////
//
// ClusterSiteImpl
//
//////////////////////////////////////////////////////////////////////////////

class ClusterSiteImpl : public ClusterSite
{
    public:
        ClusterSiteImpl(Modeler* pmodeler, Number* ptime, Viewport* pviewport,  CadetFlightImageImpl* pflightImg) :
            m_pflightImg(pflightImg)
        {
            m_pGroupScene = GroupGeo::Create();
            m_pposterImage = CreatePosterImage(pviewport);

            m_pparticleGeo = CreateParticleGeo(pmodeler, ptime);
            m_pGroupScene->AddGeo(m_pparticleGeo);

            m_pbitsGeo = CreateBitsGeo(pmodeler, ptime);
            m_pGroupScene->AddGeo(m_pbitsGeo);

            memset(&m_sectordata, 0, sizeof(m_sectordata));
        }

        void Terminate(void)
        {
            //Clear out anything left in the scanners array
            //(it wasn't cleared because the player's side was set to NULL
            //before all objects were removed from the array).
            {
                ScannerLinkIGC* l;
                while (l = m_scanners.first())  //intentional assignment
                {
                    l->data()->Release();
                    delete l;
                }
            }
            m_pGroupScene = NULL;
            m_pposterImage = NULL;
        }


        void AddExplosion(
            const Vector& vecPosition,
            const Vector& vecUp,
            const Vector& vecForward,
            const Vector& vecVelocity,
            float scale,
            const Color& color,
            int   count
        ) {
            //
            // Play a sound if the player can hear it
            //

            if (m_pflightImg->GetCluster() &&
                (m_pflightImg->GetCluster()->GetClusterSite() == this))
            {
                if (scale > 100.0f)
                    Sound::PlaySoundEffect(explodeStationSound, vecPosition, vecVelocity);
                else if (scale > 10.0f)
                    Sound::PlaySoundEffect(explodeShipSound, vecPosition, vecVelocity);
                else
                    Sound::PlaySoundEffect(otherHullHitSound, vecPosition, vecVelocity);
            }

            //
            // Create an explosion geo
            //

            m_pGroupScene->AddGeo(
                CreateExplosion(
                    GetWindow()->GetTime(),
                    vecPosition,
                    vecUp,
                    vecForward,
                    vecVelocity,
                    scale,
                    Color::White(),
                    10,
                    m_pflightImg->GetExplosionImages(),
                    m_pflightImg->GetShockWaveImage()
                )
            );
        }

        void AddExplosion(
            ImodelIGC* pmodel
        ) {
            const Orientation& orient = pmodel->GetOrientation();

            AddExplosion(
                pmodel->GetPosition(),
                orient.GetUp(),
                orient.GetForward(),
                pmodel->GetVelocity(),
                2 * pmodel->GetThingSite()->GetRadius(),
                Color::White(),
                10
            );
        }

        void AddExplosion(
            const Vector& vecPosition,
            float scale
        ) {
            AddExplosion(
                vecPosition,
                Vector(0, 1,  0),
                Vector(0, 1, -1),
                Vector(0, 0,  0),
                scale,
                Color::White(),
                10
            );
        }

        void AddThingSite(ThingSite*    pThingSite)
        {
            ThingGeo* pthing = pThingSite->GetThingGeo();

            if (pthing) {
                pthing->SetParticleGeo(m_pparticleGeo);
                pthing->SetBitsGeo(m_pbitsGeo);
            }
            m_pGroupScene->AddGeo(pThingSite->GetGeo());
        }

        void DeleteThingSite(ThingSite* pThingSite)
        {
            m_pGroupScene->RemoveGeo(pThingSite->GetThingGeo());
        }

        HRESULT AddPoster(const char* textureName, const Vector& vec, float radius)
        {
            TRef<Image> pimage = GetWindow()->GetModeler()->LoadImage(textureName, true);

            if (pimage) {
                m_pposterImage->AddPoster(pimage, vec, radius);
                return S_OK;
            }

            return E_FAIL;
        }

        HRESULT AddPosterGeo(const char* pszName)
        {
            TRef<INameSpace> pns = GetWindow()->GetModeler()->GetNameSpace(pszName);

            if (pns) {
                TRef<Geo> pgeo = Geo::Cast(pns->FindMember("object"));

                if (pgeo) {
                    m_pflightImg->SetPosterGeo(pgeo);
                }
            }

            return E_FAIL;
        }

        GroupGeo*       GetGroupScene(void)
        {
            return m_pGroupScene;
        }

        PosterImage* GetPosterImage()
        {
            return m_pposterImage;
        }

        SectorData* GetSectorData(SideID    sid)
        {
            return &m_sectordata;
        }

        virtual void                    AddScanner(SideID   sid, IscannerIGC* scannerNew)
        {
/*            assert (sid >= 0);
            assert (sid < c_cSidesMax);
            assert (scannerNew);

            if (sid == trekClient.GetSideID())
                AddIbaseIGC((BaseListIGC*)&(m_scanners), scannerNew);*/
        }
        virtual void                    DeleteScanner(SideID   sid, IscannerIGC* scannerOld)
        {
/*            assert (sid >= 0);
            assert (sid < c_cSidesMax);
            assert (scannerOld);
            if (sid == trekClient.GetSideID())
                DeleteIbaseIGC((BaseListIGC*)&(m_scanners), scannerOld);*/
        }
        virtual const ScannerListIGC*      GetScanners(SideID   sid) const
        {
/*            assert (sid >= 0);
            assert (sid < c_cSidesMax);

            return &(m_scanners);*/
            return NULL;
        }

    private:
        TRef<CadetFlightImageImpl>  m_pflightImg;
        TRef<GroupGeo>      m_pGroupScene;
        TRef<PosterImage>   m_pposterImage;
        TRef<ParticleGeo>   m_pparticleGeo;
        TRef<BitsGeo>       m_pbitsGeo;
        SectorData          m_sectordata;
        ScannerListIGC      m_scanners;
};

TRef<ClusterSite> CreateClusterSite(Modeler* pmodeler, Number* ptime, Viewport* pviewport, CadetFlightImageImpl* pflightImg)
{
    return new ClusterSiteImpl(pmodeler, ptime, pviewport, pflightImg);
}

//////////////////////////////////////////////////////////////////////////////
//
// ThingSiteImpl
//
//////////////////////////////////////////////////////////////////////////////


class ThingSiteImpl : public ThingSitePriv
{
    public:
        ThingSiteImpl(ImodelIGC* pmodel, CadetFlightImageImpl* pflightImg)
        :
            m_sm(c_smNone),
            m_bSideVisibility(false),
            m_pdamagebuckets(NULL),
            //m_radius(1),
            m_pmodel(pmodel),
            m_pflightImg(pflightImg)
        {
            assert (pmodel);
            //Don't bother to AddRef pmodel -- it lifespan always exceeds that of the thingsite.
        }

        ~ThingSiteImpl(void)
        {
            assert (!m_pdamagebuckets);
        }

        void Purge(void)
        {
            if (m_pthing) {
                assert (!m_pdecal);
                m_pthing->SetEmpty();
                m_pthing = NULL;
            }
            else if (m_pdecal) {
                m_pdecal->SetEmpty();
                m_pdecal = NULL;
            }
        }

        void Terminate(void)
        {
            if (m_pthing) {
                assert (!m_pdecal);
                m_pthing->SetEmpty();
                m_pthing = NULL;
            }
            else if (m_pdecal) {
                m_pdecal->SetEmpty();
                m_pdecal = NULL;
            }

            if (m_pbolt) {
                m_pbolt->SetEmpty();
                m_pbolt = NULL;
            }

            if (m_pdamagebuckets)
            {
                DamageBucketsLink*  l = m_pdamagebuckets->GetLink();
                assert (&(l->data()) == m_pdamagebuckets);

                delete l;

                //Deleting the bucket clears the backpointer from
                //the thingsite to the bucket.
                assert (m_pdamagebuckets == NULL);
            }
        }

        Vector GetChildOffset(const ZString& strFrame)
        {
            assert (m_pthing);
            m_pthing->Update();
            Vector vec(0, 0, 0);
            m_pthing->GetChildOffset(strFrame, vec);
            return vec;
        }

        void AddExplosion(Time time, const Vector& vecPosition, float scale)
        {
            TRef<Number> ptime = GetWindow()->GetTime();
            BillboardType* ptype = m_pflightImg->GetBillboardType(3);

            assert (m_pthing);
            m_pthing->AddGeo(new TimedBillboardGeo(ptime, vecPosition, scale, ptype->m_pimage, ptype->m_lifespan));
        }

        void AddHullHit(const Vector& vecPosition, const Vector& vecNormal)
        {
            assert (m_pthing);
            m_pthing->AddHullHit(vecPosition, vecNormal);
        }

        void SetAfterburnerThrust(const Vector& thrust, float power)
        {
            if (m_pthing)
                {
                m_pthing->SetAfterburnerThrust(thrust, power);
                }
            else
                {
                assert (m_pdecal);
                }
        }

        void AddMuzzleFlare(const Vector& vecEmissionPoint, float duration)
        {
            if (this == m_pflightImg->GetShip()->GetThingSite())
                m_pflightImg->AddMuzzleFlare(vecEmissionPoint, duration);
        }

        void AddThingSite(ThingSite*    pThingSite)
        {
            assert (m_pthing);
            m_pthing->AddGeo(pThingSite->GetThingGeo());
        }

        void DeleteThingSite(ThingSite* pThingSite)
        {
            assert (m_pthing);
            assert (pThingSite->GetThingGeo());
            m_pthing->RemoveGeo(pThingSite->GetThingGeo());
        }

        void AddFlare(Time timeArg, const Vector& vecPosition, int id,
                      const Vector* ellipseEquation)
        {
            assert (m_pthing);
            TRef<Number> ptimeArg = GetWindow()->GetTime();
            TRef<Number> ptime = Subtract(ptimeArg, ptimeArg->MakeConstant());

            m_pthing->AddFlare(
                new TextureGeo(
                    m_pflightImg->GetGeoFlares(id),
                    new AnimatedImage(
                        ptime,
                        m_pflightImg->GetFlareImage()
                    )
                ),
                ptime,
                -vecPosition,       //NYI we can avoid the - by changing the Geo
                ellipseEquation
            );
        }

        void SetVisible(unsigned char render)
        {
            if (m_pthing)
            {
                bool    vship = render == c_ucRenderAll;

                m_pthing->SetVisible(render >= c_ucRenderTrail);
                m_pthing->SetVisibleShip(vship);

                if (m_pbolt)
                {
                    if (vship)
                        m_pmodel->GetCluster()->GetClusterSite()->GetGroupScene()->AddGeo(m_pbolt);
                    else
                        m_pmodel->GetCluster()->GetClusterSite()->GetGroupScene()->RemoveGeo(m_pbolt);
                }
            }
        }

        ThingGeo* GetThingGeo()
        {
            return m_pthing;
        }

        Geo* GetGeo()
        {
            if (m_pdecal) {
                return m_pdecal;
            } else {
                assert (m_pthing);
                return m_pthing;
            }
        }

        void SetPosition(const Vector& position)
        {
            if (m_pthing) {
                m_pthing->SetPosition(position);
            } else  if (m_pdecal) {
                m_pdecal->SetPosition(position);
            }
        }

        float GetRadius(void)
        {
            if (m_pthing)
            {
                return m_pthing->GetRadius();
            }
            else if (m_pdecal)
            {
                return sqrt2;
            }
            else 
                return 0.0f;
        }

        void SetRadius(float r)
        {
            m_radius = r;

            if (m_pdecal) {
                if (m_pdecal->GetForward().IsZero()) {
                    m_pdecal->SetScale(r / sqrt2);
                } else {
                    m_pdecal->SetForward(m_pdecal->GetForward().Normalize() * m_radius);
                }
            } else {
                assert (m_pthing);
                m_pthing->Update();
                m_pthing->SetRadius(r);
            }
        }

        void SetBlendColor(const Color& color)
        {
            if (m_pdecal) {
                // !!! color always == black
                // m_pdecal->SetColor(color);
            } else {
                assert (m_pthing);
                m_pthing->SetBlendColor(color);
            }
        }

        void SetOrientation(const Orientation& orientation)
        {
            if (m_pthing) {
                m_pthing->SetOrientation(orientation);
            } else if (m_pdecal) {
                if (!m_pdecal->GetForward().IsZero()) {
                    m_pdecal->SetForward(orientation.GetForward() * m_radius);
                }
            }
        }

        void Spin(float r)
        {
            if (m_pdecal) {
                assert (!m_pthing);
                m_pdecal->SetAngle(m_pdecal->GetAngle() + r);
            }
        }

        void SetTexture(const char* pszTextureName)
        {
            assert (m_pthing);
            m_pthing->SetTexture(GetWindow()->GetModeler()->LoadImage(pszTextureName, false));
        }

        void SetAnimation(float transitionStart, float transitionStop, float transitionFPS,
                             float loopingStart,    float loopingStop,    float loopingFPS,
                             IEventSink* peventSink)
        {
            assert (m_pthing);
            m_pthing->PutAnimation(transitionStart, transitionStop, transitionFPS,
                                   loopingStart,    loopingStop,    loopingFPS,
                                   peventSink);
        }

        HRESULT LoadDecal(const char* textureName, bool bDirectional, float width)
        {
            ZAssert(m_pthing == NULL && m_pdecal == NULL);

            Number* ptime = GetWindow()->GetTime();
            TRef<AnimatedImage> pimage = 
                m_pflightImg->LoadAnimatedImage(
                    Divide(
                        Subtract(ptime, ptime->MakeConstant()),
                        new Number(2)  // number of seconds to animate through images
                    ),
                    ZString(textureName) + "bmp"
                );

            if (pimage) {
                m_pdecal =
                    new DecalGeo(
                        pimage,
                        Color::White(),
                        Vector::GetZero(),
                        Vector::GetZero(),
                        Vector::GetZero(),
                        width,
                        0
                    );

                if (bDirectional) {
                    m_pdecal->SetForward(Vector(0, 0, -1));
                }

                return S_OK;
            }

            return E_FAIL;
        }

        HRESULT LoadModel(
            int options,
            const char* modelName,
            const char* textureName,
            float originalRadius,
            bool bUseCache
        ) {
            ZAssert(m_pthing == NULL && m_pdecal == NULL);

            m_pthing =
                ThingGeo::Create(
                    GetWindow()->GetModeler(),
                    GetWindow()->GetTime()
                );

            //
            //  , always use cache
            //             multiple things will be driving the same frame pointer

            bUseCache = true;

            if (modelName) {
                if (textureName && ((!iswalpha(textureName[0])) || (strcmp(modelName, textureName) == 0))) {
                    textureName = NULL;
                }

                TRef<Image> pimageTexture;

                if (textureName) {
                    pimageTexture = GetWindow()->GetModeler()->LoadImage(textureName, false);
                }

                TRef<INameSpace> pns = GetWindow()->GetModeler()->GetNameSpace(modelName);

                if (pns != NULL) {
                    //
                    // found an mdl version
                    //

                    m_pthing->LoadMDL(options, pns, pimageTexture, originalRadius);

                    return S_OK;
                }

                //
                // Didn't find an MDL file try to load an XFile

                return m_pthing->LoadXFile(options, modelName, pimageTexture, originalRadius);
            }

            return E_FAIL;
        }

        void    SetTrailColor(const Color& color)
        {
            if (m_pthing)
                m_pthing->SetTrailColor(color);
            else
            {
                assert (m_pdecal);
            }
        }

        HRESULT LoadAleph(const char* textureName)
        {
            ZAssert(m_pthing == NULL && m_pdecal == NULL);

            m_pthing =
                ThingGeo::Create(
                    GetWindow()->GetModeler(),
                    GetWindow()->GetTime()
                );

            TRef<Image> pimageAleph = 
                GetWindow()->GetModeler()->LoadImage(ZString(textureName) + "bmp", false);

            if (pimageAleph) {
                HRESULT hr = 
                    m_pthing->Load(
                        0, 
                        CreateAlephGeo(
                            GetWindow()->GetModeler(), 
                            GetWindow()->GetTime()
                        ),
                        pimageAleph,
                        0.0f
                    );

                m_pthing->Update();

                return hr;
            }

            return E_FAIL;
        }

        void UpdateScreenPosition(
            const           Point& pointScreenPosition,
            float           fScreenRadius,
            float           distanceToEdge,
            unsigned char   ucRadarState
        ) {
            m_pointScreenPosition = pointScreenPosition;
            m_fScreenRadius       = fScreenRadius;
            m_distanceToEdge      = distanceToEdge;
            m_ucRadarState        = ucRadarState;
        }

        Point GetScreenPosition()
        {
            return m_pointScreenPosition;
        }
        float GetDistanceToEdge()
        {
            return m_distanceToEdge;
        }
        
        float GetScreenRadius()
        {
            return m_fScreenRadius;
        }
        
        unsigned char GetRadarState()
        {
            return m_ucRadarState;
        }

        SelectionMask   GetSelectionMask(void)  
        {
            return m_sm;
        }
        
        void SetSelectionMask(SelectionMask sm) 
        {
            m_sm = sm;
        }
        
        void SetCluster(ImodelIGC* pmodel, IclusterIGC* pcluster)
        {
/*            ModelAttributes ma = pmodel->GetAttributes();

            m_bSideVisibility = (ma & c_mtSeenBySide) != 0;
            m_bIsShip = (pmodel->GetObjectType() == OT_ship);

            {
                ModelClass  mc = pmodel->GetModelClass();
                if (mc != c_mcUnknown)
                {
                    assert (pmodel->GetSide());
                    m_sideVisibility.psCounter(pcluster
                                               ? (&(pcluster->GetClusterSite()->GetSectorData(NA)->nModels[mc][pmodel->GetSide()->GetObjectID()]))
                                               : NULL);
                }
            }

            if (pcluster)
            {
                if ((ma & c_mtPredictable) && trekClient.m_fm.IsConnected())
                    m_sideVisibility.fVisible(true);
                else
                    UpdateSideVisibility(pmodel, pcluster);
            }*/
        }

        void UpdateSideVisibility(ImodelIGC* pmodel, IclusterIGC* pcluster)
        {
            /*
            //We can only update it if we have one & if the client is actually on a side.
            if (m_bSideVisibility && trekClient.GetSide())
            {
                //Update the visibility of hidden or non-static objects
                //(visibile static objects stay visible)
                if (!(m_sideVisibility.fVisible() && (pmodel->GetAttributes() & c_mtPredictable)))
                {
                    //We, trivially, see anything on our side. beyond that ...
                    //does the ship that saw the object last still see it
                    //(if such a ship exists)
                    if ((trekClient.GetSide() == pmodel->GetSide()) ||
                        (m_sideVisibility.pLastSpotter() &&
                         m_sideVisibility.pLastSpotter()->InScannerRange(pmodel)))
                    {
                        //yes
                        if (!m_sideVisibility.fVisible())
                        {
                            if (m_bIsShip)
                                Sound::PlaySoundEffect(newShipSound);
                            m_sideVisibility.fVisible(true);
                        }
                    }
                    else
                    {
                        //do it the hard way
                        m_sideVisibility.fVisible(false);
                        for (ScannerLinkIGC*   l = pcluster->GetClusterSite()->GetScanners(0)->first();
                             (l != NULL);
                             l = l->next())
                        {
                            IscannerIGC*   s = l->data();
                            assert (s->GetCluster() == pcluster);

                            if (s->InScannerRange(pmodel))
                            {
                                //Ship s's side does not see the ship but this ship does
                                if (m_bIsShip)
                                    Sound::PlaySoundEffect(newShipSound);
                                m_sideVisibility.fVisible(true);
                                m_sideVisibility.pLastSpotter(s);
                                break;
                            }
                        }
                    }
                }
            }*/
        }

        bool GetSideVisibility(IsideIGC* side)
        {
/*            assert (side);

            return m_sideVisibility.fVisible();*/
            return true;
        }

        void SetSideVisibility(IsideIGC* side, bool fVisible)
        {
/*            if (m_bSideVisibility && (side == trekClient.GetSide()))
                m_sideVisibility.fVisible(fVisible);*/
        }

        DamageBuckets*       GetDamageBuckets(void) const
        {
//            return m_pdamagebuckets;
            return NULL;
        }

        void                SetDamageBuckets(DamageBuckets*   pdb)
        {
//            m_pdamagebuckets = pdb;
        }

        virtual void             ActivateBolt(void)
        {
            if ((!m_pbolt) && m_pmodel->GetVisibleF())
            {
                float           r = 0.5f * m_pmodel->GetRadius();
                Vector          f = m_pmodel->GetOrientation().GetForward();
                const Vector&   p1 = m_pmodel->GetPosition() + f * r;    //Hack till we get an emit point
                Vector          p2 = p1 + (100.0f + r) * f;

                m_pvvBoltP1 = new VectorValue(p1);
                m_pvvBoltP2 = new VectorValue(p2);

                m_pbolt = CreateBoltGeo(m_pvvBoltP1, m_pvvBoltP2, 0.125f);
                m_pmodel->GetCluster()->GetClusterSite()->GetGroupScene()->AddGeo(m_pbolt);
            }
        }

        virtual void             DeactivateBolt(void)
        {
            if (m_pbolt)
            {
                m_pbolt->SetEmpty();
                m_pbolt = NULL;
            } 
        }

    private:
        TRef<CadetFlightImageImpl>      m_pflightImg;
        SideVisibility      m_sideVisibility;
        TRef<Geo>           m_pbolt;
        TRef<ThingGeo>      m_pthing;
        TRef<DecalGeo>      m_pdecal;
        ImodelIGC*          m_pmodel;
        Point               m_pointScreenPosition;
        VectorValue*        m_pvvBoltP1;
        VectorValue*        m_pvvBoltP2;
        float               m_fScreenRadius;
        DamageBuckets*      m_pdamagebuckets;
        float               m_radius;
        float               m_distanceToEdge;
        SelectionMask       m_sm;
        bool                m_bSideVisibility;
        unsigned char       m_ucRadarState;
        bool                m_bIsShip;
};

TRef<ThingSite> CreateThingSite(ImodelIGC* pModel, CadetFlightImageImpl* pflightImg)
{
    return new ThingSiteImpl(pModel, pflightImg);
}

