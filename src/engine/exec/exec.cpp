#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Execute Buffer
//
//////////////////////////////////////////////////////////////////////////////

#define Pointer(type, p) type* p = (type*)m_pops; m_pops += sizeof(type);

class ExecuteBuffer : public GCObject {
private:
    TRef<IDirect3DDevice>        m_pd3dd;
    TRef<IDirect3DViewport>      m_pd3dview;
    TRef<IDirect3DExecuteBuffer> m_pd3dexec;

    D3DEXECUTEBUFFERDESC m_d3dexecd;
    char* m_pdata;
    char* m_pops;
    char* m_popsStart;

    int m_count;
    bool m_bLocked;

public:
    ExecuteBuffer(IDirect3DDevice* pd3dd, IDirect3DViewport* pd3dview) :
        m_pd3dd(pd3dd),
        m_pd3dview(pd3dview),
        m_count(0),
        m_bLocked(false),
        m_pdata(NULL),
        m_popsStart(NULL),
        m_pops(NULL)
    {
        m_d3dexecd.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
        m_d3dexecd.dwFlags = D3DDEB_BUFSIZE | D3DDEB_CAPS;
        m_d3dexecd.dwCaps = D3DDEBCAPS_SYSTEMMEMORY;
        m_d3dexecd.dwBufferSize = 64 * 1024;

        DDCall(m_pd3dd->CreateExecuteBuffer(&m_d3dexecd, &m_pd3dexec, NULL));
    }

    void AddOp(D3DOPCODE code, int size, int count)
    {
        Pointer(D3DINSTRUCTION, p);

        p->bOpcode = code;
        p->bSize   = size;
        p->wCount  = count;
    }

    void Lock()
    {
        if (!m_bLocked) {
            m_bLocked = true;
            DDCall(m_pd3dexec->Lock(&m_d3dexecd));
            m_pdata = (char*)m_d3dexecd.lpData;
        }
    }

    void Unlock()
    {
        EndOps();
        if (m_bLocked) {
            m_bLocked = false;
            DDCall(m_pd3dexec->Unlock());
        }
    }

    void StartOps()
    {
        if (m_popsStart == NULL) {
            Lock();
            m_popsStart = m_pdata + m_count * sizeof(D3DVERTEX);
            m_pops = m_popsStart;
        }
    }

    void EndOps()
    {
        if (m_popsStart) {
            AddOp(D3DOP_EXIT, 0, 0);

            D3DEXECUTEDATA d3dexecdata;

            d3dexecdata.dwSize              = sizeof(D3DEXECUTEDATA);
            d3dexecdata.dwVertexOffset      = 0;
            d3dexecdata.dwVertexCount       = m_count;
            d3dexecdata.dwInstructionOffset = m_popsStart - m_pdata;
            d3dexecdata.dwInstructionLength = m_pops - m_popsStart;
            d3dexecdata.dwHVertexOffset     = 0;
            d3dexecdata.dsStatus.dwFlags    = D3DSETSTATUS_STATUS;
            d3dexecdata.dsStatus.dwStatus   = 0;

            DDCall(m_pd3dexec->SetExecuteData(&d3dexecdata));

            m_pops = NULL;
            m_popsStart = NULL;
        }
    }

    void ProcessVertices()
    {
        AddOp(D3DOP_PROCESSVERTICES, sizeof(D3DPROCESSVERTICES), 1);
        Pointer(D3DPROCESSVERTICES, p);

        //p->dwFlags    = D3DPROCESSVERTICES_COPY;
        p->dwFlags    = D3DPROCESSVERTICES_TRANSFORMLIGHT;
        p->wStart     = 0;
        p->wDest      = 0;
        p->dwCount    = m_count;
        p->dwReserved = 0;
    }

    //
    // Vertex methods
    //

    HRESULT SetVertexCount(int count)
    {
        Unlock();
        m_count = count;

        return S_OK;
    }

    HRESULT SetVertex(int index, const D3DVertex& vertex)
    {
        ZAssert(index < m_count);

        Lock();
        D3DVERTEX* p = ((D3DVERTEX*)m_pdata) + index;

        *p = vertex;

        return S_OK;
    }

    HRESULT Execute()
    {
        Unlock();

        DDCall(m_pd3dd->Execute(m_pd3dexec, m_pd3dview, D3DEXECUTE_CLIPPED));

        D3DEXECUTEDATA d3dexecdata;
        d3dexecdata.dwSize              = sizeof(D3DEXECUTEDATA);
        d3dexecdata.dsStatus.dwStatus   = D3DSETSTATUS_ALL;
        DDCall(m_pd3dexec->GetExecuteData(&d3dexecdata));

        D3DSTATS stats;
        stats.dwSize = sizeof(D3DSTATS);
        DDCall(m_pd3dd->GetStats(&stats));

        return S_OK;
    }

