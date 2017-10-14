#ifdef LATER

//////////////////////////////////////////////////////////////////////////////
//
//  cubic approximation using four points on the curve
//
//////////////////////////////////////////////////////////////////////////////

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            const float xscale = float(count);
            const float x1 = 0.25f;
            const float x2 = 0.9f;

            const float g  = 1 / xscale;
            const float g2 = g * g;
            const float g3 = g2 * g;

            const float w0 =  1 / qw;
            const float w1 =  1 / (qw + dqwdx * xscale * x1);
            const float w2 =  1 / (qw + dqwdx * xscale * x2);
            const float w3 =  1 / (qw + dqwdx * xscale     );

            const float bd   = 1 / (x1 * x1 - x1);
            const float btop = (x1 * x1 * x1 - x1);
            const float q    = btop * bd;
            const float ad   = x2 * x2 * x2 - q * x2 * x2 + (q - 1) * x2;

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

            const int dddu = MakeFixed(uscale * (au * 12 * g3                       ));
                  int ddu  = MakeFixed(uscale * (au *  6 * g3 + bu * 2 * g2         ));
                  int du   = MakeFixed(uscale * (au      * g3 + bu     * g2 + cu * g));
                  int u    = MakeFixed(u0);

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

            const int dddv = MakeFixed(vscale * (av * 12 * g3                       ));
                  int ddv  = MakeFixed(vscale * (av *  6 * g3 + bv * 2 * g2         ));
                  int dv   = MakeFixed(vscale * (av      * g3 + bv     * g2 + cv * g));
                  int v    = MakeFixed(v0);

            // x loop

            for(; count > 0; count--) {
                //ZAssert(u >= 0);
                //ZAssert(v >= 0);

                DWORD index = ptexture[(v >> 16) * texturePitch + (u >> 16)];
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

//////////////////////////////////////////////////////////////////////////////
//
//  quadradic approximation using three points on the curve
//
//////////////////////////////////////////////////////////////////////////////

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            const float x2 = float(count);
            const float x1 = 0.5f * x2;

            const float w0 = 1 / qw;
            const float w1 = 1 / (qw + dqwdx * x1);
            const float w2 = 1 / (qw + dqwdx * x2);

            const float u0 = w0 * uqw;
            const float u1 = w1 * (uqw + duqwdx * x1) - u0;
            const float u2 = w2 * (uqw + duqwdx * x2) - u0;

            const float v0 = w0 * vqw;
            const float v1 = w1 * (vqw + dvqwdx * x1) - v0;
            const float v2 = w2 * (vqw + dvqwdx * x2) - v0;

            float  d = x1 * x2 * (x2 - x1);

            float au = (u2 * x1 - u1 * x2) / d;
            float bu = (u1 - au * x1 * x1) / x1;

            const int ddu = MakeFixed(2 * au);
                  int du  = MakeFixed(au + bu);
                  int u   = MakeFixed(u0);

            float av = (v2 * x1 - v1 * x2) / d;
            float bv = (v1 - av * x1 * x1) / x1;

            const int ddv = MakeFixed(2 * av);
                  int dv  = MakeFixed(av + bv);
                  int v   = MakeFixed(v0);

            for(; count > 0; count--) {
                ZAssert(u >= 0);
                ZAssert(v >= 0);

                DWORD index = ptexture[(v >> 16) * texturePitch + (u >> 16)];
                *(WORD*)pdest = pcolors[index];
                //*(WORD*)pdest += 0x4;

                // x++

                pdest += 2;

                u  += du;
                du += ddu;

                v  += dv;
                dv += ddv;
            }
        }

//////////////////////////////////////////////////////////////////////////////
//
//  quadradic taylor series approximation
//
//////////////////////////////////////////////////////////////////////////////

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            const float w = 1 / qw;
            const float w2 = w * w;
            const float w3 = w2 * w;

            const float dqw2 = dqwdx * dqwdx;

            const int ddu = MakeFixed(2 * dqw2 * uqw * w3 - duqwdx * dqwdx * w + duqwdx * dqw2 * w);
                  int du  = MakeFixed(duqwdx * w - uqw * dqwdx * w2) + ddu / 2;
                  int u   = MakeFixed(uqw * w);

            const int ddv = MakeFixed(2 * dqw2 * vqw * w3 - dvqwdx * dqwdx * w + dvqwdx * dqw2 * w);
                  int dv  = MakeFixed(dvqwdx * w - vqw * dqwdx * w2) + ddv / 2;
                  int v   = MakeFixed(vqw * w);

            for(; count > 0; count--) {
                ZAssert(u >= 0);
                ZAssert(v >= 0);

                DWORD index = ptexture[(v >> 16) * texturePitch + (u >> 16)];
                *(WORD*)pdest = pcolors[index];
                //*(WORD*)pdest += 0x4;

                // x++

                pdest += 2;

                u  += du;
                du += ddu;

                v  += dv;
                dv += ddv;
            }
        }

