#include "pch.h"

const int rangeClipAsteroids = 1500;

//////////////////////////////////////////////////////////////////////////////
//
// Peripheral Radar Image
//
//////////////////////////////////////////////////////////////////////////////

const int   c_maskTarget   = 0x01;
const int   c_maskOrdered  = 0x02;
const int   c_maskFriendly = 0x04;
const int   c_maskThreat   = 0x08;
const int   c_maskFlash    = 0x10;
const int   c_maskSelected = 0x20;

#define AWF_FLIGHT_FRIENDLY_ICON                "friendlybmp"
#define AWF_FLIGHT_TARGETED_ICON                "targetedbmp"
#define AWF_FLIGHT_ORDERED_ICON                 "orderedbmp"
#define AWF_FLIGHT_LASTFIRED_ICON               "lastfiredbmp"
#define AWF_FLIGHT_LEAD_ICON                    "leadbmp"
#define AWF_FLIGHT_PLANET_ICON                  "planetbmp"
#define AWF_FLIGHT_FIGHTER_ICON                 "fighterbmp"
#define AWF_FLIGHT_CAPSHIP_ICON                 "capitolbmp"
#define AWF_FLIGHT_ALEPH_ICON                   "alephbmp"
#define AWF_FLIGHT_MISSILE_ICON                 "misslebmp"
#define AWF_FLIGHT_STATION_ICON                 "stationbmp"
#define AWF_FLIGHT_MISSILE_LOCKED_ICON          "lockedbmp"
#define AWF_FLIGHT_PARTIAL_MISSILE_LOCK_ICON    "plockbmp"
#define AWF_FLIGHT_CURRENT_DIRECTION_ICON       "directionbmp"
#define AWF_FLIGHT_DESIRED_DIRECTION_ICON       "directionnewbmp"

class RadarImageImpl : public RadarImage {
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class TextData
    {
    public:
        const char* m_szName;
        int         m_range;
        const char* m_szOrder;
        Point       m_position;
        Point       m_direction;

        TextData(const char*    szName,
                 int            range,
                 const char*    szOrder,
                 const Point&   position,
                 const Point&   direction)
        :
            m_szName(szName),
            m_range(range),
            m_szOrder(szOrder),
            m_position(position),
            m_direction(direction)
        {
        }

        TextData(const TextData& data)
        :
            m_szName(data.m_szName),
            m_range(data.m_range),
            m_szOrder(data.m_szOrder),
            m_position(data.m_position),
            m_direction(data.m_direction)
        {
        }

        TextData()
        {
        }

        friend bool operator==(const TextData& d1, const TextData& d2)
        {
            return false;
        }

    };

    typedef TList<TextData> TextDataList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TextDataList  m_listTextData;

    TRef<Surface> m_psurfaceLastFired;
    TRef<Surface> m_psurfaceOrdered;
    TRef<Surface> m_psurfaceCmdObject;
    TRef<Surface> m_psurfaceTargeted;
    TRef<Surface> m_psurfaceLocked;
    TRef<Surface> m_psurfacePartialLock;
    TRef<Surface> m_psurfaceFriendly;
    TRef<Surface> m_psurfacePlanet;
    TRef<Surface> m_psurfaceFighter;
    TRef<Surface> m_psurfaceCapitol;
    TRef<Surface> m_psurfaceAleph;
    TRef<Surface> m_psurfaceMissle;
    TRef<Surface> m_psurfaceStation;

    CadetGameContext* m_pcontext;
    
    RadarLOD m_radarLOD;
    bool     m_bRadarLabels;

    //  , should get the color from IGC but for now use this look up table

    static Color s_colorNeutral;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    RadarImageImpl(Modeler* pmodeler, Viewport* pviewport, CadetGameContext* pcontext) :
        RadarImage(pviewport),
        m_radarLOD(radarLODNone),
        m_pcontext(pcontext),
		
		//joeld code change  -- don't mess.  :)
        m_bRadarLabels(false)

