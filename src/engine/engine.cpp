#include "pch.h"

HWND g_hwndMainWindow;

CDX9EngineSettings g_DX9Settings;

//KGJV -oct2008: cleanup, removed old commented code

//////////////////////////////////////////////////////////////////////////////
//
// Engine Implementation
//
//////////////////////////////////////////////////////////////////////////////

class EngineImpl : public PrivateEngine {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // types
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef TList<PrivateSurface*>  SurfaceList;
    //typedef TList<DeviceDependant*> DeviceDependantList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    //
    // State
    //

    bool                      m_bValid;
    bool                      m_bValidDevice;
    bool                      m_bFullscreen;
    bool                      m_bAllowSecondary;
    bool                      m_bAllow3DAcceleration;
	DWORD					  m_dwMaxTextureSize;// yp Your_Persona August 2 2006 : MaxTextureSize Patch
    bool                      m_b3DAccelerationImportant;
	bool					  m_bMipMapGenerationEnabled;

    DWORD                     m_dwBPP; // KGJV 32B - user choosen bpp or desktop bbp

	//
    //
    //

    TRef<PixelFormat>         m_ppf;
    HWND                      m_hwndClip;
    WinPoint                  m_pointPrimary;
    HWND                      m_hwndFocus;
    WinPoint                  m_pointFullscreen;
    WinPoint                  m_pointFullscreenCurrent;
//    TRef<PrivateSurface>      m_psurfaceBack;
    float                     m_gamma;

    //
    // Surface Cache
    //

