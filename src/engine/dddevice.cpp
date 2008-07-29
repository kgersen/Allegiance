#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// DeviceTextureImpl
//
//////////////////////////////////////////////////////////////////////////////

/*class DeviceTexture : public IObject {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IDirectDrawSurfaceX> m_pdds;
    TRef<IDirect3DTextureX>   m_pd3dtexture;
    DDSurface*                m_pddsurface;
    int                       m_idSurface;
    TRef<PixelFormat>         m_ppf;
    bool                      m_bMipMap;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    DeviceTexture(
        DDDevice*       pdddevice,
        D3DDevice*      pd3dd,
        DDSurface*      pddsurface,
        const WinPoint& size
    ) :
        m_pddsurface(pddsurface),
        m_idSurface(-1),
        m_ppf(pd3dd->GetTextureFormat()),
        m_bMipMap(false)
    {
        DWORD dw = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;

//        if (m_bMipMap) {
//            m_pdds = pdddevice->CreateMipMapTexture(size, m_ppf);
//        } 
//		else
		{
//            m_pdds = pdddevice->CreateSurface(size, dw, m_ppf, true);
			m_pd3dtexture = pdddevice->CreateSurface(size, dw, m_ppf, true);
        }

        if (m_pdds != NULL) {
            if (m_pddsurface->HasColorKey()) {
                DDCOLORKEY key;

                key.dwColorSpaceLowValue  =
                key.dwColorSpaceHighValue =
                    m_ppf->MakePixel(m_pddsurface->GetColorKey()).Value();

                DDCall(m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));
            }

            DDCall(m_pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&m_pd3dtexture));

            //
            // Try to load the surface
            //

            DoLoad(m_pd3dtexture, size);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsValid()
    {
        return m_pd3dtexture != NULL;
    }

    bool DoLoad(TRef<IDirect3DTextureX> pd3dtexture, WinPoint size)
    {
        while (pd3dtexture != NULL) 
		{
			_ASSERT( false );
//            /*HRESULT hr;
//				
  //              pd3dtexture->Load(
    //                m_pddsurface->GetTextureX(m_ppf, size, m_idSurface)
    //        );
//
//           if (FAILED(hr)) {
//                //
//                // couldn't load it free everything
//                //
//
//                if (
//                        hr != DDERR_SURFACELOST
//                    && hr != DDERR_SURFACEBUSY
//                ) {
//                    DDCall(hr);
//                }
//                m_pd3dtexture = NULL;
//                m_pdds        = NULL;
//                return false;
//            }

            if (m_bMipMap) {
                size.SetX(size.X() / 2);
                size.SetY(size.Y() / 2);

                TRef<IDirectDrawSurfaceX> pdds;
                DDCall(pd3dtexture->QueryInterface(IID_IDirectDrawSurfaceX, (void**)&pdds));

                DDSCAPS2 ddsCaps;
                ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
                DDCall(pdds->GetAttachedSurface(&ddsCaps, &pdds));

                DDCall(pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&pd3dtexture));
            }

            return true;
        }

        return true;
    }

    IDirect3DTextureX* GetTextureX(const WinPoint& size)
    {
        int idSave = m_idSurface;
        IDirect3DTextureX* pd3dtexture = m_pddsurface->GetTextureX(m_ppf, size, m_idSurface);

        if (idSave != m_idSurface) 
		{
            if( !DoLoad( pd3dtexture, size ) ) 
			{
                return NULL;
            }
        }

        return m_pd3dtexture;
    }
};*/

//////////////////////////////////////////////////////////////////////////////
//
// Direct Draw Device Implementation
//
//////////////////////////////////////////////////////////////////////////////

WinPoint g_validModes[] =
{
    WinPoint( 640,  480),
    WinPoint( 800,  600),
    WinPoint(1024,  768),
    WinPoint(1280, 1024),
    WinPoint(1600, 1200)
};

const int g_countValidModes = sizeof(g_validModes) / sizeof(g_validModes[0]);