    {
        m_psurfaceFriendly    = pmodeler->LoadSurface(AWF_FLIGHT_FRIENDLY_ICON, true);
        m_psurfaceTargeted    = pmodeler->LoadSurface(AWF_FLIGHT_TARGETED_ICON, true);
        m_psurfaceLocked      = pmodeler->LoadSurface(AWF_FLIGHT_MISSILE_LOCKED_ICON, true);
        m_psurfacePartialLock = pmodeler->LoadSurface(AWF_FLIGHT_PARTIAL_MISSILE_LOCK_ICON, true);
        m_psurfaceOrdered     = pmodeler->LoadSurface(AWF_FLIGHT_ORDERED_ICON, true);
        m_psurfaceLastFired   = pmodeler->LoadSurface(AWF_FLIGHT_LASTFIRED_ICON, true);
        m_psurfacePlanet      = pmodeler->LoadSurface(AWF_FLIGHT_PLANET_ICON, true);
        m_psurfaceFighter     = pmodeler->LoadSurface(AWF_FLIGHT_FIGHTER_ICON, true);
        m_psurfaceCapitol     = pmodeler->LoadSurface(AWF_FLIGHT_CAPSHIP_ICON, true);
        m_psurfaceAleph       = pmodeler->LoadSurface(AWF_FLIGHT_ALEPH_ICON, true);
        m_psurfaceMissle      = pmodeler->LoadSurface(AWF_FLIGHT_MISSILE_ICON, true);
        m_psurfaceStation     = pmodeler->LoadSurface(AWF_FLIGHT_STATION_ICON, true);
        m_psurfaceCmdObject   = pmodeler->LoadSurface(AWF_FLIGHT_ORDERED_ICON, true);

        m_psurfaceFriendly   ->SetColorKey(Color(0, 0, 0));
        m_psurfaceTargeted   ->SetColorKey(Color(0, 0, 0));
        m_psurfaceLocked     ->SetColorKey(Color(0, 0, 0));
        m_psurfacePartialLock->SetColorKey(Color(0, 0, 0));
        m_psurfaceOrdered    ->SetColorKey(Color(0, 0, 0));
        m_psurfaceCmdObject  ->SetColorKey(Color(0, 0, 0));
        m_psurfaceLastFired  ->SetColorKey(Color(0, 0, 0));
        m_psurfacePlanet     ->SetColorKey(Color(0, 0, 0));
        m_psurfaceFighter    ->SetColorKey(Color(0, 0, 0));
        m_psurfaceCapitol    ->SetColorKey(Color(0, 0, 0));
        m_psurfaceAleph      ->SetColorKey(Color(0, 0, 0));
        m_psurfaceMissle     ->SetColorKey(Color(0, 0, 0));
        m_psurfaceStation    ->SetColorKey(Color(0, 0, 0));
    }


    void SetRadarLOD(RadarLOD radarLOD)
    {
        m_radarLOD = radarLOD;
    }

    void SetRadarLabels(bool    rl)
    {
        m_bRadarLabels = rl;
    }

