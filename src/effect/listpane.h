#ifndef _listpane_h_
#define _listpane_h_

#include <event.h>
#include <genericlist.h>
#include <pane.h>

typedef TEvent<ItemID> IItemEvent;

//////////////////////////////////////////////////////////////////////////////
//
// Item Painter
//
//////////////////////////////////////////////////////////////////////////////

class ItemPainter : public IObject {
public:
    virtual int  GetXSize()                                                          = 0;
    virtual int  GetYSize()                                                          = 0;
    virtual void Paint(ItemID pitem, Surface* psurface, bool bSelected, bool bFocus) = 0;

	// Generate a tristrip for rendering the image in pSurface to the screen.
	// Note: pntImage is in screen coords, not pane coords.
/*	void RenderImage( WinPoint pntImage, Surface * pSurface )
	{
		UIVERTEX pVerts[4];
        PrivateSurface* psurfaceTexture; CastTo(psurfaceTexture, pSurface);

		if( psurfaceTexture->GetTexHandle() == INVALID_TEX_HANDLE )
		{
			return;
		}
		DWORD dwOriginalWidth, dwOriginalHeight, dwActualWidth, dwActualHeight;
		CVRAMManager::GetOriginalDimensions( psurfaceTexture->GetTexHandle(), &dwOriginalWidth, &dwOriginalHeight );
		CVRAMManager::GetActualDimensions( psurfaceTexture->GetTexHandle(), &dwActualWidth, &dwActualHeight );
		dwActualWidth --;
		dwActualHeight --;
		pVerts[0].x = (float) pntImage.X();
		pVerts[0].y = (float) pntImage.Y();
		pVerts[0].z = 0.5f;
		pVerts[0].rhw = 1.0f;
		pVerts[0].fU = 0.0f;
		pVerts[0].fV = 0.0f;

		pVerts[1].x = (float) pntImage.X() + dwOriginalWidth;
		pVerts[1].y = (float) pntImage.Y();
		pVerts[1].z = 0.5f;
		pVerts[1].rhw = 1.0f;
		pVerts[1].fU = 1.0f; //(float) dwOriginalWidth / (float) dwActualWidth;
		pVerts[1].fV = 0.0f;

		pVerts[2].x = (float) pntImage.X();
		pVerts[2].y = (float) pntImage.Y() + dwOriginalHeight;
		pVerts[2].z = 0.5f;
		pVerts[2].rhw = 1.0f;
		pVerts[2].fU = 0.0f;
		pVerts[2].fV = 1.0f; //(float) dwOriginalHeight / (float) dwActualHeight;

		pVerts[3].x = (float) pntImage.X() + dwOriginalWidth;
		pVerts[3].y = (float) pntImage.Y() + dwOriginalHeight;
		pVerts[3].z = 0.5f;
		pVerts[3].rhw = 1.0f;
		pVerts[3].fU = 1.0f; //(float) dwOriginalWidth / (float) dwActualWidth;
		pVerts[3].fV = 1.0f; //(float) dwOriginalHeight / (float) dwActualHeight;

		if( psurfaceTexture->HasColorKey() == true )
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
		// Render this pane.
		CD3DDevice9::SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UIVERTEX );
		hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
		hr = CVRAMManager::SetTexture( psurfaceTexture->GetTexHandle(), 0 );
		hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLESTRIP,
											2,
											pVerts,
											sizeof( UIVERTEX ) );
	}

	void FillRect( const WinRect rectFill, const Color fillColor )
	{
		UICOLOURFILLVERTEX pVerts[4];
		D3DCOLOR d3dColour =	( MakeInt(255.0f * fillColor.GetAlpha()) << 24)
								| (MakeInt(255.0f * fillColor.GetRed()  ) << 16)
								| (MakeInt(255.0f * fillColor.GetGreen()) <<  8)
								| (MakeInt(255.0f * fillColor.GetBlue() ) );
 

		pVerts[0].x = (float) rectFill.XMin();
		pVerts[0].y = (float) rectFill.YMin();
		pVerts[0].z = 0.5f;
		pVerts[0].rhw = 1.0f;
		pVerts[0].color = d3dColour;

		pVerts[1].x = (float) rectFill.XMax();
		pVerts[1].y = (float) rectFill.YMin();
		pVerts[1].z = 0.5f;
		pVerts[1].rhw = 1.0f;
		pVerts[1].color = d3dColour;

		pVerts[2].x = (float) rectFill.XMin();
		pVerts[2].y = (float) rectFill.YMax();
		pVerts[2].z = 0.5f;
		pVerts[2].rhw = 1.0f;
		pVerts[2].color = d3dColour;

		pVerts[3].x = (float) rectFill.XMax();
		pVerts[3].y = (float) rectFill.YMax();
		pVerts[3].z = 0.5f;
		pVerts[3].rhw = 1.0f;
		pVerts[3].color = d3dColour;

		CD3DDevice9::SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		// Render this pane.
		CD3DDevice9::SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		HRESULT hr = CD3DDevice9::SetFVF( D3DFVF_UICOLOURVERT );
		hr = CD3DDevice9::SetStreamSource( 0, NULL, 0, 0 );
		hr = CVRAMManager::SetTexture( INVALID_TEX_HANDLE, 0 );
		hr = CD3DDevice9::DrawPrimitiveUP(	D3DPT_TRIANGLESTRIP,
														2,
														pVerts,
														sizeof( UICOLOURFILLVERTEX ) );
	}*/
};