    HRESULT SetTexture(Surface* psurface)
    {
        StartOps();
        TRef<IDirect3DTexture> pd3dtexture;
        D3DTEXTUREHANDLE hd3dtexture;

        DDCall(psurface->GetDDS()->QueryInterface(IID_IDirect3DTexture, (void**)&pd3dtexture));
        DDCall(pd3dtexture->GetHandle(m_pd3dd, &hd3dtexture));
        SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, hd3dtexture);

        return S_OK;
    }

    HRESULT StartStrip(int count)
    {
        StartOps();
        AddOp(D3DOP_TRIANGLE, sizeof(D3DTRIANGLE), count);

        return S_OK;
    }

    HRESULT AddTriangle(int v1, int v2, int v3)
    {
        StartOps();
        Pointer(D3DTRIANGLE,  p);

        p->v1 = v1;
        p->v2 = v2;
        p->v3 = v3;
        p->wFlags = D3DTRIFLAG_START | D3DTRIFLAG_EDGEENABLETRIANGLE;

        return S_OK;
    }

    HRESULT StartLineStrip(int count)
    {
        StartOps();
         AddOp(D3DOP_LINE, sizeof(D3DLINE), count);

        return S_OK;
    }

    HRESULT AddLine(int v1, int v2)
    {
        StartOps();
        Pointer(D3DLINE,  p);

        p->v1 = v1;
        p->v2 = v2;

        return S_OK;
    }

    HRESULT SetRenderState(D3DRENDERSTATETYPE stateType, DWORD arg)
    {
        StartOps();
        AddOp(D3DOP_STATERENDER, sizeof(D3DSTATE), 1);
        Pointer(D3DSTATE, p);

        p->drstRenderStateType = stateType;
        p->dwArg[0] = arg;

        return S_OK;
    }

    HRESULT SetLightState(D3DLIGHTSTATETYPE stateType, DWORD arg)
    {
        StartOps();
        AddOp(D3DOP_STATELIGHT, sizeof(D3DSTATE), 1);
        Pointer(D3DSTATE, p);

        p->dlstLightStateType = stateType;
        p->dwArg[0] = arg;

        return S_OK;
    }

    HRESULT SetTransformState(D3DTRANSFORMSTATETYPE stateType, DWORD arg)
    {
        StartOps();
        AddOp(D3DOP_STATETRANSFORM, sizeof(D3DSTATE), 1);
        Pointer(D3DSTATE, p);

        p->dtstTransformStateType = stateType;
        p->dwArg[0] = arg;

        return S_OK;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Context3D
//
//
//
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////

class D3DIMContext3D : public Context3D {
private:
    TRef<Surface>           m_psurface;

    TRef<IDirect3D>         m_pd3d;
    TRef<IDirect3DDevice>   m_pd3dd;

    TRef<IDirect3DLight>    m_plight;
    TRef<IDirect3DViewport> m_pd3dview;
    TRef<ExecuteBuffer>     m_pexec;

    TRef<IDirect3DMaterial> m_pmat;
    D3DMATERIALHANDLE       m_hmat;

    D3DMATRIXHANDLE         m_hmatrixWorld;
    D3DMATRIXHANDLE         m_hmatrixProjection;
    D3DMATRIXHANDLE         m_hmatrixView;

    //
    // state
    //

    class State : public GCObject {
    public:
        TRef<State> m_pnext;
        Matrix m_mat;

        State(const Matrix& mat, State* pstate) :
            m_mat(mat),
            m_pnext(pstate)
        {
        }
    };

    TRef<State> m_pstate;

public:
    D3DIMContext3D(IDirect3D* pd3d, const IID& iid, Surface* psurface) :
        m_pd3d(pd3d),
        m_psurface(psurface)
    {
        //
        // Initialize the state
        //

        Matrix mat;
        mat.SetIdentity();
        m_pstate = new State(mat, NULL);

        //
        // create the d3d device
        //

        DDCall(m_psurface->GetDDS()->QueryInterface(iid, (void**)&m_pd3dd));

        //
        // set a viewport
        //

        DDCall(m_pd3d->CreateViewport(&m_pd3dview, NULL));
        DDCall(m_pd3dd->AddViewport(m_pd3dview));

        D3DVIEWPORT view;

        view.dwSize   = sizeof(D3DVIEWPORT);
        view.dwX      = 0;
        view.dwY      = 0;
        view.dwWidth  = m_psurface->GetRect().XSize();
        view.dwHeight = m_psurface->GetRect().YSize();
        view.dvScaleX = (float)(m_psurface->GetRect().XSize() / 2);
        view.dvScaleY = view.dvScaleX;
        view.dvMaxX   = 1;
        view.dvMaxY   = (float)m_psurface->GetRect().YSize() / (float)m_psurface->GetRect().XSize();
        view.dvMinZ   = 0;
        view.dvMaxZ   = 1;

        /*
        view.dwSize   = sizeof(D3DVIEWPORT);
        view.dwX      = 0;
        view.dwY      = 0;
        view.dwWidth  = (int)m_psurface->GetRect().XSize();
        view.dwHeight = (int)m_psurface->GetRect().YSize();
        view.dvScaleX = 1;
        view.dvScaleY = 1;
        view.dvMaxX   = view.dwWidth / 2.0f;
        view.dvMaxY   = view.dwHeight / 2.0f;
        view.dvMinZ   = 0;
        view.dvMaxZ   = 1;
        */

        DDCall(m_pd3dview->SetViewport(&view));

        //
        // create a matrix
        //

        DDCall(m_pd3dd->CreateMatrix(&m_hmatrixWorld));
        DDCall(m_pd3dd->CreateMatrix(&m_hmatrixProjection));
        DDCall(m_pd3dd->CreateMatrix(&m_hmatrixView));

        //
        // make a material
        //

        D3DMATERIAL material;

        material.dwSize     = sizeof(material);
        material.diffuse    = D3DColor(1, 1, 1, 0);
        material.ambient    = D3DColor(1, 1, 1, 0);
        material.specular   = D3DColor(0, 0, 0, 0);
        material.emissive   = D3DColor(0, 0, 0, 0);
        material.power      = 1;
        material.hTexture   = NULL;
        material.dwRampSize = 0;

        DDCall(m_pd3d->CreateMaterial(&m_pmat, NULL));
        DDCall(m_pmat->SetMaterial(&material));
        DDCall(m_pmat->GetHandle(m_pd3dd, &m_hmat));

        //DDCall(m_pd3dview->SetBackground(m_hmatBack));

        //
        // Create the execute buffer
        //

        m_pexec = new ExecuteBuffer(m_pd3dd, m_pd3dview);
    }

    HRESULT BeginScene()
    {
        DDCall(m_pd3dd->BeginScene());

        Matrix mat;
        mat.SetIdentity();
        m_pstate = new State(mat, NULL);

        D3DRect 
            rect(
                0, 
                0, 
                (long)m_psurface->GetRect().XSize(),
                (long)m_psurface->GetRect().YSize()
            );

        DDCall(m_pd3dview->Clear(1, &rect, D3DCLEAR_ZBUFFER));

        return S_OK;
    }

    HRESULT EndScene()
    {
        DDCall(m_pd3dd->EndScene());
        return S_OK;
    }

    HRESULT PushState()
    {
        m_pstate = new State(m_pstate->m_mat, m_pstate);
        return S_OK;
    }

    HRESULT PopState()
    {
        m_pstate = m_pstate->m_pnext;
        return S_OK;
    }

    void SetMatrix(D3DMATRIXHANDLE hmatrix, const Matrix& mat)
    {
        D3DMATRIX dmat;

        dmat._11 = mat[0][0];
        dmat._12 = mat[1][0];
        dmat._13 = mat[2][0];
        dmat._14 = mat[3][0];
                            
        dmat._21 = mat[0][1];
        dmat._22 = mat[1][1];
        dmat._23 = mat[2][1];
        dmat._24 = mat[3][1];
                            
        dmat._31 = mat[0][2];
        dmat._32 = mat[1][2];
        dmat._33 = mat[2][2];
        dmat._34 = mat[3][2];
                            
        dmat._41 = mat[0][3];
        dmat._42 = mat[1][3];
        dmat._43 = mat[2][3];
        dmat._44 = mat[3][3];

        m_pd3dd->SetMatrix(hmatrix, &dmat);
    }

    HRESULT Transform(const Matrix& mat)
    {
        m_pstate->m_mat.PreMultiply(mat);
        SetMatrix(m_hmatrixWorld, m_pstate->m_mat);

        return S_OK;
    }

    HRESULT OrthogonalCamera(float znear, float zfar)
    {
        Matrix mat;

        // transform the model by the inverse of the camera transform

        mat.SetInverse(m_pstate->m_mat);

        // switch to left handed coordinates

        mat.Scale(-1, -1, -1); 

        SetMatrix(m_hmatrixView, mat);

        //
        // projection:
        //
        // zbuffer = (z - znear) / (zfar - znear)
        //

        mat.SetIdentity();

        mat.Set(0, 0, 1);
        mat.Set(1, 1, 1);

        float q = 1 / (zfar - znear);
        mat.Set(2, 2, q);
        mat.Set(2, 3, -q * znear);

        SetMatrix(m_hmatrixProjection, mat);

        return S_OK;
    }

    HRESULT PerspectiveCamera(float znear, float zfar, float f)
    {
        Matrix mat;

        // transform the model by the inverse of the camera transform

        mat.SetInverse(m_pstate->m_mat);

        // switch to left handed coordinates

        mat.Scale(-1, 1, -1); 
        SetMatrix(m_hmatrixView, mat);

        // with left handed coordinates
        //
        // projection:  xscreen = f * x / z
        //
        // zbuffer = (z - znear) / (zfar - znear)
        //

        mat.SetIdentity();
        mat.Set(0, 0, f);
        mat.Set(1, 1, f);

        float q = 1 / (zfar - znear);

        mat.Set(2, 2, 0);
        mat.Set(2, 3, 0.5);

        //mat.Set(2, 2, q);
        //mat.Set(2, 3, -q * znear);

        mat.Set(3, 2, 1);
        mat.Set(3, 3, 0);

        SetMatrix(m_hmatrixProjection, mat);

        return S_OK;
    }

    HRESULT FOVCamera(float znear, float zfar, float fov)
    {
        return PerspectiveCamera(znear, zfar, cos(0.5f * fov) / sin(0.5f * fov));
    }

    HRESULT DirectionalLight(const Vector& direction)
    {
        if (m_plight) {
            DDCall(m_pd3dview->DeleteLight(m_plight));
        }

        D3DLIGHT light;

        light.dwSize         = sizeof(D3DLIGHT);
        light.dltType        = D3DLIGHT_DIRECTIONAL;
        light.dcvColor       = D3DColor(1, 0, 0, 0);
        light.dvPosition     = D3DVector(0, 0, 0);
        light.dvDirection    = D3DVector(direction.X(), direction.Y(), -direction.Z());
        light.dvRange        = 0;
        light.dvFalloff      = 0;
        light.dvAttenuation0 = 1;
        light.dvAttenuation1 = 0;
        light.dvAttenuation2 = 0;
        light.dvTheta        = 0;
        light.dvPhi          = 0;

        DDCall(m_pd3d->CreateLight(&m_plight, NULL));
        DDCall(m_plight->SetLight(&light));
        DDCall(m_pd3dview->AddLight(m_plight));

        return S_OK;
    }

    HRESULT DrawMesh(const TVector<D3DVertex>& vertices, const TVector<int>& indices)
    {
        //
        // Add the vertices
        //

        int count = vertices.GetCount();
        m_pexec->SetVertexCount(count);

        int index;
        for(index = 0; index < count; index++) {
            m_pexec->SetVertex(index, vertices[index]);
        }

        //
        // setup the render states
        //

        m_pexec->SetTransformState(D3DTRANSFORMSTATE_WORLD,      m_hmatrixWorld); 
        m_pexec->SetTransformState(D3DTRANSFORMSTATE_PROJECTION, m_hmatrixProjection); 
        m_pexec->SetTransformState(D3DTRANSFORMSTATE_VIEW,       m_hmatrixView); 

        m_pexec->SetLightState(D3DLIGHTSTATE_MATERIAL, m_hmat); 
        m_pexec->SetLightState(D3DLIGHTSTATE_AMBIENT, RGBA_MAKE(0, 0, 64, 0));

        m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, NULL);
        m_pexec->SetRenderState(D3DRENDERSTATE_DITHERENABLE, true);

        m_pexec->SetRenderState(D3DRENDERSTATE_ZENABLE, true);
        m_pexec->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);
        //m_pexec->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_GREATER);
        //m_pexec->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_NEVER);
        //m_pexec->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
        m_pexec->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);

        //m_pexec->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

        m_pexec->ProcessVertices();

        //
        // count the triangles
        //

        int cTriangle = 0;
        int idw = 0;

        count = indices.GetCount();
        while (idw < count) {
            int cVertex = indices[idw++];
            cTriangle += cVertex - 2;
            idw += cVertex;
        }

        //
        // add the triangles to the execute buffer
        //

        m_pexec->StartStrip(cTriangle);

        idw = 0;
        count = indices.GetCount();
        while (idw < count) {
            int cVertex = indices[idw++];

            for (int iVertex = 0; iVertex < cVertex - 2; iVertex++) {
                m_pexec->AddTriangle(
                    indices[idw],
                    indices[idw + iVertex + 2],
                    indices[idw + iVertex + 1]
                );
            }
            idw += cVertex;
        }

        //
        // draw them
        //

        m_pexec->Execute();

        return S_OK;
    }

    /*
    HRESULT DrawPolygon(TVector<Point>* ppoints)
    {
        if (ppoints) {
            const TVector<Point>& points = *ppoints;
            m_pexec->SetVertexCount(points.GetCount());

            int index;
            for(index = 0; index < points.GetCount(); index++) {
                m_pexec->SetTLVertex(
                    index,
                    points[index].X(), points[index].Y(), 0,
                    0, 0,
                    m_color
                );
            }

            m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, NULL);

            m_pexec->StartLineStrip(points.GetCount());
            for(index = 0; index < points.GetCount() - 1; index++) {
                m_pexec->AddLine(index, index + 1);
            }
            m_pexec->AddLine(points.GetCount() - 1, 0);

            DDCall(m_pd3dd->BeginScene());
            m_pexec->Execute();
            DDCall(m_pd3dd->EndScene());
        }

        return S_OK;
    }

    HRESULT FillPolygon(TVector<Point>* ppoints)
    {
        if (ppoints) {
            const TVector<Point>& points = *ppoints;
            m_pexec->SetVertexCount(points.GetCount());

            int index;
            for(index = 0; index < points.GetCount(); index++) {
                m_pexec->SetTLVertex(
                    index,
                    points[index].X(), points[index].Y(), 0,
                    0, 0,
                    m_color
                );
            }

            m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, NULL);

            m_pexec->StartStrip(points.GetCount() - 2);
            for(index = 1; index < points.GetCount() - 1; index++) {
                m_pexec->AddTriangle(0, index, index + 1);
            }

            DDCall(m_pd3dd->BeginScene());
            m_pexec->Execute();
            DDCall(m_pd3dd->EndScene());
        }

        return S_OK;
    }

    HRESULT FillTexturePolygon(
        TVector<Point>* ppoints,
        Surface* psurface,
        TVector<Point>* ppointsTexture
    ) {
        if (ppoints) {
            const TVector<Point>& points = *ppoints;
            const TVector<Point>& pointsTexture = *ppointsTexture;

            m_pexec->SetVertexCount(points.GetCount());

            int index;
            for(index = 0; index < points.GetCount(); index++) {
                m_pexec->SetTLVertex(
                    index,
                    points[index].X(), points[index].Y(), 0,
                    pointsTexture[index].X(),
                    pointsTexture[index].Y(),
                    m_color
                );
            }

            m_pexec->SetTexture(psurface);

            if (true) { //psurface->IsOpaque()) {
                m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_COPY);
                m_pexec->SetRenderState(D3DRENDERSTATE_BLENDENABLE, false);
            } else {
                m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_DECAL);
                m_pexec->SetRenderState(D3DRENDERSTATE_BLENDENABLE, true);
            }
            //m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR);
            //m_pexec->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR);

            m_pexec->StartStrip(points.GetCount() - 2);
            for(index = 1; index < points.GetCount() - 1; index++) {
                m_pexec->AddTriangle(0, index, index + 1);
            }

            DDCall(m_pd3dd->BeginScene());
            m_pexec->Execute();
            DDCall(m_pd3dd->EndScene());
        }

        return S_OK;
    }
    */
};

