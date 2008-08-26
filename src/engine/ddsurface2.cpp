#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Surface Modes
//
//////////////////////////////////////////////////////////////////////////////

enum SurfaceMode 
{
	SurfaceModeDD,
	SurfaceModeLocked
};

//////////////////////////////////////////////////////////////////////////////
//
// VideoSurface
//
//////////////////////////////////////////////////////////////////////////////

class DDSurfaceImpl	: public DDSurface 
{
public:
	//////////////////////////////////////////////////////////////////////////////
	//
	// Types
	//
	//////////////////////////////////////////////////////////////////////////////

	class SurfaceData 
	{
	public:
		int							m_id;
		TRef<PixelFormat>			m_ppf;
		//TRef<IDirect3DSurface9>		m_pdds;
		TRef<IDirect3DTexture9>		m_pd3dtexture;
	};

	//////////////////////////////////////////////////////////////////////////////
	//
	// Members
	//
	//////////////////////////////////////////////////////////////////////////////

	TRef<PrivateEngine>			m_pengine;
	TRef<DDDevice>				m_pdddevice;
	WinPoint					m_size;
	WinPoint					m_sizeSurface;
	SurfaceType					m_stype;

//	DDSDescription				m_ddsdLocked;

	BYTE*						m_pbits;
	bool						m_bSurfaceLocked;
	D3DLOCKED_RECT				m_LockedRect;
	int							m_pitch;

	Color						m_colorKey;
	TRef<PrivatePalette>		m_ppalette;
	Color						m_colorText;

	bool						m_bTextureSize;
	bool						m_bColorKey;
	bool						m_bSharedMemory;

	// Mip-Mapped levels
	SurfaceData					m_data;
	TVector<SurfaceData>		m_datas;

	// Converted detail	levels
	SurfaceData					m_dataConverted;
	TVector<SurfaceData>		m_datasConverted;

	// Surface invalidation	and	modes
	SurfaceMode					m_mode;

