#include "pch.h"
#include "terrain.h"

//////////////////////////////////////////////////////////////////////////////
//
// Right
//
//////////////////////////////////////////////////////////////////////////////
#define dTile 500.0f
#define hTile 100.0f

TRef<Geo> g_pgeoRight;

static Vertex s_verticesRight[16] =
    {
        Vertex(-(dTile/2), hTile, -dTile, 0, 1, 0,    0,    0),
        Vertex(-(dTile/4), hTile, -dTile, 0, 1, 0,    0, .25f),
        Vertex( (dTile/4), hTile, -dTile, 0, 1, 0,    0, .75f),
        Vertex( (dTile/2), hTile, -dTile, 0, 1, 0,    0,    1),
        Vertex(-(dTile/2), hTile,  -(3*dTile/4), 0, 1, 0, .25f,    0),
        Vertex(-(dTile/4),   0,  -(3*dTile/4), 0, 1, 0, .25f, .25f),
        Vertex( (dTile/4),   0,  -(3*dTile/4), 0, 1, 0, .25f, .75f),
        Vertex( (dTile/2),   0,  -(3*dTile/4), 0, 1, 0, .25f,    1),
        Vertex(-(dTile/2), hTile,  -(dTile/4), 0, 1, 0, .75f,    0),
        Vertex(-(dTile/4),   0,  -(dTile/4), 0, 1, 0, .75f, .25f),
        Vertex( (dTile/4),   0,  -(dTile/4), 0, 1, 0, .75f, .75f),
        Vertex( (dTile/2),   0,  -(dTile/4), 0, 1, 0, .75f,    1),
        Vertex(-(dTile/2), hTile,     0, 0, 1, 0,    1,    0),
        Vertex(-(dTile/4),   0,     0, 0, 1, 0,    1, .25f),
        Vertex( (dTile/4),   0,     0, 0, 1, 0,    1, .75f),
        Vertex( (dTile/2), hTile,     0, 0, 1, 0,    1,    1),
    };

static WORD s_indicesRight[54] = {
    0,5,4,0,1,5,
    1,2,5,2,6,5,
    2,3,6,3,7,6,
    4,5,8,5,9,8,
    5,6,9,6,10,9,
    6,7,10,7,11,10,
    8,9,12,9,13,12,
    9,10,13,10,14,13,
    10,15,14,10,11,15
};

Geo* GetRight()
{
    if (g_pgeoRight == NULL) 
        {

        TRef<GroupGeo> pgroupGeoConnector = GroupGeo::Create();
        pgroupGeoConnector->SetName("frm-conn1");

        Matrix matConn;
        Matrix matRot;
        Matrix matTran;
        matRot.SetRotateY(RadiansFromDegrees(-90));
        matTran.SetTranslate(Vector((dTile/2),0,-(dTile/2)));
        matConn.SetIdentity();
        matConn.Multiply(matRot);
        matConn.Multiply(matTran);

        TRef<Image> pimageTexture = GetModeler()->LoadImage("wallbmp", false);
        
        g_pgeoRight = GroupGeo::Create(
                    new TextureGeo(
                        Geo::CreateMesh(s_verticesRight, 16, s_indicesRight, 54, NULL),
                        pimageTexture
                        ),
                    new TransformGeo(
                        pgroupGeoConnector,
                        new MatrixTransform(matConn)
                        )
                     );
                   
    }

    return g_pgeoRight;
}

//////////////////////////////////////////////////////////////////////////////
//
// Straight
//
//////////////////////////////////////////////////////////////////////////////


TRef<Geo> g_pgeoStraight;

