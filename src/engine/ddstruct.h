#ifndef _ddstruct_h_
#define _ddstruct_h_

//////////////////////////////////////////////////////////////////////////////
//
// Zero fill for structs with a dwSize member
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class TZeroFillWithSize : public Type {
protected:
    TZeroFillWithSize()
    {
        Reinitialize();
    }

public:
    void Reinitialize()
    {
        memset(this, 0, sizeof(Type));
        this->dwSize = sizeof(Type);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Missing DirectX 5 declarations
//
//////////////////////////////////////////////////////////////////////////////

#define DDSCL_SETFOCUSWINDOW                    0x00000080l
#define DDSCL_SETDEVICEWINDOW                   0x00000100l
#define DDSCL_CREATEDEVICEWINDOW                0x00000200l

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class DDPixelFormat      : public TZeroFillWithSize<DDPIXELFORMAT> {};

class DDDeviceIdentifier : public TZeroFill<DDDEVICEIDENTIFIER> {};

//////////////////////////////////////////////////////////////////////////////
//
// DDSurface Description
//
//////////////////////////////////////////////////////////////////////////////

class DDSDescription : public TZeroFillWithSize<DDSURFACEDESCX> {
public:
    DDSDescription()
    {
        ddpfPixelFormat.dwSize = sizeof(DDPixelFormat);
    }

    DDPixelFormat& GetPixelFormat()
    {
        return *(DDPixelFormat*)&ddpfPixelFormat;
    }

    const DDPixelFormat& GetPixelFormat() const
    {
        return *(DDPixelFormat*)&ddpfPixelFormat;
    }

    BYTE* Pointer() const { return (BYTE*)lpSurface; }

    BYTE* Pointer(const WinPoint& point) const
    {
        return 
              Pointer() 
            + point.Y() * Pitch() 
            + point.X() * ddpfPixelFormat.dwRGBBitCount / 8;
    }

    int XSize() const { return (int)dwWidth;  }
    int YSize() const { return (int)dwHeight; }
    WinPoint Size() const { return WinPoint(dwWidth, dwHeight); }

    WinRect Rect() const 
    { 
        return WinRect(0, 0, dwWidth, dwHeight); 
    }

    DWORD Pitch() const { return lPitch; }
};

//////////////////////////////////////////////////////////////////////////////
//
// DD Device Caps
//
//////////////////////////////////////////////////////////////////////////////

class DDSCaps : public TZeroFill<DDSCAPSX> {};

//////////////////////////////////////////////////////////////////////////////
//
// DD Device Caps
//
//////////////////////////////////////////////////////////////////////////////

class DDCaps : public TZeroFillWithSize<DDCAPS> {};

//////////////////////////////////////////////////////////////////////////////
//
// D3D Device Description
//
//////////////////////////////////////////////////////////////////////////////

class D3DDeviceDescription : public TZeroFillWithSize<D3DDEVICEDESC> {};

//////////////////////////////////////////////////////////////////////////////
//
// D3D Device Description
//
//////////////////////////////////////////////////////////////////////////////

class DDBltFX : public TZeroFillWithSize<DDBLTFX> {};

//////////////////////////////////////////////////////////////////////////////
//
// D3D class wrappers
//
//////////////////////////////////////////////////////////////////////////////

class D3DRect : public D3DRECT {
public:
    D3DRect() {}
    D3DRect(long x1Arg, long y1Arg, long x2Arg, long y2Arg)
    {
        x1 = x1Arg;
        y1 = y1Arg;
        x2 = x2Arg;
        y2 = y2Arg;
    }
};

class D3DColorValue : public D3DCOLORVALUE {
public:
    D3DColorValue() {}
    D3DColorValue(const Color& Color)
    {
        r = Color.GetRed();
        g = Color.GetGreen();
        b = Color.GetBlue();
        a = Color.GetAlpha();
    }

    friend bool operator==(const D3DColorValue& c1, const D3DColorValue& c2)
    {
        return
               c1.r == c2.r
            && c1.g == c2.g
            && c1.b == c2.b
            && c1.a == c2.a;
    }

    D3DCOLOR MakeD3DCOLOR()
    {
        return D3DRGBA(r, g, b, a);
    }

    operator Color()
    {
        return Color(r, g, b, a);
    }
};


#endif
