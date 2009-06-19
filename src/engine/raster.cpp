#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class D3DRasterizerImpl : public D3DRasterizer {
private:
//    TRef<DDDevice>			m_pdddevice;
    TRef<D3DDevice>			m_pd3ddevice;
    TRef<IDirect3DDeviceX>	m_pd3dd;
    D3DVIEWPORT9			m_Viewport;
	HWND					m_hParentWindow;
	Point					m_Size;
    PrivateSurface*          m_psurface;

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
            //        || hr == D3DERR_SCENE_BEGIN_FAILED
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

    D3DRasterizerImpl( PrivateSurface* psurface, HWND hParentWindow ) :
								m_psurface(psurface),
								m_dwDrawPrimitiveFlags( 0 ),
								m_hParentWindow( hParentWindow ) //D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTCLIP
    {
//        CastTo(m_pddsurface, psurface->GetVideoSurface());
        InitializeD3DCall();

		m_pd3dd = CD3DDevice9::Device();

        // Tell the device about this rasterizer
//		m_pdddevice = ((DDSurface* )psurface->GetVideoSurface())->GetDDDevice();
//		m_pdddevice->AddRasterizer(this);
//		m_pd3ddevice = m_pdddevice->GetD3DDevice();;

/*        // Create a D3D device
        m_pd3ddevice = m_pdddevice->CreateD3DDevice( m_hParentWindow );
        if (m_pd3ddevice == NULL) 
		{
            return;
        }*/

  //      m_pd3dd = m_pd3ddevice->GetD3DDeviceX();
		m_Size.SetX( psurface->GetSize().X() );
		m_Size.SetY( psurface->GetSize().Y() );

        //D3DDeviceDescription d3dddSW;
   //     D3DDeviceDescription d3dddHW;

     //   D3DCall(m_pd3dd->GetDeviceCaps( &d3dddHW ) );

   /*     m_rectGuard =
            Rect(
                d3dddHW.GuardBandLeft,
                d3dddHW.GuardBandTop,
                d3dddHW.GuardBandRight,
                d3dddHW.GuardBandBottom
            );*/

        // Create the D3D viewport
		memset( &m_Viewport, 0, sizeof( D3DVIEWPORT9 ) );
//        D3DCall(m_pdddevice->GetD3D()->CreateViewport(&m_pd3dview, NULL));
//        D3DCall(m_pd3dd->AddViewport(m_pd3dview));
        UpdateViewport();

        //
        // default rendering states
        //

        D3DCall(m_pd3dd->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL));
    }

    bool IsValid()
    {
		return m_pd3dd != NULL;
//        return m_pd3ddevice != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~D3DRasterizerImpl()
    {
//        m_pdddevice->RemoveRasterizer(this);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
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

    D3DVIEWPORT9 * GetViewport()
    {
        return &m_Viewport;
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
  //      m_pddsurface->BeginScene();
        m_pdddevice->BeginScene();
        D3DCall(m_pd3dd->BeginScene());
    }

    void EndScene()
    {
//        m_pddsurface->EndScene();

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

		m_Viewport.X		= 0;
		m_Viewport.Y		= 0;
		m_Viewport.Width	= (int) size.X();
		m_Viewport.Height	= (int) size.Y();
		m_Viewport.MinZ		= 0.0f;
		m_Viewport.MaxZ		= 1.0f;

		m_pd3dd->SetViewport( &m_Viewport );
    }

    void SetClipRect(const Rect& rectClip)
    {
        m_rectClip = rectClip;
    }

    void ClearZBuffer()
    {
 /*       ZTrace(
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

		HRESULT hr = m_pd3ddevice->GetD3DDeviceX()->Clear( 1, &rect, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

		// This is actually a DDraw call so it's possible it can fail.
        if( hr != D3D_OK ) 
		{
            D3DCall(hr);
        }*/
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    bool HasZBuffer()
    {
        return true;
    }

    Point GetSurfaceSize()
    {
        return m_Size;
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
        switch (shadeMode) 
		{
            case ShadeModeNone:
            case ShadeModeCopy:
				// Outdated call to SetRenderState.
//                D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMAPBLEND, D3DTBLEND_DECAL));
                break;

            case ShadeModeFlat:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT));

				// Outdated call to SetRenderState.
//                D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMAPBLEND, D3DTBLEND_MODULATE));
                break;

            case ShadeModeGlobalColor:
            case ShadeModeGouraud:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD));

				// Outdated call to SetRenderState.
//                D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMAPBLEND, D3DTBLEND_MODULATE));
                break;

            default:
                ZError("Invalid ShadeMode");
        }
    }

    void SetBlendMode(BlendMode blendMode)
    {
        switch (blendMode) {
            case BlendModeSource:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, false));
                break;

            case BlendModeAdd:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE));
                D3DCall(m_pd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
                break;

            case BlendModeSourceAlpha:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, true));
                D3DCall(m_pd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE));
                D3DCall(m_pd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));
                break;

            default:
                ZError("Invalid BlendMode");
        }
    }

    void SetWrapMode(WrapMode wrapMode)
    {
        switch (wrapMode) {
            case WrapModeNone:
				D3DCall(m_pd3dd->SetRenderState( D3DRS_WRAP0, 0 ) );
                break;

            case WrapModeUCylinder:
				D3DCall(m_pd3dd->SetRenderState( D3DRS_WRAP0, D3DWRAPCOORD_0 ) );
                break;

            case WrapModeVCylinder:
				D3DCall(m_pd3dd->SetRenderState( D3DRS_WRAP0, D3DWRAPCOORD_1 ) );
                break;

            case WrapModeTorus:
				D3DCall(m_pd3dd->SetRenderState( D3DRS_WRAP0, D3DWRAPCOORD_0 | D3DWRAPCOORD_1 ) );
                break;

            default:
                ZError("Invalid WrapMode");
        }
    }

    void SetCullMode(CullMode cullMode)
    {
        switch (cullMode) {
            case CullModeNone:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
                break;

            case CullModeCW:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW));
                break;

            case CullModeCCW:
                D3DCall(m_pd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW));
                break;

            case CullModeBoth:
                ZUnimplemented();
                break;

            default:
                ZError("Invalid CullMode");
        }
    }

	bool Has3DAcceleration( )
	{
		return true;
	}

    void SetZTest(bool bZTest)
    {
        D3DCall(m_pd3dd->SetRenderState(
             D3DRS_ZENABLE,
             HasZBuffer() && bZTest
         ));
    }

    void SetZWrite(bool bZWrite)
    {
        D3DCall(m_pd3dd->SetRenderState(
            D3DRS_ZWRITEENABLE,
            HasZBuffer() && bZWrite
        ));
    }

    void SetLinearFilter(bool bLinearFilter)
    {
        if( bLinearFilter ) 
		{
//            D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMAG, D3DFILTER_LINEAR));
//            D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMIN, D3DFILTER_LINEAR));
			D3DCall( m_pd3dd->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ) );
			D3DCall( m_pd3dd->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
        } 
		else 
		{
//            D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMAG, D3DFILTER_NEAREST));
  //          D3DCall(m_pd3dd->SetRenderState(D3DRS_TEXTUREMIN, D3DFILTER_NEAREST));
			D3DCall( m_pd3dd->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT ) );
			D3DCall( m_pd3dd->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT ) );
        }
    }

    void SetPerspectiveCorrection(bool bPerspectiveCorrection)
    {
		// Now automagically supplied by D3D - ie, all hardware must support it.
//        D3DCall(m_pd3dd->SetRenderState(
  //          D3DRS_TEXTUREPERSPECTIVE,
    //        bPerspectiveCorrection
      //  ));
    }

    void SetDither(bool bDither)
    {
        D3DCall(m_pd3dd->SetRenderState(
            D3DRS_DITHERENABLE,
            true
        ));
    }

    void SetColorKey(bool bColorKey)
	{
		// No longer supported - I think it's configured as part of a texture now.
//        D3DCall(m_pd3dd->SetRenderState(D3DRS_COLORKEYENABLE, bColorKey));
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
			// KGJV 32B: general Texels to Pixels correction
			pcheck[index].x -= 0.5f;
			pcheck[index].y -= 0.5f;
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
		D3DCall( m_pd3dd->SetFVF( D3DFVF_LVERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST,
													0,
													vcount,
													icount / 3,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( D3DLVertex ) ) );

/*        D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawLinesD3D(const D3DLVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
 		D3DCall( m_pd3dd->SetFVF( D3DFVF_LVERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_LINELIST,
													0,
													vcount,
													icount / 2,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( D3DLVertex ) ) );
/*		D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawPointsD3D(const D3DLVertex* pvertex, int vcount)
    {
 		D3DCall( m_pd3dd->SetFVF( D3DFVF_LVERTEX ) );
		D3DCall( m_pd3dd->DrawPrimitiveUP(	D3DPT_POINTLIST,
											vcount,
											pvertex,
											sizeof( D3DLVertex ) ) );
/*        D3DCall(m_pd3dd->DrawPrimitive(
            D3DPT_POINTLIST,
            D3DFVF_LVERTEX,
            (void*)pvertex,
            vcount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawTrianglesD3D(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
  		D3DCall( m_pd3dd->SetFVF( D3DFVF_VERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST,
													0,
													vcount,
													icount / 3,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( D3DVertex ) ) );
/*		D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawLinesD3D(const D3DVertex* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
 		D3DCall( m_pd3dd->SetFVF( D3DFVF_VERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_LINELIST,
													0,
													vcount,
													icount / 2,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( D3DVertex ) ) );
/*		D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawPointsD3D(const D3DVertex* pvertex, int vcount)
    {
 		D3DCall( m_pd3dd->SetFVF( D3DFVF_VERTEX ) );
		D3DCall( m_pd3dd->DrawPrimitiveUP(	D3DPT_POINTLIST,
											vcount,
											pvertex,
											sizeof( D3DVertex ) ) );
/*		D3DCall(m_pd3dd->DrawPrimitive(
            D3DPT_POINTLIST,
            D3DFVF_VERTEX,
            (void*)pvertex,
            vcount,
            m_dwDrawPrimitiveFlags
        ));*/
    }


    void DrawTriangles(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);

  		D3DCall( m_pd3dd->SetFVF( D3DFVF_TLVERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST,
													0,
													vcount,
													icount / 3,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( VertexScreen ) ) );