//////////////////////////////////////////////////////////////////////////////
//
// Surface
//
//
// Surface
//
//
// Surface
//
//
// Surface
//
//////////////////////////////////////////////////////////////////////////////

class SurfaceImpl : public Surface {
private:
    WinRect m_rect;
    WinRect m_rectSurface;
    bool m_bOpaque;

    TRef<Engine>             m_pengine;
	TRef<Context3D>          m_pcontext;

    TRef<IDirectDraw>        m_pdd;
    TRef<IDirectDrawSurface> m_pdds;
    TRef<IDirectDrawSurface> m_pddsZBuffer;
    TRef<IDirectDrawClipper> m_pddClipper;
    DDSDescription           m_ddsd;
    DDSDescription           m_ddsdZBuffer;

    // 2D drawing attributes

    ZColor m_color;

public:
    SurfaceImpl(Engine* pengine, const WinRect& rect) :
        m_pengine(pengine),
        m_pdd(pengine->GetDD()),
        m_rect(rect),
        m_rectSurface(rect),
        m_bOpaque(false),
        m_color(1, 1, 1)
    {
        CalculateRect(m_rectSurface, false);
        CreateSurface();
    }

    void CreateSurface()
    {
        //
        // Create the surface
        //

        m_ddsd.dwFlags =
              DDSD_HEIGHT
            | DDSD_WIDTH
            | DDSD_CAPS;
        m_ddsd.dwWidth  = m_rectSurface.XSize();
        m_ddsd.dwHeight = m_rectSurface.YSize();
        m_ddsd.ddsCaps.dwCaps =
              DDSCAPS_3DDEVICE
            | DDSCAPS_SYSTEMMEMORY;

        DDCall(m_pdd->CreateSurface(&m_ddsd, &m_pdds, NULL));
        DDCall(m_pdds->GetSurfaceDesc(&m_ddsd));

        //
        // Create a ZBuffer
        //

        m_ddsdZBuffer.dwFlags =
              DDSD_HEIGHT
            | DDSD_WIDTH
            | DDSD_ZBUFFERBITDEPTH
            | DDSD_CAPS;
        m_ddsdZBuffer.dwWidth  = m_rectSurface.XSize();
        m_ddsdZBuffer.dwHeight = m_rectSurface.YSize();
        m_ddsdZBuffer.dwZBufferBitDepth = 16;
        m_ddsdZBuffer.ddsCaps.dwCaps =
              DDSCAPS_ZBUFFER
            | DDSCAPS_SYSTEMMEMORY;

        DDCall(m_pdd->CreateSurface(&m_ddsd, &m_pddsZBuffer, NULL));
        DDCall(m_pddsZBuffer->GetSurfaceDesc(&m_ddsdZBuffer));

        DDCall(m_pdds->AddAttachedSurface(m_pddsZBuffer));

        //
        // Set a Color Key
        //

        DDCOLORKEY key;
        key.dwColorSpaceLowValue  = GetColorKey();
        key.dwColorSpaceHighValue = GetColorKey();
        DDCall(m_pdds->SetColorKey(DDCKEY_SRCBLT | DDCKEY_COLORSPACE, &key));

        //
        // clip the surface it to m_rect
        //

        RGNDATA rd[2];

        rd[0].rdh.dwSize = sizeof(RGNDATAHEADER);
        rd[0].rdh.iType = RDH_RECTANGLES;
        rd[0].rdh.nCount = 1;
        rd[0].rdh.nRgnSize = 0;

        rd[0].rdh.rcBound = m_rectSurface;

        *(Rect*)(rd[0].Buffer) = m_rectSurface;

        DDCall(m_pdd->CreateClipper(0, &m_pddClipper, NULL));
        DDCall(m_pddClipper->SetClipList(rd, 0));
        DDCall(m_pdds->SetClipper(m_pddClipper));

		//
		// Create the 3D Context
		//

		m_pcontext = m_pengine->CreateContext3D(this);
    }