static Vertex s_verticesStraight[16] =
    {
        Vertex(-(dTile/2), hTile, -dTile, 0, 1, 0,    0,    0),
        Vertex(-(dTile/4),   0, -dTile, 0, 1, 0,    0, .25f),
        Vertex( (dTile/4),   0, -dTile, 0, 1, 0,    0, .75f),
        Vertex( (dTile/2), hTile, -dTile, 0, 1, 0,    0,    1),
        Vertex(-(dTile/2), hTile,  -(3*dTile/4), 0, 1, 0, .25f,    0),
        Vertex(-(dTile/4),   0,  -(3*dTile/4), 0, 1, 0, .25f, .25f),
        Vertex( (dTile/4),   0,  -(3*dTile/4), 0, 1, 0, .25f, .75f),
        Vertex( (dTile/2), hTile,  -(3*dTile/4), 0, 1, 0, .25f,    1),
        Vertex(-(dTile/2), hTile,  -(dTile/4), 0, 1, 0, .75f,    0),
        Vertex(-(dTile/4),   0,  -(dTile/4), 0, 1, 0, .75f, .25f),
        Vertex( (dTile/4),   0,  -(dTile/4), 0, 1, 0, .75f, .75f),
        Vertex( (dTile/2), hTile,  -(dTile/4), 0, 1, 0, .75f,    1),
        Vertex(-(dTile/2), hTile,     0, 0, 1, 0,    1,    0),
        Vertex(-(dTile/4),   0,     0, 0, 1, 0,    1, .25f),
        Vertex( (dTile/4),   0,     0, 0, 1, 0,    1, .75f),
        Vertex( (dTile/2), hTile,     0, 0, 1, 0,    1,    1),
    };

static WORD s_indicesStraight[54] = {
    0,5,4,0,1,5,
    1,2,5,2,6,5,
    2,3,6,3,7,6,
    4,5,8,5,9,8,
    5,6,9,6,10,9,
    6,7,10,7,11,10,
    8,9,12,9,13,12,
    9,10,13,10,14,13,
    10,15,14,10,11,15
};

Geo* GetStraight()
{
    if (g_pgeoStraight == NULL) 
        {

        TRef<GroupGeo> pgroupGeoConnector = GroupGeo::Create();
        pgroupGeoConnector->SetName("frm-conn1");

        Matrix matConn;
        matConn.SetTranslate(Vector(0,0,-dTile));

        TRef<Image> pimageTexture = GetModeler()->LoadImage("wallbmp", false);
        
        g_pgeoStraight = GroupGeo::Create(
                    new TextureGeo(
                        Geo::CreateMesh(s_verticesStraight, 16, s_indicesStraight, 54, NULL),
                        pimageTexture
                        ),
                    new TransformGeo(
                        pgroupGeoConnector,
                        new MatrixTransform(matConn)
                        )
                     );
                   
    }

    return g_pgeoStraight;
}

//////////////////////////////////////////////////////////////////////////////
//
// HeightMapData
//
//////////////////////////////////////////////////////////////////////////////
class HeightMapData
{
public:
    Vector  m_posMax;
    Vector  m_posMin;
    float   m_dMaxDiv;
    int     m_cXDiv;
    int     m_cZDiv;
    float   m_dxDiv;
    float   m_dzDiv;
    float*  m_pheights;
    int*    m_pcounts;
    
    HeightMapData() :
        m_posMax(0,0,0),
        m_posMin(0,0,0),
        m_dMaxDiv(25),
        m_pheights(NULL),
        m_pcounts(NULL)
    {
    }

    ~HeightMapData()
    {
        if (m_pheights)
            free(m_pheights);
        if (m_pcounts)
            free(m_pcounts);
    }

};

//////////////////////////////////////////////////////////////////////////////
//
// TerrainTileDataPriv
//
//////////////////////////////////////////////////////////////////////////////

class TerrainTileDataPriv : public TerrainTileData
{
public:
    virtual void AddConnection(const Matrix& mat) = 0;
    virtual HeightMapData& GetHeightMap() = 0;

};


//////////////////////////////////////////////////////////////////////////////
//
// TerrainGroupGeoCallback 
//
//////////////////////////////////////////////////////////////////////////////

class TerrainGroupGeoCallback : public GroupGeoCallback {
private:
    TRef<TerrainTileDataPriv>   m_ptileData;

public:
    TerrainGroupGeoCallback(TerrainTileDataPriv* ptileData) :
        m_ptileData(ptileData)
    {
    }

    TRef<Geo> Execute(const Matrix& mat, GroupGeo* pgroup)
    {
        ZString strName = pgroup->GetName();

        if (!strName.IsEmpty() && strName.Find("frm-") == 0 && !pgroup->AnyChildGroups())
            {
            strName = strName.RightOf(4);
            if (strName.Find("conn") != -1) 
                {
                m_ptileData->AddConnection(mat);
                }
            }

        return NULL;
    }


