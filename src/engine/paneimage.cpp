#include "paneimage.h"

#include "model.h"
#include "surface.h"
#include "toppane.h"

/////////////////////////////////////////////////////////////////////////////
//
// PaneImage
//
/////////////////////////////////////////////////////////////////////////////

class PaneImage : public Image {
private:
    /////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    /////////////////////////////////////////////////////////////////////////////

    class ImageTopPaneSiteImpl : public TopPaneSite {
    private:
        PaneImage* m_pimage;

    public:
        ImageTopPaneSiteImpl(PaneImage* pimage) :
            m_pimage(pimage)
        {
        }

        Point TransformLocalToImage(const Point& point)
        {
            return m_pimage->TransformLocalToImage(point);
        }

        void SurfaceChanged()
        {
            m_pimage->PaneChanged();
        }

        void SizeChanged()
        {
            m_pimage->PaneChanged();
        }
    };

    /////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    /////////////////////////////////////////////////////////////////////////////

    TRef<TopPane> m_ptopPane;
    Point         m_pointOrigin;

public:
    /////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    /////////////////////////////////////////////////////////////////////////////

    PaneImage(Engine* pengine, bool bColorKey, Pane* ppane)
    {
        m_ptopPane =
            new TopPane(
                pengine,
                bColorKey,
                new ImageTopPaneSiteImpl(this),
                ppane
            ); //Fix memory leak -Imago 8/2/09
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void PaneChanged()
    {
        Changed();
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void CalcBounds()
    {
        m_bounds.SetRect(
            Rect(
                Point(0, 0),
                Point::Cast(m_ptopPane->GetSize())
            )
        );
    }

    void Render(Context* pcontext)
    {
        ZEnter("PaneImage::Render()");

        pcontext->TransformLocalToImage(Vector(0, 0, 0), m_pointOrigin);

        // panes don't make use of the context, apply changes now
        pcontext->UpdateState();

        Surface* surface = m_ptopPane->GetSurface();

        //the pane rendering calls may have made all kinds of changes our context isn't aware of, reset
        pcontext->ForceState();

        //if the surface is not initialized, the pane was likely empty.
        if (surface) {
            pcontext->DrawImage3D(surface, Color(1, 1, 1));
        }

        ZExit("PaneImage::Render()");
    }

    TRef<Surface> GetSurface()
    {
        return m_ptopPane->GetSurface();
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // Value methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        m_ptopPane->Evaluate();
        Image::Evaluate();
    }

    ZString GetFunctionName() 
    { 
        return "PaneImage"; 
    }

    Point TransformLocalToImage(const Point& point)
    {
        return m_pointOrigin + point;
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void RemoveCapture()
    {
        m_ptopPane->RemoveCapture();
    }

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        return m_ptopPane->HitTest(pprovider, point, bCaptured);
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    {
        m_ptopPane->MouseMove(pprovider, point, bCaptured, bInside);
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        m_ptopPane->MouseEnter(pprovider, point);
    }

    void MouseLeave(IInputProvider* pprovider)
    {
        m_ptopPane->MouseLeave(pprovider);
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        return m_ptopPane->Button(pprovider, point, button, bCaptured, bInside, bDown);
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
/////////////////////////////////////////////////////////////////////////////

TRef<Image> CreatePaneImage(Engine* pengine, bool bColorKey, Pane* ppane)
{
    return new PaneImage(pengine, bColorKey, ppane); //Fix memory leak -Imago 8/2/09
}