    Context3D* GetContext3D()
    {
        return m_pcontext;
    }

    void CalculateRect(WinRect& rect, bool bTexture)
    {
        if (bTexture) {
            rect.SetXSize(PowerOf2(rect.XSize()));
            rect.SetYSize(PowerOf2(rect.YSize()));
        } else {
            if (rect.XSize() < 1) rect.SetXSize(1);
            if (rect.YSize() < 1) rect.SetYSize(1);
        }
    }

    LPDIRECTDRAW GetDD() { return m_pdd; }
    LPDIRECTDRAWSURFACE GetDDS() { return m_pdds; }
    Engine* GetEngine() { return m_pengine; }
    const DDSDescription& GetDescription() { return m_ddsd; }
    const WinRect& GetRect() { return m_rect; }
    const WinRect& GetSurfaceRect() { return m_rectSurface; }
    DWORD GetColorKey() { return m_ddsd.PixelFormat().Color(2, 1, 3); }

    HDC GetDC()
    {
        HDC hdc;
        DDCall(m_pdds->GetDC(&hdc));
        return hdc;
    }

    HRESULT ReleaseDC(HDC hdc)
    {
        DDCall(m_pdds->ReleaseDC(hdc));
        return S_OK;
    }

    void* GetPointer()
    {
        DDCall(m_pdds->Lock(
            NULL,
            &m_ddsd,
            DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
            NULL
        ));

        return m_ddsd.lpSurface;
    }

