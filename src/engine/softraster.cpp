#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Optimize this file for speed
//
//////////////////////////////////////////////////////////////////////////////

// /Oa assume no aliasing
// /Ow assume aliasing across function calls
// /Og global optimization
// /Ot fast code sequences
//

#ifndef _DEBUG
    #pragma optimize("t", on)
    #pragma inline_depth(255)

    //#pragma inline_recursion(off)
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Fast floating point routines 
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Reinterpret casts
//
//////////////////////////////////////////////////////////////////////////////

__forceinline float& AsFloat(DWORD& value) { return *(float *)(&value); }
__forceinline float& AsFloat(int&   value) { return *(float *)(&value); }
__forceinline int&   AsInt  (float& value) { return *(int   *)(&value); }
__forceinline DWORD& AsDWORD(float& value) { return *(DWORD *)(&value); }

// VS.Net 2003 port
#if _MSC_VER >= 1310
__forceinline void swap(float& x, float& y)
#else
__forceinline void swap<float>(float& x, float& y)
#endif
{
    DWORD temp(AsDWORD(x));
    AsDWORD(x) = AsDWORD(y);
    AsDWORD(y) = temp;
}

//////////////////////////////////////////////////////////////////////////////
//
// FPU Control word functions
//
//////////////////////////////////////////////////////////////////////////////

class ControlWordBase {};
typedef TBitMask<ControlWordBase, WORD> ControlWord;

class ControlWordExceptions : public ControlWord { public: ControlWordExceptions () : ControlWord(0x003f) {} };
class ControlWordPrecision  : public ControlWord { public: ControlWordPrecision  () : ControlWord(0x0300) {} };
class ControlWordRounding   : public ControlWord { public: ControlWordRounding   () : ControlWord(0x0c00) {} };

__forceinline ControlWord GetControlWord()
{
    WORD w;

    __asm {
        fstcw   w
    }

    ControlWord cw;
    cw.SetWord(w);

    return cw;
}

__forceinline void SetControlWord(const ControlWord& cw)
{
    WORD w = cw.GetWord();

    __asm {
        // fnclex // safe?
        fldcw   w
    }
}

void AssertChopRound()
{
    ZAssert(GetControlWord().Test(ControlWordRounding()));
}

void AssertRound()
{
    ZAssert(!GetControlWord().Test(ControlWordRounding()));
}

ControlWord g_cw;

void InitializeControlWord()
{
    g_cw = GetControlWord();
    ControlWord cw = g_cw;

    cw.Clear(ControlWordRounding());
    cw.Clear(ControlWordPrecision());

    SetControlWord(cw);
}

void RestoreControlWord()
{
    SetControlWord(g_cw);
}

//////////////////////////////////////////////////////////////////////////////
//
// Approximate tranesdental functions
//
//////////////////////////////////////////////////////////////////////////////

#define INT32_FLOAT_ONE         0x3f800000
#define FLOAT_EXPSCALE          ((float)0x00800000)

__forceinline float ApproximateLN(float f)
{
    return (float)(AsInt(f) - INT32_FLOAT_ONE) * float(1.0 / (double)FLOAT_EXPSCALE);
}

__forceinline float ApproximatePow2(float f)
{
    INT32 i = (INT32)(f * FLOAT_EXPSCALE) + INT32_FLOAT_ONE;
    return AsFloat(i);
}

__forceinline float ApproximateInv(float f)
{
    INT32 i = (INT32_FLOAT_ONE << 1) - AsInt(f);
    return AsFloat(i);
}

__forceinline float ApproximateSqrt(float f)
{
    INT32 i = (AsInt(f) >> 1) + (INT32_FLOAT_ONE >> 1);
    return AsFloat(i);
}

/* !!! alternate version what's the difference
__forceinline float ApproximateSqrt(float f)
{
    INT32 i = INT32_float_ONE + (INT32_float_ONE >> 1) - (AsInt(f) >> 1);
    return ASfloat(i);
}
*/

__forceinline float ApproximatePow(float f, float exp)
{
    INT32 i = (INT32)(exp * (AsInt(f) - INT32_FLOAT_ONE)) + INT32_FLOAT_ONE;
    return AsFloat(i);
}

/* !!! such a thing exists look at math.asm
float ApproximateInvSqrt(float value)
{
}
*/

//////////////////////////////////////////////////////////////////////////////
//
// Fast floating point
//
//////////////////////////////////////////////////////////////////////////////

__forceinline DWORD MakeDWORD(float value)
{
    return DWORD(value);
}

__forceinline float Abs(float f)
{
    DWORD i = AsDWORD(f) & 0x7fffffff;
    return AsFloat(i);
}

__forceinline float Negate(float f)
{
    DWORD i = AsDWORD(f) ^ 0x80000000;
    return AsFloat(i);
}

//////////////////////////////////////////////////////////////////////////////
//
// Overlapped Divide
//
//////////////////////////////////////////////////////////////////////////////

__forceinline void StartDivide(float numerator, float denominator)
{
    _asm {
        fld     numerator
        fdiv    denominator
    }
}

__forceinline void StartDivide(float numerator, int denominator)
{
    _asm {
        fld     numerator
        fidiv   denominator
    }
}

