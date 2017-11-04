#include "menu.h"

#include "controls.h"
#include "enginep.h"
#include "pane.h"
#include "surface.h"

/////////////////////////////////////////////////////////////////////////////
//
// SubmenuEventSinkDelegate
//
/////////////////////////////////////////////////////////////////////////////

class SubmenuEventSinkDelegate : public ISubmenuEventSink {
private:
    ISubmenuEventSink* m_psink;

public:
    SubmenuEventSinkDelegate(ISubmenuEventSink* psink) :
        m_psink(psink)
    {
    }

    TRef<IPopup> GetSubMenu(IMenuItem* pitem)
    {
        return m_psink->GetSubMenu(pitem);
    }
};

TRef<ISubmenuEventSink> ISubmenuEventSink::CreateDelegate(ISubmenuEventSink* psink)
{
    return new SubmenuEventSinkDelegate(psink);
}

/////////////////////////////////////////////////////////////////////////////
//
// MenuCommandSinkDelegate
//
/////////////////////////////////////////////////////////////////////////////

class MenuCommandSinkDelegate : public IMenuCommandSink {
private:
    IMenuCommandSink* m_psink;

public:
    MenuCommandSinkDelegate(IMenuCommandSink* psink) :
        m_psink(psink)
    {
    }

    void OnMenuCommand(IMenuItem* pitem)
    {
        m_psink->OnMenuCommand(pitem);
    }

    void OnMenuSelect(IMenuItem* pitem)
    {
        m_psink->OnMenuSelect(pitem);
    }

    void OnMenuClose(IMenu* pmenu)
    {
        m_psink->OnMenuClose(pmenu);
    }
};

TRef<IMenuCommandSink> IMenuCommandSink::CreateDelegate(IMenuCommandSink* psink)
{
    return new MenuCommandSinkDelegate(psink);
}

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

class IMenuItemPrivate : public IMenuItem {
public:
    virtual TRef<IPopup> GetPopup(Rect& rect) = 0;
    virtual void         Select(bool bSelect) = 0;
    virtual TRef<Pane>   GetPane()            = 0;
    virtual char         GetAccelerator()     = 0;
};

class IMenuPrivate : public IMenu {
public:
    virtual void MouseEnter(IMenuItemPrivate* pitem) = 0;
    virtual void MouseLeave(IMenuItemPrivate* pitem) = 0;
    virtual void MouseClick(IMenuItemPrivate* pitem) = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

class PopupSurfaceSite : public SurfaceSite {
public:
    void UpdateSurface(Surface* psurface) 
    {
        const Color color(74.0f / 255.0f, 124.0f / 255.0f, 88.0f / 255.0f);

        PrivateSurface * psurfaceSource = (PrivateSurface*)psurface;
		if( psurfaceSource->GetPixelFormat()->AlphaMask() != 0 )
		{
			psurfaceSource->SetColorKey( Color(0.0f, 0.0f, 0.0f ) );
		}

//        psurface->FillSurface(Color::Black());
		PixelFormat * pixelFmt = psurfaceSource->GetEngine()->GetPrimaryPixelFormat();
		psurface->FillSurface( Pixel::Create( 0 ) );
        psurface->FillRect(WinRect(1, 1, 2, 8), color);
        psurface->FillRect(WinRect(2, 2, 3, 7), color);
        psurface->FillRect(WinRect(3, 2, 4, 7), color);
        psurface->FillRect(WinRect(4, 3, 5, 6), color);
        psurface->FillRect(WinRect(5, 3, 6, 6), color);
        psurface->FillRect(WinRect(6, 4, 7, 5), color);
    }
};

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

class IMenuItemImpl : public IMenuItemPrivate {
private:
    /////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    /////////////////////////////////////////////////////////////////////////////

    class MenuItemPane : public Pane {
    private:
        IMenuPrivate*     m_pmenu;
        IMenuItemPrivate* m_pitem;

    public:
        MenuItemPane(
            IMenuPrivate*      pmenu,
            IMenuItemPrivate*  pitem,
            Pane*              ppaneChild
        ) :
            m_pmenu(pmenu),
            m_pitem(pitem)
        {
            InsertAtBottom(ppaneChild);
        }

        void UpdateLayout()
        {
            DefaultUpdateLayout();
        }

        void MouseEnter(IInputProvider* pprovider, const Point& point)
        {
            m_pmenu->MouseEnter(m_pitem);
        }

