#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class D3DRasterizerImpl : public D3DRasterizer {
private:
    TRef<DDDevice>           m_pdddevice;
    TRef<D3DDevice>          m_pd3ddevice;
    TRef<IDirect3DDeviceX>   m_pd3dd;
    TRef<IDirect3DViewportX> m_pd3dview;

    PrivateSurface*          m_psurface;
    DDSurface*               m_pddsurface;

    DWORD                    m_dwDrawPrimitiveFlags;
    Rect                     m_rectGuard;
    Rect                     m_rectClip;

    #ifdef _DEBUG
        bool m_bSurfaceLost;

        void InitializeD3DCall()
        {
            m_bSurfaceLost = false;
        }

        void D3DError(HRESULT hr, const char* pszCall, const char* pszFile, int line, const char* pszModule)
        {
            if (m_bSurfaceLost) {
                // ignore all errors if the surface is lost
            } else {
                if (
                       hr == DDERR_SURFACELOST
                    || hr == D3DERR_SCENE_BEGIN_FAILED
                ) {
                    m_bSurfaceLost = true;
                } else {
                    DDError(hr, pszCall, pszFile, line, pszModule);
                }
            } 
        }

        #define D3DCall(hr) D3DError(hr, #hr, __FILE__, __LINE__, __MODULE__)
    #else
        void InitializeD3DCall() {}
        void D3DCall(HRESULT hr) {}
    #endif

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    D3DRasterizerImpl(PrivateSurface* psurface) :
        m_psurface(psurface),
        m_dwDrawPrimitiveFlags(
              D3DDP_DONOTUPDATEEXTENTS
            | D3DDP_DONOTCLIP
        )
    {
        CastTo(m_pddsurface, psurface->GetVideoSurface());
        InitializeD3DCall();

        //
        // Tell the device about this rasterizer
        //

        m_pdddevice = m_pddsurface->GetDDDevice();
        m_pdddevice->AddRasterizer(this);

        //
        // Create a D3D device
        //

        m_pd3ddevice = m_pdddevice->CreateD3DDevice(m_pddsurface);
        if (m_pd3ddevice == NULL) {
            return;
        }

        m_pd3dd      = m_pd3ddevice->GetD3DDeviceX();

        D3DDeviceDescription d3dddSW;
        D3DDeviceDescription d3dddHW;

        D3DCall(m_pd3dd->GetCaps(&d3dddHW, &d3dddSW));

        #ifndef DREAMCAST
            m_rectGuard =
                Rect(
                    d3dddHW.dvGuardBandLeft,
                    d3dddHW.dvGuardBandTop,
                    d3dddHW.dvGuardBandRight,
                    d3dddHW.dvGuardBandBottom
                );
        #endif

        //
        // Create the D3D viewport
        //

        D3DCall(m_pdddevice->GetD3D()->CreateViewport(&m_pd3dview, NULL));
        D3DCall(m_pd3dd->AddViewport(m_pd3dview));
        UpdateViewport();

        //
        // default rendering states
        //

        D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL));
    }

    bool IsValid()
    {
        return m_pd3ddevice != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~D3DRasterizerImpl()
    {
        m_pdddevice->RemoveRasterizer(this);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
        m_pd3dview = NULL;
        m_pd3dd    = NULL;
    }

    TRef<IDirect3DDeviceX> GetD3DDeviceX()
    {
        return m_pd3dd;
    }

    TRef<IDirect3DX> GetD3D()
    {
        return m_pdddevice->GetD3D();
    }

    TRef<IDirect3DViewportX> GetViewport()
    {
        return m_pd3dview;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Change Render target
    //
    //////////////////////////////////////////////////////////////////////////////

    /*  , make this work
    void SetSurface(ISurfaceImpl* psurface)
    {
        ZAssert(!m_bRendering);
        m_psurface = psurface;
        D3DCall(m_pd3dd->SetRenderTarget(m_psurface->GetDDS(), 0));
    }
    */

    //////////////////////////////////////////////////////////////////////////////
    //
    // Scene
    //
    //////////////////////////////////////////////////////////////////////////////

    void BeginScene()
    {
        m_pddsurface->BeginScene();
        m_pdddevice->BeginScene();
        D3DCall(m_pd3dd->BeginScene());
    }

    void EndScene()
    {
        m_pddsurface->EndScene();

        //
        //   11/16/99 not checking return codes here since the Permedia 2
        //     driver on Win2K returns error every once in a while
        //

        m_pd3dd->EndScene();

        m_pdddevice->EndScene();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Viewport
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateViewport()
    {
        const WinPoint& size = m_psurface->GetSize();
        float aspect = (float)size.X() / (float)size.Y();

        D3DVIEWPORT2 view;

        view.dwSize   = sizeof(D3DVIEWPORT2);
        view.dwX      = 0;
        view.dwY      = 0;
        view.dwWidth  = (int)size.X();
        view.dwHeight = (int)size.Y();

        view.dvClipX      = -1;
        view.dvClipWidth  = 2;

        view.dvClipY      =     aspect;
        view.dvClipHeight = 2 * aspect;

        view.dvMinZ       = 0;
        view.dvMaxZ       = 1;

        D3DCall(m_pd3dview->SetViewport2(&view));
        D3DCall(m_pd3dd->SetCurrentViewport(m_pd3dview));
    }

    void SetClipRect(const Rect& rectClip)
    {
        m_rectClip = rectClip;
    }

    void ClearZBuffer()
    {
        ZTrace(
              "Clearing ZBuffer (" 
            + ZString(m_rectClip.XMin()) + ", "
            + ZString(m_rectClip.YMin()) + ", "
            + ZString(m_rectClip.XMax()) + ", "
            + ZString(m_rectClip.YMax()) + ")"
        );

        D3DRect rect(
            (int)m_rectClip.XMin(),
            (int)m_rectClip.YMin(),
            (int)m_rectClip.XMax(),
            (int)m_rectClip.YMax()
        );

        HRESULT hr = m_pd3dview->Clear(1, &rect, D3DCLEAR_ZBUFFER);

        //
        // This is actually a DDraw call so it's possible it can fail.
        //

        if (hr != DDERR_UNSUPPORTED) {
            D3DCall(hr);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    bool HasZBuffer()
    {
        return m_pddsurface->HasZBuffer();
    }

    bool Has3DAcceleration()
    {
        return m_pdddevice->GetAllow3DAcceleration();
    }

    Point GetSurfaceSize()
    {
        return Point::Cast(m_pddsurface->GetSize());
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // State
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetTexture(Surface* psurfaceTextureArg)
    {
        PrivateSurface* psurfaceTexture; CastTo(psurfaceTexture, psurfaceTextureArg);

        if (psurfaceTexture != NULL) {
            DDSurface* pddsurfaceTexture; CastTo(pddsurfaceTexture, psurfaceTexture->GetVideoSurface());

            D3DCall(m_pd3dd->SetTexture(
                0, 
                m_pdddevice->GetTextureX(m_pd3ddevice, pddsurfaceTexture)
            ));
        } else {
            D3DCall(m_pd3dd->SetTexture(0, NULL));
        }
    }

    void SetShadeMode(ShadeMode shadeMode)
    {
        switch (shadeMode) {
            case ShadeModeNone:
            case ShadeModeCopy:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_DECAL));
                break;

            case ShadeModeFlat:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE));
                break;

            case ShadeModeGlobalColor:
            case ShadeModeGouraud:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE));
                break;

            default:
                ZError("Invalid ShadeMode");
        }
    }

    void SetBlendMode(BlendMode blendMode)
    {
        switch (blendMode) {
            case BlendModeSource:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false));
                break;

            case BlendModeAdd:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE));
                break;

            case BlendModeSourceAlpha:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA));
                break;

            default:
                ZError("Invalid BlendMode");
        }
    }

    void SetWrapMode(WrapMode wrapMode)
    {
        switch (wrapMode) {
            case WrapModeNone:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPU, false));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPV, false));
                break;

            case WrapModeUCylinder:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPU, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPV, false));
                break;

            case WrapModeVCylinder:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPU, false));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPV, true));
                break;

            case WrapModeTorus:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPU, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_WRAPV, true));
                break;

            default:
                ZError("Invalid WrapMode");
        }
    }

    void SetCullMode(CullMode cullMode)
    {
        switch (cullMode) {
            case CullModeNone:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE));
                break;

            case CullModeCW:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW));
                break;

            case CullModeCCW:
                D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW));
                break;

            case CullModeBoth:
                ZUnimplemented();
                break;

            default:
                ZError("Invalid CullMode");
        }
    }

    void SetZTest(bool bZTest)
    {
        D3DCall(m_pd3dd->SetRenderState(
             D3DRENDERSTATE_ZENABLE,
             HasZBuffer() && bZTest
         ));
    }

    void SetZWrite(bool bZWrite)
    {
        D3DCall(m_pd3dd->SetRenderState(
            D3DRENDERSTATE_ZWRITEENABLE,
            HasZBuffer() && bZWrite
        ));
    }

    void SetLinearFilter(bool bLinearFilter)
    {
        if (bLinearFilter && Has3DAcceleration()) {
            D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR));
            D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR));
        } else {
            D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_NEAREST));
            D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_NEAREST));
        }
    }

    void SetPerspectiveCorrection(bool bPerspectiveCorrection)
    {
        D3DCall(m_pd3dd->SetRenderState(
            D3DRENDERSTATE_TEXTUREPERSPECTIVE,
            bPerspectiveCorrection || Has3DAcceleration()
        ));
    }

    void SetDither(bool bDither)
    {
        D3DCall(m_pd3dd->SetRenderState(
            D3DRENDERSTATE_DITHERENABLE,
            false //bDither || Has3DAcceleration()
        ));
    }

    void SetColorKey(bool bColorKey)
    {
        D3DCall(m_pd3dd->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, bColorKey));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Debug
    //
    //////////////////////////////////////////////////////////////////////////////

    void CheckVertices(const VertexScreen* pvertex, int vcount)
    {
        /*
        #ifdef _DEBUG
        {
            const float error = 0.5f;

            for (int index = 0; index < vcount; index++) {
                ZAssert(pvertex[index].x >= m_rectClip.XMin() - error);
                ZAssert(pvertex[index].x <= m_rectClip.XMax() + error);
                ZAssert(pvertex[index].y >= m_rectClip.YMin() - error);
                ZAssert(pvertex[index].y <= m_rectClip.YMax() + error);
                ZAssert(pvertex[index].z >= 0.0f - error);
                ZAssert(pvertex[index].z <= 1.0f + error);
            }
        }
        #endif
        */

        VertexScreen* pcheck = (VertexScreen*)pvertex;

        for (int index = 0; index < vcount; index++) {
            if (pcheck[index].x < m_rectClip.XMin()) {
                pcheck[index].x = m_rectClip.XMin();
            } else if (pcheck[index].x > m_rectClip.XMax()) {
                pcheck[index].x = m_rectClip.XMax();
            }

            if (pcheck[index].y < m_rectClip.YMin()) {
                pcheck[index].y = m_rectClip.YMin();
            } else if (pcheck[index].y > m_rectClip.YMax()) {
                pcheck[index].y = m_rectClip.YMax();
            }

            if (pcheck[index].z < 0) {
                pcheck[index].z = 0;
            } else if (pcheck[index].z > 1) {
                pcheck[index].z = 1;
            }
        }
    }

    void CheckIndices(const MeshIndex* pindex, int icount, int vcount)
    {
        #ifdef _DEBUG
            for (int index = 0; index < icount; index++) {
                ZAssert(pindex[index] >= 0 && pindex[index] < vcount);
            }
        #endif
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Rendering
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTrianglesD3D(const D3DLVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawLinesD3D(const D3DLVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawPointsD3D(const D3DLVertex* pvertex, int vcount)
    {
        D3DCall(m_pd3dd->DrawPrimitive(
            D3DPT_POINTLIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawTrianglesD3D(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawLinesD3D(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawPointsD3D(const D3DVertex* pvertex, int vcount)
    {
        D3DCall(m_pd3dd->DrawPrimitive(
            D3DPT_POINTLIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            m_dwDrawPrimitiveFlags
        ));
    }


    void DrawTriangles(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);

        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_TLVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawLines(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);

        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_TLVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));
    }

    void DrawPoints(const VertexScreen* pvertex, int vcount)
    {
        if (true) {
            PixelFormat* ppf   = m_psurface->GetPixelFormat();
            int          pitch = m_psurface->GetPitch();
            BYTE*        pdata = m_psurface->GetWritablePointer();

            ZAssert(ppf->PixelBytes() == 2   );
            ZAssert(ppf->RedSize()    == 0x1f);
            ZAssert(ppf->BlueSize()   == 0x1f);

            if (ppf->GreenSize() == 0x1f) {
                // 555

                for (int index = 0; index < vcount; index++) {
                    BYTE* ppixel = 
                          pdata 
                        + FloorInt(pvertex[index].x) * 2
                        + FloorInt(pvertex[index].y) * pitch;
                    DWORD color = pvertex[index].color;
                    WORD  
                    pixel =
                        (WORD)( 
                              ((color >> 9) & 0x7c00)
                            | ((color >> 6) & 0x03e0)
                            | ((color >> 3) & 0x001f)
                        );

                    *(WORD*)(ppixel) = pixel;
                }
            } else {
                // 565

                ZAssert(ppf->GreenSize() == 0x3f);

                for (int index = 0; index < vcount; index++) {
                    BYTE* ppixel = 
                          pdata 
                        + FloorInt(pvertex[index].x) * 2
                        + FloorInt(pvertex[index].y) * pitch;
                    DWORD color = pvertex[index].color;
                    WORD  pixel =
                        (WORD)( 
                              ((color >> 8) & 0xf800)
                            | ((color >> 5) & 0x07e0)
                            | ((color >> 3) & 0x001f)
                        );

                    *(WORD*)(ppixel) = pixel;
                }
            }

            m_psurface->ReleasePointer();
        } else {
            CheckVertices(pvertex, vcount);

            D3DCall(m_pd3dd->DrawPrimitive(
                D3DPT_POINTLIST,
                D3DFVF_TLVERTEX,
                (void*)pvertex,
                vcount,
                m_dwDrawPrimitiveFlags
            ));
        }
    }
};

TRef<D3DRasterizer> CreateD3DRasterizer(PrivateSurface* psurface)
{
    TRef<D3DRasterizer> prasterizer = new D3DRasterizerImpl(psurface);

    return prasterizer->IsValid() ? prasterizer : NULL;
}
