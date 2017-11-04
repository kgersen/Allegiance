#include "help.h"

#include <base.h>
#include <controls.h>
#include <namespace.h>
#include <token.h>

//////////////////////////////////////////////////////////////////////////////
//
// Link Pane
//
//////////////////////////////////////////////////////////////////////////////

class TabWordPane : public Pane {
private:
public:
    TabWordPane(int x) : 
        Pane(NULL, WinPoint(0, 0))
    {
    }

    int GetAlignedXSize(int xPos)
    {
        int tabs = (xPos / 16) + 1;
        int size = tabs * 16 - xPos;

        return size;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Link Pane
//
//////////////////////////////////////////////////////////////////////////////

class LinkPane : public StringPane {
private:
    TRef<StringEventSourceImpl> m_peventSource;
    ZString m_strTopic;
    Color   m_color;
    Color   m_colorSelected;
    bool    m_bInside;

public:
    LinkPane(
        StringEventSourceImpl* peventSource,
        const ZString&         str,
        const ZString&         strTopic,
        IEngineFont*           pfont,
        const Color&           color,
        const Color&           colorSelected
    ) :
        StringPane(str, pfont),
        m_peventSource(peventSource),
        m_strTopic(strTopic),
        m_color(color),
        m_colorSelected(colorSelected),
        m_bInside(false)
    {
        SetTextColor(m_color);
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    { 
        m_bInside = true;
        SetTextColor(m_colorSelected);
    }

    void MouseLeave(IInputProvider* pprovider)
    { 
        m_bInside = false;
        SetTextColor(m_color);
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    { 
        if (button == 0 && bDown) {
            m_peventSource->Trigger(m_strTopic);
        }
	    return MouseResult(); 
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Hyperlink Pane
//
//////////////////////////////////////////////////////////////////////////////

class HyperlinkPane : public StringPane {
private:
    ZString m_strName; //TRef<StringEventSourceImpl> m_peventSource;
	ZString m_strURL;
    ZString m_strTopic;
    Color   m_color;
    Color   m_colorSelected;
    bool    m_bInside;

public:
    HyperlinkPane(
        const ZString&         szName, //StringEventSourceImpl* peventSource,
        const ZString&		   szURL,
        IEngineFont*           pfont,
        const Color&           color,
        const Color&           colorSelected
    ) :
        StringPane(ZString(szName), pfont),
		m_strURL(szURL),
        m_strName(szName), //m_peventSource(peventSource),
        m_color(color),
        m_colorSelected(colorSelected),
        m_bInside(false)
    {
        SetTextColor(m_color);
    }

	void ShowWebPage(const char* szURL)
    {
        ShellExecute(NULL, NULL, szURL, NULL, NULL, SW_SHOWNORMAL);
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    { 
        m_bInside = true;
		if (m_strName!="INVALID URL ") {
			SetTextColor(m_colorSelected);
		}
    }

    void MouseLeave(IInputProvider* pprovider)
    { 
        m_bInside = false;
        SetTextColor(m_color);
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    { 
        if (button == 0 && bDown) {
			if (m_strName!="INVALID URL ") {
				ShowWebPage(m_strURL);
			}
        }
	    return MouseResult(); 
    }
};
//////////////////////////////////////////////////////////////////////////////
//
// Page Pane
//
//////////////////////////////////////////////////////////////////////////////

class PagePaneImpl : public PagePane {
private:
    TRef<Modeler>          m_pmodeler;
    TRef<PagePaneIncluder> m_ppagePaneIncluder;
    TRef<IEngineFont>      m_pfontDefault;
    Color                  m_colorDefault;
    TRef<IEngineFont>      m_pfont;
    Color                  m_color;
    Color                  m_colorMain;
    Color                  m_colorSecondary;
    Color                  m_colorHighlight;
    int                    m_width;

    TRef<StringEventSourceImpl> m_peventSourceMain;
    TRef<StringEventSourceImpl> m_peventSourceSecondary;

public:
    PagePaneImpl(
        Modeler* pmodeler, 
        int width,
        StringEventSourceImpl* peventSourceMain,
        StringEventSourceImpl* peventSourceSecondary
    ) :
        m_pmodeler(pmodeler),
        m_width(width)
    {
        if (peventSourceMain) {
            m_peventSourceMain = peventSourceMain;
        } else {
            m_peventSourceMain = new StringEventSourceImpl();
        }

        if (peventSourceSecondary) {
            m_peventSourceSecondary = peventSourceSecondary;
        } else {
            m_peventSourceSecondary = new StringEventSourceImpl();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation
    //
    //////////////////////////////////////////////////////////////////////////////

    void InsertWord(PCC pcc, PCC pccEnd)
    {
        TRef<StringPane> pstringPane = 
            new StringPane(
                ZString(pcc, (int)(pccEnd - pcc)) + " ",   //imago 6/20/09
                m_pfont
            );
        pstringPane->SetTextColor(m_color);

        InsertAtBottom(pstringPane);
    }

    void InsertString(const ZString& str)
    {
        TRef<StringPane> pstringPane = 
            new StringPane(
                str + " ",
                m_pfont
            );
        pstringPane->SetTextColor(m_color);

        InsertAtBottom(pstringPane);
    }

    void InsertTab()
    {
        InsertAtBottom(new TabWordPane(1));
    }

    bool InsertHyperlink(PCC& pcc, PCC pccEnd, const Color& color, StringEventSourceImpl* peventSource)
    {
		//
        // Name
        //

        ZString strName;
        if (!ParseSymbol(pcc, pccEnd, '|', strName)) {
            return false;
        }
        //
        // URL
        //

        ZString strURL;
        if (!ParseSymbol(pcc, pccEnd, '>', strURL)) {
            return false;
        }

        //
        // The HyperlinkPane
        //
        if (strURL.Left(7) != "http://") {
				strName="INVALID URL";
			}

        TRef<HyperlinkPane> plinkPane =
            new HyperlinkPane(
                strName + " ", //peventSource,
                strURL,
                m_pfont,
                color,
                Color(0,0,1)
            );

        InsertAtBottom(plinkPane);

        return true;
    }

	    bool InsertLink(PCC& pcc, PCC pccEnd, const Color& color, StringEventSourceImpl* peventSource)
    {
        //
        // topic|text>
        //

        //
        // topic
        //

        ZString strTopic;
        if (!ParseSymbol(pcc, pccEnd, '|', strTopic)) {
            return false;
        }

        //
        // text
        //

        ZString strText;
        if (!ParseSymbol(pcc, pccEnd, '>', strText)) {
            return false;
        }

        //
        // The LinkPane
        //
        
        TRef<LinkPane> plinkPane =
            new LinkPane(
                peventSource,
                strText + " ",
                strTopic,
                m_pfont,
                color,
                m_colorHighlight
            );

        InsertAtBottom(plinkPane);

        return true;
    }

    bool Error(const ZString& str)
    {
        m_color = Color::Red();
        Parse(NULL, "Error: " + str);
        return false;
    }

    bool MatchBrace(PCC& pcc, PCC pccEnd)
    {
        int depth = 1;

        while (pcc < pccEnd && depth != 0) {
            if (*pcc == '<') {
                depth++;
            } else if (*pcc == '>') {
                depth--;
            }

            pcc++;
        }

        if (depth != 0) {
            return Error("Expected '>'");
        }

        return true;
    }

    bool ParseComment(PCC& pcc, PCC pccEnd)
    {
        return MatchBrace(pcc, pccEnd);
    }


    bool ParseBullet(INameSpace* pns, PCC& pcc, PCC pccEnd)
    {
        ZString strImage;
        if (!ParseSymbol(pcc, pccEnd, '|', strImage)) {
            return false;
        }

        TRef<Image> pimage = m_pmodeler->LoadImage(strImage + "bmp", true, false);
        if (pimage == NULL) {
            return Error("Invalid image");
        }

        PCC pccStart = pcc;
        if (!MatchBrace(pcc, pccEnd)) {
            return false;
        }

        TRef<PagePaneImpl> ppagePane = 
            new PagePaneImpl(
                m_pmodeler, 
                m_width - int(pimage->GetBounds().GetRect().XSize()),
                m_peventSourceMain,
                m_peventSourceSecondary
            );

        ppagePane->SetAttributes(
            m_pfontDefault,
            m_colorDefault,
            m_colorMain,
            m_colorSecondary,
            m_colorHighlight
        );
        ppagePane->m_pfont = m_pfontDefault;
        ppagePane->m_color = m_colorDefault;
        ppagePane->RemoveAllChildren();
        ppagePane->Parse(pns, pccStart, pcc - 1);

        InsertAtBottom(
            new JustifyPane(
                JustifyPane::Left | JustifyPane::Top,
                WinPoint(m_width, 0),
                new RowPane(
                    new ImagePane(pimage),
                    ppagePane
                )
            )
        );

        return true;
    }

    bool ParseSymbol(PCC& pcc, PCC pccEnd, char chEnd, ZString& str)
    {
        PCC pccSymbol = pcc;

        while (
               pcc < pccEnd
            && *pcc != chEnd
        ) {
            pcc++;
        }

        if (pcc == pccEnd || *pcc != chEnd) {
            return Error("Expected '>'");
        }

        str = ZString(pccSymbol, (int)(pcc - pccSymbol)); //imago 6/20/09
        pcc++;
        return true;
    }

    bool Include(INameSpace* pns, const ZString& str, const ZString& strFile)
    {
        if (m_ppagePaneIncluder) {
            TRef<ZFile> pfile = m_ppagePaneIncluder->Include(str);

            if (pfile == NULL) {
                pfile = m_pmodeler->LoadFile(strFile, "mml", false);
            }

            if (pfile) {
                if (pfile->GetLength() > 0) {
                    PCC pcc = (PCC)(pfile->GetPointer());
                    Parse(pns, pcc, pcc + pfile->GetLength());
                }
            }
        }

        return true;
    }

    bool ParseTag(INameSpace* pns, PCC& pcc, PCC pccEnd)
    {
        pcc++;

        if (pcc == pccEnd) {
            return Error("End of string");
        }

        //
        // << or <>
        //

        if (*pcc == '<' || *pcc == '>') {
            InsertWord(pcc, pcc+1);
            pcc++;
            return true;
        }

        //
        // <xff>
        //

        if (*pcc == 'x') {
            pcc++;
            char ch = ReadHexNumber(pcc, 2);
            InsertWord(&ch, (&ch) + 1);

            if (pcc == pccEnd || pcc[0] != '>') {
                return Error("Expected '>'");
            }
                          
            pcc++;
            return true;
        }

        //
        // <!comment>
        //

        if (*pcc == '!') {
            return ParseComment(pcc, pccEnd);
        }

        //
        // <p>
        //

        if (*pcc == 'p') {
            if (pcc == pccEnd || pcc[1] != '>') {
                return Error("Expected '>'");
            }

            InsertAtBottom(new Pane(NULL, WinPoint(0, m_pfontDefault->GetHeight())));

            pcc += 2;
            return true;
        }

        //
        // Get the tag
        //

        ZString strTag;

        if (!ParseSymbol(pcc, pccEnd, '|', strTag)) {
            return false;
        }

        //
        // <Color|color>
        // <Font|smallFont>
        // <Image|image>
        // <Main|topic|text>
        // <Secondary|topic|text>
        // <Bullet|image|text>
        // <String|identifier>
        //

        ZString str;

        if (strTag == "Color") {
            if (!ParseSymbol(pcc, pccEnd, '>', str)) {
                return false;
            }

            TRef<ColorValue> pcolor; CastTo(pcolor, pns->FindMember(str));
            if (pcolor) {
                m_color = pcolor->GetValue();
                return true;
            } else {
                return Error("Invalid color");
            }
        } else if (strTag == "String") {
            if (!ParseSymbol(pcc, pccEnd, '>', str)) {
                return false;
            }

            TRef<StringValue> pstring; CastTo(pstring, pns->FindMember(str));
            if (pstring) {
                InsertString(pstring->GetValue());
                return true;
            } else {
                return Error("undefined identifier '" + str + "'");
            }
        } else if (strTag == "Include") {
            if (!ParseSymbol(pcc, pccEnd, '|', str)) {
                return false;
            }

            ZString strFile;

            if (!ParseSymbol(pcc, pccEnd, '>', strFile)) {
                return false;
            }

            return Include(pns, str, strFile);
        } else if (strTag == "Font") {
            if (!ParseSymbol(pcc, pccEnd, '>', str)) {
                return false;
            }

            TRef<FontValue> pfont; CastTo(pfont, pns->FindMember(str));
            if (pfont) {
                m_pfont = pfont->GetValue();
                return true;
            } else {
                return Error("Invalid font");
            }
        } else if (strTag == "Image") {
            if (!ParseSymbol(pcc, pccEnd, '>', str)) {
                return false;
            }

            TRef<Image> pimage = m_pmodeler->LoadImage(str + "bmp", true, false);
            if (pimage) {
                InsertAtBottom(new ImagePane(pimage));
                return true;
            } else {
                return Error("Invalid image");
            }
        } else if (strTag == "Main") {
            return InsertLink(pcc, pccEnd, m_colorMain, m_peventSourceMain);
        } else if (strTag == "Secondary") {
            return InsertLink(pcc, pccEnd, m_colorSecondary, m_peventSourceSecondary);
        } else if (strTag == "Bullet") {
            return ParseBullet(pns, pcc, pccEnd);
        } else if (strTag == "Hyperlink") {
            return InsertHyperlink(pcc, pccEnd, Color(0,0.4f,1), m_peventSourceSecondary);
        }

        return Error("Unknown tag: " + strTag);
    }

    void ParseWord(PCC& pcc, PCC pccEnd)
    {
        PCC pccStart = pcc;

        while (
               pcc < pccEnd
            && *pcc != ' '
            && *pcc != '\t'
            && *pcc != '<'
            && *pcc != 10
            && *pcc != 13
        ) {
            pcc++;
        }

        InsertWord(pccStart, pcc);
    }

    void SkipWhite(PCC& pcc, PCC pccEnd)
    {
        while (
               pcc < pccEnd
            && (
                   *pcc == ' '
                || *pcc == 10
                || *pcc == 13
            )
        ) {
            pcc++;
        }
    }

    void Parse(INameSpace* pns, PCC pccStart, PCC pccEnd)
    {
        PCC pcc = pccStart;

        //
        // Find all the words
        //

        SkipWhite(pcc, pccEnd);

        while (pcc < pccEnd) {
            if (*pcc == '<') {
                //
                // It's a tag
                //

                if (!ParseTag(pns, pcc, pccEnd)) {
                    return;
                }
            } else if (*pcc == '\t') {
                //
                // It's a tab
                //
                
                pcc++;
                InsertTab();
            } else {
                //
                // It's a word
                //
                
                ParseWord(pcc, pccEnd);
            }

            SkipWhite(pcc, pccEnd);
        }
    }

    void Parse(INameSpace* pns, const ZString& str)
    {
        Parse(pns, &str[0], (&str[0]) + str.GetLength());
    }

    void FixupLineY(Pane* ppaneFirst, Pane* ppaneLast, int ysize)
    {
        for (Pane* ppane = ppaneFirst; ppane != ppaneLast; ppane = ppane->Next()) {
            InternalSetOffset(
                ppane, 
                WinPoint(
                    ppane->XOffset(), 
                    ppane->YOffset() + (ysize - ppane->YSize())
                )
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // PagePane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetAttributes(
        IEngineFont* pfont,
        const Color& color,
        const Color& colorMain,
        const Color& colorSecondary,
        const Color& colorHighlight
    ) {
        m_pfontDefault   = pfont;
        m_colorDefault   = color;
        m_colorMain      = colorMain;
        m_colorSecondary = colorSecondary;
        m_colorHighlight = colorHighlight;
    }

    TRef<IStringEventSource> GetMainLinkEventSource()
    {
        return m_peventSourceMain;
    }

    TRef<IStringEventSource> GetSecondaryLinkEventSource()
    {
        return m_peventSourceSecondary;
    }

    void SetTopic(INameSpace* pns, const ZString& str)
    {
        RemoveAllChildren();

        m_color = m_colorDefault;
        m_pfont = m_pfontDefault;

        TRef<ZFile> pfile = m_pmodeler->LoadFile(str, "mml", false);

        if (pfile) {
            if (pfile->GetLength() > 0) {
                PCC pcc = (PCC)(pfile->GetPointer());
                Parse(pns, pcc, pcc + pfile->GetLength());
            }
        } else {
            ZString str("<Color|red>Error: Can't open file '" + str + "'");

            Parse(pns, PCC(str), PCC(str) + str.GetLength());
        }
    }

    void SetTopicText(INameSpace* pns, const ZString& str)
    {
        RemoveAllChildren();

        m_color = m_colorDefault;
        m_pfont = m_pfontDefault;

        PCC pcc = str;
        Parse(pns, pcc, pcc + str.GetLength());
    }

    void SetPagePaneIncluder(PagePaneIncluder* ppagePaneIncluder)
    {
        m_ppagePaneIncluder = ppagePaneIncluder;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateLayout()
    {
        //
        // Update the size of all the children
        //

        Pane* ppane;
        for(ppane = Child(); ppane != NULL; ppane = ppane->Next()) {
            InternalSetExpand(ppane, WinPoint(0, 0));
            ppane->UpdateLayout();
        }

        //
        // Flow the words
        //

        int xsize     = XExpand();
        int ysizeLine = 0;
        int x         = 0;
        int y         = 0;

        Pane* ppaneLine = Child();  

        for(ppane = Child(); ppane != NULL; ppane = ppane->Next()) {
            //
            // Go to the next line?
            //

            int xsizePane = ppane->GetAlignedXSize(x);

            if (
                   xsizePane == 0
                || (
                       x + xsizePane > xsize
                    && x != 0
                   )
            ) {
                //
                // Doesn't fit on this line.
                //

                FixupLineY(ppaneLine, ppane, ysizeLine);
                y         += ysizeLine;
                x          = 0;
                ysizeLine  = 0;
                ppaneLine  = ppane;
            }

            //
            // Place the word
            //

            InternalSetOffset(ppane, WinPoint(x, y));
            x         += xsizePane;
            ysizeLine  = std::max(ysizeLine, ppane->YSize());
        }

        //
        // fixup the last line
        //

        FixupLineY(ppaneLine, NULL, ysizeLine);

        //
        // This panes's size
        //

        InternalSetSize(WinPoint(xsize, y + ysizeLine));
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class ScrollingPanePane : 
    public Pane,
    public IIntegerEventSink
{
private:
    TRef<ScrollPane>           m_pscroll;
    TRef<Pane>                 m_ppane;
    TRef<IIntegerEventSink>    m_pintegerEventSink;
    //TRef<ModifiablePointValue> m_ppoint;

public:
    ScrollingPanePane(Pane* ppane, const WinPoint& size, ScrollPane* pscroll) :
        m_ppane(ppane),
        m_pscroll(pscroll)
    {
        InternalSetSize(size);
        InsertAtBottom(ppane);

        /*
        m_ppoint = new ModiablePointValue(Point(0, 0));

        InsertAtBotttom(
            new AnimatedImagePane(
                new PaneImage(
                    new AnimatedImagePaneRect(
                        new TranslateImage(
                            pimage,
                            m_ppoint
                        ),
                        rect
                    ),
                    false,
                    true
                )
            )
        );
        */

        if (m_pscroll) {
            m_pscroll->GetEventSource()->AddSink(
                m_pintegerEventSink = IIntegerEventSink::CreateDelegate(this)
            );
        }

    }

    ~ScrollingPanePane()
    {
        if (m_pscroll) {
            m_pscroll->GetEventSource()->RemoveSink(m_pintegerEventSink);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnEvent(IIntegerEventSource* pevent, int value)
    {
        NeedPaint();
        NeedLayout();
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateLayout()
    {
        InternalSetOffset(m_ppane, WinPoint(0, -m_pscroll->GetPos()));
        m_ppane->UpdateLayout();

        m_pscroll->SetLineSize(8);
        m_pscroll->SetSizes(
            m_ppane->YSize(),
            YSize()
        );
    }

    void Paint(Surface* psurface)
    {
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// PagePane Wrapper
//
//////////////////////////////////////////////////////////////////////////////

class PagePaneWrapper : public PagePane {
private:
    TRef<PagePane> m_ppagePane;

public:
    PagePaneWrapper(Modeler* pmodeler, const WinPoint& size, ScrollPane* pscrollPane)
    {
        m_ppagePane = new PagePaneImpl(pmodeler, size.X(), NULL, NULL);
        TRef<Pane> ppane =
            new JustifyPane(
                JustifyPane::Left | JustifyPane::Top,
                size,
                m_ppagePane
            );

        if (pscrollPane) {
            InsertAtBottom(
                new ScrollingPanePane(
                    ppane,
                    size,
                    pscrollPane
                )
            );
        } else {
            InsertAtBottom(ppane);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // PagePane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetAttributes(
        IEngineFont* pfont,
        const Color& color,
        const Color& colorMain,
        const Color& colorSecondary,
        const Color& colorHighlight
    ) {
        m_ppagePane->SetAttributes(
            pfont,
            color,
            colorMain,
            colorSecondary,
            colorHighlight
        );
    }

    TRef<IStringEventSource> GetMainLinkEventSource()
    {
        return m_ppagePane->GetMainLinkEventSource();
    }

    TRef<IStringEventSource> GetSecondaryLinkEventSource()
    {
        return m_ppagePane->GetSecondaryLinkEventSource();
    }

    void SetTopic(INameSpace* pns, const ZString& str)
    {
        m_ppagePane->SetTopic(pns, str);
    }

    void SetTopicText(INameSpace* pns, const ZString& str)
    {
        m_ppagePane->SetTopicText(pns, str);
    }

    void SetPagePaneIncluder(PagePaneIncluder* ppagePaneIncluder)
    {
        m_ppagePane->SetPagePaneIncluder(ppagePaneIncluder);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Factories
//
//////////////////////////////////////////////////////////////////////////////

class PagePaneFactory : public IFunction {
private:
    TRef<Modeler> m_pmodeler;

public:
    PagePaneFactory(Modeler* pmodeler) :
        m_pmodeler(pmodeler)        
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<PointValue> psize;       CastTo(psize,              (IObject*)stack.Pop());
        TRef<ScrollPane> pscrollPane; CastTo(pscrollPane, (Pane*)(IObject*)stack.Pop());

        return new PagePaneWrapper(m_pmodeler, WinPoint::Cast(psize->GetValue()), pscrollPane);
    }
};

void AddPagePaneFactory(
    INameSpace* pns,
    Modeler*    pmodeler
) {
    pns->AddMember("PagePane", new PagePaneFactory(pmodeler));
}