        void MouseLeave(IInputProvider* pprovider, const Point& point)
        {
            m_pmenu->MouseLeave(m_pitem);
        }

        MouseResult Button(
            IInputProvider* pprovider,
            const Point& point,
            int button,
            bool bCaptured,
            bool bInside,
            bool bDown
        ) {
            if (button == 0 && !bDown) {
                m_pmenu->MouseClick(m_pitem);
            }

            return MouseResult();
        }
    };

    /////////////////////////////////////////////////////////////////////////////
    //
    // data members
    //
    /////////////////////////////////////////////////////////////////////////////

    IMenuPrivate*           m_pmenu;
    TRef<ISubmenuEventSink> m_psubmenuEventSink;
    int                     m_id;

    TRef<MenuItemPane>      m_ppane;
    TRef<StringPane>        m_pstringPane;
    TRef<StringPane>        m_pstringPaneAccelerator;
    TRef<BorderPane>        m_pborderPane;
    Color                   m_colorBack;
    Color                   m_colorText;
    Color                   m_colorSelectedBack;
    Color                   m_colorSelectedText;
    bool                    m_bSelected;
    bool                    m_bEnabled;
    char                    m_chAccelerator;

public:
    IMenuItemImpl(
        IMenuPrivate* pmenu,
        Modeler* pmodeler,
        ISubmenuEventSink* psubmenuEventSink,
        char chAccelerator,
        int id,
        const ZString& str,
        IEngineFont* pfont
    ) :
        m_pmenu(pmenu),
        m_psubmenuEventSink(psubmenuEventSink),
        m_id(id),
        m_colorBack(         33 / 255.0f,  20 / 255.0f,  30 / 255.0f),
        m_colorText(        255 / 255.0f, 255 / 255.0f, 255 / 255.0f),
        m_colorSelectedBack(197 / 255.0f,   0 / 255.0f,  41 / 255.0f),
        m_colorSelectedText(255 / 255.0f, 255 / 255.0f, 255 / 255.0f),
        m_bSelected(false),
        m_bEnabled(true)
    {
        TRef<Pane> ppaneLeft;
        TRef<Pane> ppaneRight;

        ppaneLeft = new Pane();
        ppaneLeft->SetSize(WinPoint(11,11));

        TRef<Surface> psurfacePopup = 
            pmodeler->GetEngine()->CreateSurface(
                WinPoint(9, 9),
                SurfaceType2D() | SurfaceTypeColorKey(),
                new PopupSurfaceSite()
            );

        psurfacePopup->SetColorKey(Color(0, 0, 0));

        if (m_psubmenuEventSink) {
            ppaneRight = new SurfacePane(psurfacePopup);
        } else {
            ppaneRight = new Pane();
            ppaneRight->SetSize(psurfacePopup->GetSize());
        }
                      
        m_pstringPane            = new StringPane(str, pfont);
        m_pstringPaneAccelerator = new StringPane(ZString(), pfont);

        TRef<Pane> ppaneSpace = new Pane();
        ppaneSpace->SetSize(WinPoint(2, 0));

        TRef<RowPane> pRow = new RowPane();
        pRow->InsertAtBottom(ppaneLeft);
        pRow->InsertAtBottom(m_pstringPane);
        pRow->InsertAtBottom(ppaneSpace);
        pRow->InsertAtBottom(new JustifyPane(JustifyPane::Right, WinPoint(0, 0), m_pstringPaneAccelerator));
        pRow->InsertAtBottom(ppaneRight);

        m_pborderPane = new BorderPane(1, m_colorBack, pRow);

        m_ppane =
            new MenuItemPane(
                m_pmenu,
                this,
                m_pborderPane
            );

        UpdateColors();

        SetAccelerator(chAccelerator);
    }

