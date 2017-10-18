#include "pch.h"

// compile this file for speed

#pragma optimize("t", on)
// #pragma optimize("s", on)

//////////////////////////////////////////////////////////////////////////////
//
// Clipping Planes
//
//////////////////////////////////////////////////////////////////////////////

const int g_maxCountPlanes = 8;

typedef BYTE Code;
const Code ClipFront  = 0x01;
const Code ClipBack   = 0x02;
const Code ClipLeft   = 0x04;
const Code ClipRight  = 0x08;
const Code ClipBottom = 0x10;
const Code ClipTop    = 0x20;
const Code ClipPlane  = 0x40;

//////////////////////////////////////////////////////////////////////////////
//
// Transformed and Lit Vertex
//
//////////////////////////////////////////////////////////////////////////////

class VertexTL {
public:
    float x, y, z, w;
    float r, g, b, a;
    float u, v;
    WORD  m_index;
    Code  m_code;
    
    VertexTL()
    {
    }

    VertexTL(const VertexTL& v) :
        x(v.x),
        y(v.y),
        z(v.z),
        w(v.w),
        r(v.r),
        g(v.g),
        b(v.b),
        a(v.a),
        u(v.u),
        v(v.v),
        m_index(v.m_index),
        m_code(v.m_code)
    {
    }

    void Interpolate(const VertexTL& vertex0, const VertexTL& vertex1, float alpha)
    {
        float beta = 1 - alpha;

        x = alpha * vertex1.x + beta * vertex0.x;
        y = alpha * vertex1.y + beta * vertex0.y;
        z = alpha * vertex1.z + beta * vertex0.z;
        w = alpha * vertex1.w + beta * vertex0.w;
        u = alpha * vertex1.u + beta * vertex0.u;
        v = alpha * vertex1.v + beta * vertex0.v;
        r = alpha * vertex1.r + beta * vertex0.r;
        g = alpha * vertex1.g + beta * vertex0.g;
        b = alpha * vertex1.b + beta * vertex0.b;
        a = alpha * vertex1.a + beta * vertex0.a;
    }

    void UpdateClipCode()
    {
        m_code = 0;

        if (x < -w) m_code |= ClipLeft;
        if (x >  w) m_code |= ClipRight;

        if (y < -w) m_code |= ClipBottom;
        if (y >  w) m_code |= ClipTop;

        if (z <  0) m_code |= ClipFront;
        if (z >  w) m_code |= ClipBack;
    }

