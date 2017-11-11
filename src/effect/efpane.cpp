#include "efpane.h"

#include <button.h>
#include <controls.h>
#include <event.h>
#include <namespace.h>
#include <tref.h>

#include "listpane.h"

/////////////////////////////////////////////////////////////////////////////
//
// ScreenPane
//
/////////////////////////////////////////////////////////////////////////////

class ScreenPane : public ImagePane {
private:
    Window*  m_pwindow;
    WinPoint m_pointWindowStart;
    WinPoint m_pointMouseStart;

public:
    ScreenPane(Window* pwindow, Image* pimage) :
        m_pwindow(pwindow),
        ImagePane(pimage)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// Gauge Pane
//
/////////////////////////////////////////////////////////////////////////////

GaugePane::GaugePane(Surface*         psurface,
                     const Color&     colorFlash,
                     float            minValue,
                     float            maxValue,
                     const Color&     colorEmpty)
:
    m_psurface(psurface),
    m_colorFlash(colorFlash),
    m_colorEmpty(colorEmpty),
    m_minValue(minValue),
    m_maxValue(maxValue),
    m_value(0),
    m_valueOld(0),
    m_valueFlash(0),
    m_timeLastChange(Time::Now())
{
    assert(m_psurface);
    assert(m_minValue < m_maxValue);

    InternalSetSize(m_psurface->GetSize());
}

void GaugePane::Paint(Surface* psurface)
{
    if (m_value != 0) {
        psurface->BitBlt(
            WinPoint(0, 0),
            m_psurface,
            WinRect(0, 0, m_value, YSize())
        );
    }

    if (m_value < m_valueFlash) {
        psurface->FillRect(
            WinRect(
                m_value,
                0,
                m_valueFlash,
                YSize()
            ),
            m_colorFlash
        );
    }
}

void GaugePane::SetValue(float v, bool fFlash)
{
    m_value =
        (int)bound(
            (v - m_minValue) * ((float)XSize()) / (m_maxValue - m_minValue),
            0.0f,
            (float)XSize()
        );
}

void GaugePane::Update(Time time)
{
    if (m_value != m_valueOld) {
        if (m_value < m_valueOld) {
            m_valueFlash = m_valueOld;
        } else {
            m_valueFlash = m_value;
        }

        m_timeLastChange = time;
        m_valueOld = m_value;

        NeedPaint();
    }

    if (m_value != m_valueFlash && time - m_timeLastChange > 0.25f) {
        m_valueFlash = m_value;
        NeedPaint();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// ParseIntVector
//
//////////////////////////////////////////////////////////////////////////////

void ParseIntVector(IObject* pobject, TVector<int>& vec)
{
    IObjectList* plistVertices; CastTo(plistVertices, pobject);

    plistVertices->GetFirst();

    while (plistVertices->GetCurrent() != NULL) {
        vec.PushEnd((int)GetNumber(plistVertices->GetCurrent()));
        plistVertices->GetNext();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// ParseFloatVector
//
//////////////////////////////////////////////////////////////////////////////

void ParseFloatVector(IObject* pobject, TVector<float>& vec)
{
    IObjectList* plistVertices; CastTo(plistVertices, pobject);

    plistVertices->GetFirst();

    while (plistVertices->GetCurrent() != NULL) {
        vec.PushEnd((float)GetNumber(plistVertices->GetCurrent()));
        plistVertices->GetNext();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// ParseStringVector
//
//////////////////////////////////////////////////////////////////////////////

void ParseStringVector(IObject* pobject, TVector<ZString>& vec)
{
    IObjectList* plistVertices; CastTo(plistVertices, pobject);

    plistVertices->GetFirst();

    while (plistVertices->GetCurrent() != NULL) {
        vec.PushEnd(GetString(plistVertices->GetCurrent()));
        plistVertices->GetNext();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// ThumbPane
//
//////////////////////////////////////////////////////////////////////////////

class ThumbPaneImpl : public ThumbPane{
private:
    TRef<Surface> m_psurface;
    bool          m_bHorizontal;

public:
    ThumbPaneImpl(Modeler* pmodeler, bool bHorizontal, TRef<Image>& pImageThumb) :
      m_bHorizontal(bHorizontal)
    {
// TODO        if (pImageThumb == NULL)

        if(bHorizontal)
            m_psurface =pmodeler->LoadSurface("scrollhorizontalthumbbmp", true);
        else
            m_psurface =pmodeler->LoadSurface("scrollthumbbmp", true);
    }

    void Paint(Surface* psurface)
    {
/*		PrivateSurface* pprivateSurface; CastTo(pprivateSurface, m_psurface);
		WinPoint currOffset, currSize;
		TEXHANDLE hTexture = pprivateSurface->GetTexHandle( );
		_ASSERT( hTexture != INVALID_TEX_HANDLE );

		// If the texture has alpha, enable blending.
		if( pprivateSurface->HasColorKey() == true )
		{
			CD3DDevice9::SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			CD3DDevice9::SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

			CD3DDevice9::SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			CD3DDevice9::SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			CD3DDevice9::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			CD3DDevice9::SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		}
		// Set texture, ensure z-testing is off.
		CD3DDevice9::SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		CVRAMManager::SetTexture( hTexture, 0 );

		// Draw horizontal or vertical bar.
		if( m_bHorizontal == true )
		{
			const WinPoint& size  = m_psurface->GetSize();
			int x = 0;

			while (x + size.X() < XSize()) 
			{
				// Generate offset and size.
				currOffset.SetX( psurface->GetOffset().X() + x);
				currOffset.SetY( psurface->GetOffset().Y() );
				currSize.SetX( size.X() );
				currSize.SetY( min( size.Y(), YSize() ) );
				
				// Create vertices and render.
				GenerateScreenVertices( hTexture, &currOffset, &currSize );
				HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UIVERTEX );
				hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
				hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
																m_dwNumPolys,
																m_pPaneVerts,
																sizeof( UIVERTEX ) );
				_ASSERT( hr == D3D_OK );
				x += size.X();
			}
			// Generate offset and size.
			currOffset.SetX( psurface->GetOffset().X() + x);
			currOffset.SetY( psurface->GetOffset().Y() );
			currSize.SetX( XSize() - x );
			currSize.SetY( min( size.Y(), YSize() ) );
			
			// Create vertices and render.
			GenerateScreenVertices( hTexture, &currOffset, &currSize );
			HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UIVERTEX );
			hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
			hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
															m_dwNumPolys,
															m_pPaneVerts,
															sizeof( UIVERTEX ) );
		}
		else
		{
			int y = 0;
			const WinPoint& size  = m_psurface->GetSize();
			
			while( y + size.Y() < YSize() ) 
			{
				// Generate offset and size.
				currOffset.SetX( psurface->GetOffset().X() );
				currOffset.SetY( psurface->GetOffset().Y() + y );
				currSize.SetX( min( size.X(), XSize() ) );
				currSize.SetY( size.Y() );
				
				// Create vertices and render.
				GenerateScreenVertices( hTexture, &currOffset, &currSize );
				HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UIVERTEX );
				hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
				hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
																m_dwNumPolys,
																m_pPaneVerts,
																sizeof( UIVERTEX ) );
				_ASSERT( hr == D3D_OK );
				y += size.Y();
			}

			// Generate offset and size.
			currOffset.SetX( psurface->GetOffset().X() );
			currOffset.SetY( psurface->GetOffset().Y() + y );
			currSize.SetX( min( size.X(), XSize() ) );
			currSize.SetY( YSize() - y );
			
			// Create vertices and render.
			GenerateScreenVertices( hTexture, &currOffset, &currSize );
			HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UIVERTEX );
			hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
			hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
															m_dwNumPolys,
															m_pPaneVerts,
															sizeof( UIVERTEX ) );
			_ASSERT( hr == D3D_OK );
		}
	}*/

       if (m_bHorizontal)
        {
            const WinPoint& size  = m_psurface->GetSize();
            int x = 0;

            while (x + size.X() < XSize()) {
                psurface->BitBlt(
                    WinPoint(x, 0),
                    m_psurface,
                    WinRect(0, 0, size.X(), std::min(size.Y(), YSize()))
                );
                x += size.X();
            }

            psurface->BitBlt(
                WinPoint(x, 0),
                m_psurface,
                WinRect(0, 0, XSize() - x, std::min(size.Y(), YSize()))
            );
        }
        else
        {
            const WinPoint& size  = m_psurface->GetSize();
            int y = 0;

            while (y + size.Y() < YSize()) {
                psurface->BitBlt(
                    WinPoint(0, y),
                    m_psurface,
                    WinRect(0, 0, std::min(size.X(), XSize()), size.Y())
                );
                y += size.Y();
            }

            psurface->BitBlt(
                WinPoint(0, y),
                m_psurface,
                WinRect(0, 0, std::min(size.X(), XSize()), YSize() - y)
            );
        }
    }

    void UpdateLayout()
    {
        WinPoint size = GetExpand();

        if (m_bHorizontal)
        {
            if (size.X() < 8) {
                size.SetX(8);
            }
        }
        else
        {
            if (size.Y() < 8) {
                size.SetY(8);
            }
        }

        InternalSetSize(size);
    }
};


TRef<ThumbPane> ThumbPane::Create(Modeler* pmodeler, 
                                  bool bHorizontal, 
                                  TRef<Image> pImageThumb) // Set this to NULL for default image
{
    return new ThumbPaneImpl(pmodeler, bHorizontal, pImageThumb);
}

//////////////////////////////////////////////////////////////////////////////
//
// TrekScrollPane
//
//////////////////////////////////////////////////////////////////////////////

TRef<Pane> CreateTrekScrollPane(
    WinPoint size,
    Modeler* pmodeler,
    TRef<IIntegerEventSource>& pevent,
    TRef<ScrollPane>& pscrollPane,
    bool bHoriztonal,
    TRef<ButtonPane>& pbuttonUp,
    TRef<ButtonPane>& pbuttonDown,
    TRef<Image>& pImageThumb
) {


    if (pbuttonUp == NULL || pbuttonDown == NULL)
    {                                   
        TRef<Image> pimageUp;      
        TRef<Image> pimageUpSel;   
        TRef<Image> pimageDown;    
        TRef<Image> pimageDownSel; 

        //
        // Load default images if no buttons are passed as params
        //
        if (bHoriztonal)
        {
            pimageUp      = pmodeler->LoadImage("scrollleftbmp"     , true);
            pimageUpSel   = pmodeler->LoadImage("scrollleftselbmp"  , true);
            pimageDown    = pmodeler->LoadImage("scrollrightbmp"   , true);
            pimageDownSel = pmodeler->LoadImage("scrollrightselbmp", true);
        }
        else
        {
            pimageUp      = pmodeler->LoadImage("scrollupbmp"     , true);
            pimageUpSel   = pmodeler->LoadImage("scrollupselbmp"  , true);
            pimageDown    = pmodeler->LoadImage("scrolldownbmp"   , true);
            pimageDownSel = pmodeler->LoadImage("scrolldownselbmp", true);
        }

        pbuttonUp =
            CreateButton(
                new ImagePane(pimageUp),
                new ImagePane(pimageUpSel),
                false,
                0.1f,
                0.5f
            );

        pbuttonDown =
            CreateButton(
                new ImagePane(pimageDown),
                new ImagePane(pimageDownSel),
                false,
                0.1f,
                0.5f
            );
    }

    pscrollPane =
        CreateScrollPane(
            new JustifyPane(),
            ThumbPane::Create(pmodeler, bHoriztonal, pImageThumb),
            pbuttonUp,
            pbuttonUp->GetEventSource(),
            pbuttonDown,
            pbuttonDown->GetEventSource(),
            bHoriztonal, // false == vertical
            0.25f,
            1,
            1,
            1,
            1,
            size
        );

    pevent = pscrollPane->GetEventSource();

    return pscrollPane;
};

//////////////////////////////////////////////////////////////////////////////
//
// ScrollPane
//
//////////////////////////////////////////////////////////////////////////////

class ScrollPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;
    bool          m_bHorizontal;
    bool          m_bDefaultImages;

public:
    ScrollPaneFactory(Modeler* pmodeler, bool bHorizontal, bool bDefaultImages) :
        m_pmodeler(pmodeler),
        m_bHorizontal(bHorizontal),
        m_bDefaultImages(bDefaultImages)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint;      CastTo(ppoint, (IObject*)stack.Pop());

        TRef<ButtonPane> pbuttonUp;
        TRef<ButtonPane> pbuttonDown; 
        TRef<Image>      pImageThumb;

        if(m_bDefaultImages)
        {
            pbuttonUp = NULL;
            pbuttonDown = NULL;
            pImageThumb = NULL;
        }
        else
        {
            //
            // Pop buttons off stack
            //

            CastTo(pbuttonUp, (IObject*)stack.Pop());

            CastTo(pbuttonDown, (IObject*)stack.Pop());

            CastTo(pImageThumb, (Value*)(IObject*)stack.Pop());
        }

        WinPoint temp;

        temp.SetX((int)ppoint->GetValue().X());
        temp.SetY((int)ppoint->GetValue().Y());

        TRef<IIntegerEventSource> evtmp;
        TRef<ScrollPane> sptmp;
        return
            CreateTrekScrollPane(
                temp,
                m_pmodeler,
                evtmp,
                sptmp,
                m_bHorizontal,
                pbuttonUp,
                pbuttonDown,
                pImageThumb
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ListPane
//
//////////////////////////////////////////////////////////////////////////////

class ListPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ListPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint;      CastTo(ppoint, (IObject*)stack.Pop());

        return
            (Pane*)CreateListPane(
                WinPoint::Cast(ppoint->GetValue()),
                NULL,
                NULL,
                NULL,
                false
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// EditPane
//
//////////////////////////////////////////////////////////////////////////////

class EditPaneFactory : public IFunction {
private:
    TRef<Number> m_ptime;

public:
    EditPaneFactory(Number* ptime) :
        m_ptime(ptime)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint; CastTo(ppoint, (IObject*)stack.Pop());
        TRef<FontValue>  pfont;  CastTo(pfont,  (IObject*)stack.Pop());

        TRef<Pane> ppane = CreateEditPane(ZString(), pfont->GetValue(), m_ptime);
        ppane->SetSize(WinPoint::Cast(ppoint->GetValue()));

        return ppane;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// StringListPane
//
//////////////////////////////////////////////////////////////////////////////

class StringListWithScrollBarPaneFactory: public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    StringListWithScrollBarPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint;      CastTo(ppoint,      (IObject*)stack.Pop());
        TRef<ScrollPane> pscrollPane; CastTo(pscrollPane, (Pane*)(IObject*)stack.Pop());
        TRef<FontValue>  pfont;       CastTo(pfont,       (IObject*)stack.Pop());
        TRef<ColorValue> pcolor;      CastTo(pcolor,      (IObject*)stack.Pop());

        return (Pane*) 
            CreateStringListPane(
                WinPoint::Cast(ppoint->GetValue()), 
                NULL, 
                NULL, 
                pscrollPane,
                false,
                pfont->GetValue(),
                Color::White(),
                pcolor->GetValue()
            );
    }
};



//////////////////////////////////////////////////////////////////////////////
//
// ListPane
//
//////////////////////////////////////////////////////////////////////////////

class ListWithScrollBarPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ListWithScrollBarPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint;      CastTo(ppoint,      (IObject*)stack.Pop());
        TRef<ScrollPane> pscrollPane; CastTo(pscrollPane, (Pane*)(IObject*)stack.Pop());

        return 
            (Pane*)CreateListPane(
                WinPoint::Cast(ppoint->GetValue()), 
                NULL, 
                NULL, 
                pscrollPane,
                false
            );
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// ListHoriztonalPane
//
//////////////////////////////////////////////////////////////////////////////

class ListHorizontalWithScrollBarPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ListHorizontalWithScrollBarPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> ppoint;      CastTo(ppoint,      (IObject*)stack.Pop());
        TRef<ScrollPane> pscrollPane; CastTo(pscrollPane, (Pane*)(IObject*)stack.Pop());

        return 
            (Pane*)CreateListPane(
                WinPoint::Cast(ppoint->GetValue()), 
                NULL, 
                NULL, 
                pscrollPane,
                true
            );
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// ImagePaneFactory
//
//////////////////////////////////////////////////////////////////////////////

class ImagePaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ImagePaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    void AddChildren(Pane* ppaneParent, ObjectStack& stack)
    {
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());

        plist->GetFirst();

        while (plist->GetCurrent() != NULL) {
            IObjectPair*      ppair;       CastTo(ppair,       plist->GetCurrent());
            TRef<Pane>        ppaneChild;  CastTo(ppaneChild,  ppair->GetFirst()  );
            TRef<PointValue>  ppointChild; CastTo(ppointChild, ppair->GetSecond() );

            ppaneParent->InsertAtBottom(ppaneChild);
            ppaneChild->SetOffset(
                WinPoint(
                    (int)ppointChild->GetValue().X(),
                    (int)ppointChild->GetValue().Y()
                )
            );

            plist->GetNext();
        }
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Image> pimage; CastTo(pimage, (Value*)(IObject*)stack.Pop());
        TRef<Pane>  ppane = new ImagePane(pimage);

        AddChildren(ppane, stack);

        return ppane;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ScreenPane
//
//////////////////////////////////////////////////////////////////////////////

Window* g_pwindow;

void SetEffectWindow(Window* pwindow)
{
    g_pwindow = pwindow;
}

class ScreenPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ScreenPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    void AddChildren(Pane* ppaneParent, ObjectStack& stack)
    {
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());

        plist->GetFirst();

        while (plist->GetCurrent() != NULL) {
            IObjectPair*      ppair;       CastTo(ppair,       plist->GetCurrent());
            TRef<Pane>        ppaneChild;  CastTo(ppaneChild,  ppair->GetFirst()  );
            TRef<PointValue>  ppointChild; CastTo(ppointChild, ppair->GetSecond() );

            ppaneParent->InsertAtBottom(ppaneChild);
            ppaneChild->SetOffset(
                WinPoint(
                    (int)ppointChild->GetValue().X(),
                    (int)ppointChild->GetValue().Y()
                )
            );

            plist->GetNext();
        }
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Image> pimage; CastTo(pimage, (Value*)(IObject*)stack.Pop());
        TRef<Pane>  ppane = new ScreenPane(g_pwindow, pimage);

        AddChildren(ppane, stack);

        return ppane;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Hover Site
//
//////////////////////////////////////////////////////////////////////////////

class HoverSiteImpl : public HoverSite {
private:
    TRef<ModifiableNumber> m_pnumber;
    
public:
    HoverSiteImpl(float initialID) :
        m_pnumber(new ModifiableNumber(initialID))
    {
    }

    void Enter(float id) 
    {
        m_pnumber->SetValue(id);
    }

    void Leave(float id)
    {
        m_pnumber->SetValue(0);
    }

    Number* GetID()
    {
        return m_pnumber;
    }
};

TRef<HoverSite> CreateHoverSite(float initialID)
{
    return new HoverSiteImpl(initialID);
}

class HoverSiteFactory : public IFunction {
public:
    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Number> pnumber; CastTo(pnumber, (IObject*)stack.Pop());

        return CreateHoverSite(pnumber->GetValue());
    }
};

class GetHoverSiteIDFactory : public IFunction {
public:
    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<HoverSite>  psite;  CastTo(psite,  (IObject*)stack.Pop());

        return psite->GetID();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Hover Pane
//
//////////////////////////////////////////////////////////////////////////////

class HoverPane : public Pane {
private:
    TRef<HoverSite> m_psite;
    float           m_id;
    bool            m_bEntered;

public:
    HoverPane(HoverSite* psite, float id, Pane* ppane) :
        Pane(ppane),
        m_psite(psite),
        m_id(id),
        m_bEntered(false)
    {
    }

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        if (!Child()->IsHidden()) {
            m_bEntered = true;
            m_psite->Enter(m_id);
        }
    }

    void MouseLeave(IInputProvider* pprovider)
    {
        if (m_bEntered) {
            m_bEntered = false;
            m_psite->Leave(m_id);
        }
    }
};

TRef<Pane> CreateHoverPane(HoverSite* psite, float id, Pane* ppane)
{
    return new HoverPane(psite, id, ppane);
}

class HoverPaneFactory : public IFunction {
public:
    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<HoverSite>  psite;  CastTo(psite,  (IObject*)stack.Pop());
        TRef<Number>     pid;    CastTo(pid,    (IObject*)stack.Pop());
        TRef<Pane>       ppane;  CastTo(ppane,  (IObject*)stack.Pop());

        return
            (Pane*)CreateHoverPane(
                psite,
                pid->GetValue(),
                ppane
            );
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// Hover Pane for Button Bar Panes and Lists
//
//////////////////////////////////////////////////////////////////////////////

class HoverPaneColumns : public Pane {
private:
    TRef<HoverSite> m_psite;
    float           m_id;
	TVector<int>	m_vecColumns;
	float			m_startid;

public:
    HoverPaneColumns(HoverSite* psite, float id, Pane* ppane, TVector<int>& pvecColumns) :
        Pane(ppane),
        m_psite(psite),
        m_id(id),
		m_startid(id),
		m_vecColumns(pvecColumns)
	
	
    {
    }

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        m_psite->Enter(m_id);
	}

    void MouseLeave(IInputProvider* pprovider)
    {
        m_psite->Leave(m_id);
    }

	void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
	{
		int count = m_vecColumns.GetCount();
		int xprev = 0;
			
		for (int index = 0; index < count; index++) {
			int x = m_vecColumns[index];
			if ((int)point.X() < x) {
				if ((int)point.X() > xprev) {
					m_id = (float)(m_startid + index);
					m_psite->Enter(m_id);
				}
			} else {
				m_id = 0;
			}

    		xprev = x;			
		}
	}
};

TRef<Pane> CreateHoverPaneColumns(HoverSite* psite, float id, Pane* ppane, TVector<int>& pvecColumns)
{
    return new HoverPaneColumns(psite, id, ppane, pvecColumns);
}

class HoverPaneColumnsFactory : public IFunction {
public:
    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<HoverSite>		psite;				CastTo(psite,			(IObject*)stack.Pop());
        TRef<Number>		pid;				CastTo(pid,				(IObject*)stack.Pop());
        TRef<Pane>          ppane;				CastTo(ppane,			(IObject*)stack.Pop());
		TVector<int>		pvecColumns;
		ParseIntVector((IObject*)stack.Pop(), pvecColumns);
		

        return
            (Pane*)CreateHoverPaneColumns(
                psite,
                pid->GetValue(),
                ppane, 
				pvecColumns
            );
    }
};



//////////////////////////////////////////////////////////////////////////////
//
// ParentPane
//
//////////////////////////////////////////////////////////////////////////////

class ParentPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ParentPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    void AddChildren(Pane* ppaneParent, ObjectStack& stack)
    {
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());

        plist->GetFirst();

        while (plist->GetCurrent() != NULL) {
            IObjectPair*      ppair;       CastTo(ppair,       plist->GetCurrent());
            TRef<Pane>        ppaneChild;  CastTo(ppaneChild,  ppair->GetFirst()  );
            TRef<PointValue>  ppointChild; CastTo(ppointChild, ppair->GetSecond() );

            ppaneParent->InsertAtBottom(ppaneChild);
            ppaneChild->SetOffset(
                WinPoint(
                    (int)ppointChild->GetValue().X(),
                    (int)ppointChild->GetValue().Y()
                )
            );

            plist->GetNext();
        }
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Pane>  ppane; CastTo(ppane, (Value*)(IObject*)stack.Pop());

        AddChildren(ppane, stack);

        return ppane;
    }
};


//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ButtonEffectPaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    ButtonEffectPaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Image>        pimage;       CastTo(pimage, (Value*)(IObject*)stack.Pop());
        TRef<Number>       pnumberFaces; CastTo(pnumberFaces, (IObject*)stack.Pop());
        TRef<Boolean>      pboolToggle;  CastTo(pboolToggle,  (IObject*)stack.Pop());

        DWORD dwFaces = (DWORD)pnumberFaces->GetValue();

        return
            CreateButton(
                CreateButtonFacePane(
                    pimage->GetSurface(),
                    dwFaces
                ),
                pboolToggle->GetValue(),
                0,
                0
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Pane> CreateBlackPane(Pane* ppane)
{
    return new BorderPane(0, Color::Black(), ppane);
}

class BlackPaneFactory : public IFunction {
public:
    BlackPaneFactory()
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<Pane> ppane; CastTo(ppane, (Value*)(IObject*)stack.Pop());

        return CreateBlackPane(ppane);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

void AddPaneFactories(
    INameSpace*      pns,
    Modeler*         pmodeler,
    IPopupContainer* ppopupContainer,
    Number*          ptime
) {
    pns->AddMember("ButtonFaceUp",                    new Number((float)ButtonFaceUp));
    pns->AddMember("ButtonFaceDown",                  new Number((float)ButtonFaceDown));
    pns->AddMember("ButtonNormal",                    new Number((float)ButtonNormal));
    pns->AddMember("ButtonNormalCheckBox",            new Number((float)ButtonNormalCheckBox));
    pns->AddMember("Button3State",                    new Number((float)Button3State));
    pns->AddMember("ButtonUpDown",                    new Number((float)(ButtonFaceUp | ButtonFaceDown)));
    pns->AddMember("ButtonEffectPane",                new ButtonEffectPaneFactory(pmodeler));

    pns->AddMember("BlackPane",                       new BlackPaneFactory());
    pns->AddMember("StringListWithScrollBarPane",     new StringListWithScrollBarPaneFactory(pmodeler));
    pns->AddMember("EditPane",                        new EditPaneFactory(ptime));
    pns->AddMember("ScrollBarPane",                   new ScrollPaneFactory(pmodeler, false, true));
    pns->AddMember("ScrollBarHorizontalPane",         new ScrollPaneFactory(pmodeler, true, true));
    pns->AddMember("CustomScrollBarPane",             new ScrollPaneFactory(pmodeler, false, false));
    pns->AddMember("CustomScrollBarHorizontalPane",   new ScrollPaneFactory(pmodeler, true, false));
    pns->AddMember("ListPane",                        new ListPaneFactory(pmodeler));
    pns->AddMember("ListWithScrollBarPane",           new ListWithScrollBarPaneFactory(pmodeler));
    pns->AddMember("ListHorizontalWithScrollBarPane", new ListHorizontalWithScrollBarPaneFactory(pmodeler));
    pns->AddMember("ImagePane",                       new ImagePaneFactory(pmodeler));
    pns->AddMember("ScreenPane",                      new ScreenPaneFactory(pmodeler));
    pns->AddMember("HoverSite",                       new HoverSiteFactory());
    pns->AddMember("HoverPane",                       new HoverPaneFactory());
	pns->AddMember("HoverPaneColumns",                new HoverPaneColumnsFactory());
    pns->AddMember("HoverSiteGetID",                  new GetHoverSiteIDFactory());

    pns->AddMember("ParentPane",                      new ParentPaneFactory(pmodeler));
}