    void UpdateColors()
    {
        if (m_bSelected) {
            m_pstringPane           ->SetTextColor(m_colorSelectedText);
            m_pstringPaneAccelerator->SetTextColor(m_colorSelectedText);
            m_pborderPane           ->SetColor(m_colorSelectedBack);
        } else {
            m_pstringPane           ->SetTextColor(m_colorText);
            m_pstringPaneAccelerator->SetTextColor(m_colorText);
            m_pborderPane           ->SetColor(m_colorBack);
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IMenuItemPrivate methods
    //
    /////////////////////////////////////////////////////////////////////////////

    TRef<IPopup> GetPopup(Rect& rect)
    {
        if (m_psubmenuEventSink) {
            Point point = m_ppane->TransformLocalToImage(WinPoint(0, 0));

            rect =
                Rect(
                    point.X(),
                    point.Y() - (float)m_ppane->YSize(),
                    point.X() + (float)m_ppane->XSize(),
                    point.Y()
                );

            return m_psubmenuEventSink->GetSubMenu(this);
        }

        return NULL;
    }

    void Select(bool bSelect)
    {
        m_bSelected = bSelect;
        UpdateColors();
    }

    TRef<Pane> GetPane()
    {
        return m_ppane;
    }

    char GetAccelerator()
    {
        return m_chAccelerator;
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IMenuItem methods
    //
    /////////////////////////////////////////////////////////////////////////////

    int GetID()
    {
        return m_id;
    }

    const ZString& GetString()
    {
        return m_pstringPane->GetString();
    }

    void SetString(const ZString& str)
    {
        m_pstringPane->SetString(str);
    }

    void SetColors(
        const Color& colorBack,
        const Color& colorText,
        const Color& colorSelectedBack,
        const Color& colorSelectedText
    ) {
        m_colorBack         = colorBack        ;
        m_colorText         = colorText        ;
        m_colorSelectedBack = colorSelectedBack;
        m_colorSelectedText = colorSelectedText;

        UpdateColors();
    }

    Color GetColor()
    {
        return m_colorBack;
    }

    void SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
    }

    bool IsEnabled()
    {
        return m_bEnabled;
    }

    void SetAccelerator(char ch)
    {
        if (ch >= 'a' && ch <= 'z') {
            ch += 'A' - 'a';
        }
        m_chAccelerator = ch;

        char chShow = ch;
        if (chShow == 0) {
            chShow = ' ';
        }

        m_pstringPaneAccelerator->SetString(ZString(chShow, 1));
    }
};

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////

class IMenuImpl : public IMenuPrivate {
private:
    /////////////////////////////////////////////////////////////////////////////
    //
    // types
    //
    /////////////////////////////////////////////////////////////////////////////

    typedef TList<TRef<IMenuItemPrivate> > MenuItemList;

    /////////////////////////////////////////////////////////////////////////////
    //
    // data members
    //
    /////////////////////////////////////////////////////////////////////////////

    TRef<IPopup>            m_ppopup;

    MenuItemList            m_listItems;
    TRef<IMenuItemPrivate>  m_pitemSelection;
    TRef<IMenuCommandSink>  m_psink;

    TRef<Modeler>           m_pmodeler;
    TRef<ColumnPane>        m_pcolumnPane;
    TRef<Pane>              m_ppane;
    TRef<IEngineFont>       m_pfont;

public:
    IMenuImpl(
        Modeler* pmodeler,
        IEngineFont* pfont,
        IMenuCommandSink* psink
    ) :
        m_pmodeler(pmodeler),
        m_pfont(pfont),
        m_psink(psink)
    {
        m_ppane =
            new EdgePane(
                m_pcolumnPane = new ColumnPane(),
                true
            );
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void NextItem()
    {
        if (m_pitemSelection == NULL) {
            SelectItem(m_listItems.GetFront());
        } else {
            MenuItemList::Iterator iter(m_listItems);

            while (!iter.End()) {
                if (iter.Value() == m_pitemSelection) {
                    iter.Next();
                    if (!iter.End()) {
                        SelectItem(iter.Value());
                        return;
                    } else {
                        SelectItem(m_listItems.GetFront());
                    }
                }
                iter.Next();
            }
        }
    }

    void PreviousItem()
    {
        if (m_pitemSelection == NULL) {
            SelectItem(m_listItems.GetEnd());
        } else {
            MenuItemList::Iterator iter(m_listItems);

            while (!iter.End()) {
                if (iter.Value() == m_pitemSelection) {
                    iter.Prev();
                    if (!iter.End()) {
                        SelectItem(iter.Value());
                        return;
                    } else {
                        SelectItem(m_listItems.GetEnd());
                    }
                }
                iter.Next();
            }
        }
    }

    bool OpenItem(IMenuItemPrivate* pitem)
    {
        if (pitem && m_ppopup == NULL) {
            if (m_ppopup) {
                m_pcontainer->ClosePopup(m_ppopup);
            }

            Rect rect;
            m_ppopup = pitem->GetPopup(rect);

            if (m_ppopup) {
                m_pcontainer->OpenPopup(m_ppopup, rect, true, true, this);
                return true;
            }
        }

        return false;
    }

    void SelectItem(IMenuItemPrivate* pitem)
    {
        if (pitem != m_pitemSelection) {
            if (m_ppopup) {
                m_pcontainer->ClosePopup(m_ppopup);
                m_ppopup = NULL;
            }
            if (m_pitemSelection) {
                m_pitemSelection->Select(false);
            }
            m_pitemSelection = pitem;
            if (m_pitemSelection) {
                m_pitemSelection->Select(true);
            }
            if (m_psink) {
                m_psink->OnMenuSelect(m_pitemSelection);
            }
        }
    }

    void ItemCommand(IMenuItemPrivate* pitem)
    {
        if (pitem && pitem->IsEnabled()) {
            SelectItem(pitem);

            if (!OpenItem(pitem)) {
                if (m_psink) {
                    m_psink->OnMenuCommand(pitem);
                }
            }
        }
    }

    bool HandleAccelerator(char ch) 
    {
        MenuItemList::Iterator iter(m_listItems);

        while (!iter.End()) {
            if (iter.Value()->GetAccelerator() == ch) {
                ItemCommand(iter.Value());
                return true;
            }
            iter.Next();
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IMenuPrivate methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void MouseEnter(IMenuItemPrivate* pitem)
    {
        SelectItem(pitem);
        OpenItem(m_pitemSelection);
    }

    void MouseLeave(IMenuItemPrivate* pitem)
    {
    }

    void MouseClick(IMenuItemPrivate* pitem)
    {
        ItemCommand(m_pitemSelection);
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IMenu methods
    //
    /////////////////////////////////////////////////////////////////////////////

    TRef<IMenuItem> AddMenuItem(
        int                id,
        const ZString&     str,
        char               chAccelerator     = 0,
        ISubmenuEventSink* psubmenuEventSink = NULL
    ) {
        TRef<IMenuItemPrivate> pitem =
            new IMenuItemImpl(
                this,
                m_pmodeler,
                psubmenuEventSink,
                chAccelerator,
                id,
                str,
                m_pfont
            );

        m_listItems.PushEnd(pitem);
        m_pcolumnPane->InsertAtBottom(pitem->GetPane());

        if (m_pitemSelection == NULL) {
            m_pitemSelection = pitem;
            pitem->Select(true);
        }

        return pitem;
    }


    IMenuItem* FindMenuItem(int id)
    {
        MenuItemList::Iterator iter(m_listItems);

        while (!iter.End()) {
            if (iter.Value()->GetID() == id) {
                return iter.Value();
            }
            iter.Next();
        }

        // not found
        return NULL;
    }

    void SetMenuCommandSink(IMenuCommandSink* psink)
    {
        m_psink = psink;
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IPopup methods
    //
    /////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }

    void ClosePopup(IPopup* ppopup)
    {
        ZAssert(m_ppopup == ppopup);
        m_pcontainer->ClosePopup(ppopup);
        m_ppopup = NULL;
    }

    void OnClose()
    {
        if (m_psink) {
            m_psink->OnMenuClose(this);
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // IKeyboardInput methods
    //
    /////////////////////////////////////////////////////////////////////////////

    bool OnChar(IInputProvider* pprovider, const KeyState& ks)
    {
        return false;
    }

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown) {
            switch (ks.vk) {
                case VK_DOWN:
                    NextItem();
                    return true;

                case VK_UP:
                    PreviousItem();
                    return true;

                case VK_RIGHT:
                    OpenItem(m_pitemSelection);
                    return true;

                case VK_LEFT:
                case VK_ESCAPE:
                    if (m_ppopupOwner) {
                        m_ppopupOwner->ClosePopup(this);
                    } else {
                        m_pcontainer->ClosePopup(this);
                    }
                    return true;

                case 13: // enter
                    ItemCommand(m_pitemSelection);
                    return true;

                default:
                    return HandleAccelerator(ks.vk);
            }
        }

        return false;
    }
};

TRef<IMenu> CreateMenu(Modeler* pmodeler, IEngineFont* pfont, IMenuCommandSink* psink)
{
    return new IMenuImpl(pmodeler, pfont, psink);
}
