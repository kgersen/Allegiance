#include "pch.h"

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
    typedef TList<DeviceDependant*> DeviceDependantList;

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

    DWORD                     m_dwBPP; // KGJV 32B - user choosen bpp or desktop bbp
    //
    // Direct Draw Devices
    //

    TRef<DDDevice>            m_pdddevice;
    TRef<DDDevice>            m_pdddeviceFullscreen;
    TRef<DDDevice>            m_pdddevicePrimary;
    TRef<DDDevice>            m_pdddeviceSecondary;

    //
    //
    //

    TRef<PixelFormat>         m_ppf;
    TRef<IDirectDrawSurfaceX> m_pdds;
    HWND                      m_hwndClip;
    WinPoint                  m_pointPrimary;
    HWND                      m_hwndFocus;
    WinPoint                  m_pointFullscreen;
    WinPoint                  m_pointFullscreenCurrent;
    TRef<PrivateSurface>      m_psurfaceBack;
    TRef<IDirectDrawClipper>  m_pddClipper;
    float                     m_gamma;

    //
    // Surface Cache
    //

    DeviceDependantList       m_listDeviceDependant;
    SurfaceList               m_listSurfaces;
    SurfaceList               m_listDeviceFormatSurfaces;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Direct Draw Device Enumeration
    //
    //////////////////////////////////////////////////////////////////////////////

    static BOOL PASCAL StaticDDDeviceCallback(
        GUID FAR* lpGuid,
        LPTSTR    lpDriverDesc,
        LPTSTR    lpDriverName,
        LPVOID    lpvoid
    ) {
        EngineImpl* pthis = (EngineImpl*)lpvoid;

        return pthis->DDDeviceCallback(lpGuid, lpDriverDesc, lpDriverName);
    }

    BOOL DDDeviceCallback(
        GUID FAR* lpGuid,
        LPTSTR    lpDriverDesc,
        LPTSTR    lpDriverName
    ) {
        if (lpGuid != NULL) {
            //
            // Create the DD device.
            //

            TRef<IDirectDraw> pdd;
            HRESULT hr = DirectDrawCreate(lpGuid, &pdd, NULL);

            if (SUCCEEDED(hr)) {
                TRef<IDirectDrawX> pddx;
                DDCall(pdd->QueryInterface(IID_IDirectDrawX, (void**)&pddx));

                //
                // Create a device data object
                //

                TRef<DDDevice> pdddevice = CreateDDDevice(this, m_bAllow3DAcceleration, pddx);

                if (pdddevice->IsValid()) {
                    pdddevice->SetPrimaryDevice(m_pdddevicePrimary);

                    //
                    // If the device has 3D acceleration we'll use it as our secondary device
                    //

                    if (
                           pdddevice->Has3DAcceleration()
                        && (pdddevice->GetZBufferPixelFormat() != NULL)
                    ) {
                        m_pdddeviceSecondary = pdddevice;
                    }
                }
            }
        }

        return DDENUMRET_OK;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

public:
    EngineImpl(bool bAllow3DAcceleration, bool bAllowSecondary, DWORD dwBPP) :
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
        m_dwBPP(dwBPP) // KGJV 32B
    {
        //
        // Get the primary device
        //

        m_pdddevicePrimary = CreateDDDevice(this, m_bAllow3DAcceleration, NULL);

        if (!m_pdddevicePrimary->IsValid()) {
            // !!! replace with a ZErrorHandler call

            ::MessageBox(
                NULL,
                "Unable to create primary DirectDraw Device.\n"
                "Please update your DirectX installation",
                "Initialization Error",
                MB_ICONEXCLAMATION | MB_OK
            );
            return;
        }

        //
        // Search for other devices with 3D support for fullscreen
        //

        DDCall(DirectDrawEnumerate(StaticDDDeviceCallback, this));

        //
        // Start on the primary device
        //

        m_pdddevice = m_pdddevicePrimary;

        //
        // Create a default pixel format
        //

        // KGJV 32B - set PixelFormat according to bpp
        
        if (m_dwBPP == 0)
        {
            // fetch the desktop bpp
            DDSDescription ddsd;
            DDCall(m_pdddevicePrimary->GetDD()->GetDisplayMode(&ddsd));
            m_dwBPP = ddsd.ddpfPixelFormat.dwRGBBitCount;
            if (m_dwBPP != 32) m_dwBPP = 16; // fallback to 16 if desktop bpp isnt 32
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

    //////////////////////////////////////////////////////////////////////////////
    //
    // Terminate Dependants
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClearDependants()
    {
        {
            DeviceDependantList::Iterator iter(m_listDeviceDependant);

            while (!iter.End()) {
                iter.Value()->ClearDevice();
                iter.Next();
            }
        }

        {
            SurfaceList::Iterator iter(m_listSurfaces);

            while (!iter.End()) {
                iter.Value()->ClearDevice();
                iter.Next();
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Device Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void TerminateDevice()
    {
        ClearDependants();

        m_hwndClip     = NULL;
        m_psurfaceBack = NULL;
        m_pddClipper   = NULL;
        m_pdds         = NULL;

        m_pdddevice->FreeEverything();
        m_pdddevice = NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
        ClearDependants();

        m_hwndClip       = NULL;
        m_psurfaceBack   = NULL;
        m_pddClipper     = NULL;
        m_pdds           = NULL;
        m_pdddevice      = NULL;

        m_pdddevicePrimary->Terminate();

        if (m_pdddeviceSecondary != NULL) {
            m_pdddeviceSecondary->Terminate();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    DDDevice* GetCurrentDevice()
    {
        return m_pdddevice;
    }

    DDDevice* GetPrimaryDevice()
    {
        return m_pdddevicePrimary;
    }

    DDSDescription GetPrimaryDDSD()
    {
        DDSDescription ddsd;

        DDCall(m_pdds->GetSurfaceDesc(&ddsd));

        return ddsd;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Validation
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsValid()
    {
        return m_pdddevicePrimary->IsValid();
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

        m_hwndFocus   = pwindow->GetHWND();
        m_bFullscreen = bStartFullscreen;
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
        if (m_pdds) {
            TRef<IDirectDrawGammaControlX> pddGammaControl;

            DDCall(m_pdds->QueryInterface(IID_IDirectDrawGammaControlX, (void**)&(pddGammaControl)));

            DDGAMMARAMP gammaRamp;

            for (int index = 0; index < 256; index ++) {
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

            pddGammaControl->SetGammaRamp(0, &gammaRamp);
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
    // Create Primary Surface
    //
    //////////////////////////////////////////////////////////////////////////////

    bool CreatePrimarySurface()
    {
        //
        // Create the surface
        //

        DDSDescription ddsd;

        ddsd.dwFlags        = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        m_pdds = NULL;
        DDCall(m_pdddevice->GetDD()->CreateSurface(&ddsd, &m_pdds, NULL));

        if (m_pdds == NULL) {
            return false;
        }

        //
        // Update the gamma ramp
        //

        SetGammaRamp();

        //
        // Get the pixel format
        //

        DDCall(m_pdds->GetSurfaceDesc(&ddsd));
        m_ppf = GetPixelFormat(ddsd.GetPixelFormat());

        //
        // Create a clipper for the surface
        //

        DDCall(m_pdddevice->GetDD()->CreateClipper(0, &m_pddClipper, NULL));
        DDCall(m_pdds->SetClipper(m_pddClipper));

        return true;
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Switch to windowed
    //
    //////////////////////////////////////////////////////////////////////////////

    bool InitializeWindowed()
    {
        if (g_bWindowLog) {
            ZDebugOutput("InitializeWindowed\n");
        }

        //
        // If we were fullscreen go back to windowed mode
        //

        if (m_pdddeviceFullscreen != NULL) {
            if (g_bWindowLog) {
                ZDebugOutput("SetCooperativeLevel(Normal)\n");
            }
            DDCall(m_pdddeviceFullscreen->GetDD()->SetCooperativeLevel(NULL, DDSCL_NORMAL));
            m_pdddeviceFullscreen = NULL;
        }

        //
        // Free up all the device specific objects
        //

        TerminateDevice();

        //
        // switch to the windowed device
        //

        m_pdddevice              = m_pdddevicePrimary;
        m_pointFullscreenCurrent = WinPoint(0, 0);

        //
        // Get the primary surface
        //

        if (!CreatePrimarySurface()) {
            return false;
        }

        //
        // If the primary surface isn't 16bpp go to fullscreen automatically
        //

        // KGJV 32B : if game bpp != desktop bpp go fullscreen
        if (m_ppf->PixelBits() != m_dwBPP) {
            m_bFullscreen = true;
            return false;
        }

        //
        // Update any device format surfaces
        //

        UpdateSurfacesPixelFormat();

        if (g_bWindowLog) {
            ZDebugOutput("InitializeWindowed exiting\n");
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create Fullscreen Surface
    //
    //////////////////////////////////////////////////////////////////////////////

    bool CreateFullscreenSurface(DDDevice* pdddevice, bool& bError)
    {
        bError = false;
        HRESULT hr;

        //
        // Create a double buffered surface
        //

        DDSDescription ddsd;

        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;

        ddsd.ddsCaps.dwCaps =
              DDSCAPS_PRIMARYSURFACE
            | DDSCAPS_FLIP
            | DDSCAPS_COMPLEX
            | DDSCAPS_3DDEVICE;

        ddsd.dwBackBufferCount = 1;

        hr = pdddevice->GetDD()->CreateSurface(&ddsd, &m_pdds, NULL);

        if (hr == DDERR_OUTOFVIDEOMEMORY) {
            ZAssert(m_pdds == NULL);
            if (g_bWindowLog) {
                ZDebugOutput("Not enough memory for primary surface\n");
            }
            return false;
        }
        if (hr == DDERR_NOEXCLUSIVEMODE) {
            ZAssert(m_pdds == NULL);
            if (g_bWindowLog) {
                ZDebugOutput("Exclusive mode was lost\n");
            }
            bError = true;
            return false;
        }
        if (hr == DDERR_UNSUPPORTEDMODE) {
            ZAssert(m_pdds == NULL);
            if (g_bWindowLog) {
                ZDebugOutput("Unsupported mode\n");
            }
            bError = true;
            return false;
        }
        DDCall(hr);

        if (m_pdds == NULL) {
            return false;
        }

        //
        // Create a clipper for the surface
        //

        #ifndef DREAMCAST
            DDCall(pdddevice->GetDD()->CreateClipper(0, &m_pddClipper, NULL));
            DDCall(m_pdds->SetClipper(m_pddClipper));
        #endif        

        //
        // Update the gamma ramp
        //

        SetGammaRamp();

        //
        // Get the pixel format
        //

        DDCall(m_pdds->GetSurfaceDesc(&ddsd));

        m_ppf = GetPixelFormat(ddsd.GetPixelFormat());

        //
        // Create the ZBuffer
        //

        TRef<IDirectDrawSurfaceX> pddsZBuffer;

        if (pdddevice->GetAllow3DAcceleration()) {
            #ifdef DREAMCAST
                ddsd.dwFlags                           = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_ZBUFFERBITDEPTH;
                ddsd.dwWidth                           = ddsd.dwWidth;
                ddsd.dwHeight                          = ddsd.dwHeight;
                ddsd.ddsCaps.dwCaps                    = DDSCAPS_ZBUFFER;
                ddsd.dwZBufferBitDepth                 = 16UL;
                ddsd.ddpfPixelFormat.dwFlags           = DDPF_ZBUFFER;
                ddsd.ddpfPixelFormat.dwZBufferBitDepth = 16UL;
            #else
                ddsd.dwFlags         = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_PIXELFORMAT;
                ddsd.dwWidth         = ddsd.dwWidth;
                ddsd.dwHeight        = ddsd.dwHeight;
                ddsd.ddsCaps.dwCaps  = DDSCAPS_ZBUFFER;
                ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
                ddsd.ddpfPixelFormat = pdddevice->GetZBufferPixelFormat()->GetDDPF();
            #endif

            hr = pdddevice->GetDD()->CreateSurface(&ddsd, &pddsZBuffer, NULL);

            if (hr == DDERR_OUTOFVIDEOMEMORY) {
                if (g_bWindowLog) {
                    ZDebugOutput("Not enough memory for ZBuffer\n");
                }
                m_pdds = NULL;
                return false;
            }
        
            if (hr == DDERR_NOZBUFFERHW) {
                if (g_bWindowLog) {
                    ZDebugOutput("Device doesn't support ZBuffers\n");
                }

                m_pdds = NULL;
                return false;
            }

            DDCall(hr);
        }

        //
        // Get the back buffer
        //

        DDSCaps caps;
        caps.dwCaps = DDSCAPS_BACKBUFFER;

        TRef<IDirectDrawSurfaceX> pddsBack;
        DDCall(m_pdds->GetAttachedSurface(&caps, &pddsBack));

        if (pddsBack == NULL) {
            m_pdds = NULL;
            return false;
        }

        //
        // Attach the ZBuffer to the back buffer
        //

        if (pddsZBuffer) {
            if (FAILED(pddsBack->AddAttachedSurface(pddsZBuffer))) {
                m_pdds = NULL;
                return false;
            }
        }

        //
        // Create a surface wrapper
        //

        SurfaceType stype = SurfaceType2D() | SurfaceType3D() | SurfaceTypeZBuffer() | SurfaceTypeVideo();

        m_psurfaceBack =
            CreatePrivateSurface(
                this,
                CreateDDSurface(
                    pdddevice, 
                    pddsBack,
                    pddsZBuffer,
                    m_ppf,
                    NULL,
                    stype
                ),
                NULL
            );
        
        //
        // Fill the surface with black
        //

        m_psurfaceBack->FillSurface(Color::Black());
        Flip();
        m_psurfaceBack->FillSurface(Color::Black());

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Switch to a device at a certain resolution
    //
    //////////////////////////////////////////////////////////////////////////////

    bool SwitchToFullscreenDevice(DDDevice* pdddevice, const WinPoint& size, bool& bError)
    {
        bError = false;

        if (g_bWindowLog) {
            ZDebugOutput(
                  "SwitchToFullscreenDevice( " 
                + pdddevice->GetName() 
                + ", resolution: " 
                + GetString(size) 
                + ")\n"
            );
        }

        //
        // If switching to a different device go to normal mode
        //

        if (m_pdddeviceFullscreen != NULL && m_pdddeviceFullscreen != pdddevice) {
            if (g_bWindowLog) {
                ZDebugOutput("SetCooperativeLevel(" + pdddevice->GetName() + ", Normal)\n");
            }
            DDCall(m_pdddeviceFullscreen->GetDD()->SetCooperativeLevel(NULL, DDSCL_NORMAL));
            m_pdddeviceFullscreen = NULL;
        }

        //
        // Free up all the device specific objects
        //

        TerminateDevice();
        m_pdddevice = pdddevice;

        //
        // If this is a new fullscreen device go to exclusive mode
        //

        if (m_pdddeviceFullscreen != pdddevice) {
            if (g_bWindowLog) {
                ZDebugOutput("SetCooperativeLevel(" + pdddevice->GetName() + ", Exclusive)\n");
            }

            HRESULT hr =
                pdddevice->GetDD()->SetCooperativeLevel(
                    m_hwndFocus,
                      DDSCL_EXCLUSIVE
                    | DDSCL_FULLSCREEN
                );

            if (hr == DDERR_EXCLUSIVEMODEALREADYSET) {
                if (g_bWindowLog) {
                    ZDebugOutput("Can't set exclusive mode\n");
                }
                bError = true;
                return false;
            }

            DDCall(hr);

            m_pdddeviceFullscreen = pdddevice;
        }

        //
        // Switch resolutions
        //

        HRESULT hr =
            m_pdddevice->GetDD()->SetDisplayMode(
                size.X(),
                size.Y(),
                m_dwBPP, // KGJV 32B - set as parameter
                0,
                0
            );

        if (
               hr == DDERR_NOEXCLUSIVEMODE
            || hr == DDERR_UNSUPPORTED
        ) {
            if (g_bWindowLog) {
                ZDebugOutput("Error setting display mode\n");
            }
            bError = true;
            return false;
        }

        if (hr == DDERR_INVALIDMODE) {
            pdddevice->EliminateModes(size);
            if (g_bWindowLog) {
                ZDebugOutput("Invalid resolution\n");
            }
            return false;
        }

        DDCall(hr);

        //
        // Create the primary surface and back buffer
        //

        if (!CreateFullscreenSurface(pdddevice, bError)) {
            return false;
        }

        //
        // Everything worked.  Update any device format surfaces.
        //

        UpdateSurfacesPixelFormat();

        if (g_bWindowLog) {
            ZDebugOutput("SwitchToFullscreenDevice exiting\n");
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

        //
        // Try the secondary device first
        //

        DDDevice* pdddevice;

        if (
               m_bAllowSecondary 
            && m_bAllow3DAcceleration
            && m_b3DAccelerationImportant
            && m_pdddeviceSecondary != NULL
            && m_pdddeviceSecondary->GetAllow3DAcceleration()
        ) {
            pdddevice = m_pdddeviceSecondary;
        } else {
            pdddevice = m_pdddevicePrimary;
        }

        //
        // Don't do anything if we don't need to change the device
        // or resolution
        //

        if (  
               m_bValidDevice
            && m_pdddevice              == pdddevice 
            && m_pointFullscreenCurrent == m_pointFullscreen
        ) {
            ZDebugOutput("Device and resolution match\n");
            return true;
        }

        //
        // Try different resolutions until we find one that actually works
        //

        bChanges = true;

        while (true) {
            //
            // Try the current resolution
            //

            bool bError;
            if (SwitchToFullscreenDevice(pdddevice, m_pointFullscreen, bError)) {
                m_pointFullscreenCurrent = m_pointFullscreen;
                return true;
            }

            //
            // If there was an error just return
            //

            if (bError) {
                return false;
            }

            //
            // Didn't work goto to the next lower resolution
            //

            WinPoint pointNew = pdddevice->PreviousMode(m_pointFullscreen);

            if (pointNew == m_pointFullscreen) {
                if (g_bWindowLog) {
                    ZDebugOutput("No more valid resolutions\n");
                }
                return false;
            }

            m_pointFullscreen = pointNew;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DebugSetWindowed()
    {
        #ifndef DREAMCAST
            if (m_pdddevice != NULL) {
                m_pdddevice->GetDD()->SetCooperativeLevel(NULL, DDSCL_NORMAL);
            }
        #endif
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
            m_pdddevicePrimary->SetAllow3DAcceleration(m_bAllow3DAcceleration);
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
			m_pdddevicePrimary->SetMaxTextureSize(m_dwMaxTextureSize);
			m_bValid		= false;
			m_bValidDevice	= false;
		}
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

    void ChangeFullscreenSize(bool bLarger)
    {
        WinPoint point;

        if (bLarger) {
            point = m_pdddevice->NextMode(m_pointFullscreen);
        } else {
            point = m_pdddevice->PreviousMode(m_pointFullscreen);
        }

        SetFullscreenSize(point);
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
        return 
               m_pdddevicePrimary->Has3DAcceleration()
            && (m_pdddevicePrimary->GetZBufferPixelFormat() != NULL);
    }

    ZString GetDeviceName()
    {
        return m_pdddevice->GetName();
    }

    bool GetUsing3DAcceleration()
    {
        return m_pdddevice->GetAllow3DAcceleration();
    }

    PrivateSurface* GetBackBuffer()
    {
        return m_psurfaceBack;
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
        if (!m_bValid) {
            if (m_bFullscreen) {
                m_bValid = InitializeFullscreen(bChanges);
            } else {
                bChanges = true;
                m_bValid = InitializeWindowed();
            }

            m_bValidDevice = m_bValid;
        }

        return m_bValid;
    }

    bool IsDeviceReady(bool& bChanges)
    {
        bChanges = false;

        if (m_pdddevice) {
            HRESULT hr = m_pdddevice->TestCooperativeLevel();

            switch (hr) {
                case DD_OK:
                    return DeviceOK(bChanges);

                case DDERR_NOEXCLUSIVEMODE:
                    //
                    // fullscreen but not active
                    //

                    m_bValidDevice = false;
                    m_bValid       = false;
                    break;

                case DDERR_EXCLUSIVEMODEALREADYSET:
                    //
                    // windowed somebody else is fullscreen
                    //

                    m_bValidDevice = false;
                    m_bValid       = false;
                    break;

                case DDERR_WRONGMODE:
                    //
                    // windowed the pixel depth has changed
                    //

                    m_pdddevicePrimary->Reset(NULL);
                    m_bValidDevice = false;
                    m_bValid       = false;

                    return DeviceOK(bChanges);

                default:
                    ZError("Unexpected result\n");
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen updates
    //
    //////////////////////////////////////////////////////////////////////////////

    void Flip()
    {
        ZAssert(m_pdddeviceFullscreen);

        DDSurface* pddsurface; CastTo(pddsurface, m_psurfaceBack->GetVideoSurface());
        pddsurface->GetDDSX();

        HRESULT hr = m_pdds->Flip(NULL, DDFLIP_WAIT);
        if (
               hr == DDERR_NOEXCLUSIVEMODE
            || hr == DDERR_SURFACELOST

        ) {
            // These errors are ok if we are no longer active.
        } else {
            DDCall(hr);
        }
    }

    void BltToWindow(Window* pwindow, const WinPoint& point, Surface* psurface, const WinRect& rectSource)
    {
        if (m_pdddeviceFullscreen == NULL) {
            if (m_hwndClip != pwindow->GetHWND()) {
                m_hwndClip = pwindow->GetHWND();
                DDCall(m_pddClipper->SetHWnd(0, m_hwndClip));

            }
            m_pointPrimary = pwindow->ClientToScreen(WinPoint(0, 0));

            WinRect
                rectTarget(
                    m_pointPrimary + point,
                    m_pointPrimary + point + rectSource.Size()
                );

            PrivateSurface* pprivateSurface; CastTo(pprivateSurface, psurface);
            DDSurface*      pddsurface;      CastTo(pddsurface, pprivateSurface->GetVideoSurface()); 
            HRESULT hr =
                m_pdds->Blt(
                    (RECT*)&rectTarget,
                    pddsurface->GetDDSX(),
                    (RECT*)&rectSource,
                    DDBLT_WAIT,
                    NULL
                );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pixel Format cache
    //
    //////////////////////////////////////////////////////////////////////////////

    TVector<TRef<PixelFormat> > m_ppfs;

    TRef<PixelFormat> GetPixelFormat(const DDPixelFormat& ddpf)
    {
        int count = m_ppfs.GetCount();

        for(int index = 0; index < count; index++) {
            if (m_ppfs[index]->Equivalent(ddpf)) {
                return m_ppfs[index];
            }
        }

        TRef<PixelFormat> ppf = new PixelFormat(ddpf);

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
        DDPixelFormat ddpf;

        ddpf.dwSize            = sizeof(DDPIXELFORMAT);
        ddpf.dwFlags           = DDPF_RGB;
        ddpf.dwFourCC          = 0;
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

    int GetTotalTextureMemory()     { return m_pdddevice->GetTotalTextureMemory();     }
    int GetAvailableTextureMemory() { return m_pdddevice->GetAvailableTextureMemory(); }
    int GetTotalVideoMemory()       { return m_pdddevice->GetTotalVideoMemory();       }
    int GetAvailableVideoMemory()   { return m_pdddevice->GetAvailableVideoMemory();   }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface Cache
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddDeviceDependant(DeviceDependant* pdeviceDependant)
    {
        m_listDeviceDependant.PushFront(pdeviceDependant);
    }

    void RemoveDeviceDependant(DeviceDependant* pdeviceDependant)
    {
        m_listDeviceDependant.Remove(pdeviceDependant);
    }

    void RemovePrivateSurface(PrivateSurface* psurface)
    {
        //
        // Remove from the surface lists
        //

        m_listDeviceFormatSurfaces.Remove(psurface);
        m_listSurfaces.Remove(psurface);

        //
        // free up any device textures
        //

        DDSurface* pddsurface; CastTo(pddsurface, psurface->GetVideoSurfaceNoAlloc());

        if (pddsurface) {
            m_pdddevicePrimary->RemoveSurface(pddsurface);
            if (m_pdddeviceSecondary != NULL) {
                m_pdddeviceSecondary->RemoveSurface(pddsurface);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // VideoSurface Constructors
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<VideoSurface> CreateVideoSurface(
          SurfaceType     stype,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size,
          int             pitch,
          BYTE*           pbits
    ) {
        if (stype.Test(SurfaceTypeVideo())) {
            return 
                CreateDDSurface(
                    m_pdddevice,
                    stype,
                    m_ppf,
                    NULL,
                    size
                );
        } else {
            PrivatePalette* pprivatePalette; CastTo(pprivatePalette, ppalette);

            return
                CreateDDSurface(
                    m_pdddevicePrimary,
                    stype,
                    ppf,
                    ppalette,
                    size,
                    pitch,
                    pbits
                );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface Constructors
    //
    //////////////////////////////////////////////////////////////////////////////

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
        Palette*        ppalette,
        SurfaceType     stype,
        SurfaceSite*    psite
    ) {
        PrivatePalette* pprivatePalette; CastTo(pprivatePalette, ppalette);

        return
            AddSurface(
                CreatePrivateSurface(
                    this,
                    ppf,
                    pprivatePalette,
                    size,
                    stype,
                    psite
                ),
                false
            );
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
        if (stype.Test(SurfaceTypeVideo())) {
            TRef<DDSurface> pddsurface = 
                CreateDDSurface(
                    m_pdddevice,
                    stype,
                    m_ppf,
                    NULL,
                    size
                );

            if (pddsurface != NULL) {
                TRef<PrivateSurface> psurface = CreatePrivateSurface(this, pddsurface, psite);
                return AddSurface(psurface, true);
            }

            return NULL;
        } else {
            return 
                AddSurface(
                    CreatePrivateSurface(
                        this,
                        m_ppf, 
                        NULL,
                        size, 
                        stype, 
                        psite
                    ), 
                    true
                );
        }
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
        PrivatePalette* pprivatePalette; CastTo(pprivatePalette, psurface->GetPalette());

        return
            AddSurface(
                CreatePrivateSurface(
                    this,
                    psurface->GetPixelFormat(),
                    pprivatePalette,
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
        //
        // Get the bitmap size
        //

        BITMAP bm;
        ZVerify(::GetObject(hbitmap, sizeof(bm), &bm));

        //
        // Create a DC for the bitmap
        //

        HDC hdcBitmap = ::CreateCompatibleDC(NULL);
        ZAssert(hdcBitmap != NULL);
        HBITMAP hbitmapOld = (HBITMAP)::SelectObject(hdcBitmap, hbitmap);
        ZAssert(hbitmapOld != NULL);

        //
        // Create a surface whose pixel format matches the bitmap
        //

        DDPixelFormat ddpf;
        TRef<IDirectDrawPaletteX> pddpal;

        ZVerify(FillDDPF(ddpf, m_pdddevicePrimary->GetDD(), hdcBitmap, hbitmap, &pddpal));

        TRef<PrivatePalette> ppalette;
        if (pddpal) {
            ppalette = CreatePaletteImpl(pddpal);
        }

        //
        // Create the source surface
        //

        TRef<PrivateSurface> psurface =
            CreatePrivateSurface(
                this,
                GetPixelFormat(ddpf),
                ppalette,
                WinPoint(bm.bmWidth, bm.bmHeight),
                SurfaceType2D(),
                NULL
            );

        //
        // Copy the bitmap to the surface
        //

        psurface->BitBltFromDC(hdcBitmap);

        //
        // Release the DC we created
        //

        ZVerify(::SelectObject(hdcBitmap, hbitmapOld));
        ZVerify(::DeleteDC(hdcBitmap));

        return AddSurface(psurface, false);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateSurface(
    const WinPoint&    size,
          PixelFormat* ppf,
          Palette*     ppalette,
          int          pitch,
          BYTE*        pdata,
          IObject*     pobjectMemory
    ) {
        PrivatePalette* pprivatePalette; CastTo(pprivatePalette, ppalette);

        return
            AddSurface(
                CreatePrivateSurface(
                    this, 
                    ppf, 
                    pprivatePalette,
                    size, 
                    pitch, 
                    pdata, 
                    pobjectMemory
                ),
                false
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
// KGJV 32B - BPP parameter
TRef<Engine> CreateEngine(bool bAllow3DAcceleration, bool bAllowSecondary, DWORD dwBPP)
{
    return new EngineImpl(bAllow3DAcceleration, bAllowSecondary, dwBPP);
}
