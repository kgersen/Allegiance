#include "geometry.h"


#include <alloc.h>
#include <base.h>
#include <tmap.h>

#include "surface.h"

//////////////////////////////////////////////////////////////////////////////
//
// Empty Geo
//
//////////////////////////////////////////////////////////////////////////////

class EmptyGeo : public Geo {
public:
    EmptyGeo() : Geo()
    {
    }

    //
    // Value members
    //

	ZString GetString(int indent) { return "emptyGeo"; }
    ZString GetFunctionName() 
    { 
        return "emptyGeo"; 
    }

    void Write(IMDLBinaryFile* pfile)
    {
        pfile->WriteReference("emptyGeo");
    }
};

static TRef<Geo> g_pemptyGeo = new EmptyGeo();

Geo* Geo::GetEmpty()
{
    return g_pemptyGeo;
}

//////////////////////////////////////////////////////////////////////////////
//
// Geo
//
//////////////////////////////////////////////////////////////////////////////

void Geo::SetEmpty()
{
    ChangeTo(GetEmpty());
}

void Geo::Render(Context* pcontext)
{
}

bool Geo::AnyChildGroups(bool bHead)
{
    return false;
}

TRef<Geo> Geo::RemoveMaterial()
{
    return NULL;
}

TRef<Geo> Geo::FoldTexture()
{
    return NULL;
}

MeshGeo* Geo::GetMeshGeo()
{
    return NULL;
}

GroupGeo* Geo::GetGroupGeo()
{
    return NULL;
}

TextureGeo* Geo::GetTextureGeo()
{
    return NULL;
}

Image* Geo::FindTexture()
{
    return NULL;
}

void Geo::CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback)
{
}

float Geo::GetRadius(const Matrix& mat)
{
    return 0;
}

void Geo::RemoveCapture()
{
}

bool Geo::HitTest(const Vector& vecOrigin, const Vector& vecRay, HitData3D& hitData, bool bCaptured)
{
    return false;
}

void Geo::MouseMove(const Vector& vec, bool bCaptured, bool bInside)
{
}

void Geo::MouseEnter(const Vector& vec)
{
}

void Geo::MouseLeave()
{
}

MouseResult Geo::Button(const Vector& vec, int button, bool bCaptured, bool bInside, bool bDown)
{
    return MouseResult();
}

TRef<Geo> Geo::ApplyTransform(Transform* ptrans)
{
    return NULL;
}

int Geo::GetTriangleCount()
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// TMeshGeo
//
//////////////////////////////////////////////////////////////////////////////

ZString GetString(const Vertex& v)
{
    return
          ZString(v.x)  + ", "
        + ZString(v.y)  + ", "
        + ZString(v.z)  + ", "
        + ZString(v.nx) + ", "
        + ZString(v.ny) + ", "
        + ZString(v.nz) + ", "
        + ZString(v.u)  + ", "
        + ZString(v.v);
}

ZString GetString(const VertexL& v)
{
    return
          ZString(v.x) + ", "
        + ZString(v.y) + ", "
        + ZString(v.z) + ", "
        + ZString(v.u) + ", "
        + ZString(v.v);
}

//////////////////////////////////////////////////////////////////////////////
//
// TMeshGeo
//
//////////////////////////////////////////////////////////////////////////////

class MeshGeo : public Geo {
public:
    virtual TRef<MeshGeo> Combine(MeshGeo* pmesh) = 0;
};

