#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
//
// TopPane
//
/////////////////////////////////////////////////////////////////////////////

class TopPaneSurfaceSite : public SurfaceSite {
private:
    TopPane* m_ptopPane;

public:
    TopPaneSurfaceSite(TopPane* ptopPane) :
        m_ptopPane(ptopPane)
    {
    }

    void UpdateSurface(Surface* psurface)
    {
        m_ptopPane->RepaintSurface();
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// TopPane
//
/////////////////////////////////////////////////////////////////////////////

TopPane::TopPane(Engine* pengine, SurfaceType stype, bool bColorKey, TopPaneSite* psite, Pane* pchild) :
    Pane(pchild),
    m_pengine(pengine),
    m_stype(stype),
    m_psurface(pengine->CreateSurface(WinPoint(1, 1), stype, new TopPaneSurfaceSite(this))),
    m_psite(psite),
    m_bColorKey(bColorKey),
    m_bNeedLayout(true)
{
    if (m_bColorKey) {
        m_psurface->SetColorKey(Color(0, 0, 0));
    }
    SetSize(WinPoint(1, 1));
}

void TopPane::RepaintSurface()
{
    m_bNeedPaint = true;
    m_bPaintAll = true;
}

void TopPane::NeedLayout()
{
    if (!m_bNeedLayout) {
        m_bNeedLayout = true;
        m_psite->SizeChanged();
    }
}

void TopPane::NeedPaintInternal()
{
    if (!m_bNeedPaint) {
        m_bNeedPaint = true;
        m_psite->SurfaceChanged();
    }
}

void TopPane::Paint(Surface* psurface)
{
    // psurface->FillSurface(Color(0.8f, 0.5f, 1.0f));
}

void TopPane::Evaluate()
{
    if (m_bNeedLayout) {
        m_bNeedLayout = false;

        WinPoint sizeOld = GetSize();

        UpdateLayout();

        if (GetSize() != sizeOld) {
            m_bNeedPaint = true;
            m_bPaintAll = true;
            m_psurface = NULL;
            m_psurface =
                m_pengine->CreateSurface(
                    GetSize(),
                    m_stype,
                    new TopPaneSurfaceSite(this)
                );
            if (m_bColorKey) {
                m_psurface->SetColorKey(Color(0, 0, 0));
            }
        }
    }
}

void TopPane::UpdateLayout()
{
    DefaultUpdateLayout();
}

bool g_bPaintAll = false;

void TopPane::UpdateBits()
{
    ZEnter("TopPane::UpdateBits()");

    if (m_bNeedPaint) {
        ZTrace("m_bNeedPaint == true");
        if (CalcPaint()) {
            m_bNeedPaint = true;
            m_bPaintAll = true;
        }

        ZTrace("after CalcPaint() m_bNeedPaint ==" + ZString(m_bNeedPaint));
        ZTrace("after CalcPaint() m_bPaintAll  ==" + ZString(m_bPaintAll ));
        m_bPaintAll |= g_bPaintAll;
        InternalPaint(m_psurface);
        m_bNeedPaint = false;
    }

    ZExit("TopPane::UpdateBits()");
}

const WinPoint& TopPane::GetSurfaceSize()
{
    Evaluate();
    return GetSize();
}

Surface* TopPane::GetSurface()
{
    Evaluate();
    UpdateBits();

    return m_psurface;
}

Point TopPane::TransformLocalToImage(const WinPoint& point)
{
    return
        m_psite->TransformLocalToImage(
            GetPanePoint(
                Point::Cast(point)
            )
        );
}

Point TopPane::GetPanePoint(const Point& point)
{
    return
        Point(
            point.X(),
            (float)GetSize().Y() - 1.0f - point.Y()
        );
}

MouseResult TopPane::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    return Pane::HitTest(pprovider, GetPanePoint(point), bCaptured);
}

MouseResult TopPane::Button(
    IInputProvider* pprovider, 
    const Point& point, 
    int button, 
    bool bCaptured, 
    bool bInside, 
    bool bDown
) {
    return Pane::Button(pprovider, GetPanePoint(point), button, bCaptured, bInside, bDown);
}
