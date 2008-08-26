#include "pch.h"      


//////////////////////////////////////////////////////////////////////////////
//
// CadetGameContextImpl
//
//////////////////////////////////////////////////////////////////////////////

class CadetGameContextImpl : public CadetGameContext
{
private:

    ImissionIGC*                m_pmissionIGC;
    TRef<MissionManager>        m_pmissionManager;
    TRef<CadetFlightImage>      m_pflightImage;
    TRef<WrapImage>             m_pimage;
    TRef<GameScene>             m_pscene;
    TRef<IshipIGC>              m_pship;
    TRef<CadetPlay>             m_pcadetPlay;

public:
    CadetGameContextImpl() :
        m_pimage(new WrapImage(Image::GetEmpty())),
        m_pmissionIGC(NULL)
    {
    }

//#define TESTONLY

#ifdef TESTONLY
    void Initialize()
    {
        TRef<GameScene> pscene = CreateTestScene(this);
        SwitchToScene(pscene);
    }

    void Terminate()
    {
        if (m_pscene)
            {
            m_pscene->TermScene();
            m_pscene = NULL;
            }
        m_pimage = NULL;
    }

    void Update(Time time, float dtime)
    {
        if (m_pscene)
            m_pscene->UpdateScene(time, dtime);
    }

#else

    void Initialize()
    {
        // IgcMission
        m_pmissionIGC = CreateMission();
        g_drones.SetCore(m_pmissionIGC);        

        // CadetFligthImage
        m_pflightImage = CadetFlightImage::Create(this);
        m_pmissionIGC->Initialize(Time::Now(), m_pflightImage);

        // MissionManager
        m_pmissionManager = MissionManager::Create(this);

        m_pmissionManager->CreateDefaults();
        m_pmissionManager->LoadTechTree("techtree");
 
        // Noetworking
        m_pcadetPlay = CadetPlay::Create();

        // Initial Scene
        TRef<GameScene> pscene = CreateSplashScene(this);
        SwitchToScene(pscene);

    }

    void Terminate()
    {
        m_pmissionManager = NULL;
        m_pship = NULL;
        m_pmissionIGC->Terminate();
        m_pmissionIGC = NULL;
        if (m_pscene)
            {
            m_pscene->TermScene();
            m_pscene = NULL;
            }
        m_pimage = NULL;
        m_pflightImage = NULL;
        m_pcadetPlay = NULL;
    }

    void Update(Time time, float dtime)
    {
        static Time lastUpdate = time;
        g_drones.Update(lastUpdate, time, dtime);
        m_pmissionManager->Update(time, dtime);
        if (m_pscene)
            m_pscene->UpdateScene(time, dtime);
    }


#endif

    void LoadMap(const ZString& strMap)
    {
       m_pmissionManager->LoadClusters(strMap);
 
        // Start the Mission
        m_pflightImage->SetCluster(m_pmissionIGC->GetCluster(0));
        m_pmissionIGC->GetCluster(0)->SetActive(true);
        m_pmissionIGC->SetMissionStage(STAGE_STARTED);
    }

    TRef<Image> GetImage()
    {
        return m_pimage;
    }

    void SwitchToScene(GameScene* pscene)
    {
        if (m_pscene)
            m_pscene->TermScene();
        m_pscene = pscene;
        if (m_pscene)
            {
            m_pscene->InitScene();
            m_pimage->SetImage(m_pscene->GetSceneImage());
            }
        else
            m_pimage->SetImage(Image::GetEmpty());
    }

    TRef<CadetFlightImage> GetFlightImage()
    {
        return m_pflightImage;
    }

    TRef<MissionManager> GetMissionManager()
    {
        return m_pmissionManager;
    }


    TRef<IshipIGC> GetShip()
    {
        return m_pship;
    }

    TRef<IclusterIGC> GetCluster()
    {
        if (m_pship)
            return m_pship->GetCluster();
        return NULL;
    }

    void SetShip(IshipIGC* pship)
    {
        if (m_pship)
            {
            m_pship->Terminate();
            m_pship = NULL;
            }
            
        if (pship)
            {
            m_pship = pship;
            m_pship->SetCluster(m_pmissionIGC->GetCluster(0));
            }
    }

    
    ImissionIGC* GetMissionIGC()    
    {
        return m_pmissionIGC;
    }

    TRef<CadetPlay> GetCadetPlay()
    {
        return m_pcadetPlay;
    }
    
};


TRef<GameContext> GameContext::Create()
{
    return new CadetGameContextImpl();
}


Drones          g_drones;

