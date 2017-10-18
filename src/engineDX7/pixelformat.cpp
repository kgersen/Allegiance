#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

PixelFormat::PixelFormat(
    int   bits,
    DWORD redMask,
    DWORD greenMask,
    DWORD blueMask,
    DWORD alphaMask
) {
    m_ddpf.dwSize            = sizeof(DDPIXELFORMAT);
    m_ddpf.dwFlags           = DDPF_RGB;
    m_ddpf.dwFourCC          = 0;
    m_ddpf.dwRGBBitCount     = bits;
    m_ddpf.dwRBitMask        = redMask;
    m_ddpf.dwGBitMask        = greenMask;
    m_ddpf.dwBBitMask        = blueMask;
    m_ddpf.dwRGBAlphaBitMask = alphaMask;
}

DWORD PixelFormat::RedSize()    const { return RedMask()   >> RedShift();   }
DWORD PixelFormat::GreenSize()  const { return GreenMask() >> GreenShift(); }
DWORD PixelFormat::BlueSize()   const { return BlueMask()  >> BlueShift();  }
DWORD PixelFormat::AlphaSize()  const { return AlphaMask() >> AlphaShift(); }

DWORD PixelFormat::RedShift()   const { return GetShift(RedMask());   }
DWORD PixelFormat::GreenShift() const { return GetShift(GreenMask()); }
DWORD PixelFormat::BlueShift()  const { return GetShift(BlueMask());  }
DWORD PixelFormat::AlphaShift() const { return GetShift(AlphaMask()); }

Pixel PixelFormat::MakePixel(DWORD red, DWORD green, DWORD blue) const
{
    return Pixel::Create(
          ((red   << RedShift()  ) & RedMask()  )
        | ((green << GreenShift()) & GreenMask())
        | ((blue  << BlueShift() ) & BlueMask() ));
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
        case 4: *(DWORD*)pb = (pixel.Value()); break;
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
                      (DWORD(pb[0]) <<  0)
                    | (DWORD(pb[1]) <<  8)
                    | (DWORD(pb[2]) << 16)
                );
        case 4: return Pixel::Create(*(DWORD*)pb);
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
    BitMask mask(m_ddpf.dwFlags);

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
    //

    return false;
}

bool PixelFormat::Equivalent(const DDPIXELFORMAT& ddpf)
{
    return
           m_ddpf.dwFlags           == ddpf.dwFlags
        && m_ddpf.dwRGBBitCount     == ddpf.dwRGBBitCount
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
    DDPixelFormat& ddpf,
    IDirectDrawX* pdd,
    HDC hdc,
    HBITMAP hbitmap,
    IDirectDrawPalette** pppalette
) {
    BYTE        ajBitmapInfo[sizeof(BITMAPINFO) + 3 * sizeof(DWORD)];
    BITMAPINFO* pbmi = (BITMAPINFO*)ajBitmapInfo;
    BOOL        bRet = FALSE;

    memset(pbmi, 0, sizeof(ajBitmapInfo));
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    ZVerify(::GetDIBits(hdc, hbitmap, 0, 0, NULL, pbmi, DIB_RGB_COLORS));

    ddpf.dwFlags           = DDPF_RGB;
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
                    ddpf.dwFlags |= ((ddpf.dwRGBBitCount == 4) ? DDPF_PALETTEINDEXED4 : DDPF_PALETTEINDEXED8);

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

                    DDCall(pdd->CreatePalette(
                        (ddpf.dwRGBBitCount == 4) ? DDPCAPS_4BIT : DDPCAPS_8BIT,
                        ppe,
                        pppalette,
                        NULL
                    ));

                    return true;
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

            ddpf.dwRBitMask = *(DWORD *)&pbmi->bmiColors[0];
            ddpf.dwGBitMask = *(DWORD *)&pbmi->bmiColors[1];
            ddpf.dwBBitMask = *(DWORD *)&pbmi->bmiColors[2];

            return true;
    }

    return false;
}
