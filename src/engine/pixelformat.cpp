#include "pixelformat.h"

#include <zassert.h>
#include <zmath.h>

#include "ddstruct.h"

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

PixelFormat::PixelFormat(
    int   bits,
    uint32_t redMask,
    uint32_t greenMask,
    uint32_t blueMask,
    uint32_t alphaMask
) {
	m_d3dFormat					= D3DFMT_UNKNOWN;
	m_ddpf.dwSize				= sizeof( D3D9PIXELFORMAT );
	m_ddpf.dwFlags				= 0;
    m_ddpf.dwRGBBitCount		= bits;
    m_ddpf.dwRBitMask			= redMask;
    m_ddpf.dwGBitMask			= greenMask;
    m_ddpf.dwBBitMask			= blueMask;
    m_ddpf.dwRGBAlphaBitMask	= alphaMask;
	m_bSoftwareTexture			= false;

	m_d3dFormat					= GetEquivalentD3DFormat();
}

PixelFormat::PixelFormat( D3DFORMAT d3dFmt )
{
	m_ddpf.dwSize		= sizeof( D3D9PIXELFORMAT );
	m_ddpf.dwFlags		= 0;
	m_bSoftwareTexture	= false;
	m_d3dFormat			= d3dFmt;

	switch( d3dFmt )
	{
	case D3DFMT_X8R8G8B8:
		m_ddpf.dwRGBBitCount		= 32;
		m_ddpf.dwRBitMask			= 255 << 16;
		m_ddpf.dwGBitMask			= 255 << 8;
		m_ddpf.dwBBitMask			= 255;
		m_ddpf.dwRGBAlphaBitMask	= 0;
		break;

	case D3DFMT_A8R8G8B8:
		m_ddpf.dwRGBBitCount		= 32;
		m_ddpf.dwRBitMask			= 255 << 16;
		m_ddpf.dwGBitMask			= 255 << 8;
		m_ddpf.dwBBitMask			= 255;
		m_ddpf.dwRGBAlphaBitMask	= 255 << 24;
		break;

	case D3DFMT_A1R5G5B5:
		m_ddpf.dwRGBBitCount		= 16;
		m_ddpf.dwRBitMask			= 31 << 10;
		m_ddpf.dwGBitMask			= 31 << 5;
		m_ddpf.dwBBitMask			= 31;
		m_ddpf.dwRGBAlphaBitMask	= 1 << 15;
		break;

	case D3DFMT_R5G6B5:
		m_ddpf.dwRGBBitCount		= 16;
		m_ddpf.dwRBitMask			= 31 << 11;
		m_ddpf.dwGBitMask			= 63 << 5;
		m_ddpf.dwBBitMask			= 31;
		m_ddpf.dwRGBAlphaBitMask	= 0;
		break;

	default:
        ZAssert( false && "Unsupported D3DFORMAT value.");
	}
}

uint32_t PixelFormat::RedSize()    const { return RedMask()   >> RedShift();   }
uint32_t PixelFormat::GreenSize()  const { return GreenMask() >> GreenShift(); }
uint32_t PixelFormat::BlueSize()   const { return BlueMask()  >> BlueShift();  }
uint32_t PixelFormat::AlphaSize()  const { return AlphaMask() >> AlphaShift(); }

uint32_t PixelFormat::RedShift()   const { return GetShift(RedMask());   }
uint32_t PixelFormat::GreenShift() const { return GetShift(GreenMask()); }
uint32_t PixelFormat::BlueShift()  const { return GetShift(BlueMask());  }
uint32_t PixelFormat::AlphaShift() const { return GetShift(AlphaMask()); }

Pixel PixelFormat::MakePixel(uint32_t red, uint32_t green, uint32_t blue) const
{
    return Pixel::Create(
          ((red   << RedShift()  ) & RedMask()  )
        | ((green << GreenShift()) & GreenMask())
        | ((blue  << BlueShift() ) & BlueMask() )
		| AlphaMask() );
}

