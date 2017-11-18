#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
//
// TopPaneSiteImpl
//
/////////////////////////////////////////////////////////////////////////////

class WindowTopPaneSiteImpl : public TopPaneSite {
private:
    PaneWindow* m_pwindow;

public:
    WindowTopPaneSiteImpl(PaneWindow* pwindow) :
        m_pwindow(pwindow)
    {
    }

    Point TransformLocalToImage(const Point& point)
    {
        return m_pwindow->TransformLocalToImage(point);
    }

    void SurfaceChanged()
    {
        m_pwindow->PaneSurfaceChanged();
    }

    void SizeChanged()
    {
        m_pwindow->PaneSizeChanged();
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// PaneWindow
//
/////////////////////////////////////////////////////////////////////////////

const WPARAM IDD_SizeChanged    = 0;
const WPARAM IDD_SurfaceChanged = 1;

PaneWindow::PaneWindow(
    Window* pwindow,
    Engine* pengine,
    Pane* ppane,
    SurfaceType stype
) :
    ChildWindow(
        pwindow,
        WinRect(0, 0, 0, 0)
    ),
    m_pengine(pengine)
{
    m_ptopPane =
        new TopPane(
            pengine,
            stype,
            false,
            new WindowTopPaneSiteImpl(this),
            ppane
        );

    SetMouseInput(m_ptopPane);

    UpdateSize();
}

void PaneWindow::SetPane(Pane* ppane)
{
    m_ptopPane->RemoveAllChildren();
    m_ptopPane->InsertAtTop(ppane);
    UpdateSize();
}

Point PaneWindow::TransformLocalToImage(const Point& point)
{
    return point;
}

void PaneWindow::UpdateSize()
{
    SetClientSize(m_ptopPane->GetSurfaceSize());
}

void PaneWindow::PaneSizeChanged()
{
    PostMessage(WM_COMMAND, IDD_SizeChanged, 0);
}

void PaneWindow::PaneSurfaceChanged()
{
    PostMessage(WM_COMMAND, IDD_SurfaceChanged, 0);
}

bool PaneWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch(wParam) {
        case IDD_SizeChanged:
            UpdateSize();
            return true;

        case IDD_SurfaceChanged:
            //InvalidateRgn();
            HDC hdc = GetDC();
            OnPaint(hdc, WinRect(WinPoint(0, 0), GetClientRect().Size()));
            ReleaseDC(hdc);
            return true;
    }

    return false;
}

void PaneWindow::OnPaint(HDC hdc, const WinRect& rect)
{
    Surface* psurface = m_ptopPane->GetSurface();

    m_pengine->BltToWindow(
        this,
        WinPoint(0, 0),
        psurface,
        WinRect(
            WinPoint(0, 0),
            psurface->GetSize()
        )
    );
}
