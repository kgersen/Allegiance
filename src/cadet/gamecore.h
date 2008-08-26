//////////////////////////////////////////////////////////////////////////////
//
// Globals
//
//////////////////////////////////////////////////////////////////////////////

TRef<Engine> GetEngine();
TRef<Modeler> GetModeler();
TRef<EffectWindow> GetWindow();


//////////////////////////////////////////////////////////////////////////////
//
// GameScene
//
//////////////////////////////////////////////////////////////////////////////

class GameScene : public IObject
{
public:
    virtual void InitScene() = 0;
    virtual TRef<Image> GetSceneImage() = 0;
    virtual void UpdateScene(Time time, float dtime) = 0;
    virtual void TermScene() = 0;
};


//////////////////////////////////////////////////////////////////////////////
//
// GameContext
//
//////////////////////////////////////////////////////////////////////////////

class GameContext : public IObject
{
public:

    // creation

    static TRef<GameContext> Create();

    // incoming calls from app

    virtual void Initialize() = 0;
    virtual TRef<Image> GetImage() = 0;
    virtual void Update(Time time, float dtime) = 0;
    virtual void Terminate() = 0;

    // scene control

    virtual void SwitchToScene(GameScene* pscene) = 0;


};




