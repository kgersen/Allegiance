#ifndef _pixelformat_h_
#define _pixelformat_h_

#define D3D9PF_ALPHAPIXELS			0x00000001l		// The surface has alpha channel information in the pixel format.
#define D3D9PF_ALPHA                0x00000002l		// The pixel format contains alpha only information
#define D3D9PF_PALETTEINDEXED4      0x00000008l		// The surface is 4-bit color indexed.
#define D3D9PF_PALETTEINDEXED8      0x00000020l		// The surface is 8-bit color indexed.

//
// D3D9PIXELFORMAT
//
typedef struct _D3D9PIXELFORMAT
{
	DWORD	dwSize;
	DWORD	dwFlags;				// See above flags.
	DWORD   dwRGBBitCount;          // how many bits per pixel
	DWORD   dwRBitMask;             // mask for red bit
	DWORD   dwGBitMask;             // mask for green bits
	DWORD   dwBBitMask;             // mask for blue bits
	DWORD   dwRGBAlphaBitMask;      // mask for alpha channel
} D3D9PIXELFORMAT;

typedef D3D9PIXELFORMAT FAR* LPD3D9PIXELFORMAT;

//////////////////////////////////////////////////////////////////////////////
//
// Pixel Formats
//
//////////////////////////////////////////////////////////////////////////////

class PixelFormat : public IObject {
private:
    D3D9PIXELFORMAT	m_ddpf;
	D3DFORMAT		m_d3dFormat;
    bool			m_bSoftwareTexture;

    void SetSoftwareTexture()
    {
        m_bSoftwareTexture = true;
    }

/*    PixelFormat(const DDPIXELFORMAT& ddpf) :
        m_ddpf(ddpf),
        m_bSoftwareTexture(false)
    {
    }*/

    friend class EngineImpl;

public:
    PixelFormat(
        int   bits,
        DWORD redMask,
        DWORD greenMask,
        DWORD blueMask,
        DWORD alphaMask
    );

	PixelFormat( D3DFORMAT d3dFmt );

    const D3D9PIXELFORMAT& GetDDPF() { return m_ddpf; }

    void SetPixelBits(DWORD value) { m_ddpf.dwRGBBitCount = value; }

    bool IsSoftwareTexture() { return m_bSoftwareTexture; }

    DWORD PixelBits()  const { return m_ddpf.dwRGBBitCount;     }
    DWORD PixelBytes() const { return m_ddpf.dwRGBBitCount / 8; }
    DWORD RedMask()    const { return m_ddpf.dwRBitMask;        }
    DWORD GreenMask()  const { return m_ddpf.dwGBitMask;        }
    DWORD BlueMask()   const { return m_ddpf.dwBBitMask;        }
    DWORD AlphaMask()  const { return m_ddpf.dwRGBAlphaBitMask; }

    DWORD RedSize()    const;
    DWORD GreenSize()  const;
    DWORD BlueSize()   const;
    DWORD AlphaSize()  const;

    DWORD RedShift()   const;
    DWORD GreenShift() const;
    DWORD BlueShift()  const;
    DWORD AlphaShift() const;

    Pixel MakePixel(DWORD red, DWORD green, DWORD blue) const;
    Pixel MakePixel(const Color& color)                 const;
	Pixel MakeD3DPixel( const Color& color)				const;
    Color MakeColor(Pixel pixel)                        const;
    void  SetPixel(BYTE* pb, Pixel pixel)               const;
    Pixel GetPixel(const BYTE* pb)                      const;
    void  SetColor(BYTE* pb, const Color& color)        const;
    Color GetColor(const BYTE* pb)                      const;
    bool  ValidGDIFormat()                              const;

    bool Equivalent(const D3D9PIXELFORMAT& ddpf);

	D3DFORMAT GetD3DFormat() const { return m_d3dFormat; }
	D3DFORMAT GetEquivalentD3DFormat( );
};

#endif
