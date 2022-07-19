#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Video Screen
//
//////////////////////////////////////////////////////////////////////////////

class VideoScreen : 
    public Screen,
    public IEventSink,
    public IKeyboardInput
{
public:
    /////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    /////////////////////////////////////////////////////////////////////////////

    class PickImage : public Image {
    private:
        VideoScreen* m_pobject;

    public:
        PickImage(VideoScreen* pobject) :
            m_pobject(pobject)
        {
        }

        MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
        {
            return MouseResultHit();
        }

        MouseResult Button(
            IInputProvider* pprovider,
            const Point& point,
            int button,
            bool bCaptured,
            bool bInside,
            bool bDown
        ) {
            if (bDown) {
                m_pobject->Picked();
            }
            return MouseResult();
        }
    };

    /////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    /////////////////////////////////////////////////////////////////////////////

    TRef<Engine>         m_pengine;
    TRef<WrapImage>      m_pwrapImageVideo;
    TRef<VideoImage>     m_pvideoImage;
    TRef<Image>          m_pimage;
    TRef<IKeyboardInput> m_pkeyboardInputOldFocus;
    int                  m_indexVideo;
    bool                 m_bNextVideo;
    ZString              m_strCD;

    /////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    /////////////////////////////////////////////////////////////////////////////

    VideoScreen(Modeler* pmodeler, bool bIntroOnly) :
        m_pengine(pmodeler->GetEngine()),
        m_indexVideo(bIntroOnly ? 2 : -1),
        m_bNextVideo(false)
    {
        //
        // Load the location of the cd drive
        //

        HKEY hKey;

        if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT,
                0, "", REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hKey, NULL)) {
            char buf[128];
            DWORD dwSize = sizeof(buf);
            DWORD dwType = REG_SZ;

            ::RegQueryValueEx(hKey, "CDPath", NULL, &dwType, (BYTE*)buf, &dwSize);
            ::RegCloseKey(hKey);

            if (dwType == REG_SZ) {
                m_strCD = ZString(buf);
                m_strCD.ReplaceAll("\\", '/');

                if (m_strCD[m_strCD.GetLength() - 1] != '/') {
                    m_strCD += '/';
                }
            }
        }

        //
        // Create the image hierarchy
        //

        m_pwrapImageVideo = new WrapImage(Image::GetEmpty());
        m_pimage =
            new GroupImage(
                new PickImage(this),
                m_pwrapImageVideo
            );

        m_pkeyboardInputOldFocus = GetWindow()->GetFocus();
        GetWindow()->SetFocus(IKeyboardInput::CreateDelegate(this));

        //
        // Start the first video
        //

        NextVideo();
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    /////////////////////////////////////////////////////////////////////////////

    void Dismiss()
    {
        GetWindow()->SetFocus(m_pkeyboardInputOldFocus);
        GetWindow()->screen(ScreenIDIntroScreen);
    }

    void NextVideo()
    {
// BUILD_DX9
        //ZString str;

        //m_indexVideo++;
        //switch (m_indexVideo) {
        //    case 0: str = "msr_games.avi"; break;
        //    case 1: str = "logo1.avi";     break;
        //    case 2: str = "logo2.avi";     break;
        //    case 3: str = "intro.avi";     break;
        //    default: 
        //        return;
        //};

        ////
        //// Load from the artpath first
        ////

        //m_pvideoImage = 
        //    CreateVideoImage(
        //        m_pengine,
        //        GetWindow()->GetScreenRectValue(), 
        //        GetModeler()->GetArtPath() + "/" + str
        //    );

        //if (
        //       (m_pvideoImage == NULL || (!m_pvideoImage->IsValid()))
        //    && (!m_strCD.IsEmpty())
        //) {
        //    m_pvideoImage = 
        //        CreateVideoImage(
        //            m_pengine,
        //            GetWindow()->GetScreenRectValue(), 
        //            m_strCD + str
        //        );
        //}

        //if (!m_pvideoImage->IsValid()) {
        //    NextVideo();
        //} else {
        //    m_pwrapImageVideo->SetImage(m_pvideoImage);
        //    m_pvideoImage->GetEventSource()->AddSink(IEventSink::CreateDelegate(this));
        //}
// BUILD_DX9
    }

    bool OnEvent(IEventSource* pevent)
    {
        m_bNextVideo = true;
        return false;
    }

    void Picked()
    {
        NextVideo();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IKeyboardInput
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        if (ks.bDown) {
            NextVideo();
        }
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Image* GetImage()
    {
        return m_pimage;
    }

    WinPoint GetSize()
    {
        return WinPoint(640, 480);
    }

    void OnFrame() 
    {
        if (m_bNextVideo) {
            m_bNextVideo = false;
            NextVideo();
        }
        if (m_indexVideo > 3) {
            Dismiss();
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateVideoScreen(Modeler* pmodeler, bool bIntroOnly)
{
    return new VideoScreen(pmodeler, bIntroOnly);
}