class DDDeviceImpl : public DDDevice 
{
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // types
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef TList<D3DDevice*>  D3DDeviceList;
    typedef TList<Rasterizer*> RasterizerList;
    typedef TMap<DDSurface*, TRef<DeviceTexture> > DeviceTextureMap;

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<DDDevice>			m_pdddevicePrimary;
    TRef<IDirectDrawX>		m_pdd;
    TRef<IDirect3DX>		m_pd3d;
	TRef<D3DDevice>			m_pD3DDevice;
	HWND					m_hParentWindow;
	DeviceTextureMap		m_mapDeviceTextures;
    ZString					m_strName;
    bool					m_b3DAcceleration;
    bool					m_bAllow3DAcceleration;
	D3DFORMAT				m_zbufFormat;
	DWORD					m_dwMaxTextureSize;// yp Your_Persona August 2 2006 : MaxTextureSize Patch

    PrivateEngine*			m_pengine;
//    D3DDeviceList			m_listD3DDevices;
    RasterizerList			m_listRasterizers;

//    DDCaps					m_ddcapsHW;
//    DDCaps					m_ddcapsSW;
    DWORD					m_dwTotalVideoMemory;
    TRef<PixelFormat>		m_ppfZBuffer;
    TVector<WinPoint>		m_modes;

    //////////////////////////////////////////////////////////////////////////////
	// EnumerateDisplayModes()
	// Using the D3D9 interface, grab all available display modes and add
	// to the vector of available modes.
	// For now, we'll just interrogate the primary device, although this should
	// be extended to all devices, so the user could select which monitor the game
	// is displayed on. We are also sticking with 16 bit format for now too,
	// although we really want to move to 32 bit.
    //////////////////////////////////////////////////////////////////////////////
	HRESULT EnumerateDisplayModes( )
	{
		DWORD i;
		D3DFORMAT			desiredFormat = D3DFMT_R5G6B5;
		DWORD				dwModeCount = m_pd3d->GetAdapterModeCount( D3DADAPTER_DEFAULT, desiredFormat );
		D3DDISPLAYMODE		dispMode;
		WinPoint			winSize;

		for( i=0; i<dwModeCount; i++ )
		{
			if( m_pd3d->EnumAdapterModes(	D3DADAPTER_DEFAULT,
											desiredFormat,
											i,
											&dispMode ) != D3D_OK )
			{
				OutputDebugString( "Failed to enumerate adapter modes.\n");
				_ASSERT( false );
				return E_FAIL;
			}
			
			for( int iIndex=0; iIndex<g_countValidModes; iIndex++ )
			{
				if( ( dispMode.Width == g_validModes[iIndex].X() ) &&
					( dispMode.Height == g_validModes[iIndex].Y() ) )
				{
					winSize.SetX( dispMode.Width );
					winSize.SetY( dispMode.Height );
					m_modes.PushEnd( winSize );
					break;
				}
			}
		}

		return S_OK;
	}


    //////////////////////////////////////////////////////////////////////////////
	// EnumerateZBufferModes()
	// Use CheckDeviceFormat() to find available Z buffer modes.
	// Original Alleg implementation just selected the first mode where the 
	// bit depth was at least 16 bit. For now, we'll do the same.
    //////////////////////////////////////////////////////////////////////////////
	HRESULT EnumerateZBufferModes( )
	{
		// Attempt to get a 32, 24 or 16 bit z buffer.
		m_zbufFormat = D3DFMT_D32;
		HRESULT hRes = m_pd3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
													D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

		if( hRes != D3D_OK )
		{
			_ASSERT( hRes == D3DERR_NOTAVAILABLE );

			m_zbufFormat = D3DFMT_D24S8;
			HRESULT hRes = m_pd3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
														D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

			if( hRes != D3D_OK )
			{
				_ASSERT( hRes == D3DERR_NOTAVAILABLE );

				m_zbufFormat = D3DFMT_D24X8;
				HRESULT hRes = m_pd3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
															D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

				if( hRes != D3D_OK )
				{
					_ASSERT( hRes == D3DERR_NOTAVAILABLE );
	
					m_zbufFormat = D3DFMT_D16;
					HRESULT hRes = m_pd3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
															D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

					if( hRes != D3D_OK )
					{
						m_zbufFormat = D3DFMT_UNKNOWN;
						_ASSERT( hRes == D3DERR_NOTAVAILABLE );
						return E_FAIL;
					}
				}
			}
		}
		return S_OK;
	}

