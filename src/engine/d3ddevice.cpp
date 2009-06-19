#include "pch.h"




// REPLACE THIS WITH PROPER MODE CHECKING.
WinPoint g_ValidD3DModes[] =
{
    WinPoint( 640,  480),
    WinPoint( 800,  600),
    WinPoint(1024,  768),
    WinPoint(1280, 1024),
    WinPoint(1600, 1200)
};

const int g_CountValidD3DModes = sizeof(g_ValidD3DModes) / sizeof(g_ValidD3DModes[0]);

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

//    TRef<IDirect3DX>		m_pD3D;
//    TRef<IDirect3DDeviceX>	m_pD3DDevice;

    TRef<PixelFormat>		m_ppfPrefered;
    TRef<PixelFormat>		m_ppfTexture;
    bool					m_bHardwareAccelerated;
	D3DFORMAT				m_zbufFormat;
    TVector<WinPoint>		m_modes;					// To be replaced.

    //////////////////////////////////////////////////////////////////////////////
    //
    // Texture Pixel Format Enumeration
    //
    //////////////////////////////////////////////////////////////////////////////

/*    static HRESULT WINAPI StaticEnumTextures(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext)
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
    }*/

public:

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    D3DDeviceImpl( HWND hParentWindow )
    {
        // Find a 16 bit Texture Format
//		_ASSERT( false );
//        m_pd3dd->EnumTextureFormats(StaticEnumTextures, (void*)this);

		// Obtain the D3D interface.
/*		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if( m_pD3D == NULL )
		{
			_ASSERT( false );
			return;						// This error is picked up by the caller.
		}

        // Enumerate the display modes
        EnumerateDisplayModes( );

		// TBD: Enumerate the zbuffer formats properly!
		EnumerateZBufferModes( );*/

		_ASSERT( false );

		// 
		m_ppfTexture = new PixelFormat(16, 0xf800, 0x07e0, 0x001f, 0x0000);			// Left over from old code.
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
//        m_pdddevice->RemoveD3DDevice(this);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
		// Proper clean up required.
		_ASSERT( false );

//		m_pD3DDevice->Release();
//		m_pD3D->Release();

//		m_pD3DDevice	= NULL;
//		m_pD3D			= NULL;
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

/*    IDirect3DDeviceX* GetD3DDeviceX()
    {
        return m_pD3DDevice;
    }*/

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

/*	//////////////////////////////////////////////////////////////////////////////
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
		DWORD				dwModeCount = m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, desiredFormat );
		D3DDISPLAYMODE		dispMode;
		WinPoint			winSize;

		for( i=0; i<dwModeCount; i++ )
		{
			if( m_pD3D->EnumAdapterModes(	D3DADAPTER_DEFAULT,
											desiredFormat,
											i,
											&dispMode ) != D3D_OK )
			{
				OutputDebugString( "Failed to enumerate adapter modes.\n");
				_ASSERT( false );
				return E_FAIL;
			}
			
			for( int iIndex=0; iIndex<g_CountValidD3DModes; iIndex++ )
			{
				if( ( dispMode.Width == g_ValidD3DModes[iIndex].X() ) &&
					( dispMode.Height == g_ValidD3DModes[iIndex].Y() ) )
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
		HRESULT hRes = m_pD3D->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
													D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

		if( hRes != D3D_OK )
		{
			_ASSERT( hRes == D3DERR_NOTAVAILABLE );

			m_zbufFormat = D3DFMT_D24S8;
			HRESULT hRes = m_pD3D->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
														D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

			if( hRes != D3D_OK )
			{
				_ASSERT( hRes == D3DERR_NOTAVAILABLE );

				m_zbufFormat = D3DFMT_D24X8;
				HRESULT hRes = m_pD3D->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
															D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, m_zbufFormat );

				if( hRes != D3D_OK )
				{
					_ASSERT( hRes == D3DERR_NOTAVAILABLE );

					m_zbufFormat = D3DFMT_D16;
					HRESULT hRes = m_pD3D->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5,
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
	}*/

};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<D3DDevice> CreateD3DDevice( HWND hParentWindow ) 
{
    TRef<D3DDevice> pD3DDevice = new D3DDeviceImpl( hParentWindow );
	
	if( pD3DDevice->IsValid() )
	{
        return pD3DDevice;
    } 
	else 
	{
        return NULL;
    }
}
