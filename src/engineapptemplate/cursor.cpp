//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class CursorSurfaceSite : public SurfaceSite {
public:
    void UpdateSurface(Surface* psurface) 
    {
        const Color& color = Color::White();

        const WinPoint& size = psurface->GetSize();
        int             x    = size.X();
        int             y    = size.Y();

        psurface->FillSurface(Color::Black());

        psurface->FillRect(WinRect(        0, y / 2 - 1, x / 2 - 2, y / 2 + 1), color);
        psurface->FillRect(WinRect(x / 2 + 2, y / 2 - 1,         x, y / 2 + 1), color);

        psurface->FillRect(WinRect(x / 2 - 1,         0, x / 2 + 1, y / 2 - 2), color);
        psurface->FillRect(WinRect(x / 2 - 1, y / 2 + 2, x / 2 + 1,         y), color);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

TRef<Image> CreateCursor(Engine* pengine)
{
    volatile float size = 16;

    TRef<Surface> psurfaceCursor =
        pengine->CreateSurface(
            WinPoint(int(size), int(size)), 
            SurfaceType2D(), 
            new CursorSurfaceSite()
        );

    psurfaceCursor->SetColorKey(Color::Black());

    return
       new TransformImage(
            new ConstantImage(
                psurfaceCursor,
                ZString("cursor")
            ),
            new TranslateTransform2(
                new PointValue(
                    Point(-(size / 2), -(size / 2))
                )
            )
       );
}