    HRESULT ReleasePointer(void *pointer)
    {
        DDCall(m_pdds->Unlock(pointer));
        return S_OK;
    }

    //
    // attributes
    //

    //
    // GDI Calls
    //

    HRESULT TextOut(const Point& point, const ZString& str)
    {
        HDC hdc = GetDC();
        ::TextOut(hdc, (int)point.X(), (int)point.Y(), str, str.Length());
        ReleaseDC(hdc);
        return S_OK;
    }

    Point GetTextExtent(const ZString& str)
    {
        HDC hdc = GetDC();

        SIZE size;

        ::GetTextExtentPoint(hdc, str, str.Length(), &size);

        ReleaseDC(hdc);

        return Point((float)size.cx, (float)size.cy);
    }

    //
    // 2d drawing
    //

    HRESULT SetColor(const ZColor& color)
    {
        m_color = color;
        return S_OK;
    }

    HRESULT FillSurfaceWithColorKey()
    {
        DDBLTFX ddbltfx;

        ddbltfx.dwSize = sizeof(ddbltfx);
        ddbltfx.dwFillColor = GetColorKey();

        DDCall(m_pdds->Blt(
            &m_rectSurface, NULL,
            &m_rectSurface, DDBLT_COLORFILL, &ddbltfx
        ));

        m_bOpaque = false;
        return S_OK;
    }

