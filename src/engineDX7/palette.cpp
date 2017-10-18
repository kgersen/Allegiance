#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Palette
//
//
//
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////

class PaletteImpl : public PrivatePalette {
private:
    TVector<Color>            m_pcolors;
    TRef<IDirectDrawPaletteX> m_pddpal;
public:
    PaletteImpl(IDirectDrawPaletteX* pddpal) :
        m_pddpal(pddpal)
    {
        DWORD caps;

        DDCall(m_pddpal->GetCaps(&caps));

        int count;

        if (caps & DDPCAPS_4BIT) {
            count = 16;
        } else if (caps & DDPCAPS_8BIT) {
            count = 256;
        } else {
            ZError("Unsupported palette size");
        }

        PALETTEENTRY ppe[256];
        DDCall(m_pddpal->GetEntries(0, 0, count, ppe));

        m_pcolors.SetCount(count);
        for (int index = 0; index < count; index++) {
            float scale = (1.0f / 255);

            m_pcolors.Set(
                index,
                Color(
                    ppe[index].peRed   / 255.0f,
                    ppe[index].peGreen / 255.0f,
                    ppe[index].peBlue  / 255.0f
                )
            );
        }
    }

    IDirectDrawPaletteX* GetDDPal() { return m_pddpal; }
    const Color& GetColor(int index) { return m_pcolors[index]; }
};

TRef<PrivatePalette> CreatePaletteImpl(IDirectDrawPaletteX* pddpal)
{
    return new PaletteImpl(pddpal);
}
