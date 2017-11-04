#include "efpopup.h"

#include <button.h>
#include <controls.h>
#include <event.h>
#include <image.h>

#include "efapp.h"

/////////////////////////////////////////////////////////////////////////////
//
// MessageBox
//
/////////////////////////////////////////////////////////////////////////////

class MessageBoxImpl :
    public IMessageBox,
    public IEventSink
{
    TRef<Modeler>                m_pmodeler;
    TRef<Pane>                   m_ppane;
    TRef<Pane>                   m_ppaneWrapper;
    TRef<IntegerEventSourceImpl> m_psource;
    int                          m_delay;
    TRef<ButtonPane>             m_pbuttonOK;
    TRef<ButtonPane>             m_pbuttonCancel;
    TRef<WrapImage>              m_pwrapImage;
    TRef<IEventSink>             m_peventSink;
    TRef<ITimerEventSource>      m_ptimerEventSource;

public:
    MessageBoxImpl(
        ITimerEventSource* ptimerEventSource,
        Modeler*           pmodeler,
        const ZString&     str,
        ButtonPane*        pbuttonIn,
        bool               fAddDefaultButton,
        ButtonPane*        pbuttonCancel,
        float              paintDelay
    ) :
        m_pmodeler(pmodeler)
    {
        m_pwrapImage = new WrapImage(Image::GetEmpty()); //Fix memory leak -Imago 8/2/09

		// Need a background with colour key.
        m_ppane = new ImagePane(pmodeler->LoadImage("dialogbkgndbmp", true ));

        TRef<Image> pstringImage =
            CreateStringImage(
                JustifyLeft(),
                TrekResources::LargeFont(),
                new ColorValue(Color::White()),
                403,
                new StringValue(str)
            );

        float bright = 1.0f / 16.0f;
        TRef<Image> pstringImage2 =
            CreateStringImage(
                JustifyLeft(),
                TrekResources::LargeFont(),
                new ColorValue(Color(bright)),
                403,
                new StringValue(str)
            );

        TRef<AnimatedImagePane> panimatedImagePane =
            new AnimatedImagePane(
                new GroupImage(
                    pstringImage,
                    new TranslateImage(pstringImage2, Point(1, -1))
                )
            );

        panimatedImagePane->SetOffset(WinPoint(50, 50));
        m_ppane->InsertAtBottom(panimatedImagePane);

        TRef<ButtonPane> pbutton = pbuttonIn;

        if (!pbutton && fAddDefaultButton) {
            TRef<Surface> psurfaceButton = pmodeler->LoadImage("btnokbmp", true)->GetSurface();

            m_pbuttonOK = 
                CreateButton(
                    CreateButtonFacePane(psurfaceButton, ButtonNormal, 0, psurfaceButton->GetSize().X()),
                    false, 0, 1.0
                );

            pbutton = m_pbuttonOK;
        } else if (pbutton) {
            m_pbuttonOK = pbutton;            
        }

        if (pbuttonCancel) {
            m_pbuttonCancel = pbuttonCancel;

            if (pbutton == NULL) {
                pbutton = m_pbuttonCancel;
            }
        }

        if (pbutton) {
            m_ppane->InsertAtBottom(pbutton);
            pbutton->GetEventSource()->AddSink(IEventSink::CreateDelegate(this));
            pbutton->SetOffset(WinPoint(140, 170));

            if (m_pbuttonCancel != NULL && m_pbuttonCancel != pbutton) {
                m_ppane->InsertAtBottom(m_pbuttonCancel);
                m_pbuttonCancel->GetEventSource()->AddSink(IEventSink::CreateDelegate(this));

                pbutton->SetOffset(WinPoint(110, 170));
                pbuttonCancel->SetOffset(WinPoint(290, 170));
            }
        }

        m_psource = new IntegerEventSourceImpl();

        if (paintDelay == 0) {
            m_ppaneWrapper = m_ppane;
            m_pwrapImage->SetImage(IPopup::GetImage(m_pmodeler->GetEngine()));
        } else {
            m_ppaneWrapper = new AnimatedImagePane(m_pwrapImage);  //Fix memory leak -Imago 8/2/09
            m_peventSink = IEventSink::CreateDelegate(this);
            m_ptimerEventSource = ptimerEventSource;
            ptimerEventSource->AddSink(m_peventSink, paintDelay);
        }
    }

    ~MessageBoxImpl()
    {
        if (m_peventSink != NULL) {
            m_ptimerEventSource->RemoveSink(m_peventSink);
        }
    }

    void Close(int id)
    {
        // an event may delete the last reference to this, so keep a reference
        // to ourselves for the duration of the function.

        TRef<MessageBoxImpl> pthis = this;

        if (m_ppopupOwner) {
            m_ppopupOwner->ClosePopup(this);
        } else {
            m_pcontainer->ClosePopup(this);
        }
        m_psource->Trigger(id);
    }

    //
    // IEventSync
    //

    bool OnEvent(IEventSource* pevent)
    {
        if (m_pbuttonOK && pevent == m_pbuttonOK->GetEventSource()) {
            Close(IDOK);
        } else if (m_pbuttonCancel && pevent == m_pbuttonCancel->GetEventSource()) {
            Close(IDCANCEL);
        } else {
            //
            // Timer
            //

            ZAssert(m_peventSink != NULL);

            m_peventSink = NULL;
            m_pwrapImage->SetImage(IPopup::GetImage(m_pmodeler->GetEngine()));
        }

        return false;
    }

    //
    // IKeyboardInput
    //

    bool OnChar(IInputProvider* pprovider, const KeyState& ks)
    {
        return true;
    }

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown) {
            if (ks.vk == VK_ESCAPE) {
                if (m_pbuttonCancel) {
                    Close(IDCANCEL);
                } else if (m_pbuttonOK) {
                    Close(IDOK);
                }
            } else {
                if (ks.vk == VK_RETURN) {
                    if (m_pbuttonOK) {
                        Close(IDOK);
                    }
                }
            }
        }

        return true;
    }

    //
    // IPopup methods
    //

    Pane* GetPane()
    {
        return m_ppaneWrapper;
    }

    Image* GetImage()
    {
        return m_pwrapImage;
    }

    IntegerEventSourceImpl* GetEventSource()
    {
        return m_psource;
    }
};

TRef<IMessageBox> CreateMessageBox(
    ITimerEventSource* ptimerEventSource,
    Modeler*           pmodeler, 
    const ZString&     str, 
    ButtonPane*        pbuttonIn, 
    bool               fAddDefaultButton,
    ButtonPane*        pbuttonCancel,
    float              paintDelay
) {
    return new MessageBoxImpl(ptimerEventSource, pmodeler, str, pbuttonIn, fAddDefaultButton, pbuttonCancel, paintDelay);
}