    // do points pA & pB fall on the same side of line p1,p2
    bool PointsDivided(const Point& pA, const Point& pB, const Point& p1, const Point& p2)
    {
        if (p2.X()==p1.X())
            {
            float dxA = pA.X() - p1.X();
            float dxB = pB.X() - p1.X();
            return (dxA*dxB < 0);
            }
        else
            {
            float slope = (p2.Y()-p1.Y())/(p2.X()-p1.X());
            float y0 = p1.Y()-slope*p1.X();
            float dyA = pA.Y() - (slope*pA.X() + y0);
            float dyB = pB.Y() - (slope*pB.X() + y0);
            return (dyA*dyB < 0);
            }
    }

    // is point p contained within triangle p1,p2,p3
    bool PointInTriangle(const Point& p, const Point& p1, const Point& p2, const Point& p3)
    {
        return (!PointsDivided(p, p1, p2, p3) 
                && !PointsDivided(p, p2, p1, p3)
                && !PointsDivided(p, p3, p1, p2));
    }
    
    void ReportTriangles(const Matrix& mat, const TVector<Vertex>&  vertices, const TVector<WORD>& indices)
    {
        HeightMapData& hm = m_ptileData->GetHeightMap();
        
        // get bounding box
        int vcount = vertices.GetCount();
        for (int i=0; i<vcount; i++)
            {
            if (vertices[i].x > hm.m_posMax.X())
                hm.m_posMax.SetX(vertices[i].x);
            if (vertices[i].x < hm.m_posMin.X())
                hm.m_posMin.SetX(vertices[i].x);
            if (vertices[i].y > hm.m_posMax.Y())
                hm.m_posMax.SetY(vertices[i].y);
            if (vertices[i].y < hm.m_posMin.Y())
                hm.m_posMin.SetY(vertices[i].y);
            if (vertices[i].z > hm.m_posMax.Z())
                hm.m_posMax.SetZ(vertices[i].z);
            if (vertices[i].z < hm.m_posMin.Z())
                hm.m_posMin.SetZ(vertices[i].z);
            }

        // calc grid
        hm.m_cXDiv = (int)((hm.m_posMax.X() - hm.m_posMin.X())/hm.m_dMaxDiv + 1);
        hm.m_cZDiv = (int)((hm.m_posMax.Z() - hm.m_posMin.Z())/hm.m_dMaxDiv + 1);
        hm.m_dxDiv = (hm.m_posMax.X() - hm.m_posMin.X())/hm.m_cXDiv;
        hm.m_dzDiv = (hm.m_posMax.Z() - hm.m_posMin.Z())/hm.m_cZDiv;
        hm.m_pheights = (float*)malloc(sizeof(float)*(hm.m_cXDiv+1)*(hm.m_cZDiv+1));
        ZeroMemory(hm.m_pheights, sizeof(float)*(hm.m_cXDiv+1)*(hm.m_cZDiv+1));
        hm.m_pcounts = (int*)malloc(sizeof(int)*(hm.m_cXDiv+1)*(hm.m_cZDiv+1));
        ZeroMemory(hm.m_pcounts, sizeof(float)*(hm.m_cXDiv+1)*(hm.m_cZDiv+1));

        
        // for each point on grid
        for (int iX = 0; iX <= hm.m_cXDiv; iX++)
            for (int iZ = 0; iZ <= hm.m_cXDiv; iZ++)
                {
                Point pt(hm.m_posMin.X() + iX*hm.m_dxDiv, hm.m_posMin.Z() + iZ*hm.m_dzDiv);

                // check each triangle projected onto xz plane
                int cTriangles = indices.GetCount()/3;
                for (int iTriangle = 0; iTriangle < cTriangles; iTriangle++)
                    {
                    Point pt1(vertices[indices[iTriangle*3]].x, vertices[indices[iTriangle*3]].z);
                    Point pt2(vertices[indices[iTriangle*3+1]].x, vertices[indices[iTriangle*3+1]].z);
                    Point pt3(vertices[indices[iTriangle*3+2]].x, vertices[indices[iTriangle*3+2]].z);
                    if (PointInTriangle(pt, pt1, pt2, pt3))
                        {
                        hm.m_pheights[iX + iZ*(hm.m_cXDiv+1)] = vertices[indices[iTriangle*3]].y+vertices[indices[iTriangle*3+1]].y+vertices[indices[iTriangle*3+2]].y/3;
                        hm.m_pcounts[iX + iZ*(hm.m_cXDiv+1)]++;
                        }
                    }
                }
    }

