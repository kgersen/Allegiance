/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PaneWindow_H_
#define _PaneWindow_H_

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

class PaneWindow : public ChildWindow {
private:
    TRef<TopPane>   m_ptopPane;
    TRef<Engine>    m_pengine;

public:
    PaneWindow(
        Window* pwindow,
        Engine* pengine,
        Pane* ppane = NULL,
        SurfaceType = SurfaceType2D()
    );

    void SetPane(Pane* ppane);
    void UpdateSize();
    void PaneSizeChanged();
    void PaneSurfaceChanged();
    Point TransformLocalToImage(const Point& point);

    //
    // window members
    //

    void OnPaint(HDC hdc, const WinRect& rect);
    bool OnCommand(WPARAM wParam, LPARAM lParam);
};

#endif
