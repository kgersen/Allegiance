#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
//
// Pane Statics
//
/////////////////////////////////////////////////////////////////////////////

Color Pane::s_colors[SystemColorMax];

void Pane::Initialize()
{
    s_colors[SystemColor3DHighLight  ] = MakeColorFromCOLORREF((COLORREF)GetSysColor(COLOR_3DHIGHLIGHT));
    s_colors[SystemColor3DShadow     ] = MakeColorFromCOLORREF((COLORREF)GetSysColor(COLOR_3DSHADOW));
    s_colors[SystemColor3DDKShadow   ] = MakeColorFromCOLORREF((COLORREF)GetSysColor(COLOR_3DDKSHADOW));
    s_colors[SystemColorWindowText   ] = MakeColorFromCOLORREF((COLORREF)GetSysColor(COLOR_WINDOWTEXT));
    s_colors[SystemColorHighLightText] = MakeColorFromCOLORREF((COLORREF)GetSysColor(COLOR_HIGHLIGHTTEXT));
}

const Color& Pane::GetSystemColor(int index)
{
    ZAssert(index >= 0 && index < SystemColorMax);

    return s_colors[index];
}

void Pane::SetSystemColor(int index, const Color& color)
{
    ZAssert(index >= 0 && index < SystemColorMax);

    s_colors[index] = color;
}

/////////////////////////////////////////////////////////////////////////////
//
// Pane
//
/////////////////////////////////////////////////////////////////////////////

Pane::Pane(Pane* pchild, const WinPoint& size) :
    m_index(0),
    m_pparent(NULL),
    m_paintSize(0, 0),
    m_paintOffset(0, 0),
    m_bHiddenPaint(false),
    m_offset(0, 0),
    m_expand(0, 0),
    m_size(size),
    m_bHidden(false),
    m_bXExpandable(true),
    m_bYExpandable(true),
    m_bSelected(false),
	m_bOpaque(false),
    m_bPaintAll(true),
    m_bNeedPaint(true)
{
    if (pchild) {
        InsertAtBottom(pchild);
    }
}

Pane::~Pane()
{
    TRef<Pane> ppane = m_pchild;

    while (ppane) {
        TRef<Pane> ppaneNext = ppane->m_pnext;
        ppane->m_pparent = NULL;
        ppane->m_pnext = NULL;
        ppane = ppaneNext;
    }
}

void Pane::SetIndex(int index)
{
    m_index = index;
}

void Pane::SetSelected(bool bSelected)
{
    if (m_bSelected != bSelected) {
        m_bSelected = bSelected;
        NeedPaint();
    }
}

bool Pane::IsSelected() const
{
    return m_bSelected || (m_pparent != NULL && m_pparent->IsSelected());
}

void Pane::InternalSetSize(const WinPoint& point)
{
    ZAssert(point.X() >= 0 && point.Y() >= 0);
    m_size = point;
}

void Pane::InternalSetOffset(const WinPoint& point)
{
    m_offset = point;
}

void Pane::InternalSetExpand(const WinPoint& point)
{
    ZAssert(point.X() >= 0 && point.Y() >= 0);

    if (m_bXExpandable) {
        m_expand.SetX(point.X());
    }

    if (m_bYExpandable) {
        m_expand.SetY(point.Y());
    }
}

void Pane::InternalSetHidden(bool bHidden)
{
    m_bHidden = bHidden;
}

void Pane::SetOffset(const WinPoint& point)
{
    InternalSetOffset(point);
    NeedLayout();
}

void Pane::SetSize(const WinPoint& point)
{
    InternalSetSize(point);
    NeedLayout();
}

void Pane::SetExpand(const WinPoint& point)
{
    InternalSetExpand(point);
    NeedLayout();
}

void Pane::SetHidden(bool bHidden)
{
    InternalSetHidden(bHidden);
    NeedLayout();
}

void Pane::SetXExpandable(bool bExpandable)
{
    m_bXExpandable = bExpandable;
    NeedLayout();
}