__forceinline float EndDivide()
{
    float result;

    _asm {
        fstp    result
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////////
//
// Comparison
//
//////////////////////////////////////////////////////////////////////////////

__forceinline bool GPositiveGreater    (float a, float b) { return AsInt(a) >  AsInt(b); }
__forceinline bool GPositiveLess       (float a, float b) { return AsInt(a) <  AsInt(b); }
__forceinline bool GPositiveEqual      (float a, float b) { return AsInt(a) == AsInt(b); }
__forceinline bool GPositiveGreateEqual(float a, float b) { return AsInt(a) >= AsInt(b); }
__forceinline bool GPositiveLessEqual  (float a, float b) { return AsInt(a) <= AsInt(b); }
__forceinline bool GPositiveNotEqual   (float a, float b) { return AsInt(a) != AsInt(b); }

__forceinline bool GreaterZero     (float a) { return AsInt(a)   >  0;                }
__forceinline bool LessZero        (float a) { return AsDWORD(a) >  0x80000000;       }
__forceinline bool GreaterEqualZero(float a) { return AsDWORD(a) <= 0x80000000;       }
__forceinline bool LessEqualZero   (float a) { return AsInt(a)   <= 0;                }
__forceinline bool EqualZero       (float a) { return (AsDWORD(a) & 0x7fffffff) == 0; }
__forceinline bool NotEqualZero    (float a) { return (AsDWORD(a) & 0x7fffffff) != 0; }

//////////////////////////////////////////////////////////////////////////////
//
// Constants
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Fixed Point Template
//
//////////////////////////////////////////////////////////////////////////////

template<class Type, bool t_bSigned, int t_integerBits, bool t_bAssert = false>
class TFixed {
private:
    Type m_value;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Static functions
    //
    //////////////////////////////////////////////////////////////////////////////

    __forceinline static Type MaxValue()
    {
        if (t_bSigned) {
            return int(PowerOf2(t_integerBits - 1)) - 1;
        } else {
            return PowerOf2(t_integerBits) - 1;
        }
    }

    __forceinline static Type MinValue()
    {
        if (t_bSigned) {
            return -int(PowerOf2(t_integerBits - 1));
        } else {
            return 0;
        }
    }

    __forceinline static int FractionBits()
    {
        return 32 - t_integerBits;
    }

    __forceinline static float Scale()
    {
        return float(PowerOf2(FractionBits()));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructors
    //
    //////////////////////////////////////////////////////////////////////////////

    __forceinline TFixed()
    {
    }

    __forceinline TFixed(const TFixed& x) :
        m_value(x.m_value)
    {
    }

    __forceinline TFixed(float x)
    {
        if (t_bAssert) {
            ZAssert(x <= float(MaxValue()));
            ZAssert(x >= float(MinValue()));
        }

        if (t_bSigned) {
            m_value = MakeInt(x * Scale());
        } else {
            m_value = MakeDWORD(x * Scale());
        }
    }

    __forceinline TFixed(Type x)
    {
        if (t_bAssert) {
            ZAssert(x <= MaxValue());
        }

        if (t_bSigned) {
            ZAssert(x >= MinValue());
        }

        m_value = x << FractionBits();
    }

    __forceinline TFixed(Type x, int shift)
    {
        int bits = shift + FractionBits();

        if (bits == 0) {
            m_value = x;
        } else if (bits > 0) {
            m_value = x <<   bits ;
        } else {
            m_value = x >> (-bits);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Conversions
    //
    //////////////////////////////////////////////////////////////////////////////

    __forceinline float GetFloat() const
    {
        return float(m_value) / Scale();
    }

    __forceinline Type GetInteger(int shift = 0, bool bAlwaysRight = false) const
    {
        int bits = shift - FractionBits();

        if (bAlwaysRight || (bits < 0)) {
            return m_value >> (-bits);
        } else if (bits == 0) {
            return m_value;
        } else {
            return m_value <<   bits ;
        }
    }

    __forceinline const Type& GetFixed() const
    {
        return m_value;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Operators
    //
    //////////////////////////////////////////////////////////////////////////////

    __forceinline TFixed& operator+=(const TFixed<int, t_bSigned, t_integerBits, t_bAssert>& x)
    {
        m_value += x.GetFixed();
        return *this;
    }

    __forceinline TFixed& operator+=(const TFixed<DWORD, t_bSigned, t_integerBits, t_bAssert>& x)
    {
        m_value += x.GetFixed();
        return *this;
    }

    __forceinline TFixed& operator-=(const TFixed& x)
    {
        m_value -= x.m_value;
        return *this;
    }

    __forceinline friend TFixed operator+(const TFixed& x, const TFixed& y)
    {
        return TFixed(x.m_value + y.m_value, -FractionBits());
    }

    __forceinline friend TFixed operator-(const TFixed& x, const TFixed& y)
    {
        return TFixed(x.m_value - y.m_value, -FractionBits());
    }

    __forceinline friend TFixed operator*(const TFixed& x, const TFixed& y)
    {
        return TFixed(x.m_value * y.m_value, -2 * FractionBits());
    }

    __forceinline friend TFixed operator*(const TFixed& x, Type y)
    {
        return TFixed(x.m_value * y, -FractionBits());
    }

    __forceinline friend TFixed operator<<(const TFixed& x, int shift)
    {
        return TFixed(x.m_value, shift - FractionBits());
    }

    __forceinline friend TFixed operator>>(const TFixed& x, int shift)
    {
        return TFixed(x.m_value, FractionBits() - shift);
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// Fixed Point Macros
//
//////////////////////////////////////////////////////////////////////////////

typedef TFixed<int,   true, 12> Fixed;
typedef TFixed<DWORD, true,  2> FixedZ;
typedef TFixed<int,   true,  2> FixedDZ;
typedef TFixed<DWORD, true, 16> Fixed1616;
typedef TFixed<int,   true, 16> FixedD1616;
typedef TFixed<DWORD, true,  2> Fixed131;
typedef TFixed<int,   true,  2> FixedD131;

//////////////////////////////////////////////////////////////////////////////
//
// Colors
//
//////////////////////////////////////////////////////////////////////////////

/*
    Color555
        10-14     0x00007c00    5   Red
         5- 9     0x000003e0    5   Green
         0- 4     0x0000001f    5   Blue

    Color565
        11-15     0x0000f800    5   Red
         5-10     0x000007e0    6   Green
         0- 4     0x0000001f    5   Blue

    ColorE
        30-31     0xa0000000    2   Unused               
        20-29     0x3ff00000    5.5 Red
        10-19     0x000ffa00    5.5 Green
         0- 9     0x000003ff    5.5 Blue
*/

#define Color555RMask 0x7c00
#define Color555GMask 0x03e0
#define Color555BMask 0x001f

#define Color555RShift 15
#define Color555GShift 10
#define Color555BShift 5

#define Color565RMask 0xf800
#define Color565GMask 0x07e0
#define Color565BMask 0x001f

#define Color565RShift 16
#define Color565GShift 11
#define Color565BShift 5

#define ColorERMask 0x3fe00000
#define ColorEGMask 0x000ff800
#define ColorEBMask 0x000003ff

#define ColorERShift 30
#define ColorEGShift 20
#define ColorEBShift 10

#define ColorERSaturationMask 0xc0000000
#define ColorEGSaturationMask 0x00100000
#define ColorEBSaturationMask 0x00000400
#define ColorESaturationMask  (ColorERSaturationMask | ColorEGSaturationMask | ColorEBSaturationMask)

typedef WORD  Color555;
typedef WORD  Color565;
typedef DWORD ColorE;

const float g_componentFloatToIntScale = 255.0f / 256.0f;

//////////////////////////////////////////////////////////////////////////////
//
// Fixed1616 functions
//
//////////////////////////////////////////////////////////////////////////////

template<class Type> 
__forceinline Type ShiftLeft(Type value, int shift)
{
    if (shift >= 0) {
        return value << shift;
    } else {
        return value >> (-shift);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Color565 Helpers
//
//////////////////////////////////////////////////////////////////////////////

__forceinline Fixed1616 GetR(const Color565& color) { return Fixed1616(color & Color565RMask, -Color565RShift); }
__forceinline Fixed1616 GetG(const Color565& color) { return Fixed1616(color & Color565GMask, -Color565GShift); }
__forceinline Fixed1616 GetB(const Color565& color) { return Fixed1616(color & Color565BMask, -Color565BShift); }

__forceinline Color565 MakeColor565(const Fixed1616& r, const Fixed1616& g, const Fixed1616& b)
{
    DWORD r565 = r.GetInteger(Color565RShift) & Color565RMask;
    DWORD g565 = g.GetInteger(Color565GShift) & Color565GMask;
    DWORD b565 = b.GetInteger(Color565BShift) & Color565BMask;

    return Color565(r565 | g565 | b565);
}

__forceinline Color565 operator*(Color565 color, const Fixed1616& i)
{
    /*
    return 
        MakeColor565(
            (GetR(color) * i1616),
            (GetG(color) * i1616),
            (GetB(color) * i1616)
       );
    */

    DWORD r565 = ((GetR(color).GetFixed() * i.GetFixed()) >> 32 - Color565RShift) & Color565RMask;
    DWORD g565 = ((GetG(color).GetFixed() * i.GetFixed()) >> 32 - Color565GShift) & Color565GMask;
    DWORD b565 = ((GetB(color).GetFixed() * i.GetFixed()) >> 32 - Color565BShift) & Color565BMask;

    return Color565(r565 | g565 | b565);
}

__forceinline Color565 operator*(Color565 color, const Fixed131& i)
{
    return color * Fixed1616(i.GetFixed(), -i.FractionBits());
}

//////////////////////////////////////////////////////////////////////////////
//
// ColorE Helpers
//
//////////////////////////////////////////////////////////////////////////////

__forceinline Fixed1616 GetR(const ColorE& color) { return Fixed1616(color & ColorERMask, -ColorERShift); }
__forceinline Fixed1616 GetG(const ColorE& color) { return Fixed1616(color & ColorEGMask, -ColorEGShift); }
__forceinline Fixed1616 GetB(const ColorE& color) { return Fixed1616(color & ColorEBMask, -ColorEBShift); }

__forceinline ColorE MakeColorE(const Fixed1616& r, const Fixed1616& g, const Fixed1616& b)
{
    return 
          (r.GetInteger(ColorERShift) & ColorERMask)
        | (g.GetInteger(ColorEGShift) & ColorEGMask)
        | (b.GetInteger(ColorEBShift) & ColorEBMask);
}

__forceinline ColorE Color555ToColorE(Color555 color)
{
    DWORD dw = color;

    return
        ColorE(
              ShiftLeft(dw & Color555RMask, ColorERShift - Color555RShift)
            | ShiftLeft(dw & Color555GMask, ColorEGShift - Color555GShift)
            | ShiftLeft(dw & Color555BMask, ColorEBShift - Color555BShift)
        );
}

__forceinline Color555 ColorEToColor555(ColorE color)
{
    DWORD dw = color;

    return
        Color555(
              (ShiftLeft(dw, Color555RShift - ColorERShift) & Color555RMask)
            | (ShiftLeft(dw, Color555GShift - ColorEGShift) & Color555GMask)
            | (ShiftLeft(dw, Color555BShift - ColorEBShift) & Color555BMask)
        );
}

__forceinline ColorE Color565ToColorE(Color565 color)
{
    DWORD dw = color;

    return
        ColorE(
              ShiftLeft(dw & Color565RMask, ColorERShift - Color565RShift)
            | ShiftLeft(dw & Color565GMask, ColorEGShift - Color565GShift)
            | ShiftLeft(dw & Color565BMask, ColorEBShift - Color565BShift)
        );
}

__forceinline Color565 ColorEToColor565(ColorE color)
{
    DWORD dw = color;

    return
        Color565(
              (ShiftLeft(dw, Color565RShift - ColorERShift) & Color565RMask)
            | (ShiftLeft(dw, Color565GShift - ColorEGShift) & Color565GMask)
            | (ShiftLeft(dw, Color565BShift - ColorEBShift) & Color565BMask)
        );
}

__forceinline ColorE operator*(ColorE colorE, const Fixed1616& i)
{
    /*
    return
        MakeColorE(
            GetR(colorE) * i,
            GetG(colorE) * i,
            GetB(colorE) * i
        );
    */

    DWORD r = ((GetR(colorE).GetFixed() * i.GetFixed()) >> 32 - ColorERShift) & ColorERMask;
    DWORD g = ((GetG(colorE).GetFixed() * i.GetFixed()) >> 32 - ColorEGShift) & ColorEGMask;
    DWORD b = ((GetB(colorE).GetFixed() * i.GetFixed()) >> 32 - ColorEBShift) & ColorEBMask;

    return ColorE(r | g | b);
}

__forceinline ColorE operator*(ColorE colorE, const Fixed131& i)
{
    return colorE * Fixed1616(i.GetFixed(), -i.FractionBits());
}

//////////////////////////////////////////////////////////////////////////////
//
// Constants
//
//////////////////////////////////////////////////////////////////////////////

#define ChunkSize      16
#define MaxChunkPasses 10

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class SoftwareRasterizer : public Rasterizer {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef void (SoftwareRasterizer::*PFNDrawTriangle)(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const VertexScreen* pvc
    );

    typedef void (SoftwareRasterizer::*PFNDrawLine)(
        const VertexScreen* pva,
        const VertexScreen* pvb
    );

    typedef void (SoftwareRasterizer::*PFNDrawPoint)(
        const VertexScreen* pva
    );

    typedef void (SoftwareRasterizer::*PFNFillSubTriangle)(
        const int ymin,
        const int ymax
    );

    typedef void (SoftwareRasterizer::*PFNFillChunk)();

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pipeline
    //
    //////////////////////////////////////////////////////////////////////////////

    bool                 m_bUpdatePointers;
    bool                 m_bColorKeyOn;
    PFNDrawTriangle      m_pfnDrawTriangle;
    PFNDrawLine          m_pfnDrawLine;
    PFNDrawPoint         m_pfnDrawPoint;
    PFNFillSubTriangle   m_pfnFillSubTriangle;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Plane deltas
    //
    //////////////////////////////////////////////////////////////////////////////

    float                m_floatdudx;
    float                m_floatdudy;

    float                m_floatdvdx;
    float                m_floatdvdy;

    float                m_floatdidx;
    float                m_floatdidy;

    float                m_floatdzdx;
    float                m_floatdzdy;

    Fixed                m_dudx;
    Fixed                m_dvdx;
    FixedD131            m_didx;
    FixedDZ              m_dzdx;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Edge deltas
    //
    //////////////////////////////////////////////////////////////////////////////

    int                  m_xmin         ;
    int                  m_dxmindyLittle;
    int                  m_dxmindyBig   ;
    float                m_xminFrac     ;
    float                m_dxmindyFrac  ;

    int                  m_xmax         ;
    int                  m_dxmaxdyLittle;
    int                  m_dxmaxdyBig   ;
    float                m_xmaxFrac     ;
    float                m_dxmaxdyFrac  ;

    Fixed                m_u            ;
    Fixed                m_dudyLittle   ;
    Fixed                m_dudyBig      ;

    Fixed                m_v            ;
    Fixed                m_dvdyLittle   ;
    Fixed                m_dvdyBig      ;

    Fixed131             m_i            ;
    FixedD131            m_didyLittle   ;
    FixedD131            m_didyBig      ;

    FixedZ               m_z            ;
    FixedDZ              m_dzdyLittle   ;
    FixedDZ              m_dzdyBig      ;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Performance
    //
    //////////////////////////////////////////////////////////////////////////////

    float                m_pixels;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Render state
    //
    //////////////////////////////////////////////////////////////////////////////

    bool                 m_bZTest;
    bool                 m_bZWrite;
    bool                 m_bDither;
    bool                 m_bLinearFilter;
    bool                 m_bPerspectiveCorrection;
    bool                 m_bColorKeyEnabled;
    ShadeMode            m_shadeMode;
    BlendMode            m_blendMode;
    WrapMode             m_wrapMode;
    CullMode             m_cullMode;
    Rect                 m_rectClip;
    ColorE               m_colorE;
    Fixed1616            m_colorR;
    Fixed1616            m_colorG;
    Fixed1616            m_colorB;
    Fixed1616            m_colorA;
    Fixed1616            m_colorInvA;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Surface
    //
    //////////////////////////////////////////////////////////////////////////////

    Surface*             m_psurface;
    BYTE*                m_pbSurface;
    DWORD                m_pitchSurface;
    WinPoint             m_sizeSurface;
    bool                 m_b565;
    bool                 m_b888; // KGJV 32B

    //////////////////////////////////////////////////////////////////////////////
    //
    // ZBuffer
    //
    //////////////////////////////////////////////////////////////////////////////

    BYTE*                m_pbZBuffer;
    DWORD                m_pitchZBuffer;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Texture
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface>        m_psurfaceTexture;
    BYTE*                m_pbTexture;
    DWORD                m_pitchTexture;
    WinPoint             m_sizeTexture;

    float                m_uscale;
    float                m_vscale;
    DWORD                m_ushift;
    DWORD                m_vshift;
    DWORD                m_umask;
    DWORD                m_vmask;
    bool                 m_bColorKey;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    bool                 m_bInScene;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    SoftwareRasterizer(PrivateSurface* psurface) :
        m_psurface(psurface),
        m_pixels(0),
        m_bInScene(false),
        m_bUpdatePointers(true),
        m_bZTest(false),
        m_bZWrite(false),
        m_bDither(false),
        m_bLinearFilter(false),
        m_bPerspectiveCorrection(false),
        m_bColorKeyEnabled(false),
        m_shadeMode(ShadeModeCopy),
        m_blendMode(BlendModeSource),
        m_wrapMode(WrapModeNone),
        m_cullMode(CullModeNone),
        m_pbSurface(NULL),
        m_pbZBuffer(NULL),
        m_pbTexture(NULL),
        m_pitchZBuffer(0)
    {
        PixelFormat* ppf = m_psurface->GetPixelFormat();

        // KGJV 32B
        //ZAssert(ppf->RedSize()  == 0x1f);
        //ZAssert(ppf->BlueSize() == 0x1f);

        // KGJV 32B TODO: seems we dont need this anymore since we going full D3DRasterizer
        if (ppf->PixelBits() == 32)
        {
            m_b888 = true;
        }

        if (ppf->GreenSize() == 0x1f) {
            m_b565 = false;
        } else {
            //ZAssert(ppf->GreenSize() == 0x3f);
            // KGJV 32B
            m_b565 = true;
        }
    }

    ~SoftwareRasterizer()
    {
        if (m_pbZBuffer) {
            delete m_pbZBuffer;
        }
    }

    bool IsValid()
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Termination
    //
    //////////////////////////////////////////////////////////////////////////////

    void Terminate()
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Scene
    //
    //////////////////////////////////////////////////////////////////////////////

    void BeginScene()
    {
        m_bInScene     = true;
        m_pbSurface    = m_psurface->GetWritablePointer();
        m_pitchSurface = m_psurface->GetPitch();
        m_sizeSurface  = m_psurface->GetSize();

        UpdateZBuffer();
        UpdateTextureInfo();

        m_bUpdatePointers = true;
    }

    void EndScene()
    {
        m_bInScene = false;
        m_psurface->ReleasePointer();
        if (m_psurfaceTexture) {
            m_psurfaceTexture->ReleasePointer();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Viewport
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetClipRect(const Rect& rectClip)
    {
        m_rectClip = rectClip;
    }

    void ClearZBuffer()
    {
        if (m_pbZBuffer) {
            memset(
                m_pbZBuffer, 
                0xff, 
                  m_pitchSurface
                * m_sizeSurface.Y()
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////////////////////////

    bool Has3DAcceleration()
    {
        return false;
    }

    Point GetSurfaceSize()
    {
        return Point::Cast(m_psurface->GetSize());
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // State
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetTexture(Surface* psurfaceTextureArg)
    {
        if (m_bInScene && m_psurfaceTexture) {
            m_psurfaceTexture->ReleasePointer();
        }

        CastTo(m_psurfaceTexture, psurfaceTextureArg);

        if (m_bInScene) {
            UpdateTextureInfo();
        }

        m_bUpdatePointers = true;
    }

    void SetShadeMode(ShadeMode shadeMode)
    {
        m_shadeMode       = shadeMode;
        m_bUpdatePointers = true;
    }

    void SetBlendMode(BlendMode blendMode)
    {
        m_blendMode       = blendMode;
        m_bUpdatePointers = true;
    }

    void SetWrapMode(WrapMode wrapMode)
    {
        m_wrapMode        = wrapMode;
        m_bUpdatePointers = true;
    }

    void SetCullMode(CullMode cullMode)
    {
        m_cullMode        = cullMode;
        m_bUpdatePointers = true;
    }

    void SetZTest(bool bZTest)
    {
        m_bZTest          = bZTest;
        m_bUpdatePointers = true;
    }

    void SetZWrite(bool bZWrite)
    {
        m_bZWrite         = bZWrite;
        m_bUpdatePointers = true;
    }

    void SetLinearFilter(bool bLinearFilter)
    {
        m_bLinearFilter   = bLinearFilter;
        m_bUpdatePointers = true;
    }

    void SetPerspectiveCorrection(bool bPerspectiveCorrection)
    {
        m_bPerspectiveCorrection = bPerspectiveCorrection;
        m_bUpdatePointers        = true;
    }

    void SetDither(bool bDither)
    {
        m_bDither         = bDither;
        m_bUpdatePointers = true;
    }

    void SetColorKey(bool bColorKey)
    {
        m_bColorKeyEnabled = bColorKey;
        m_bUpdatePointers  = true;
    }

    void SetColor(const D3DCOLOR& color)
    {
        m_colorA    = Fixed1616(color & 0xff000000, -32);
        m_colorR    = Fixed1616(color & 0x00ff0000, -24);
        m_colorG    = Fixed1616(color & 0x0000ff00, -16);
        m_colorB    = Fixed1616(color & 0x000000ff, - 8);
        m_colorInvA = Fixed1616(255 - (color >> 24), - 8);

        m_colorE = MakeColorE(m_colorR, m_colorG, m_colorB);
    }

    void SetColor(const Color& color)
    {
        m_colorR    = Fixed1616(color.GetRed  () * g_componentFloatToIntScale);
        m_colorG    = Fixed1616(color.GetGreen() * g_componentFloatToIntScale);
        m_colorB    = Fixed1616(color.GetBlue () * g_componentFloatToIntScale);
        m_colorA    = Fixed1616(color.GetAlpha() * g_componentFloatToIntScale);
        m_colorInvA = Fixed1616(1.0f - color.GetAlpha());

        m_colorE = MakeColorE(m_colorR, m_colorG, m_colorB);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Chunk Pass Members
    //
    //////////////////////////////////////////////////////////////////////////////

    PFNFillChunk m_ppfnFillChunk[MaxChunkPasses];
    int          m_passCount;

    WORD         m_wZBufferMask;
    int          m_count;

    ColorE       m_pcolorEChunk[ChunkSize];
    ColorE       m_pcolorEDest[ChunkSize];
    BYTE*        m_pzChunk;
    BYTE*        m_pdest;

    Fixed        m_uChunk;
    Fixed        m_vChunk;
    Fixed131     m_iChunk;
    FixedZ       m_zChunk;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Chunk Pass Helpers
    //
    //////////////////////////////////////////////////////////////////////////////

    __forceinline int MakeIndex(int index)
    {
        return index + ChunkSize;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // This function works for both CK and not CK since Blend11 with a black 
    // source pixel is the same as color keying.
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteBlend11()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD    xt        = ((u.GetFixed() >> Fixed::FractionBits()) & m_umask) << 1;
                DWORD    yt        = ((v.GetFixed() >> Fixed::FractionBits()) & m_vmask) << (m_ushift + 1);
                Color565 color565T = *(Color565*)(m_pbTexture + xt + yt);

                if (color565T != 0) {
                    pz[MakeIndex(index)] = zs;

                    Color565 color565D = pcolor565[MakeIndex(index)];
                    ColorE   colorET   = Color565ToColorE(color565T);
                    ColorE   colorED   = Color565ToColorE(color565D);
                    ColorE   colorE    = Blend11(colorET, colorED);
                    Color565 color565  = ColorEToColor565(colorE);

                    pcolor565[MakeIndex(index)] = color565;
                }
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // This function works for both CK and not CK since Blend11 with a black 
    // source pixel is the same as color keying.
    //
    //////////////////////////////////////////////////////////////////////////////

    void FlatTextureZTestZWriteBlend11()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD    xt        = ((u.GetFixed() >> Fixed::FractionBits()) & m_umask) << 1;
                DWORD    yt        = ((v.GetFixed() >> Fixed::FractionBits()) & m_vmask) << (m_ushift + 1);
                Color565 color565T = *(Color565*)(m_pbTexture + xt + yt);

                if (color565T != 0) {
                    pz[MakeIndex(index)] = zs;

                    Color565 color565D = pcolor565[MakeIndex(index)];
                    ColorE   colorET   = FlatShade(Color565ToColorE(color565T));
                    ColorE   colorED   = Color565ToColorE(color565D);
                    ColorE   colorE    = Blend11(colorET, colorED);
                    Color565 color565  = ColorEToColor565(colorE);

                    pcolor565[MakeIndex(index)] = color565;
                }
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // This function works for both CK and not CK since Blend11 with a black 
    // source pixel is the same as color keying.
    //
    //////////////////////////////////////////////////////////////////////////////

    void FlatBlend1InvA()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        int       index     = m_count;

        do {
            Color565 color565D = pcolor565[MakeIndex(index)];
            ColorE   colorD    = Color565ToColorE(color565D);
            ColorE   colorE    = m_colorE + colorD * m_colorInvA;
            Color565 color565  = ColorEToColor565(colorE);
                               
            pcolor565[MakeIndex(index)] = color565;

            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteAndColorKey()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                if (color565 != 0) {
                    pz[MakeIndex(index)] = zs;
                    pcolor565[MakeIndex(index)] = color565;
                }
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZWriteAndColorKey()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                if (color565 != 0) {
                    if ((mask & 1) == 0) {
                        WORD zs = WORD(z.GetInteger(16));
                        pz[MakeIndex(index)] = zs;
                        pcolor565[MakeIndex(index)] = color565;
                    }
                }
            }

            mask     = mask >> 1;
            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void LoadColor565Texture()
    {
        ColorE* pcolorE = m_pcolorEChunk;
        Fixed   u       = m_uChunk;
        Fixed   v       = m_vChunk;
        int     index   = m_count;

        do {
            DWORD xt = (u.GetInteger(1)                  & m_umask);
            DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

            Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);
            ColorE   colorE   = Color565ToColorE(color565);

            pcolorE[MakeIndex(index)] = colorE;

            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Load565Dest()
    {
        ColorE*   pcolorE   = m_pcolorEDest;
        Color565* pcolor565 = (Color565*)m_pdest;
        int       index     = m_count;

        do {
            Color565 color565 = pcolor565[MakeIndex(index)];
            ColorE   colorE   = Color565ToColorE(color565);

            pcolorE[MakeIndex(index)] = colorE;

            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Load555Dest()
    {
        ColorE*   pcolorE   = m_pcolorEDest;
        Color555* pcolor555 = (Color555*)m_pdest;
        int       index     = m_count;

        do {
            Color555 color555 = pcolor555[MakeIndex(index)];;
            ColorE   colorE   = Color555ToColorE(color555);

            pcolorE[MakeIndex(index)] = colorE;

            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void LoadFlatColor()
    {
        ColorE* pcolorE = m_pcolorEChunk;
        int     index   = m_count;

        do {
            pcolorE[MakeIndex(index)] = m_colorE;

            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteFlat()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                pz[MakeIndex(index)] = zs;
                pcolor565[MakeIndex(index)] = 
                    MakeColor565(
                        GetR(color565) * m_colorR,
                        GetG(color565) * m_colorG,
                        GetB(color565) * m_colorB
                   );
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteFlatAndColorKey()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                if (color565 != 0) {
                    pz[MakeIndex(index)] = zs;
                    pcolor565[MakeIndex(index)] = 
                        MakeColor565(
                            GetR(color565) * m_colorR,
                            GetG(color565) * m_colorG,
                            GetB(color565) * m_colorB
                       );
                }
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    ColorE FlatShade(const ColorE& colorS)
    {
        return
            MakeColorE(
                GetR(colorS) * m_colorR,
                GetG(colorS) * m_colorG,
                GetB(colorS) * m_colorB
            );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void FlatShade()
    {
        ColorE*   pcolorE   = m_pcolorEChunk;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        do {
            if ((mask & 1) == 0) {
                pcolorE[MakeIndex(index)] = FlatShade(pcolorE[MakeIndex(index)]);
            }

            mask  =  mask >> 1;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteGouraud()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        Fixed131  i         = m_iChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                pz[MakeIndex(index)] = zs;

                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);
   
                pcolor565[MakeIndex(index)] = color565 * i;
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            i       += m_didx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyTextureZTestZWriteGouraudAndColorKey()
    {
        Color565* pcolor565 = (Color565*)m_pdest;
        Fixed     u         = m_uChunk;
        Fixed     v         = m_vChunk;
        Fixed131  i         = m_iChunk;
        FixedZ    z         = m_zChunk;
        WORD*     pz        = (WORD*)m_pzChunk;
        int       index     = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                DWORD xt = (u.GetInteger(1)                  & m_umask);
                DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                if (color565 != 0) {
                    pz[MakeIndex(index)] = zs;
                    pcolor565[MakeIndex(index)] = color565 * i;
                }
            }

            z       += m_dzdx;
            u       += m_dudx;
            v       += m_dvdx;
            i       += m_didx;
            index   ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void GouraudShade()
    {
        ColorE*   pcolorE   = m_pcolorEChunk;
        Fixed131  i         = m_iChunk;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        do {
            if ((mask & 1) == 0) {
                ColorE colorS = pcolorE[MakeIndex(index)];
                pcolorE[MakeIndex(index)] = colorS * i;
            }

            mask   = mask >> 1;
            i     += m_didx;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void LoadGouraudColor()
    {
        ColorE*   pcolorE   = m_pcolorEChunk;
        Fixed131  i         = m_iChunk;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        do {
            if ((mask & 1) == 0) {
                Fixed1616 i1616 = Fixed1616(i.GetFixed(), -i.FractionBits());
                pcolorE[MakeIndex(index)] = MakeColorE(i1616, i1616, i1616);
            }

            mask   = mask >> 1;
            i     += m_didx;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    ColorE Blend11(const ColorE& c1, const ColorE& c2)
    {
        ColorE color = c1 + c2;

        if ((color & ColorESaturationMask) != 0) {
            if ((color & ColorERSaturationMask) != 0) {
                color |= ColorERMask;
            }
            if ((color & ColorEGSaturationMask) != 0) {
                color |= ColorEGMask;
            }
            if ((color & ColorEBSaturationMask) != 0) {
                color |= ColorEBMask;
            }
        }

        return color;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Blend11()
    {
        ColorE*   pcolorE     = m_pcolorEChunk;
        ColorE*   pcolorEDest = m_pcolorEDest;
        WORD      mask        = m_wZBufferMask;
        int       index       = m_count;

        do {
            if ((mask & 1) == 0) {
                ColorE colorS = pcolorE    [MakeIndex(index)];
                ColorE colorD = pcolorEDest[MakeIndex(index)];

                ColorE color = Blend11(colorS, colorD);

                pcolorE[MakeIndex(index)] = color;
            }

            mask   = mask >> 1;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Blend1InvA()
    {
        ColorE*   pcolorE     = m_pcolorEChunk;
        ColorE*   pcolorEDest = m_pcolorEDest;
        WORD      mask        = m_wZBufferMask;
        int       index       = m_count;

        do {
            if ((mask & 1) == 0) {
                ColorE colorS = pcolorE    [MakeIndex(index)];
                ColorE colorD = pcolorEDest[MakeIndex(index)];

                pcolorE[MakeIndex(index)] = colorS + colorD * m_colorInvA;
            }

            mask   = mask >> 1;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyToColor565Destination()
    {
        ColorE*   pcolorE   = m_pcolorEChunk;
        Color565* pcolor565 = (Color565*)m_pdest;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        if (mask == 0) {
            do {
                pcolor565[MakeIndex(index)] = ColorEToColor565(pcolorE[MakeIndex(index)]);
                index++;
            } while (index < 0);
        } else {
            do {
                if ((mask & 1) == 0) {
                    pcolor565[MakeIndex(index)] = ColorEToColor565(pcolorE[MakeIndex(index)]);
                }

                mask  =  mask >> 1;
                index ++;
            } while (index < 0);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CopyToColor555Destination()
    {
        ColorE*   pcolorE   = m_pcolorEChunk;
        Color555* pcolor555 = (Color555*)m_pdest;
        WORD      mask      = m_wZBufferMask;
        int       index     = m_count;

        if (mask == 0) {
            do {
                pcolor555[MakeIndex(index)] = ColorEToColor555(pcolorE[MakeIndex(index)]);
                index++;
            } while (index < 0);
        } else {
            do {
                if ((mask & 1) == 0) {
                    pcolor555[MakeIndex(index)] = ColorEToColor555(pcolorE[MakeIndex(index)]);
                }

                mask  =  mask >> 1;
                index++;
            } while (index < 0);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ZTestAndWrite()
    {
        FixedZ    z     = m_zChunk;
        WORD*     pz    = (WORD*)m_pzChunk;
        WORD      bit   = 1;
        int       index = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (zs <= zd) {
                pz[MakeIndex(index)] = zs;
            } else {
                m_wZBufferMask |= bit;    
            }

            bit    = bit << 1;
            z     += m_dzdx;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ZTest()
    {
        FixedZ    z     = m_zChunk;
        WORD*     pz    = (WORD*)m_pzChunk;
        WORD      bit   = 1;
        int       index = m_count;

        do {
            WORD zs = WORD(z.GetInteger(16));
            WORD zd = pz[MakeIndex(index)];

            if (!(zs <= zd)) {
                m_wZBufferMask |= bit;    
            }

            bit    = bit << 1;
            z     += m_dzdx;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ZWrite()
    {
        FixedZ z     = m_zChunk;
        WORD*  pz    = (WORD*)m_pzChunk;
        WORD   mask  = m_wZBufferMask;
        int    index = m_count;

        do {
            if ((mask & 1) == 0) {
                WORD zs = WORD(z.GetInteger(16));
                pz[MakeIndex(index)] = zs;
            }

            mask   = mask >> 1;
            z     += m_dzdx;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void ColorKeyTest()
    {
        ColorE*   pcolorE = m_pcolorEChunk;
        WORD      bit     = 1;
        int       index   = m_count;

        do {
            ColorE colorE = pcolorE[MakeIndex(index)];

            if (colorE == 0) {
                m_wZBufferMask |= bit;
            }
            
            bit    = bit << 1;
            index ++;
        } while (index < 0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DoPasses()
    {
        for(int pass = 0; pass < m_passCount; pass++) {
            (this->*(m_ppfnFillChunk[pass]))();

            if (m_wZBufferMask == 0xffff) {
                break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void FillSubTriangleMultiPassChunk(
        const int ymin,
        const int ymax
    ) {
        //
        // fill the triangle
        //

        BYTE* pscan  = m_pbSurface + ymin * m_pitchSurface;
        BYTE* pzscan = m_pbZBuffer + ymin * m_pitchSurface;

        for (int y = ymin; y < ymax; y++) {
            int count = m_xmax - m_xmin;

            if (count > 0) {
                //
                // Initial values
                //

                m_pdest        = pscan  + m_xmin * 2;
                m_pzChunk      = pzscan + m_xmin * 2;
                m_uChunk       = m_u;
                m_vChunk       = m_v;
                m_iChunk       = m_i;
                m_zChunk       = m_z;

                //
                // ChunkSize pixel chunks
                //

                m_count = -ChunkSize;

                while (count >= ChunkSize) {
                    m_wZBufferMask = 0;
                    DoPasses();

                    m_pdest   += 2 * ChunkSize;
                    m_pzChunk += 2 * ChunkSize;
                    m_uChunk  += m_dudx * ChunkSize;
                    m_vChunk  += m_dvdx * ChunkSize;
                    m_iChunk  += m_didx * ChunkSize;
                    m_zChunk  += m_dzdx * ChunkSize;
                    count     -= ChunkSize;
                }

                //
                // Remaining pixels
                //

                if (count > 0) {
                    int delta = ChunkSize - count;

                    m_count    = -count;
                    m_pdest   -= 2 * delta;
                    m_pzChunk -= 2 * delta;

                    m_wZBufferMask = 0;
                    DoPasses();
                }
            }

            //
            // y++
            //

            pscan      += m_pitchSurface;
            pzscan     += m_pitchSurface;

            m_xminFrac += m_dxmindyFrac;

            if (m_xminFrac >= 0) {
                m_xminFrac -= 1;

                m_xmin += m_dxmindyBig;
                m_u    += m_dudyBig;
                m_v    += m_dvdyBig;
                m_i    += m_didyBig;
                m_z    += m_dzdyBig;
            } else {
                m_xmin += m_dxmindyLittle;
                m_u    += m_dudyLittle;
                m_v    += m_dvdyLittle;
                m_i    += m_didyLittle;
                m_z    += m_dzdyLittle;
            }

            m_xmaxFrac += m_dxmaxdyFrac;

            if (m_xmaxFrac >= 0) {
                m_xmaxFrac -= 1;

                m_xmax += m_dxmaxdyBig;
            } else {
                m_xmax += m_dxmaxdyLittle;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void FillSubTriangleMultiPass(
        const int ymin,
        const int ymax
    ) {
        //
        // fill the triangle
        //

        BYTE* pscan  = m_pbSurface + ymin * m_pitchSurface;
        BYTE* pzscan = m_pbZBuffer + ymin * m_pitchSurface;

        for (int y = ymin; y < ymax; y++) {
            int count = m_xmax - m_xmin;

            if (count > 0) {
                //
                // Initial values
                //

                m_pdest        = pscan  + m_xmin * 2;
                m_pzChunk      = pzscan + m_xmin * 2;
                m_uChunk       = m_u;
                m_vChunk       = m_v;
                m_iChunk       = m_i;
                m_zChunk       = m_z;

                //
                // Call the scan routine
                //

                int delta = ChunkSize - count;

                m_count    = -count;
                m_pdest   -= 2 * delta;
                m_pzChunk -= 2 * delta;

                DoPasses();
            }

            //
            // y++
            //

            pscan      += m_pitchSurface;
            pzscan     += m_pitchSurface;

            m_xminFrac += m_dxmindyFrac;

            if (m_xminFrac >= 0) {
                m_xminFrac -= 1;

                m_xmin += m_dxmindyBig;
                m_u    += m_dudyBig;
                m_v    += m_dvdyBig;
                m_i    += m_didyBig;
                m_z    += m_dzdyBig;
            } else {
                m_xmin += m_dxmindyLittle;
                m_u    += m_dudyLittle;
                m_v    += m_dvdyLittle;
                m_i    += m_didyLittle;
                m_z    += m_dzdyLittle;
            }

            m_xmaxFrac += m_dxmaxdyFrac;

            if (m_xmaxFrac >= 0) {
                m_xmaxFrac -= 1;

                m_xmax += m_dxmaxdyBig;
            } else {
                m_xmax += m_dxmaxdyLittle;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void FillSubTriangleTextureZTestZWriteAndColorKey(
        const int ymin,
        const int ymax
    ) {
        //
        // fill the triangle
        //

        BYTE* pscan  = m_pbSurface + ymin * m_pitchSurface;
        BYTE* pzscan = m_pbZBuffer + ymin * m_pitchSurface;

        for (int y = ymin; y < ymax; y++) {
            int count = m_xmax - m_xmin;

            if (count > 0) {
                //
                // Initial values
                //

                Color565* pcolor565 = (Color565*)(pscan  + m_xmin * 2);
                WORD*     pz        = (WORD*)(pzscan + m_xmin * 2);
                Fixed     u         = m_u;
                Fixed     v         = m_v;
                FixedZ    z         = m_z;

                //
                // Call the scan routine
                //

                int index = 0;

                if (m_bColorKeyOn) {
                    do {
                        WORD zs = WORD(z.GetInteger(16));
                        WORD zd = pz[index];

                        if (zs <= zd) {
                            DWORD xt = (u.GetInteger(1)                  & m_umask);
                            DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                            Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                            if (color565 != 0) {
                                pz[index] = zs;
                                pcolor565[index] = color565;
                            }
                        }

                        z       += m_dzdx;
                        u       += m_dudx;
                        v       += m_dvdx;
                        index   ++;
                    } while (index < count);
                } else {
                    do {
                        WORD zs = WORD(z.GetInteger(16));
                        WORD zd = pz[index];

                        if (zs <= zd) {
                            DWORD xt = (u.GetInteger(1)                  & m_umask);
                            DWORD yt = (v.GetInteger(m_ushift + 1, true) & m_vmask);

                            Color565 color565 = *(Color565*)(m_pbTexture + xt + yt);

                            pz[index] = zs;
                            pcolor565[index] = color565;
                        }

                        z       += m_dzdx;
                        u       += m_dudx;
                        v       += m_dvdx;
                        index   ++;
                    } while (index < count);
                }
            }

            //
            // y++
            //

            pscan      += m_pitchSurface;
            pzscan     += m_pitchSurface;

            m_xminFrac += m_dxmindyFrac;

            if (m_xminFrac >= 0) {
                m_xminFrac -= 1;

                m_xmin += m_dxmindyBig;
                m_u    += m_dudyBig;
                m_v    += m_dvdyBig;
                m_z    += m_dzdyBig;
            } else {
                m_xmin += m_dxmindyLittle;
                m_u    += m_dudyLittle;
                m_v    += m_dvdyLittle;
                m_z    += m_dzdyLittle;
            }

            m_xmaxFrac += m_dxmaxdyFrac;

            if (m_xmaxFrac >= 0) {
                m_xmaxFrac -= 1;

                m_xmax += m_dxmaxdyBig;
            } else {
                m_xmax += m_dxmaxdyLittle;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void FillSubTriangleNoop(
        const int ymin,
        const int ymax
    ) {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateZBuffer()
    {
        if (m_psurface->GetSurfaceType().Test(SurfaceTypeZBuffer())) {
            if (m_pitchZBuffer != m_pitchSurface) {
                m_pitchZBuffer = m_pitchSurface;

                if (m_pbZBuffer) {
                    delete m_pbZBuffer;
                }

                m_pbZBuffer = 
                    new BYTE[
                          m_pitchSurface
                        * m_sizeSurface.Y()
                    ];
            }
        } else {
            if (m_pbZBuffer) {
                delete m_pbZBuffer;
            }

            m_pbZBuffer = NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateTextureInfo()
    {
        if (m_psurfaceTexture) {
            m_pbTexture    = m_psurfaceTexture->GetWritablePointer();
            m_pitchTexture = m_psurfaceTexture->GetPitch();
            m_sizeTexture  = m_psurfaceTexture->GetSize();

            m_uscale       = (float)m_sizeTexture.X();
            m_vscale       = (float)m_sizeTexture.Y();
            m_ushift       = GetShift(m_sizeTexture.X());
            m_vshift       = GetShift(m_sizeTexture.Y());
            m_umask        = MakeMask(m_ushift, 1);
            m_vmask        = MakeMask(m_vshift, m_ushift + 1);
            m_bColorKey    = m_psurfaceTexture->HasColorKey();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcXMin(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const int           ymin
    ) {
        //
        // calculate ydelta
        //

        const float qdy    = 1 / (pvb->y - pva->y);
        const float yDelta = (float(ymin) + 0.5f) - pva->y;

        //
        // calculate dxmindy
        //

        const float dxmindy       = (pvb->x - pva->x) * qdy;
        const float xminIntercept = pva->x + dxmindy * yDelta;
                    m_xmin        = MakeInt(xminIntercept);
        const float xDelta        = (float(m_xmin) + 0.5f) - pva->x;

        if (GreaterEqualZero(dxmindy)) {
            m_dxmindyLittle = MakeInt(dxmindy - 0.5f);
            m_dxmindyBig    = m_dxmindyLittle + 1;
            m_dxmindyFrac   = dxmindy - float(m_dxmindyLittle);
            m_xminFrac      = xminIntercept - (float(m_xmin) + 0.5f);
        } else {
            m_dxmindyLittle = MakeInt(dxmindy + 0.5f);
            m_dxmindyBig    = m_dxmindyLittle - 1;
            m_dxmindyFrac   = float(m_dxmindyLittle) - dxmindy;
            m_xminFrac      = (float(m_xmin) - 0.5f) - xminIntercept;
        }

        //
        // calculate qu, qv, and qw deltas
        //

        m_dudyLittle = Fixed(m_uscale * (m_floatdudy + m_floatdudx * m_dxmindyLittle));
        m_dvdyLittle = Fixed(m_vscale * (m_floatdvdy + m_floatdvdx * m_dxmindyLittle));
        m_didyLittle = FixedD131(m_floatdidy + m_floatdidx * m_dxmindyLittle);
        m_dzdyLittle = FixedDZ  (m_floatdzdy + m_floatdzdx * m_dxmindyLittle);

        if (m_dxmindyBig >= 0) {
            m_dudyBig = m_dudyLittle + m_dudx;
            m_dvdyBig = m_dvdyLittle + m_dvdx;
            m_didyBig = m_didyLittle + m_didx;
            m_dzdyBig = m_dzdyLittle + m_dzdx;
        } else {
            m_dudyBig = m_dudyLittle - m_dudx;
            m_dvdyBig = m_dvdyLittle - m_dvdx;
            m_didyBig = m_didyLittle - m_didx;
            m_dzdyBig = m_dzdyLittle - m_dzdx;
        }

        //
        // Initial values
        //

        m_u = Fixed(m_uscale * (pva->u + m_floatdudy * yDelta + m_floatdudx * xDelta));
        m_v = Fixed(m_vscale * (pva->v + m_floatdvdy * yDelta + m_floatdvdx * xDelta));
        m_i = 
            Fixed131(
                  (float(pva->color & 0xff) * g_componentFloatToIntScale / 255.0f)
                + m_floatdidy * yDelta 
                + m_floatdidx * xDelta
            );
        m_z = FixedZ(pva->z + m_floatdzdy * yDelta + m_floatdzdx * xDelta);
   }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcXMax(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const int           ymax
    ) {
        //
        // calculate ydelta
        //

        const float qdy    = 1 / (pvb->y - pva->y);
        const float yDelta = (float(ymax) + 0.5f) - pva->y;

        //
        // calculate dxmaxdy
        //

        const float dxmaxdy       = (pvb->x - pva->x) * qdy;
        const float xmaxIntercept = pva->x + dxmaxdy * yDelta;
                    m_xmax        = MakeInt(xmaxIntercept);

        if (GreaterEqualZero(dxmaxdy)) {
            m_dxmaxdyLittle = MakeInt(dxmaxdy - 0.5f);
            m_dxmaxdyBig    = m_dxmaxdyLittle + 1;
            m_dxmaxdyFrac   = dxmaxdy - float(m_dxmaxdyLittle);
            m_xmaxFrac      = xmaxIntercept - (float(m_xmax) + 0.5f);
        } else {
            m_dxmaxdyLittle = MakeInt(dxmaxdy + 0.5f);
            m_dxmaxdyBig    = m_dxmaxdyLittle - 1;
            m_dxmaxdyFrac   = float(m_dxmaxdyLittle) - dxmaxdy;
            m_xmaxFrac      = (float(m_xmax) - 0.5f) - xmaxIntercept;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Calculate Interpolants
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalculateInterpolants(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const VertexScreen* pvc,
        float xabn,
        float yabn,
        float xacn,
        float yacn
    ) {
        float uab = pvb->u - pva->u;
        float uac = pvc->u - pva->u;
        m_floatdudy = uab * xacn - uac * xabn;
        m_floatdudx = uac * yabn - uab * yacn;
        m_dudx = Fixed(m_uscale * m_floatdudx);

        float vab = pvb->v - pva->v;
        float vac = pvc->v - pva->v;
        m_floatdvdy = vab * xacn - vac * xabn;
        m_floatdvdx = vac * yabn - vab * yacn;
        m_dvdx = Fixed(m_vscale * m_floatdvdx);

        int ia = int(pva->color & 0xff);
        int ib = int(pvb->color & 0xff);
        int ic = int(pvc->color & 0xff);

        float iab = (float(ib - ia) * g_componentFloatToIntScale / 255.0f);
        float iac = (float(ic - ia) * g_componentFloatToIntScale / 255.0f);
        m_floatdidy = iab * xacn - iac * xabn;
        m_floatdidx = iac * yabn - iab * yacn;
        m_didx = FixedD131(m_floatdidx);

        float zab = pvb->z - pva->z;
        float zac = pvc->z - pva->z;
        m_floatdzdy = zab * xacn - zac * xabn;
        m_floatdzdx = zac * yabn - zab * yacn;
        m_dzdx = FixedDZ(m_floatdzdx);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawFlatTriangle(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const VertexScreen* pvc
    ) {
        SetColor(pva->color);

        DrawTriangle(pva, pvb, pvc);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTriangle(
        const VertexScreen* pva,
        const VertexScreen* pvb,
        const VertexScreen* pvc
    ) {
        //
        // Get int y coordinates
        //

        float ya = pva->y;
        float yb = pvb->y;
        float yc = pvc->y;

        int iya;
        int iyb;
        int iyc;

        MakeIntMacro(ya, iya);
        MakeIntMacro(yb, iyb);
        MakeIntMacro(yc, iyc);

        {
            //
            // If the triangle has zero height there is nothing to draw.
            //

            int idyab = (iyb - iya);
            int idyac = (iyc - iya);
            int idybc = (iyc - iyb);

            if (idyab == 0 && idyac == 0 && idybc == 0) {
                return;
            }
        }

        //
        // Get int x coordinates
        //

        {
            float xa = pva->x;
            float xb = pvb->x;
            float xc = pvc->x;

            int ixa;
            int ixb;
            int ixc;

            MakeIntMacro(xa, ixa);
            MakeIntMacro(xb, ixb);
            MakeIntMacro(xc, ixc);

            int idxab = (ixb - ixa);
            int idxac = (ixc - ixa);
            int idxbc = (ixc - ixb);

            //
            // If the triangle has zero width there is nothing to draw.
            //

            if (idxab == 0 && idxac == 0 && idxbc == 0) {
                return;
            }
        }

        //
        // Calculate the triangles area
        //

        float xab  = pvb->x - pva->x;
        float yab  = pvb->y - pva->y;
        float xac  = pvc->x - pva->x;
        float yac  = pvc->y - pva->y;
        float area = yab * xac - yac * xab;

        //
        // Figure out which vertex is topmost.
        //

        const VertexScreen* pvaDraw;
        const VertexScreen* pvbDraw;
        const VertexScreen* pvcDraw;

        if (iya <= iyc && iya <= iyb) {
            pvaDraw = pva;
            pvbDraw = pvb;
            pvcDraw = pvc;
        } else if (iyc < iya && iyc < iyb) {
            pvaDraw = pvc;
            pvbDraw = pva;
            pvcDraw = pvb;

            int i = iya;
            iya = iyc;
            iyc = iyb;
            iyb = i;
        } else {
            pvaDraw = pvb;
            pvbDraw = pvc;
            pvcDraw = pva;

            int i = iya;
            iya = iyb;
            iyb = iyc;
            iyc = i;
        }

        //
        // If area is negative switch from ccw to cw
        //

        if (GreaterZero(area)) {
            if (m_cullMode == CullModeCCW) {
                return;
            }

            m_pixels += area;
        } else {
            if (m_cullMode == CullModeCW) {
                return;
            }

            m_pixels -= area;

            swap(pvbDraw, pvcDraw);
            swap(iyb, iyc);
        }

        //
        // Calculate the interpolants
        //

        float qarea = 1 / area;

        float xabn = xab * qarea;
        float yabn = yab * qarea;
        float xacn = xac * qarea;
        float yacn = yac * qarea;

        CalculateInterpolants(pva, pvb, pvc, xabn, yabn, xacn, yacn);

        //
        // Draw the subtriangles
        //

        if (iya == iyb) {
            CalcXMin(pvbDraw, pvcDraw, iyb);
            CalcXMax(pvaDraw, pvcDraw, iya);
            (this->*m_pfnFillSubTriangle)(iyb, iyc);
        } else if (iya == iyc) {
            CalcXMin(pvaDraw, pvbDraw, iya);
            CalcXMax(pvcDraw, pvbDraw, iyc);
            (this->*m_pfnFillSubTriangle)(iya, iyb);
        } else if (iyb == iyc) {
            CalcXMin(pvaDraw, pvbDraw, iya);
            CalcXMax(pvaDraw, pvcDraw, iya);
            (this->*m_pfnFillSubTriangle)(iya, iyb);
        } else if (iyb < iyc) {
            CalcXMin(pvaDraw, pvbDraw, iya);
            CalcXMax(pvaDraw, pvcDraw, iya);
            (this->*m_pfnFillSubTriangle)(iya, iyb);
            CalcXMin(pvbDraw, pvcDraw, iyb);
            (this->*m_pfnFillSubTriangle)(iyb, iyc);
        } else {
            CalcXMin(pvaDraw, pvbDraw, iya);
            CalcXMax(pvaDraw, pvcDraw, iya);
            (this->*m_pfnFillSubTriangle)(iya, iyc);
            CalcXMax(pvcDraw, pvbDraw, iyc);
            (this->*m_pfnFillSubTriangle)(iyc, iyb);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawLine(
        const VertexScreen* pva, 
        const VertexScreen* pvb
    ) {
        ZAssert(pva->color == pvb->color);

        //
        // snap to pixel centers
        //

        int xstart = FloorInt(pva->x);
        int ystart = FloorInt(pva->y);

        int xend   = FloorInt(pvb->x);
        int yend   = FloorInt(pvb->y);

        int xdelta = xend - xstart;
        int ydelta = yend - ystart;

        int error;
        int ddestMajor;
        int ddestMinor;
        int derrorMajor;
        int derrorMinor;
        int count;

        if (abs(xdelta) >= abs(ydelta)) {
            //
            // Handle zero sized lines
            //

            if (xdelta == 0) {
                return;
            }

            //
            // Mostly horizontal line
            //

            count = abs(xdelta);

            if (ydelta > 0) {
                ddestMinor  =  m_pitchSurface;
                derrorMajor =  ydelta;
            } else {
                ddestMinor  = -int(m_pitchSurface);
                derrorMajor = -ydelta;
            }

            if (xdelta > 0) {
                ddestMajor =   2;
                derrorMinor = -xdelta;
                error       = -xdelta / 2;
            } else {
                ddestMajor  = -2;
                derrorMinor =  xdelta;
                error       =  xdelta / 2;
            }
        } else {
            //
            // Mostly Vertical line.
            //

            count = abs(ydelta);

            if (xdelta > 0) {
                ddestMinor  =  2;
                derrorMajor =  xdelta;
            } else {
                ddestMinor  = -2;
                derrorMajor = -xdelta;
            }

            if (ydelta > 0) {
                ddestMajor   =  m_pitchSurface;
                derrorMinor  = -ydelta;
                error        = -ydelta / 2;
            } else {
                ddestMajor  = -int(m_pitchSurface);
                derrorMinor =  ydelta;
                error       =  ydelta / 2;
            }
        }

        //
        // Minor steps include major steps
        //

        derrorMinor += derrorMajor;
        ddestMinor  += ddestMajor;

        //
        // Draw the pixels
        //

        if (count > 0) {
            //
            // Get a pixel value from a D3DColor
            //

            WORD pixel;

            if (m_b565) {
                pixel =
                    (WORD)( 
                          ((pva->color >> 8) & 0xf800)
                        | ((pva->color >> 5) & 0x07e0)
                        | ((pva->color >> 3) & 0x001f)
                    );
            } else {
                pixel =
                    (WORD)( 
                          ((pva->color >> 9) & 0x7c00)
                        | ((pva->color >> 6) & 0x03e0)
                        | ((pva->color >> 3) & 0x001f)
                    );
            }

            //
            // Pre make the first major step
            //

            error += derrorMajor;

            //
            // Starting address
            //

            BYTE* pdest =
                  m_pbSurface 
                + xstart * 2
                + ystart * m_pitchSurface;

            //
            // Loop through the pixels
            //

            do {
                *(WORD*)pdest = pixel;
                if (error > 0) {
                    pdest += ddestMinor;
                    error += derrorMinor;
                } else {
                    pdest += ddestMajor;
                    error += derrorMajor;
                }
                count--;
            } while (count > 0);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // This code is subpixel accurate but sometimes doesn't draw the last pixel when it should
    //
    //////////////////////////////////////////////////////////////////////////////

    /*
    void DrawLine(
        const VertexScreen* pva, 
        const VertexScreen* pvb
    ) {
        ZAssert(pva->color == pvb->color);

        WORD color = WORD(pva->color);

        float xdelta = pvb->x - pva->x;
        float ydelta = pvb->y - pva->y;

        if (abs(xdelta) >= abs(ydelta)) {
            //
            // Handle zero sized line.
            //

            if (xdelta == 0.0f) {
                return;
            }

            //
            // Mostly horizontal line
            //

            int   xstart = FloorInt(pva->x);
            int   xend   = FloorInt(pvb->x);
            int   ystart = FloorInt(pva->y);

            int   ddestdx;
            int   ddestdy;

            if (xend > xstart) {
                ddestdx =  2;
            } else {
                ddestdx = -2;
                xdelta  = -xdelta;
            }

            float slope  = ydelta / xdelta;
            float e;

            if (slope == 0) {
                e = 0;
            } else {
                //
                // x distance from end point to starting pixel center
                //

                float xdeltaCenter  =  (float(xstart) + 0.5f - pva->x);

                //
                // y intercept of pixel x center line
                //

                float ycenter = pva->y + slope * xdeltaCenter;

                //
                // error
                //

                if (slope > 0) {
                    //
                    // error = distance of intercept from pixel max
                    //

                    e       = ycenter - (float(ystart) + 1.0f);
                    ddestdy = m_pitchSurface;
                } else {
                    //
                    // error = distance of intercept from pixel min
                    //

                    e       = float(ystart) - ycenter;
                    slope   = -slope;
                    ddestdy = -FloorInt(m_pitchSurface);
                }
            }

            //
            // Draw the pixels
            //

            BYTE* pdest =
                  m_pbSurface 
                + xstart * 2
                + ystart * m_pitchSurface;

            int count = abs(xend - xstart);

            while (count > 0) {
                *(WORD*)pdest = color;
                pdest += ddestdx;
                e += slope;
                if (e > 0) {
                    pdest += ddestdy;
                    e -= 1;
                }
                count--;
            }
        } else {
            //
            // Mostly Vertical line.
            //
            int   xstart = FloorInt(pva->x);
            int   ystart = FloorInt(pva->y);
            int   yend   = FloorInt(pvb->y);

            int   ddestdx;
            int   ddestdy;

            if (yend > ystart) {
                ddestdy =  m_pitchSurface;
            } else {
                ddestdy = -FloorInt(m_pitchSurface);
                ydelta  = -ydelta;
            }

            float slope  = xdelta / ydelta;
            float e;

            if (slope == 0) {
                e = 0;
            } else {
                //
                // y distance from end point to starting pixel center
                //

                float ydeltaCenter  =  (float(ystart) + 0.5f - pva->y);

                //
                // x intercept of pixel x center line
                //

                float xcenter = pva->x + slope * ydeltaCenter;

                //
                // error
                //

                if (slope > 0) {
                    //
                    // error = distance of intercept from pixel max
                    //

                    e       = xcenter - (float(xstart) + 1.0f);
                    ddestdx = 2;
                } else {
                    //
                    // error = distance of intercept from pixel min
                    //

                    e       = float(xstart) - xcenter;
                    slope   = -slope;
                    ddestdx = -2;
                }
            }

            //
            // Draw the pixels
            //

            BYTE* pdest =
                  m_pbSurface 
                + xstart * 2
                + ystart * m_pitchSurface;

            int count = abs(yend - ystart);

            while (count > 0) {
                *(WORD*)pdest = color;
                pdest += ddestdy;
                e += slope;
                if (e > 0) {
                    pdest += ddestdx;
                    e -= 1;
                }
                count--;
            }
        }
    }
    */

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawPoint(
        const VertexScreen* pva
    ) {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ShouldZTest()
    {
        return (m_pbZBuffer != NULL) && m_bZTest;
    }

    bool ShouldZWrite()
    {
        return (m_pbZBuffer != NULL) && m_bZWrite;
    }

    void UpdatePointers()
    {
        if (m_bUpdatePointers) {
            m_bUpdatePointers = false;
            
            //
            // Primitives
            //
			
			// mdvalley: lots of pointers and spelled out classes to make '05 happy. Do a find on '&SoftwareRasterizer::' to spot them.

			m_pfnDrawTriangle = &SoftwareRasterizer::DrawTriangle;
            m_pfnDrawLine     = &SoftwareRasterizer::DrawLine;
            m_pfnDrawPoint    = &SoftwareRasterizer::DrawPoint;

            switch (m_shadeMode) {
                case ShadeModeFlat:    
                    m_pfnDrawTriangle = &SoftwareRasterizer::DrawFlatTriangle; 
                    break;

                case ShadeModeNone:
                case ShadeModeCopy:    
                case ShadeModeGouraud: 
                    m_pfnDrawTriangle = &SoftwareRasterizer::DrawTriangle; 
                    break;
            }

            //
            // Is ColorKey on?
            //

            m_bColorKeyOn = 
                   m_bColorKeyEnabled 
                && m_psurfaceTexture != NULL 
                && m_psurfaceTexture->HasColorKey();

            if (m_bColorKeyOn) {
                ZAssert(m_psurfaceTexture->GetColorKey() == Color::Black());
            }

            //
            // Subtriangles
            //

            m_passCount = 0;

            if (m_shadeMode == ShadeModeNone) {
                m_pfnFillSubTriangle = &SoftwareRasterizer::FillSubTriangleNoop;
                return;
            } else if (m_psurfaceTexture == NULL) {
                if (
                       m_blendMode == BlendModeSourceAlpha
                    && m_shadeMode == ShadeModeFlat
                    && !ShouldZTest()
                    && !ShouldZWrite()
                    && m_b565
                ) {
                    m_pfnFillSubTriangle = &SoftwareRasterizer::FillSubTriangleMultiPass;
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::FlatBlend1InvA;
                    return;
                }
            } else if (
                   (m_pbZBuffer != NULL)
                && ShouldZTest()
                && ShouldZWrite()
                && m_b565
            ) {
                //
                // Fast paths
                //

                m_pfnFillSubTriangle = &SoftwareRasterizer::FillSubTriangleMultiPass;

                if (m_blendMode == BlendModeSource) {
                    if (m_shadeMode == ShadeModeCopy) {
                        m_pfnFillSubTriangle = &SoftwareRasterizer::FillSubTriangleTextureZTestZWriteAndColorKey;
                    } else {
                        //
                        // Multi pass
                        //

                        switch (m_shadeMode) {
                            case ShadeModeFlat:
                                if (m_bColorKeyOn) {
                                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyTextureZTestZWriteFlatAndColorKey;
                                } else {
                                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyTextureZTestZWriteFlat;
                                }
                                break;

                            case ShadeModeGouraud:
                                if (m_bColorKeyOn) {
                                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyTextureZTestZWriteGouraudAndColorKey;  
                                } else {
                                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyTextureZTestZWriteGouraud;
                                }
                                break;

                            default:
                                ZBadCase();
                        }
                    }

                    return;
                } else if (m_blendMode == BlendModeAdd) {
                    switch (m_shadeMode) {
                        case ShadeModeCopy:
                            m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyTextureZTestZWriteBlend11;
                            return;

                        case ShadeModeFlat:
                            if (m_bColorKeyOn) {
                                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::FlatTextureZTestZWriteBlend11;
                                return;
                            }
                    }
                }
            }
            
            //
            // Chunked multi pass
            //

            m_pfnFillSubTriangle = &SoftwareRasterizer::FillSubTriangleMultiPassChunk;

            //
            // ZTest
            //

            if (ShouldZTest()) {
                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::ZTest;
            }

            //
            // Source color
            //

            if (m_psurfaceTexture) {
                //
                // Texture
                //

                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::LoadColor565Texture;

                //
                // Color key
                //

                if (m_bColorKeyOn) {
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::ColorKeyTest;
                }

                //
                // Shading
                //

                switch (m_shadeMode) {
                    case ShadeModeCopy:
                        break;

                    case ShadeModeFlat:
                        m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::FlatShade;
                        break;

                    case ShadeModeGouraud:
                    case ShadeModeGlobalColor:
                        m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::GouraudShade;
                        break;
                }
            } else {
                //
                // No texture shading
                //

                switch (m_shadeMode) {
                    case ShadeModeGlobalColor:
                        // !!! ZUnimplemented();
                        // fall through to flat shading

                    case ShadeModeCopy:
                    case ShadeModeFlat:
                        m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::LoadFlatColor;
                        break;

                    case ShadeModeGouraud:
                        m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::LoadGouraudColor;
                        break;
                }
            }

            //
            // Alpha Blending
            //

            if (
                   m_blendMode == BlendModeAdd
                || m_blendMode == BlendModeSourceAlpha
            ) {
                if (m_b565) {
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::Load565Dest;
                } else {
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::Load555Dest;
                }

                if (m_blendMode == BlendModeAdd) {
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::Blend11;
                } else {
                    m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::Blend1InvA;
                }
            };

            //
            // Pixel store
            //

            if (m_b565) {
                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyToColor565Destination;
            } else {
                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::CopyToColor555Destination;
            }

            //
            // ZWrite
            //

            if (ShouldZWrite()) {
                m_ppfnFillChunk[m_passCount++] = &SoftwareRasterizer::ZWrite;
            }

            ZAssert(m_passCount <= MaxChunkPasses);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Debug
    //
    //////////////////////////////////////////////////////////////////////////////

    void CheckVertices(const VertexScreen* pvertex, int vcount)
    {
        /*
        #ifdef _DEBUG
        {
            float error = 0.5f;

            for (int index = 0; index < vcount; index++) {
                ZAssert(pvertex[index].x >= m_rectClip.XMin() - error);
                ZAssert(pvertex[index].x <= m_rectClip.XMax() + error);
                ZAssert(pvertex[index].y >= m_rectClip.YMin() - error);
                ZAssert(pvertex[index].y <= m_rectClip.YMax() + error);
                ZAssert(pvertex[index].z >= 0.0f - error);
                ZAssert(pvertex[index].z <= 1.0f + error);
            }
        }
        #endif
        */

        VertexScreen* pcheck = (VertexScreen*)pvertex;

        for (int index = 0; index < vcount; index++) {
            if (pcheck[index].x < m_rectClip.XMin()) {
                pcheck[index].x = m_rectClip.XMin();
            } else if (pcheck[index].x > m_rectClip.XMax()) {
                pcheck[index].x = m_rectClip.XMax();
            }

            if (pcheck[index].y < m_rectClip.YMin()) {
                pcheck[index].y = m_rectClip.YMin();
            } else if (pcheck[index].y > m_rectClip.YMax()) {
                pcheck[index].y = m_rectClip.YMax();
            }

            if (pcheck[index].z < 0) {
                pcheck[index].z = 0;
            } else if (pcheck[index].z > 1) {
                pcheck[index].z = 1;
            }
        }
    }

    void CheckIndices(const WORD* pindex, int icount, int vcount)
    {
        #ifdef _DEBUG
            for (int index = 0; index < icount; index++) {
                ZAssert(pindex[index] >=0 && pindex[index] < vcount);
            }
        #endif
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Rendering
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawTriangles(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount)
    {
        InitializeControlWord();

        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);
        UpdatePointers();

        for(int index = 0; index < icount; index += 3) {
            (this->*m_pfnDrawTriangle)(
                &(pvertex[pindex[index    ]]),
                &(pvertex[pindex[index + 1]]),
                &(pvertex[pindex[index + 2]])
            );
        }

        RestoreControlWord();
    }

    void DrawLines(const VertexScreen* pvertex, int vcount, const WORD* pindex, int icount)
    {
        InitializeControlWord();

        CheckVertices(pvertex, vcount);
        CheckIndices(pindex, icount, vcount);
        UpdatePointers();

        for(int index = 0; index < icount; index += 2) {
            (this->*m_pfnDrawLine)(
                &(pvertex[pindex[index    ]]),
                &(pvertex[pindex[index + 1]])
            );
        }
    }

    void DrawPoints(const VertexScreen* pvertex, int vcount)
    {
        CheckVertices(pvertex, vcount);

        if (m_b565) {
            // 565

            for (int index = 0; index < vcount; index++) {
                BYTE* ppixel = 
                      m_pbSurface 
                    + FloorInt(pvertex[index].x) * 2
                    + FloorInt(pvertex[index].y) * m_pitchSurface;
                DWORD color = pvertex[index].color;
                WORD  pixel =
                    (WORD)( 
                          ((color >> 8) & 0xf800)
                        | ((color >> 5) & 0x07e0)
                        | ((color >> 3) & 0x001f)
                    );

                *(WORD*)(ppixel) = pixel;
            }
        } else {
            // 555

            for (int index = 0; index < vcount; index++) {
                BYTE* ppixel = 
                      m_pbSurface 
                    + FloorInt(pvertex[index].x) * 2
                    + FloorInt(pvertex[index].y) * m_pitchSurface;
                DWORD color = pvertex[index].color;
                WORD  
                pixel =
                    (WORD)( 
                          ((color >> 9) & 0x7c00)
                        | ((color >> 6) & 0x03e0)
                        | ((color >> 3) & 0x001f)
                    );

                *(WORD*)(ppixel) = pixel;
            }
        }
    }
};

TRef<Rasterizer> CreateSoftwareRasterizer(PrivateSurface* psurface)
{
    return new SoftwareRasterizer(psurface);
}