    HRESULT BitBlt(Rect& rectTarget, Surface* pddsSource, Rect& rectSource)
    {
        ZError("");
        // !!! stretchblt
        return S_OK;
    }

    HRESULT BitBlt(Rect& rectTarget, Surface* psurfaceSource, Point& pointSource)
    {
        WinRect rectSource(pointSource, pointSource + rectTarget.Size());

        DDBLTFX ddbltfx;

        ddbltfx.dwSize = sizeof(ddbltfx);
        ddbltfx.dwROP = SRCCOPY;

        DDCall(m_pdds->Blt(
            &WinRect(rectTarget),
            psurfaceSource->GetDDS(),
            &rectSource,
            DDBLT_WAIT | DDBLT_ROP,
            &ddbltfx
        ));
        return S_OK;
    }

    HRESULT FillRect(const Rect& rect, DWORD color)
    {
        if (rect.XSize() > 0 && rect.YSize() > 0) {
            DDBLTFX ddbltfx;

            ddbltfx.dwSize = sizeof(ddbltfx);
            ddbltfx.dwFillColor = color;

            WinRect rc = rect;
            DDCall(m_pdds->Blt(&rc, NULL, &rc, DDBLT_COLORFILL, &ddbltfx));
        }
        return S_OK;
    }

    HRESULT FillRect(const Rect& rect, const ZColor& color)
    {
        FillRect(rect, m_ddsd.PixelFormat().Color(color));
        return S_OK;
    }

