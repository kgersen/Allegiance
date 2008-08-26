#include "pch.h"
#include "terrain.h"


template<class VertexType>
class TestGeo : public Geo {
private:
    int              m_countWidth;
    int              m_countHeight;
    TVector<VertexType>  m_vertices;
    TVector<WORD>    m_indices;


public:
    //
    // Constructor
    //

    TestGeo(int countWidth, int countHeight) :
        m_countWidth(countWidth),
        m_countHeight(countHeight),
        m_vertices(
            (m_countWidth + 1) * (m_countHeight + 1),
            (m_countWidth + 1) * (m_countHeight + 1)
        ),
        m_indices(
            m_countWidth * m_countHeight * 6,
            m_countWidth * m_countHeight * 6
        )
    {

        InitializeVertices();
        InitializeIndices();
    }

    //
    // Members
    //

    void InitializeVertices()
    {
        for(int indexWidth = 0; indexWidth < m_countWidth + 1; indexWidth++) 
            {
            for (int indexHeight = 0; indexHeight < (m_countHeight + 1); indexHeight++)
                {
                m_vertices.Set(
                    indexWidth + indexHeight * (m_countWidth + 1),
                    VertexType(
                        (10.0f/m_countWidth) * indexWidth, 
                        -(10.0f/m_countHeight) * indexHeight, 
                        0,
                        0,
                        0,
                        1,
                        1.0f * indexHeight / m_countHeight,
                        1.0f * indexWidth / m_countWidth
                        )
                    );
                }
            }
    }

    void InitializeIndices()
    {
        int indexIndex = 0;
        for(int indexWidth = 0; indexWidth < m_countWidth; indexWidth++) 
            {
            for(int indexHeight = 0; indexHeight < m_countHeight; indexHeight++) 
                {
                int indexStart = indexWidth + indexHeight * (m_countWidth + 1);
                m_indices.Set(indexIndex + 0, indexStart);
                m_indices.Set(indexIndex + 1, indexStart + (m_countWidth + 1) + 1);
                m_indices.Set(indexIndex + 2, indexStart + (m_countWidth + 1));
                                            
                m_indices.Set(indexIndex + 3, indexStart);
                m_indices.Set(indexIndex + 4, indexStart + 1);
                m_indices.Set(indexIndex + 5, indexStart + (m_countWidth + 1) + 1);

                indexIndex += 6;
                }
            }
    }

    float GetRadius(const Matrix& mat)
    {
        return 1;
    }

    void Render(Context* pcontext)
    {
        pcontext->DrawTriangles(
            &m_vertices[0],
            m_vertices.GetCount(),
            &m_indices[0],
            m_indices.GetCount());
    }

};


template<class VertexType>
class BoidGeo : public Geo {
private:
    VertexType	boid_vertices[16];
    WORD		boid_indices[30];


public:

    BoidGeo()
    {
    	boid_vertices[ 0] = VertexType(Vector(0.0f, 0.0f, 10.0f), Vector(0.2f, 1.0f, 0.0f).Normalize(), Point(0.0f, 0.5f));
    	boid_vertices[ 1] = VertexType(Vector(10.0f, 0.0f, -10.0f), Vector(0.1f, 1.0f, 0.0f).Normalize(), Point(0.5f, 1.0f));
    	boid_vertices[ 2] = VertexType(Vector(3.0f, 3.0f, -7.0f), Vector(0.0f, 1.0f, 0.0f).Normalize(), Point(0.425f, 0.575f));
    	boid_vertices[ 3] = VertexType(Vector(-3.0f, 3.0f, -7.0f), Vector(-0.1f, 1.0f, 0.0f).Normalize(), Point(0.425f, 0.425f));
    	boid_vertices[ 4] = VertexType(Vector(-10.0f, 0.0f, -10.0f), Vector(-0.2f, 1.0f, 0.0f).Normalize(), Point(0.5f, 0.0f));
    	boid_vertices[ 5] = VertexType(Vector(0.0f, 0.0f, 10.0f), Vector(0.2f, -1.0f, 0.0f).Normalize(), Point(1.0f, 0.5f));
    	boid_vertices[ 6] = VertexType(Vector(10.0f, 0.0f, -10.0f), Vector(0.1f, -1.0f, 0.0f).Normalize(), Point(0.5f, 1.0f));
    	boid_vertices[ 7] = VertexType(Vector(3.0f, -3.0f, -7.0f), Vector(0.0f, -1.0f, 0.0f).Normalize(), Point(0.575f, 0.575f));
    	boid_vertices[ 8] = VertexType(Vector(-3.0f, -3.0f, -7.0f), Vector(-0.1f, -1.0f, 0.0f).Normalize(), Point(0.575f, 0.425f));
    	boid_vertices[ 9] = VertexType(Vector(-10.0f, 0.0f, -10.0f), Vector(-0.2f, -1.0f, 0.0f).Normalize(), Point(0.5f, 0.0f));
    	boid_vertices[10] = VertexType(Vector(10.0f, 0.0f, -10.0f), Vector(-0.4f, 0.0f, -1.0f).Normalize(), Point(0.5f, 1.0f));
    	boid_vertices[11] = VertexType(Vector(3.0f, 3.0f, -7.0f), Vector(-0.2f, 0.0f, -1.0f).Normalize(), Point(0.425f, 0.575f));
    	boid_vertices[12] = VertexType(Vector(-3.0f, 3.0f, -7.0f), Vector(0.2f, 0.0f, -1.0f).Normalize(), Point(0.425f, 0.425f));
    	boid_vertices[13] = VertexType(Vector(-10.0f, 0.0f, -10.0f), Vector(0.4f, 0.0f, -1.0f).Normalize(), Point(0.5f, 0.0f));
    	boid_vertices[14] = VertexType(Vector(-3.0f, -3.0f, -7.0f), Vector(0.2f, 0.0f, -1.0f).Normalize(), Point(0.575f, 0.425f));
    	boid_vertices[15] = VertexType(Vector(3.0f, -3.0f, -7.0f), Vector(-0.2f, 0.0f, -1.0f).Normalize(), Point(0.575f, 0.575f));

    	boid_indices[ 0] = 0;
    	boid_indices[ 1] = 1;
    	boid_indices[ 2] = 2;
    	boid_indices[ 3] = 0;
    	boid_indices[ 4] = 2;
    	boid_indices[ 5] = 3;
    	boid_indices[ 6] = 0;
    	boid_indices[ 7] = 3;
    	boid_indices[ 8] = 4;
    	boid_indices[ 9] = 5;
    	boid_indices[10] = 7;
    	boid_indices[11] = 6;
    	boid_indices[12] = 5;
    	boid_indices[13] = 8;
    	boid_indices[14] = 7;
    	boid_indices[15] = 5;
    	boid_indices[16] = 9;
    	boid_indices[17] = 8;
    	boid_indices[18] = 10;
    	boid_indices[19] = 15;
    	boid_indices[20] = 11;
    	boid_indices[21] = 11;
    	boid_indices[22] = 15;
    	boid_indices[23] = 12;
    	boid_indices[24] = 12;
    	boid_indices[25] = 15;
    	boid_indices[26] = 14;
    	boid_indices[27] = 12;
    	boid_indices[28] = 14;
    	boid_indices[29] = 13;

    	// scale the boid to be unit length
/*    	for (int i=0; i<16; i++) {
    		boid_vertices[i].x /= 20.0f;
    		boid_vertices[i].y /= 20.0f;
    		boid_vertices[i].z /= 20.0f;
    	}
*/
    }

    float GetRadius(const Matrix& mat)
    {
        return 1;
    }

    void Render(Context* pcontext)
    {
        pcontext->DrawTriangles(
            &boid_vertices[0],
            16,
            &boid_indices[0],
            30);
    }

};


//////////////////////////////////////////////////////////////////////////////
//
// BaseCadetScene
//
//////////////////////////////////////////////////////////////////////////////

class BaseTestScene : public GameScene, public IKeyboardInput
{
protected:
    TRef<GroupGeo>          m_pgroupGeo;
    TRef<GeoImage>          m_pimage;
    TRef<CadetGameContext>  m_pgameContext;
    TRef<Camera>            m_pcamera;
    TRef<Viewport>          m_pviewport;
    
public:

    BaseTestScene(CadetGameContext* pgameContext) :
        m_pgameContext(pgameContext)
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
    

    void InitScene()
    {
        m_pcamera = new Camera();
        m_pcamera->SetPosition(Vector(0, 0, 30));
        m_pcamera->SetOrientation(Orientation(Vector(0, 0, -1), Vector(0, 1, 0)));
        m_pcamera->SetZClip(1, 10000);
        m_pcamera->SetFOV(RadiansFromDegrees(65));
        m_pviewport = new Viewport(m_pcamera, GetWindow()->GetRenderRectValue());
        m_pimage = new GeoImage(
                m_pgroupGeo = GroupGeo::Create(),
                m_pviewport, true);

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
    }