void Pane::SetYExpandable(bool bExpandable)
{
    m_bYExpandable = bExpandable;
    NeedLayout();
}

int Pane::GetAlignedXSize(int xPos)
{
    return XSize(); 
}

int Pane::GetChildCount() const
{
    int index = 0;
    Pane* ppane = m_pchild;

    while (ppane) {
        index++;
        ppane = ppane->m_pnext;
    }

    return index;
}

int Pane::FindChild(Pane* pchild) const
{
    int index = 0;
    Pane* ppane = m_pchild;

    while (ppane) {
        if (ppane == pchild) {
            return index;
        }
        index++;
        ppane = ppane->m_pnext;
    }

    return -1;
}

Pane* Pane::FindChild(int index) const
{
    Pane* ppane = m_pchild;

    while (ppane) {
        if (index == 0) {
            return ppane;
        }
        index--;
        ppane = ppane->m_pnext;
    }

    return NULL;
}

bool Pane::IsAncestor(Pane* ppaneAncestor) const
{
    if (ppaneAncestor == this) {
        return true;
    }

    if (m_pparent) {
        return m_pparent->IsAncestor(ppaneAncestor);
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
// Pane add and remove children
//
/////////////////////////////////////////////////////////////////////////////

void Pane::RemoveSelf()
{
    if (m_pparent != NULL) {
        m_pparent->RemoveChild(this);
    }
}

void Pane::RemoveChild(Pane* ppane)
{
    TRef<Pane> ppaneNext = ppane->m_pnext;

    ppane->m_pparent = NULL;
    ppane->m_pnext = NULL;

    TRef<Pane>* pppane = m_pchild.Pointer();

    while ((*pppane) != ppane) {
        pppane = (*pppane)->m_pnext.Pointer();
    }

    *pppane = ppaneNext;

    NeedLayout();
    NeedPaint();
    m_bPaintAll = true;
}

void Pane::RemoveChild(int index)
{
    ZAssert(index >= 0);

    TRef<Pane> ppane = m_pchild;

    for(; index > 0; index--) {
        ppane = ppane->m_pnext;
    }

    ppane->RemoveSelf();
}

void Pane::RemoveAllChildren()
{
    while (Child()) {
        Child()->RemoveSelf();
    }
}

void Pane::Insert(int index, Pane* ppane)
{
    ZAssert(index >= 0);

    // make sure ppane doesn't get deleted while switching pointers around

    TRef<Pane> ppaneSave = ppane;
    ppane->RemoveSelf();

    TRef<Pane>* pppane = m_pchild.Pointer();

    for(; index > 0; index--) {
        pppane = (*pppane)->m_pnext.Pointer();
    }

    ppane->m_pnext = (*pppane);
    ppane->m_pparent = this;
    *pppane = ppane;

    NeedLayout();
    NeedPaint();
    m_bPaintAll = true;
}

void Pane::InsertAtBottom(Pane* ppane)
{
    TRef<Pane> ppaneSave = ppane;
    ppane->RemoveSelf();

    Insert(GetChildCount(), ppane);
}

void Pane::InsertAtTop(Pane* ppane)
{
    Insert(0, ppane);
}

/////////////////////////////////////////////////////////////////////////////
//
// Pane painting
//
/////////////////////////////////////////////////////////////////////////////

void Pane::NeedPaint()
{
    m_bPaintAll = true;
    NeedPaintInternal();
}

void Pane::NeedPaintInternal()
{
    if (!m_bNeedPaint) {
        m_bNeedPaint = true;
        if (m_pparent) {
            m_pparent->NeedPaintInternal();
        }
    }
}

void Pane::Paint(Surface* psurface)
{
}

void Pane::PaintAll(Surface* psurface)
{
    if (!m_bHidden) {
        Paint(psurface);

        for(Pane* ppane = m_pchild; ppane != NULL; ppane = ppane->m_pnext) {
            WinRect rectClipOld = psurface->GetClipRect();
            psurface->Offset(ppane->m_offset);
            psurface->SetClipRect(WinRect(WinPoint(0, 0), ppane->GetSize()));

            ppane->PaintAll(psurface);

            psurface->Offset(-ppane->m_offset);
            psurface->RestoreClipRect(rectClipOld);
        }

        m_bNeedPaint = false;
        m_bPaintAll  = false;
    }
}

void Pane::TunnelPaint(Surface* psurface, bool bPaintAll)
{
    if (m_pparent != NULL && !m_bOpaque) {
        psurface->Offset(-m_offset);
        m_pparent->TunnelPaint(psurface, false);    
        psurface->Offset(m_offset);
    }

    if (bPaintAll) {
        PaintAll(psurface);
    } else {
        Paint(psurface);
    }
}

void Pane::InternalPaint(Surface* psurface)
{
    if (m_bPaintAll) {
        TunnelPaint(psurface, true);
    } else if (m_bNeedPaint) {
        if (m_pchild != NULL) {
            for(Pane* ppane = m_pchild; ppane != NULL; ppane = ppane->m_pnext) {
                WinRect rectClipOld = psurface->GetClipRect();
                psurface->Offset(ppane->m_offset);
                psurface->SetClipRect(WinRect(WinPoint(0, 0), ppane->GetSize()));

                ppane->InternalPaint(psurface);

                psurface->Offset(-ppane->m_offset);
                psurface->RestoreClipRect(rectClipOld);
            }
        } else {
            TunnelPaint(psurface, false);
        }

        m_bNeedPaint = false;
    }
}

bool Pane::CalcPaint()
{
    if (m_bHidden) {
        m_paintOffset = WinPoint(0, 0);
        m_paintSize   = WinPoint(0, 0);
        m_bNeedPaint  = false;
        m_bPaintAll   = false;

        if (m_bHiddenPaint != m_bHidden) {
            m_bHiddenPaint = m_bHidden;
            return true;
        }

        return false;
    }

    bool bAnyChildSized = false;

    for(Pane* ppane = m_pchild; ppane != NULL; ppane = ppane->m_pnext) {
        bAnyChildSized |= ppane->CalcPaint();
        if (ppane->m_bNeedPaint) {
            m_bNeedPaint = true;
        }
    }

    if (bAnyChildSized) {
        m_bNeedPaint = true;
        m_bPaintAll = true;
    }

    if (
           m_paintOffset  != m_offset 
        || m_paintSize    != m_size
        || m_bHiddenPaint != m_bHidden
    ) {
        m_bNeedPaint = true;
        m_bPaintAll  = true;

        m_paintOffset  = m_offset;
        m_paintSize    = m_size;
        m_bHiddenPaint = m_bHidden;

        return true;
    } else {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Pane layout
//
/////////////////////////////////////////////////////////////////////////////

void Pane::NeedLayout()
{
    if (m_pparent) {
        m_pparent->NeedLayout();
    }
}

bool Pane::NeedEvenHeight()
{
    return false;
}

void Pane::UpdateLayout()
{
    Pane* ppane;
    for(ppane = Child(); ppane != NULL; ppane = ppane->Next()) {
        ppane->UpdateLayout();
    }
}

void Pane::DefaultUpdateLayout()
{
    if (m_pchild) {
        m_pchild->InternalSetExpand(m_expand);
        m_pchild->UpdateLayout();
        m_pchild->InternalSetOffset(WinPoint(0, 0));
        m_size = m_pchild->GetSize();
    } else {
        m_size = m_expand;
    }
}

WinPoint Pane::GetOffsetFrom(Pane* ppane) const
{
    if (m_pparent == ppane) {
        return m_offset;
    } else {
        return m_offset + m_pparent->GetOffsetFrom(ppane);
    }
}

Point Pane::TransformLocalToImage(const WinPoint& point)
{
    return m_pparent->TransformLocalToImage(point + m_offset);
}

/////////////////////////////////////////////////////////////////////////////
//
// IMouseInput
//
/////////////////////////////////////////////////////////////////////////////

void Pane::RemoveCapture()
{
    if (m_ppaneCapture) {
        m_ppaneCapture->RemoveCapture();
        m_ppaneCapture = NULL;
    }
}

Pane* Pane::GetHitPane() 
{ 
    return m_ppaneHit;
}

Pane* Pane::GetCapturePane() 
{ 
    return m_ppaneCapture;
}

MouseResult Pane::HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
{
    bool bInside = 
           !m_bHidden
        && point.X() >= 0
        && point.X() < (float)XSize() 
        && point.Y() >= 0 
        && point.Y() < (float)YSize();

    if (m_ppaneCapture) {
        ZAssert(bCaptured);

        //
        // Is the mouse over the captured pane?
        //

        MouseResult mouseResult = 
            m_ppaneCapture->HitTest(
                pprovider, 
                point - Point::Cast(m_ppaneCapture->m_offset),
                true
            );

        if (mouseResult.Test(MouseResultHit())) {
            m_ppaneHit = m_ppaneCapture;
        } else {
            m_ppaneHit = NULL;
        }

        //
        // Release Capture?
        //

        if (mouseResult.Test(MouseResultRelease())) {
            RemoveCapture();
            return mouseResult;
        }

        //
        // Call MouseMove
        //

        m_ppaneCapture->MouseMove(
            pprovider,
            point - Point::Cast(m_ppaneCapture->m_offset),
            true,
            m_ppaneHit != NULL
        );
    } else {
        //
        //
        // Which image are we over?
        //

        Pane* ppaneHit  = NULL;

        // !!! if (bInside || m_ppaneHit != NULL) {

        if (bInside) {
            TRef<Pane> ppane;

            for(ppane = m_pchild; ppane!= NULL; ppane = ppane->m_pnext) {
                MouseResult mouseResult =
                    ppane->HitTest(
                        pprovider,
                        point - Point::Cast(ppane->m_offset),
                        false
                    );

                if (mouseResult.Test(MouseResultHit())) {
                    // !!! some of the mdl files violate this
                    // ZAssert(ppaneHit == NULL);
                    if (ppaneHit == NULL) {
                        ppaneHit = ppane;
                    }
                }
            }
        }

        //
        // Call MouseMove, MouseEnter, or MouseLeave
        //

        if (m_ppaneHit != ppaneHit) {
            if (m_ppaneHit) {
                m_ppaneHit->MouseLeave(pprovider);
            }
            m_ppaneHit = ppaneHit;
            if (m_ppaneHit) {
                m_ppaneHit->MouseEnter(
                    pprovider,
                    point - Point::Cast(m_ppaneHit->m_offset)
                );
            }
        } else if (m_ppaneHit) {
            m_ppaneHit->MouseMove(
                pprovider,
                point - Point::Cast(m_ppaneHit->m_offset),
                false,
                true
            );
        }
    }

    if (bInside) {
        return MouseResultHit();
    } else {
        return MouseResult();
    }
}

void Pane::MouseLeave(IInputProvider* pprovider)
{
    if (m_ppaneHit) {
        m_ppaneHit->MouseLeave(pprovider);
        m_ppaneHit = NULL;
    }
}

MouseResult Pane::Button(
    IInputProvider* pprovider,
    const Point& point,
    int button,
    bool bCaptured,
    bool bInside,
    bool bDown
) {
    MouseResult mouseResult;
    TRef<Pane> pthis = this;

    if (m_ppaneCapture) {
        mouseResult =
            m_ppaneCapture->Button(
                pprovider,
                point - Point::Cast(m_ppaneCapture->m_offset),
                button,
                true,
                m_ppaneHit != NULL,
                bDown
            );

        if (mouseResult.Test(MouseResultRelease())) {
            m_ppaneCapture = NULL;
        }
    } else if (m_ppaneHit) {
        mouseResult = 
            m_ppaneHit->Button(
                pprovider,
                point - Point::Cast(m_ppaneHit->m_offset),
                button,
                false,
                true,
                bDown
            );

        if (mouseResult.Test(MouseResultCapture())) {
            m_ppaneCapture = m_ppaneHit;
        }
    }

    return mouseResult;
}