    HRESULT FillSurface(const ZColor& color)
    {
		// !!! NT5 beta1 dx5 crashes unless the rectangle is reduced by one.

        WinRect rc = m_rect;
		rc.SetYSize(rc.YSize() - 1);

        FillRect(rc, m_ddsd.PixelFormat().Color(color));
        return S_OK;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Graphics Engine
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geometry> ImportXFile(IDirect3DRM* pd3drm, const RelativePathString& str);

class EngineImpl : public Engine {
    TRef<IDirectDraw>        m_pdd;
    TRef<IDirectDrawSurface> m_pddsPrimary;
    DDSURFACEDESC            m_ddsdPrimary;

    TRef<IDirect3DRM>        m_pd3drm;

    TRef<Context3D>          m_pcontext;
    TRef<IDirect3D>          m_pd3d;
    IID m_iidD3DHW;
    IID m_iidD3DSW;
    IID m_iidD3D;

    D3DDeviceDescription     m_d3ddescHW;
    D3DDeviceDescription     m_d3ddescSW;
    D3DDeviceDescription*    m_pd3ddesc;

    Surface* m_psurfaceList;

    //
    // static functions
    //

    static BOOL PASCAL StaticDDDeviceCallback(
        GUID FAR* lpGuid,
        LPSTR     lpDriverDesc,
        LPSTR     lpDriverName,
        LPVOID    lpvoid
    ) {
        EngineImpl* pthis = (EngineImpl*)lpvoid;

        return pthis->DDDeviceCallback(lpGuid, lpDriverDesc, lpDriverName);
    }

    static HRESULT PASCAL StaticD3DDeviceCallback(
        LPGUID          lpGuid,
        LPSTR           lpDeviceDescription,
        LPSTR           lpDeviceName,
        LPD3DDEVICEDESC pd3ddescHW,
        LPD3DDEVICEDESC pd3ddescSW,
        LPVOID          lpvoid
    ) {
        EngineImpl* pthis = (EngineImpl*)lpvoid;

        return
            pthis->D3DDeviceCallback(
                lpGuid,
                lpDeviceDescription,
                lpDeviceName,
                pd3ddescHW,
                pd3ddescSW
            );
    }

public:
    EngineImpl() :
        m_psurfaceList(NULL)
    {
        //
        // get the direct draw object
        //

        // DDCall(DirectDrawEnumerate(StaticDDDeviceCallback, this));

        DDCall(DirectDrawCreate(NULL, &m_pdd, NULL));
        DDCall(m_pdd->SetCooperativeLevel(NULL, DDSCL_NORMAL));

        //
        // create the d3d object and device objects
        //

        DDCall(m_pdd->QueryInterface(IID_IDirect3D, (void**)&m_pd3d));
        DDCall(m_pd3d->EnumDevices(StaticD3DDeviceCallback, this));

        m_iidD3D = m_iidD3DSW;

        //
        // create a d3drm device for loading xfiles
        //

        DDCall(Direct3DRMCreate(&m_pd3drm));

        //
        // Get the primary surface
        //

        /*
        m_ddsd.dwFlags = DDSD_CAPS;
        m_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        DDCall(m_pdd->CreateSurface(&m_ddsd, &m_pddsPrimary, NULL));
        DDCall(m_pdds->GetSurfaceDesc(&m_ddsd));
        */
    }

    //
    //
    //

    BOOL DDDeviceCallback(
        GUID FAR* lpGuid,
        LPSTR     lpDriverDesc,
        LPSTR     lpDriverName
    ) {	/*
        // Use the guid to create the corresponding DD device.

        TRef<IDirectDraw> pdd;

        DDCall(DirectDrawCreate(lpGuid, &dd, NULL));

        //
        // Get device capabilities.
        //

        DDCaps ddcapsHW;
        DDCaps ddcapsSW;
        DDCall(m_pdd->GetCaps(&ddcapsHW, &ddcapsSW));

        if (ddcapsHW.dwCaps & DDCAPS_3D) {
            //
            // Found a DD driver with 3D support.
            //

            DDCall(pdd->QueryInterface(IID_IDirectDraw2, (void**)&m_pdd));

            if (SUCCEEDED(rc)) {
                //Found a 3D accelerated driver and got the
                //DirectDraw2 interface handle (which is saved in
                //the game display)

                //On a pass through card?
                gd->m_passThroughF = (lpGuid != NULL) &&
                                     (!(driverCaps.dwCaps & DDCAPS_GDI));

                //end the enumeration in any case.
                return DDENUMRET_CANCEL;
            }
        }
		*/
        return DDENUMRET_OK;
    }

    //
    //
    //

    HRESULT D3DDeviceCallback(
        LPGUID lpGuid,
        LPSTR lpDeviceDescription,
        LPSTR lpDeviceName,
        LPD3DDEVICEDESC pd3ddescHW,
        LPD3DDEVICEDESC pd3ddescSW
    ) {
        if (pd3ddescHW->dcmColorModel) {
            //
            // hardware device
            //

            m_iidD3DHW = *lpGuid;
        } else {
            //
            // software device
            //

            if (pd3ddescSW->dcmColorModel == D3DCOLOR_RGB) {
                //
                // use an RGB color model
                //

                m_iidD3DSW = *lpGuid;
                return D3DENUMRET_CANCEL;
            }
        }

        return D3DENUMRET_OK;
    }

    TRef<Context3D> CreateContext3D(Surface* psurface)
    {
        return new D3DIMContext3D(m_pd3d, m_iidD3D, psurface);
    }

    LPDIRECTDRAW GetDD() { return m_pdd; }

    virtual TRef<Geometry> ImportXFile(const RelativePathString& str)
    {
        return ::ImportXFile(m_pd3drm, str);
    }

    TRef<Surface> CreateImportSurface(const ZString& szname)
    {
        // !!! if this is a system memory surface no need to keep the bitmap in memory

        HBITMAP hbm =
            (HBITMAP)LoadImage(
                NULL,
                szname,
                IMAGE_BITMAP,
                0,
                0,
                LR_LOADFROMFILE
            );

        if (!hbm) {
            return NULL;
        }

        BITMAP bm;
        GetObject(hbm, sizeof(bm), &bm);
        WinRect rect(0, 0, bm.bmWidth, bm.bmHeight);

        //
        // Create a surface that is the same size as the bitmap
        //

        TRef<Surface> psurface = new SurfaceImpl(this, rect);

        //
        // blt the bitmap to the surface
        //

        HDC hdc = psurface->GetDC();
        HDC hdcSource = ::CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)::SelectObject(hdcSource, hbm);

        ::BitBlt(hdc, 0, 0, rect.XSize(), rect.YSize(), hdcSource, 0, 0, SRCCOPY);

        ::SelectObject(hdcSource, hbmOld);
        ::DeleteDC(hdcSource);

        psurface->ReleaseDC(hdc);

        //
        // return the surface
        //

        return psurface;
    }

    TRef<Surface> CreateSurface(const WinRect& rect)
    {
        return new SurfaceImpl(this, rect);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Engine> Engine::Create()
{
    return new EngineImpl();
}