    //////////////////////////////////////////////////////////////////////////////
    //
    // Initialize
    //
    //////////////////////////////////////////////////////////////////////////////

    void Initialize( HWND hParentWindow )
    {
		// Store the window handle.
		m_hParentWindow = hParentWindow;

		// Get the D3D pointer
		m_pd3d = Direct3DCreate9( D3D_SDK_VERSION );

        // Enumerate the display modes
        EnumerateDisplayModes( );

		// TBD: Enumerate the zbuffer formats properly!
		EnumerateZBufferModes( );

		// Create the D3D device now.
		m_pD3DDevice = CreateD3DDevice( hParentWindow );		// Calls CreateD3DDevice in this (dddevice) class().
		
/*		if (pdd == NULL) 
		{
            TRef<IDirectDraw> pddPrimary;

            HRESULT hr = DirectDrawCreate(NULL, &pddPrimary, NULL);

            if (SUCCEEDED(hr)) 
			{
                DDCall(pddPrimary->QueryInterface(IID_IDirectDrawX, (void**)&m_pdd));
            }
        } 
		else 
		{
            m_pdd = pdd;
        }

        if (m_pdd) 
		{
            // Get Device Info
            DDDeviceIdentifier dddi;
            m_pdd->GetDeviceIdentifier(&dddi, DDGDI_GETHOSTIDENTIFIER);

            m_strName = dddi.szDescription;

            // Get device capabilities.
            DDCall(m_pdd->GetCaps(&m_ddcapsHW, &m_ddcapsSW));

            // Get the amount of video memory
            DWORD dwFree;
            DDSCaps ddsc;
            ddsc.dwCaps = DDSCAPS_VIDEOMEMORY;

            HRESULT hr = m_pdd->GetAvailableVidMem(&ddsc, &m_dwTotalVideoMemory, &dwFree);
            if (DDERR_NODIRECTDRAWHW != hr)
                DDCall(hr);

            // Does the driver support 3D with texture mapping and zbuffer?
            DWORD ddsCaps = DDSCAPS_TEXTURE | DDSCAPS_ZBUFFER;

            m_b3DAcceleration =
                   ((m_ddcapsHW.dwCaps         & DDCAPS_3D) !=       0)
                && ((m_ddcapsHW.ddsCaps.dwCaps & ddsCaps  ) == ddsCaps);

            // Set the cooperative level to Normal
			DDCall(m_pdd->SetCooperativeLevel(NULL, DDSCL_NORMAL));

            // Get the D3D pointer
//            DDCall(m_pdd->QueryInterface(IID_IDirect3DX, (void**)&m_pd3d));
			m_pd3d = Direct3DCreate9( D3D_SDK_VERSION );

            // Enumerate the display modes
            m_pdd->EnumDisplayModes(0, NULL, this, StaticEnumModes);

			// Enumerate the zbuffer formats
//            m_pd3d->EnumZBufferFormats(GetIID(true), StaticEnumZBufferFormats, this);
        }*/
    }

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructors
    //
    //////////////////////////////////////////////////////////////////////////////

