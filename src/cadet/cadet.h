
//////////////////////////////////////////////////////////////////////////////
//
// Forwards
//
//////////////////////////////////////////////////////////////////////////////

class CadetGameContext;

//////////////////////////////////////////////////////////////////////////////
//
// CadetFlightImage
//
//////////////////////////////////////////////////////////////////////////////

class CadetFlightImage : public WrapImage, public IIgcSite
{
public:
    CadetFlightImage(): WrapImage(Image::GetEmpty()) {}

    virtual void AddGeo(Geo* pgeo) = 0;
    virtual void RemoveGeo(Geo* pgeo) = 0;
    virtual void AddImage(Image* pimage) = 0;
    virtual void RemoveImage(Image* pimage) = 0;

    virtual void SetCluster(IclusterIGC* pcluster) = 0;
    virtual void SetCameraPosition(const Vector& vec) = 0;
    virtual void SetCameraOrientation(const Orientation& orientation) = 0;

    static TRef<CadetFlightImage> Create(CadetGameContext* pcontext);
};


//////////////////////////////////////////////////////////////////////////////
//
// CadetGameContext
//
//////////////////////////////////////////////////////////////////////////////

class CadetGameContext : public GameContext, public MissionContext
{
public:
    virtual void SetShip(IshipIGC* pship) = 0;
    virtual TRef<IclusterIGC> GetCluster() = 0;

    virtual TRef<CadetFlightImage> GetFlightImage() = 0;
    virtual TRef<MissionManager> GetMissionManager() = 0;

    virtual TRef<CadetPlay> GetCadetPlay() = 0;
    virtual void LoadMap(const ZString& strMap) = 0;
};


//////////////////////////////////////////////////////////////////////////////
//
// Scenes
//
//////////////////////////////////////////////////////////////////////////////


TRef<GameScene> CreateSplashScene(CadetGameContext* pgamecontext);
TRef<GameScene> CreateFlightScene(CadetGameContext* pgamecontext);
TRef<GameScene> CreateLoadoutScene(CadetGameContext* pgamecontext);
TRef<GameScene> CreateTestScene(CadetGameContext* pgamecontext);


