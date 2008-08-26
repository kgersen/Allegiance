#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// D3DDevice
//
//////////////////////////////////////////////////////////////////////////////

class D3DDeviceImpl : public D3DDevice {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<DDDevice>         m_pdddevice;
    TRef<IDirect3DDeviceX> m_pd3dd;

    TRef<PixelFormat>      m_ppfPrefered;
    TRef<PixelFormat>      m_ppfTexture;
    bool                   m_bHardwareAccelerated;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Texture Pixel Format Enumeration
    //
    //////////////////////////////////////////////////////////////////////////////

    static HRESULT WINAPI StaticEnumTextures(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext)
    {
        D3DDeviceImpl* pthis = (D3DDeviceImpl*)lpContext;
        return pthis->EnumTextures(*(DDPixelFormat*)lpDDPixFmt);
    }

    HRESULT EnumTextures(const DDPixelFormat& ddpf)
    {
        TRef<PixelFormat> ppf = m_pdddevice->GetEngine()->GetPixelFormat(ddpf);

        if (
               ppf->ValidGDIFormat()
            && ppf->PixelBits() >= 16
        ) {
            if (
                   (m_ppfTexture == NULL)
                || (ppf == m_ppfPrefered)
                || (ddpf.dwRGBBitCount < m_ppfTexture->PixelBits())
            ) {
                m_ppfTexture = ppf;
            }
        }

        return DDENUMRET_OK;
    }

public:

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    D3DDeviceImpl(
        DDDevice*        pdddevice,
        IDirect3DDeviceX* pd3dd,
        bool              bHardwareAccelerated,
        PixelFormat*      ppf
    ) :
        m_pdddevice(pdddevice),
        m_pd3dd(pd3dd),
        m_bHardwareAccelerated(bHardwareAccelerated),
        m_ppfPrefered(ppf)
    {
        //
        // Find a 16 bit Texture Format
        //

        m_pd3dd->EnumTextureFormats(StaticEnumTextures, (void*)this);
    }

    bool IsValid()
    {
        return m_ppfTexture != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~D3DDeviceImpl()
    {
        m_pdddevice->RemoveD3DDevice(this);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
        m_pd3dd = NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // D3DDevice Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsHardwareAccelerated()
    {
        return m_bHardwareAccelerated;
    }

    PixelFormat* GetTextureFormat()
    {
        return m_ppfTexture;
    }

    IDirect3DDeviceX* GetD3DDeviceX()
    {
        return m_pd3dd;
    }

    WinPoint GetMinTextureSize()
    {
        return WinPoint(1, 1);
        //return WinPoint(m_pd3ddd->dwMinTextureWidth, m_pd3ddd->dwMinTextureHeight);
    }

    WinPoint GetMaxTextureSize(DWORD dwMaxTextureSize)
    {
		// yp Your_Persona August 2 2006 : MaxTextureSize Patch
		int i = pow(2.0f,(float)(8+dwMaxTextureSize)); 
		return WinPoint(i, i);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<D3DDevice> CreateD3DDevice(
    DDDevice*        pdddevice,
    IDirect3DDeviceX* pd3dd,
    bool              bHardwareAccelerated,
    PixelFormat*      ppfPrefered
) {
    TRef<D3DDevice> pd3ddevice = 
        new D3DDeviceImpl(
            pdddevice, 
            pd3dd, 
            bHardwareAccelerated, 
            ppfPrefered
        );

    if (pd3ddevice->IsValid()) {
        return pd3ddevice;
    } else {
        return NULL;
    }
}