    void ReportTriangles(const Matrix& mat, const TVector<VertexL>& vertices, const TVector<WORD>& indices)
    {
        ZAssert(false);
    }

    void ReportTriangles(const Matrix& mat, const TVector<D3DLVertex>& vertices, const TVector<WORD>& indices)
    {
        ZAssert(false);
    }

    void ReportTriangles(const Matrix& mat, const TVector<D3DVertex>& vertices, const TVector<WORD>& indices)
    {
        ZAssert(false);
    }

};


//////////////////////////////////////////////////////////////////////////////
//
// TerrainTileDataImpl
//
//////////////////////////////////////////////////////////////////////////////

class TerrainTileDataImpl : public TerrainTileDataPriv
{
private:
    TRef<Geo> m_pgeoTile;
    TVector<Matrix> m_rgMatrixConnections;
    HeightMapData m_heightMap;
    
public:

    TerrainTileDataImpl(Geo* pgeoTile) :
        m_pgeoTile(pgeoTile)
    {
        AddRef();
        TRef<TerrainGroupGeoCallback> pcallback = new TerrainGroupGeoCallback(this);
        m_pgeoTile->CallGroupGeoCallback(Matrix::GetIdentity(), pcallback);
        pcallback = NULL;
        Internal_Release();
    }

    bool HitTest(const Vector& pos)
    {
        if (pos.x > m_heightMap.m_posMax.X()
                || pos.z > m_heightMap.m_posMax.Z()
                || pos.x < m_heightMap.m_posMin.X()
                || pos.z < m_heightMap.m_posMin.Z())
            return false;

        int iX = (int)((pos.x - m_heightMap.m_posMin.X())/m_heightMap.m_dxDiv);
        int iZ = (int)((pos.z - m_heightMap.m_posMin.Z())/m_heightMap.m_dzDiv);
        float y = m_heightMap.m_pheights[iX + iZ*(m_heightMap.m_cXDiv+1)];

/*        if (pos.y < y)
            {
            debugf("Collided with tile at: (%f,%f,%f) iX:%d iZ:%d Y:%f  \r\n",pos.x,pos.y,pos.z,iX,iZ,y);
            }
        else
            {
            debugf("Didn't collided with tile at: (%f,%f,%f) iX:%d iZ:%d Y:%f  \r\n",pos.x,pos.y,pos.z,iX,iZ,y);
            }
*/
        return (pos.y < y);
    }        

    void AddConnection(const Matrix& mat)
    {
        m_rgMatrixConnections.PushEnd(mat);
    }

    HeightMapData& GetHeightMap()
    {
        return m_heightMap;
    }

    int GetNumConnections()
    {
        return m_rgMatrixConnections.GetCount();
    }

    const Matrix& GetConnectionMatrix(int iConnection)
    {
        return m_rgMatrixConnections[iConnection];
    }

    Geo* GetGeo()
    {
        return m_pgeoTile;
    }

};

TRef<TerrainTileData> TerrainTileData::Create(Geo* pgeoTile)
{
     return new TerrainTileDataImpl(pgeoTile);
}


//////////////////////////////////////////////////////////////////////////////
//
// TerrainTileImpl
//
//////////////////////////////////////////////////////////////////////////////

class TerrainTileImpl : public TerrainTile
{
protected:
    Matrix                              m_matTile;  // transformation of world coordinates to tile
    Matrix                              m_matWorld; // transformation of tile coordinates to world
    TRef<TerrainTileDataImpl>           m_ptileData;
    TVector<TRef<TerrainTileImpl> >     m_rgTileConnections;
    TRef<TerrainTileImpl>               m_ptileSource;

public:

    TerrainTileImpl(TerrainTileDataImpl* ptileData) :
        m_ptileData(ptileData)
    {
        m_matTile.SetIdentity();
        m_matWorld.SetIdentity();
        m_rgTileConnections.SetCount(m_ptileData->GetNumConnections());
    }

    void Connect(TerrainTileImpl* ptileDest, int iConnection)
    {
        m_rgTileConnections.Set(iConnection, ptileDest);
        Matrix matConnection;
        matConnection.SetMultiply(m_ptileData->GetConnectionMatrix(iConnection), m_matTile);
        ptileDest->SetSource(this, matConnection);
    }

    void SetSource(TerrainTileImpl* ptile, const Matrix& matTile)
    {
        m_matTile = matTile;
        m_matWorld.SetInverse(m_matTile);
        m_ptileSource = ptile;
    }

    bool HitTest(const Vector& posWorld)
    {
        Vector posTile = m_matWorld.Transform(posWorld);
        return m_ptileData->HitTest(posTile);
    }

    const Matrix& GetMatrixTile()
    {
        return m_matTile;
    }

    const Matrix& GetMatrixWorld()
    {
        return m_matWorld;
    }


    TerrainTileDataImpl* GetTileData()
    {
        return m_ptileData;
    }
    
};

//////////////////////////////////////////////////////////////////////////////
//
// TerrainGeoImpl
//
//////////////////////////////////////////////////////////////////////////////

class TerrainGeoImpl : public TerrainGeo
{
private:
    TRef<GroupGeo> m_pgroupGeo;
    
public:

    TerrainGeoImpl()
    {
        m_pgroupGeo = GroupGeo::Create();
        SetGeo(m_pgroupGeo);
    }

    void AddTile(TerrainTileImpl* ptile)
    {
        m_pgroupGeo->AddGeo(
                        new TransformGeo(
                            ptile->GetTileData()->GetGeo(),
                            new MatrixTransform(ptile->GetMatrixTile())
                        )
                    );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// TerrainMapImpl
//
//////////////////////////////////////////////////////////////////////////////

class TerrainMapImpl : public TerrainMap
{
private:
    TList<TRef<TerrainTileImpl> >   m_lstTiles;
    TRef<TerrainGeoImpl>        m_pterrainGeo;

public:

    TerrainMapImpl()
    {
        m_pterrainGeo = new TerrainGeoImpl();
    }        

    bool HitTest(const Vector& posWorld)
    {
        TList<TRef<TerrainTileImpl> >::Iterator iter(m_lstTiles);
        while (!iter.End()) 
            {
                if (iter.Value()->HitTest(posWorld))
                    return true;
                iter.Next();
            }
        return false;            
    }

    TRef<TerrainTile> CreateTile(TerrainTileData* ptileData)
    {
        TRef<TerrainTileDataImpl> ptileDataImpl; CastTo(ptileDataImpl, ptileData);
        TRef<TerrainTileImpl> ptile = new TerrainTileImpl(ptileDataImpl);
        m_lstTiles.PushEnd(ptile);
        return ptile;
    }

    TRef<TerrainTile> Connect(TerrainTile* ptileSrc, int iConnectionSrc, TerrainTile* ptileDest)
    {
        TRef<TerrainTileImpl> ptileImplSrc; CastTo(ptileImplSrc, ptileSrc);
        TRef<TerrainTileImpl> ptileImplDest; CastTo(ptileImplDest, ptileDest);
        ptileImplSrc->Connect(ptileImplDest, iConnectionSrc);
        m_pterrainGeo->AddTile(ptileImplDest);
        return ptileDest;
    }
    
    TRef<TerrainTile> SetRoot(TerrainTile* ptile)
    {
        TRef<TerrainTileImpl> ptileImpl; CastTo(ptileImpl, ptile);
        m_pterrainGeo->AddTile(ptileImpl);
        return ptile;
    }

    TRef<TerrainGeo> GetGeo()
    {
        return m_pterrainGeo;
    }

};

TRef<TerrainMap> TerrainMap::Create()
{
    return new TerrainMapImpl();
}



