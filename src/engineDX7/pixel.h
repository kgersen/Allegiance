#ifndef _pixel_h_
#define _pixel_h_

//////////////////////////////////////////////////////////////////////////////
//
// Pixel
//
//////////////////////////////////////////////////////////////////////////////

class Pixel {
private:
    DWORD m_dw;

    Pixel(DWORD dw) :
        m_dw(dw)
    {
    }

public:
    Pixel(const Pixel& pixel) :
        m_dw(pixel.m_dw)
    {
    }

    static Pixel Create(DWORD dw) { return Pixel(dw); }

    DWORD Value() const { return m_dw; }
};

#endif