    virtual bool   OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case 'Q':
                    GetWindow()->PostMessage(WM_QUIT);
                    return true;
                case 'F':
                    GetWindow()->SetShowFPS(!GetWindow()->GetShowFPS());
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
// TestScene1
//
//////////////////////////////////////////////////////////////////////////////

class TestScene1 : public BaseTestScene
{
private:
    TRef<TerrainTileData>   m_ptileDataRight;
    TRef<TerrainTileData>   m_ptileDataStraight;
    TRef<TerrainMap>        m_pmap;
    TRef<TerrainTile>       m_ptile;
    TRef<ThingGeo>          m_pthing;
    Orientation             m_orientation;
    Vector                  m_position;
    
public:

    TestScene1(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext),
        m_position(Vector(0,0,50)),
        m_orientation(Orientation(Vector(0,0,-1), Vector(0,1,0)))
    {
        // load map
        m_ptileDataStraight = TerrainTileData::Create(GetStraight());
        m_ptileDataRight = TerrainTileData::Create(GetRight());
        m_pmap = TerrainMap::Create();
        m_ptile = m_pmap->SetRoot(m_pmap->CreateTile(m_ptileDataStraight));

        // load ship
        m_pthing= ThingGeo::Create(GetModeler(), GetWindow()->GetTime());
        TRef<INameSpace> pns = GetWindow()->GetModeler()->GetNameSpace("fig12");
        m_pthing->LoadMDL(0, pns, NULL, 15);
        m_pthing->SetRadius(10);
    }

    void Init()
    {
        m_pgroupGeo->AddGeo(m_pmap->GetGeo());
        m_pgroupGeo->AddGeo(m_pthing);
        UpdateShip();
    }

    void UpdateShip()
    {
        m_pthing->SetPosition(m_position);
        m_pthing->SetOrientation(m_orientation);
        Vector cameraPosition = m_position - (m_orientation.GetForward() * m_pthing->GetRadius() * 7);
        m_pcamera->SetPosition(cameraPosition);
        m_pcamera->SetOrientation(m_orientation);
        m_pmap->HitTest(m_position);
    }

    bool   OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case '1':
                    m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataStraight));
                    return true;
                case '2':
                    m_ptile = m_pmap->Connect(m_ptile, 0, m_pmap->CreateTile(m_ptileDataRight));
                    return true;
                case VK_UP:
                    if (ks.bCtrl)
                        m_orientation.Pitch(RadiansFromDegrees(-10));
                    else
                        m_position = m_position + m_orientation.GetForward() * 20;
                    UpdateShip();
                    return true;
                case VK_DOWN:
                    if (ks.bCtrl)
                        m_orientation.Pitch(RadiansFromDegrees(10));
                    else
                        m_position = m_position - m_orientation.GetForward() * 20;
                    UpdateShip();
                    return true;
                case VK_RIGHT:
                        m_orientation.Yaw(RadiansFromDegrees(-10));
                    UpdateShip();
                    return true;
                case VK_LEFT:
                        m_orientation.Yaw(RadiansFromDegrees(10));
                    UpdateShip();
                    return true;
                }
            }
		return BaseTestScene::OnKey(pprovider, ks, fForceTranslate);
	}

};


//////////////////////////////////////////////////////////////////////////////
//
// TestScene2
//
//////////////////////////////////////////////////////////////////////////////

class TestScene2 : public BaseTestScene
{
private:
    
public:

    TestScene2(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext)
    {
    }

    void Init()
    {
          for (int z = 0; z < 4; z++)
            for (int y = 0; y < 4; y++)
                {
                TRef<Geo> pgeo = GetModeler()->LoadGeo("fig04");
                m_pgroupGeo->AddGeo(
                        new TransformGeo(
                                pgeo,
                                new TranslateTransform(Vector(0, 100+(float)y*100, 100+(float)z*100))
                                )
                        );
                }
    }

};

class TestScene3 : public BaseTestScene, public IEventSink
{
private:
    TRef<GroupGeo> m_pgroupTest;   
    TRef<IEventSink> m_peventSinkTimer;
    int m_iTest;

public:

    TestScene3(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext),
        m_iTest(0)
    {
    
    }

    void Init()
    {
        SetupTest(m_iTest);
        m_peventSinkTimer = IEventSink::CreateDelegate(this);
        GetWindow()->GetTimer()->AddSink(m_peventSinkTimer, 3.0f);
    }

    void Term()
    {
        GetWindow()->GetTimer()->RemoveSink(m_peventSinkTimer);
    }