    DDDeviceImpl(
        PrivateEngine*	pengine,
        bool			bAllow3DAcceleration,
		HWND			hParentWindow
    ) :
        m_pengine(pengine),
        m_bAllow3DAcceleration(bAllow3DAcceleration)
    {
        Initialize( hParentWindow );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void FreeEverything()
    {
        m_mapDeviceTextures.SetEmpty();

/*       {
	         D3DDeviceList::Iterator iter(m_listD3DDevices);

            while (!iter.End()) {
                iter.Value()->Terminate();
                iter.Next();
            }
        }*/

        {
            RasterizerList::Iterator iter(m_listRasterizers);

            while (!iter.End()) {
                iter.Value()->Terminate();
                iter.Next();
            }
        }
    }

    void Terminate()
    {
        FreeEverything();
        m_pd3d = NULL;
        m_pdd  = NULL;
    }

    void Reset( )
    {
        Terminate();
        Initialize( m_hParentWindow );
    }

/*    void RemoveD3DDevice(D3DDevice* pd3ddevice)
    {
        m_listD3DDevices.Remove(pd3ddevice);
    }*/

    void RemoveRasterizer(Rasterizer* praster)
    {
        m_listRasterizers.Remove(praster);
    }

    void AddRasterizer(Rasterizer* praster)
    {
        m_listRasterizers.PushFront(praster);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Validation
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsValid()
    {
        return m_pd3d != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Set Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetPrimaryDevice(DDDevice* pdddevice)
    {
        m_pdddevicePrimary = pdddevice;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Get Methods
    //
    //////////////////////////////////////////////////////////////////////////////
	D3DDevice *	GetD3DDevice()
	{
		return m_pD3DDevice;
	}


    HRESULT TestCooperativeLevel()
    {
		return m_pD3DDevice->GetD3DDeviceX()->TestCooperativeLevel();
    }

/*    const IID& GetIID(bool bAllowHAL)
    {
        if (m_b3DAcceleration && bAllowHAL) {
            return IID_IDirect3DHALDevice;
        } else {
            return IID_IDirect3DRGBDevice;
        }
    }*/

    IDirectDrawX* GetDD()     
    { 
        return m_pdd;
    }

    IDirect3DX*   GetD3D()
    { 
        return m_pd3d;
    }

    PrivateEngine*  GetEngine()
    { 
        return m_pengine;
    }

    PixelFormat* GetZBufferPixelFormat()
    {
        return m_ppfZBuffer;
    }

    bool Has3DAcceleration()
    {
        return m_b3DAcceleration;
    }

// yp Your_Persona August 2 2006 : MaxTextureSize Patch
	void SetMaxTextureSize(DWORD dwMaxTextureSize)
	{
		m_dwMaxTextureSize = dwMaxTextureSize;
	}
// yp Your_Persona August 2 2006 : MaxTextureSize Patch
	DWORD GetMaxTextureSize()
	{
		return m_dwMaxTextureSize;
	}

	void SetAllow3DAcceleration(bool bAllow3DAcceleration)
    {
        m_bAllow3DAcceleration = bAllow3DAcceleration;
    }

    bool GetAllow3DAcceleration()
    {
        return m_b3DAcceleration && m_bAllow3DAcceleration;
    }

    ZString GetName()
    {
        return m_strName;
    }

    WinPoint NextMode(const WinPoint& size)
    {
        int count = m_modes.GetCount();

        for(int index = 0; index < count; index++) {
            if (
                   m_modes[index].X() > size.X() 
                || m_modes[index].Y() > size.Y() 
            ) {
                return m_modes[index];
            }
        }

        return m_modes[count - 1];
    }

    WinPoint PreviousMode(const WinPoint& size)
    {
        int count = m_modes.GetCount();

        for(int index = count - 1 ; index > 0; index--) {
            if (
                   m_modes[index].X() < size.X() 
                || m_modes[index].Y() < size.Y() 
            ) {
                return m_modes[index];
            }
        }

        return m_modes[0];
    }

    void EliminateModes(const WinPoint& size)
    {
        int count = m_modes.GetCount();

        for(int index = 0; index < count; index++) {
            if (
                   m_modes[index].X() >= size.X() 
                && m_modes[index].Y() >= size.Y() 
            ) {
                m_modes.SetCount(index);
                return;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a D3D Device
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<D3DDevice> CreateD3DDevice( HWND hParentWindow )
    {
        TRef<IDirect3DDeviceX> pd3dd;

        bool bAcceleration = m_bAllow3DAcceleration; // && pddsurface->InVideoMemory();

/*        pddsurface->GetDDSXZBuffer();

//		HRESULT hr = D3D_OK;
            m_pd3d->CreateDevice(
                GetIID(bAcceleration),
                pddsurface->GetDDSX(),
                &pd3dd
                #ifndef DREAMCAST
                    ,NULL
                #endif
            );*/
		D3DPRESENT_PARAMETERS d3dPresParams;
		memset( &d3dPresParams, 0, sizeof( D3DPRESENT_PARAMETERS ) );
		d3dPresParams.AutoDepthStencilFormat		= D3DFMT_D24S8;
		d3dPresParams.EnableAutoDepthStencil		= TRUE;
		d3dPresParams.BackBufferCount				= 1;
		d3dPresParams.hDeviceWindow					= hParentWindow;
		d3dPresParams.Windowed						= TRUE;
		d3dPresParams.BackBufferFormat				= D3DFMT_R5G6B5;
		d3dPresParams.BackBufferWidth				= 800;
		d3dPresParams.BackBufferHeight				= 600;
		d3dPresParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		d3dPresParams.FullScreen_RefreshRateInHz	= 0;
		d3dPresParams.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
		d3dPresParams.MultiSampleQuality			= D3DMULTISAMPLE_NONE;
		d3dPresParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
		d3dPresParams.Flags							= 0;

		HRESULT hr = m_pd3d->CreateDevice(	D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											hParentWindow,
											D3DCREATE_HARDWARE_VERTEXPROCESSING,
											&d3dPresParams,
											&pd3dd );


        if (hr == DDERR_INVALIDPARAMS) {
            // this can happen if the surface is lost
            return NULL;
        } 

        DDCall(hr);

        if (FAILED(hr)) {
            return NULL;
        }

        TRef<D3DDevice> pd3ddevice = ::CreateD3DDevice( hParentWindow );		// Calls the CreateD3DDevice in d3ddevice.cpp.

/*        if (pd3ddevice != NULL) 
		{
            m_listD3DDevices.PushFront(pd3ddevice);
        }*/

        return pd3ddevice;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a Direct Draw Surface
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IDirect3DTextureX> CreateSurface(
        const WinPoint& size,
        DWORD           caps,
        PixelFormat*    ppf,
        bool            bAllocationCanFail
    ) 
	{
		// Create a D3D texture instead of a surface.
		_ASSERT( false );
		return NULL;
/*        if (m_pdddevicePrimary != NULL && (caps & DDSCAPS_VIDEOMEMORY) == 0) {
            return m_pdddevicePrimary->CreateSurface(size, caps, ppf, bAllocationCanFail);
        } else 
		{
            DDSDescription ddsd;

			if( ppf == NULL )
			{
				return NULL;
			}

            ddsd.dwFlags         = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_PIXELFORMAT;
            ddsd.dwWidth         = size.X();
            ddsd.dwHeight        = size.Y();
            ddsd.ddsCaps.dwCaps  = caps;
            ddsd.ddpfPixelFormat = ppf->GetDDPF();

            TRef<IDirectDrawSurfaceX> pdds;
            HRESULT hr = m_pdd->CreateSurface(&ddsd, &pdds, NULL);

            if (
                   hr == DDERR_OUTOFMEMORY
                || hr == DDERR_OUTOFVIDEOMEMORY
            ) {
                if (bAllocationCanFail) {
                    return NULL;
                }
            }

            DDCall(hr);

            return pdds;
        }*/
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a Mip Map texture Surface
    //
    //////////////////////////////////////////////////////////////////////////////

/*    TRef<IDirectDrawSurfaceX> CreateMipMapTexture(
        const WinPoint& size,
        PixelFormat*    ppf
    ) {
        //
        // Figure out how many levels we need
        //

        int minSize = min(size.X(), size.Y());
        int maps    = 0;

        while (minSize > 1) {
            maps++;
            minSize /= 2;
        }

        //
        // Create the surface
        //

        DDSDescription ddsd;

        ddsd.dwFlags         = 
              DDSD_HEIGHT 
            | DDSD_WIDTH 
            | DDSD_CAPS 
            | DDSD_PIXELFORMAT 
            | DDSD_MIPMAPCOUNT;
        ddsd.dwWidth         = size.X(); 
        ddsd.dwHeight        = size.Y();
        ddsd.dwMipMapCount   = 2;//maps;
        ddsd.ddsCaps.dwCaps  = 
              DDSCAPS_VIDEOMEMORY 
            | DDSCAPS_TEXTURE 
            | DDSCAPS_MIPMAP 
            | DDSCAPS_COMPLEX
            | DDSCAPS_ALLOCONLOAD;
        ddsd.ddpfPixelFormat = ppf->GetDDPF();

        TRef<IDirectDrawSurfaceX> pdds;
        HRESULT hr = m_pdd->CreateSurface(&ddsd, &pdds, NULL);

        //
        // Check for errors
        //

        if (
               hr == DDERR_OUTOFMEMORY
            || hr == DDERR_OUTOFVIDEOMEMORY
        ) {
            return NULL;
        }

        DDCall(hr);

        return pdds;
    }
*/
    //////////////////////////////////////////////////////////////////////////////
    //
    // Texture Cache
    //
    //////////////////////////////////////////////////////////////////////////////

    void RevokeTextures()
    {
        m_mapDeviceTextures.SetEmpty();
    }

    void BeginScene()
    {
    }

    void EndScene()
    {
    }

    TRef<DeviceTexture> CreateDeviceTexture(D3DDevice* pd3dd, DDSurface* pddsurface, const WinPoint& size)
    {
        TRef<DeviceTexture> pdeviceTexture = new DeviceTexture(this, pd3dd, pddsurface, size);

        if (pdeviceTexture->IsValid()) {
            return pdeviceTexture;
        }

        return NULL;
    }

    IDirect3DTextureX* GetTextureX(D3DDevice* pd3dd, DDSurface* pddsurface)
    {
        WinPoint size = pddsurface->GetSize();

        WinPoint pointMin = pd3dd->GetMinTextureSize();
        WinPoint pointMax = pd3dd->GetMaxTextureSize(GetMaxTextureSize());// yp Your_Persona August 2 2006 : MaxTextureSize Patch

        if (pointMin.X() != 0 || pointMin.Y() != 0) {
            while (
                   size.X() < pointMin.X()
                || size.Y() < pointMin.Y()
            ) {
                size.SetX(size.X() * 2);
                size.SetY(size.Y() * 2);
            }
        }

        if (pointMax.X() != 0 || pointMax.Y() != 0) {
            while (
                   size.X() > pointMax.X()
                || size.Y() > pointMax.Y()
            ) {
                size.SetX(size.X() / 2);
                size.SetY(size.Y() / 2);
            }
        }

        if (pd3dd->IsHardwareAccelerated()) {
            TRef<DeviceTexture> ptexture;

            if (!m_mapDeviceTextures.Find(pddsurface, ptexture)) {
                while (true) {
                    ptexture = CreateDeviceTexture(pd3dd, pddsurface, size);
                    if (ptexture) {
                        m_mapDeviceTextures.Set(pddsurface, ptexture);
                        break;
                    } 

                    RevokeTextures();
                }
            }

            if (ptexture) {
                TRef<IDirect3DTextureX> pd3dtexture = ptexture->GetTextureX(size);

                if (pd3dtexture) {
                    return pd3dtexture;
                }

                m_mapDeviceTextures.Remove(pddsurface);
            }

            return NULL;
        } else {
            int id;
            return pddsurface->GetTextureX(pd3dd->GetTextureFormat(), size, id);
        }
    }

    void RemoveSurface(DDSurface* pddsurface)
    {
        m_mapDeviceTextures.Remove(pddsurface);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Performance counters
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetTotalTextureMemory()
    {
 		return CD3DDevice9::Device()->GetAvailableTextureMem();
    }

    int GetAvailableTextureMemory()
    {
 		return CD3DDevice9::Device()->GetAvailableTextureMem();
    }

    int GetTotalVideoMemory()
    {
		return CD3DDevice9::Device()->GetAvailableTextureMem();
    }

    int GetAvailableVideoMemory()
    {
		return CD3DDevice9::Device()->GetAvailableTextureMem();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Performance counters
//
//////////////////////////////////////////////////////////////////////////////

TRef<DDDevice> CreateDDDevice( PrivateEngine* pengine, bool bAllow3DAcceleration, HWND hParentWindow )
{
    return new DDDeviceImpl(pengine, bAllow3DAcceleration, hParentWindow );
}
