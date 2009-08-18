#ifndef  _VIDEOSCREEN_HEADER_
#define  _VIDEOSCREEN_HEADER_

#include <ddraw.h>
#include "ddstream.h"

#pragma comment(lib,"ddraw.lib") 
#pragma comment(lib,"dxguid.lib") 
#pragma comment(lib,"amstrmid.lib")  

#define SAFE_RELEASE(x)  { if(x) {x->Release(); x = NULL;} }

/* Global variables */
class DDVideo
{
public:
	HWND                    m_hWnd;
	LPDIRECTDRAW7           m_lpDD;
	LPDIRECTDRAWSURFACE7    m_lpDDSPrimary;
	LPDIRECTDRAWSURFACE7    m_lpDDSBack;	

	BOOL   m_Running;
	// DirectShow object instance
	CDShow * m_pVideo;

	DDVideo();
	~DDVideo();

	HRESULT Play(ZString& strPath);  //this is what we're here for folks
	HRESULT InitDirectDraw();

	VOID DestroyDirectDraw();
	VOID DestroyDDVid();
};

#endif