//////////////////////////////////////////////////////////////////////////////
//
//  divide per pixel
//
//////////////////////////////////////////////////////////////////////////////

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            float qwSpan  = qw;
            float uqwSpan = uqw;
            float vqwSpan = vqw;

            for(; count > 0; count--) {
                const float w = 1 / qwSpan;
                const int u = MakeFixed(uqwSpan * w);
                const int v = MakeFixed(vqwSpan * w);

                ZAssert(u >= 0);
                ZAssert(v >= 0);

                DWORD index = ptexture[(v>>16) * texturePitch + (u>>16)];
                *(WORD*)pdest = pcolors[index];
                //*(WORD*)pdest += 0x4;

                // x++

                qwSpan  += dqwdx;
                uqwSpan += duqwdx;
                vqwSpan += dvqwdx;
                pdest += 2;
            }
        }

//////////////////////////////////////////////////////////////////////////////
//
//  piecewise linear approximation
//
//////////////////////////////////////////////////////////////////////////////

        if (count > 0) {
            BYTE* pdest = pscan + xmin * 2;

            float qwSpan  = qw  +  dqwdx16;
            float uqwSpan = uqw + duqwdx16;
            float vqwSpan = vqw + dvqwdx16;

            float w = 1 / qwSpan;
            int u = MakeFixed(uqw * w);
            int v = MakeFixed(vqw * w);

            ZAssert(u >= 0);
            ZAssert(v >= 0);

            while (true) {
                float wNext = 1 / qwSpan;

                int uNext = MakeFixed(uqwSpan * wNext);
                int vNext = MakeFixed(vqwSpan * wNext);

                int du = (uNext - u) / 16;
                int dv = (vNext - v) / 16;

                int insideCount = count;
                if (insideCount > 16) {
                    insideCount = 16;
                }

                for(; insideCount > 0; insideCount--) {
                    DWORD index = ptexture[(v>>16) * texturePitch + (u>>16)];
                    *(WORD*)pdest = pcolors[index];
                    //*(WORD*)pdest += 0x4;

                    // x++

                    u += du;
                    v += dv;
                    pdest += 2;
                }

                count -= 16;
                if (count <= 0) break;

                // x += 16

                u = uNext;
                v = vNext;

                qwSpan  += dqwdx16 ;
                uqwSpan += duqwdx16;
                vqwSpan += dvqwdx16;
            }
        }

//////////////////////////////////////////////////////////////////////////////
//
//  rgb texture
//
//////////////////////////////////////////////////////////////////////////////

    DWORD rTexture = colors[index].r;
    DWORD gTexture = colors[index].g;
    DWORD bTexture = colors[index].b;

    DWORD rFinal = (rTexture >> 16) & redMask;
    DWORD gFinal = (gTexture >> 16) & greenMask;
    DWORD bFinal = (bTexture >> 16);

    *(WORD*)pdest = (WORD)(rFinal | gFinal | bFinal);

    WORD word = *(WORD*)pdest;
    *(WORD*)pdest = (word << 5) + 0x1f;

//////////////////////////////////////////////////////////////////////////////
//
//  This routine doesn't support perspective correct textures
//
//////////////////////////////////////////////////////////////////////////////

