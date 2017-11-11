#include "help.h"

#include <button.h>
#include <controls.h>
#include <event.h>
#include <popup.h>
#include <namespace.h>

//////////////////////////////////////////////////////////////////////////////
//
// MMLPopup
//
//////////////////////////////////////////////////////////////////////////////

class MMLPopup :
    public IPopup,
    public EventTargetContainer<MMLPopup>
{
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Pane>       m_ppane;
    TRef<INameSpace> m_pns;
    TRef<ButtonPane> m_pbuttonClose;
	TRef<ButtonPane> m_pbuttonDiscord;
	TRef<ButtonPane> m_pbuttonWiki;
    TRef<PagePane>   m_ppageMain;
    TRef<ScrollPane> m_pscrollMain;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    MMLPopup(Modeler* pmodeler, const ZString& strTopic, bool bText)
    {
        //
        // Exports
        //

        m_pns = pmodeler->GetNameSpace("mmlpane");

        //
        // The Help pane
        //

        CastTo(m_ppane, m_pns->FindMember("mmlPane"));

        //
        // Buttons
        //

        CastTo(m_pbuttonClose, m_pns->FindMember("closeButton"));
		// mdvalley: OnButtonClose now needs to be pointer and with class spelled out in 2005.
		AddEventTarget(&MMLPopup::OnButtonClose, m_pbuttonClose->GetEventSource());

		CastTo(m_pbuttonDiscord, m_pns->FindMember("DiscordButton"));
        if (m_pbuttonDiscord) {
            AddEventTarget(&MMLPopup::OnButtonDiscord, m_pbuttonDiscord->GetEventSource());
        }
		CastTo(m_pbuttonWiki, m_pns->FindMember("WikiButton"));
        if (m_pbuttonWiki) {
            AddEventTarget(&MMLPopup::OnButtonWiki, m_pbuttonWiki->GetEventSource());
        }
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
        // PagePanes
        //

        CastTo(m_ppageMain, m_pns->FindMember("mainPage"));

        m_ppageMain->SetAttributes(
            pfont->GetValue(),
            pcolor->GetValue(),
            pcolorMain->GetValue(),
            pcolorSecondary->GetValue(),
            pcolorHighlight->GetValue()
        );

        //
        // Scroll Bars
        //

        CastTo(m_pscrollMain, (Pane*)m_pns->FindMember("mainPageScrollBar"));

        //
        // Set the initial page
        //

        if (bText) {
            m_ppageMain->SetTopicText(m_pns, strTopic);
        } else {
            m_ppageMain->SetTopic(m_pns, strTopic);
        }
    }

    ~MMLPopup()
    {
        m_ppageMain->SetTopicText(m_pns, "");
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Events
    //
    //////////////////////////////////////////////////////////////////////////////
	void ShowWebPage(const char* szURL)
	{
		ShellExecute(NULL, NULL, szURL, NULL, NULL, SW_SHOWNORMAL);
	}
    bool OnButtonClose()
    {
        Close();
        return true;
    }
	bool OnButtonDiscord()
	{

		ShowWebPage("https://discord.gg/WcEJ9VH");
		int GetWindow();
		return true;
	}
	bool OnButtonWiki()
	{

		ShowWebPage("http://www.freeallegiance.org/FAW/index.php/Quick_Crash_Course");
		int GetWindow();
		return true;
	}

    void Close()
    {
        if (m_ppopupOwner) {
            m_ppopupOwner->ClosePopup(this);
        } else {
            m_pcontainer->ClosePopup(this);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IPopup methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IKeyboardInput methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown) {
            if (ks.vk == VK_ESCAPE || ks.vk == VK_RETURN) {
                Close();
            }
        }

        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

TRef<IPopup> CreateMMLPopup(Modeler* pmodeler, const ZString& strTopic, bool bText)
{
    return new MMLPopup(pmodeler, strTopic, bText);
}     
