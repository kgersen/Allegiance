#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// DD Struct Wrappers
//
//////////////////////////////////////////////////////////////////////////////

DWORD MakeMask(int bits, int shift)
{
    return ((1 << bits) - 1) << shift;
}

HRESULT DDPixelFormat::SetFormat(IDirectDrawX* pdd, HDC hdc, HBITMAP hbitmap, IDirectDrawPalette** pppalette)
{
    BYTE        ajBitmapInfo[sizeof(BITMAPINFO) + 3*sizeof(DWORD)];
    BITMAPINFO *pbmi = (BITMAPINFO *) ajBitmapInfo;
    BOOL        bRet = FALSE;

    memset(pbmi, 0, sizeof(ajBitmapInfo));
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    ZVerify(::GetDIBits(hdc, hbitmap, 0, 0, NULL, pbmi, DIB_RGB_COLORS));

    dwFlags           = DDPF_RGB;
    dwRGBBitCount     = pbmi->bmiHeader.biBitCount;
    dwRGBAlphaBitMask = 0;

    switch(pbmi->bmiHeader.biCompression) {
        //
        // Default DIB format.  Color masks are implicit for each bit depth.
        //

        case BI_RGB:
            switch (dwRGBBitCount) {
                case 4:
                case 8:
                {
                    dwFlags |= ((dwRGBBitCount == 4) ? DDPF_PALETTEINDEXED4 : DDPF_PALETTEINDEXED8);

                    //
                    // Create a palette for the surface
                    //

                    RGBQUAD prgb[256];
                    int ncolors = GetDIBColorTable(hdc, 0, 256, prgb);

                    ZAssert(ncolors == (1 << dwRGBBitCount));

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
                        (dwRGBBitCount == 4) ? DDPCAPS_4BIT : DDPCAPS_8BIT,
                        ppe,
                        pppalette,
                        NULL
                    ));

                    return S_OK;
                }

                case 16:
                    // 16bpp default is 555 BGR-ordering

                    dwRBitMask = MakeMask(5, 10);
                    dwGBitMask = MakeMask(5,  5);
                    dwBBitMask = MakeMask(5,  0);

                    return S_OK;

                case 24:
                case 32:
                    // 24 and 32bpp default is 888 BGR-ordering

                    dwRBitMask = MakeMask(8, 16);
                    dwGBitMask = MakeMask(8,  8);
                    dwBBitMask = MakeMask(8,  0);

                    return S_OK;
            }
            break;

        case BI_BITFIELDS:

            //
            // Call a second time to get the color masks.
            // It's a GetDIBits Win32 "feature".
            //

            ZVerify(::GetDIBits(hdc, hbitmap, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS));

            dwRBitMask = *(DWORD *)&pbmi->bmiColors[0];
            dwGBitMask = *(DWORD *)&pbmi->bmiColors[1];
            dwBBitMask = *(DWORD *)&pbmi->bmiColors[2];

            return S_OK;
    }

    ZError("Unknown bitmap pixel format");
    return E_FAIL;
}