    void UpdateClipCode(const Plane& plane, Code code)
    {
        if (plane.Distance(HVector(x, y, z, w)) <= 0) {
            m_code |= code;
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Device3D
//
//////////////////////////////////////////////////////////////////////////////

class Device3D : public IDevice3D {
    //
    // Types
    //

    typedef void (Device3D::*PFNClip)(VertexTL* pvertex, int vcount, const WORD* pindex, int icount);
    typedef void (Device3D::*PFNDraw)(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount);
    typedef void (Device3D::*PFNVertex)(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw);
    typedef void (Device3D::*PFNVertexL)(const VertexL* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw);
    typedef void (Device3D::*PFNDrawD3D)(const D3DLVertex* pvertex, int vcount, const WORD* pindex, int icount);
    typedef void (Device3D::*PFNVertexDrawD3D)(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNDrawD3D pfnDrawD3D);

    //
    // Interfaces
    //

    TRef<ID3DRasterizer>    m_prasterizer;

    //
    // Current state
    //

    bool                 m_bDrawD3D;

    Rect                 m_rectView;
    bool                 m_bClip;
    ShadeMode            m_shadeMode;
    PFNVertexL           m_pfnTransform;
    PFNVertex            m_pfnLightVertex;
    PFNVertexL           m_pfnLightVertexL;
    PFNVertexDrawD3D     m_pfnLightD3D;
    

    Matrix               m_mat;
    Matrix               m_matPerspective;
    Matrix               m_matFull;
    bool                 m_bUpdateMatFull;
    bool                 m_bUpdateLighting;
    bool                 m_bUpdatePointers;
    float                m_brightAmbient;
    Vector               m_vecLight;
    Vector               m_vecLightWorld;

    TRef<IDirect3DLight> m_plight;
    TRef<IDirect3DMaterial2> m_pmat;

    TRef<Material>       m_pmaterial;

    //
    // Clipping
    //

    float m_scalex;
    float m_scaley;
    float m_xoffset;
    float m_yoffset;

    int   m_countPlanes;
    Plane m_pplane[g_maxCountPlanes];

    //
    // Temporary vertex and index buffers
    //

    D3DLVertex*   m_pvertexLBufferD3D;
    int           m_countVertexLBufferD3D;

    VertexL*      m_pvertexLBuffer;
    int           m_countVertexLBuffer;

    VertexTL*     m_pvertexTLBuffer;
    int           m_countVertexTLBuffer;

    VertexScreen* m_pvertexScreenBuffer;
    int           m_countVertexScreenBuffer;
    int           m_indexVertexScreenBuffer;

    WORD*         m_pindexBuffer;
    int           m_countIndexBuffer;
    int           m_indexIndexBuffer;

    //
    // Performance counters
    //

    #ifdef EnablePerformanceCounters
        int m_countPoints;
        int m_countLines;
        int m_countTriangles;
        int m_countMeshes;
        int m_countObjects;
        int m_countObjectsRendered;
        int m_countMatrixLoads;
    #endif

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    Device3D(ID3DRasterizer* prasterizer) :
        m_prasterizer(prasterizer),
        
        m_pvertexLBufferD3D(NULL),
        m_countVertexLBufferD3D(0),

        m_pvertexLBuffer(NULL),
        m_countVertexLBuffer(0),

        m_pvertexTLBuffer(NULL),
        m_countVertexTLBuffer(0),

        m_pvertexScreenBuffer(NULL),
        m_countVertexScreenBuffer(0),
        m_indexVertexScreenBuffer(0),

        m_pindexBuffer(NULL),
        m_countIndexBuffer(0),
        m_indexIndexBuffer(0),
        
        m_bDrawD3D(true),

        m_bClip(true),
        m_shadeMode(ShadeModeGouraud),
        m_bUpdateMatFull(true),
        m_bUpdateLighting(true),
        m_bUpdatePointers(true),
        m_brightAmbient(0.5f),
        m_vecLightWorld(1, 0, 0)
    {
        m_mat.SetIdentity();
        m_matPerspective.SetIdentity();

        //
        // Setup the clipping planes
        //

        m_countPlanes = 6;
        m_pplane[0] = Plane(HVector( 0, 0, 1,  0));
        m_pplane[1] = Plane(HVector( 0, 0, 1, -1));
        m_pplane[2] = Plane(HVector( 1, 0, 0,  1));
        m_pplane[3] = Plane(HVector(-1, 0, 0,  1));
        m_pplane[4] = Plane(HVector( 0, 1, 0,  1));
        m_pplane[5] = Plane(HVector( 0,-1, 0,  1));

        //
        // Initialize Performance counters
        //

        #ifdef EnablePerformanceCounters
            ResetPerformanceCounters();
        #endif
    }

    bool IsValid()
    {
        return m_prasterizer->IsValid();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Performance counters
    //
    //////////////////////////////////////////////////////////////////////////////

    #ifdef EnablePerformanceCounters
        void ResetPerformanceCounters()
        {
            m_countPoints          = 0;
            m_countLines           = 0;
            m_countTriangles       = 0;
            m_countMeshes          = 0;
            m_countObjects         = 0;
            m_countObjectsRendered = 0;
            m_countMatrixLoads     = 0;
        }

        int GetPerformanceCounter(Counter counter)
        {
            switch (counter) {
                case CounterPoints          : return m_countPoints         ;
                case CounterLines           : return m_countLines          ;
                case CounterTriangles       : return m_countTriangles      ;
                case CounterMeshes          : return m_countMeshes         ;
                case CounterObjects         : return m_countObjects        ;
                case CounterObjectsRendered : return m_countObjectsRendered;
                case CounterMatrixLoads     : return m_countMatrixLoads    ;
            }

            ZError("Unknown counter");
            return 0;
        }
    #endif

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~Device3D()
    {
        if (m_pvertexLBufferD3D) {
            free(m_pvertexLBufferD3D);
        }

        if (m_pvertexLBuffer) {
            free(m_pvertexLBuffer);
        }

        if (m_pvertexTLBuffer) {
            free(m_pvertexTLBuffer);
        }

        if (m_pvertexScreenBuffer) {
            free(m_pvertexScreenBuffer);
        }

        if (m_pindexBuffer) {
            free(m_pindexBuffer);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Device
    //
    //////////////////////////////////////////////////////////////////////////////

    void BeginScene()
    {
        m_prasterizer->BeginScene();
    }

    void EndScene()
    {
        m_prasterizer->EndScene();
    }

    void ClearZBuffer()
    {
        m_prasterizer->ClearZBuffer();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // View Rect
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetViewRect(const Rect& rectView)
    {
        m_rectView       = rectView;
        m_bUpdateMatFull = true;
        m_prasterizer->SetViewRect(rectView);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Lighting
    //
    //////////////////////////////////////////////////////////////////////////////

    void DirectionalLight(const Color& color, const Vector& vec)
    {
        m_vecLightWorld = m_mat.TransformDirection(vec);

        if (!m_plight)
            {
            DDCall(m_prasterizer->GetD3D()->CreateLight(&m_plight, NULL));
               
            D3DLIGHT light;
            ZeroMemory(&light, sizeof(D3DLIGHT));
            light.dwSize = sizeof(D3DLIGHT);
            light.dltType = D3DLIGHT_DIRECTIONAL; 
            light.dcvColor = D3DColorValue(color); 
    
            light.dvDirection = D3DVector(vec); 
            light.dvFalloff = 1.0f; 
            light.dvAttenuation0 = 1.0f;
            light.dvAttenuation1 = 0.0f;
            light.dvAttenuation2 = 0.0f;
        	light.dvRange = D3DLIGHT_RANGE_MAX;
	        //light.dwFlags = D3DLIGHT_ACTIVE;
    
    
            DDCall(m_plight->SetLight(&light));
            DDCall(m_prasterizer->GetViewport()->AddLight(m_plight));

            D3DMATERIAL material;
            D3DMATERIALHANDLE hmat;

            material.dwSize     = sizeof(material);
            material.diffuse.r = material.diffuse.a = 1.0;
            material.diffuse.g = material.diffuse.b = 1.0;
            material.ambient.a = 1.0;
            material.ambient.r = material.ambient.g = material.ambient.b = 1.0;
            material.emissive.a = 1.0;
            material.emissive.r = material.emissive.g = material.emissive.b = 1.0;
            material.specular.a = 1.0;
            material.specular.r = material.specular.g = material.specular.b = 1.0;
            material.power      = 1;
            material.hTexture   = NULL;
            material.dwRampSize = 0;

            DDCall(m_prasterizer->GetD3D()->CreateMaterial(&m_pmat, NULL));
            DDCall(m_pmat->SetMaterial(&material));
            DDCall(m_pmat->GetHandle(m_prasterizer->GetD3DDeviceX(), &hmat));

            DDCall(m_prasterizer->GetD3DDeviceX()->SetLightState(D3DLIGHTSTATE_MATERIAL, hmat));

            }
        else
            {
            D3DLIGHT light;
            ZeroMemory(&light, sizeof(D3DLIGHT));
            light.dwSize = sizeof(D3DLIGHT);
            DDCall(m_plight->GetLight(&light));
            light.dcvColor = D3DColorValue(color); 
            light.dvDirection = D3DVector(vec); 
            DDCall(m_plight->SetLight(&light));
            }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetMatrix(D3DMATRIX* pdmat, const Matrix& mat)
    {
        pdmat->_11 = mat[0][0];
        pdmat->_12 = mat[1][0];
        pdmat->_13 = mat[2][0];
        pdmat->_14 = mat[3][0];
                            
        pdmat->_21 = mat[0][1];
        pdmat->_22 = mat[1][1];
        pdmat->_23 = mat[2][1];
        pdmat->_24 = mat[3][1];
                            
        pdmat->_31 = mat[0][2];
        pdmat->_32 = mat[1][2];
        pdmat->_33 = mat[2][2];
        pdmat->_34 = mat[3][2];
                            
        pdmat->_41 = mat[0][3];
        pdmat->_42 = mat[1][3];
        pdmat->_43 = mat[2][3];
        pdmat->_44 = mat[3][3];
    }

    const Matrix& GetFullMatrix()
    {
        if (m_bUpdateMatFull) {
            m_bUpdateMatFull = false;

            m_matFull = m_mat;
            m_matFull.Multiply(m_matPerspective);

            Point pointCenter = m_rectView.Center();
            m_scalex = 0.5f * m_rectView.XSize();

            // !!! In D3D pixels centers are on integer boundaries
            //     The device clips to integer boundaries
            //     subtract 0.25 to move the first row of pixels centers into the clip rect

            m_xoffset = pointCenter.X() - 0.25f;
            m_yoffset = pointCenter.Y() - 0.25f;

            if (m_bClip) {
                m_scaley = 0.5f * m_rectView.YSize();

                m_matFull.Scale(
                    Vector(
                        1,
                        m_scalex / m_scaley,
                        1.0f
                    )
                );
            } else {
                m_matFull.Scale(
                    Vector(
                         m_scalex,
                        -m_scalex,
                        1.0f
                    )
                );
                m_matFull.Translate(
                    Vector(
                        m_xoffset,
                        m_yoffset,
                        0
                    )
                );
            }

        }

        return m_matFull;
    }

    void UpdateLighting()
    {
        if (m_bUpdateLighting) {
            m_bUpdateLighting = false;

            Matrix matInverse;

            matInverse.SetInverse(m_mat);

            m_vecLight = matInverse.TransformDirection(m_vecLightWorld);
            m_vecLight.SetNormalize();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Update Pointers
    //
    //////////////////////////////////////////////////////////////////////////////
    void UpdatePointers()
    {
        if (m_bUpdatePointers) {
            m_bUpdatePointers = false;

            if (!m_bDrawD3D) {

                bool bClip = m_bClip || m_countPlanes > 6;

                if (bClip) {
                    m_pfnTransform = TransformClip;
                } else {
                    m_pfnTransform = TransformNoClip;
                }

                m_pfnLightVertexL = m_pfnTransform;

                switch (m_shadeMode) {
                    case ShadeModeCopy:
                        m_pfnLightVertex = LightVertexCopy;
                        break;

                    case ShadeModeFlat:
                    case ShadeModeGouraud:
                        if (m_pmaterial) {
                            m_pfnLightVertex = MaterialLightVertex;
                        } else {
                            if (bClip) {
                                m_pfnLightVertex = LightVertex;
                            } else {
                                m_pfnLightVertex = TransformAndLightNoClip;
                            }
                        }
                        break;

                    default:
                        ZError("Invalid ShadeMode");
                }
            } else {
                
                switch (m_shadeMode) {
                    case ShadeModeCopy:
                        m_pfnLightD3D = CopyLightVertexD3D;
                        break;

                    case ShadeModeFlat:
                    case ShadeModeGouraud:
                        if (m_pmaterial) {
                            m_pfnLightD3D = MaterialLightVertexD3D;
                        } else {
                            m_pfnLightD3D = LightVertexD3D;
                        }
                        break;

                    default:
                        ZError("Invalid ShadeMode");
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Get State
    //
    //////////////////////////////////////////////////////////////////////////////

    bool GetClipping()
    {
        return m_bClip;
    }

    const Matrix& GetMatrix()
    {
        return m_mat;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Set State
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetMatrix(const Matrix& mat)
    {
        m_mat             = mat;
        m_bUpdateMatFull  = true;
        m_bUpdateLighting = true;
        
        if (m_bDrawD3D)
            {
            D3DMATRIX dmat;
            SetMatrix(&dmat, m_mat);
            m_prasterizer->GetD3DDeviceX()->SetTransform(
                    D3DTRANSFORMSTATE_WORLD,
                    &dmat);
            }
    }

    void SetPerspectiveMatrix(const Matrix& mat)
    {
        m_matPerspective = mat;
        m_bUpdateMatFull = true;


        if (m_bDrawD3D)
            {
            D3DMATRIX dmat;
            SetMatrix(&dmat, m_matPerspective);
            m_prasterizer->GetD3DDeviceX()->SetTransform(
                    D3DTRANSFORMSTATE_PROJECTION,
                    &dmat);
            }

    }

    void SetClipping(bool bClip)
    {
        m_bClip = bClip;
        m_bUpdateMatFull  = true;
        m_bUpdatePointers = true;
    }

    void RemoveClipPlane(int indexRemove)
    {
        ZAssert(indexRemove >= 0 && indexRemove + 6 < m_countPlanes);

        for(int index = m_countPlanes - indexRemove - 1; index < m_countPlanes - 1; index++) {
            m_pplane[index] = m_pplane[index + 1];
        }

        m_countPlanes--;
    }

    void AddClipPlane(const Plane& plane)
    {
        ZAssert(m_countPlanes < g_maxCountPlanes);

        //
        // transform the plane into eye coordinates
        //

        const Matrix& mat = GetFullMatrix();
        Matrix mati;

        mati.SetInverse(mat);
        mati.Transpose();

        HVector p  = plane.GetHVector();
        HVector pp = mati.Transform(p);

        m_pplane[m_countPlanes] = Plane(pp);

        m_countPlanes++;
    }

    void SetShadeMode(ShadeMode shadeMode)
    {
        m_shadeMode = shadeMode;
        m_bUpdatePointers = true;
        m_prasterizer->SetShadeMode(shadeMode);
    }

    void SetMaterial(Material* pmaterial)
    {
        m_pmaterial = pmaterial;
        m_bUpdatePointers = true;
    }

    void SetBlendMode(BlendMode blendMode)
    {
        m_prasterizer->SetBlendMode(blendMode);
    }

    void SetTexture(Surface* psurfaceTexture)
    {
        m_prasterizer->SetTexture(psurfaceTexture);
    }

    void SetWrapMode(WrapMode wrapMode)
    {
        m_prasterizer->SetWrapMode(wrapMode);
    }

    void SetCullMode(CullMode cullMode)
    {
        m_prasterizer->SetCullMode(cullMode);
    }

    void SetZTest(bool bZTest)
    {
        m_prasterizer->SetZTest(bZTest);
    }

    void SetZWrite(bool bZWrite)
    {
        m_prasterizer->SetZWrite(bZWrite);
    }

    void SetLinearFilter(bool bLinearFilter)
    {
        m_prasterizer->SetLinearFilter(bLinearFilter);
    }

    void SetPerspectiveCorrection(bool bPerspectiveCorrection)
    {
        m_prasterizer->SetPerspectiveCorrection(bPerspectiveCorrection);
    }

    void SetDither(bool bDither)
    {
        m_prasterizer->SetDither(bDither);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Buffers
    //
    //////////////////////////////////////////////////////////////////////////////

    //
    //  , may want to grow to more than count
    //

    void SizeVertexLBufferD3D(int count)
    {
        if (m_countVertexLBufferD3D < count) {
            m_countVertexLBufferD3D = count;
            m_pvertexLBufferD3D = (D3DLVertex*)realloc(m_pvertexLBufferD3D, sizeof(D3DLVertex) * count);
        }
    }

    void SizeVertexLBuffer(int count)
    {
        if (m_countVertexLBuffer < count) {
            m_countVertexLBuffer = count;
            m_pvertexLBuffer = (VertexL*)realloc(m_pvertexLBuffer, sizeof(VertexL) * count);
        }
    }

    void SizeVertexTLBuffer(int count)
    {
        if (m_countVertexTLBuffer < count) {
            m_countVertexTLBuffer = count;
            m_pvertexTLBuffer = (VertexTL*)realloc(m_pvertexTLBuffer, sizeof(VertexTL) * count);
        }
    }

    void SizeVertexScreenBuffer(int count)
    {
        if (m_countVertexScreenBuffer < count) {
            m_pvertexScreenBuffer = (VertexScreen*)realloc(m_pvertexScreenBuffer, sizeof(VertexScreen) * count);
            for (int index = m_countVertexScreenBuffer; index < count; index++) {
                m_pvertexScreenBuffer[index].specular = 0;
            }
            m_countVertexScreenBuffer = count;
        }
    }

    void SizeIndexBuffer(int count)
    {
        if (m_countIndexBuffer < count) {
            m_countIndexBuffer = count;
            m_pindexBuffer = (WORD*)realloc(m_pindexBuffer, sizeof(WORD) * count);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Vertex Buffers
    //
    //////////////////////////////////////////////////////////////////////////////

    D3DLVertex* GetVertexLBufferD3D(int count)
    {
        SizeVertexLBufferD3D(count);
        return m_pvertexLBufferD3D;
    }

    VertexL* GetVertexLBuffer(int count)
    {
        SizeVertexLBuffer(count);
        return m_pvertexLBuffer;
    }

    VertexScreen* GetVertexScreenBuffer(int count)
    {
        SizeVertexScreenBuffer(count);
        return m_pvertexScreenBuffer;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void StoreTriangle(WORD index1, WORD index2, WORD index3)
    {
        SizeIndexBuffer(m_indexIndexBuffer + 3);

        m_pindexBuffer[m_indexIndexBuffer    ] = index1;
        m_pindexBuffer[m_indexIndexBuffer + 1] = index2;
        m_pindexBuffer[m_indexIndexBuffer + 2] = index3;

        m_indexIndexBuffer += 3;
    }

    void StoreLine(WORD index1, WORD index2)
    {
        SizeIndexBuffer(m_indexIndexBuffer + 2);

        m_pindexBuffer[m_indexIndexBuffer    ] = index1;
        m_pindexBuffer[m_indexIndexBuffer + 1] = index2;

        m_indexIndexBuffer += 2;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClampVertex(VertexScreen& vertex)
    {
        if (vertex.x < m_rectView.XMin()) {
            vertex.x = m_rectView.XMin();
        } else if (vertex.x > m_rectView.XMax()) {
            vertex.x = m_rectView.XMax();
        }

        if (vertex.y < m_rectView.YMin()) {
            vertex.y = m_rectView.YMin();
        } else if (vertex.y > m_rectView.YMax()) {
            vertex.y = m_rectView.YMax();
        }

        if (vertex.z < 0) {
            vertex.z = 0;
        } else if (vertex.z > 1) {
            vertex.z = 1;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    WORD ConvertVertex(VertexTL& vertex)
    {
        SizeVertexScreenBuffer(m_indexVertexScreenBuffer + 1);

        vertex.m_index = m_indexVertexScreenBuffer;
        VertexScreen* pvertex = m_pvertexScreenBuffer + m_indexVertexScreenBuffer;

        float rhw  = 1.0f / vertex.w;

        pvertex->x       = m_scalex * rhw * vertex.x + m_xoffset;
        pvertex->y       = m_yoffset - m_scaley * rhw * vertex.y;
        pvertex->z       = rhw * vertex.z;
        pvertex->qw      = rhw;
        pvertex->u       = vertex.u;
        pvertex->v       = vertex.v;
        pvertex->color    = MakeD3DCOLOR(Color(vertex.r, vertex.g, vertex.b, vertex.a));
        pvertex->specular = 0;

        ClampVertex(*pvertex);

        m_indexVertexScreenBuffer++;

        return vertex.m_index;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    WORD TranslateIndex(VertexTL& vertex)
    {
        if (vertex.m_index == 0xffff) {
            return ConvertVertex(vertex);
        }

        return vertex.m_index;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    int TranslateIntersection(
        const Plane&    plane,
        const VertexTL& vertex0, 
        const VertexTL& vertex1 
    ) {
        VertexTL vertex;

        CalculateIntersection(plane, vertex0, vertex1, vertex);
        return ConvertVertex(vertex);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Plane
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalculateIntersection(
        const Plane&     plane,
        const VertexTL&  vertex0,
        const VertexTL&  vertex1,
              VertexTL&  vertexOut
    ) {
        HVector v0(vertex0.x, vertex0.y, vertex0.z, vertex0.w);
        HVector v1(vertex1.x, vertex1.y, vertex1.z, vertex1.w);

        float alpha = plane.Intersect(v0, v1);

        vertexOut.Interpolate(vertex0, vertex1, alpha);
        vertexOut.m_index = 0xffff;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip a polygon to a plane
    //
    //////////////////////////////////////////////////////////////////////////////

    int ClipPolygonToPlane(
        const Plane&     plane,
              Code       bit,
              VertexTL*  pvertexIn,
              int        vcountIn,
              VertexTL*  pvertexOut
    ) {
        int  vcount         = 0;
        int  vindexPrevious = vcountIn - 1;
        Code codePrevious   = pvertexIn[vindexPrevious].m_code & bit;

        for (int vindex = 0; vindex < vcountIn; vindex++) {
            Code code = pvertexIn[vindex].m_code & bit;

            if ((code ^ codePrevious) != 0) {
                CalculateIntersection(
                    plane,
                    pvertexIn[vindexPrevious],
                    pvertexIn[vindex],
                    pvertexOut[vcount]
                );

                pvertexOut[vcount].UpdateClipCode();
                if (m_countPlanes > 6) {
                    Code code = ClipPlane;

                    for (int index = 6; index < m_countPlanes; index++, code <<= 1) {
                        pvertexOut[vcount].UpdateClipCode(m_pplane[index], code);
                    }
                }
                vcount++;
            }

            if (code == 0) {
                pvertexOut[vcount] = pvertexIn[vindex];
                vcount++;
            }

            ZAssert(vcount <= 8);

            codePrevious = code;
            vindexPrevious = vindex;
        }

        return vcount;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip a polygon to a set of planess
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipPolygonToPlanes(
        const Plane* pplane,
              int        pcount,
              Code       codeOr,
        const VertexTL&  vertex0,
        const VertexTL&  vertex1,
        const VertexTL&  vertex2
    ) {
        VertexTL pverticesIn[3] = 
            { 
                vertex0, 
                vertex1, 
                vertex2 
            };
        VertexTL pverticesOut0[8];
        VertexTL pverticesOut1[8];

        VertexTL* pvertexIn    = pverticesIn;
        VertexTL* pvertexOut   = pverticesOut0;
        VertexTL* pvertexOther = pverticesOut1;

        int  vcount = 3;
        Code bit    = 1;
        int  pindex = 0;

        while (pindex < pcount) {
            if ((codeOr & bit) != 0) {
                vcount = ClipPolygonToPlane(pplane[pindex], bit, pvertexIn, vcount, pvertexOut);

                pvertexIn    = pvertexOut;
                pvertexOut   = pvertexOther;
                pvertexOther = pvertexIn;
            }

            bit *= 2;
            pindex++;
        }

        //
        // Translate the vertices to screen coordinates
        //

        WORD aindex[7];
        int index;

        for(index = 0; index < vcount; index++) {
            aindex[index] = TranslateIndex(pvertexIn[index]);
        }

        //
        // Form all of the triangle fans
        //

        for(index = 1; index < vcount - 1; index++) {
            StoreTriangle(
                aindex[0],
                aindex[index],
                aindex[index + 1]
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipPolygonToPlane(
        const Plane&     plane,
              Code       code0,
              Code       code1,
              Code       code2,
              VertexTL&  vertex0,
              VertexTL&  vertex1,
              VertexTL&  vertex2
    ) {
        if (code0 != 0) {
            if (code1 != 0) {
                // p0 and p1 outside

                StoreTriangle(
                    TranslateIntersection(plane, vertex2, vertex0),
                    TranslateIntersection(plane, vertex1, vertex2),
                    TranslateIndex(vertex2)
                );
            } else if (code2 != 0) {
                // p0 and p2 outside

                StoreTriangle(
                    TranslateIntersection(plane, vertex0, vertex1),
                    TranslateIndex(vertex1),
                    TranslateIntersection(plane, vertex2, vertex1)
                );
            } else {
                // p0 outside

                int index0 = TranslateIntersection(plane, vertex0, vertex1);
                int index2 = TranslateIndex(vertex2);

                StoreTriangle(
                    index0,
                    TranslateIndex(vertex1),
                    index2
                );

                StoreTriangle(
                    index0,
                    index2,
                    TranslateIntersection(plane, vertex0, vertex2)
                );
            }
        } else {
            if (code1 != 0) {
                if (code2 != 0) {
                    // p1 and p2 outside

                    StoreTriangle(
                        TranslateIndex(vertex0),
                        TranslateIntersection(plane, vertex0, vertex1),
                        TranslateIntersection(plane, vertex2, vertex0)
                    );
                } else {
                    // p1 outside

                    int index0 = TranslateIndex(vertex0);
                    int index2 = TranslateIntersection(plane, vertex1, vertex2);

                    StoreTriangle(
                        index0,
                        TranslateIntersection(plane, vertex0, vertex1),
                        index2
                    );

                    StoreTriangle(
                        index0,
                        index2,
                        TranslateIndex(vertex2)
                    );
                }
            } else {
                // p2 outside

                int index0 = TranslateIndex(vertex0);
                int index2 = TranslateIntersection(plane, vertex1, vertex2);

                StoreTriangle(
                    index0,
                    TranslateIndex(vertex1),
                    index2
                );

                StoreTriangle(
                    index0,
                    index2,
                    TranslateIntersection(plane, vertex2, vertex0)
                );
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip Triangles
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipTriangles(VertexTL* pvertex, int vcountIn, const WORD* pindex, int icountIn)
    {
        //
        // Clip each triangle
        //

        for(int index = 0; index < icountIn; index += 3) {
            int index0 = pindex[index + 0];
            int index1 = pindex[index + 1];
            int index2 = pindex[index + 2];

            Code code0 = pvertex[index0].m_code;
            Code code1 = pvertex[index1].m_code;
            Code code2 = pvertex[index2].m_code;

            Code codeAnd = code0 & code1 & code2;

            if (codeAnd == 0) {
                Code codeOr  = code0 | code1 | code2;

                if (codeOr == 0) {
                    //
                    // Not clipped
                    //

                    StoreTriangle(
                        TranslateIndex(pvertex[index0]),
                        TranslateIndex(pvertex[index1]),
                        TranslateIndex(pvertex[index2])
                    );
                } else {
                    //
                    // Partially clipped
                    //

                    Code bit = 1;
                    int pindex = 0;

                    while (
                           (pindex < m_countPlanes)
                        && (codeOr != bit)
                    ) {
                        pindex++;
                        bit *= 2;
                    }

                    if (pindex < m_countPlanes) {
                        //
                        // Clipped by a single plane
                        //

                        ClipPolygonToPlane(
                            m_pplane[pindex],
                            code0,
                            code1,
                            code2,
                            pvertex[index0],
                            pvertex[index1],
                            pvertex[index2]
                        );
                    } else {
                        //
                        // Clipped by multiple planes
                        //

                        ClipPolygonToPlanes(
                            m_pplane,
                            m_countPlanes,
                            codeOr,
                            pvertex[index0],
                            pvertex[index1],
                            pvertex[index2]
                        );
                    }
                }
            } else {
                //
                // Completely clipped
                //

                int i = 0;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip a line to a set of planess
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipLineToPlanes(
        const Plane* pplane,
              int        pcount,
              Code       codeOr,
        const VertexTL&  vertex0,
        const VertexTL&  vertex1
    ) {
        HVector v0(vertex0.x, vertex0.y, vertex0.z, vertex0.w);
        HVector v1(vertex1.x, vertex1.y, vertex1.z, vertex1.w);

        float t0 = 0;
        float t1 = 1;

        int  vcount = 3;
        Code bit    = 1;
        int  pindex = 0;

        while (pindex < pcount) {
            if ((codeOr & bit) != 0) {
                float t = pplane[pindex].Intersect(v0, v1);

                if ((vertex0.m_code & bit) != 0) {
                    if (t > t0) {
                        t0 = t;
                    }
                } else {
                    if (t < t1) {
                        t1 = t;
                    }
                }
            }

            bit *= 2;
            pindex++;
        }

        if (t0 < t1) {
            VertexTL vertexT0; vertexT0.Interpolate(vertex0, vertex1, t0);
            VertexTL vertexT1; vertexT1.Interpolate(vertex0, vertex1, t1);

            StoreLine(
                ConvertVertex(vertexT0),
                ConvertVertex(vertexT1)
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip Lines
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipLines(VertexTL* pvertex, int vcount, const WORD* pindex, int icount)
    {
        for (int index = 0; index < icount; index += 2) {
            int index0 = pindex[index + 0];
            int index1 = pindex[index + 1];

            Code code0 = pvertex[index0].m_code;
            Code code1 = pvertex[index1].m_code;

            Code codeAnd = code0 & code1;

            if (codeAnd == 0) {
                Code codeOr  = code0 | code1;

                if (codeOr == 0) {
                    //
                    // Not clipped
                    //

                    StoreLine(
                        TranslateIndex(pvertex[index0]),
                        TranslateIndex(pvertex[index1])
                    );
                } else {
                    //
                    // Clipped
                    //

                    ClipLineToPlanes(
                        m_pplane,
                        m_countPlanes,
                        codeOr,
                        pvertex[index0],
                        pvertex[index1]
                    );
                }
            } else {
                //
                // Completely clipped
                //
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clip Points
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClipPoints(VertexTL* pvertex, int vcount, const WORD* pindex, int icount)
    {
        for(int index = 0; index < vcount; index++) {
            if (pvertex[index].m_code == 0) {
                ConvertVertex(pvertex[index]);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Transform with clipping
    //
    //////////////////////////////////////////////////////////////////////////////

    void TransformClip(const VertexL* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw)
    {
        SizeVertexTLBuffer(vcount);

        const Matrix& mat = GetFullMatrix();

        for (int index = 0; index < vcount; index++) {
            const float x = pvertex[index].x;
            const float y = pvertex[index].y;
            const float z = pvertex[index].z;

            m_pvertexTLBuffer[index].x = mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3];
            m_pvertexTLBuffer[index].y = mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3];
            m_pvertexTLBuffer[index].z = mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3];
            m_pvertexTLBuffer[index].w = mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3];
            m_pvertexTLBuffer[index].u = pvertex[index].u;
            m_pvertexTLBuffer[index].v = pvertex[index].v;
            m_pvertexTLBuffer[index].r = pvertex[index].r;
            m_pvertexTLBuffer[index].g = pvertex[index].g;
            m_pvertexTLBuffer[index].b = pvertex[index].b;
            m_pvertexTLBuffer[index].a = pvertex[index].a;
            m_pvertexTLBuffer[index].m_index = 0xffff;

            m_pvertexTLBuffer[index].UpdateClipCode();
        }

        if (m_countPlanes > 6) {
            for (int index = 0; index < vcount; index++) {
                Code code = ClipPlane;

                for (int iplane = 6; iplane < m_countPlanes; iplane++, code <<= 1) {
                    m_pvertexTLBuffer[index].UpdateClipCode(m_pplane[iplane], code);
                }
            }
        }

        m_indexVertexScreenBuffer = 0;
        m_indexIndexBuffer = 0;

        (this->*pfnClip)(m_pvertexTLBuffer, vcount, pindex, icount);

        if (m_indexVertexScreenBuffer != 0) {
            (this->*pfnDraw)(
                m_pvertexScreenBuffer, 
                m_indexVertexScreenBuffer, 
                m_pindexBuffer, 
                m_indexIndexBuffer
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Special case Transform and light without clipping
    //
    //////////////////////////////////////////////////////////////////////////////

#ifndef DREAMCAST
#define USEASM
#endif

#ifndef USEASM
    void TransformAndLightNoClip(
        const Vertex* pvertex, 
        int vcount, 
        const WORD* pindex, 
        int icount, 
        PFNClip pfnClip, 
        PFNDraw pfnDraw
    ) {
        SizeVertexScreenBuffer(vcount);

        UpdateLighting();
        const Matrix& mat = GetFullMatrix();

        for (int index = 0; index < vcount; index++) {
            const float x = pvertex[index].x;
            const float y = pvertex[index].y;
            const float z = pvertex[index].z;

            const float rhw = 1.0f / (mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3]);

            m_pvertexScreenBuffer[index].x  = rhw * (mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3]);
            m_pvertexScreenBuffer[index].y  = rhw * (mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3]);
            m_pvertexScreenBuffer[index].z  = rhw * (mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3]);
            m_pvertexScreenBuffer[index].qw = rhw;
            m_pvertexScreenBuffer[index].u  = pvertex[index].u;
            m_pvertexScreenBuffer[index].v  = pvertex[index].v;

            {
                float fbright =
                    255.0f * (
                          pvertex[index].nx * m_vecLight.X()
                        + pvertex[index].ny * m_vecLight.Y()
                        + pvertex[index].nz * m_vecLight.Z()
                        + m_brightAmbient
                    );

#ifndef DREAMCAST
                int bright = 0;
                __asm {
                    fld     fbright
                    fistp   bright
                }
#else
                int bright = (int)fbright;
#endif

                if (bright > 255) {
                    bright = 255;
                } else if (bright < 0) {
                    bright = 0;
                }

                m_pvertexScreenBuffer[index].color = 0xff000000 | bright | (bright << 8) | (bright << 16);
            }
        }

        (this->*pfnDraw)(m_pvertexScreenBuffer, vcount, pindex, icount);
    }
#else
    void TransformAndLightNoClip(
        const Vertex* pvertex, 
        int vcount, 
        const WORD* pindex, 
        int icount, 
        PFNClip pfnClip, 
        PFNDraw pfnDraw
    ) {
        UpdateLighting();
        SizeVertexScreenBuffer(vcount);

        const Matrix mat = GetFullMatrix();
        const Vector vecLight = m_vecLight;
        const float brightAmbient = m_brightAmbient;
        const float mult = 255.0f;
        const float one  = 1.0f;
        int   bright;

        _asm {
            mov     ebx,    255
            mov     ecx,    this
            mov     esi,    pvertex
            mov     edi,    [ecx].m_pvertexScreenBuffer
            mov     ecx,    vcount

           loopTop:
            // int bright = (int)(255.0f * (pvertex[index].GetNormal() * m_vecLight + m_brightAmbient));
            // const float rhw = 1.0f / (mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3]);
            // m_pvertexScreenBuffer[index].SetX(rhw * (mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3]);
            // m_pvertexScreenBuffer[index].SetY(rhw * (mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3]);
            // m_pvertexScreenBuffer[index].SetZ(rhw * (mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3]));
            // m_pvertexScreenBuffer[index].SetTextureCoordinate(pvertex[index].GetTextureCoordinate());

            fld     [vecLight].x                //
            fmul    [esi]Vertex.nx              // lx
            fld     [vecLight].y                //
            fmul    [esi]Vertex.ny              // ly, lx
            fld     [vecLight].z                //
            fmul    [esi]Vertex.nz              // lz, ly, lx
            fxch    st(2)                       // lx, ly, lz
            faddp   st(1), st(0)                // lx + ly, lz
            fld     one                         // 1, lx + ly, lz
            fld     [mat + 48 +  0]             //
            fmul    [esi]Vertex.x               // wx, 1, lx + ly, lz
            fxch    st(3)                       // lz, 1, lx + ly, wx
            fadd    [brightAmbient]             // lz + dl, 1, lx + ly, wx
            fld     [mat + 48 +  4]             //
            fmul    [esi]Vertex.y               // wy, lz + dl, 1, lx + ly, wx
            fxch    st(1)                       // lz + dl, wy, 1, lx + ly, wx
            faddp   st(3), st(0)                // wy, 1, l, wx
            fld     [mat + 48 +  8]             //
            fmul    [esi]Vertex.z               // wz, wy, 1, l, wx
            fxch    st(1)                       // wy, wz, 1, l, wx
            faddp   st(4), st(0)                // wz, 1, l, wx + wy
            fxch    st(2)                       // l, 1, wz, wx + wy
            fmul    mult                        // 256 * l, 1, wz, wx + wy
            fld     [mat +  0 +  0]             //
            fmul    [esi]Vertex.x               // xx, 255 * l, 1, wz, wx + wy
            fxch    st(3)                       // wz, 255 * l, 1, xx, wx + wy
            fadd    DWORD PTR [mat + 48 + 12]   // wz + dw, 255 * l, 1, xx, wx + wy
            fxch    st(1)                       // 255 * l, wz + dw, 1, xx, wx + wy
            fistp   bright                      // wz + dw, 1, xx, wx + wy
            fld     [mat +  0 +  4]             //
            fmul    [esi]Vertex.y               // xy, wz + dw, 1, xx, wx + wy
            fxch    st(1)                       // wz + dw, xy, 1, xx, wx + wy
            faddp   st(4), st(0)                // xy, 1, xx, w
            fld     [mat +  0 +  8]             //
            fmul    [esi]Vertex.z               // xz, xy, 1, xx, w
            fxch    st(1)                       // xy, xz, 1, xx, w
            faddp   st(3), st(0)                // xz, 1, xx + xy, w
            fxch    st(3)                       // w, 1, xx + xy, xz
            fdivp   st(1), st(0)                // rhw, xx + xy, xz
            fxch    st(2)                       // xz, xx + xy, rhw

            // if (bright > 255) {
            //     bright = 255;
            // } else if (bright < 0) {
            //     bright = 0;
            // }

            mov     eax,bright
            cmp     eax,ebx
            jle     label1
            mov     eax,ebx
            jmp     label2
          label1:
            test    eax,eax
            jge     label2
            xor     eax,eax

            // m_pvertexScreenBuffer[index].color = 0xff000000 | bright | (bright << 8) | (bright << 16);

          label2:
            mov     edx,eax
            shl     edx,8
            or      edx,eax
            shl     edx,8
            or      edx,eax
            or      edx, 0xff000000
            mov     [edi]VertexScreen.color, edx

            mov     eax, [esi]Vertex.u
            mov     edx, [esi]Vertex.v
            mov     [edi]VertexScreen.tu, eax
            mov     [edi]VertexScreen.tv, edx

            // continue with floating point stuff

                                                // xz, xx + xy, rhw
            fadd    DWORD PTR [mat +  0 + 12]   // xz + dx, xx + xy, rhw
            fld     [mat + 16 +  0]             //
            fmul    [esi]Vertex.x               // yx, xz + dx, xx + xy, rhw
            fld     [mat + 16 +  4]             //
            fmul    [esi]Vertex.y               // yy, yx, xz + dx, xx + xy, rhw
            fld     [mat + 16 +  8]             //
            fmul    [esi]Vertex.z               // yz, yy, yx, xz + dx, xx + xy, rhw
            fxch    st(1)                       // yy, yz, yx, xz + dx, xx + xy, rhw
            faddp   st(2), st(0)                // yz, yx + yy, xz + dx, xx + xy, rhw
            fld     [mat + 32 +  0]             //
            fmul    [esi]Vertex.x               // zx, yz, yx + yy, xz + dx, xx + xy, rhw
            fld     [mat + 32 +  4]             //
            fmul    [esi]Vertex.y               // zy, zx, yz, yx + yy, xz + dx, xx + xy, rhw
            fld     [mat + 32 +  8]             //
            fmul    [esi]Vertex.z               // zz, zy, zx, yz, yx + yy, xz + dx, xx + xy, rhw
            fxch    st(3)                       // yz, zy, zx, zz, yx + yy, xz + dx, xx + xy, rhw
            fadd    DWORD PTR [mat + 16 + 12]   // yz + dy, zy, zx, zz, yx + yy, xz + dx, xx + xy, rhw
            fxch    st(1)                       // zy, yz + dy, zx, zz, yx + yy, xz + dx, xx + xy, rhw
            faddp   st(2), st(0)                // yz + dy, zx + zy, zz, yx + yy, xz + dx, xx + xy, rhw
            fxch    st(2)                       // zz, zx + zy, yz + dy, yx + yy, xz + dx, xx + xy, rhw
            fadd    DWORD PTR [mat + 32 + 12]   // zz + dz, zx + zy, yz + dy, yx + yy, xz + dx, xx + xy, rhw
            fxch    st(4)                       // xz + dx, zx + zy, yz + dy, yx + yy, zz + dz, xx + xy, rhw
            faddp   st(5), st(0)                // zx + zy, yz + dy, yx + yy, zz + dz, x, rhw
            fxch    st(1)                       // yz + dy, zx + zy, yx + yy, zz + dz, x, rhw
            faddp   st(2), st(0)                // zx + zy, y, zz + dz, x, rhw
            faddp   st(2), st(0)                // y, z, x, rhw
            fxch    st(2)                       // x, z, y, rhw
            fmul    st(0), st(3)                // rhw*x, z, y, rhw
            fxch    st(2)                       // y, z, rhw*x, rhw
            fmul    st(0), st(3)                // rhw*y, z, rhw*x, rhw
            fxch    st(1)                       // z, rhw*y, rhw*x, rhw
            fmul    st(0), st(3)                // rhw*z, rhw*y, rhw*x, rhw
            fxch    st(3)                       // rhw, rhw*y, rhw*x, rhw*z
            fstp    [edi]VertexScreen.rhw       // rhw*y, rhw*x, rhw*z
            fxch    st(1)                       // rhw*x, rhw*y, rhw*z
            fstp    [edi]VertexScreen.x        // rhw*y, rhw*z
            fstp    [edi]VertexScreen.y        // rhw*z
            fstp    [edi]VertexScreen.z        //

            // loop

            add     edi, SIZE VertexScreen
            add     esi, SIZE Vertex
            dec     ecx
            jne     loopTop
        }

        (this->*pfnDraw)(m_pvertexScreenBuffer, vcount, pindex, icount);
    }
#endif

    //////////////////////////////////////////////////////////////////////////////
    //
    // Tranform without clipping
    //
    //////////////////////////////////////////////////////////////////////////////

    void TransformNoClip(const VertexL* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw)
    {
        SizeVertexScreenBuffer(vcount);

        const Matrix& mat = GetFullMatrix();

        for (int index = 0; index < vcount; index++) {
            const float x = pvertex[index].x;
            const float y = pvertex[index].y;
            const float z = pvertex[index].z;

            const float rhw = 1.0f / (mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3]);

            m_pvertexScreenBuffer[index].x  = rhw * (mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3]);
            m_pvertexScreenBuffer[index].y  = rhw * (mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3]);
            m_pvertexScreenBuffer[index].z  = rhw * (mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3]);
            m_pvertexScreenBuffer[index].qw = rhw;
            m_pvertexScreenBuffer[index].u  = pvertex[index].u;
            m_pvertexScreenBuffer[index].v  = pvertex[index].v;

            // !!! write asm MakeD3DColor

            m_pvertexScreenBuffer[index].color = 
                MakeD3DCOLOR(
                    Color(
                        pvertex[index].r,
                        pvertex[index].g,
                        pvertex[index].b,
                        pvertex[index].a
                    )
                );
        }

        (this->*pfnDraw)(m_pvertexScreenBuffer, vcount, pindex, icount);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Copy Lighting
    //
    //////////////////////////////////////////////////////////////////////////////

    void LightVertexCopy(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw)
    {
        SizeVertexLBuffer(vcount);

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBuffer[index].x = pvertex[index].x;
            m_pvertexLBuffer[index].y = pvertex[index].y;
            m_pvertexLBuffer[index].z = pvertex[index].z;
            m_pvertexLBuffer[index].u = pvertex[index].u;
            m_pvertexLBuffer[index].v = pvertex[index].v;
            m_pvertexLBuffer[index].r = 1;
            m_pvertexLBuffer[index].g = 1;
            m_pvertexLBuffer[index].b = 1;
            m_pvertexLBuffer[index].a = 1;
        }

        (this->*m_pfnTransform)(m_pvertexLBuffer, vcount, pindex, icount, pfnClip, pfnDraw);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Directional Lighting
    //
    //////////////////////////////////////////////////////////////////////////////

    void LightVertex(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw)
    {
        UpdateLighting();

        SizeVertexLBuffer(vcount);

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBuffer[index].x = pvertex[index].x;
            m_pvertexLBuffer[index].y = pvertex[index].y;
            m_pvertexLBuffer[index].z = pvertex[index].z;
            m_pvertexLBuffer[index].u = pvertex[index].u;
            m_pvertexLBuffer[index].v = pvertex[index].v;

            float bright =
                  pvertex[index].nx * m_vecLight.X()
                + pvertex[index].ny * m_vecLight.Y()
                + pvertex[index].nz * m_vecLight.Z()
                + m_brightAmbient;

            if (bright > 1) {
                bright = 1;
            } else if (bright < 0) {
                bright = 0;
            }

            m_pvertexLBuffer[index].r = bright;
            m_pvertexLBuffer[index].g = bright;
            m_pvertexLBuffer[index].b = bright;
            m_pvertexLBuffer[index].a = 1;
        }

        (this->*m_pfnTransform)(m_pvertexLBuffer, vcount, pindex, icount, pfnClip, pfnDraw);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Directional Lighting
    //
    //////////////////////////////////////////////////////////////////////////////

    void MaterialLightVertex(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNClip pfnClip, PFNDraw pfnDraw)
    {
        UpdateLighting();

        SizeVertexLBuffer(vcount);

        float r = m_pmaterial->GetDiffuse().GetRed();
        float g = m_pmaterial->GetDiffuse().GetGreen();
        float b = m_pmaterial->GetDiffuse().GetBlue();
        float a = m_pmaterial->GetDiffuse().GetAlpha();

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBuffer[index].x = pvertex[index].x;
            m_pvertexLBuffer[index].y = pvertex[index].y;
            m_pvertexLBuffer[index].z = pvertex[index].z;
            m_pvertexLBuffer[index].u = pvertex[index].u;
            m_pvertexLBuffer[index].v = pvertex[index].v;

            float bright =
                  pvertex[index].nx * m_vecLight.X()
                + pvertex[index].ny * m_vecLight.Y()
                + pvertex[index].nz * m_vecLight.Z()
                + m_brightAmbient;

            if (bright > 1) {
                bright = 1;
            } else if (bright < 0) {
                bright = 0;
            }

            m_pvertexLBuffer[index].r = r * bright;
            m_pvertexLBuffer[index].g = g * bright;
            m_pvertexLBuffer[index].b = b * bright;
            m_pvertexLBuffer[index].a = a;
        }

        (this->*m_pfnTransform)(m_pvertexLBuffer, vcount, pindex, icount, pfnClip, pfnDraw);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Lighting for D3D Transformations
    //
    //////////////////////////////////////////////////////////////////////////////

    void LightVertexD3D(
        const Vertex* pvertex, 
        int vcount, 
        const WORD* pindex, 
        int icount, 
        PFNDrawD3D pfnDraw
    ) {
        UpdateLighting();
        SizeVertexLBufferD3D(vcount);
        
        for (int index = 0; index < vcount; index++) {
            m_pvertexLBufferD3D[index].x = pvertex[index].x;
            m_pvertexLBufferD3D[index].y = pvertex[index].y;
            m_pvertexLBufferD3D[index].z = pvertex[index].z;
            m_pvertexLBufferD3D[index].tu = pvertex[index].u;
            m_pvertexLBufferD3D[index].tv = pvertex[index].v;

           float bright =
                      pvertex[index].nx * m_vecLight.X()
                    + pvertex[index].ny * m_vecLight.Y()
                    + pvertex[index].nz * m_vecLight.Z()
                    + m_brightAmbient;

            if (bright > 1) {
                bright = 1;
            } else if (bright < 0) {
                bright = 0;
            }

            m_pvertexLBufferD3D[index].color = MakeD3DCOLOR(Color(bright, bright, bright, 1));
            m_pvertexLBufferD3D[index].specular = 0;
        }

        (this->*pfnDraw)(m_pvertexLBufferD3D, vcount, pindex, icount);
    }

    void CopyLightVertexD3D(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNDrawD3D pfnDraw)
    {
        SizeVertexLBufferD3D(vcount);

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBufferD3D[index].x = pvertex[index].x;
            m_pvertexLBufferD3D[index].y = pvertex[index].y;
            m_pvertexLBufferD3D[index].z = pvertex[index].z;
            m_pvertexLBufferD3D[index].tu = pvertex[index].u;
            m_pvertexLBufferD3D[index].tv = pvertex[index].v;
            m_pvertexLBufferD3D[index].color = MakeD3DCOLOR(Color(1, 1, 1, 1));
            m_pvertexLBufferD3D[index].specular = 0;
        }

        (this->*pfnDraw)(m_pvertexLBufferD3D, vcount, pindex, icount);
    }

    void MaterialLightVertexD3D(const Vertex* pvertex, int vcount, const WORD* pindex, int icount, PFNDrawD3D pfnDraw)
    {
        UpdateLighting();

        SizeVertexLBufferD3D(vcount);

        float r = m_pmaterial->GetDiffuse().GetRed();
        float g = m_pmaterial->GetDiffuse().GetGreen();
        float b = m_pmaterial->GetDiffuse().GetBlue();
        float a = m_pmaterial->GetDiffuse().GetAlpha();

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBufferD3D[index].x = pvertex[index].x;
            m_pvertexLBufferD3D[index].y = pvertex[index].y;
            m_pvertexLBufferD3D[index].z = pvertex[index].z;
            m_pvertexLBufferD3D[index].tu = pvertex[index].u;
            m_pvertexLBufferD3D[index].tv = pvertex[index].v;

            float bright =
                  pvertex[index].nx * m_vecLight.X()
                + pvertex[index].ny * m_vecLight.Y()
                + pvertex[index].nz * m_vecLight.Z()
                + m_brightAmbient;

            if (bright > 1) {
                bright = 1;
            } else if (bright < 0) {
                bright = 0;
            }
            
            m_pvertexLBufferD3D[index].color = 
                        MakeD3DCOLOR(
                            Color(r * bright, g * bright, b * bright, a));

            m_pvertexLBufferD3D[index].specular = 0;
        }

        (this->*pfnDraw)(m_pvertexLBufferD3D, vcount, pindex, icount);
    }

    void CopyDrawD3D(const VertexL* pvertex, int vcount, const WORD* pindex, int icount, PFNDrawD3D pfnDraw)
    {
        SizeVertexLBufferD3D(vcount);

        for (int index = 0; index < vcount; index++) {
            m_pvertexLBufferD3D[index].x = pvertex[index].x;
            m_pvertexLBufferD3D[index].y = pvertex[index].y;
            m_pvertexLBufferD3D[index].z = pvertex[index].z;
            m_pvertexLBufferD3D[index].tu = pvertex[index].u;
            m_pvertexLBufferD3D[index].tv = pvertex[index].v;
            m_pvertexLBufferD3D[index].color = 
                        MakeD3DCOLOR(
                            Color(
                                pvertex[index].r, 
                                pvertex[index].g, 
                                pvertex[index].b,
                                pvertex[index].a));
            m_pvertexLBufferD3D[index].specular = 0;
        }

        (this->*pfnDraw)(m_pvertexLBufferD3D, vcount, pindex, icount);
    }

    
    //////////////////////////////////////////////////////////////////////////////
    //
    // Vertices need lighting
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTriangles(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawTriangles(pvertex, vcount, pindex, icount);
    }

    void DrawLines(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawLines(pvertex, vcount, pindex, icount);
    }

    void DrawPoints(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawPoints(pvertex, vcount);
    }

    void DrawPoints(const VertexScreen* pvertex, int vcount)
    {
        m_prasterizer->DrawPoints(pvertex, vcount);
    }

    void DrawTriangles(const D3DVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countTriangles += icount / 3;
        #endif

        m_prasterizer->DrawTrianglesD3D(pvertex, vcount, pindex, icount);
    }

    void DrawLines(const D3DVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countLines += icount / 3;
        #endif

        m_prasterizer->DrawLinesD3D(pvertex, vcount, pindex, icount);
    }

    void DrawPoints(const D3DVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countPoints += icount - 1;
        #endif

        m_prasterizer->DrawPointsD3D(pvertex, vcount);
    }

    void DrawPoints(const D3DVertex* pvertex, int vcount)
    {
        #ifdef EnablePerformanceCounters
            m_countPoints += vcount;
        #endif

        m_prasterizer->DrawPointsD3D(pvertex, vcount);
    }

    void DrawTriangles(const D3DLVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawTrianglesD3D(pvertex, vcount, pindex, icount);
    }

    void DrawLines(const D3DLVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawLinesD3D(pvertex, vcount, pindex, icount);
    }

    void DrawPoints(const D3DLVertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        m_prasterizer->DrawPointsD3D(pvertex, vcount);
    }

    void DrawPoints(const D3DLVertex* pvertex, int vcount)
    {
        m_prasterizer->DrawPointsD3D(pvertex, vcount);
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTriangles(const Vertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countTriangles += icount / 3;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            (this->*m_pfnLightD3D)(pvertex, vcount, pindex, icount, DrawTriangles);
        else
            (this->*m_pfnLightVertex)(pvertex, vcount, pindex, icount, ClipTriangles, DrawTriangles);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawLines(const Vertex* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countLines += icount - 1;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            (this->*m_pfnLightD3D)(pvertex, vcount, pindex, icount, DrawLines);
        else
            (this->*m_pfnLightVertex)(pvertex, vcount, pindex, icount, ClipLines, DrawLines);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawPoints(const Vertex* pvertex, int vcount)
    {
        #ifdef EnablePerformanceCounters
            m_countPoints += vcount;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            (this->*m_pfnLightD3D)(pvertex, vcount, NULL, 0, DrawPoints);
        else
            (this->*m_pfnLightVertex)(pvertex, vcount, NULL, 0, ClipPoints, DrawPoints);
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Draw Colored Triangles
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTriangles(const VertexL* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countTriangles += icount / 3;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            CopyDrawD3D(pvertex, vcount, pindex, icount, DrawTriangles);
        else
            (this->*m_pfnLightVertexL)(pvertex, vcount, pindex, icount, ClipTriangles, DrawTriangles);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Draw Colored Lines
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawLines(const VertexL* pvertex, int vcount, const WORD* pindex, int icount)
    {
        #ifdef EnablePerformanceCounters
            m_countLines += icount - 1;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            CopyDrawD3D(pvertex, vcount, pindex, icount, DrawLines);
        else
            (this->*m_pfnLightVertexL)(pvertex, vcount, pindex, icount, ClipLines, DrawLines);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Draw Colored Points
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawPoints(const VertexL* pvertex, int vcount)
    {
        #ifdef EnablePerformanceCounters
            m_countPoints += vcount;
        #endif

        UpdatePointers();
        if (m_bDrawD3D)
            CopyDrawD3D(pvertex, vcount, NULL, 0, DrawPoints);
        else
            (this->*m_pfnLightVertexL)(pvertex, vcount, NULL, 0, ClipPoints, DrawPoints);
    }
};

TRef<IDevice3D> CreateDevice3D(IRasterizer* prasterizerArg)
{
    ID3DRasterizer* prasterizer; CastTo(prasterizer, prasterizerArg);
    return new Device3D(prasterizer);
}
