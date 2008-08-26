//////////////////////////////////////////////////////////////////////////////
//
// Scan UI
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// !!!
//
//////////////////////////////////////////////////////////////////////////////

    // translate the verticies to eye space
    // light the verticies
        // ?? how does opengl handle lights with attenuation
    // form triangles
    // clip triangles
    // perspective project triangles
    // do setup
    // fill

    // modulated
        // can just modulate palette
    // perspective correct
        // do divide every 16 pixels
    // texture
    // 16bpp


//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// Fixed Point Macros
//
//////////////////////////////////////////////////////////////////////////////

inline int MakeFixed(float x) { return int(x * 65536); }

//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

class Color {
public:
    DWORD m_r, m_g, m_b;

    Color(DWORD r, DWORD g, DWORD b) :
        m_r(r),
        m_g(g),
        m_b(b)
    {
    }
};

class SVertex {
public:
    float m_x, m_y, m_z, m_qw, m_uqw, m_vqw;

    SVertex(float x, float y, float z, float qw, float u, float v) :
        m_x(x),
        m_y(y),
        m_z(z),
        m_qw(qw),
        m_uqw(u * qw),
        m_vqw(v * qw)
    {}
};

//////////////////////////////////////////////////////////////////////////////
//
// Scan Window
//
//////////////////////////////////////////////////////////////////////////////

const WORD Black565  = 0x0000;
const WORD White565  = 0xffff;
const WORD Blue565   = 0x001f;
const WORD Red565    = 0xf800;
const WORD Green565  = 0x07e0;
const WORD Violet565 = Red565 | Blue565;
const WORD Yellow565 = Red565 | Green565;
const WORD Cyan565   = Green565 | Blue565;

WORD g_pcolor[] =
    {
        Black565,  // 0
        White565,  // 1
        Red565,    // 2
        Green565,  // 3
        Blue565,   // 4
        Violet565, // 5
        Yellow565, // 6
        Cyan565    // 7
    };

BYTE g_ptexture[] =
        {
            4, 4, 4, 4, 4, 4, 4, 4,
            4, 0, 1, 0, 1, 0, 1, 4,
            4, 1, 3, 2, 3, 2, 0, 4,
            4, 0, 2, 5, 6, 3, 1, 4,
            4, 1, 3, 6, 5, 2, 0, 4,
            4, 0, 2, 3, 2, 3, 1, 4,
            4, 1, 0, 1, 0, 1, 0, 4,
            4, 4, 4, 4, 4, 4, 4, 4
        };

class ScanWindow : public AnimationWindow {
protected:
    float m_pixels;
    float m_lastTime;

    BYTE* m_pdata;
    DWORD m_pitch;

    WORD* m_pcolors;
    BYTE* m_ptexture;
    DWORD m_texturePitch;

    // plane deltas

    float m_dqwdx;
    float m_dqwdy;

    float m_duqwdx;
    float m_duqwdy;

    float m_dvqwdx;
    float m_dvqwdy;

    // edge deltas

    int   m_xmin         ;
    float m_xminFrac     ;
    int   m_dxmindyLittle;
    int   m_dxmindyBig   ;
    float m_dxmindyFrac  ;

    int   m_xmax         ;
    float m_xmaxFrac     ;
    int   m_dxmaxdyLittle;
    int   m_dxmaxdyBig   ;
    float m_dxmaxdyFrac  ;

    float m_qw;
    float m_dqwdyLittle;
    float m_dqwdyBig ;

    float m_uqw;
    float m_duqwdyLittle;
    float m_duqwdyBig;

    float m_vqw;
    float m_dvqwdyLittle;
    float m_dvqwdyBig;

public:
    ScanWindow() :
        AnimationWindow(Rect(8, 28, 640, 480), "Scan"),
        m_lastTime(0)
    {
    }

    ~ScanWindow()
    {
    }

    //
    // Rendering Methods

    void FillTriangle(const SVertex& v0, const SVertex& v1, const SVertex& v2);
    void DeltaSetup(const SVertex& va, const SVertex& vb, const SVertex& vc);
    void CalcXMin(const SVertex& va, const SVertex& vb);
    void CalcXMax(const SVertex& va, const SVertex& vb);
    void FillSubTriangle(const SVertex& va, const SVertex& vb);