Pixel PixelFormat::MakePixel(const Color& color) const
{
    return
        MakePixel(
            int(color.GetRed()   * RedSize()  ),
            int(color.GetGreen() * GreenSize()),
            int(color.GetBlue()  * BlueSize() )
        );

    // !!! this causes all of the artwork to change slightly
    //     since the rounding mode is different

    /*
    return
        MakePixel(
            MakeInt(color.GetRed()   * RedSize()  ),
            MakeInt(color.GetGreen() * GreenSize()),
            MakeInt(color.GetBlue()  * BlueSize() )
        );
    */
}

// Added - make color into a 32 bit D3D colour value.
Pixel PixelFormat::MakeD3DPixel( const Color& color) const
{
	PixelFormat tempFmt( D3DFMT_A8R8G8B8 );
	return tempFmt.MakePixel( color );
}

Color PixelFormat::MakeColor(Pixel pixel) const
{
    return
        Color(
            (float)((pixel.Value() &   RedMask()) >>   RedShift()) /   RedSize(),
            (float)((pixel.Value() & GreenMask()) >> GreenShift()) / GreenSize(),
            (float)((pixel.Value() &  BlueMask()) >>  BlueShift()) /  BlueSize()
        );
}

void PixelFormat::SetPixel(BYTE* pb, Pixel pixel) const
{
    switch (PixelBytes()) {
        case 1:         *pb = (BYTE)(pixel.Value()); break;
        case 2:  *(WORD*)pb = (WORD)(pixel.Value()); break;
        case 3:
            pb[0] = (BYTE)(((pixel.Value()) >>  0) & 0xff);
            pb[1] = (BYTE)(((pixel.Value()) >>  8) & 0xff);
            pb[2] = (BYTE)(((pixel.Value()) >> 16) & 0xff);
            break;
        case 4: *(uint32_t*)pb = (pixel.Value()); break;
    }
}

Pixel PixelFormat::GetPixel(const BYTE* pb) const
{
    switch (PixelBytes()) {
        case 1: return Pixel::Create(*pb);
        case 2: return Pixel::Create(*(WORD*)pb);
        case 3:
            return
                Pixel::Create(
                      (uint32_t(pb[0]) <<  0)
                    | (uint32_t(pb[1]) <<  8)
                    | (uint32_t(pb[2]) << 16)
                );
        case 4: return Pixel::Create(*(uint32_t*)pb);
    }

    return Pixel::Create(0);
}

void PixelFormat::SetColor(BYTE* pb, const Color& color) const
{
    SetPixel(pb, MakePixel(color));
}

Color PixelFormat::GetColor(const BYTE* pb) const
{
    return MakeColor(GetPixel(pb));
}

bool PixelFormat::ValidGDIFormat() const
{
    ZAssert( false );
/*    BitMask mask(m_ddpf.dwFlags);

    //
    // gdi doesn't support alpha
    //

    if (mask.Test(BitMask(DDPF_ALPHA | DDPF_ALPHAPIXELS))) {
        return false;
    }

    if (m_ddpf.dwRGBBitCount == 8) {
        //
        // 8 bpp must be palettized
        //

        return mask.Test(BitMask(DDPF_PALETTEINDEXED8));
    } else if (m_ddpf.dwRGBBitCount > 8) {
        //
        // 16 bpp must be RGB
        //

        return mask.Test(BitMask(DDPF_RGB));
    }

    //
    // less than 8 bpp not supported
    //*/

    return false;
}

bool PixelFormat::Equivalent(const D3D9PIXELFORMAT& ddpf)
{
    return m_ddpf.dwRGBBitCount     == ddpf.dwRGBBitCount
        && m_ddpf.dwRBitMask        == ddpf.dwRBitMask
        && m_ddpf.dwGBitMask        == ddpf.dwGBitMask
        && m_ddpf.dwGBitMask        == ddpf.dwGBitMask
        && m_ddpf.dwRGBAlphaBitMask == ddpf.dwRGBAlphaBitMask;
}

//////////////////////////////////////////////////////////////////////////////
//
// constructor
//
//////////////////////////////////////////////////////////////////////////////

