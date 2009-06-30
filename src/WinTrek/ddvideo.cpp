#include "pch.h"
#include "ddvideo.h" //Macros and crap
#include "ddstream.h" //DShow object

DDVideo::DDVideo()
{
	m_hWnd         = NULL;
    m_lpDD         = NULL;
    m_lpDDSPrimary = NULL;
    m_lpDDSBack    = NULL;
	m_Running      = TRUE; 
}

// This is it...
HRESULT DDVideo::Play(ZString& strPath)
{
    HRESULT hr;  
	if( FAILED( hr = InitDirectDraw() ) )
		return hr;

	m_pVideo=new CDShow();

	// Pop up a message and return FALSE if failed.
	if (m_pVideo==NULL) 
	{
		return FALSE;	
	}                                    
    	
	if(!( m_pVideo->Open(strPath,m_lpDD) ))
		{
	
		// remember to destroy DShow (pVideo) object to clean up after us.
		// Return FALSE to let caller know we failed.
		return FALSE;
		}			
	else
		m_pVideo->Start();     

	return S_OK;
}

HRESULT DDVideo::InitDirectDraw()
{
	DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps;
	HRESULT		hRet;
 
    //Create the main DirectDraw object
    LPDIRECTDRAW pDD;

    hRet = DirectDrawCreate(NULL,&pDD, NULL);
    hRet = pDD->QueryInterface(IID_IDirectDraw7, (LPVOID *) & m_lpDD);
      
	//Set cooperative level
	hRet = m_lpDD->SetCooperativeLevel(m_hWnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN |DDSCL_ALLOWREBOOT| DDSCL_ALLOWMODEX);

	//Let's keep it simple and roll on primary adapter... 32-bits start fullscreen on 800x600
	hRet = m_lpDD->SetDisplayMode(800,600,32,0,0);

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

    ddsd.dwBackBufferCount = 1;

	hRet = m_lpDD->CreateSurface(&ddsd, &m_lpDDSPrimary, NULL);

    ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	hRet = m_lpDDSPrimary->GetAttachedSurface(&ddscaps,&m_lpDDSBack);

	pDD->Release();
return hRet;
}

void DDVideo::DestroyDirectDraw()
{	
	m_lpDD->SetCooperativeLevel(m_hWnd,DDSCL_NORMAL);

	SAFE_RELEASE(m_lpDDSBack);
	SAFE_RELEASE(m_lpDDSPrimary);
	SAFE_RELEASE(m_lpDD);
}	  

void DDVideo::DestroyDDVid()
{	
	delete m_pVideo; 
	DestroyDirectDraw();
}