	//////////////////////////////////////////////////////////////////////////////
	// CalculateSurfaceSize()
	//
	//////////////////////////////////////////////////////////////////////////////
	void CalculateSurfaceSize()
	{
		m_sizeSurface =	m_size;

		if (m_size.X() < 1)	
		{
			ZAssert(m_size.X() == 0);
			m_sizeSurface.SetX(1);
		}

		if (m_size.Y() < 1)	
		{
			ZAssert(m_size.Y() == 0);
			m_sizeSurface.SetY(1);
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Initialize()
	//
	//////////////////////////////////////////////////////////////////////////////
	void Initialize()
	{
		CalculateSurfaceSize();

		m_bTextureSize =	m_sizeSurface.Y() == (int)NextPowerOf2((DWORD)m_sizeSurface.Y())
						 && m_sizeSurface.X() == (int)NextPowerOf2((DWORD)m_sizeSurface.X());

		m_bColorKey	= false;
		m_colorKey	= Color(0, 0, 0);
		m_data.m_id	= 0;
		m_mode		= SurfaceModeDD;
		m_pbits		= NULL;
	}

	//////////////////////////////////////////////////////////////////////////////
	// FinishConstruction()
	//
	//////////////////////////////////////////////////////////////////////////////

	void FinishConstruction()
	{
		// Set the palette if there	is one
		if (m_ppalette)	
		{
			_ASSERT( false );
//			m_data.m_pdds->SetPalette(m_ppalette->GetDDPal());
		}

		// Get a texture handle	if needed
//		if (m_bTextureSize && !m_stype.Test(SurfaceTypeVideo())) 
//		{
//			DDCall(m_data.m_pdds->QueryInterface(IID_IDirect3DTextureX,	(void**)&(m_data.m_pd3dtexture)));
//		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// DDSurfaceImpl() - constructor 1.
	// Create a	video memory surface
	//////////////////////////////////////////////////////////////////////////////
	DDSurfaceImpl(	DDDevice*		  pdddevice,
					SurfaceType	  stype,
					PixelFormat*	  ppf,
					PrivatePalette* ppalette,
				const WinPoint&		  size	) :
		m_pdddevice(pdddevice),
		m_pengine(pdddevice->GetEngine()),
		m_stype(stype),
		m_ppalette(ppalette),
		m_size(size),
		m_bSharedMemory(false)
	{
		m_data.m_ppf   = ppf;

		Initialize();

		_ASSERT( false );			// CREATE A D3D TEXTURE HERE.

		// Figure out the DD caps
		DWORD caps = DDSCAPS_VIDEOMEMORY;

		if (Is3D())	
		{
			caps |=	DDSCAPS_3DDEVICE;
		} 
		else 
		{
			if (m_bTextureSize)	
			{
				caps |=	DDSCAPS_TEXTURE;
			}
			else 
			{
				caps |=	DDSCAPS_OFFSCREENPLAIN;
			}
		}

		// Create the surface
		_ASSERT( false );
//		m_data.m_pdds =	m_pdddevice->CreateSurface(size, caps, ppf,	true);

		// Was there enough	memory to allocate the surface?	
//		if( m_data.m_pdds != NULL )
		if( m_data.m_pd3dtexture != NULL )
		{
			// Get the pitch
			D3DSURFACE_DESC ddsd;
			m_data.m_pd3dtexture->GetLevelDesc( 0, &ddsd );

//			DDSDescription ddsd;
//			DDCall(m_data.m_pdds->GetSurfaceDesc(&ddsd));
			//m_pitch	= ddsd.lPitch;

			// Do any other	common construction	tasks
			FinishConstruction();
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// DDSurfaceImpl() - constructor 2.
	// Wrap	an previously created DirectDraw surface
	//////////////////////////////////////////////////////////////////////////////
	DDSurfaceImpl(	DDDevice*				pdddevice,
					IDirect3DTexture9 *		pdds,
					IDirect3DTexture9 *		pddsZBuffer,
					PixelFormat *			ppf,
					PrivatePalette *		ppalette,
					SurfaceType				stype	) :
		m_pdddevice(pdddevice),
		m_pengine(pdddevice->GetEngine()),
		m_ppalette(ppalette),
		m_stype(stype |	SurfaceType2D()),
		m_bSharedMemory(false)
	{
		//DDSDescription ddsd;
		_ASSERT( false );
		D3DSURFACE_DESC ddsd;
		pdds->GetLevelDesc( 0, &ddsd );
//		DDCall(pdds->GetSurfaceDesc(&ddsd));

//		m_data.m_pdds			= pdds;
		m_data.m_pd3dtexture	= pdds;
		m_data.m_ppf			= ppf;
		m_size					= WinPoint( ddsd.Width, ddsd.Height );
		m_pitch					= -1;

		Initialize();
	}

	//////////////////////////////////////////////////////////////////////////////
	// DDSurfaceImpl() - constructor 3.
	// Create a	surface	from a binary representation
	//
	//////////////////////////////////////////////////////////////////////////////

	DDSurfaceImpl(	DDDevice *			pdddevice,
					SurfaceType			stype,
					PixelFormat*		ppf,
					PrivatePalette*		ppalette,
				const WinPoint&			size,
					int					pitch,
					BYTE*				pbits	) :
									m_pdddevice(pdddevice),
									m_pengine(pdddevice->GetEngine()),
									m_stype(stype),
									m_ppalette(ppalette),
									m_size(size)
	{

		// initialize
		m_pitch		   = pitch;
		m_data.m_ppf   = ppf;

		Initialize();

		// Create a TEXTURE.
		IDirect3DDeviceX * pD3DDev = pdddevice->GetD3DDevice()->GetD3DDeviceX();
		_ASSERT( pD3DDev != NULL );

		// For speed, what we should be doing is creating the texture in D3DPOOL_DEFAULT, then creating a seperate
		// texture in D3DPOOL_SYSTEMMEM, and using UpdateTexture to update the VRAM version.
		// For now, we create managed so that we can easily copy our texture data into it.
		HRESULT hr = pD3DDev->CreateTexture(	m_size.X(),
												m_size.Y(),
												1,
												0,
												m_data.m_ppf->GetEquivalentD3DFormat(),
												D3DPOOL_MANAGED,
												&m_data.m_pd3dtexture,
												NULL );

		DDCall( hr );

		// Lock the texture and copy our data in.
		D3DLOCKED_RECT lockRect;
		hr = m_data.m_pd3dtexture->LockRect( 0, &lockRect, NULL, D3DLOCK_DISCARD );

		DDCall( hr );

		m_pitch = lockRect.Pitch;
		BYTE * pDest = (BYTE*) lockRect.pBits;

		for	(int y = 0;	y <	m_size.Y();	y++) 
		{
			memcpy( pDest + y * m_pitch, 
					pDest + y * pitch, 
					pitch );
		}

		// Unlock.
		hr = m_data.m_pd3dtexture->UnlockRect( 0 );
		DDCall( hr );

		// Do any other	common construction	tasks
		FinishConstruction();

		// Is the memory writable?
/*		MEMORY_BASIC_INFORMATION meminfo;
		ZVerify(VirtualQuery(pbits,	&meminfo, sizeof(meminfo)) == sizeof(meminfo));
		m_bSharedMemory	= ( ( meminfo.Protect & 
							(PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY) ) != 0 );

		// initialize
		m_pitch		   = pitch;
		m_data.m_ppf   = ppf;

		Initialize();

		// Figure out the DD caps
		DWORD caps = DDSCAPS_SYSTEMMEMORY;

		if (Is3D())	
		{
			caps |=	DDSCAPS_3DDEVICE;
		}

		if (m_bTextureSize)	
		{
			caps |=	DDSCAPS_TEXTURE;
		} 
		else 
		{
			caps |=	DDSCAPS_OFFSCREENPLAIN;
		}

		// Create the surface
		DDSDescription ddsd;

		ddsd.dwFlags			= DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_PITCH;
		ddsd.dwWidth			= m_size.X();
		ddsd.dwHeight			= m_size.Y();
		ddsd.lPitch				= pitch;
		ddsd.ddsCaps.dwCaps		= caps;
		ddsd.ddpfPixelFormat	= m_data.m_ppf->GetDDPF();

		if( m_bSharedMemory )
		{
			_ASSERT( false );
			// use the passed in memory
			ddsd.dwFlags   = ddsd.dwFlags |	DDSD_LPSURFACE;
			ddsd.lpSurface = pbits;
			HRESULT	hr = m_pdddevice->GetDD()->CreateSurface(&ddsd,	&m_data.m_pdds,	NULL);
			DDCall(hr);

			#ifdef _DEBUG
				DDSDescription ddsd;
				DDCall(m_data.m_pdds->GetSurfaceDesc(&ddsd));
				ZAssert(pitch == ddsd.lPitch);
			#endif

			m_pitch	= pitch;
		} 
		else 
		{
			_ASSERT( false );
			// Let directx allocate	the	memory
			HRESULT	hr = m_pdddevice->GetDD()->CreateSurface(&ddsd,	&m_data.m_pdds,	NULL);
			DDCall(hr);

			// Copy	the	bits over
			DDCall(m_data.m_pdds->Lock(NULL, &ddsd,	DDLOCK_SURFACEMEMORYPTR	| DDLOCK_WAIT, NULL));
			m_pitch	= ddsd.Pitch();

			ZAssert(pitch <= m_pitch);

			for	(int y = 0;	y <	m_size.Y();	y++) 
			{
				memcpy( ddsd.Pointer() + y * m_pitch, pbits	+ y * pitch, pitch );
			}

			DDCall(m_data.m_pdds->Unlock(NULL));
		}

		// Do any other	common construction	tasks
		FinishConstruction();*/
	}

	//////////////////////////////////////////////////////////////////////////////
	// IsMemoryShared()
	//
	//////////////////////////////////////////////////////////////////////////////
	bool IsMemoryShared()
	{
		return m_bSharedMemory;
	}

	//////////////////////////////////////////////////////////////////////////////
	// IsValid()
	// 
	//////////////////////////////////////////////////////////////////////////////
	bool IsValid()
	{
		return m_data.m_pd3dtexture != NULL;
//		return m_data.m_pdds !=	NULL;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Destructor
	//
	//////////////////////////////////////////////////////////////////////////////
	~DDSurfaceImpl()
	{
	}

/*	//////////////////////////////////////////////////////////////////////////////
	// GetDDSXZBuffer()
	// Attach a	ZBuffer	surface.
	// Not needed for D3D.
	//////////////////////////////////////////////////////////////////////////////
	IDirectDrawSurfaceX * GetDDSXZBuffer()
	{
		if (HasZBuffer()) {
			if (m_pddsZBuffer == NULL) {
				DWORD caps = DDSCAPS_ZBUFFER;

				if (m_stype.Test(SurfaceTypeVideo())) {
					caps |=	DDSCAPS_VIDEOMEMORY;
				} else {
					caps |=	DDSCAPS_SYSTEMMEMORY;
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
	}*/

	//////////////////////////////////////////////////////////////////////////////
	// SurfaceChanged()
	// Surface updates
	//////////////////////////////////////////////////////////////////////////////
	void SurfaceChanged()
	{
		m_data.m_id++;
		if (m_data.m_id	< 0) m_data.m_id = 0;
	}

/*	//////////////////////////////////////////////////////////////////////////////
	// Unpack()
	// Down	Sample
	//////////////////////////////////////////////////////////////////////////////
	static DWORD Unpack(WORD pixelWord)
	{
		DWORD pixel	= pixelWord;
		return ((pixel & 0xf800) << 5) | ((pixel & 0x07e0) << 3) | ((pixel & 0x001f) );
	}

	//////////////////////////////////////////////////////////////////////////////
	// Pack()
	//////////////////////////////////////////////////////////////////////////////
	static WORD	Pack(DWORD pixel)
	{
		return (WORD)(((pixel >> 5)	& 0xf800) | ((pixel >> 3) & 0x07e0) | ((pixel) & 0x001f));
	}

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	static WORD	Average(WORD pixel0, WORD pixel1, WORD pixel2, WORD	pixel3)
	{
		return Pack( ( Unpack(pixel0) + Unpack(pixel1) + Unpack(pixel2) + Unpack(pixel3) ) >> 2 );
	}

	static void	DownSample(
		const WinPoint&		 sizeTarget,
		IDirectDrawSurfaceX* pddsTarget,
		IDirectDrawSurfaceX* pddsSource
	) {
		DDSDescription ddsdSource;
		DDSDescription ddsdTarget;

		DDCall(pddsSource->Lock(NULL, &ddsdSource, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
		DDCall(pddsTarget->Lock(NULL, &ddsdTarget, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));

		int	pitchSource	= ddsdSource.Pitch();
		int	pitchTarget	= ddsdTarget.Pitch();

		ZAssert(
			   ddsdSource.GetPixelFormat().dwRGBBitCount
			== ddsdTarget.GetPixelFormat().dwRGBBitCount
		);

		switch (ddsdSource.GetPixelFormat().dwRGBBitCount) {
			case 16:
				{
					ZAssert(ddsdSource.GetPixelFormat().dwRBitMask		  == 0xf800);
					ZAssert(ddsdSource.GetPixelFormat().dwGBitMask		  == 0x07e0);
					ZAssert(ddsdSource.GetPixelFormat().dwBBitMask		  == 0x001f);
					ZAssert(ddsdSource.GetPixelFormat().dwRGBAlphaBitMask == 0x0000);

					WORD* psource	= (WORD*)ddsdSource.Pointer();
					WORD* ptarget	= (WORD*)ddsdTarget.Pointer();

					for	(int y = sizeTarget.Y()	- 1; y >= 0; y--) {
						for	(int x = sizeTarget.X()	- 1; x >= 0; x--) {
							ptarget[x] =
								Average(
									psource[x *	2],
									psource[x *	2 +	1],
									psource[x *	2	  +	pitchSource	/ 2],
									psource[x *	2 +	1 +	pitchSource	/ 2]
								);
						}

						psource	+= 2 * pitchSource / 2;
						ptarget	+=	   pitchTarget / 2;
					}
				}
				break;

			case 24:
				{
					BYTE* psource	= (BYTE*)ddsdSource.Pointer();
					BYTE* ptarget	= (BYTE*)ddsdTarget.Pointer();

					for	(int y = sizeTarget.Y()	- 1; y >= 0; y--) {
						for	(int x = sizeTarget.X()	- 1; x >= 0; x--) {
							BYTE* psource0 = psource + x * 6;
							BYTE* psource1 = psource + x * 6 + 3;
							BYTE* psource2 = psource + x * 6	 + pitchSource;
							BYTE* psource3 = psource + x * 6 + 3 + pitchSource;

							ptarget[x *	3 +	0] = (psource0[0] +	psource1[0]	+ psource2[0] +	psource3[0]) / 4;
							ptarget[x *	3 +	1] = (psource0[1] +	psource1[1]	+ psource2[1] +	psource3[1]) / 4;
							ptarget[x *	3 +	2] = (psource0[2] +	psource1[2]	+ psource2[2] +	psource3[2]) / 4;
						}

						psource	+= pitchSource * 2;
						ptarget	+= pitchTarget;
					}
				}
				break;

			default:
				ZError("DownSample:	Invalid	PixelFormat");
		};

		DDCall(pddsTarget->Unlock(NULL));
		DDCall(pddsSource->Unlock(NULL));
	}*/

	//////////////////////////////////////////////////////////////////////////////
	//
	// Construct a new lod or converted	surface
	//
	//////////////////////////////////////////////////////////////////////////////

	void ConstructSurfaceData(SurfaceData& data, PixelFormat* ppf, const WinPoint& size)
	{
		data.m_id  = -1;
		data.m_ppf = ppf;

		DWORD caps = DDSCAPS_SYSTEMMEMORY;

		if (m_bTextureSize/* &&	ppf->IsSoftwareTexture()*/)	{
			caps |=	DDSCAPS_TEXTURE;
		} else {
			caps |=	DDSCAPS_OFFSCREENPLAIN;
		}

		_ASSERT( false );
//		data.m_pdds	=
//			m_pdddevice->CreateSurface( size, caps, ppf, false );
		data.m_pd3dtexture = m_pdddevice->CreateSurface( size, caps, ppf, false );

		if (HasColorKey()) 
		{
			_ASSERT( false );
/*			DDCOLORKEY key;

			key.dwColorSpaceLowValue  =
			key.dwColorSpaceHighValue =
				ppf->MakePixel(GetColorKey()).Value();

			DDCall(data.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));*/
		}

//		if (caps & DDSCAPS_TEXTURE)	
//		{
//			DDCall(data.m_pdds->QueryInterface(IID_IDirect3DTextureX, (void**)&(data.m_pd3dtexture)));
//		} 
//		else 
//		{
//			data.m_pd3dtexture = NULL;
//		}
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Convert pixel formats using GDI
	//
	//////////////////////////////////////////////////////////////////////////////

//	static void	Convert(const WinPoint&	size, IDirect3DSurface9 * pddsTarget, IDirect3DSurface9 * pddsSource)
	static void	Convert(const WinPoint&	size, IDirect3DTexture9 * pddsTarget, IDirect3DTexture9 * pddsSource)
	{
/*		HDC	hdcSource;
		DDCall(pddsSource->GetDC(&hdcSource));

		HDC	hdcTarget;
		DDCall(pddsTarget->GetDC(&hdcTarget));

		ZVerify(::BitBlt(hdcTarget,	0, 0, size.X(),	size.Y(), hdcSource, 0,	0, SRCCOPY));

		DDCall(pddsTarget->ReleaseDC(hdcTarget));
		DDCall(pddsSource->ReleaseDC(hdcSource));*/
	}

	//////////////////////////////////////////////////////////////////////////////
	// UpdateConvertedSurface()
	// Make	sure the bits for a	converted surface are available	and	up to date.
	//////////////////////////////////////////////////////////////////////////////
	void UpdateConvertedSurface(PixelFormat* ppf, const	WinPoint& size,	SurfaceData& data, const SurfaceData& dataSource)
	{
		if( ppf != data.m_ppf )
		{
			ConstructSurfaceData(data, ppf,	size);
		}

		if( data.m_id != dataSource.m_id ) 
		{
			SetSurfaceMode(SurfaceModeDD);

			Convert(size, data.m_pd3dtexture, dataSource.m_pd3dtexture);
//			Convert(size, data.m_pdds, dataSource.m_pdds);
			data.m_id =	dataSource.m_id;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// GetDDSX()
	// Get a surface with a	certain	pixel format.
	//////////////////////////////////////////////////////////////////////////////
//	IDirect3DSurface9 * GetDDSX()
	IDirect3DTexture9 * GetDDSX()
	{
		SetSurfaceMode(SurfaceModeDD);
		return m_data.m_pd3dtexture;
	}

	//////////////////////////////////////////////////////////////////////////////
	// GetDDSX()
	//
	//////////////////////////////////////////////////////////////////////////////
	IDirect3DTexture9 * GetDDSX(PixelFormat* ppf)
	{
		if (ppf	== m_data.m_ppf) 
		{
			SetSurfaceMode(SurfaceModeDD);
			return m_data.m_pd3dtexture;
		} 
		else 
		{
			UpdateConvertedSurface(ppf,	m_size,	m_dataConverted, m_data);
			return m_dataConverted.m_pd3dtexture;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Get a texture with a	certain	pixel format.
	//
	//////////////////////////////////////////////////////////////////////////////

	IDirect3DTextureX* GetTextureX(PixelFormat*	ppf, const WinPoint& size, int&	id)
	{
		id = m_data.m_id;

		// Return the fullsize surface if that's what's	wanted.
		if (size ==	m_size)	
		{
			if (ppf	== m_data.m_ppf) 
			{
				SetSurfaceMode(SurfaceModeDD);
				return m_data.m_pd3dtexture;
			}

			UpdateConvertedSurface(ppf,	m_size,	m_dataConverted, m_data);
			return m_dataConverted.m_pd3dtexture;
		}

		// Need	to return a	lower level	of detail
		int	index =	0;
		WinPoint sizeSource	= m_size;

		while (true) {
			sizeSource.SetX(sizeSource.X() / 2);
			sizeSource.SetY(sizeSource.Y() / 2);

			//
			// Do we need to allocate a	new	lower level	of detail?
			//

			if (index == m_datas.GetCount()) {
				m_datas.PushEnd();
				m_datasConverted.PushEnd();
				ConstructSurfaceData(m_datas.Get(index), m_data.m_ppf, sizeSource);
			}

			// Do we need to update	this level?
			if (m_datas[index].m_id	!= m_data.m_id)	
			{
				_ASSERT( false );
/*				DownSample(
					sizeSource,
					m_datas[index].m_pdds,
					index == 0 ?
						  m_data.m_pdds
						: m_datas[index	- 1].m_pdds
				);

				m_datas.Get(index).m_id	= m_data.m_id;*/
			}

			// Did we find the right size?
			if (sizeSource == size)	
			{
				// Does	the	format need	to be converted?
				if (ppf	== m_data.m_ppf) 
				{
					return m_datas[index].m_pd3dtexture;
				} 
				else 
				{
					UpdateConvertedSurface(ppf,	size, m_datasConverted.Get(index), m_datas[index]);
					return m_datasConverted[index].m_pd3dtexture;
				}
			}

			index++;
		}
	}

	TRef<IDirect3DTexture9> GetDDS()
	{
//		SetSurfaceMode(SurfaceModeDD);

//		TRef<IDirectDrawSurface> pdds;
//		DDCall(m_data.m_pdds->QueryInterface(IID_IDirectDrawSurface, (void**)&pdds));
		return m_data.m_pd3dtexture;
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
		return m_stype.Test(SurfaceTypeVideo())	!= 0;
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

	PrivatePalette*	GetPalette()
	{
		return m_ppalette;
	}

	DDDevice* GetDDDevice()
	{
		return m_pdddevice;
	}

	Engine*	GetEngine()
	{
		return m_pengine;
	}

	PrivateEngine* GetEngineImpl()
	{
		return m_pengine;
	}

	const WinPoint&	GetSize()
	{
		return m_size;
	}

	SurfaceType	GetSurfaceType()
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
		ZAssert(m_datasConverted.GetCount()	== 0);

		m_bColorKey	= true;
		m_colorKey = color;

		// Update the ddraw	surface
		SetSurfaceMode(SurfaceModeDD);
		DDCOLORKEY key;
		key.dwColorSpaceLowValue  =
		key.dwColorSpaceHighValue =	m_data.m_ppf->MakePixel(m_colorKey).Value();

//		DDCall(m_data.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));

		// Update the converted	ddraw surface
		if( m_dataConverted.m_pd3dtexture )	
		{
			_ASSERT( false );
//			DDCall(m_dataConverted.m_pdds->SetColorKey(DDCKEY_SRCBLT, &key));
		}

		SurfaceChanged();
	}

	int	GetPitch()
	{
		return m_pitch;
	}

	BYTE* GetPointer()
	{
		_ASSERT( false );
		return NULL;
/*		if (m_pbits	!= NULL) {
			return m_pbits;
		} else {
			SetSurfaceMode(SurfaceModeLocked);

			BYTE* pbits	= m_ddsdLocked.Pointer();

			if (pbits == NULL) 
			{
				// DDraw returned null.	 Allocate some memory so the caller	won't crash.
				m_pbits	= new BYTE[m_pitch * m_size.Y()];
				return m_pbits;
			}

			return pbits;
		}*/
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

	void SetSurfaceMode(SurfaceMode	mode)
	{
		if (m_mode != mode)	
		{
			// switch back to DDMode
			switch (m_mode)	
			{
			case SurfaceModeLocked:
				if (m_pbits	== NULL) 
				{
//					DDSCall(m_data.m_pdds->Unlock(NULL));
					DDSCall(m_data.m_pd3dtexture->UnlockRect( 0 ));
				}
				SurfaceChanged();
				break;
			}

			// switch to the new mode
			m_mode = mode;
			switch (mode) 
			{
			case SurfaceModeLocked:
				_ASSERT( false );
//				DDSCall(m_data.m_pdds->Lock(NULL, &m_ddsdLocked, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,	NULL));
				//m_data.m_pdds->LockRect( &m_LockedRect, NULL, 
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Blts	to and from	a dc
	//
	//////////////////////////////////////////////////////////////////////////////

	void BitBltFromDC(HDC hdc)
	{
		_ASSERT( false );
/*		SetSurfaceMode(SurfaceModeDD);
		HDC	hdcSurface;
		if (DDSCall(m_data.m_pdds->GetDC(&hdcSurface)))	
		{
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
		SetSurfaceMode(SurfaceModeDD);*/
	}

	struct SVertDefn
	{
		float fX, fY, fZ, fRHW;
		D3DCOLOR colour;
	//	float fU, fV;
	};

	SVertDefn sTempVerts[6];

	//////////////////////////////////////////////////////////////////////////////
	//
	// StretchBlt
	//
	//////////////////////////////////////////////////////////////////////////////

	void UnclippedBlt(	const WinRect& rectTarget,
						DDSurface*  psurfaceSource,
						const WinRect& rectSource,
						bool		   bColorKey ) 
	{
//		_ASSERT( false );
		HRESULT hr;
		const LPDIRECT3DDEVICE9 pD3DDevice = CD3DDevice9::Device();
//		IDirect3DSurface9 * pSrc, * pDest;
		
//		hr = psurfaceSource->GetDDSX()->GetSurfaceLevel( 0, &pSrc );
//		hr = pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pDest );
//		hr = pD3DDevice->StretchRect(	pSrc, NULL, pDest, NULL, D3DTEXF_NONE );

//		DWORD dwFVF = 1 << D3DFVF_TEXCOUNT_SHIFT;
//		dwFVF |= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEXTUREFORMAT2;
		DWORD dwFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

		sTempVerts[0].fX = 0.0f;
		sTempVerts[0].fY = 0.0f;
		sTempVerts[0].fZ = 0.0f;
		sTempVerts[0].fRHW = 1.0f;
		sTempVerts[0].colour = 0xFFFFFFFF;
//		sTempVerts[0].fU = 0.0f;
//		sTempVerts[0].fV = 0.0f;

		sTempVerts[1].fX = 100.0f;
		sTempVerts[1].fY = 0.0f;
		sTempVerts[1].fZ = 0.0f;
		sTempVerts[1].fRHW = 1.0f;
		sTempVerts[1].colour = 0xFFFFFFFF;
//		sTempVerts[1].fU = 1.0f;
//		sTempVerts[1].fV = 0.0f;

		sTempVerts[2].fX = 0.0f;
		sTempVerts[2].fY = 100.0f;
		sTempVerts[2].fZ = 0.0f;
		sTempVerts[2].fRHW = 1.0f;
		sTempVerts[2].colour = 0xFFFFFFFF;
//		sTempVerts[2].fU = 0.0f;
//		sTempVerts[2].fV = 1.0f;

		sTempVerts[3].fX = 0.0f;
		sTempVerts[3].fY = 100.0f;
		sTempVerts[3].fZ = 0.0f;
		sTempVerts[3].fRHW = 1.0f;
		sTempVerts[3].colour = 0xFFFFFFFF;
//		sTempVerts[3].fU = 0.0f;
//		sTempVerts[3].fV = 1.0f;

		sTempVerts[4].fX = 100.0f;
		sTempVerts[4].fY = 0.0f;
		sTempVerts[4].fZ = 0.0f;
		sTempVerts[4].fRHW = 1.0f;
		sTempVerts[4].colour = 0xFFFFFFFF;
//		sTempVerts[4].fU = 1.0f;
//		sTempVerts[4].fV = 0.0f;

		sTempVerts[5].fX = 100.0f;
		sTempVerts[5].fY = 100.0f;
		sTempVerts[5].fZ = 0.0f;
		sTempVerts[5].fRHW = 1.0f;
		sTempVerts[5].colour = 0xFFFFFFFF;
//		sTempVerts[5].fU = 1.0f;
//		sTempVerts[5].fV = 1.0f;

//		hr = pD3DDevice->SetTexture( 0, psurfaceSource->GetDDSX() );
//		_ASSERT( hr == D3D_OK );

		hr = pD3DDevice->SetFVF( dwFVF );
		_ASSERT( hr == D3D_OK );

		hr = pD3DDevice->TestCooperativeLevel();
		_ASSERT( hr == D3D_OK );

		// 
		hr = pD3DDevice->DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
											2,
											&sTempVerts[0],
											sizeof( DDSurfaceImpl::SVertDefn ) );

//		_ASSERT( hr == D3D_OK );

/*		SetSurfaceMode(SurfaceModeDD);
		DWORD flags	= bColorKey	? DDBLT_KEYSRC : 0;
		DDSurface* pddsurface; 
		CastTo(pddsurface, psurfaceSource);

		DDSCall( m_data.m_pdds->Blt(	(RECT*)&rectTarget,	
										pddsurface->GetDDSX(),
										(RECT*)&rectSource,	
										flags |	DDBLT_WAIT,	
										NULL ) );*/
	}


	//////////////////////////////////////////////////////////////////////////////
	//
	// Blt
	//
	//////////////////////////////////////////////////////////////////////////////

	void UnclippedBlt(		const WinRect&			rect, 
							IDirect3DSurface9 *		pddsSource, 
							const WinPoint&			pointSource, 
							bool					bColorKey ) 
	{
		_ASSERT( false );
/*		SetSurfaceMode(SurfaceModeDD);
		DWORD flags	= bColorKey	? DDBLT_KEYSRC : 0;
		WinRect	rectSource(pointSource,	pointSource	+ rect.Size());

		DDSCall(m_data.m_pdds->Blt(
			(RECT*)&rect, 
			pddsSource,	
			(RECT*)&rectSource,	
			flags |	DDBLT_WAIT,	
			NULL
		));*/
	}

	void UnclippedBlt(const	WinRect& rect, DDSurface * psurfaceSource, const WinPoint& pointSource)
	{
		_ASSERT( false );
		DDSurface* pddsurface; CastTo(pddsurface, psurfaceSource);

//		UnclippedBlt(rect, pddsurface->GetDDSX(), pointSource, pddsurface->HasColorKey());
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Fill
	//
	//////////////////////////////////////////////////////////////////////////////

	void UnclippedFill(const WinRect& rect,	Pixel pixel)
	{
		_ASSERT( false );
/*		DDBltFX	ddbltfx;
		ddbltfx.dwFillColor	= pixel.Value();

		DDSCall(m_data.m_pdds->Blt(
			(RECT*)&rect, 
			NULL, 
			(RECT*)&rect, 
			DDBLT_COLORFILL	| DDBLT_WAIT, 
			&ddbltfx
		));*/
	}
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructors
//
//////////////////////////////////////////////////////////////////////////////

TRef<DDSurface>	CreateDDSurface(	DDDevice*		pdddevice,
									SurfaceType		stype,
									PixelFormat*	ppf,
									PrivatePalette* ppalette,
								const WinPoint&		size ) 
{
	TRef<DDSurface>	pddsurface = new DDSurfaceImpl(pdddevice, stype, ppf, ppalette,	size);

	if (pddsurface->IsValid()) 
	{
		return pddsurface;
	} 
	else 
	{
		return NULL;
	}
}

TRef<DDSurface>	CreateDDSurface(	DDDevice*		pdddevice,
									SurfaceType		stype,
									PixelFormat*	ppf,
									PrivatePalette* ppalette,
								const WinPoint&		size,
									int				pitch,
									BYTE*			pbits ) 
{
	return new DDSurfaceImpl(pdddevice,	stype, ppf,	ppalette, size,	pitch, pbits);
}

TRef<DDSurface>	CreateDDSurface(	DDDevice*				pdddevice,
									IDirect3DTexture9 *		pdds,
									IDirect3DTexture9 *		pddsZBuffer,
									PixelFormat*			ppf,
									PrivatePalette*			ppalette,
									SurfaceType				stype ) 
{
	return new	DDSurfaceImpl(	pdddevice,
								pdds,
								pddsZBuffer,
								ppf,
								ppalette,
								stype );
}