void ScanWindow::FillSubTriangle(
    float fymin,
    float fymax,
    float fxmin0,
    float fxmax0,
    float fxmin1,
    float fxmax1
) {
    const DWORD ymin  = fymin;
    const DWORD ymax  = fymax;
    const float qdy   = 1 / (fymax - fymin);

    DWORD xmin  = MakeFixed(fxmin0);
    DWORD xmax  = MakeFixed(fxmax0);
    const int dxmin   = MakeFixed((fxmin1 - fxmin0) * qdy);
    const int dxmax   = MakeFixed((fxmax1 - fxmax0) * qdy);

    const DWORD dqwdx  = MakeFixed(m_dqwdy  + m_dqwdx  * (fxmin1 - fxmin0) * qdy);
    const DWORD duqwdx = MakeFixed(m_duqwdy + m_duqwdx * (fxmin1 - fxmin0) * qdy);
    const DWORD dvqwdx = MakeFixed(m_dvqwdy + m_dvqwdx * (fxmin1 - fxmin0) * qdy);

    DWORD uqw = MakeFixed(m_uqw);
    DWORD vqw = MakeFixed(m_vqw);
    DWORD du  = MakeFixed(m_duqwdx);
    DWORD dv  = MakeFixed(m_dvqwdx);

    //
    // save members on stack
    //

    const WORD* pcolors      = m_pcolors;
    const BYTE* ptexture     = m_ptexture;
    const DWORD texturePitch = m_texturePitch;
    const DWORD pitch        = m_pitch;

    //
    // pixel format
    //

    const DWORD redMask   = 0x1f << 11;
    const DWORD greenMask = 0x3f << 5;
    const DWORD blueMask  = 0x1f;

    //
    // fill the triangle
    //

    BYTE* pscan = m_pdata + ymin * m_pitch;

    if (true) {
        DWORD ycount = ymax - ymin;

        // eax - work
        // ebx - work
        //
        // ecx - u
        // edx - v
        // edi - pdest
        // esi - count
        //     - ptexture
        //     - pcolors
        //     - texturePitch

        __asm {
            // for (DWORD y = ymin; y < ymax; y++) {
            //     if (count > 0) {
            //         BYTE* pdest = pscan + HIWORD(xmin) * 2;

            mov     eax, xmin
            mov     esi, xmax
            mov     ecx, uqw
            mov     edx, vqw
            mov     edi, pscan

            jmp     skipdelta

          labely:
            // uqw += duqwdx;
            // vqw += dvqwdx;
            // xmin += dxmin;
            // xmax += dxmax;
            // pscan += pitch;

            mov     esi, xmax
            mov     ebx, dxmax
            add     esi, ebx
            mov     xmax, esi

            mov     eax, xmin
            mov     ebx, dxmin
            add     eax, ebx
            mov     xmin, eax

            mov     edi, pscan
            mov     ebx, pitch
            add     edi, ebx
            mov     pscan, edi

            mov     ecx, uqw
            mov     ebx, duqwdx
            add     ecx, ebx
            mov     uqw, ecx

            mov     edx, vqw
            mov     ebx, dvqwdx
            add     edx, ebx
            mov     vqw, edx

          skipdelta:

            // int count = (HIWORD(xmax) - HIWORD(xmin)) * 2;
            // BYTE* pdest = pscan + HIWORD(xmin) * 2;

            shr     esi, 10h
            shr     eax, 10h
            sub     esi, eax

            je      nextscan

            lea     edi, [edi + eax * 2]

          labelx:

            // DWORD index = ptexture[(v>>16) * texturePitch + (u>>16)];

            mov     eax, ecx
            shr     eax, 10h
            mov     ebx, edx
            shr     ebx, 10h
            add     ebx, ptexture

            movsx   eax, byte ptr [ebx + eax * 8]

            // *(WORD*)pdest = pcolors[index];

            mov     ebx, pcolors

            mov     ax, word ptr [ebx + eax * 2]
            mov     word ptr [edi], ax

            // u += du;
            // v += dv;
            // pdest += 2;

            add     ecx, du
            add     edx, dv
            add     edi, 2

            // count--

            dec     esi
            jne     labelx

            // ycount--

          nextscan:
            dec     ycount
            jne     labely
        }
    } else if (true) {
        for (DWORD y = ymin; y < ymax; y++) {
            int count = HIWORD(xmax) - HIWORD(xmin);
            if (count > 0) {
                BYTE* pdest = pscan + HIWORD(xmin) * 2;

                DWORD  u = uqw;
                DWORD  v = vqw;

                for (; count > 0; count--) {
                    DWORD index = ptexture[(v>>16) * 8 /*texturePitch*/ + (u>>16)];
                    *(WORD*)pdest = pcolors[index];

                    // x++

                    u += du;
                    v += dv;
                    pdest += 2;
                }
            }

            // y++

            uqw += duqwdx;
            vqw += dvqwdx;

            xmin += dxmin;
            xmax += dxmax;

            pscan += pitch;
        }
    }
}

#endif
