#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Viewport
//
//////////////////////////////////////////////////////////////////////////////

const Rect& Viewport::GetRect()
{
    return GetViewRect()->GetValue();
}

float Viewport::GetAspect()
{
    const Rect& rect = GetViewRect()->GetValue();

    return rect.YSize() / rect.XSize();
}

Point Viewport::TransformNDCToImage(const Point& point)
{
    const Rect& rect = GetViewRect()->GetValue();

    return rect.Center() + point * (rect.XSize() * 0.5f);
}

WinPoint Viewport::TransformImageToScreen(const Point& point)
{
    const Rect& rect = GetViewRect()->GetValue();

    return
        WinPoint(
            (int)point.X(),
            (int)(rect.YMin() + rect.YMax() - point.Y())
        );
}

WinPoint Viewport::TransformNDCToScreen(const Point& point)
{
    return TransformImageToScreen(TransformNDCToImage(point));
}