    bool OnEvent(IEventSource* pevent)
    {
        if (m_iTest == 25)
            return true;
            
        GetWindow()->OutputPerformanceCounters();
        if (++m_iTest < 25)
            SetupTest(m_iTest);
        return true;
    }

    void SetupTest(int iTest)
    {
        static int rgnTriangles[] = {50,200,450,800,1250};
        static int rgnMeshes[] = {1,5,10,15,20};
        SetupTest(rgnMeshes[iTest/5],rgnTriangles[iTest%5]);
    }

    void SetupTest(int nMeshes, int nTriangles)
    {
        if (m_pgroupTest)
            {
            m_pgroupGeo->RemoveGeo(m_pgroupTest);
            m_pgroupTest = NULL;
            }

        m_pgroupTest = GroupGeo::Create();
        m_pgroupGeo->AddGeo(m_pgroupTest);

        TRef<Image> pimageTexture = GetModeler()->LoadImage("wallbmp", false);

        ZString str = "D3DVertex, " + ZString(nMeshes) + " meshes w/ " + ZString(nTriangles) + " triangles\r\n";
        ZDebugOutput(str);
        for (int i = 0; i < nMeshes; i++)
            {
            m_pgroupTest->AddGeo(
                new TextureGeo(
                    new TestGeo<D3DVertex>((int)sqrt((float)nTriangles/2.0f), (int)sqrt((float)nTriangles/2.0f)),
                    pimageTexture
                    )
                );
            }
    }

};

class TestScene4 : public BaseTestScene
{
private:

public:

    TestScene4(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext)
    {
    
    }

    void Init()
    {
        TRef<Image> pimageTexture = GetModeler()->LoadImage("wallbmp", false);

/*
        m_pgroupGeo->AddGeo(
                    new TransformGeo(
                        new TextureGeo(
                            new TestGeo<Vertex>(3, 3),
                            pimageTexture
                            ),
                    new TranslateTransform(Vector(0,10,0))
                    )
                );
*/
        m_pgroupGeo->AddGeo(
                new TextureGeo(
                    new TestGeo<D3DVertex>(3, 3),
                    pimageTexture
                    )
                );

/*        m_pgroupGeo->AddGeo(
                new MaterialGeo(
                    new TestGeo<D3DVertex>(3, 3),
                    CreateMaterial(Color(1,0,0,0), Color(1,0,0,0), Color(1,0,0,0), 0.5)
                    )
                );
*/                

    }

};


class TestScene5 : public BaseTestScene
{
private:

public:

    TestScene5(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext)
    {
    
    }

    void Init()
    {
        Sound::DefineDiscreteSoundEffect(energySound,           
            "ef10", false);
        Sound::PlaySoundEffect(energySound);
     }

    bool   OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case 'P':
                    Sound::PlaySoundEffect(energySound);
                    return true;
                }
            }
		return BaseTestScene::OnKey(pprovider, ks, fForceTranslate);
	}

};

class TestSceneDPlay : public BaseTestScene, public CadetPlaySite
{
private:
    TRef<CadetPlay>             m_pcadetPlay;

public:

    TestSceneDPlay(CadetGameContext* pgameContext) :
        BaseTestScene(pgameContext)
    {
    
    }

    void Init()
    {
        m_pgameContext->GetCadetPlay()->SetCadetPlaySite(this);
#ifdef DREAMCAST
        HRESULT hr = m_pgameContext->GetCadetPlay()->HostSession();
#else        
        HRESULT hr = m_pgameContext->GetCadetPlay()->JoinSession(m_pgameContext->GetMissionIGC());
#endif        
    }

    void Term()
    {
        m_pgameContext->GetCadetPlay()->SetCadetPlaySite(NULL);
    }

    void UpdateScene(Time time, float dtime)
    {
        m_pgameContext->GetCadetPlay()->ReceiveMessages();
    }

    bool   OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
	{ 
        if (ks.bDown)
            {
            switch (ks.vk)
                {
                case 'C':
                    ChatMessage chatMsg;
                    chatMsg.msgID = ChatMessageID;
                    chatMsg.cbMsg = sizeof(ChatMessage);
                    strcpy(chatMsg.szMsg, "This is a chat.");
                    m_pgameContext->GetCadetPlay()->SendMessage(&chatMsg);
                    return true;
                }
            }
		return BaseTestScene::OnKey(pprovider, ks, fForceTranslate);
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
                }
            }
    }

};


TRef<GameScene> CreateTestScene(CadetGameContext* pgameContext)
{
    return new TestScene3(pgameContext);
}