    //DeviceDependantList       m_listDeviceDependant;
    SurfaceList               m_listSurfaces;
    SurfaceList               m_listDeviceFormatSurfaces;


    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

public:
    EngineImpl(bool bAllow3DAcceleration, bool bAllowSecondary, DWORD dwBPP, HWND hWindow) :
        m_pointFullscreen(800, 600),
        m_pointFullscreenCurrent(0, 0),
        m_bFullscreen(false),
        m_bAllow3DAcceleration(bAllow3DAcceleration),
        m_bAllowSecondary(bAllowSecondary),
        m_b3DAccelerationImportant(false),
        m_bValid(false),
        m_bValidDevice(false),
        m_hwndFocus(NULL),
        m_hwndClip(NULL),
        m_gamma(1.0f),
        m_dwBPP(dwBPP), // KGJV 32B
		m_bMipMapGenerationEnabled( false ),
		m_dwMaxTextureSize( 0 )
    {

		if( ( CD3DDevice9::Get()->GetCurrentMode()->mode.Format == D3DFMT_A8B8G8R8 ) ||
			( CD3DDevice9::Get()->GetCurrentMode()->mode.Format == D3DFMT_A8R8G8B8 ) ||
			( CD3DDevice9::Get()->GetCurrentMode()->mode.Format == D3DFMT_X8B8G8R8 ) ||
			( CD3DDevice9::Get()->GetCurrentMode()->mode.Format == D3DFMT_X8R8G8B8 ) )
		{
			m_dwBPP = 32;
		}
		else
		{
			m_dwBPP = 16;
		}
        if (m_dwBPP == 32)
            m_ppf = new PixelFormat(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (m_dwBPP == 16)
            m_ppf = new PixelFormat(16, 0xf800, 0x07e0, 0x001f, 0x0000);
    }

private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~EngineImpl()
    {
    }

    void TerminateDevice()
    {
        m_hwndClip     = NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate( bool bEngineAppTerminate /*=false*/)
    {
        m_hwndClip       = NULL;
		
		// Reset D3D device.
		if( bEngineAppTerminate == true )
		{
			CD3DDevice9::Get()->Shutdown();
		}
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Validation
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsValid()
    {
		return CD3DDevice9::Get()->IsDeviceValid();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetFocusWindow(Window* pwindow, bool bStartFullscreen)
    {
        //
        // This function can only be called once
        //

        ZAssert(m_hwndFocus == NULL && pwindow->GetHWND() != NULL);
        ZAssert(!m_bValid);

        m_hwndFocus			= pwindow->GetHWND();
        m_bFullscreen		= bStartFullscreen;
		g_hwndMainWindow	= m_hwndFocus;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Device Initialization
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateSurfacesPixelFormat()
    {
        //
        // Tell all the Device format surfaces their new pixel format
        //

        {
            SurfaceList::Iterator iterSurface(m_listDeviceFormatSurfaces);

            while (!iterSurface.End()) {
                iterSurface.Value()->SetPixelFormat(m_ppf);
                iterSurface.Next();
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // SetGammaRamp
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetGammaRamp()
    {
		if( CD3DDevice9::Get()->IsDeviceValid() == true )
		{
            D3DGAMMARAMP gammaRamp;

            for (int index = 0; index < 256; index ++) 
			{
                float value  = (float)index / 255;
                float level  = pow(value, 1.0f / m_gamma);
                //float level  = (m_gamma - 1) + (1 - (m_gamma - 1)) * value;
                int   ilevel = MakeInt(level * 65535.0f);

                gammaRamp.red  [index] = ilevel;
                gammaRamp.green[index] = ilevel;
                gammaRamp.blue [index] = ilevel;
            };

            //
            // zero is always black
            //

            gammaRamp.red  [0] = 0;
            gammaRamp.green[0] = 0;
            gammaRamp.blue [0] = 0;

			CD3DDevice9::Get()->SetGammaRamp(0, D3DSGR_CALIBRATE, &gammaRamp);
        }
    }

    void SetGammaLevel(float value)
    {
        m_gamma = bound(value, 1.0f, 2.0f);
        SetGammaRamp();
    }

    float GetGammaLevel()
    {
        return m_gamma;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Switch to windowed
    //
    //////////////////////////////////////////////////////////////////////////////

    bool InitializeWindowed(const WinPoint& size)
    {
        if (g_bWindowLog) {
            ZDebugOutput("InitializeWindowed\n");
        }

        TerminateDevice();

        m_pointFullscreenCurrent = WinPoint(0, 0);

		CD3DDevice9::Get()->ResetDevice( true, size.X(), size.Y() );

        if (g_bWindowLog) {
            ZDebugOutput("InitializeWindowed exiting\n");
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Switch to full screen
    //
    //////////////////////////////////////////////////////////////////////////////

    bool InitializeFullscreen(bool& bChanges)
    {
        if (g_bWindowLog) {
            ZDebugOutput("InitalizeFullscreen()\n");
        }

		if( ( CD3DDevice9::Get()->IsDeviceValid() == true ) && 
			( m_pointFullscreenCurrent == m_pointFullscreen ) )
		{
			return true;
		}

        bChanges = true;

		if (g_bWindowLog) 
		{
			ZDebugOutput( "SwitchToFullscreenDevice\n" );
        }

        TerminateDevice();

		CD3DDevice9::Get()->ResetDevice( false, m_pointFullscreen.X(), m_pointFullscreen.Y() );

        if (g_bWindowLog) {
            ZDebugOutput("SwitchToFullscreenDevice exiting\n");
        }

        m_pointFullscreenCurrent = m_pointFullscreen;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DebugSetWindowed()
    {
		// obsolete
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Set Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetAllowSecondary(bool bAllowSecondary)
    {
        if (m_bAllowSecondary != bAllowSecondary) {
            m_bAllowSecondary = bAllowSecondary;
            m_bValid       = false;
            m_bValidDevice = false;
        }
    }

    void SetAllow3DAcceleration(bool bAllow3DAcceleration)
    {
        if (m_bAllow3DAcceleration != bAllow3DAcceleration) {
            m_bAllow3DAcceleration = bAllow3DAcceleration;
            m_bValid       = false;
            m_bValidDevice = false;
        }
    }
// yp Your_Persona August 2 2006 : MaxTextureSize Patch
	void SetMaxTextureSize(DWORD dwMaxTextureSize)
	{
		if (m_dwMaxTextureSize != dwMaxTextureSize)
		{
			m_dwMaxTextureSize = dwMaxTextureSize;
			m_bValid		= false;
			m_bValidDevice	= false;
		}
	}

	DWORD GetMaxTextureSize(void)
	{
		return m_dwMaxTextureSize;
	}

	void SetEnableMipMapGeneration(bool bEnable)
	{
		// Store a local copy, reflect the setting in the vram manager.
		m_bMipMapGenerationEnabled = bEnable;
		CVRAMManager::Get()->SetEnableMipMapGeneration( bEnable );
	}

    void Set3DAccelerationImportant(bool b3DAccelerationImportant)
    {
        if (m_b3DAccelerationImportant != b3DAccelerationImportant) {
            m_b3DAccelerationImportant = b3DAccelerationImportant;
            m_bValid = false;
        }
    }

    void SetFullscreen(bool bFullscreen)
    {
        if (m_bFullscreen != bFullscreen) {
            m_bFullscreen = bFullscreen;
            m_bValid       = false;
            m_bValidDevice = false;
        }
    }

    void SetFullscreenSize(const WinPoint& point)
    {
        if (g_bWindowLog) {
            ZDebugOutput("Engine::SetFullscreenSize(" + GetString(point) + ")\n");
        }

        if (m_pointFullscreen != point) {
            m_pointFullscreen = point;
            m_bValid          = false;
        }

        if (g_bWindowLog) {
            ZDebugOutput("Engine::SetFullscreenSize() Exiting\n");
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Get Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    bool GetAllowSecondary()
    {
        return m_bAllowSecondary;
    }

    bool GetAllow3DAcceleration()
    {
        return m_bAllow3DAcceleration;
    }

    bool Get3DAccelerationImportant()
    {
        return m_b3DAccelerationImportant;
    }

    const WinPoint& GetFullscreenSize()
    {
        return m_pointFullscreen;
    }

    bool IsFullscreen()
    {
        return m_bFullscreen;
    }

    bool PrimaryHas3DAcceleration()
    {
        return true;
    }

    ZString GetDeviceName()
    {
		return CD3DDevice9::Get()->GetDeviceSetupString();
    }

    bool GetUsing3DAcceleration()
    {
		return true;
    }

    ZString GetPixelFormatName()
    {
        return ZString("Bits per pixel = ") + ZString((int)(GetPrimaryPixelFormat()->PixelBits()));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //  
    //
    //////////////////////////////////////////////////////////////////////////////

    bool DeviceOK(bool& bChanges)
    {
        if (!m_bValid) 
		{
            if (m_bFullscreen) 
			{
                m_bValid = InitializeFullscreen(bChanges);
			} 
			else 
			{
                bChanges = true;
                m_bValid = InitializeWindowed(m_pointFullscreen);
            }

            m_bValidDevice = m_bValid;
        }

        return m_bValid;
    }

    bool IsDeviceReady(bool& bChanges)
    {
		CD3DDevice9 * pDev = CD3DDevice9::Get();
		if( pDev->IsDeviceValid() )
		{
			HRESULT hr = pDev->TestCooperativeLevel( );
			switch( hr )
			{
			case D3D_OK:
               return DeviceOK(bChanges);

			case D3DERR_DEVICELOST:
				// Device lost - for example, full screen window lost focus.
				// Sleep but carry on running. At some point we should hit a D3DERR_DEVICENOTRESET
				// return value in TestCooperativeLevel().
				m_bValidDevice = false;
				m_bValid       = false;
				break;

			case D3DERR_DEVICENOTRESET:
				hr = pDev->ResetDevice( pDev->IsWindowed() );
				if( hr == D3D_OK )
				{
					m_bValid = true;
					return DeviceOK(bChanges);
				}
				break;

			default:
				break;
			}
		}
		return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pixel Format cache
    //
    //////////////////////////////////////////////////////////////////////////////

    TVector<TRef<PixelFormat> > m_ppfs;

    TRef<PixelFormat> GetPixelFormat(const D3D9PixelFormat& ddpf)
    {
        int count = m_ppfs.GetCount();

        for(int index = 0; index < count; index++) {
            if (m_ppfs[index]->Equivalent(ddpf)) {
                return m_ppfs[index];
            }
        }

		TRef<PixelFormat> ppf = new PixelFormat(ddpf.dwRGBBitCount, 
												ddpf.dwRBitMask, 
												ddpf.dwGBitMask, 
												ddpf.dwBBitMask, 
												ddpf.dwRGBAlphaBitMask );

        m_ppfs.PushEnd(ppf);

        return ppf;
    }

    TRef<PixelFormat> GetPixelFormat(
        int   bits,
        DWORD redMask,
        DWORD greenMask,
        DWORD blueMask,
        DWORD alphaMask
    ) {
        D3D9PixelFormat ddpf;

        ddpf.dwRGBBitCount     = bits;
        ddpf.dwRBitMask        = redMask;
        ddpf.dwGBitMask        = greenMask;
        ddpf.dwBBitMask        = blueMask;
        ddpf.dwRGBAlphaBitMask = alphaMask;

        return GetPixelFormat(ddpf);
    }

    PixelFormat* GetPrimaryPixelFormat()
    {
        return m_ppf;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Performance Counters
    //
    //////////////////////////////////////////////////////////////////////////////

	int GetTotalTextureMemory()     { return 0; } //m_pdddevice->GetTotalTextureMemory();     }
    int GetAvailableTextureMemory() { return 0; } //m_pdddevice->GetAvailableTextureMemory(); }
    int GetTotalVideoMemory()       { return 0; } //m_pdddevice->GetTotalVideoMemory();       }
    int GetAvailableVideoMemory()   { return 0; } //m_pdddevice->GetAvailableVideoMemory();   }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface Cache
    //
    //////////////////////////////////////////////////////////////////////////////

    void RemovePrivateSurface(PrivateSurface* psurface)
    {
        //
        // Remove from the surface lists
        //
        m_listDeviceFormatSurfaces.Remove(psurface);
        m_listSurfaces.Remove(psurface);
    }

    TRef<PrivateSurface> AddSurface(PrivateSurface* psurface, bool bDevice)
    {
        m_listSurfaces.PushEnd(psurface);

        if (bDevice) {
            m_listDeviceFormatSurfaces.PushEnd(psurface);
        }

        return psurface;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a surface with the specified pixel format
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateSurface(
        const WinPoint& size,
        PixelFormat*    ppf,
        SurfaceType     stype,
        SurfaceSite*    psite
) {

        return
            AddSurface(
                CreatePrivateSurface(
                    this,
                    ppf,
                    size,
                    stype,
                    psite
                ),
                false
            );
    }


    //////////////////////////////////////////////////////////////////////////////
	// CreateDummySurface()
	// Create a dummy surface which has valid dimensions, but no actual
	// VRAM resources associated with it.
    //////////////////////////////////////////////////////////////////////////////
	TRef<Surface> CreateDummySurface(	const WinPoint& size, 
								        SurfaceSite*    psite /*=NULL*/ )
	{
		TRef<PrivateSurface> psurface = CreatePrivateDummySurface( this, size, psite );
		return AddSurface( psurface, true );
	}


    //////////////////////////////////////////////////////////////////////////////
	// CreateRenderTargetSurface()
	// Create a render target surface.
    //////////////////////////////////////////////////////////////////////////////
	TRef<Surface> CreateRenderTargetSurface( const WinPoint& size, 
											 SurfaceSite*    psite /*=NULL*/ )
	{
		TRef<PrivateSurface> psurface = CreatePrivateRenderTargetSurface( this, size, psite );
		return AddSurface( psurface, true );
	}


    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a device format surface
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateSurface(
        const WinPoint& size, 
        SurfaceType stype, 
        SurfaceSite* psite
    ) {
		if( ( stype.Test( SurfaceTypeColorKey() ) == true ) &&
			( m_ppf->AlphaMask() == 0 ) )
		{
			if( m_ppf->PixelBytes() == 4 )
			{
				m_ppf = new PixelFormat(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
			}
			else
			{
				if( CD3DDevice9::Get()->GetDevFlags()->bSupportsA1R5G6B6Format == true )
				{
					m_ppf = new PixelFormat(16, 0x7C00, 0x03e0, 0x001f, 0x8000);
				}
				else
				{
					m_ppf = new PixelFormat(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
				}
			}
		}
        return 
            AddSurface(
                CreatePrivateSurface(
                    this,
                    m_ppf, 
                    size, 
                    stype, 
                    psite
                ), 
                true
            );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<PrivateSurface> CreateCompatibleSurface(
        PrivateSurface* psurface, 
        const WinPoint& size,
        SurfaceType     stype, 
        SurfaceSite*    psite
    ) {

		// Construct the pixel format, including any alpha due to color keying.
		PixelFormat * pixelFormat;

		if( psurface->HasColorKey() == false )
		{
			// No colour key, just a straight copy of the pixel format.
			pixelFormat = psurface->GetPixelFormat();
		}
		else
		{
			// For now we just handle two explicit cases. 16 bit with 1 bit alpha or full 32 bit.
			pixelFormat = psurface->GetPixelFormat();
			if( ( pixelFormat->PixelBytes() == 2 ) &&
				( CD3DDevice9::Get()->GetDevFlags()->bSupportsA1R5G6B6Format == true ) )
			{
				pixelFormat = new PixelFormat( D3DFMT_A1R5G5B5 );
			}
			else
			{
				pixelFormat = new PixelFormat( D3DFMT_A8R8G8B8 );
			}
		}
        return
            AddSurface(
                CreatePrivateSurface(
                    this,
                    pixelFormat,
                    size,
                    stype,
                    psite
                ),
                false
            );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateSurface(HBITMAP hbitmap)
    {
		_ASSERT( false );

		return AddSurface(NULL, false);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateSurface(    const WinPoint&		size,
									PixelFormat*		ppf,
									int					pitch,
									BYTE*				pdata,
									IObject*			pobjectMemory,
									const bool			bColorKey,
									const Color &		cColourKey,
									const ZString &		szTextureName, /*=""*/
									const bool			bSystemMemory ) 
	{
        return AddSurface(	CreatePrivateSurface(	this, 
													ppf, 
													size, 
													pitch, 
													pdata, 
													pobjectMemory,
													bColorKey,
													cColourKey,
													szTextureName,
													bSystemMemory),
							false );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////////
	TRef<Surface> CreateSurfaceD3DX(	const D3DXIMAGE_INFO *	pImageInfo,
										const WinPoint *		pTargetSize,
										IObject *				pobjectMemory,
										const bool				bColorKey,
										const Color &			cColorKey,
										const ZString &			szTextureName /*= ""*/,
										const bool				bSystemMemory /*= false*/ )
	{
		return AddSurface( CreatePrivateSurface(	this,
													pImageInfo,
													pTargetSize,
													pobjectMemory,
													bColorKey,
													cColorKey,
													szTextureName,
													bSystemMemory ),
							false );
	}
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Engine> CreateEngine(bool bAllow3DAcceleration, bool bAllowSecondary, DWORD dwBPP, HWND hWindow )
{
    return new EngineImpl(bAllow3DAcceleration, bAllowSecondary, dwBPP, hWindow );
}