/*		D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST,
            D3DFVF_TLVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawLines(const VertexScreen* pvertex, int vcount, const MeshIndex* pindex, int icount)
    {
        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);

 		D3DCall( m_pd3dd->SetFVF( D3DFVF_TLVERTEX ) );
		D3DCall( m_pd3dd->DrawIndexedPrimitiveUP(	D3DPT_LINELIST,
													0,
													vcount,
													icount / 2,
													pindex,
													D3DFMT_INDEX16,
													pvertex,
													sizeof( VertexScreen ) ) );
/*		D3DCall(m_pd3dd->DrawIndexedPrimitive(
            D3DPT_LINELIST,
            D3DFVF_TLVERTEX,
            (void*)pvertex,
            vcount,
            (MeshIndex*)pindex,
            icount,
            m_dwDrawPrimitiveFlags
        ));*/
    }

    void DrawPoints(const VertexScreen* pvertex, int vcount)
    {
/*        if (true) {
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
        } else {*/
            CheckVertices(pvertex, vcount);
 			D3DCall( m_pd3dd->SetFVF( D3DFVF_TLVERTEX ) );
			D3DCall( m_pd3dd->DrawPrimitiveUP(	D3DPT_POINTLIST,
												vcount,
												pvertex,
												sizeof( VertexScreen ) ) );
/*            D3DCall(m_pd3dd->DrawPrimitive(
                D3DPT_POINTLIST,
                D3DFVF_TLVERTEX,
                (void*)pvertex,
                vcount,
                m_dwDrawPrimitiveFlags
            ));*/
 //       }
    }
};

TRef<D3DRasterizer> CreateD3DRasterizer(PrivateSurface* psurface, HWND hParentWindow )
{
    TRef<D3DRasterizer> prasterizer = new D3DRasterizerImpl( psurface, hParentWindow );

    return prasterizer->IsValid() ? prasterizer : NULL;
}