//////////////////////////////////////////////////////////////////////////////
//
// List Pane
//
//////////////////////////////////////////////////////////////////////////////

class ScrollPane;
class ListPane : 
    public Pane,
    public IKeyboardInput
{
public:
    virtual void   SetList(List* plist)                  = 0;
    virtual void   SetItemPainter(ItemPainter* ppainter) = 0;
    virtual void   SetSelection(ItemID pitem)            = 0;
    virtual ItemID GetSelection()                        = 0;
    virtual int    GetScrollPos()                        = 0;
    virtual void   ScrollToItem(ItemID pitem)            = 0;
    virtual void   ForceRefresh()                        = 0;
    virtual void   SetScrollPos(int pos)                 = 0;
    virtual ScrollPane * GetScrollPane()                 = 0;

    virtual IItemEvent::Source* GetSelectionEventSource()     = 0;
    virtual IEventSource*       GetSingleClickEventSource()   = 0;
	virtual IEventSource*       GetSingleRightClickEventSource()   = 0;
    virtual IEventSource*       GetDoubleClickEventSource()   = 0;
	virtual IEventSource*       GetDoubleRightClickEventSource()   = 0;
};

TRef<ListPane> CreateListPane(
    const WinPoint& sizeMin,
    List*           plist,
    ItemPainter*    ppainter,
    ScrollPane*     pscroll,
    bool            bHorizontal
);

//////////////////////////////////////////////////////////////////////////////
//
// StringList
//
//////////////////////////////////////////////////////////////////////////////

class StringList : 
    public List,
    public ItemPainter
{
public:
    virtual void AddItem(const ZString& str) = 0;
    virtual void SetEmpty()                  = 0;
};

class IEngineFont;
TRef<StringList> CreateStringList(
    IEngineFont* pfont,
    const Color& color,
    const Color& colorSelected,
    int          xsize
);

//////////////////////////////////////////////////////////////////////////////
//
// StringListItem
//
//////////////////////////////////////////////////////////////////////////////

class StringListItem : public IObject {
private:
    TRef<StringListItem> m_pnext;
    ZString          m_str;

public:
    StringListItem(const ZString& str, StringListItem* pnext) :
        m_str(str),
        m_pnext(pnext)
    {
    }

    StringListItem* GetNext()
    {
        return m_pnext;
    }

    const ZString& GetString()
    {
        return m_str;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// StringListPane
//
//////////////////////////////////////////////////////////////////////////////

class StringListPane : 
    public ListPane
{
public:
    virtual TRef<StringList> GetStringList() = 0;
};


TRef<StringListPane> CreateStringListPane(
    const WinPoint& sizeMin,
    List*           plist,
    ItemPainter*    ppainter,
    ScrollPane*     pscroll,
    bool            bHorizontal,
    IEngineFont*    pfont,
    const Color&    color,
    const Color&    colorSelected
);

#endif
