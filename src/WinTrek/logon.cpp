#include "pch.h"

#include <button.h>
#include <controls.h>

//////////////////////////////////////////////////////////////////////////////
//
// Logon Popup
//
//////////////////////////////////////////////////////////////////////////////

class LogonPopup :
    public IPopup,
    public EventTargetContainer<LogonPopup>,
    public TEvent<ZString>::Sink
{
private:
    TRef<Pane>           m_ppane;
    TRef<ButtonPane>     m_pbuttonLogon;
    TRef<ButtonPane>     m_pbuttonAbort;
    TRef<ButtonPane>     m_pbuttonSignUp;
    TRef<ButtonPane>     m_pbuttonCDKey;
    TRef<ButtonPane>     m_pbuttonSavePassword;
    TRef<EditPane>       m_peditName;
    TRef<EditPane>       m_peditPassword;
    TRef<Pane>           m_ppanePassword;
    TRef<LogonSite>      m_psite;
    TRef<IKeyboardInput> m_pfocus;
    LogonType            m_lt;

public:
    LogonPopup(Modeler* pmodeler, LogonSite* psite, LogonType lt,
               LPCSTR szPrompt, LPCSTR szName, LPCSTR szPW, BOOL fRememberPW) :
        m_psite(psite),
        m_lt(lt)
    {
        enum LogonNameType { lntCallsign, lntZone, lntPassport } logonNameType;
        if (lt == LogonLAN)
            logonNameType = lntCallsign;
        else if (trekClient.GetCfgInfo().bUsePassport)
            logonNameType = lntPassport;
        else
            logonNameType = lntZone;

        TRef<INameSpace> pnsLogonData = pmodeler->CreateNameSpace("logondata", pmodeler->GetNameSpace("gamepanes"));
        pnsLogonData->AddMember("promptText", new ModifiableString(szPrompt));
        pnsLogonData->AddMember("Callsign", new ModifiableNumber((float)lntCallsign));
        pnsLogonData->AddMember("ZoneID", new ModifiableNumber((float)lntZone));
        pnsLogonData->AddMember("PassportID", new ModifiableNumber((float)lntPassport));
        pnsLogonData->AddMember("logonNameType", new ModifiableNumber((float)logonNameType));

        TRef<INameSpace> pns = pmodeler->GetNameSpace("logon");
        CastTo(m_ppane, pns->FindMember("popup"));

		// mdvalley: Needs the pointer and class name.
        CastTo(m_pbuttonLogon,        pns->FindMember("logonButtonPane"));
        AddEventTarget(&LogonPopup::OnButtonLogon, m_pbuttonLogon->GetEventSource());
        CastTo(m_pbuttonAbort,        pns->FindMember("abortButtonPane"));
        AddEventTarget(&LogonPopup::OnButtonAbort, m_pbuttonAbort->GetEventSource());
        CastTo(m_pbuttonSignUp,       pns->FindMember("signUpButtonPane"));
        AddEventTarget(&LogonPopup::OnButtonSignUp, m_pbuttonSignUp->GetEventSource());
        CastTo(m_pbuttonCDKey,        pns->FindMember("cdKeyButtonPane"));
        AddEventTarget(&LogonPopup::OnButtonCDKey, m_pbuttonCDKey->GetEventSource());
        CastTo(m_peditName,     (Pane*)pns->FindMember("nameEditPane"    ));
        AddEventTarget(&LogonPopup::OnNameClick,     m_peditName->GetClickEvent());

        CastTo(m_pbuttonSavePassword, pns->FindMember("savePasswordButtonPane"));
        CastTo(m_ppanePassword, pns->FindMember("passwordPane"));
        CastTo(m_peditPassword, (Pane*)pns->FindMember("passwordEditPane"));
        AddEventTarget(&LogonPopup::OnPasswordClick, m_peditPassword->GetClickEvent());
        m_peditPassword->SetType(EditPane::Password);
        m_peditPassword->SetString(ZString());

        m_peditName->SetString(ZString());

        // !!! disable logon button if name is empty
        // Get defaults
        if (LogonLAN == lt)
        {
            m_ppanePassword->SetHidden(true);
            m_pbuttonSavePassword->SetHidden(true);
            m_pbuttonCDKey->SetHidden(true);
            m_pbuttonSignUp->SetHidden(true);
        }
        else
        {
            m_peditPassword->SetString(szPW);
            m_pbuttonSavePassword->SetChecked(!!fRememberPW);
        }

        m_peditName->SetString(szName);
        m_peditName->SetMaxLength((LogonLAN != lt && trekClient.GetCfgInfo().bUsePassport) 
          ? (c_cbPassportName - 1)
          : (c_cbName - 1)
          );

        if (m_peditName->GetString().IsEmpty()) {
            SetFocus(m_peditName);
        } else {
            SetFocus(m_peditPassword);
        }

        pmodeler->UnloadNameSpace(pns);
    }

    bool OnButtonLogon()
    {
        // Make sure that they have entered a CD Key
        if (g_bAskForCDKey && trekClient.GetCDKey().IsEmpty())
        {
            TRef<IPopup> ppopupCDKey = CreateCDKeyPopup();
            GetWindow()->GetPopupContainer()->OpenPopup(ppopupCDKey, false);
        }
        else
        {
            //
            // Grab info from the controls
            //
            ZString strName = m_peditName->GetString();
            ZString strPassword = m_peditPassword->GetString();
            bool fRememberPW = m_pbuttonSavePassword->GetChecked();

            if (strName[0] != ' ' && strName[0] != '\0') // give 'em a msg box?
            {
                //
                // Close
                //

                TRef<LogonPopup> pthis = this;
                ClosePopup(NULL);


                //
                // Tell the site
                //

                m_psite->OnLogon(strName, strPassword, fRememberPW);
            }
        }

        return true;
    }

    bool OnButtonAbort()
    {
        m_psite->OnAbort();
        ClosePopup(NULL);
        return true;
    }

    bool OnButtonSignUp()
    {
		// KGJV - Alleg.net registration
        GetWindow()->ShowWebPage("http://asgs.alleg.net/");
        return true;
    }

    bool OnButtonCDKey()
    {
        TRef<IPopup> ppopupCDKey = CreateCDKeyPopup();
        GetWindow()->GetPopupContainer()->OpenPopup(ppopupCDKey, false);

        return true;
    }

    void SetFocus(IKeyboardInput* pfocus) 
    {
        if (m_pfocus) {
            m_pfocus->SetFocusState(false);
        }
        m_pfocus = pfocus;
        m_pfocus->SetFocusState(true);
    }

    bool OnEvent(TEvent<ZString>::Source* psource, ZString str)
    {
        m_pbuttonLogon->SetEnabled(!str.IsEmpty());
        return true;
    }

    bool OnNameClick()
    {
        SetFocus(m_peditName);
        return true;
    }

    bool OnPasswordClick()
    {
        SetFocus(m_peditPassword);
        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IPopup Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetContainer(IPopupContainer* pcontainer)
    {
        IPopup::SetContainer(pcontainer);

        if (g_bQuickstart || g_bReloaded)
        {
            g_bReloaded = false; // we do this just once
            OnButtonLogon();
        }
    }

    Pane* GetPane()
    {
        return m_ppane;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IKeyboardInput Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown) 
        {
            if (ks.vk == VK_RETURN) 
            {
                if (m_pbuttonLogon->GetEnabled()) 
                {
                    OnButtonLogon();
                } 
                else 
                {
                    OnButtonAbort();
                }
                return true;
            } 
            else if (ks.vk == VK_ESCAPE) 
            {
                OnButtonAbort();
                return true;
            } 
            else if (ks.vk == VK_TAB) 
            {
                if (m_pfocus == static_cast<EditPane*>(m_peditName)	// Resolve ambiguity - added by Dhauzmmer 8/14/04
                    && (LogonAllegianceZone == m_lt || LogonFreeZone == m_lt)) 
                {
                    SetFocus(m_peditPassword);
                } else 
                {
                    SetFocus(m_peditName);
                }
                return true;
            }
        }

        return m_pfocus->OnKey(pprovider, ks, fForceTranslate);
    }

    bool OnChar(IInputProvider* pprovider, const KeyState& ks)
    {
        return m_pfocus->OnChar(pprovider, ks);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<IPopup> CreateLogonPopup(Modeler* pmodeler, LogonSite* psite, LogonType lt, LPCSTR szPrompt, LPCSTR szName, LPCSTR szPW, BOOL fRememberPW)
{
    return new LogonPopup(pmodeler, psite, lt, szPrompt, szName, szPW, fRememberPW);
}