    void Render(float time) {
        m_psurface->FillRect(Rect(9, 9, 19, 19), ZColor(0, 0.5, 0));

        const DDSDescription& ddsd = m_psurface->GetDescription();

        m_pitch = ddsd.Pitch();
        m_pdata = (BYTE*)(m_psurface->GetPointer());

        m_pcolors = g_pcolor;
        m_ptexture = g_ptexture;
        m_texturePitch = 8;

        for (int index = 0; index < 1; index++) {
            /*
            {
                float x = 200 ;//+ 3 * sin(time);
                float qw0 = 0.5;
                float qw1 = 1;

                SVertex v0(x - qw0 * 100,  50, 0, qw0, 0, 0);
                SVertex v1(x - qw1 * 100, 250, 0, qw1, 0, 8);
                SVertex v2(x + qw1 * 100, 250, 0, qw1, 8, 8);
                SVertex v3(x + qw0 * 100,  50, 0, qw0, 8, 0);

                g_pcolor[4] = Blue565;
                FillTriangle(v0, v1, v2);
                g_pcolor[4] = Red565;
                FillTriangle(v0, v2, v3);
            }
            */
            {
                float xc = 200;// + 3 * sin(time);
                float yc = 150;
                float xsize = 100;
                float ysize = 100;
                float qw0 = 0.5;
                float qw1 = 1;

                SVertex v0(xc - xsize, yc - qw1 * ysize, 0, qw1, 0, 0);
                SVertex v1(xc - xsize, yc + qw1 * ysize, 0, qw1, 0, 8);
                SVertex v2(xc + xsize, yc + qw0 * ysize, 0, qw0, 8, 8);
                SVertex v3(xc + xsize, yc - qw0 * ysize, 0, qw0, 8, 0);

                g_pcolor[4] = Blue565;
                FillTriangle(v0, v1, v3);
                g_pcolor[4] = Red565;
                FillTriangle(v3, v1, v2);
            } 
            /*
            {
                SVertex v0(x + 250 - 100,  50, 0, 1, 0, 0);
                SVertex v1(x + 250 - 100, 250, 0, 1, 0, 8);
                SVertex v2(x + 250 + 100, 250, 0, 1, 8, 8);
                SVertex v3(x + 250 + 100,  50, 0, 1, 8, 0);

                g_pcolor[4] = Blue565;
                FillTriangle(v0, v1, v2);
                g_pcolor[4] = Red565;
                FillTriangle(v0, v2, v3);
            }
            {
                SVertex v0(10, 10, 0, 1, 0, 0);
                SVertex v1(10, 18, 0, 1, 0, 8);
                SVertex v2(18, 18, 0, 1, 8, 8);
                SVertex v3(18, 10, 0, 1, 8, 0);

                g_pcolor[4] = Blue565;
                FillTriangle(v0, v1, v2);
                g_pcolor[4] = Red565;
                FillTriangle(v0, v2, v3);
            }
            */
        }

        m_psurface->ReleasePointer(m_pdata);
    }

    //
    // Animation Window methods
    //

    ZString GetFPSString(float dtime, float mspf, Context* pcontext)
    {
        ZString str = 
            AnimationWindow::GetFPSString(dtime,mspf,pcontext) + " mps: " + 
            ZString(m_pixels / (1000000 * dtime), 5, 2);
        m_pixels = 0;
        return str;
    }

    void DrawFrame(float time)
    {
        const float dtime = time - m_lastTime;
        m_lastTime = time;

        if (DoClear()) {
            m_psurface->FillSurface(ZColor(0.25, 0.25, 0.25));
        }

        if (DoDraw()) {
            Render(time);
        }
    }

    //
    // Window methods
    //

    bool OnMouseMessage(UINT message, UINT nFlags, const WinPoint& point)
    {
        switch (message) {
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
                return true;
        }

        return false;
    }

