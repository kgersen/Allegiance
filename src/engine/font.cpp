#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Fonts
//
//////////////////////////////////////////////////////////////////////////////

class FontImpl : public IEngineFont {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class CharData {
    public:
        WinPoint m_size;
        int      m_offset;
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    //////////////////////////////////////////////////////////////////////////////

    int               m_height;
    int               m_width;
    CharData          m_data[256];
    BYTE*             m_pdata;
    int               m_dataSize;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Private members
    //
    //////////////////////////////////////////////////////////////////////////////

    void FillFontData(HFONT hfont)
    {
        int length = 0;
        int index;

        //
        // figure out the total data size
        //

        for (index = 0; index < 256; index++) {
            int xsize = m_data[index].m_size.X();
            int ysize = m_data[index].m_size.Y();

            m_data[index].m_offset = length;

            length += ((xsize + 7) / 8) * ysize;
        }

        //
        // Allocate the data
        //

        m_dataSize = (length + 3) & (~3);
        m_pdata    = new BYTE[m_dataSize];

        //
        // Create a dib section to get the characters with
        //

        class MyHeader {
        public:
            BITMAPINFOHEADER bmih;
            RGBQUAD          colors[256];
        } bmih;

        bmih.bmih.biSize          = sizeof(BITMAPINFOHEADER);
        bmih.bmih.biWidth         = m_width;
        bmih.bmih.biHeight        = -m_height;
        bmih.bmih.biPlanes        = 1;
        bmih.bmih.biBitCount      = 8;
        bmih.bmih.biCompression   = BI_RGB;
        bmih.bmih.biSizeImage     = 0;
        bmih.bmih.biXPelsPerMeter = 0;
        bmih.bmih.biYPelsPerMeter = 0;
        bmih.bmih.biClrUsed       = 0;
        bmih.bmih.biClrImportant  = 0;

        for (index = 0; index < 256; index++) {
            bmih.colors[index].rgbBlue     = index;
            bmih.colors[index].rgbGreen    = index;
            bmih.colors[index].rgbRed      = index;
            bmih.colors[index].rgbReserved = 0;
        }

        BYTE*   pbits;
        HBITMAP hbitmap =
            ::CreateDIBSection(
                NULL,
                (BITMAPINFO*)&bmih,
                DIB_RGB_COLORS,
                (void**)&pbits,
                NULL,
                0
            );
        ZAssert(hbitmap != NULL);

        HDC hdcBitmap = ::CreateCompatibleDC(NULL);
        ZAssert(hdcBitmap != NULL);
        HBITMAP hbitmapOld = (HBITMAP)::SelectObject(hdcBitmap, hbitmap);
        ZAssert(hbitmapOld != NULL);
        HFONT hfontOld = (HFONT)::SelectObject(hdcBitmap, hfont);
        ZVerify(hfontOld != NULL);
        ZVerify(::SetBkMode(hdcBitmap, TRANSPARENT));
        ZVerify(::SetTextColor(hdcBitmap, Color::White().MakeCOLORREF()) != CLR_INVALID);
        ZVerify(::SetBkColor  (hdcBitmap, Color::Black().MakeCOLORREF()) != CLR_INVALID);

        //
        // Get the character bitmaps
        //

        BYTE* pdata = m_pdata;
        int scanBytes = ((m_width + 3) / 4) * 4;

        for (index = 0; index < 256; index++) {
            int xsize = m_data[index].m_size.X();
            int ysize = m_data[index].m_size.Y();
            unsigned char ch = index;

            ZVerify(::FillRect(
                hdcBitmap,
                &(RECT)WinRect(0, 0, m_width, m_height),
                (HBRUSH)::GetStockObject(BLACK_BRUSH)
            ));
            ZVerify(::TextOut(hdcBitmap, 0, 0, (PCC)&ch, 1));

            //
            // pull out the data
            //

            int xbytes = (xsize + 7) / 8;
            for (int yindex = 0; yindex < ysize; yindex++) {
                for (int xbyte = 0; xbyte < xbytes; xbyte++) {
                    WORD word = 0;
                    for (int xbit = 0; xbit < 8; xbit++) {
                        if (pbits[yindex * scanBytes + xbyte * 8 + xbit] == 0xff) {
                            word |= 0x100;
                        }
                        word >>= 1;
                    }
                    *pdata = (BYTE)word;
                    pdata++;
                }
            }
        }
        ZAssert(pdata == m_pdata + length);

        //
        // Release the DC we created
        //

        ZVerify(::SelectObject(hdcBitmap, hfontOld));
        ZVerify(::SelectObject(hdcBitmap, hbitmapOld));
        ZVerify(::DeleteDC(hdcBitmap));
        ZVerify(::DeleteObject(hbitmap));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void GetSizes(HFONT hfont)
	{
        HDC hdc = ::GetDC(NULL);
        ZVerify(hdc != NULL);

        HFONT hfontOld = (HFONT)::SelectObject(hdc, hfont);
        ZVerify(hfontOld != NULL);

        //
        // Get the character widths, offsets
        //

        m_height = 0;
        m_width  = 0;
        int index;

        for (index = 0; index < 256; index++) {
            SIZE size;
            unsigned char ch = index;

            ZVerify(::GetTextExtentPoint(
                hdc,
                (PCC)&ch,
                1,
                &size
            ));

            m_data[index].m_size   = WinPoint(size.cx, size.cy);

            m_height = max(m_height, size.cy);
            m_width  = max(m_width,  size.cx);
        }

        //
        // release the hdc
        //

        ZVerify(::SelectObject(hdc, hfontOld));
        ZVerify(::ReleaseDC(NULL, hdc));

        //
        // Make that all of the value GDI returned to us are good
        //

        for (index = 0; index < 256; index++) {
            ZAssert(m_data[index].m_size.X() >  0           );
            ZAssert(m_data[index].m_size.X() <  m_height * 4);
            ZAssert(m_data[index].m_size.Y() == m_height    );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

public:
    FontImpl(HFONT hfont) :
        m_pdata(NULL)
    {
        GetSizes(hfont);
        FillFontData(hfont);
        ZVerify(::DeleteObject(hfont));
    }

    FontImpl(IBinaryReaderSite* psite)
    {
        m_height = psite->GetDWORD();
        m_width  = psite->GetDWORD();
        m_dataSize = psite->GetDWORD();

        memcpy(m_data, psite->GetPointer(), sizeof(m_data));
        psite->MovePointer(sizeof(m_data));

        m_pdata = new BYTE[m_dataSize];
        memcpy(m_pdata, psite->GetPointer(), m_dataSize);
        psite->MovePointer(m_dataSize);
    }

    ~FontImpl()
    {
        if (m_pdata) {
            delete m_pdata;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    void Write(IMDLBinaryFile* pmdlFile)
    {
        pmdlFile->WriteReference("ImportFont");
        TRef<ZFile> pfile = pmdlFile->WriteBinary();

        pfile->Write(m_height);
        pfile->Write(m_width);
        pfile->Write(m_dataSize);
        pfile->Write(m_data, sizeof(m_data));
        pfile->Write(m_pdata, m_dataSize);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IEngineFont members
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetHeight()
    {
        return m_height;
    }

    WinPoint GetTextExtent(const ZString& str)
    {
        int count = str.GetLength();
        int width = 0;

        for (int index = 0; index < count; index++) {
            if (str[index] == START_COLOR_CODE)
                index += 10 - 1;
            else if (str[index] == END_COLOR_CODE)
                index += 2 - 1;
            else
                width += m_data[(BYTE)str[index]].m_size.X();
        }

        return WinPoint(width, m_height);
    }

    WinPoint GetTextExtent(const char* sz)
    {
        int count = strlen(sz);
        int width = 0;

        for (int index = 0; index < count; index++) {
            if (sz[index] == START_COLOR_CODE)
                index += 10 - 1;
            else if (sz[index] == END_COLOR_CODE)
                index += 2 - 1;
            else
                width += m_data[(BYTE)sz[index]].m_size.X();
        }

        return WinPoint(width, m_height);
    }

    int GetMaxTextLength(const ZString& str, int maxWidth, bool bLeft)
    {
        int count = str.GetLength();
        int width = 0;

        for (int index = 0; index < count; index++) {
            int ichar = bLeft ? index : count - (index + 1);
            // note that this method is not adjusted to account for colored text, since it
            // may work backwards across the string, resulting in a difficult case that I
            // don't think is causing problems right now, ergo no need to fix it.
            width += m_data[(BYTE)str[ichar]].m_size.X();
            if (width > maxWidth)
                return index;
        }

        return count;
    }

    void DrawClippedChar(
        const WinRect& rectClip,
        byte ch,
        int ytop,
        int ysize,
        int x, BYTE*
        pdestTop,
        DWORD pixel, // KGJV 32B
        int pitchDest,
		int pixelBytes // KGJV 32B - bbp
    ) {
        const CharData& charData  = m_data[ch];
        int             xsizeChar = charData.m_size.X();

        //
        // figure out the left and right clipping
        //

        int xleft;

        if (x < rectClip.XMin()) {
            xleft = rectClip.XMin() - x;
        } else {
            xleft = 0;
        }

        int xright;

        if (x + xsizeChar > rectClip.XMax()) {
            xright = rectClip.XMax() - x;
        } else {
            xright = xsizeChar;
        }

        //
        // pointers
        //

        int   bytesSource         = (xsizeChar + 7) / 8;
        BYTE* psourceStart        =
              m_pdata
            + charData.m_offset
            + bytesSource * ytop;
        BYTE* pdestStart          = pdestTop + (x + xleft) * pixelBytes; // KGJV 32B 2 -> pixelBytes

        //
        // copy bits between xleft and xright
        //

        for (int scans = ysize; scans > 0; scans--) {
            BYTE* psource = psourceStart;
            BYTE* pdest   = pdestStart;
            int  ibit     = 0;
            BYTE byte     = *psource;
            psource++;

            //
            // skip clipped left bits
            //

            while (ibit < xleft) {
                byte = byte >> 1;
                ibit++;
                if (ibit & 0x7 == 0) {
                    byte = *psource;
                    psource++;
                }
            }

            //
            // render bits between xleft and xright
            //

            while (ibit < xright) {
                if (byte & 1) {
					if (pixelBytes==4) // KGJV 32B
						*(DWORD*)pdest = pixel;
					else // assumes 16bpp
						*(WORD*)pdest = (WORD)pixel;
                }

                pdest += pixelBytes; // KGJV 32B
                byte   = byte >> 1;
                ibit  += 1;
                if (ibit & 0x7 == 0) {
                    byte = *psource;
                    psource++;
                }
            }

            psourceStart += bytesSource;
            pdestStart   += pitchDest;
        }
    }

    void DrawString(
              Surface*  psurface,
        const WinPoint& point,
        const WinRect&  rectClip,
        const ZString&  str,
        const Color&    color
    ) {
        if (str.GetLength() == 0) {
            return;
        }

        //
        // are we clipped in the y direction?
        //

        int ytop    = 0;
        int ybottom = m_height;

        //
        // is the top clipped?
        //

        if (point.Y() < rectClip.YMin()) {
            ytop = rectClip.YMin() - point.Y();
        }

        //
        // is the bottom clipped?
        //

        if (point.Y() + m_height > rectClip.YMax()) {
            ybottom = rectClip.YMax() - point.Y();
        }

        //
        // figure out the total number of scan lines to draw
        //

        int ysize = ybottom - ytop;

        if (ysize <= 0) {
            //
            // clipped in y, nothing to draw
            //

            return;
        }

        //
        // Get a pointer to the destination memory
        //

        BYTE* pdestTop =
              psurface->GetWritablePointer()
            + (point.Y() + ytop) * psurface->GetPitch();

        //
        // All access to the surface needs to be inside an exception handler
        //

        //KGJV 32B
        //debugf("drawstring: @%d,%d = %s\n",point.X(), point.Y(), (const char *)str);
        __try {
            int ichar = 0;
            int x = point.X ();
            DrawStringHandled(
                psurface,
                x,
                rectClip,
                str,
                color,
                pdestTop,
                ytop,
                ybottom,
                ysize,
                ichar
            );
        } __except (true) {
            //
            // DDraw took away our access to some video memory.  Ignore the exception
            // and continue.
            //
        }

        psurface->ReleasePointer();
    }

    int  HexDigitToInt (int hexDigit)
    {
        if (isdigit (hexDigit))
            return hexDigit - '0';
        else if (isxdigit (hexDigit))
            return (tolower (hexDigit) - 'a') + 10;
        else
            return -1;
    }

    int ReadHexPair (const ZString& str, int& ichar)
    {
        int highNibble = HexDigitToInt (str[ichar++]);
        int lowNibble = HexDigitToInt (str[ichar++]);
        assert ((highNibble >= 0) && (lowNibble >= 0));
        return ((highNibble << 4) | lowNibble);
    }

    Color   ReadColor (const ZString& str, int& ichar)
    {
        float   red = ReadHexPair (str, ichar) / 255.0f;
        float   green = ReadHexPair (str, ichar) / 255.0f;
        float   blue = ReadHexPair (str, ichar) / 255.0f;
        float   alpha = ReadHexPair (str, ichar) / 255.0f;
        return Color (red, green, blue, alpha);
    }

    void DrawColorCodedString(
              Surface*  psurface,
              int&      x,
        const WinRect&  rectClip,
        const ZString&  str,
              BYTE*     pdestTop,
              int       ytop,
              int       ybottom,
              int       ysize,
              int&      ichar
    )
    {
        // we started here because we encountered a color code,
        // so the first thing is to skip over the color code and
        // the required following character
        ichar += 2;

        // next we need to read the color, in the form rrggbbaa,
        // where each pair of digits is
        Color   nextColor = ReadColor (str, ichar);
        DrawStringHandled (psurface, x, rectClip, str, nextColor, pdestTop, ytop, ybottom, ysize, ichar);
    }


    void DrawStringHandled(
              Surface*  psurface,
              int&      x,
        const WinRect&  rectClip,
        const ZString&  str,
        const Color&    color,
              BYTE*     pdestTop,
              int       ytop,
              int       ybottom,
              int       ysize,
              int&      ichar
    ) {
        int length = str.GetLength();

        //
        // get all the surface info
        //

        PixelFormat* ppf        = psurface->GetPixelFormat();
        int          pixelBytes = ppf->PixelBytes();
        WinPoint     sizeDest   = psurface->GetSize();
        int          pitchDest  = psurface->GetPitch();
        WORD         pixel      = (WORD)(ppf->MakePixel(color).Value());

        //ZAssert(pixelBytes == 2); // KGJV 32B
        DWORD        pixel32    = ppf->MakePixel(color).Value();
        //
        // loop through characters that are clipped on the left
        //

        bool    loop = true;
        while (loop)
        {
            BYTE    ch = str[ichar];
            switch (ch)
            {
                case START_COLOR_CODE:
                    DrawColorCodedString (psurface, x, rectClip, str, pdestTop, ytop, ybottom, ysize, ichar);
                    if (ichar >= length)
                        return;
                    break;
                case END_COLOR_CODE:
                    ichar += 2;
                    return;
                default:
                    {
                        const CharData& charData  = m_data[ch];
                        int             xsizeChar = charData.m_size.X();
                        if ((x + xsizeChar) > rectClip.XMin())
                        {
                            loop = false;
                            break;
                        }
                        if (++ichar >= length)
                            return;
                        x += xsizeChar;
                    }
                    break;
            }
        }

        //
        // there can be one character clipped on the left
        //

        if (x < rectClip.XMin())
        {
            DrawClippedChar(rectClip, str[ichar], ytop, ysize, x, pdestTop, pixel, pitchDest, pixelBytes);
            if (++ichar >= length)
                return;
            x     += m_data[(BYTE)str[ichar]].m_size.X();
        }

        //
        // loop through unclipped characters
        //
        loop = true;
        while (loop)
        {
            BYTE            ch        = str[ichar];
            switch (ch)
            {
                case START_COLOR_CODE:
                    DrawColorCodedString (psurface, x, rectClip, str, pdestTop, ytop, ybottom, ysize, ichar);
                    if (ichar >= length)
                        return;
                    break;
                case END_COLOR_CODE:
                    ichar += 2;
                    return;
                default:
                    {
                        const CharData& charData  = m_data[ch];
                        int             xsizeChar = charData.m_size.X();
                        if ((x + xsizeChar) > rectClip.XMax())
                        {
                            loop = false;
                            break;
                        }

                        int   scans               = ysize;
                        int   bytesSource         = (xsizeChar + 7) / 8;
                        BYTE* psource             = m_pdata + charData.m_offset + (bytesSource * ytop);
                        BYTE* pdest               = pdestTop + x * pixelBytes; // KGJV 32B (was 2)

                        //
                        // transparent blt 1bpp -> 16bpp
                        //

                        // KGJV 32B - 16 bpp mode
                        if (pixelBytes == 2)
                        {
                        _asm {
                            mov     esi, psource
                            mov     ax,  pixel
                            mov     ecx, pdest

                          yloop:
                            mov     edi, ecx
                            mov     ebx, [bytesSource]

                          byteLoop:
                            mov     dl, [esi]
                            inc     esi
                            shr     dl, 1
                            jnc     skipbit1
                            mov     [edi + 0], ax
                          skipbit1:
                            shr     dl, 1
                            jnc     skipbit2
                            mov     [edi + 2], ax
                          skipbit2:
                            shr     dl, 1
                            jnc     skipbit3
                            mov     [edi + 4], ax
                          skipbit3:
                            shr     dl, 1
                            jnc     skipbit4
                            mov     [edi + 6], ax
                          skipbit4:
                            shr     dl, 1
                            jnc     skipbit5
                            mov     [edi + 8], ax
                          skipbit5:
                            shr     dl, 1
                            jnc     skipbit6
                            mov     [edi + 10], ax
                          skipbit6:
                            shr     dl, 1
                            jnc     skipbit7
                            mov     [edi + 12], ax
                          skipbit7:
                            shr     dl, 1
                            jnc     skipbit8
                            mov     [edi + 14], ax
                          skipbit8:
                            add     edi, 16
                            dec     ebx
                            jne     byteLoop

                            add     ecx, [pitchDest]
                            dec     [scans]
                            jne     yloop
                        }
                        }

                        else  // KGJV 32B - (assume) 32 bpp mode
                        {
                        _asm {
                            mov     esi, psource
                            mov     eax, pixel32
                            mov     ecx, pdest

                          b32_yloop:
                            mov     edi, ecx
                            mov     ebx, [bytesSource]

                          b32_byteLoop:
                            mov     dl, [esi]
                            inc     esi
                            shr     dl, 1
                            jnc     b32_skipbit1
                            mov     [edi + 0], eax
                          b32_skipbit1:
                            shr     dl, 1
                            jnc     b32_skipbit2
                            mov     [edi + 4], eax
                          b32_skipbit2:
                            shr     dl, 1
                            jnc     b32_skipbit3
                            mov     [edi + 8], eax
                          b32_skipbit3:
                            shr     dl, 1
                            jnc     b32_skipbit4
                            mov     [edi + 12], eax
                          b32_skipbit4:
                            shr     dl, 1
                            jnc     b32_skipbit5
                            mov     [edi + 16], eax
                          b32_skipbit5:
                            shr     dl, 1
                            jnc     b32_skipbit6
                            mov     [edi + 20], eax
                          b32_skipbit6:
                            shr     dl, 1
                            jnc     b32_skipbit7
                            mov     [edi + 24], eax
                          b32_skipbit7:
                            shr     dl, 1
                            jnc     b32_skipbit8
                            mov     [edi + 28], eax

                          b32_skipbit8:
                            add     edi, 32
                            dec     ebx
                            jne     b32_byteLoop

                            add     ecx, [pitchDest]
                            dec     [scans]
                            jne     b32_yloop
                        }
                        }
                        if (++ichar >= length)
                            return;
                        x += xsizeChar;
                    }
                    break;
            }

        }

        //
        // one character can be clipped on the right
        //

        if (x < rectClip.XMax())
		{
           DrawClippedChar(rectClip, str[ichar], ytop, ysize, x, pdestTop, pixel32, pitchDest, pixelBytes);
		}
    }
};

TRef<IEngineFont> CreateEngineFont(HFONT hfont)
{
    return new FontImpl(hfont);
}

TRef<IEngineFont> CreateEngineFont(IBinaryReaderSite* psite)
{
    return new FontImpl(psite);
}

//////////////////////////////////////////////////////////////////////////////
//
// Color code utility function
//
//////////////////////////////////////////////////////////////////////////////

ZString ConvertColorToString (const Color& color)
{
    char    buffer[9] = {0};
    sprintf (buffer, "%02x%02x%02x%02x",
        int (color.R () * 255.0f),
        int (color.G () * 255.0f),
        int (color.B () * 255.0f),
        int (color.A () * 255.0f)
    );
    return ZString (buffer);
}

//////////////////////////////////////////////////////////////////////////////
//
// Fonts
//
//////////////////////////////////////////////////////////////////////////////

ZString GetString(int indent, TRef<IEngineFont> pfont)
{
    ZUnimplemented();

    return "font";
}

void Write(IMDLBinaryFile* pmdlFile, TRef<IEngineFont> pfont)
{
    FontImpl* pimpl; CastTo(pimpl, pfont);

    pimpl->Write(pmdlFile);
}

ZString GetFunctionName(const TRef<IEngineFont>& value)
{
    return "ImportFont";
}

//////////////////////////////////////////////////////////////////////////////
//
// Alternate blting implementations
//
//////////////////////////////////////////////////////////////////////////////

            /* version 1
            _asm {
                mov     edi, pdest
                mov     esi, psource
                mov     ax,  pixel

              yloop:
                mov     ebx, bytesSource
                and     ebx, ebx
                je      extraLoop

              byteLoop:
                mov     dl, [esi]
                inc     esi
                mov     ecx, 8

              bitloop1:
                shr     dl, 1
                jnc     bitloop1Skip
                mov     [edi], ax
              bitloop1Skip:
                add     edi, 2
                dec     ecx
                jne     bitloop1

                dec     ebx
                jne     byteLoop

              extraLoop:
                mov     ecx, extraSource
                and     ecx, ecx
                je      end

                mov     dl, [esi]
                inc     esi

              bitloop2:
                shr     dl, 1
                jnc     bitloop2Skip
                mov     [edi], ax
              bitloop2Skip:
                add     edi, 2
                dec     ecx
                jne     bitloop2

              end:
                add     edi, [scanDeltaDest]
                dec     [ysizeSource]
                jne     yloop
            }
            */

            /* this doesn't work yet.  Try to use a jump table dependant on the number of bits
            static void* bitJumps[8] = {
                bits8,
                bits1,
                bits2,
                bits3,
                bits4,
                bits5,
                bits6,
                bits7
            };

            _asm {
                mov     esi, psource
                mov     ax,  pixel
                mov     ecx, pdest

              ayloop:
                mov     edi, ecx

                //
                // eax  pixel
                // ebx  temporary
                // ecx  pdestScanLine
                // dl   bits
                // edi  destination
                // esi  source
                // ebp
                //

              byteLoop:
                mov     dl, [esi]
                inc     esi

                mov     ebx, [xsize]
                cmp     ebx, 8
                jg      bits8
                and     ebx, 0x7
                jmp     [bitJumps + ecx]

              bits8:
                shl     dl, 1
                jnc     bits7
                mov     [edi + 14], ax
              bits7:
                shl     dl, 1
                jnc     bits6
                mov     [edi + 12], ax
              bits6:
                shl     dl, 1
                jnc     bits5
                mov     [edi + 10], ax
              bits5:
                shl     dl, 1
                jnc     bits4
                mov     [edi +  8], ax
              bits4:
                shl     dl, 1
                jnc     bits3
                mov     [edi +  6], ax
              bits3:
                shl     dl, 1
                jnc     bits2
                mov     [edi +  4], ax
              bits2:
                shl     dl, 1
                jnc     bits1
                mov     [edi +  2], ax
              bits1:
                shl     dl, 1
                jnc     bits1
                mov     [edi +  0], ax

                add     edi, 16
                dec     [xsize]
                jne     byteLoop

                add     ecx, [pitchDest]
                dec     [ysizeSource]
                jne     ayloop
            }
            */

            /*
            do {
                //
                // Copy the byte sized chunks
                //

                {

                    for (int iBytes = bytesSource; iBytes > 0; iBytes--) {
                        BYTE byte = *psource;
                        psource++;

                        for (int index = 8; index > 0; index--) {
                            if (byte & 1) {
                                *(WORD*)pdest = pixel;
                            }

                            byte >>= 1;
                            pdest += 2;
                        }
                    }
                }

                //
                // Copy the last few bits
                //

                if (extraSource != 0) {
                    BYTE byte = *psource;
                    psource++;
                    for (int index = extraSource; index > 0; index--) {
                        if (byte & 1) {
                            *(WORD*)pdest = pixel;
                        }

                        byte >>= 1;
                        pdest += 2;
                    }
                }

                //
                // Next scan line
                //

                pdest       += scanDeltaDest;
                psource     += scanDeltaSource;
                ysizeSource -= 1;
            } while (ysizeSource > 0);
            */
