#ifndef _pixel_h_
#define _pixel_h_

#include <cstdint>

//////////////////////////////////////////////////////////////////////////////
//
// Pixel
//
//////////////////////////////////////////////////////////////////////////////

class Pixel {
private:
    uint32_t m_dw;

    Pixel(uint32_t dw) :
        m_dw(dw)
    {
    }

public:
    Pixel(const Pixel& pixel) :
        m_dw(pixel.m_dw)
    {
    }

    static Pixel Create(uint32_t dw) { return Pixel(dw); }

    uint32_t Value() const { return m_dw; }
};

#endif
