#include "help.h"

#include <controls.h>
#include <button.h>
#include <namespace.h>
//////////////////////////////////////////////////////////////////////////////
//
// Help
//
//////////////////////////////////////////////////////////////////////////////

class HelpPaneImpl : 
    public HelpPane,
    public EventTargetContainer<HelpPaneImpl>
{
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class MainSink : public IStringEventSink {
    public:
        HelpPaneImpl* m_ppane;

        MainSink(HelpPaneImpl* ppane) :
            m_ppane(ppane)
        {
        }

        bool OnEvent(IStringEventSource* pevent, ZString str)
        {
            m_ppane->OnMainTopic(str);
            return true;
        }
    };

    class SecondarySink : public IStringEventSink {
    public:
        HelpPaneImpl* m_ppane;

        SecondarySink(HelpPaneImpl* ppane) :
            m_ppane(ppane)
        {
        }

        bool OnEvent(IStringEventSource* pevent, ZString str)
        {
            m_ppane->OnSecondaryTopic(str);
            return true;
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Modeler>    m_pmodeler;
    TRef<INameSpace> m_pnsData;
    TRef<INameSpace> m_pns;
    TRef<ButtonPane> m_pbuttonBack;
    TRef<ButtonPane> m_pbuttonClose;
	TRef<ButtonPane> m_pbuttonDiscord;
	TRef<ButtonPane> m_pbuttonWiki;
    TRef<PagePane>   m_ppageMain;
    TRef<PagePane>   m_ppageSecondary;
    TRef<NavPane>    m_pnavPane;
    TList<ZString>   m_listHistory;
    TRef<ScrollPane> m_pscrollMain;
    TRef<ScrollPane> m_pscrollSecondary;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    HelpPaneImpl(Modeler* pmodeler, const ZString& strTopic, PagePaneIncluder* ppagePageIncluder) :
        m_pmodeler(pmodeler)
    {
        //
        // Exports
        //

		m_pmodeler->SetColorKeyHint( true );

        m_pnsData = pmodeler->CreateNameSpace("helpdata");

        //
        // Exports
        //

        m_pns = pmodeler->GetNameSpace("helppane");

        //
        // The Help pane
        //

        TRef<Pane> ppane;
        CastTo(ppane, m_pns->FindMember("helpPane"));
        InsertAtBottom(ppane);

        //
        // Buttons
        //

        CastTo(m_pbuttonBack, m_pns->FindMember("backButton"   ));
		// mdvalley: OnButtonBack now pointered and with class named.
		AddEventTarget(&HelpPaneImpl::OnButtonBack, m_pbuttonBack->GetEventSource());


        CastTo(m_pbuttonClose, m_pns->FindMember("closeButton"));
		CastTo(m_pbuttonDiscord, m_pns->FindMember("DiscordButton"));
		AddEventTarget(&HelpPaneImpl::OnButtonDiscord, m_pbuttonDiscord->GetEventSource());
		CastTo(m_pbuttonWiki, m_pns->FindMember("WikiButton"));
		AddEventTarget(&HelpPaneImpl::OnButtonWiki, m_pbuttonWiki->GetEventSource());
        //
        // Default attributes
        //

        TRef<FontValue>  pfont;
        TRef<ColorValue> pcolor;
        TRef<ColorValue> pcolorMain;
        TRef<ColorValue> pcolorSecondary;
        TRef<ColorValue> pcolorHighlight;

        CastTo(pfont,           m_pns->FindMember("helpFont"          ));
        CastTo(pcolor,          m_pns->FindMember("helpColor"         ));
        CastTo(pcolorMain,      m_pns->FindMember("mainLinkColor"     ));
        CastTo(pcolorSecondary, m_pns->FindMember("secondaryLinkColor"));
        CastTo(pcolorHighlight, m_pns->FindMember("highlightColor"    ));

        //
        // The Nav pane
        //

        CastTo(m_pnavPane,       m_pns->FindMember("navPane"      ));

        //
        // PagePanes
        //

        CastTo(m_ppageMain,      m_pns->FindMember("mainPage"     ));
        CastTo(m_ppageSecondary, m_pns->FindMember("secondaryPage"));

        m_ppageMain->SetAttributes(
            pfont->GetValue(),
            pcolor->GetValue(),
            pcolorMain->GetValue(),
            pcolorSecondary->GetValue(),
            pcolorHighlight->GetValue()
        );

        m_ppageSecondary->SetAttributes(
            pfont->GetValue(),
            pcolor->GetValue(),
            pcolorMain->GetValue(),
            pcolorSecondary->GetValue(),
            pcolorHighlight->GetValue()
        );

        //
        // Includer
        //

        m_ppageMain     ->SetPagePaneIncluder(ppagePageIncluder);
        m_ppageSecondary->SetPagePaneIncluder(ppagePageIncluder);

        //
        // Scroll Bars
        //

        CastTo(m_pscrollMain,      (Pane*)m_pns->FindMember("mainPageScrollBar"));
        CastTo(m_pscrollSecondary, (Pane*)m_pns->FindMember("secondaryPageScrollBar"));
        
        //
        // Sink topic event
        //

        TRef<MainSink> psinkMain = new MainSink(this);

        m_ppageMain     ->GetMainLinkEventSource()->AddSink(psinkMain);
        m_ppageSecondary->GetMainLinkEventSource()->AddSink(psinkMain);
        m_pnavPane      ->GetEventSource()        ->AddSink(psinkMain);

        TRef<SecondarySink> psinkSecondary = new SecondarySink(this);

        m_ppageMain     ->GetSecondaryLinkEventSource()->AddSink(psinkSecondary);
        m_ppageSecondary->GetSecondaryLinkEventSource()->AddSink(psinkSecondary);

        //
        // Set the initial page
        //

        SetTopic(strTopic);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////
	void ShowWebPage(const char* szURL)
	{
		ShellExecute(NULL, NULL, szURL, NULL, NULL, SW_SHOWNORMAL);
	}
    void DoSetTopic()
    {
        const ZString& str = m_listHistory.GetFront();

        m_pnavPane->SetTopic(str);
        m_ppageMain->SetTopic(m_pns, str);
        m_ppageSecondary->SetTopic(m_pns, m_pnavPane->GetSecondary(str));
        m_pscrollMain->SetPos(0);
        m_pscrollSecondary->SetPos(0);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Events
    //
    //////////////////////////////////////////////////////////////////////////////
    bool OnButtonBack()
    {
        if (m_listHistory.GetCount() > 1) {
            m_listHistory.PopFront();
            DoSetTopic();
        }
        return true;
    }

    bool OnMainTopic(const ZString& str)
    {
        if (m_listHistory.GetFront()   != str) {
            m_listHistory.PushFront(str);
            DoSetTopic();
        }
        return true;
    }
	bool OnButtonDiscord() 
	{
	
		ShowWebPage("https://discord.gg/WcEJ9VH");
		int Getwindow();
		return true;
	}
	bool OnButtonWiki()
	{
		
		ShowWebPage("http://www.freeallegiance.org/FAW/index.php/Quick_Crash_Course");
		int Getwindow();
		return true;
	}

    bool OnSecondaryTopic(const ZString& str)
    {
        m_ppageSecondary->SetTopic(m_pns, str);
        m_pscrollSecondary->SetPos(0);
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // HelpPane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    IEventSource* GetEventSourceClose()
    {
        return m_pbuttonClose->GetEventSource();
    }

    void SetTopic(const ZString& str)
    {
        m_listHistory.SetEmpty();
        m_listHistory.PushFront(str);
        DoSetTopic();
    }

    void SetString(const ZString& strIdentifier, const ZString& strValue)
    {
        m_pnsData->AddMember(strIdentifier, new StringValue(strValue));
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

    void Paint(Surface* psurface)
    {
        HelpPane::Paint(psurface);
    }
    //
    // IMouseInput added by Imago 8/14/09 mouse wheel
    //

    MouseResult Button(
        IInputProvider* pprovider,
        const Point& point,
        int button,
        bool bCaptured,
        bool bInside,
        bool bDown
    ) {
        if(button == 8 && bDown) { //Imago 8/14/09 mouse wheel
            m_pscrollMain->LineDown(); //pagedown is too much and just one line doesn't seem enough
            m_pscrollMain->LineDown();
        } else if (button == 9 && bDown) { 
            m_pscrollMain->LineUp(); //intentional duplication
            m_pscrollMain->LineUp();
        }
        return Pane::Button(pprovider, point, button, bCaptured, bInside, bDown);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Help
//
//////////////////////////////////////////////////////////////////////////////

TRef<HelpPane> CreateHelpPane(Modeler* pmodeler, const ZString& strTopic, PagePaneIncluder* ppagePageIncluder)
{
    return new HelpPaneImpl(pmodeler, strTopic, ppagePageIncluder);
}     
