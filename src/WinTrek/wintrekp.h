#ifndef _WinTrekP_H__
#define _WinTrekP_H__

//////////////////////////////////////////////////////////////////////////////
//
// Wintrek Private Headers
//
//////////////////////////////////////////////////////////////////////////////

class ThingSitePrivate : public ThingSite {
public:
    virtual void UpdateScreenPosition(
        const Point& pointScreenPosition,
        float fScreenRadius,
        float distanceToEdge,
        unsigned char ucRadarState
    ) {
    }
    virtual TRef<ISoundPositionSource> GetSoundSource(void) = 0;
};

#endif
