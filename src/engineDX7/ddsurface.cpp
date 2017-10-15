#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Surface Modes
//
//////////////////////////////////////////////////////////////////////////////

enum SurfaceMode {
    SurfaceModeDD,
    SurfaceModeLocked
};

//////////////////////////////////////////////////////////////////////////////
//
// VideoSurface
//
//////////////////////////////////////////////////////////////////////////////

class DDSurfaceImpl : public DDSurface {
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class SurfaceData {
    public:
        int                       m_id;
        TRef<PixelFormat>         m_ppf;
        TRef<IDirectDrawSurfaceX> m_pdds;
        TRef<IDirect3DTextureX>   m_pd3dtexture;
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<PrivateEngine>       m_pengine;
    TRef<DDDevice>            m_pdddevice;
    TRef<IDirectDrawSurfaceX> m_pddsZBuffer;
    WinPoint                  m_size;
    WinPoint                  m_sizeSurface;
    SurfaceType               m_stype;

    DDSDescription            m_ddsdLocked;

    BYTE*                     m_pbits;
    int                       m_pitch;
    Color                     m_colorKey;
    TRef<PrivatePalette>      m_ppalette;
    Color                     m_colorText;

    bool                      m_bTextureSize;
    bool                      m_bColorKey;
    bool                      m_bSharedMemory;

    //
    // Mip-Mapped levels
    //

    SurfaceData               m_data;
    TVector<SurfaceData>      m_datas;

    //
    // Converted detail levels
    //

    SurfaceData               m_dataConverted;
    TVector<SurfaceData>      m_datasConverted;

    //
    // Surface invalidation and modes
    //

    SurfaceMode               m_mode;

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalculateSurfaceSize()
    {
        m_sizeSurface = m_size;

        if (m_size.X() < 1) {
            ZAssert(m_size.X() == 0);
            m_sizeSurface.SetX(1);
        }

        if (m_size.Y() < 1) {
            ZAssert(m_size.Y() == 0);
            m_sizeSurface.SetY(1);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Initialize()
    {
        CalculateSurfaceSize();

        m_bTextureSize =
               m_sizeSurface.Y() == (int)NextPowerOf2((DWORD)m_sizeSurface.Y())
            && m_sizeSurface.X() == (int)NextPowerOf2((DWORD)m_sizeSurface.X());

        m_bColorKey = false;
        m_colorKey  = Color(0, 0, 0);
        m_data.m_id = 0;
        m_mode      = SurfaceModeDD;
        m_pbits     = NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    void FinishConstruction()
    {
        //
        // Set the palette if there is one
        //

        if (m_ppalette) {
            m_data.m_pdds->SetPalette(m_ppalette->GetDDPal());
        }

        //
        // Get a texture handle if needed
        //
        if (m_bTextureSize && !m_stype.Test(SurfaceTypeVideo())) {
#ifdef USEDX7
            m_data.m_pd3dtexture = m_data.m_pdds;
#else
            DDCall(m_data.m_pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&(m_data.m_pd3dtexture)));
#endif
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a video memory surface
    //
    //////////////////////////////////////////////////////////////////////////////

    DDSurfaceImpl(
              DDDevice*       pdddevice,
              SurfaceType     stype,
              PixelFormat*    ppf,
              PrivatePalette* ppalette,
        const WinPoint&       size
    ) :
        m_pdddevice(pdddevice),
        m_pengine(pdddevice->GetEngine()),
        m_stype(stype),
        m_ppalette(ppalette),
        m_size(size),
        m_bSharedMemory(false)
    {
        m_data.m_ppf   = ppf;

        Initialize();

        //
        // Figure out the DD caps
        //

        DWORD caps = DDSCAPS_VIDEOMEMORY;

        if (Is3D()) {
            caps |= DDSCAPS_3DDEVICE;
        } else {
            if (m_bTextureSize) {
                caps |= DDSCAPS_TEXTURE;
            } else {
                caps |= DDSCAPS_OFFSCREENPLAIN;
            }
        }

        //
        // Create the surface
        //

        m_data.m_pdds = m_pdddevice->CreateSurface(size, caps, ppf, true);

        //
        // Was there enough memory to allocate the surface? 
        //

        if (m_data.m_pdds != NULL) {
            //
            // Get the pitch
            //

            DDSDescription ddsd;
            DDCall(m_data.m_pdds->GetSurfaceDesc(&ddsd));
            m_pitch = ddsd.lPitch;

            //
            // Do any other common construction tasks
            //

            FinishConstruction();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Wrap an previously created DirectDraw surface
    //
    //////////////////////////////////////////////////////////////////////////////

    DDSurfaceImpl(
        DDDevice*            pdddevice,
        IDirectDrawSurfaceX* pdds,
        IDirectDrawSurfaceX* pddsZBuffer,
        PixelFormat*         ppf,
        PrivatePalette*      ppalette,
        SurfaceType          stype
    ) :
        m_pdddevice(pdddevice),
        m_pengine(pdddevice->GetEngine()),
        m_pddsZBuffer(pddsZBuffer),
        m_ppalette(ppalette),
        m_stype(stype | SurfaceType2D()),
        m_bSharedMemory(false)
    {
        DDSDescription ddsd;

        DDCall(pdds->GetSurfaceDesc(&ddsd));

        m_data.m_pdds  = pdds;
        m_data.m_ppf   = ppf;
        m_size         = WinPoint(ddsd.XSize(), ddsd.YSize());
        m_pitch        = ddsd.Pitch();

        Initialize();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Create a surface from a binary representation
    //
    //////////////////////////////////////////////////////////////////////////////

    DDSurfaceImpl(
              DDDevice*       pdddevice,
              SurfaceType     stype,
              PixelFormat*    ppf,
              PrivatePalette* ppalette,
        const WinPoint&       size,
              int             pitch,
              BYTE*           pbits
    ) :
        m_pdddevice(pdddevice),
        m_pengine(pdddevice->GetEngine()),
        m_stype(stype),
        m_ppalette(ppalette),
        m_size(size)
    {
        //
        // Is the memory writable?
        //

        MEMORY_BASIC_INFORMATION meminfo;
        ZVerify(VirtualQuery(pbits, &meminfo, sizeof(meminfo)) == sizeof(meminfo));
        m_bSharedMemory = 
            (
                   (
                          meminfo.Protect 
                        & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY)
                   )
                != 0
            );

        //
        // initialize
        //

        m_pitch        = pitch;
        m_data.m_ppf   = ppf;

        Initialize();

        //
        // !!! Dreamcast doesn't support client allocated memory
        //     what to do about that?
        //

        //
        // Figure out the DD caps
        //

        DWORD caps = DDSCAPS_SYSTEMMEMORY;

        if (Is3D()) {
            caps |= DDSCAPS_3DDEVICE;
        }

        if (m_bTextureSize) {
            caps |= DDSCAPS_TEXTURE;
        } else {
            caps |= DDSCAPS_OFFSCREENPLAIN;
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
            | DDSD_PITCH;
        ddsd.dwWidth         = m_size.X();
        ddsd.dwHeight        = m_size.Y();
        ddsd.lPitch          = pitch;
        ddsd.ddsCaps.dwCaps  = caps;
        ddsd.ddpfPixelFormat = m_data.m_ppf->GetDDPF();


        if (m_bSharedMemory) {
            //
            // use the passed in memory
            //

            ddsd.dwFlags   = ddsd.dwFlags | DDSD_LPSURFACE;
            ddsd.lpSurface = pbits;
            HRESULT hr = m_pdddevice->GetDD()->CreateSurface(&ddsd, &m_data.m_pdds, NULL);
            DDCall(hr);

            #ifdef _DEBUG
                DDSDescription ddsd;
                DDCall(m_data.m_pdds->GetSurfaceDesc(&ddsd));
                ZAssert(pitch == ddsd.lPitch);
            #endif

            m_pitch = pitch;
        } else {
            //
            // Let directx allocate the memory
            //

            HRESULT hr = m_pdddevice->GetDD()->CreateSurface(&ddsd, &m_data.m_pdds, NULL);
            DDCall(hr);

            //
            // Copy the bits over
            //

            DDCall(m_data.m_pdds->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
            m_pitch = ddsd.Pitch();

            ZAssert(pitch <= m_pitch);

            for (int y = 0; y < m_size.Y(); y++) {
                memcpy(
                    ddsd.Pointer() + y * m_pitch,
                    pbits          + y * pitch,
                    pitch
                );
            }

            DDCall(m_data.m_pdds->Unlock(NULL));
        }

        //
        // Do any other common construction tasks
        //

        FinishConstruction();
    }

    bool IsMemoryShared()
    {
        return m_bSharedMemory;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsValid()
    {
        return m_data.m_pdds != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~DDSurfaceImpl()
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attach a ZBuffer surface
    //
    //////////////////////////////////////////////////////////////////////////////

    IDirectDrawSurfaceX* GetDDSXZBuffer()
    {
        if (HasZBuffer()) {
            if (m_pddsZBuffer == NULL) {
                DWORD caps = DDSCAPS_ZBUFFER;

                if (m_stype.Test(SurfaceTypeVideo())) {
                    caps |= DDSCAPS_VIDEOMEMORY;
                } else {
                    caps |= DDSCAPS_SYSTEMMEMORY;
                }

                m_pddsZBuffer =
                    m_pdddevice->CreateSurface(
                        m_size,
                        caps,
                        m_pdddevice->GetZBufferPixelFormat(),
                        true
                    );

                if (m_pddsZBuffer) {
                    DDCall(m_data.m_pdds->AddAttachedSurface(m_pddsZBuffer));
                }
            }
        }

        return m_pddsZBuffer;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface updates
    //
    //////////////////////////////////////////////////////////////////////////////

    void SurfaceChanged()
    {
        m_data.m_id++;
        if (m_data.m_id < 0) m_data.m_id = 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Down Sample
    //
    //////////////////////////////////////////////////////////////////////////////

    static DWORD Unpack(WORD pixelWord)
    {
        DWORD pixel = pixelWord;

        return
              ((pixel & 0xf800) << 5)
            | ((pixel & 0x07e0) << 3)
            | ((pixel & 0x001f)     );
    }

    static WORD Pack(DWORD pixel)
    {
        return
            (WORD)(
                  ((pixel >> 5) & 0xf800)
                | ((pixel >> 3) & 0x07e0)
                | ((pixel     ) & 0x001f)
            );
    }

    static WORD Average(WORD pixel0, WORD pixel1, WORD pixel2, WORD pixel3)
    {
        return
            Pack(
                (
                      Unpack(pixel0)
                    + Unpack(pixel1)
                    + Unpack(pixel2)
                    + Unpack(pixel3)
                ) >> 2
            );
    }

    static void DownSample(
        const WinPoint&      sizeTarget,
        IDirectDrawSurfaceX* pddsTarget,
        IDirectDrawSurfaceX* pddsSource
    ) {
        DDSDescription ddsdSource;
        DDSDescription ddsdTarget;

        DDCall(pddsSource->Lock(NULL, &ddsdSource, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
        DDCall(pddsTarget->Lock(NULL, &ddsdTarget, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));

        int pitchSource = ddsdSource.Pitch();
        int pitchTarget = ddsdTarget.Pitch();

        ZAssert(
               ddsdSource.GetPixelFormat().dwRGBBitCount
            == ddsdTarget.GetPixelFormat().dwRGBBitCount
        );

        switch (ddsdSource.GetPixelFormat().dwRGBBitCount) {
            case 16:
                {
                    ZAssert(ddsdSource.GetPixelFormat().dwRBitMask        == 0xf800);
                    ZAssert(ddsdSource.GetPixelFormat().dwGBitMask        == 0x07e0);
                    ZAssert(ddsdSource.GetPixelFormat().dwBBitMask        == 0x001f);
                    ZAssert(ddsdSource.GetPixelFormat().dwRGBAlphaBitMask == 0x0000);

                    WORD* psource   = (WORD*)ddsdSource.Pointer();
                    WORD* ptarget   = (WORD*)ddsdTarget.Pointer();

                    for (int y = sizeTarget.Y() - 1; y >= 0; y--) {
                        for (int x = sizeTarget.X() - 1; x >= 0; x--) {
                            ptarget[x] =
                                Average(
                                    psource[x * 2],
                                    psource[x * 2 + 1],
                                    psource[x * 2     + pitchSource / 2],
                                    psource[x * 2 + 1 + pitchSource / 2]
                                );
                        }

                        psource += 2 * pitchSource / 2;
                        ptarget +=     pitchTarget / 2;
                    }
                }
                break;

            case 24:
                {
                    BYTE* psource   = (BYTE*)ddsdSource.Pointer();
                    BYTE* ptarget   = (BYTE*)ddsdTarget.Pointer();

                    for (int y = sizeTarget.Y() - 1; y >= 0; y--) {
                        for (int x = sizeTarget.X() - 1; x >= 0; x--) {
                            BYTE* psource0 = psource + x * 6;
                            BYTE* psource1 = psource + x * 6 + 3;
                            BYTE* psource2 = psource + x * 6     + pitchSource;
                            BYTE* psource3 = psource + x * 6 + 3 + pitchSource;

                            ptarget[x * 3 + 0] = (psource0[0] + psource1[0] + psource2[0] + psource3[0]) / 4;
                            ptarget[x * 3 + 1] = (psource0[1] + psource1[1] + psource2[1] + psource3[1]) / 4;
                            ptarget[x * 3 + 2] = (psource0[2] + psource1[2] + psource2[2] + psource3[2]) / 4;
                        }

                        psource += pitchSource * 2;
                        ptarget += pitchTarget;
                    }
                }
                break;

            default:
                ZError("DownSample: Invalid PixelFormat");
        };

        DDCall(pddsTarget->Unlock(NULL));
        DDCall(pddsSource->Unlock(NULL));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Construct a new lod or converted surface
    //
    //////////////////////////////////////////////////////////////////////////////

    void ConstructSurfaceData(SurfaceData& data, PixelFormat* ppf, const WinPoint& size)
    {
        data.m_id  = -1;
        data.m_ppf = ppf;

        DWORD caps = DDSCAPS_SYSTEMMEMORY;

        if (m_bTextureSize/* && ppf->IsSoftwareTexture()*/) {
            caps |= DDSCAPS_TEXTURE;
        } else {
            caps |= DDSCAPS_OFFSCREENPLAIN;
        }

        data.m_pdds =
            m_pdddevice->CreateSurface(
                size,
                caps,
                ppf,
                false
            );

        if (HasColorKey()) {
            DDCOLORKEY key;

            key.dwColorSpaceLowValue  =
            key.dwColorSpaceHighValue =
                ppf->MakePixel(GetColorKey()).Value();

            DDCall(data.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));
        }

        if (caps & DDSCAPS_TEXTURE) {
#ifdef USEDX7
            data.m_pd3dtexture = data.m_pdds;
#else
            DDCall(data.m_pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&(data.m_pd3dtexture)));
#endif
        } else {
            data.m_pd3dtexture = NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Convert pixel formats using GDI
    //
    //////////////////////////////////////////////////////////////////////////////

    static void Convert(const WinPoint& size, IDirectDrawSurfaceX* pddsTarget, IDirectDrawSurfaceX* pddsSource)
    {
        HDC hdcSource;
        DDCall(pddsSource->GetDC(&hdcSource));

        HDC hdcTarget;
        DDCall(pddsTarget->GetDC(&hdcTarget));

        ZVerify(::BitBlt(hdcTarget, 0, 0, size.X(), size.Y(), hdcSource, 0, 0, SRCCOPY));

        DDCall(pddsTarget->ReleaseDC(hdcTarget));
        DDCall(pddsSource->ReleaseDC(hdcSource));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Make sure the bits for a converted surface are available and up to date.
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateConvertedSurface(PixelFormat* ppf, const WinPoint& size, SurfaceData& data, const SurfaceData& dataSource)
    {
        if (ppf != data.m_ppf) {
            ConstructSurfaceData(data, ppf, size);
        }

        if (data.m_id != dataSource.m_id) {
            SetSurfaceMode(SurfaceModeDD);

            Convert(size, data.m_pdds, dataSource.m_pdds);
            data.m_id = dataSource.m_id;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Get a surface with a certain pixel format.
    //
    //////////////////////////////////////////////////////////////////////////////

    IDirectDrawSurfaceX* GetDDSX()
    {
        SetSurfaceMode(SurfaceModeDD);
        return m_data.m_pdds;
    }

    IDirectDrawSurfaceX* GetDDSX(PixelFormat* ppf)
    {
        if (ppf == m_data.m_ppf) {
            SetSurfaceMode(SurfaceModeDD);
            return m_data.m_pdds;
        } else {
            UpdateConvertedSurface(ppf, m_size, m_dataConverted, m_data);
            return m_dataConverted.m_pdds;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Get a texture with a certain pixel format.
    //
    //////////////////////////////////////////////////////////////////////////////

    IDirect3DTextureX* GetTextureX(PixelFormat* ppf, const WinPoint& size, int& id)
    {
        id = m_data.m_id;

        //
        // Return the fullsize surface if that's what's wanted.
        //

        if (size == m_size) {
            if (ppf == m_data.m_ppf) {
                SetSurfaceMode(SurfaceModeDD);
                #ifdef DREAMCAST
                    if (!m_data.m_pd3dtexture) {
                        DDCall(m_data.m_pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&(m_data.m_pd3dtexture)));
                    }
                #endif
                return m_data.m_pd3dtexture;
            }

            UpdateConvertedSurface(ppf, m_size, m_dataConverted, m_data);
            return m_dataConverted.m_pd3dtexture;
        }

        //
        // Need to return a lower level of detail
        //

        int index = 0;
        WinPoint sizeSource = m_size;

        while (true) {
            sizeSource.SetX(sizeSource.X() / 2);
            sizeSource.SetY(sizeSource.Y() / 2);

            //
            // Do we need to allocate a new lower level of detail?
            //

            if (index == m_datas.GetCount()) {
                m_datas.PushEnd();
                m_datasConverted.PushEnd();
                ConstructSurfaceData(m_datas.Get(index), m_data.m_ppf, sizeSource);
            }

            //
            // Do we need to update this level?
            //

            if (m_datas[index].m_id != m_data.m_id) {
                DownSample(
                    sizeSource,
                    m_datas[index].m_pdds,
                    index == 0 ?
                          m_data.m_pdds
                        : m_datas[index - 1].m_pdds
                );

                m_datas.Get(index).m_id = m_data.m_id;
            }

            //
            // Did we find the right size?
            //

            if (sizeSource == size) {
                //
                // Does the format need to be converted?
                //

                if (ppf == m_data.m_ppf) {
                    return m_datas[index].m_pd3dtexture;
                } else {
                    UpdateConvertedSurface(ppf, size, m_datasConverted.Get(index), m_datas[index]);
                    return m_datasConverted[index].m_pd3dtexture;
                }
            }

            index++;
        }
    }

    TRef<IDirectDrawSurface> GetDDS()
    {
        SetSurfaceMode(SurfaceModeDD);

        TRef<IDirectDrawSurface> pdds;
        DDCall(m_data.m_pdds->QueryInterface(IID_IDirectDrawSurface, (void**)&pdds));
        return pdds;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface Type
    //
    //////////////////////////////////////////////////////////////////////////////

    bool Is3D()
    {
        return m_stype.Test(SurfaceType3D());
    }

    bool HasZBuffer()
    {
        return m_stype.Test(SurfaceTypeZBuffer());
    }

    bool InVideoMemory()
    {
        return m_stype.Test(SurfaceTypeVideo()) != 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    PixelFormat* GetPixelFormat()
    {
        return m_data.m_ppf;
    }

    PrivatePalette* GetPalette()
    {
        return m_ppalette;
    }

    DDDevice* GetDDDevice()
    {
        return m_pdddevice;
    }

    Engine* GetEngine()
    {
        return m_pengine;
    }

    PrivateEngine* GetEngineImpl()
    {
        return m_pengine;
    }

    const WinPoint& GetSize()
    {
        return m_size;
    }

    SurfaceType GetSurfaceType()
    {
        return m_stype;
    }

    bool HasColorKey()
    {
        return m_bColorKey;
    }

    const Color& GetColorKey()
    {
        ZAssert(m_bColorKey);
        return m_colorKey;
    }

    void SetColorKey(const Color& color)
    {
        ZAssert(m_datas.GetCount() == 0);
        ZAssert(m_datasConverted.GetCount() == 0);

        m_bColorKey = true;
        m_colorKey = color;

        //
        // Update the ddraw surface
        //

        SetSurfaceMode(SurfaceModeDD);
        DDCOLORKEY key;
        key.dwColorSpaceLowValue  =
        key.dwColorSpaceHighValue = m_data.m_ppf->MakePixel(m_colorKey).Value();

        DDCall(m_data.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));

        //
        // Update the converted ddraw surface
        //

        if (m_dataConverted.m_pdds) {
            DDCall(m_dataConverted.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));
        }

        SurfaceChanged();
    }

    int GetPitch()
    {
        return m_pitch;
    }

    BYTE* GetPointer()
    {
        if (m_pbits != NULL) {
            return m_pbits;
        } else {
            SetSurfaceMode(SurfaceModeLocked);

            BYTE* pbits = m_ddsdLocked.Pointer();

            if (pbits == NULL) {
                //
                // DDraw returned null.  Allocate some memory so the caller won't crash.
                //

                m_pbits = new BYTE[m_pitch * m_size.Y()];
                return m_pbits;
            }

            return pbits;
        }
    }

    void ReleasePointer()
    {
        SetSurfaceMode(SurfaceModeDD);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface modes
    //
    //////////////////////////////////////////////////////////////////////////////

    void BeginScene()
    {
        SetSurfaceMode(SurfaceModeDD);
    }

    void EndScene()
    {
    }

    void SetSurfaceMode(SurfaceMode mode)
    {
        if (m_mode != mode) {
            //
            // switch back to DDMode
            //

            switch (m_mode) {
                case SurfaceModeLocked:
                    if (m_pbits == NULL) {
                        DDSCall(m_data.m_pdds->Unlock(NULL));
                    }
                    SurfaceChanged();
                    break;
            }

            //
            // switch to the new mode
            //

            m_mode = mode;

            switch (mode) {
                case SurfaceModeLocked:
                    DDSCall(m_data.m_pdds->Lock(NULL, &m_ddsdLocked, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
                    break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Blts to and from a dc
    //
    //////////////////////////////////////////////////////////////////////////////

    void BitBltFromDC(HDC hdc)
    {
        SetSurfaceMode(SurfaceModeDD);
        HDC hdcSurface;
        if (DDSCall(m_data.m_pdds->GetDC(&hdcSurface))) {
            ZVerify(::BitBlt(
                hdcSurface,
                0,
                0,
                m_size.X(),
                m_size.Y(),
                hdc,
                0,
                0,
                SRCCOPY
            ));

            DDSCall(m_data.m_pdds->ReleaseDC(hdcSurface));
        }

        SurfaceChanged();
        SetSurfaceMode(SurfaceModeDD);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // StretchBlt
    //
    //////////////////////////////////////////////////////////////////////////////

    void UnclippedBlt(
        const WinRect& rectTarget,
        VideoSurface*  psurfaceSource,
        const WinRect& rectSource,
        bool           bColorKey
    ) {
        SetSurfaceMode(SurfaceModeDD);
        DWORD flags = bColorKey ? DDBLT_KEYSRC : 0;
        DDSurface* pddsurface; CastTo(pddsurface, psurfaceSource);

        DDSCall(m_data.m_pdds->Blt(
            (RECT*)&rectTarget, 
            pddsurface->GetDDSX(),
            (RECT*)&rectSource, 
            flags | DDBLT_WAIT, 
            NULL
        ));
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // Blt
    //
    //////////////////////////////////////////////////////////////////////////////

    void UnclippedBlt(
        const WinRect&       rect, 
        IDirectDrawSurfaceX* pddsSource, 
        const WinPoint&      pointSource, 
        bool                 bColorKey
    ) {
        SetSurfaceMode(SurfaceModeDD);
        DWORD flags = bColorKey ? DDBLT_KEYSRC : 0;
        WinRect rectSource(pointSource, pointSource + rect.Size());

        DDSCall(m_data.m_pdds->Blt(
            (RECT*)&rect, 
            pddsSource, 
            (RECT*)&rectSource, 
            flags | DDBLT_WAIT, 
            NULL
        ));
    }

    void UnclippedBlt(const WinRect& rect, VideoSurface* psurfaceSource, const WinPoint& pointSource)
    {
        DDSurface* pddsurface; CastTo(pddsurface, psurfaceSource);

        UnclippedBlt(rect, pddsurface->GetDDSX(), pointSource, pddsurface->HasColorKey());
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Fill
    //
    //////////////////////////////////////////////////////////////////////////////

    void UnclippedFill(const WinRect& rect, Pixel pixel)
    {
        DDBltFX ddbltfx;
        ddbltfx.dwFillColor = pixel.Value();

        DDSCall(m_data.m_pdds->Blt(
            (RECT*)&rect, 
            NULL, 
            (RECT*)&rect, 
            DDBLT_COLORFILL | DDBLT_WAIT, 
            &ddbltfx
        ));
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructors
//
//////////////////////////////////////////////////////////////////////////////

TRef<DDSurface> CreateDDSurface(
          DDDevice*       pdddevice,
          SurfaceType     stype,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size
) {
    TRef<DDSurface> pddsurface = new DDSurfaceImpl(pdddevice, stype, ppf, ppalette, size);

    if (pddsurface->IsValid()) {
        return pddsurface;
    } else {
        return NULL;
    }
}

TRef<DDSurface> CreateDDSurface(
          DDDevice*       pdddevice,
          SurfaceType     stype,
          PixelFormat*    ppf,
          PrivatePalette* ppalette,
    const WinPoint&       size,
          int             pitch,
          BYTE*           pbits
) {
    return new DDSurfaceImpl(pdddevice, stype, ppf, ppalette, size, pitch, pbits);
}

TRef<DDSurface> CreateDDSurface(
    DDDevice*            pdddevice,
    IDirectDrawSurfaceX* pdds,
    IDirectDrawSurfaceX* pddsZBuffer,
    PixelFormat*         ppf,
    PrivatePalette*      ppalette,
    SurfaceType          stype
) {
    return
        new DDSurfaceImpl(
            pdddevice,
            pdds,
            pddsZBuffer,
            ppf,
            ppalette,
            stype
        );
}
