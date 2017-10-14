#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class CaptionImpl :
    public ICaption,
    public EventTargetContainer<CaptionImpl>
{
private:
    TRef<ButtonPane>   m_pbuttonClose;
    TRef<ButtonPane>   m_pbuttonRestore;
    TRef<ButtonPane>   m_pbuttonMinimize;
    TRef<Pane>         m_ppane;
    TRef<ICaptionSite> m_psite;

    void DoCreateButton(
              Modeler*          pmodeler,
              TRef<ButtonPane>& pbuttonPane,
        const ZString&          str,
        const WinPoint&         offset,
              Pane*             ppane
    ) {
        TRef<ButtonFacePane> pface = 
            CreateButtonFacePane(
                pmodeler->LoadSurface(str, false),
                ButtonFaceUp | ButtonFaceDown
            );

        pbuttonPane = CreateButton(pface);
        pbuttonPane->SetOffset(offset);
        ppane->InsertAtTop(pbuttonPane);
    }

public:
    CaptionImpl(Modeler* pmodeler, Pane* ppane, ICaptionSite* psite) :
        m_ppane(ppane),
        m_psite(psite)
    {
        DoCreateButton(pmodeler, m_pbuttonClose,    "btnclosebmp",    WinPoint(780, 5), ppane);
        DoCreateButton(pmodeler, m_pbuttonRestore,  "btnrestorebmp",  WinPoint(761, 5), ppane);
        DoCreateButton(pmodeler, m_pbuttonMinimize, "btnminimizebmp", WinPoint(744, 5), ppane);

		// mdvalley: Three pointers and class names
        AddEventTarget(&CaptionImpl::OnClose,    m_pbuttonClose->GetEventSource());
        AddEventTarget(&CaptionImpl::OnRestore,  m_pbuttonRestore->GetEventSource());
        AddEventTarget(&CaptionImpl::OnMinimize, m_pbuttonMinimize->GetEventSource());
    }

    bool OnClose()
    {
        m_psite->OnCaptionClose();
        return true;
    }

    bool OnMinimize()
    {
        m_psite->OnCaptionMinimize();
        return true;
    }

    bool OnRestore()
    {
        m_psite->OnCaptionRestore();
        return true;
    }

    void SetFullscreen(bool bFullscreen)
    {
        m_pbuttonClose->SetHidden(!bFullscreen);
        m_pbuttonRestore->SetHidden(!bFullscreen);
        m_pbuttonMinimize->SetHidden(!bFullscreen);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<ICaption> CreateCaption(Modeler* pmodeler, Pane* ppane, ICaptionSite* psite)
{
    return new CaptionImpl(pmodeler, ppane, psite);
}