bool FillDDPF(
    D3D9PixelFormat& ddpf,
    HDC hdc,
    HBITMAP hbitmap,
    uint32_t * dwPaletteIndex
) {
    BYTE        ajBitmapInfo[sizeof(BITMAPINFO) + 3 * sizeof(uint32_t)];
    BITMAPINFO* pbmi = (BITMAPINFO*)ajBitmapInfo;
    BOOL        bRet = FALSE;

    memset(pbmi, 0, sizeof(ajBitmapInfo));
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    ZVerify(::GetDIBits(hdc, hbitmap, 0, 0, NULL, pbmi, DIB_RGB_COLORS));

    ddpf.dwRGBBitCount     = pbmi->bmiHeader.biBitCount;
    ddpf.dwRGBAlphaBitMask = 0;

    switch(pbmi->bmiHeader.biCompression) {
        //
        // Default DIB format.  Color masks are implicit for each bit depth.
        //

        case BI_RGB:
            switch (ddpf.dwRGBBitCount) {
                case 4:
                case 8:
                {
                    ZAssert( false );		// Need to look at palette conversion if this is used.
/*                    ddpf.dwFlags |= ((ddpf.dwRGBBitCount == 4) ? DDPF_PALETTEINDEXED4 : DDPF_PALETTEINDEXED8);

                    //
                    // Create a palette for the surface
                    //

                    RGBQUAD prgb[256];
                    int ncolors = GetDIBColorTable(hdc, 0, 256, prgb);

                    ZAssert(ncolors == (1 << ddpf.dwRGBBitCount));

                    PALETTEENTRY ppe[256];

                    //
                    // convert BGR to RGB
                    //

                    for (int index = 0; index < ncolors; index++) {
                        ppe[index].peRed   = prgb[index].rgbRed;
                        ppe[index].peGreen = prgb[index].rgbGreen;
                        ppe[index].peBlue  = prgb[index].rgbBlue;
                    }

                    //
                    // create a DirectDraw palette for the texture.
                    //
                    ZAssert( false && "TBD" );
					// Add palette management to CVRAMManager. Should be easy.
//                    DDCall(pdd->CreatePalette(
//                        (ddpf.dwRGBBitCount == 4) ? DDPCAPS_4BIT : DDPCAPS_8BIT,
//                        ppe,
//                        pppalette,
//                        NULL
//                    ));

                    return true;*/
					return false;
                }

                case 16:
                    // 16bpp default is 555 BGR-ordering

                    ddpf.dwRBitMask = MakeMask(5, 10);
                    ddpf.dwGBitMask = MakeMask(5,  5);
                    ddpf.dwBBitMask = MakeMask(5,  0);

                    return true;

                case 24:
                case 32:
                    // 24 and 32bpp default is 888 BGR-ordering

                    ddpf.dwRBitMask = MakeMask(8, 16);
                    ddpf.dwGBitMask = MakeMask(8,  8);
                    ddpf.dwBBitMask = MakeMask(8,  0);

                    return true;
            }
            break;

        case BI_BITFIELDS:

            //
            // Call a second time to get the color masks.
            // It's a GetDIBits Win32 "feature".
            //

            ZVerify(::GetDIBits(hdc, hbitmap, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS));

            ddpf.dwRBitMask = *(uint32_t *)&pbmi->bmiColors[0];
            ddpf.dwGBitMask = *(uint32_t *)&pbmi->bmiColors[1];
            ddpf.dwBBitMask = *(uint32_t *)&pbmi->bmiColors[2];

            return true;
    }

    return false;
}



//////////////////////////////////////////////////////////////////////////////
// GetEquivalentD3DFormat()
// Temporary function.
//////////////////////////////////////////////////////////////////////////////
D3DFORMAT PixelFormat::GetEquivalentD3DFormat( )
{
	D3DFORMAT retVal;
	if( m_ddpf.dwRGBBitCount == 8 ) 
	{
		retVal = D3DFMT_P8;
	}
	else if( m_ddpf.dwRGBBitCount == 16 )
	{
		if( m_ddpf.dwRGBAlphaBitMask != 0 )
		{
			retVal = D3DFMT_A1R5G5B5;
		}
		else
		{
			retVal = D3DFMT_R5G6B5;
		}
	}
	else
	{
		if( m_ddpf.dwRGBAlphaBitMask != 0 )
		{
			retVal = D3DFMT_A8R8G8B8;
		}
		else
		{
			retVal = D3DFMT_X8R8G8B8;
		}
	}
	if( m_d3dFormat != D3DFMT_UNKNOWN )
	{
        ZAssert( m_d3dFormat == retVal );
	}
	return retVal;
}