    bool GetRadarLabels(void) const
    {
        return m_bRadarLabels;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Draw a blip expanded to fit around the object
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawExpandedBlip(Context* pcontext, float radius, Surface* psurface)
    {
        Point size  = Point::Cast(psurface->GetRect().Size());
        float xsize = size.X();
        float ysize = size.Y();

        float xsizeMid = xsize / 2;
        float ysizeMid = ysize / 2;

        float delta = radius / sqrt2;

        pcontext->DrawImage3D(psurface, Rect(       0,        0, xsizeMid, ysizeMid), true, Point(-delta, -delta));
        pcontext->DrawImage3D(psurface, Rect(xsizeMid,        0,    xsize, ysizeMid), true, Point( delta, -delta));
        pcontext->DrawImage3D(psurface, Rect(xsizeMid, ysizeMid,    xsize,    ysize), true, Point( delta,  delta));
        pcontext->DrawImage3D(psurface, Rect(       0, ysizeMid, xsizeMid,    ysize), true, Point(-delta,  delta));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Map types to icon bitmaps
    //
    //////////////////////////////////////////////////////////////////////////////

    Surface* GetIcon(ObjectType type)
    {
        switch (type) {
            case OT_asteroid    : return m_psurfacePlanet;
            case OT_warp        : return m_psurfaceAleph;
            case OT_station     : return m_psurfaceStation;
            case OT_missile     : return m_psurfaceMissle;
            case OT_mine        : return m_psurfaceMissle;
            case OT_probe       : return m_psurfaceMissle;
            case OT_treasure    : return m_psurfaceMissle;
            case OT_ship        : return m_psurfaceFighter;
        }

        return NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Draw a blip based on passed parameters
    //
    //////////////////////////////////////////////////////////////////////////////
                        //
    void DrawBlip(Context*      pcontext, 
                  const Rect&   rectImage,
                  unsigned char ucRadarState,
                  ThingSite*    pts,
                  Surface*      psurfaceIcon,
                  const char*   szName,
                  int           range,
                  const char*   szOrder,
                  int           maskBrackets,
                  float         lock,
                  bool          bLabels)
    {

        float               radiusObject = pts->GetScreenRadius();
        {
            static const float maxRadius = sqrt2 * 80.0f;
            if (radiusObject > maxRadius)
                radiusObject = maxRadius;
        }
        float radiusBracket = 1.05f * radiusObject;

        const Point&    positionObject = pts->GetScreenPosition();

        {
            //Draw the brackets around the "object"
            pcontext->PreTranslate(positionObject);

            if (maskBrackets & c_maskFriendly)
                DrawExpandedBlip(pcontext, radiusBracket, m_psurfaceFriendly);

            if (maskBrackets & c_maskTarget)
                DrawExpandedBlip(pcontext, radiusBracket, m_psurfaceTargeted);

            if (lock == 1.0f)
                DrawExpandedBlip(pcontext, radiusBracket, m_psurfaceLocked);
            else if (lock != 0.0f)
                DrawExpandedBlip(pcontext, radiusBracket + 40.0f * (1.0f - lock), m_psurfacePartialLock);

            if (maskBrackets & c_maskThreat)
                DrawExpandedBlip(pcontext, radiusBracket, m_psurfaceLastFired);

            if (maskBrackets & (c_maskOrdered | c_maskSelected))
                DrawExpandedBlip(pcontext, radiusBracket, m_psurfaceOrdered);
        }

        //Get the vector from the object to the center of the scree
        //where this direction for something close to the center of the
        //screen is always straight down.
        Point           directionCenter;
        {
            directionCenter.SetX(positionObject.X());
            directionCenter.SetY(positionObject.Y() + rectImage.Size().Y() / 5.0f);

            float   l2 = directionCenter.X() * directionCenter.X() + directionCenter.Y() * directionCenter.Y();
            if (l2 < 1.0f)
            {
                directionCenter.SetX(0.0f);
                directionCenter.SetY(-1.0f);
            }
            else
            {
                float   f = float(-1.0 / sqrt(l2));
                directionCenter.SetX(directionCenter.X() * f);
                directionCenter.SetY(directionCenter.Y() * f);
            }
        }

        Point   positionLabel;
        if (ucRadarState != c_ucRadarOnScreenLarge)
        {
            //Do not offset the icon position but do turn alpha blending on
            positionLabel = positionObject;
            if (ucRadarState == c_ucRadarOffScreen)
                pcontext->SetBlendMode(BlendModeSourceAlpha);
        }
        else
        {
            float   d = pts->GetDistanceToEdge();
            float   f = (d < radiusObject)
                        ? (d / radiusObject)
                        : 1.0f;

            Point   positionIcon;
            positionIcon.SetX(directionCenter.X() * f * radiusBracket);
            positionIcon.SetY(directionCenter.Y() * f * radiusBracket);

            //Offset the center enough to draw the icon at the edge of the object
            pcontext->PreTranslate(positionIcon);

            positionLabel = positionObject + positionIcon;
        }
        pcontext->DrawImage3D(psurfaceIcon, true);

        if (bLabels)
        {
            m_listTextData.PushFront(TextData(szName, range, szOrder,
                                              positionLabel, directionCenter));
        }
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Render one blip for each object in the cluster
    //
    //////////////////////////////////////////////////////////////////////////////

    void RenderBlips(Context* pcontext, const Rect& rectImage)
    {
        if (!m_pcontext->GetCluster())
            return;

        const ModelListIGC* models = m_pcontext->GetCluster()->GetPickableModels();
        ZAssert(models != NULL);

        //
        // Draw a blip for every model in the cluster
        //

        const Vector& vecPositionPlayer = m_pcontext->GetShip()->GetPosition();
        float         radiusPlayer      = m_pcontext->GetShip()->GetThingSite()->GetRadius();

        ImodelIGC* pmodelOrders[c_cmdMax];

        {
            // For command targets not in the current sector ... show the user how to get there.

            for (Command i = 0; (i < c_cmdMax); i++)
            {
                pmodelOrders[i] = m_pcontext->GetShip()->GetCommandTarget(i);
                if (pmodelOrders[i])
                {
                    IclusterIGC*    c = pmodelOrders[i]->GetCluster();

                    if (c == NULL)
                    {
                        pmodelOrders[i] = NULL;
                    }
                    else if (c != m_pcontext->GetCluster())
                    {
                        // The target is not in the sector ... find out how to get to the target
                        pmodelOrders[i] = FindPath(m_pcontext->GetShip(), pmodelOrders[i]);
                    }
                }
            }
        }

        ImodelIGC*  pmodelTarget = m_pcontext->GetShip()->GetCommandTarget(c_cmdCurrent);
        IsideIGC*   psideMine = m_pcontext->GetShip()->GetSide();

        Time        now = Time::Now();

        ImodelIGC*  pmodelFriendly = FindTarget(m_pcontext->GetShip(),
                                                c_ttShip | c_ttFriendly | c_ttNearest);

        for (ModelLinkIGC* l = models->first();
             (l != NULL); l = l->next())
        {
            ImodelIGC* pmodel = l->data();

            if (pmodel->GetVisibleF() && m_pcontext->GetShip()->CanSee(pmodel))
            {
                ObjectType      type = pmodel->GetObjectType();
                Surface*        psurfaceIcon = GetIcon(type);
                assert (psurfaceIcon);
                int   maskBrackets = (pmodel == pmodelTarget) ? c_maskTarget : 0x00;

                static const char*  c_szEmpty = "";

                const char*     szName = pmodel->GetName();
                const char*     szOrder = c_szEmpty;
                {
                    for (Command i = 0; (i < c_cmdMax); i++)
                    {
                        if (pmodel == pmodelOrders[i])
                        {
                            maskBrackets |= c_maskOrdered;
                            szOrder = m_pcontext->GetShip()->GetCommandVerb(i);
                            break;
                        }
                    }
                }

                float lock    = 0.0f;
                if (maskBrackets & c_maskTarget)
                {
                    //Never display both the ordered and target brackets simultaneously
                    maskBrackets &= ~c_maskOrdered;

                    ImagazineIGC* magazine = (ImagazineIGC*)(m_pcontext->GetShip()->GetMountedPart(ET_Magazine, 0));
                    if (magazine)
                        lock = magazine->GetLock();
                }

                int             range;
                {
                    if (pmodel == m_pcontext->GetShip())
                        range = -1;
                    else
                    {
                        const Vector&   vecPosition = pmodel->GetPosition();
                        float           radius = pmodel->GetRadius();
                        range = int((vecPosition - vecPositionPlayer).Length() -
                                    radius - radiusPlayer + 0.5f);
                        if (range < 0)
                            range = 0;
                    }
                }

                ThingSite*  pts = pmodel->GetThingSite();
                unsigned char ucRadarState = pts->GetRadarState();

                if (pmodel == pmodelFriendly)
                    maskBrackets |= c_maskFriendly;

                if (type == OT_ship)
                {
                    assert (pmodel->GetThingSite());
                    DamageBuckets*   b = pmodel->GetThingSite()->GetDamageBuckets();
                    if (b)
                    {
                        maskBrackets |= (b->flash(now))
                                        ? (c_maskThreat | c_maskFlash)
                                        : c_maskThreat;
                    }
                }

                //
                // Color for the side
                //
                {
                    static const Color c_colorRed(1.0f, 0.0f, 0.0f);
                    IsideIGC*       side = pmodel->GetSide();

                    Color color = side
                                  ? ((maskBrackets & c_maskFlash) ? c_colorRed : side->GetColor())
                                  : s_colorNeutral;

                    static const int    closeRange = 100;
                    static const int    farRange = 1000;
                    static const float  closeAlpha = 1.0f;
                    static const float  farAlpha = 0.25f;

                    color.SetAlpha((range < closeRange)
                                   ? closeAlpha
                                   : ((range > farRange)
                                      ? farAlpha
                                      : (farAlpha +
                                         (float(range - farRange) * (closeAlpha - farAlpha) / float(closeRange - farRange)))));

                    pcontext->SetColor(color);
                }

                if (pts->GetSelectionMask())
                    maskBrackets |= c_maskSelected;

                bool    bRender;
                bool    bLabels = (type == OT_warp) || (maskBrackets != 0);
                switch (m_radarLOD)
                {
                    case radarLOD3:
                        //Lowest level of detail
                        bRender = bLabels;
                    break;
                    case radarLOD2:
                        //Normal level of detail
                        bRender = bLabels = bLabels || (range <= rangeClipAsteroids);
                    break;
                    case radarLOD1:
                        //Highest level of detail
                        bLabels = bLabels || (range <= rangeClipAsteroids) || (ucRadarState != c_ucRadarOffScreen) || (szName[0] != '\0') || m_bRadarLabels;
                        bRender = true;
                    break;
                    default:
                        assert (false);
                }

                if (bRender)
                {
                    //
                    // Draw the Blip
                    //
                    pcontext->PushState();
                    DrawBlip(pcontext, 
                             rectImage,
                             ucRadarState,
                             pts,
                             psurfaceIcon,
                             szName,
                             range,
                             szOrder,
                             maskBrackets,
                             lock,
                             bLabels);
                    pcontext->PopState();
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Render the image
    //
    //////////////////////////////////////////////////////////////////////////////

    void Render(Context* pcontext)
    {
		if (m_radarLOD != radarLODNone)
        {
            //
            // Use flat shading
            //
            pcontext->SetShadeMode(ShadeModeFlat);
            pcontext->SetLinearFilter(false, true);

			//
			// Shrink the rect size by half the size of the gauge bitmap
			// and center the rect around the origin
			//
            Rect  rectImage   = GetViewRect()->GetValue();
            {
			    Point pointCenter = rectImage.Center();

			    pcontext->PreTranslate(pointCenter);
            }

			//
			// Draw some Blips
            //

			RenderBlips(pcontext, 
                        rectImage);

            //
            // Draw the text
            //

            pcontext->SetColor(Color::White());

            TextDataList::Iterator iter(m_listTextData);

            while (!iter.End())
            {
                const TextData& data = iter.Value();

                float   lines = 0.0f;
                float   width = 0.0f;
                if (data.m_szName[0] != '\0')
                {
                    float   w = pcontext->GetTextExtent(data.m_szName).X();
                    if (w > width)
                        width = w;

                    lines += 1.0f;
                }

                char    szRange[20];
                if (data.m_range >= 0)
                {
                    _itoa(data.m_range, szRange, 10);
                    float   w = pcontext->GetTextExtent(szRange).X();
                    if (w > width)
                        width = w;

                    lines += 1.0f;
                }

                if (data.m_szOrder[0] != '\0')
                {
                    float   w = pcontext->GetTextExtent(data.m_szOrder).X();
                    if (w > width)
                        width = w;

                    lines += 1.0f;
                }

                float   heightFont = float(pcontext->GetTextHeight());

                //Find the offset to center the text at position + offset
                Point   offset(-0.5f * width, heightFont * 0.5f * (lines - 2.0f));

                //Adjust the offset by the amount required to move a point from the center to the edge
                //in the specified direction
                {
                    float   w = width + 15.0f;
                    float   h = heightFont * lines + 15.0f;

                    float   x = data.m_direction.X();
                    float   y = data.m_direction.Y();
            
                    if (x == 0.0f)
                        y = (y < 0.0f) ? -h : h;
                    else if (y == 0.0f)
                        x = (x <= 0.0f) ? -w : w;
                    else
                    {
                        float   tX = w / float(fabs(x));
                        float   tY = h / float(fabs(y));
                        float   tMin = (tX < tY) ? tX : tY;

                        x *= tMin;
                        y *= tMin;
                    }

                    offset.SetX(offset.X() + x * 0.5f);
                    offset.SetY(offset.Y() + y * 0.5f);
                }

                if (data.m_szName[0] != '\0')
                {
                    pcontext->DrawString(ZString(data.m_szName),
                                         data.m_position + offset);
                    offset.SetY(offset.Y() - heightFont);
                }

                if (data.m_range >= 0)
                {
                    pcontext->DrawString(ZString(szRange),
                                         data.m_position + offset);
                    offset.SetY(offset.Y() - heightFont);
                }

                if (data.m_szOrder[0] != '\0')
                {
                    pcontext->DrawString(ZString(data.m_szOrder),
                                         data.m_position + offset);
                }

                iter.Next();
            }

            m_listTextData.SetEmpty();
        }
    }
};

Color RadarImageImpl::s_colorNeutral(1, 1, 1);

//////////////////////////////////////////////////////////////////////////////
//
// RadarImage Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<RadarImage> RadarImage::Create(Modeler* pmodeler, Viewport* pviewport, CadetGameContext* pcontext)
{
    return new RadarImageImpl(pmodeler, pviewport, pcontext);
}