template<class VertexType>
class TMeshGeo : public MeshGeo {
    TRef<IObject>       m_pobjectMemory;
    TVector<VertexType> m_vertices;
    TVector<MeshIndex>  m_indices;
	CVBIBManager::SVBIBHandle m_hVB;
	CVBIBManager::SVBIBHandle m_hIB;
	DWORD m_dwFVF;
	D3DFORMAT m_d3dIndexFormat;

public:
	// D3DVertex
    TMeshGeo(D3DVertex* pvertices, int vcount, MeshIndex* pindices, int icount, DWORD dwFVF = 0, IObject* pobjectMemory = NULL) :
        m_pobjectMemory(pobjectMemory),
        m_vertices(vcount, pvertices, pobjectMemory != NULL),
        m_indices(icount, pindices, pobjectMemory != NULL)
    {
		CVBIBManager::Get()->AddVerticesToBuffer(
					&m_hVB,
					(BYTE*) pvertices,
					vcount,
					sizeof( D3DVertex ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(
					&m_hIB,
					(BYTE*) pindices,
					icount,
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

    TMeshGeo(const TVector<D3DVertex>& vertices, const TVector<MeshIndex>& indices, DWORD dwFVF ) :
        m_vertices(vertices),
        m_indices(indices)
    {
		CVBIBManager::Get()->AddVerticesToBuffer(
					&m_hVB,
					(BYTE*) &m_vertices.GetFront(),
					m_vertices.GetCount(),
					sizeof( D3DVertex ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(
					&m_hIB,
					(BYTE*) &m_indices.GetFront(),
					m_indices.GetCount(),
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

	// Vertex
    TMeshGeo(Vertex* pvertices, int vcount, MeshIndex* pindices, int icount, DWORD dwFVF = 0, IObject* pobjectMemory = NULL) :
        m_pobjectMemory(pobjectMemory),
        m_vertices(vcount, pvertices, pobjectMemory != NULL),
        m_indices(icount, pindices, pobjectMemory != NULL)
    {
		CVBIBManager::Get()->AddLegacyVerticesToBuffer(
					&m_hVB,
					(BYTE*) pvertices,
					vcount,
					sizeof( Vertex ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(
					&m_hIB,
					(BYTE*) pindices,
					icount,
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

    TMeshGeo(const TVector<Vertex>& vertices, const TVector<MeshIndex>& indices, DWORD dwFVF ) :
        m_vertices(vertices),
        m_indices(indices)
    {
		CVBIBManager::Get()->AddLegacyVerticesToBuffer(
					&m_hVB,
					(BYTE*) &m_vertices.GetFront(),
					m_vertices.GetCount(),
					sizeof( Vertex ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(
					&m_hIB,
					(BYTE*) &m_indices.GetFront(),
					m_indices.GetCount(),
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

	// LVertex - prelit vertices.
    TMeshGeo(VertexL * pvertices, int vcount, MeshIndex* pindices, int icount, DWORD dwFVF = 0, IObject* pobjectMemory = NULL) :
        m_pobjectMemory(pobjectMemory),
        m_vertices(vcount, pvertices, pobjectMemory != NULL),
        m_indices(icount, pindices, pobjectMemory != NULL)
    {
		CVBIBManager::Get()->AddLegacyLitVerticesToBuffer(	
					&m_hVB,
					(BYTE*) pvertices,
					vcount,
					sizeof( VertexL ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(
					&m_hIB,
					(BYTE*) pindices,
					icount,
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

    TMeshGeo(const TVector<VertexL>& vertices, const TVector<MeshIndex>& indices, DWORD dwFVF ) :
        m_vertices(vertices),
        m_indices(indices)
    {
		CVBIBManager::Get()->AddLegacyLitVerticesToBuffer(
					&m_hVB,
					(BYTE*) &m_vertices.GetFront(),
					m_vertices.GetCount(),
					sizeof( VertexL ),
					dwFVF );
		CVBIBManager::Get()->AddIndicesToBuffer(	
					&m_hIB,
					(BYTE*) &m_indices.GetFront(),
					m_indices.GetCount(),
					D3DFMT_INDEX16 );
		m_dwFVF = dwFVF;
		m_d3dIndexFormat = D3DFMT_INDEX16;
    }

    void Render(Context* pcontext)
    {
//        pcontext->DrawTriangles(m_vertices, m_indices);
		pcontext->DrawTriangles( &m_hVB, &m_hIB ); //NEW RENDER PATH
    }

    void CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback)
    {
        pcallback->ReportTriangles(mat, m_vertices, m_indices);
    }

    MeshGeo* GetMeshGeo()
    {
        return this;
    }

    TRef<MeshGeo> Combine(MeshGeo* pmeshArg)
    {
        TMeshGeo* pmesh; CastTo(pmesh, pmeshArg);

        TVector<VertexType> vertices(pmesh->m_vertices);
        TVector<MeshIndex>       indices(pmesh->m_indices);

        int countSource = m_vertices.GetCount();
        int indexSource;
        TVector<MeshIndex> indexMap(countSource, countSource);

        //
        // Combine any common vertices
        //

        for(indexSource = 0; indexSource < countSource; indexSource++) {
            int count = vertices.GetCount();
            int index = 0;
            while (true) {
                if (m_vertices[indexSource] == vertices[index]) {
                    indexMap.Set(indexSource, index);
                    break;
                }
                index++;
                if (index == count) {
                    indexMap.Set(indexSource, count);
                    vertices.PushEnd(m_vertices[indexSource]);
                    break;
                }
            }
        }

        //
        // Map the indices to the new vertex list
        //

        countSource = m_indices.GetCount();
        for(indexSource = 0; indexSource < countSource; indexSource++) {
            indices.PushEnd(indexMap[m_indices[indexSource]]);
        }

        //
        // Create the new Geo
        //

        return new TMeshGeo(vertices, indices, pmesh->m_dwFVF );
    }

    TRef<Geo> ApplyTransform(Transform* ptrans)
    {
        Matrix mat = ptrans->GetValue();

        Matrix matNormal;
        matNormal.SetInverse(mat);
        matNormal.Transpose();

        TVector<VertexType> vertices(m_vertices);

        int count = m_vertices.GetCount();
        for(int index = 0; index < count; index++) {
            vertices.Get(index).SetPosition(
                mat.Transform(
                    m_vertices[index].GetPosition()
                )
            );

            vertices.Get(index).SetNormal(
                matNormal.Transform(
                    m_vertices[index].GetNormal()
                )
            );
        }

        return new TMeshGeo(vertices, m_indices, m_dwFVF);
    }

    int GetTriangleCount()
    {
        return m_indices.GetCount() / 3;
    }

    float GetRadius(const Matrix& mat)
    {
        float radius = 0;
        int count = m_vertices.GetCount();
        for(int index = 0; index < count; index++) {
            Vector vec = mat.Transform(m_vertices[index].GetPosition());
            radius = std::max(radius, vec.Length());
        }

        return radius;
    }

    ZString GetString(int indent)
    {
        // Mesh( [(vertices, normals, texture coordinates)], [indices])

        ZString str = "MeshGeo([\n";

        int count = m_vertices.GetCount();
        for(int index = 0; index < count; index++) {
            str += Indent(indent + 1) + ::GetString(m_vertices[index]);

            if (index + 1 == count) {
                str += "\n";
            } else {
                str += ",\n";
            }
        }

        str += Indent(indent) + "], [\n";

        count = m_indices.GetCount();
        for(int index = 0; index < count; index += 3) {
            str +=
                  Indent(indent + 1)
                + ZString((int)m_indices[index]) + ", "
                + ZString((int)m_indices[index + 1]) + ", "
                + ZString((int)m_indices[index + 2]);

            if (index + 3 == count) {
                str += "\n";
            } else {
                str += ",\n";
            }
        }

        return str + Indent(indent) + "])";
    }

    //
    // Value members
    //

    bool IsConstant()
    {
        return true;
    }

    ZString GetFunctionName() { return "MeshGeo"; }

    void Write(IMDLBinaryFile* pmdlFile)
    {
        pmdlFile->WriteReference("MeshGeo");
        TRef<ZFile> pfile = pmdlFile->WriteBinary();

        pfile->Write(m_vertices.GetCount());
        pfile->Write(m_indices.GetCount());
        pfile->Write((void*)&(m_vertices[0]), sizeof(VertexType) * m_vertices.GetCount());
        pfile->Write((void*)&(m_indices[0]),  sizeof(MeshIndex) * m_indices.GetCount());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// TMeshGeo
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> Geo::CreateMesh(
    Vertex* pvertices,
    int vcount,
    MeshIndex* pindices,
    int icount,
    IObject* pobjectMemory
)
{
    return new TMeshGeo<Vertex>(pvertices, vcount, pindices, icount, D3DFVF_VERTEX, pobjectMemory);
}

TRef<Geo> Geo::CreateMesh(
    D3DVertex* pvertices,
    int vcount,
    MeshIndex* pindices,
    int icount,
    IObject* pobjectMemory
)
{
    return new TMeshGeo<D3DVertex>(pvertices, vcount, pindices, icount, D3DFVF_VERTEX, pobjectMemory);
}

TRef<Geo> Geo::CreateMesh(const TVector<Vertex>& vertices, const TVector<MeshIndex>& indices)
{
    return new TMeshGeo<Vertex>(vertices, indices, D3DFVF_VERTEX);
}

TRef<Geo> Geo::CreateMesh(const TVector<VertexL>& vertices, const TVector<MeshIndex>& indices)
{
    return new TMeshGeo<VertexL>(vertices, indices, D3DFVF_LVERTEX);
}

TRef<Geo> Geo::CreateMesh(const TVector<D3DVertex>& vertices, const TVector<MeshIndex>& indices)
{
    return new TMeshGeo<D3DVertex>(vertices, indices, D3DFVF_VERTEX);
}

TRef<Geo> CreateStaticMesh(VertexL* pvertices, int vcount, MeshIndex* pindices, int icount)
{
    return new TMeshGeo<VertexL>(pvertices, vcount, pindices, icount, D3DFVF_LVERTEX);
}

TRef<Geo> CreateStaticMesh(Vertex* pvertices, int vcount, MeshIndex* pindices, int icount)
{
    return new TMeshGeo<Vertex>(pvertices, vcount, pindices, icount, D3DFVF_VERTEX);
}

//////////////////////////////////////////////////////////////////////////////
//
// Mesh Objects
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> g_pmeshSquare;
TRef<Geo> g_pmeshIcosahedron;
TRef<Geo> g_pmeshWhiteEmissiveSquare;

Geo* Geo::GetWhiteEmissiveSquare()
{
    if (g_pmeshWhiteEmissiveSquare == NULL) {
        static VertexL vertices[4] =
            {
                VertexL(-1,  1, 0, 1, 1, 1, 1, 0, 0),
                VertexL(-1, -1, 0, 1, 1, 1, 1, 0, 1),
                VertexL( 1, -1, 0, 1, 1, 1, 1, 1, 1),
                VertexL( 1,  1, 0, 1, 1, 1, 1, 1, 0)
            };
        static MeshIndex indices[12] = {
            0, 2, 1, 0, 3, 2,
            0, 1, 2, 0, 2, 3
        };
        g_pmeshWhiteEmissiveSquare = CreateStaticMesh(vertices, 4, indices, 12);
    }

    return g_pmeshWhiteEmissiveSquare;
}

Geo* Geo::GetSquare()
{
    if (g_pmeshSquare == NULL) {
        static Vertex vertices[4] =
            {
                Vertex(-1,  1, 0, 0, 0, 1, 0, 0),
                Vertex(-1, -1, 0, 0, 0, 1, 0, 1),
                Vertex( 1, -1, 0, 0, 0, 1, 1, 1),
                Vertex( 1,  1, 0, 0, 0, 1, 1, 0)
            };

        static MeshIndex indices[12] = {
            0, 2, 1, 0, 3, 2,
            0, 1, 2, 0, 2, 3
        };
        g_pmeshSquare = CreateStaticMesh(vertices, 4, indices, 12 );
      }

    return g_pmeshSquare;
}

Geo* Geo::GetIcosahedron()
{
    if (g_pmeshIcosahedron == NULL) {
        const float x = 0.5256f;
        const float z = 0.8506f;

        Vertex pvertices[] =
            {
                Vertex(  -x, 0.0f,   z,   -x, 0.0f,   z),
                Vertex(   x, 0.0f,   z,    x, 0.0f,   z),
                Vertex(  -x, 0.0f,  -z,   -x, 0.0f,  -z),
                Vertex(   x, 0.0f,  -z,    x, 0.0f,  -z),
                Vertex(0.0f,    z,   x, 0.0f,    z,   x),
                Vertex(0.0f,    z,  -x, 0.0f,    z,  -x),
                Vertex(0.0f,   -z,   x, 0.0f,   -z,   x),
                Vertex(0.0f,   -z,  -x, 0.0f,   -z,  -x),
                Vertex(   z,   x, 0.0f,    z,   x, 0.0f),
                Vertex(  -z,   x, 0.0f,   -z,   x, 0.0f),
                Vertex(   z,  -x, 0.0f,    z,  -x, 0.0f),
                Vertex(  -z,  -x, 0.0f,   -z,  -x, 0.0f)
            };

        MeshIndex pindices[] = {
            0, 4, 1,
            0, 9, 4,
            9, 5, 4,
            4, 5, 8,
            4, 8, 1,
            8, 10, 1,
            8, 3, 10,
            5, 3, 8,
            5, 2, 3,
            2, 7, 3,
            7, 10, 3,
            7, 6, 10,
            7, 11, 6,
            11, 0, 6,
            0, 1, 6,
            6, 1, 10,
            9, 0, 11,
            9, 11, 2,
            9, 2, 5,
            7, 2, 11
        };

        g_pmeshIcosahedron =
            CreateStaticMesh(
                pvertices,
                ArrayCount(pvertices),
                pindices,
                ArrayCount(pindices)
            );

        /*
        Vertex pvertices[] =
            {
                Vertex(-1,  0,  1, 0, 0, 1),
                Vertex(-1, -1,  1, 0, 0, 1),
                Vertex( 1,  0, -1, 0, 0, 1)
            };

        MeshIndex pindices[] = { 0, 2, 1 };

        g_pmeshIcosahedron =
            CreateStaticMesh(
                pvertices,
                3,
                pindices,
                3
            );
        */
    }

    return g_pmeshIcosahedron;
}

//////////////////////////////////////////////////////////////////////////////
//
// Wrap Geo
//
//////////////////////////////////////////////////////////////////////////////

WrapGeo::WrapGeo(Geo* pgeo) :
    Geo(pgeo)
{
}

WrapGeo::WrapGeo(Geo* pgeo, Value* pvalue1) :
    Geo(pgeo, pvalue1)
{
}

TRef<WrapGeo> WrapGeo::Duplicate(Geo* pgeo)
{
    return new WrapGeo(pgeo);
}

TRef<Geo> WrapGeo::ApplyTransform(Transform* ptrans)
{
    TRef<Geo> pgeo = GetGeo()->ApplyTransform(ptrans);

    if (pgeo) {
        return Duplicate(pgeo);
    }

    return NULL;
}

bool WrapGeo::AnyChildGroups(bool bHead)
{
    return GetGeo()->AnyChildGroups(false);
}

TRef<Geo> WrapGeo::RemoveMaterial()
{
    TRef<Geo> pgeo = GetGeo()->RemoveMaterial();

    if (pgeo) {
        SetGeo(pgeo);
        return this;
    }

    return NULL;
}

TRef<Geo> WrapGeo::FoldTexture()
{
    bool bAnyFold = false;

    TRef<Geo> pgeo = GetGeo();
    TRef<Geo> pgeoFold = pgeo->FoldTexture();

    if (pgeoFold) {
        bAnyFold = true;
        SetGeo(pgeoFold);
    } else {
        pgeoFold = pgeo;
    }

    TRef<TextureGeo> ptextureGeo = pgeoFold->GetTextureGeo();

    if (ptextureGeo) {
        return
            new TextureGeo(
                Duplicate(ptextureGeo->GetGeo()),
                ptextureGeo->GetTexture(),
                false
            );
    }

    if (bAnyFold) {
        return this;
    } else {
        return NULL;
    }
}

TRef<Value> WrapGeo::Fold()
{
    if (GetGeo() == Geo::GetEmpty()) {
        return Geo::GetEmpty();
    }

    return NULL;
}

void WrapGeo::Render(Context* pcontext)
{
    GetGeo()->Render(pcontext);
}

Image* WrapGeo::FindTexture()
{
    return GetGeo()->FindTexture();
}

void WrapGeo::CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback)
{
    GetGeo()->CallGroupGeoCallback(mat, pcallback);
}

float WrapGeo::GetRadius(const Matrix& mat)
{
    return GetGeo()->GetRadius(mat);
}

int WrapGeo::GetTriangleCount()
{
    return GetGeo()->GetTriangleCount();
}

void WrapGeo::RemoveCapture()
{
    GetGeo()->RemoveCapture();
}

bool WrapGeo::HitTest(const Vector& vecOrigin, const Vector& vecRay, HitData3D& hitData, bool bCaptured)
{
    return GetGeo()->HitTest(vecOrigin, vecRay, hitData, bCaptured);
}

void WrapGeo::MouseMove(const Vector& vec, bool bCaptured, bool bInside)
{
    GetGeo()->MouseMove(vec, bCaptured, bInside);
}

void WrapGeo::MouseEnter(const Vector& vec)
{
    GetGeo()->MouseEnter(vec);
}

void WrapGeo::MouseLeave()
{
    GetGeo()->MouseLeave();
}

MouseResult WrapGeo::Button(const Vector& vec, int button, bool bCaptured, bool bInside, bool bDown)
{
    return GetGeo()->Button(vec, button, bCaptured, bInside, bDown);
}

//////////////////////////////////////////////////////////////////////////////
//
// DecalGeo
//
//////////////////////////////////////////////////////////////////////////////

DecalGeo::DecalGeo(
    Image* pimage,
    const Color& color,
    const Vector& vecPosition,
    const Vector& vecForward,
    const Vector& vecRight,
    float scale,
    float angle
) :
    Geo(pimage),
	m_color(color),
    m_vecPosition(vecPosition),
    m_vecForward(vecForward),
    m_vecRight(vecRight),
    m_scale(scale),
    m_angle(0)
{
}

void DecalGeo::Render(Context* pcontext)
{
    TRef<Surface> psurface = GetImage()->GetSurface();

    pcontext->DrawDecal(
        psurface,
        m_color,
        m_vecPosition,
        m_vecForward,
        m_vecRight,
        m_scale,
        m_angle
    );
}

//////////////////////////////////////////////////////////////////////////////
//
// Visible Geo
//
//////////////////////////////////////////////////////////////////////////////

VisibleGeo::VisibleGeo(Geo* pgeo) :
    WrapGeo(pgeo),
    m_bVisible(true)
{
}

TRef<WrapGeo> VisibleGeo::Duplicate(Geo* pgeo)
{
    return new VisibleGeo(pgeo);
}

void VisibleGeo::Render(Context* pcontext)
{
    if (m_bVisible) {
        GetGeo()->Render(pcontext);
    }
}

void VisibleGeo::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

//////////////////////////////////////////////////////////////////////////////
//
// Clip Geo
//
//////////////////////////////////////////////////////////////////////////////

ClipGeo::ClipGeo(Geo* pgeo) :
    WrapGeo(pgeo),
    m_bClip(true)
{
}

TRef<WrapGeo> ClipGeo::Duplicate(Geo* pgeo)
{
    return new ClipGeo(pgeo);
}

void ClipGeo::Render(Context* pcontext)
{
    bool bClipSave = pcontext->GetClipping();
    pcontext->SetClipping(m_bClip);
    GetGeo()->Render(pcontext);
    pcontext->SetClipping(bClipSave);
}

void ClipGeo::SetClip(bool bClip)
{
    m_bClip = bClip;
}

//////////////////////////////////////////////////////////////////////////////
//
// ClipPlane Geo
//
//////////////////////////////////////////////////////////////////////////////

ClipPlaneGeo::ClipPlaneGeo(Geo* pgeo, const Plane& plane) :
    WrapGeo(pgeo),
    m_plane(plane)
{
}

TRef<WrapGeo> ClipPlaneGeo::Duplicate(Geo* pgeo)
{
    return new ClipPlaneGeo(pgeo, m_plane);
}

void ClipPlaneGeo::Render(Context* pcontext)
{
    pcontext->SetCullMode(CullModeNone);
    pcontext->AddClipPlane(m_plane);
    GetGeo()->Render(pcontext);
}

void ClipPlaneGeo::SetClipPlane(const Plane& plane)
{
    m_plane = plane;
}

//////////////////////////////////////////////////////////////////////////////
//
// Transform Geo
//
//////////////////////////////////////////////////////////////////////////////

TransformGeo::TransformGeo(Geo* pgeo, Transform* ptrans) :
    WrapGeo(pgeo, ptrans)
{
}

TRef<WrapGeo> TransformGeo::Duplicate(Geo* pgeo)
{
    return new TransformGeo(pgeo, GetTransform());
}

void TransformGeo::Render(Context* pcontext)
{
    pcontext->Multiply(GetTransform()->GetValue());
    GetGeo()->Render(pcontext);
}

float TransformGeo::GetRadius(const Matrix& matArg)
{
    Matrix mat;

    mat.SetMultiply(GetTransform()->GetValue(), matArg);

    return GetGeo()->GetRadius(mat);
}

void TransformGeo::CallGroupGeoCallback(const Matrix& matArg, GroupGeoCallback* pcallback)
{
    Matrix mat;

    mat.SetMultiply(GetTransform()->GetValue(), matArg);

    GetGeo()->CallGroupGeoCallback(mat, pcallback);
}

TRef<Value> TransformGeo::Fold()
{
    TRef<Geo>       pgeo   = GetGeo();
    TRef<Transform> ptrans = GetTransform();

    if (ptrans->IsConstant()) {
        if (ptrans->IsIdentity()) {
            return pgeo;
        } else {
            TRef<Geo> pgeoNew = pgeo->ApplyTransform(ptrans);
            if (pgeoNew) {
                return pgeoNew;
            }
        }
    }

    return WrapGeo::Fold();
}

//////////////////////////////////////////////////////////////////////////////
//
// Material Geo
//
//////////////////////////////////////////////////////////////////////////////

MaterialGeo::MaterialGeo(Geo* pgeo, Material* pmaterial, bool bOverride) :
    WrapGeo(pgeo, pmaterial),
    m_bOverride(bOverride)
{
}

TRef<WrapGeo> MaterialGeo::Duplicate(Geo* pgeo)
{
    return new MaterialGeo(pgeo, GetMaterial(), m_bOverride);
}

TRef<Geo> MaterialGeo::RemoveMaterial()
{
    return GetGeo();
}

void MaterialGeo::Render(Context* pcontext)
{
    pcontext->SetMaterial(GetMaterial(), m_bOverride);
    GetGeo()->Render(pcontext);
}

//////////////////////////////////////////////////////////////////////////////
//
// Texture Geo
//
//////////////////////////////////////////////////////////////////////////////

TextureGeo::TextureGeo(Geo* pgeo, Image* pimageTexture, bool bOverride) :
    WrapGeo(pgeo, pimageTexture),
    m_bOverride(bOverride)
{
}

TRef<WrapGeo> TextureGeo::Duplicate(Geo* pgeo)
{
    return new TextureGeo(pgeo, GetTexture(), m_bOverride);
}

TRef<Geo> TextureGeo::FoldTexture()
{
    bool bAnyFold = false;

    TRef<Geo> pgeo = GetGeo();
    TRef<Geo> pgeoFold = pgeo->FoldTexture();

    if (pgeoFold) {
        bAnyFold = true;
        SetGeo(pgeoFold);
    } else {
        pgeoFold = pgeo;
    }

    TRef<TextureGeo> ptextureGeo = pgeoFold->GetTextureGeo();

    if (ptextureGeo) {
        bAnyFold = true;
        SetGeo(ptextureGeo->GetGeo());
    }

    if (bAnyFold) {
        return this;
    } else {
        return NULL;
    }
}

TextureGeo* TextureGeo::GetTextureGeo()
{
    return this;
}

Image* TextureGeo::FindTexture()
{
    if (GetTexture()) {
        return GetTexture();
    } else {
        return GetGeo()->FindTexture();
    }
}

void TextureGeo::Render(Context* pcontext)
{
    pcontext->SetTexture(GetTexture()->GetSurface(), m_bOverride);
    WrapGeo::Render(pcontext);
}

//////////////////////////////////////////////////////////////////////////////
//
// Timed Geo
//
//////////////////////////////////////////////////////////////////////////////

TimedGeo::TimedGeo(Geo* pgeo, Number* ptime, float lifespan) :
    WrapGeo(pgeo, ptime),
    m_lifespan(lifespan)
{
}

TRef<WrapGeo> TimedGeo::Duplicate(Geo* pgeo)
{
    return new TimedGeo(pgeo, GetTime(), m_lifespan);
}

void TimedGeo::Evaluate()
{
    if (GetTime()->GetValue() > m_lifespan) {
        Trigger();
        SetEmpty();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Group Geo
//
//////////////////////////////////////////////////////////////////////////////

class GroupGeoImpl : public GroupGeo, private ValueList::Site {
private:
    TRef<Geo> m_pgeoHit;
    TRef<Geo> m_pgeoCapture;
    ZString   m_strName;

    ValueList* GetValueList() { return ValueList::Cast(GetChild(0)); }

public:
    GroupGeoImpl() :
        GroupGeo(new ValueList(this))
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // ValueList::Site members
    //
    //////////////////////////////////////////////////////////////////////////////

    bool RemoveValue(Value* pvalue)
    {
        return pvalue == Geo::GetEmpty();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Geo> ApplyTransform(Transform* ptrans)
    {
        ValueList* plist = GetValueList();
        TRef<GroupGeo> pgroupNew = new GroupGeoImpl();

        Geo* pgeo = Geo::Cast(plist->GetFirst());

        while (pgeo) {
            TRef<Geo> pgeoNew = pgeo->ApplyTransform(ptrans);

            if (pgeoNew == NULL) {
                return NULL;
            }

            pgroupNew->AddGeo(pgeoNew);
            pgeo = Geo::Cast(plist->GetNext());
        }

        return pgroupNew;
    }

    bool AnyChildGroups(bool bHead)
    {
        if (bHead) {
            TRef<ValueList> plist = GetValueList();

            Geo* pgeo = Geo::Cast(plist->GetFirst());
            while (pgeo) {
                if (pgeo->AnyChildGroups(false)) {
                    return true;
                }
                pgeo = Geo::Cast(plist->GetNext());
            }

            return false;
        }

        return true;
    }

    TRef<Geo> RemoveMaterial()
    {
        bool bAnyFold = false;
        TRef<ValueList> plist = GetValueList();
        TRef<GroupGeo> pgroupNew = new GroupGeoImpl();

        Geo* pgeo = Geo::Cast(plist->GetFirst());
        while (pgeo) {
            TRef<Geo> pgeoFold = pgeo->RemoveMaterial();
            if (pgeoFold) {
                bAnyFold = true;
                pgroupNew->AddGeo(pgeoFold);
            } else {
                pgroupNew->AddGeo(pgeo);
            }
            pgeo = Geo::Cast(plist->GetNext());
        }

        if (bAnyFold) {
            return pgroupNew;
        }

        return NULL;
    }

    TRef<Geo> RemoveTextures()
    {
        TRef<ValueList> plist = GetValueList();
        TRef<GroupGeo> pgroupNew = new GroupGeoImpl();

        TRef<Geo> pgeo = Geo::Cast(plist->GetFirst());
        while(pgeo) {
            pgroupNew->AddGeo(pgeo->GetTextureGeo()->GetGeo());
            pgeo = Geo::Cast(plist->GetNext());
        }

        return pgroupNew;
    }

    class TextureData : public IObject {
    public:
        TRef<Geo>          m_pgeo;
        TRef<GroupGeoImpl> m_pgroup;
        TRef<TextureGeo>   m_ptextureGeo;
        int                m_count;
    };
    typedef TMap<TRef<Surface>, TRef<TextureData> > MapType;

    TRef<Geo> FoldTexture()
    {
        bool bAnyFold = false;
        TRef<ValueList> plist = GetValueList();

        //
        // Fold the children
        //

        MapType            map;
        TRef<GroupGeoImpl> pgroupNew = new GroupGeoImpl();

        TRef<Geo> pgeo = Geo::Cast(plist->GetFirst());
        while (pgeo) {
            TRef<Geo> pgeoFold = pgeo->FoldTexture();

            if (pgeoFold) {
                bAnyFold = true;
            } else {
                pgeoFold = pgeo;
            }

            TRef<TextureGeo> ptextureGeo = pgeoFold->GetTextureGeo();
            if (ptextureGeo) {
                TRef<Image>       pimage   = ptextureGeo->GetTexture();
                TRef<Surface>     psurface = pimage->GetSurface();
                TRef<TextureData> pdata;

                if (!map.Find(psurface, pdata)) {
                    pdata                = new TextureData();
                    pdata->m_pgeo        = ptextureGeo;
                    pdata->m_pgroup      = new GroupGeoImpl();
                    pdata->m_ptextureGeo = new TextureGeo(pdata->m_pgroup, pimage, false);
                    pdata->m_count       = 0;

                    map.Set(psurface, pdata);
                }

                pdata->m_count++;
                pdata->m_pgroup->AddGeo(ptextureGeo->GetGeo());
            } else {
                pgroupNew->AddGeo(pgeoFold);
            }

            pgeo = Geo::Cast(plist->GetNext());
        }

        //
        // Fix up the group if any folding happened
        //

        if (bAnyFold || map.Count() != 0) {
            MapType::Iterator iter(map);

            while (!iter.End()) {
                TRef<TextureData> pdata = iter.Value();

                if (pdata->m_count == 1) {
                    pgroupNew->AddGeo(pdata->m_pgeo);
                } else {
                    bAnyFold = true;
                    pgroupNew->AddGeo(pdata->m_ptextureGeo);
                }

                iter.Next();
            }

            if (bAnyFold) {
                return pgroupNew;
            }
        }

        return NULL;
    }

    TRef<Value> Fold()
    {
        bool bAnyFold = false;
        TRef<ValueList> plist = GetValueList();

        //
        // Remove EmptyGeos
        //

        TRef<Geo> pgeo = Geo::Cast(plist->GetFirst());
        while (pgeo) {
            if (pgeo == Geo::GetEmpty()) {
                bAnyFold = true;
                pgeo = Geo::Cast(plist->RemoveCurrent());
            } else {
                pgeo = Geo::Cast(plist->GetNext());
            }
        }

        //
        // Fold any of the child groups into the group
        //

        pgeo = Geo::Cast(plist->GetFirst());
        while (pgeo) {
            TRef<GroupGeoImpl> pgroupChild;
            CastTo(pgroupChild, pgeo->GetGroupGeo());

            if (pgroupChild != NULL) {
                bAnyFold = true;
                pgeo = Geo::Cast(plist->RemoveCurrent());

                TRef<ValueList> plistChild = pgroupChild->GetValueList();

                TRef<Geo> pgeoChild = Geo::Cast(plistChild->GetFirst());
                while (pgeoChild) {
                    // !!! should this be push end?
                    plist->PushFront(pgeoChild);
                    pgeoChild = Geo::Cast(plistChild->GetNext());
                }
            } else {
                pgeo = Geo::Cast(plist->GetNext());
            }
        }

        //
        // Combine meshes
        //

        TRef<MeshGeo> pmeshGeo;

        pgeo = Geo::Cast(plist->GetFirst());
        while (pgeo) {
            TRef<MeshGeo> pmeshGeoChild = pgeo->GetMeshGeo();

            if (pmeshGeoChild != NULL) {
                if (pmeshGeo != NULL) {
                    bAnyFold = true;
                    pmeshGeo = pmeshGeo->Combine(pmeshGeoChild);
                } else {
                    pmeshGeo = pmeshGeoChild;
                }
                pgeo = Geo::Cast(plist->RemoveCurrent());
            } else {
                pgeo = Geo::Cast(plist->GetNext());
            }
        }

        if (pmeshGeo) {
            plist->PushFront(pmeshGeo);
        }

        //
        // Fold this node depending on the number of children
        //

        int count = plist->GetCount();

        if (count == 0) {
            return Geo::GetEmpty();
        } else if (count == 1) {
            return Geo::Cast(plist->GetFirst());
        } else {
            if (bAnyFold) {
                return this;
            } else {
                return NULL;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // GroupGeo members
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetName(const ZString& strName)
    {
        m_strName = strName;
    }

    const ZString& GetName()
    {
        return m_strName;
    }

    void AddGeo(Geo* pgeo)
    {
        GetValueList()->PushFront(pgeo);
    }

    void RemoveGeo(Geo* pgeo)
    {
        GetValueList()->Remove(pgeo);
    }

    ValueList* GetList()
    {
        return GetValueList();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Geo members
    //
    //////////////////////////////////////////////////////////////////////////////

    GroupGeo* GetGroupGeo()
    {
        return this;
    }

    void Render(Context* pcontext)
    {
        Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());
        while (pgeo) {
            if (GetValueList()->IsLast()) {
                pgeo->Render(pcontext);
            } else {
                pcontext->PushState();
                pgeo->Render(pcontext);
                pcontext->PopState();
            }

            pgeo = Geo::Cast(GetValueList()->GetNext());
        }
    }

    Image* FindTexture()
    {
        Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());
        while (pgeo) {
            Image* pimage = pgeo->FindTexture();
            if (pimage) {
                return pimage;
            }

            pgeo = Geo::Cast(GetValueList()->GetNext());
        }

        return NULL;
    }

    int GetTriangleCount()
    {
        int tcount = 0;
        Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());
        while (pgeo) {
            tcount += pgeo->GetTriangleCount();
            pgeo = Geo::Cast(GetValueList()->GetNext());
        }

        return tcount;
    }

    float GetRadius(const Matrix& mat)
    {
        float radius = 0;
        Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());
        while (pgeo) {
            radius = std::max(radius, pgeo->GetRadius(mat));
            pgeo = Geo::Cast(GetValueList()->GetNext());
        }

        return radius;
    }

    void CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback)
    {
        TRef<Geo> pgeoReplace = pcallback->Execute(mat, this);

        if (pgeoReplace) {
            ChangeTo(pgeoReplace);
        } else {
            //
            // recurse over the children
            //

            Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());
            while (pgeo) {
                pgeo->CallGroupGeoCallback(mat, pcallback);
                pgeo = Geo::Cast(GetValueList()->GetNext());
            }
        }
    }

    void RemoveCapture()
    {
        if (m_pgeoCapture) {
            m_pgeoCapture->RemoveCapture();
            m_pgeoCapture = NULL;
        }
    }

    bool HitTest(const Vector& vecOrigin, const Vector& vecRay, HitData3D& hitData, bool bCaptured)
    {
        /* !!! this isn't exactly right
        m_pgeoHit = NULL;

        if (m_pgeoCapture) {
            if (m_pgeoCapture->HitTest(vecOrigin, vecRay, hitData, bCaptured)) {
                m_pgeoHit = m_pgeoCapture;
            }
        } else {
            Geo* pgeo = Geo::Cast(GetValueList()->GetFirst());

            while (pgeo) {
                if (pgeo->HitTest(vecOrigin, vecRay, hitData, bCaptured)) {
                    m_pgeoHit = pgeo;
                }

                pgeo = Geo::Cast(GetValueList()->GetNext());
            }
        }

        return m_pgeoHit != NULL;
        */

        return false;
    }

    MouseResult Button(
        const Vector& vec,
        int button,
        bool bCaptured,
        bool bInside,
        bool bDown
    ) {
        MouseResult mouseResult;

        if (m_pgeoCapture) {
            mouseResult =
                m_pgeoCapture->Button(
                    vec,
                    button,
                    true,
                    m_pgeoHit != NULL,
                    bDown
                );

            if (mouseResult.Test(MouseResultRelease())) {
                m_pgeoCapture = NULL;
            }
        } else {
            mouseResult = m_pgeoHit->Button(vec, button, false, true, bDown);

            if (mouseResult.Test(MouseResultCapture())) {
                m_pgeoCapture = m_pgeoHit;
            }
        }

        return mouseResult;
    }

};

TRef<GroupGeo> GroupGeo::Create()
{
    return new GroupGeoImpl();
}

TRef<GroupGeo> GroupGeo::Create(Geo* pgeo)
{
    TRef<GroupGeo> pgroup = new GroupGeoImpl();
    pgroup->AddGeo(pgeo);
    return pgroup;
}

TRef<GroupGeo> GroupGeo::Create(Geo* pgeo, Geo* pgeo2)
{
    TRef<GroupGeo> pgroup = new GroupGeoImpl();
    pgroup->AddGeo(pgeo);
    pgroup->AddGeo(pgeo2);
    return pgroup;
}

TRef<GroupGeo> GroupGeo::Create(Geo* pgeo, Geo* pgeo2, Geo* pgeo3)
{
    TRef<GroupGeo> pgroup = new GroupGeoImpl();
    pgroup->AddGeo(pgeo);
    pgroup->AddGeo(pgeo2);
    pgroup->AddGeo(pgeo3);
    return pgroup;
}

//////////////////////////////////////////////////////////////////////////////
//
// LODGeo
//
//////////////////////////////////////////////////////////////////////////////

class LODGeoImpl : public LODGeo {
private:
    //
    // types
    //

    friend class LODGeo;

    //
    // members
    //

    TVector<float> m_radii;

public:
    LODGeoImpl(Geo* pgeo) :
        LODGeo(pgeo)
    {
        pgeo->Update();
    }

    TRef<Geo> RemoveTextures()
    {
        TRef<LODGeoImpl> plodGeoNew = new LODGeoImpl(Geo::GetEmpty());

        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            TRef<Geo> pgeo = Geo::Cast(GetChild(index))->GetTextureGeo()->GetGeo();

            if (index == 0) {
                plodGeoNew->SetGeo(pgeo);
            } else {
                plodGeoNew->AddGeo(pgeo, m_radii[index - 1]);
            }
        }

        return plodGeoNew;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // LODGeo members
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddGeo(Geo* pgeo, float radius)
    {
        //
        // Insert in sorted order
        //

        int count = GetChildCount();
		// mdvalley: define it out here
		int index;
        for (index = 0; index < count - 1; index++) {
            if (radius > m_radii[index]) {
                break;
            }
        }

        if (index == count - 1) {
            //
            // Insert in the last slot
            //

            AddChild(pgeo);
            m_radii.PushEnd(radius);
        } else {
            //
            // move the last geo
            //

            AddChild(GetChild(count - 1));
            m_radii.PushEnd();

            //
            // Insert before index
            //

            for (int inew = count - 1; inew > index; inew--) {
                m_radii.Set(inew, m_radii[inew - 1]);
            }

            m_radii.Set(index, radius);

            for (int inew = count - 1; inew > index + 1; inew--) {
                SetChild(inew, GetChild(inew - 1));
            }

            SetChild(index + 1, pgeo);
        }
    }

    ZString GetInfo()
    {
        int count = GetChildCount();
        ZString str = "LODGeo with " + ZString(count) + " levels\n";
        for (int index = 0; index < count; index++) {
            TRef<Geo> pgeo = Geo::Cast(GetChild(index));

            int ctriangle = pgeo->GetTriangleCount();

            str += "level " + ZString(index) + " has " + ZString(ctriangle) + " triangles.\n";
        }

        return str;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Geo members
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetTriangleCount()
    {
        int tcount = 0;

        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            tcount += Geo::Cast(GetChild(index))->GetTriangleCount();
        }

        return tcount;
    }

    float GetRadius(const Matrix& mat)
    {
        float radius = 0;

        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            radius = std::max(radius, Geo::Cast(GetChild(index))->GetRadius(mat));
        }

        return radius;
    }

    void Render(Context* pcontext)
    {
        float radius = pcontext->GetLOD();

        int count = GetChildCount();
		int index;
        for (index = 0; index < count - 1; index++) {
            if (radius > m_radii[index]) {
                break;
            }
        }

        Geo::Cast(GetChild(index))->Render(pcontext);
    }

    bool AnyChildGroups(bool bHead)
    {
        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            TRef<Geo> pgeo = Geo::Cast(GetChild(index));

            if (pgeo->AnyChildGroups(false)) {
                return true;
            }
        }

        return false;
    }

    TRef<Geo> RemoveMaterial()
    {
        bool bAnyFold = false;
        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            TRef<Geo> pgeo = Geo::Cast(GetChild(index));
            TRef<Geo> pgeoFold = pgeo->RemoveMaterial();

            if (pgeoFold) {
                bAnyFold = true;
                SetChild(index, pgeoFold);
            } else {
                SetChild(index, pgeo);
            }
        }

        if (bAnyFold) {
            return this;
        }

        return NULL;
    }

    TRef<Geo> FoldTexture()
    {
        bool bAnyFold = false;
        TRef<LODGeoImpl>   plodGeoNew = new LODGeoImpl(Geo::GetEmpty());

        bool               bTexturesMatch = true;
        TRef<Image>        pimage;
        TRef<Surface>      psurface;

        int count = GetChildCount();
        for (int index = 0; index < count; index++) {
            TRef<Geo> pgeo     = Geo::Cast(GetChild(index));
            TRef<Geo> pgeoFold = pgeo->FoldTexture();

            if (pgeoFold) {
                bAnyFold = true;
            } else {
                pgeoFold = pgeo;
            }

            if (index == 0) {
                plodGeoNew->SetGeo(pgeoFold);
            } else {
                plodGeoNew->AddGeo(pgeoFold, m_radii[index - 1]);
            }

            TRef<TextureGeo> ptextureGeo = pgeoFold->GetTextureGeo();
            if (ptextureGeo) {
                if (pimage == NULL) {
                    pimage   = ptextureGeo->GetTexture();
                    psurface = pimage->GetSurface();
                } else {
                    if (ptextureGeo->GetTexture()->GetSurface() != psurface) {
                        bTexturesMatch = false;
                    }
                }
            } else {
                bTexturesMatch = false;
            }
        }

        //
        // If the textures match colapse the TextureGeos
        //

        if (bTexturesMatch) {
            return
                new TextureGeo(
                    plodGeoNew->RemoveTextures(),
                    pimage,
                    false
                );
        }

        if (bAnyFold) {
            return plodGeoNew;
        }

        return NULL;
    }

    TRef<WrapGeo> Duplicate(Geo* pgeo)
    {
        TRef<LODGeoImpl> pgeoNew = new LODGeoImpl(pgeo);

        int count = GetChildCount();
        for (int index = 1; index < count; index++) {
            pgeoNew->AddGeo(
                Geo::Cast(GetChild(index)), 
                m_radii[index - 1]
            );
        }

        return pgeoNew;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Value> Fold()
    {
        if (GetChildCount() == 1) {
            return GetGeo();
        }

        return NULL;
    }

    ZString GetFunctionName()
    {
        return "LODGeo";
    }

    ZString GetString(int indent)
    {
        // LODGeo (baseGeo, [(pixels, geo)])

        ZString str = "LODGeo(\n";
        str += Indent(indent + 1) + GetGeo()->GetString(indent + 1) + ",\n";
        str += Indent(indent + 1) + "[\n";

        int count = GetChildCount();
        for (int index = 1; index < count; index++) {
			Geo* pgeo = Geo::Cast(GetChild(index));

            str += Indent(indent + 2) + "(\n";
            str += Indent(indent + 3) + ZString(m_radii[index - 1]) + ",\n";
            str += Indent(indent + 3) + pgeo->GetString(indent + 3) + "\n";

            if (index == count - 1) {
                str += Indent(indent + 2) + ")\n";
            } else {
                str += Indent(indent + 2) + "),\n";
            }
        }

        str += Indent(indent + 1) + "]\n";
        str += Indent(indent) + ")";

        return str;
    }

    void Write(IMDLBinaryFile* pmdlFile)
    {
        int count = GetChildCount();

        pmdlFile->WriteList(count - 1);

        for (int index = count - 1; index > 0; index--) {
            Geo* pgeo = Geo::Cast(GetChild(index));
            pgeo->Write(pmdlFile);
            pmdlFile->WriteNumber(m_radii[index - 1]);
            pmdlFile->WritePair();
            pmdlFile->WriteEnd();
        }

        GetGeo()->Write(pmdlFile);
        pmdlFile->WriteReference("LODGeo");
        pmdlFile->WriteApply();
    }
};

TRef<LODGeo> LODGeo::Create(Geo* pgeo)
{
    return new LODGeoImpl(pgeo);
}

//////////////////////////////////////////////////////////////////////////////
//
// Callback Geo
//
//////////////////////////////////////////////////////////////////////////////

CallbackGeo::CallbackGeo(Geo* pgeo) :
    WrapGeo(pgeo)
{
}

TRef<WrapGeo> CallbackGeo::Duplicate(Geo* pgeo)
{
    return new CallbackGeo(pgeo);
}

void CallbackGeo::Render(Context* pcontext)
{
    pcontext->DrawCallbackGeo(this, false);
}

void CallbackGeo::RenderCallback(Context* pcontext)
{
    GetGeo()->Render(pcontext);
}

//////////////////////////////////////////////////////////////////////////////
//
// Blend Add Geo
//
//////////////////////////////////////////////////////////////////////////////

class BlendGeo : public CallbackGeo {
private:
    BlendMode m_blendMode;

public:
    BlendGeo(Geo* pgeo, BlendMode blendMode) :
        CallbackGeo(pgeo),
        m_blendMode(blendMode)
    {
    }

    //
    // WrapGeo members
    //

    TRef<WrapGeo> Duplicate(Geo* pgeo)
    {
        return new BlendGeo(pgeo, m_blendMode);
    }

    //
    // Geo members
    //

    void RenderCallback(Context* pcontext)
    {
        pcontext->SetBlendMode(m_blendMode);
        pcontext->SetZWrite(false);
        GetGeo()->Render(pcontext);
    }
};

TRef<Geo> CreateBlendGeo(Geo* pgeo, BlendMode blendMode)
{
    return new BlendGeo(pgeo, blendMode);
}
