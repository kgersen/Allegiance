#ifndef _pixelformat_h_
#define _pixelformat_h_

//////////////////////////////////////////////////////////////////////////////
//
// Pixel Formats
//
//////////////////////////////////////////////////////////////////////////////

class PixelFormat : public IObject {
private:
    DDPIXELFORMAT m_ddpf;
    bool          m_bSoftwareTexture;

    void SetSoftwareTexture()
    {
        m_bSoftwareTexture = true;
    }

    PixelFormat(const DDPIXELFORMAT& ddpf) :
        m_ddpf(ddpf),
        m_bSoftwareTexture(false)
    {
    }

    friend class EngineImpl;

public:
    PixelFormat(
        int   bits,
        DWORD redMask,
        DWORD greenMask,
        DWORD blueMask,
        DWORD alphaMask
    );

    const DDPIXELFORMAT& GetDDPF() { return m_ddpf; }

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
    Color MakeColor(Pixel pixel)                        const;
    void  SetPixel(BYTE* pb, Pixel pixel)               const;
    Pixel GetPixel(const BYTE* pb)                      const;
    void  SetColor(BYTE* pb, const Color& color)        const;
    Color GetColor(const BYTE* pb)                      const;
    bool  ValidGDIFormat()                              const;

    bool Equivalent(const DDPIXELFORMAT& ddpf);
};

#endif
