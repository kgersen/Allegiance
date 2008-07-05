

////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC STORAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////
CD3DDevice9::SD3DDevice9State		CD3DDevice9::sD3DDev9 = { false };
CD3DDevice9::SD3DDeviceSetupParams	CD3DDevice9::sDevSetupParams;

#ifdef EnablePerformanceCounters
int CD3DDevice9::m_pPerformanceCounters[ eD9S_NumStatTypes ];
#endif // EnablePerformanceCounters

////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialise()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::Initialise( CLogFile * pLogFile )
{
	_ASSERT( sD3DDev9.bInitialised == false );

	// Should only be called once, but on the off chance it isn't, we don't want to destroy
	// valid interfaces.
	if( sD3DDev9.bInitialised == false )
	{
		memset( &sD3DDev9, 0, sizeof( CD3DDevice9::SD3DDevice9State ) );
		sD3DDev9.bInitialised = true;
	}

	pLogFile->OutputString( "CD3DDevice9 initialised.\n" );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::Shutdown( )
{
	if( sD3DDev9.bInitialised == true )
	{
		// Clear out a bunch of state. Anything assigned to the device will keep a reference
		// until released. D3D9 won't shut down properly until all references released.
		ResetReferencedResources( );
		
		CVBIBManager::Shutdown( );
		CVRAMManager::Shutdown( );

		if( sD3DDev9.pBackBufferDepthStencilSurface != NULL )
		{
			sD3DDev9.pBackBufferDepthStencilSurface->Release();
			sD3DDev9.pBackBufferDepthStencilSurface = NULL;
		}
		if( sD3DDev9.pRTDepthStencilSurface != NULL )
		{
			sD3DDev9.pRTDepthStencilSurface->Release();
			sD3DDev9.pRTDepthStencilSurface = NULL;
		}

		sD3DDev9.pD3DDevice->Release();
		sD3DDev9.pD3DDevice = NULL;
		sD3DDev9.pD3D9->Release( );
		sD3DDev9.pD3D9 = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// ResetReferencedResources()
// Clear out a bunch of state. Anything assigned to the device will keep a reference
// until released. D3D9 won't shut down properly until all references released.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::ResetReferencedResources( )
{
	if( sD3DDev9.bInitialised == true )
	{
		// Clear out a bunch of state. Anything assigned to the device will keep a reference
		// until released. D3D9 won't shut down properly until all references released.
		sD3DDev9.pD3DDevice->SetFVF( 0 );
		sD3DDev9.pD3DDevice->SetIndices( NULL );
		sD3DDev9.pD3DDevice->SetStreamSource( 0, NULL, 0, 0 );
		sD3DDev9.pD3DDevice->SetPixelShader( NULL );
		sD3DDev9.pD3DDevice->SetVertexDeclaration( NULL );
		sD3DDev9.pD3DDevice->SetVertexShader( NULL );
		sD3DDev9.pD3DDevice->EvictManagedResources( );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseD3D9()
// Initialise the D3D9 interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::CreateD3D9( CLogFile * pLogFile )
{
	// Create the D3D9 interface.
	sD3DDev9.pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );

	_ASSERT( sD3DDev9.pD3D9 != NULL );
	if( sD3DDev9.pD3D9 == NULL )
	{
		pLogFile->OutputString( "Direct3DCreate9 failed.\n" );
		return E_FAIL;
	}

	pLogFile->OutputString( "Direct3DCreate9 succeeded.\n" );
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseDevice()
// Create a new D3D9 device, given a window.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::CreateDevice( HWND hParentWindow, CLogFile * pLogFile )
{
	DWORD dwCreationFlags;
	HRESULT hr;

	if( sDevSetupParams.bRunWindowed == true )
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sWindowedMode;
	}
	else
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sFullScreenMode;
	}

	// Create a new 3D device.
	memset( &sD3DDev9.d3dPresParams, 0, sizeof( D3DPRESENT_PARAMETERS ) );
	sD3DDev9.d3dPresParams.AutoDepthStencilFormat		= sD3DDev9.pCurrentMode->fmtDepthStencil;
	sD3DDev9.d3dPresParams.EnableAutoDepthStencil		= TRUE;
	sD3DDev9.d3dPresParams.BackBufferCount				= 1;
	sD3DDev9.d3dPresParams.hDeviceWindow				= hParentWindow;
	sD3DDev9.d3dPresParams.Windowed						= sDevSetupParams.bRunWindowed;
	sD3DDev9.d3dPresParams.BackBufferFormat				= sD3DDev9.pCurrentMode->mode.Format;
	sD3DDev9.d3dPresParams.BackBufferWidth				= sD3DDev9.pCurrentMode->mode.Width;
	sD3DDev9.d3dPresParams.BackBufferHeight				= sD3DDev9.pCurrentMode->mode.Height;
	sD3DDev9.d3dPresParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;

	if( sDevSetupParams.bRunWindowed == true )
	{
		sD3DDev9.d3dPresParams.FullScreen_RefreshRateInHz	= 0;
	}
	else
	{
		sD3DDev9.d3dPresParams.FullScreen_RefreshRateInHz	= sD3DDev9.pCurrentMode->mode.RefreshRate;
	}

	if( sDevSetupParams.bWaitForVSync == true )
	{
		sD3DDev9.d3dPresParams.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{
		sD3DDev9.d3dPresParams.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	sD3DDev9.d3dPresParams.MultiSampleQuality			= D3DMULTISAMPLE_NONE;
	sD3DDev9.d3dPresParams.MultiSampleType				= sD3DDev9.pCurrentMode->d3dMultiSampleSetting;
	sD3DDev9.d3dPresParams.Flags						= 0;

	dwCreationFlags = D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;

	hr = sD3DDev9.pD3D9->CreateDevice(	sDevSetupParams.iAdapterID,
											D3DDEVTYPE_HAL,
											hParentWindow,
											dwCreationFlags,
											&sD3DDev9.d3dPresParams,
											&sD3DDev9.pD3DDevice );

	// Imago comment out the D3DERR_NOTAVAILABLE checks...
	// Did we create a valid device?
	if( hr != D3D_OK )
	{
		pLogFile->OutputStringV( "Pure HWVP device creation failed: 0x%08x.\n", hr );
		//if( hr == D3DERR_NOTAVAILABLE )
		//{
			// No, try a non-pure device.
			dwCreationFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			hr = sD3DDev9.pD3D9->CreateDevice(	sDevSetupParams.iAdapterID,
												D3DDEVTYPE_HAL,
												hParentWindow,
												dwCreationFlags,
												&sD3DDev9.d3dPresParams,
												&sD3DDev9.pD3DDevice );
			if( hr != D3D_OK )
			{
				pLogFile->OutputStringV( "HWVP device creation failed: 0x%08x.\n", hr );
				// Still no joy, try a software vp device.
				//if( hr == D3DERR_NOTAVAILABLE )
				//{
					dwCreationFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
					hr = sD3DDev9.pD3D9->CreateDevice(	sDevSetupParams.iAdapterID,
														D3DDEVTYPE_HAL,
														hParentWindow,
														dwCreationFlags,
														&sD3DDev9.d3dPresParams,
														&sD3DDev9.pD3DDevice );
					if( hr == D3D_OK )
					{
						sD3DDev9.bHardwareVP = false;
						sD3DDev9.bPureDevice = false;
						pLogFile->OutputString( "SWVP device created.\n" );
					}
					else
					{
						pLogFile->OutputStringV( "SWVP device creation failed: 0x%08x.\n", hr );
					}
				//}
			}
			else
			{
				sD3DDev9.bHardwareVP = true;
				sD3DDev9.bPureDevice = false;
				pLogFile->OutputString( "HWVP device created.\n" );
			}
		//}
	}
	else
	{
		// Valid pure hw device.
		sD3DDev9.bHardwareVP = true;
		sD3DDev9.bPureDevice = true;

		pLogFile->OutputString( "Pure HWVP device created.\n" );
	}

	if( hr != D3D_OK )
	{
		return hr;
	}
 
	if( hr == D3D_OK )
	{
		hr = sD3DDev9.pD3DDevice->GetDeviceCaps( &sD3DDev9.sD3DDevCaps );
		_ASSERT( hr == D3D_OK );
		hr = sD3DDev9.pD3D9->GetAdapterIdentifier(	sDevSetupParams.iAdapterID,
													0,
													&sD3DDev9.d3dAdapterID );
		_ASSERT( hr == D3D_OK );
	}

	// Recreate the AA depth stencil buffer, if required.
	CreateAADepthStencilBuffer();

	ClearScreen();
	RenderFinished();
	ClearScreen();
	RenderFinished();

	// Store state.
	sD3DDev9.bIsWindowed = sDevSetupParams.bRunWindowed;

	// Imago don't use this it breaks D3DCOLOR
	sD3DDev9.sFormatFlags.bSupportsA1R5G6B6Format = false;

	// Auto gen mipmaps flag - include user setting.
	if( ( ( sD3DDev9.sD3DDevCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP ) != 0 ) &&
		( sDevSetupParams.bAutoGenMipmap == true ) )
	{
		pLogFile->OutputString( "Device can auto generate mipmaps.\n" );
		sD3DDev9.sFormatFlags.bCanAutoGenMipMaps = true;
	}

	// Initialise the caches.
	InitialiseDeviceStateCache( &sD3DDev9.sDeviceStateCache );
	InitialiseTransformCache( &sD3DDev9.sTransformCache );
	InitialiseMaterialCache( &sD3DDev9.sMaterialCache );
	InitialiseLightCache( &sD3DDev9.sLightCache );

	// Setup stats.
#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[eD9S_InitialTexMem] = (int)sD3DDev9.pD3DDevice->GetAvailableTextureMem();

	if( sD3DDev9.bPureDevice == true )
	{
		sprintf_s( sDevSetupParams.szDevType, 64, "Pure HWVP" );
	}
	else if( sD3DDev9.bHardwareVP == true )
	{
		sprintf_s( sDevSetupParams.szDevType, 64, "HWVP" );
	}
	else
	{
		sprintf_s( sDevSetupParams.szDevType, 64, "SWVP" );
	}

	sprintf_s( sD3DDev9.pszDevSetupString, 256,
		"%s [%s, %s]", sD3DDev9.d3dAdapterID.Description,
		sDevSetupParams.szDevType,
		sDevSetupParams.szAAType );
#endif // EnablePerformanceCounters

	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateCurrentMode()
// Called after initial settings have been made. Sets up the current pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::UpdateCurrentMode( )
{
	if( sDevSetupParams.bRunWindowed == true )
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sWindowedMode;
	}
	else
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sFullScreenMode;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateAADepthStencilBuffer()
// If needed create the additional non-aa depthstencil buffer for render targets.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::CreateAADepthStencilBuffer()
{
	if( ( sDevSetupParams.bAntiAliased == true ) &&
		( sD3DDev9.d3dPresParams.MultiSampleType != D3DMULTISAMPLE_NONE ) )
	{
		// Store the back buffer depth stencil surface.
		HRESULT hr = sD3DDev9.pD3DDevice->GetDepthStencilSurface( &sD3DDev9.pBackBufferDepthStencilSurface );
		_ASSERT( hr == D3D_OK );
		
		// If these fire, need to look at how we create these.
		_ASSERT( sDevSetupParams.sFullScreenMode.mode.Width >= sDevSetupParams.sWindowedMode.mode.Width );
		_ASSERT( sDevSetupParams.sFullScreenMode.mode.Height >= sDevSetupParams.sWindowedMode.mode.Height );
		_ASSERT( sDevSetupParams.sFullScreenMode.fmtDepthStencil == sDevSetupParams.sWindowedMode.fmtDepthStencil );

		// Create the additional render target depthstencil surface.
		hr = sD3DDev9.pD3DDevice->CreateDepthStencilSurface(
			sDevSetupParams.sFullScreenMode.mode.Width,
			sDevSetupParams.sFullScreenMode.mode.Height,
			sDevSetupParams.sFullScreenMode.fmtDepthStencil,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&sD3DDev9.pRTDepthStencilSurface,
			NULL );
		_ASSERT( hr == D3D_OK );
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ResetDevice()
// Reset the device, switching from full screen to windowed.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::ResetDevice(	bool	bWindowed, 
									DWORD	dwWidth /*=0*/, 
									DWORD	dwHeight /*=0*/ )
{
	HRESULT hr				= D3D_OK;
	bool bResetRequired		= false;

	if( bWindowed == true )
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sWindowedMode;
	}
	else
	{
		sD3DDev9.pCurrentMode = &sDevSetupParams.sFullScreenMode;
	}

	if( (BOOL)bWindowed != sD3DDev9.d3dPresParams.Windowed )
	{
		sD3DDev9.d3dPresParams.Windowed = bWindowed;
		sD3DDev9.bIsWindowed = bWindowed;
		bResetRequired = true;
	}

	// If both zero, we want to force a reset. Used to recover from lost devices.
	if( ( dwWidth == 0 ) || ( dwHeight == 0 ) )
	{
		bResetRequired = true;
		if( bWindowed == true )
		{
			_ASSERT( ( sD3DDev9.dwCurrentWindowedWidth != 0 ) && ( sD3DDev9.dwCurrentWindowedHeight != 0 ) );
			dwWidth = sD3DDev9.dwCurrentWindowedWidth;
			dwHeight = sD3DDev9.dwCurrentWindowedHeight; 
		}
		else
		{
			_ASSERT( ( sD3DDev9.dwCurrentFullscreenWidth != 0 ) && ( sD3DDev9.dwCurrentFullscreenHeight != 0 ) );
			dwWidth = sD3DDev9.dwCurrentFullscreenWidth;
			dwHeight = sD3DDev9.dwCurrentFullscreenHeight;
		}
	}

	// Configure the back buffer width setting.
	if( bWindowed == true )
	{
		if( sD3DDev9.d3dPresParams.BackBufferWidth != dwWidth )
		{
			sD3DDev9.d3dPresParams.BackBufferWidth = dwWidth;
			sD3DDev9.d3dPresParams.BackBufferHeight = dwHeight;
			bResetRequired = true;
		}
		sD3DDev9.d3dPresParams.FullScreen_RefreshRateInHz = 0;		// Must set to 0 for windowed.
		sD3DDev9.dwCurrentWindowedWidth = dwWidth;
		sD3DDev9.dwCurrentWindowedHeight = dwHeight;
	}
	else
	{
		if( sD3DDev9.d3dPresParams.BackBufferWidth != dwWidth )
		{
			sD3DDev9.d3dPresParams.BackBufferWidth = dwWidth;
			sD3DDev9.d3dPresParams.BackBufferHeight = dwHeight;
			sD3DDev9.d3dPresParams.FullScreen_RefreshRateInHz = sD3DDev9.pCurrentMode->mode.RefreshRate;
			bResetRequired = true;
		}
		sD3DDev9.dwCurrentFullscreenWidth = dwWidth;
		sD3DDev9.dwCurrentFullscreenHeight = dwHeight;
	}

	debugf( "CD3DDevice9: switching to %s, size %d x %d\n", + bWindowed ? "windowed" : "fullscreen", 
				sD3DDev9.d3dPresParams.BackBufferWidth, 
				sD3DDev9.d3dPresParams.BackBufferHeight );

	if( bResetRequired == true )
	{
		// Prepare the d3dPresParams.
		sD3DDev9.d3dPresParams.BackBufferFormat	= sD3DDev9.pCurrentMode->mode.Format;
		sD3DDev9.d3dPresParams.MultiSampleType = sD3DDev9.pCurrentMode->d3dMultiSampleSetting;
		sD3DDev9.d3dPresParams.AutoDepthStencilFormat = sD3DDev9.pCurrentMode->fmtDepthStencil;

		ResetReferencedResources( );

		// Currently, render targets and dynamic buffers reside in the the default pool.
		CVRAMManager::EvictDefaultPoolResources( );
		CVBIBManager::EvictDefaultPoolResources( );

		if( sDevSetupParams.bAntiAliased == true )
		{
			sD3DDev9.pRTDepthStencilSurface->Release();
			sD3DDev9.pRTDepthStencilSurface = NULL;

			// We also stored a pointer to the main back buffer.
			sD3DDev9.pBackBufferDepthStencilSurface->Release();
			sD3DDev9.pBackBufferDepthStencilSurface = NULL;
		}

		// Perform the reset.
		hr = sD3DDev9.pD3DDevice->Reset( &sD3DDev9.d3dPresParams );
		_ASSERT( hr == D3D_OK );

		// Initialise the caches.
		InitialiseDeviceStateCache( &sD3DDev9.sDeviceStateCache );
		InitialiseTransformCache( &sD3DDev9.sTransformCache );
		InitialiseMaterialCache( &sD3DDev9.sMaterialCache );
		InitialiseLightCache( &sD3DDev9.sLightCache );

		// Recreate the AA depth stencil buffer, if required.
		CreateAADepthStencilBuffer( );
	}

	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ClearScreen()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::ClearScreen( )
{
	HRESULT hr = D3D_OK;

	hr = sD3DDev9.pD3DDevice->Clear(	0, 
										NULL, 
										D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
										0xFF000000,
										1.0f,
										0 );
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// TestCooperativeLevel()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::TestCooperativeLevel( )
{
	return sD3DDev9.pD3DDevice->TestCooperativeLevel();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderFinished()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::RenderFinished( )
{
	HRESULT hr;
	hr = sD3DDev9.pD3DDevice->Present( NULL, NULL, NULL, NULL );
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetCurrentResolution()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
WinPoint CD3DDevice9::GetCurrentResolution( )
{
	return WinPoint( sD3DDev9.d3dPresParams.BackBufferWidth, sD3DDev9.d3dPresParams.BackBufferHeight );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// IsDeviceValid()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DDevice9::IsDeviceValid( )
{
	return sD3DDev9.bInitialised && ( sD3DDev9.pD3DDevice != NULL );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// IsInScene()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DDevice9::IsInScene( )
{
	return sD3DDev9.bInScene;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// IsWindowed()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CD3DDevice9::IsWindowed( )
{
	return sD3DDev9.bIsWindowed;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetBackBufferDepthStencil()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::SetBackBufferDepthStencil( )
{
	_ASSERT( sDevSetupParams.bAntiAliased == true );
	_ASSERT( sD3DDev9.pBackBufferDepthStencilSurface != NULL );

	return sD3DDev9.pD3DDevice->SetDepthStencilSurface( sD3DDev9.pBackBufferDepthStencilSurface );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetRTDepthStencil()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::SetRTDepthStencil( )
{
	_ASSERT( sDevSetupParams.bAntiAliased == true );
	_ASSERT( sD3DDev9.pRTDepthStencilSurface != NULL );

	return sD3DDev9.pD3DDevice->SetDepthStencilSurface( sD3DDev9.pRTDepthStencilSurface);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// STATE CACHING ROUTINES
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseDeviceStateCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseDeviceStateCache( SD3D9DeviceStateCache * pCache )
{
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

	DWORD i;

	// Reset the render states.
	InitialiseRenderStateCache( pCache->pRenderState );

	// Reset the texture stage states.
	for( i=0; i<dwMaxTextureStages; i++ )
	{
		InitialiseTextureStageCache( i, pCache->pTextureStageStates[i] );
	}

	// Reset the sampler states.
	pCache->dwNumSamplers = sD3DDev9.sD3DDevCaps.MaxSimultaneousTextures;
	pCache->pSamplerState = new DWORD* [ pCache->dwNumSamplers ];

	for( i=0; i<pCache->dwNumSamplers; i++ )
	{
		pCache->pSamplerState[i] = new DWORD[ dwMaxSamplerStatesAllowed ];
		InitialiseSamplerStateCache( i, pCache->pSamplerState[i] );
	}

	// Reset remaining cached values.
	pCache->dwFVF = 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
#define RESET_STATE_CACHE_VALUE( state, value ) pRenderStateCache[state] = value;\
	sD3DDev9.pD3DDevice->SetRenderState( state, value );

////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseRenderStateCache()
// Clear out a state cache and reset to defaults.
// Default values taken from Oct '06 SDK documentation.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseRenderStateCache( DWORD * pRenderStateCache )
{
	float fOne = 1.0f;
	float fSixtyFour = 64.0f;
	memset( pRenderStateCache, 0xFF, dwMaxRenderStatesAllowed * sizeof( DWORD ) );

	RESET_STATE_CACHE_VALUE( D3DRS_ZENABLE, D3DZB_TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_FILLMODE, D3DFILL_SOLID );
	RESET_STATE_CACHE_VALUE( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	RESET_STATE_CACHE_VALUE( D3DRS_ZWRITEENABLE, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_ALPHATESTENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_LASTPIXEL, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_SRCBLEND, D3DBLEND_ONE );
	RESET_STATE_CACHE_VALUE( D3DRS_DESTBLEND, D3DBLEND_ZERO );
	RESET_STATE_CACHE_VALUE( D3DRS_CULLMODE, D3DCULL_CCW );
	RESET_STATE_CACHE_VALUE( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	RESET_STATE_CACHE_VALUE( D3DRS_ALPHAREF, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_ALPHAFUNC, D3DCMP_ALWAYS );
	RESET_STATE_CACHE_VALUE( D3DRS_DITHERENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_ALPHABLENDENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_SPECULARENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGCOLOR, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGTABLEMODE, D3DFOG_NONE );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGSTART, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGEND, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_FOGDENSITY, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_RANGEFOGENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILREF, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILMASK, 0xFFFFFFFF );
	RESET_STATE_CACHE_VALUE( D3DRS_STENCILWRITEMASK, 0xFFFFFFFF );
	RESET_STATE_CACHE_VALUE( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP0, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP1, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP2, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP3, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP4, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP5, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP6, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP7, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_CLIPPING, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_LIGHTING, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_AMBIENT, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_FOGVERTEXMODE, D3DFOG_NONE );
	RESET_STATE_CACHE_VALUE( D3DRS_COLORVERTEX, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_LOCALVIEWER, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_NORMALIZENORMALS, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	RESET_STATE_CACHE_VALUE( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2 );
	RESET_STATE_CACHE_VALUE( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL );
	RESET_STATE_CACHE_VALUE( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL );
	RESET_STATE_CACHE_VALUE( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
	RESET_STATE_CACHE_VALUE( D3DRS_CLIPPLANEENABLE, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSIZE, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSIZE_MIN, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSPRITEENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSCALEENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSCALE_A, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSCALE_B, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSCALE_C, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	RESET_STATE_CACHE_VALUE( D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF );
	RESET_STATE_CACHE_VALUE( D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE );
	RESET_STATE_CACHE_VALUE( D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE );
	RESET_STATE_CACHE_VALUE( D3DRS_POINTSIZE_MAX, *((DWORD*) (&fSixtyFour)));
	RESET_STATE_CACHE_VALUE( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_COLORWRITEENABLE, 0x0000000F );
	RESET_STATE_CACHE_VALUE( D3DRS_TWEENFACTOR, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	RESET_STATE_CACHE_VALUE( D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC );
	RESET_STATE_CACHE_VALUE( D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR );
	RESET_STATE_CACHE_VALUE( D3DRS_SCISSORTESTENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_SLOPESCALEDEPTHBIAS, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_MINTESSELLATIONLEVEL, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_MAXTESSELLATIONLEVEL, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_ADAPTIVETESS_X, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_ADAPTIVETESS_Y, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_ADAPTIVETESS_Z, *((DWORD*) (&fOne)));
	RESET_STATE_CACHE_VALUE( D3DRS_ADAPTIVETESS_W, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_ENABLEADAPTIVETESSELLATION, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_TWOSIDEDSTENCILMODE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP );
	RESET_STATE_CACHE_VALUE( D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS );
	RESET_STATE_CACHE_VALUE( D3DRS_COLORWRITEENABLE1, 0x0000000F );
	RESET_STATE_CACHE_VALUE( D3DRS_COLORWRITEENABLE2, 0x0000000F );
	RESET_STATE_CACHE_VALUE( D3DRS_COLORWRITEENABLE3, 0x0000000F );
	RESET_STATE_CACHE_VALUE( D3DRS_BLENDFACTOR, 0xFFFFFFFF );
	RESET_STATE_CACHE_VALUE( D3DRS_SRGBWRITEENABLE, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_DEPTHBIAS, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP8, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP9, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP10, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP11, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP12, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP13, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP14, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_WRAP15, 0 );
	RESET_STATE_CACHE_VALUE( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
	RESET_STATE_CACHE_VALUE( D3DRS_SRCBLENDALPHA, D3DBLEND_ONE );
	RESET_STATE_CACHE_VALUE( D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO );
	RESET_STATE_CACHE_VALUE( D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
#define RESET_TEXTURE_STATE_CACHE_VALUE( stage, state, value ) pTextureStateCache[state] = value;\
	sD3DDev9.pD3DDevice->SetTextureStageState( stage, state, value );

////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseTextureStageCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseTextureStageCache(DWORD dwStageIndex, DWORD *pTextureStateCache)
{
	memset( pTextureStateCache, 0xFF, dwMaxTextureStageStatesAllowed * sizeof( DWORD ) );

	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_COLOROP, (dwStageIndex == 0) ? D3DTOP_MODULATE : D3DTOP_DISABLE );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_COLORARG2, D3DTA_CURRENT );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_ALPHAOP, (dwStageIndex == 0) ? D3DTOP_SELECTARG1 : D3DTOP_DISABLE );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVMAT00, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVMAT01, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVMAT10, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVMAT11, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_TEXCOORDINDEX, dwStageIndex );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVLSCALE, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_BUMPENVLOFFSET, 0 );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_COLORARG0, D3DTA_CURRENT );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_ALPHAARG0, D3DTA_CURRENT );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_RESULTARG, D3DTA_CURRENT );
	RESET_TEXTURE_STATE_CACHE_VALUE( dwStageIndex, D3DTSS_CONSTANT, D3DTA_CURRENT );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
#define RESET_SAMPLER_STATE_CACHE_VALUE( sampler, state, value ) pSamplerStateCache[state] = value;\
	sD3DDev9.pD3DDevice->SetSamplerState( sampler, state, value );

////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseSamplerStateCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseSamplerStateCache( DWORD dwSamplerIndex, DWORD * pSamplerStateCache )
{
	memset( pSamplerStateCache, 0xFF, dwMaxSamplerStatesAllowed * sizeof( DWORD ) );

	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_BORDERCOLOR, 0 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MIPMAPLODBIAS, 0 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MAXMIPLEVEL, 0 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_MAXANISOTROPY, 1 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_SRGBTEXTURE, 0 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_ELEMENTINDEX, 0 );
	RESET_SAMPLER_STATE_CACHE_VALUE( dwSamplerIndex, D3DSAMP_DMAPOFFSET, 0 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseTransformCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseTransformCache( SD3D9TransformCache * pCache )
{
	// No need to clear, already reset.
//	memset( pCache, 0, sizeof( SD3D9TransformCache ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseMaterialCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseMaterialCache( SD3D9MaterialCache * pCache )
{
	// No need to clear already reset.
//	memset( pCache, 0, sizeof( SD3D9MaterialCache ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// InitialiseLightCache()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::InitialiseLightCache( SD3D9LightCache * pCache )
{
	pCache->dwNumLights = sD3DDev9.sD3DDevCaps.MaxActiveLights;

	if( pCache->dwNumLights == 0xFFFFFFFF )
	{
		// Software VP supports any number of lights, so returns -1. We'll limit it to 8,
		// even though we don't use that many.
		_ASSERT( sD3DDev9.bHardwareVP == false );
		pCache->dwNumLights = 8;
	}

	pCache->pLightsActive = new BOOL[ pCache->dwNumLights ];
	pCache->pLights = new D3DLIGHT9[ pCache->dwNumLights ];

	memset( pCache->pLightsActive, 0, pCache->dwNumLights * sizeof( BOOL ) );
	memset( pCache->pLights, 0, pCache->dwNumLights * sizeof( D3DLIGHT9 ) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Cache access function.
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// SetRenderState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( State < dwMaxRenderStatesAllowed );
	if( sD3DDev9.sDeviceStateCache.pRenderState[ State ] == Value )
	{
		return D3D_OK;
	}
#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumStateChanges ] ++;
#endif // EnablePerformanceCounters

	sD3DDev9.sDeviceStateCache.pRenderState[ State ] = Value;
	return sD3DDev9.pD3DDevice->SetRenderState( State, Value );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetRenderState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetRenderState( D3DRENDERSTATETYPE State, DWORD * pValue )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( State < dwMaxRenderStatesAllowed );
	*pValue = sD3DDev9.sDeviceStateCache.pRenderState[ State ];
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetTextureStageState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Stage < dwMaxTextureStages );
	_ASSERT( Type < dwMaxTextureStageStatesAllowed );
	if( sD3DDev9.sDeviceStateCache.pTextureStageStates[Stage][Type] == Value )
	{
		return D3D_OK;
	}
#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumStateChanges ] ++;
#endif // EnablePerformanceCounters

	sD3DDev9.sDeviceStateCache.pTextureStageStates[Stage][Type] = Value;
	return sD3DDev9.pD3DDevice->SetTextureStageState( Stage, Type, Value );
}

	

////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTextureStageState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Stage < dwMaxTextureStages );
	_ASSERT( Type < dwMaxTextureStageStatesAllowed );
	*pValue = sD3DDev9.sDeviceStateCache.pTextureStageStates[Stage][Type];
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetSamplerState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Sampler < sD3DDev9.sDeviceStateCache.dwNumSamplers );
	_ASSERT( Type < dwMaxSamplerStatesAllowed );
	
	if( sD3DDev9.sDeviceStateCache.pSamplerState[Sampler][Type] == Value )
	{
		return D3D_OK;
	}
#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumStateChanges ] ++;
#endif // EnablePerformanceCounters

	sD3DDev9.sDeviceStateCache.pSamplerState[Sampler][Type] = Value;
	return sD3DDev9.pD3DDevice->SetSamplerState( Sampler, Type, Value );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetSamplerState()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Sampler < sD3DDev9.sDeviceStateCache.dwNumSamplers );
	_ASSERT( Type < dwMaxSamplerStatesAllowed );

	*pValue = sD3DDev9.sDeviceStateCache.pSamplerState[Sampler][Type];
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetTransform()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::SetTransform( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT(	( State == D3DTS_PROJECTION ) ||
				( State == D3DTS_VIEW ) ||
				( State == D3DTS_WORLD ) );

	HRESULT hr;
	switch( State )
	{
	case D3DTS_PROJECTION:
		memcpy( &sD3DDev9.sTransformCache.matProjection, pMatrix, sizeof( D3DMATRIX ) );
		hr = sD3DDev9.pD3DDevice->SetTransform( D3DTS_PROJECTION, pMatrix );
		break;

	case D3DTS_VIEW:
		memcpy( &sD3DDev9.sTransformCache.matView, pMatrix, sizeof( D3DMATRIX ) );
		hr = sD3DDev9.pD3DDevice->SetTransform( D3DTS_VIEW, pMatrix );
		break;

	case D3DTS_WORLD:
		memcpy( &sD3DDev9.sTransformCache.matWorld, pMatrix, sizeof( D3DMATRIX ) );
		hr = sD3DDev9.pD3DDevice->SetTransform( D3DTS_WORLD, pMatrix );
		break;

	default:
		_ASSERT( false );
		hr = D3DERR_INVALIDCALL;
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTransform()
// 
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT(	( State == D3DTS_PROJECTION ) ||
				( State == D3DTS_VIEW ) ||
				( State == D3DTS_WORLD ) );
	HRESULT hr = D3D_OK;
	switch( State )
	{
	case D3DTS_PROJECTION:
		memcpy( pMatrix, &sD3DDev9.sTransformCache.matProjection, sizeof( D3DMATRIX ) );
		break;

	case D3DTS_VIEW:
		memcpy( pMatrix, &sD3DDev9.sTransformCache.matView, sizeof( D3DMATRIX ) );
		break;

	case D3DTS_WORLD:
		memcpy( pMatrix, &sD3DDev9.sTransformCache.matWorld, sizeof( D3DMATRIX ) );
		break;

	default:
		_ASSERT( false );
		hr = D3DERR_INVALIDCALL;
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetMaterial()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetMaterial( CONST D3DMATERIAL9 * pMaterial )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( pMaterial != NULL );

	memcpy( &sD3DDev9.sMaterialCache.currentMaterial, pMaterial, sizeof( D3DMATERIAL9 ) );
	return sD3DDev9.pD3DDevice->SetMaterial( pMaterial );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetMaterial()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetMaterial( D3DMATERIAL9 * pMaterial )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( pMaterial != NULL );
	memcpy( pMaterial, &sD3DDev9.sMaterialCache.currentMaterial, sizeof( D3DMATERIAL9 ) );
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetLight()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::SetLight( DWORD Index, CONST D3DLIGHT9 * pLight )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( pLight != NULL );
	_ASSERT( Index < sD3DDev9.sLightCache.dwNumLights );

	memcpy( &sD3DDev9.sLightCache.pLights[ Index ], pLight, sizeof( D3DLIGHT9 ) );
	return sD3DDev9.pD3DDevice->SetLight( Index, pLight );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetLight()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::GetLight( DWORD Index, D3DLIGHT9 * pLight )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( pLight != NULL );
	_ASSERT( Index < sD3DDev9.sLightCache.dwNumLights );

	memcpy( pLight, &sD3DDev9.sLightCache.pLights[ Index ], sizeof( D3DLIGHT9 ) );
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// LightEnable()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::LightEnable( DWORD Index, BOOL bEnable )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Index < sD3DDev9.sLightCache.dwNumLights );

	HRESULT hr = D3D_OK;
	if( sD3DDev9.sLightCache.pLightsActive[ Index ] != bEnable )
	{
		sD3DDev9.sLightCache.pLightsActive[ Index ] = bEnable;
		hr = sD3DDev9.pD3DDevice->LightEnable( Index, bEnable );
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetLightEnable()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetLightEnable( DWORD Index, BOOL * pEnable )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	_ASSERT( Index < sD3DDev9.sLightCache.dwNumLights );

	*pEnable = sD3DDev9.sLightCache.pLightsActive[Index];
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetFVF()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetFVF( DWORD FVF )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	
	if( sD3DDev9.sDeviceStateCache.dwFVF == FVF )
	{
		return D3D_OK;
	}

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumShaderChanges ] ++;
#endif // EnablePerformanceCounters

	sD3DDev9.sDeviceStateCache.dwFVF = FVF;
	return sD3DDev9.pD3DDevice->SetFVF( FVF );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetFVF()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetFVF( DWORD * pFVF )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

	*pFVF = sD3DDev9.sDeviceStateCache.dwFVF;
	return D3D_OK;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetTexture()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetTexture( DWORD Sampler, LPDIRECT3DBASETEXTURE9 pTexture )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumTextureChanges ] ++;
#endif // EnablePerformanceCounters

	return sD3DDev9.pD3DDevice->SetTexture( Sampler, pTexture );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTexture()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetTexture( DWORD Sampler, LPDIRECT3DBASETEXTURE9 * ppTexture )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->GetTexture( Sampler, ppTexture );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetViewport()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetViewport( CONST D3DVIEWPORT9 * pViewport )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->SetViewport( pViewport );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetViewport()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetViewport( D3DVIEWPORT9 * pViewport )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->GetViewport( pViewport );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetIndices()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetIndices( LPDIRECT3DINDEXBUFFER9 pIndexData )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->SetIndices( pIndexData );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetIndices()
// 
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetIndices( LPDIRECT3DINDEXBUFFER9 * ppIndexData )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->GetIndices( ppIndexData );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetStreamSource()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::SetStreamSource( UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 pStreamData, UINT OffsetInBytes, UINT Stride )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->SetStreamSource( StreamNumber, pStreamData, OffsetInBytes, Stride );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetStreamSource()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::GetStreamSource( UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 * ppStreamData, UINT * pOffsetInBytes, UINT * pStride )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->GetStreamSource( StreamNumber, ppStreamData, pOffsetInBytes, pStride );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// BeginScene()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::BeginScene()
{
	HRESULT hr;

	_ASSERT( sD3DDev9.bInScene == false );
	hr = sD3DDev9.pD3DDevice->BeginScene();

	if( hr == D3D_OK )
	{
		sD3DDev9.bInScene = true;
	}

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_CurrentTexMem ] = (int)sD3DDev9.pD3DDevice->GetAvailableTextureMem();
#endif // EnablePerformanceCounters

	_ASSERT( hr == D3D_OK );
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// EndScene()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::EndScene()
{
	HRESULT hr;

	_ASSERT( sD3DDev9.bInScene == true );
	hr = sD3DDev9.pD3DDevice->EndScene();
	
	if( hr == D3D_OK )
	{
		sD3DDev9.bInScene = false;
	}

	_ASSERT( hr == D3D_OK );
	return hr;
}

	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawPrimitive()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::DrawPrimitive(	D3DPRIMITIVETYPE Type,
									UINT StartVertex,
									UINT PrimitiveCount )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumDrawPrims ] ++;
	m_pPerformanceCounters[ eD9S_NumPrimsRendered ] += (int)PrimitiveCount;
#endif // EnablePerformanceCounters

	return sD3DDev9.pD3DDevice->DrawPrimitive( Type, StartVertex, PrimitiveCount );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawIndexedPrimitive()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::DrawIndexedPrimitive(	D3DPRIMITIVETYPE Type, 
											INT BaseVertexIndex, 
											UINT MinIndex, 
											UINT NumVertices, 
											UINT StartIndex, 
											UINT PrimitiveCount )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumDrawPrims ] ++;
	m_pPerformanceCounters[ eD9S_NumPrimsRendered ] += (int)PrimitiveCount;
#endif // EnablePerformanceCounters

	return sD3DDev9.pD3DDevice->DrawIndexedPrimitive( 
				Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawIndexedPrimitiveUP()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CD3DDevice9::DrawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, 
												UINT MinVertexIndex, 
												UINT NumVertices, 
												UINT PrimitiveCount, 
												CONST void * pIndexData, 
												D3DFORMAT IndexDataFormat, 
												CONST void * pVertexStreamZeroData,
												UINT VertexStreamZeroStride )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumDrawPrims ] ++;
	m_pPerformanceCounters[ eD9S_NumPrimsRendered ] += (int)PrimitiveCount;
#endif // EnablePerformanceCounters

	return sD3DDev9.pD3DDevice->DrawIndexedPrimitiveUP( 
				PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, 
				pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawIndexedPrimitive()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::DrawPrimitiveUP(	D3DPRIMITIVETYPE PrimitiveType, 
										UINT PrimitiveCount,
										CONST void * pVertexStreamZeroData,
										UINT VertexStreamZeroStride )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );

#ifdef EnablePerformanceCounters
	m_pPerformanceCounters[ eD9S_NumDrawPrims ] ++;
	m_pPerformanceCounters[ eD9S_NumPrimsRendered ] += (int)PrimitiveCount;
#endif // EnablePerformanceCounters

	return sD3DDev9.pD3DDevice->DrawPrimitiveUP( PrimitiveType, PrimitiveCount, 
				pVertexStreamZeroData, VertexStreamZeroStride );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ColorFill()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::ColorFill( LPDIRECT3DSURFACE9 pSurface, CONST RECT * pRect, D3DCOLOR color )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->ColorFill( pSurface, pRect, color );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetGammaRamp()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CD3DDevice9::Clear( DWORD Count, CONST D3DRECT * pRects, DWORD Flags, 
							D3DCOLOR Color, float Z, DWORD Stencil )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	return sD3DDev9.pD3DDevice->Clear( Count, pRects, Flags, Color, Z, Stencil );
}

	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// SetGammaRamp()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::SetGammaRamp( UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp )
{
	_ASSERT( sD3DDev9.bInitialised == true );
	_ASSERT( sD3DDev9.pD3DDevice != NULL );
	sD3DDev9.pD3DDevice->SetGammaRamp( iSwapChain, Flags, pRamp );
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Performance statistics gathering.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EnablePerformanceCounters

////////////////////////////////////////////////////////////////////////////////////////////////////
// ResetPerformanceCounters()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CD3DDevice9::ResetPerformanceCounters()
{
	m_pPerformanceCounters[ eD9S_NumDrawPrims ] = 0;
	m_pPerformanceCounters[ eD9S_NumPrimsRendered ] = 0;
	m_pPerformanceCounters[ eD9S_NumStateChanges ] = 0;
	m_pPerformanceCounters[ eD9S_NumTextureChanges ] = 0;
	m_pPerformanceCounters[ eD9S_NumShaderChanges ] = 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetPerformanceCounter()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
int CD3DDevice9::GetPerformanceCounter(EDevice9Stat stat)
{
	return m_pPerformanceCounters[ stat ];
}

#endif // EnablePerformanceCounters