    DWORD WndProc(
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        switch (message) {
            case WM_CLOSE:
                PostQuitMessage(0);
                return 0;
        }

        return Window::WndProc(message, wParam, lParam);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Scan Application
//
//////////////////////////////////////////////////////////////////////////////

class ScanApp : public Win32App {
protected:
   TRef<ScanWindow> m_papp;

public:
    void Initialize()
    {
        m_papp = new ScanWindow();
    }
} g_app;


//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

template<int bits>
class Fixed {
protected:
    DWORD m_value;

public:
    Fixed(float f) :
        m_value(f)
    {}

    Fixed(const Fixed& value) :
        m_value(value)
    {}

    void operator=(const Fixed& value)
    {
        m_value = value;
    }

    operator DWORD()
    {
        return m_value >> bits;
    }

};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class Component {
protected:
    Type m_value;
    Type m_dvalue;

public:
    Component(Type value, Type dvalue) :
        m_value(value),
        m_dvalue(dvalue)
    {
    }

    operator Type()
    {
        return m_value;
    }

    void operator++()
    {
        m_value += m_dvalue;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::FillTriangle(
    const SVertex& va,
    const SVertex& vb,
    const SVertex& vc
) {
    DeltaSetup(va, vb, vc);

    if (va.m_y == vc.m_y) {
        CalcXMin(va, vb);
        CalcXMax(vc, vb);
        FillSubTriangle(va, vb);
    } else if (vb.m_y == vc.m_y) {
        CalcXMin(va, vb);
        CalcXMax(va, vc);
        FillSubTriangle(va, vb);
    } else if (vb.m_y < vc.m_y) {
        CalcXMin(va, vb);
        CalcXMax(va, vc);
        FillSubTriangle(va, vb);
        CalcXMin(vb, vc);
        FillSubTriangle(vb, vc);
    } else {
        CalcXMin(va, vb);
        CalcXMax(va, vc);
        FillSubTriangle(va, vc);
        CalcXMax(vc, vb);
        FillSubTriangle(vc, vb);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::DeltaSetup(
    const SVertex& va,
    const SVertex& vb,
    const SVertex& vc
) {
    float yab = vb.m_y - va.m_y;
    float yac = vc.m_y - va.m_y;

    float xab = vb.m_x - va.m_x;
    float xac = vc.m_x - va.m_x;

    float area = yab * xac - yac * xab;
    float qarea = 1 / area;

    m_pixels += area / 2;
    
    float yabn = yab * qarea;
    float yacn = yac * qarea;
    float xabn = xab * qarea;
    float xacn = xac * qarea;

    float qwab = vb.m_qw - va.m_qw;
    float qwac = vc.m_qw - va.m_qw;

    float uqwab = vb.m_uqw - va.m_uqw;
    float uqwac = vc.m_uqw - va.m_uqw;

    float vqwab = vb.m_vqw - va.m_vqw;
    float vqwac = vc.m_vqw - va.m_vqw;

    m_dqwdy  =  qwab * xacn -  qwac * xabn;
    m_duqwdy = uqwab * xacn - uqwac * xabn;
    m_dvqwdy = vqwab * xacn - vqwac * xabn;
                                          
    m_dqwdx  =  qwac * yabn -  qwab * yacn;
    m_duqwdx = uqwac * yabn - uqwab * yacn;
    m_dvqwdx = vqwac * yabn - vqwab * yacn;
}                                         

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::CalcXMin(
    const SVertex& va,
    const SVertex& vb
) {
    //
    // calculate ydelta
    //

    const int ymin  = int(va.m_y + 0.5f);
    const float qdy = 1 / (vb.m_y - va.m_y);

    const float yDelta = (float(ymin) + 0.5f) - va.m_y;

    //
    // calculate dxmindy
    //

    const float dxmindy       = (vb.m_x - va.m_x) * qdy;
    const float xminIntercept = va.m_x + dxmindy * yDelta + 0.5f;

    m_xmin   = int(xminIntercept);
    const float xDelta = (float(m_xmin) + 0.5f) - va.m_x;

    if (dxmindy >= 0) {
        m_dxmindyLittle = int(dxmindy);
        m_dxmindyBig    = m_dxmindyLittle + 1;
        m_dxmindyFrac   = dxmindy - float(m_dxmindyLittle);
        m_xminFrac      = xminIntercept - float(m_xmin) - 1;
    } else {
        m_dxmindyLittle = int(dxmindy);
        m_dxmindyBig    = m_dxmindyLittle - 1;
        m_dxmindyFrac   = float(m_dxmindyLittle) - dxmindy;
        m_xminFrac      = float(m_xmin) - xminIntercept;
    }

    //
    // calculate qu, qv, and qw deltas
    //

    m_dqwdyLittle  = m_dqwdy  + m_dqwdx  * m_dxmindyLittle;
    m_duqwdyLittle = m_duqwdy + m_duqwdx * m_dxmindyLittle;
    m_dvqwdyLittle = m_dvqwdy + m_dvqwdx * m_dxmindyLittle;

    if (m_dxmindyBig >= 0) {
        m_dqwdyBig  = m_dqwdyLittle  + m_dqwdx ;
        m_duqwdyBig = m_duqwdyLittle + m_duqwdx;
        m_dvqwdyBig = m_dvqwdyLittle + m_dvqwdx;
    } else {
        m_dqwdyBig  = m_dqwdyLittle  - m_dqwdx ;
        m_duqwdyBig = m_duqwdyLittle - m_duqwdx;
        m_dvqwdyBig = m_dvqwdyLittle - m_dvqwdx;
    }

    //
    // Initial values
    //

    m_qw  = va.m_qw  + m_dqwdy  * yDelta + m_dqwdx  * xDelta;
    m_uqw = va.m_uqw + m_duqwdy * yDelta + m_duqwdx * xDelta;
    m_vqw = va.m_vqw + m_dvqwdy * yDelta + m_dvqwdx * xDelta;

    ZAssert(m_qw  >= 0);
    ZAssert(m_uqw >= 0);
    ZAssert(m_vqw >= 0);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::CalcXMax(
    const SVertex& va,
    const SVertex& vb
) {
    //
    // calculate ydelta
    //

    const int ymin     = int(va.m_y + 0.5f);
    const float qdy    = 1 / (vb.m_y - va.m_y);
    const float yDelta = (float(ymin) + 0.5f) - va.m_y;

    //
    // calculate dxmaxdy
    //

    const float dxmaxdy       = (vb.m_x - va.m_x) * qdy;
    const float xmaxIntercept = va.m_x + dxmaxdy * yDelta + 0.5f;

    m_xmax = int(xmaxIntercept);

    if (dxmaxdy >= 0) {
        m_dxmaxdyLittle = int(dxmaxdy);
        m_dxmaxdyBig    = m_dxmaxdyLittle + 1;
        m_dxmaxdyFrac   = dxmaxdy - float(m_dxmaxdyLittle);
        m_xmaxFrac      = xmaxIntercept - float(m_xmax) - 1;
    } else {
        m_dxmaxdyLittle = int(dxmaxdy);
        m_dxmaxdyBig    = m_dxmaxdyLittle - 1;
        m_dxmaxdyFrac   = float(m_dxmaxdyLittle) - dxmaxdy;
        m_xmaxFrac      = float(m_xmax) - xmaxIntercept;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::FillSubTriangle(
    const SVertex& va,
    const SVertex& vb
) {
    //
    // calculate y range
    //

    const int ymin = int(va.m_y + 0.5f);
    const int ymax = int(vb.m_y + 0.5f);

    //
    // copy stuff onto the stack
    //

    const WORD* pcolors      = m_pcolors;
    const BYTE* ptexture     = m_ptexture;
    const DWORD texturePitch = m_texturePitch;
    const DWORD pitch        = m_pitch;

    const int   dxmindyLittle = m_dxmindyLittle;
    const int   dxmindyBig    = m_dxmindyBig   ;
    const float dxmindyFrac   = m_dxmindyFrac  ;
    const int   dxmaxdyLittle = m_dxmaxdyLittle;
    const int   dxmaxdyBig    = m_dxmaxdyBig   ;
    const float dxmaxdyFrac   = m_dxmaxdyFrac  ;
    const float dqwdx         = m_dqwdx        ;
    const float duqwdx        = m_duqwdx       ;
    const float dvqwdx        = m_dvqwdx       ;
    const float dqwdyLittle   = m_dqwdyLittle  ;
    const float duqwdyLittle  = m_duqwdyLittle ;
    const float dvqwdyLittle  = m_dvqwdyLittle ;
    const float dqwdyBig      = m_dqwdyBig     ;
    const float duqwdyBig     = m_duqwdyBig    ;
    const float dvqwdyBig     = m_dvqwdyBig    ;

    int   xmin     = m_xmin    ;
    float xminFrac = m_xminFrac;
    int   xmax     = m_xmax    ;
    float xmaxFrac = m_xmaxFrac;
    float qw       = m_qw      ;
    float uqw      = m_uqw     ;
    float vqw      = m_vqw     ;

    //
    // pixel format
    //

    const DWORD redMask   = 0x1f << 11;
    const DWORD greenMask = 0x3f << 5;
    const DWORD blueMask  = 0x1f;

    //
    // cubic approximation loop invariants
    //

    const float x1 = 0.25f;
    const float x2 = 0.90f;

    const float bd   = 1 / (x1 * x1 - x1);
    const float btop = (x1 * x1 * x1 - x1);
    const float q    = btop * bd;
    const float ad   = x2 * x2 * x2 - q * x2 * x2 + (q - 1) * x2;

    //
    // fill the triangle
    //

    BYTE* pscan = m_pdata + ymin * pitch;

    for (int y = ymin; y < ymax; y++) {
        int count = xmax - xmin;

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            // xdeltas

            const float xscale = float(count);
            const float g  = 1 / xscale;
            const float g2 = g * g;
            const float g3 = g2 * g;

            // w values

            const float w0 =  1 / qw;
            const float w1 =  1 / (qw + dqwdx * xscale * x1);
            const float w2 =  1 / (qw + dqwdx * xscale * x2);
            const float w3 =  1 / (qw + dqwdx * xscale     );

            // u values

            const float u0 = w0 *  uqw;
            const float u3 = w3 * (uqw + duqwdx * xscale);
            const float uscale = u3 - u0;
            const float quscale = 1 / uscale;

            const float u1 = quscale * (w1 * (uqw + duqwdx * xscale * x1) - u0);
            const float u2 = quscale * (w2 * (uqw + duqwdx * xscale * x2) - u0);

            const float ru = (u1 - x1) * bd;
            const float au = (u2 - ru * x2 * x2 + (ru - 1) * x2) * ad;
            const float bu = (u1 - au * btop - x1) * bd;
            const float cu = 1 - au - bu;

            const float dddu = uscale * (au * 12 * g3                       );
                  float ddu  = uscale * (au *  6 * g3 + bu * 2 * g2         );
                  float du   = uscale * (au      * g3 + bu     * g2 + cu * g);
                  float u    = u0;

            // v values

            const float v0 = w0 *  vqw;
            const float v3 = w3 * (vqw + dvqwdx * xscale);
            const float vscale = v3 - v0;
            const float qvscale = 1 / vscale;

            const float v1 = qvscale * (w1 * (vqw + dvqwdx * xscale * x1) - v0);
            const float v2 = qvscale * (w2 * (vqw + dvqwdx * xscale * x2) - v0);

            const float rv = (v1 - x1) * bd;
            const float av = (v2 - rv * x2 * x2 + (rv - 1) * x2) * ad;
            const float bv = (v1 - av * btop - x1) * bd;
            const float cv = 1 - av - bv;

            const float dddv = vscale * (av * 12 * g3                       );
                  float ddv  = vscale * (av *  6 * g3 + bv * 2 * g2         );
                  float dv   = vscale * (av      * g3 + bv     * g2 + cv * g);
                  float v    = v0;

            // x loop

            for(; count > 0; count--) {
                DWORD index = ptexture[int(v) * texturePitch + int(u)];
                *(WORD*)pdest = pcolors[index];
                //*(WORD*)pdest += 0x4;

                // x++

                pdest += 2;

                u   += du;
                du  += ddu;
                ddu += dddu;

                v   += dv;
                dv  += ddv;
                ddv += dddv;
            }
        }

        // y++

        pscan += pitch;

        xminFrac += dxmindyFrac;

        if (xminFrac >= 0) {
            xminFrac -= 1;

            xmin += dxmindyBig;
            qw   += dqwdyBig;
            uqw  += duqwdyBig;
            vqw  += dvqwdyBig;
        } else {
            xmin += dxmindyLittle;
            qw   += dqwdyLittle;
            uqw  += duqwdyLittle;
            vqw  += dvqwdyLittle;
        }

        xmaxFrac += dxmaxdyFrac;

        if (xmaxFrac >= 0) {
            xmaxFrac -= 1;

            xmax += dxmaxdyBig;
        } else {
            xmax += dxmaxdyLittle;
        }
    }

    //
    // Save the left hand values for the next part of the triangle
    //

    m_xmin     = xmin    ;
    m_xminFrac = xminFrac;
    m_xmax     = xmax    ;
    m_xmaxFrac = xmaxFrac;
    m_qw       = qw      ;
    m_uqw      = uqw     ;
    m_vqw      = vqw     ;
}
