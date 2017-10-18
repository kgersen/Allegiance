#include "pch.h"
#include "mmstream.h"
#include "amstream.h"
#include <ddstream.h>

//////////////////////////////////////////////////////////////////////////////
//
// VideoImage
//
//////////////////////////////////////////////////////////////////////////////

class VideoImageImpl : public VideoImage {
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    //////////////////////////////////////////////////////////////////////////////

    RectValue* GetRect() { return RectValue::Cast(GetChild(0)); }

    TRef<EventSourceImpl>         m_peventSource;
    TRef<PrivateEngine>           m_pengine;
    TRef<IAMMultiMediaStream>     m_pAMStream;
    TRef<IDirectDrawStreamSample> m_psample;
    TRef<IDirectDrawSurfaceX>     m_pddsSample;
    TRef<PrivateSurface>          m_psurface;
    ZString                       m_strFile;
    bool                          m_bTriggered;
    bool                          m_bFirstFrame;
    bool                          m_bStarted;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    VideoImageImpl(
        PrivateEngine* pengine,
        RectValue*     prect, 
        const ZString& str
    ) :
        VideoImage(prect),
        m_pengine(pengine),
        m_peventSource(new EventSourceImpl()),
        m_strFile(str),
        m_bStarted(false),
        m_bTriggered(false),
        m_bFirstFrame(true)
    {
        //
        // Create the Active Movie Multi Media Stream
        //


        HRESULT hr = CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER, IID_IAMMultiMediaStream, (void **)&m_pAMStream);
            
        if (FAILED(hr) || m_pAMStream == NULL) {
            m_pAMStream = NULL;
            return;
        }

        ZSucceeded(m_pAMStream->Initialize(STREAMTYPE_READ, 0, NULL));

        //
        // Add renders for sound and video
        //

        DDDevice* pdddevice = m_pengine->GetPrimaryDevice();
        ZSucceeded(m_pAMStream->AddMediaStream(pdddevice->GetDD(), &MSPID_PrimaryVideo, 0, NULL));

        //
        // Don't check the return value.  If this fails there just won't be audio playback.
        //

        m_pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL);

        //
        // Open the AVI File
        //

        WCHAR wPath[MAX_PATH];
        MultiByteToWideChar(CP_ACP, 0, m_strFile, -1, wPath, ArrayCount(wPath));

        if (FAILED(m_pAMStream->OpenFile(wPath, 0))) {
            m_pAMStream = NULL;
        }
    }

    bool IsValid()
    {
        return m_pAMStream != NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void AllocateSample()
    {
        if (m_pAMStream) {
            //
            // Create a Direct Draw video steam
            //

            TRef<IMediaStream>            pPrimaryVidStream;
            TRef<IDirectDrawMediaStream>  pDDStream;
            TRef<IDirectDrawSurface>      pdds;

            ZSucceeded(m_pAMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream));
            ZSucceeded(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream));

            //
            // Get Information about the stream
            //

            DDSDescription      ddsdCurrent;
            DDSDescription      ddsdDesired;
            IDirectDrawPalette* pddpal;
            DWORD               flags;

            HRESULT hr =
                pDDStream->GetFormat(
                    (DDSURFACEDESC*)&ddsdCurrent, 
                    &pddpal, 
                    (DDSURFACEDESC*)&ddsdDesired, 
                    &flags
                );

            //
            // !!! sometime the size will come back zero, in that case just don't display anything
            //

            if ( 
                   FAILED(hr)
                || ddsdDesired.XSize() == 0
                || ddsdDesired.YSize() == 0
            ) {
                End();
                return;
            }

            //
            // Create a surface that the video will be rendered to
            //

            CastTo(
                m_psurface, 
                m_pengine->CreateSurface(
                    ddsdDesired.Size(),
                    SurfaceType2D()
                )
            );

            DDSurface* pddsurface; CastTo(pddsurface, m_psurface->GetVideoSurface());

            m_pddsSample = pddsurface->GetDDSX();

            //
            // Create a ddsample from the surface
            //

            ZSucceeded(pDDStream->CreateSample(
                pddsurface->GetDDS(), 
                NULL, 
                DDSFF_PROGRESSIVERENDER, 
                &m_psample
            ));
        }
    }

    void Start()
    {
        //
        // Start playback
        //

        m_bStarted = true;
        ZSucceeded(m_pAMStream->SetState(STREAMSTATE_RUN));
    }

    void End()
    {
        if (!m_bTriggered) {
            m_bTriggered = true;
            m_peventSource->Trigger();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // DeviceDependant methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClearDevice()
    {
        //
        // no need to free the stream since we always use the primary device
        //
        // m_pAMStream  = NULL;

        m_pddsSample = NULL;
        m_psample    = NULL;  
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // VideoImage methods
    //
    //////////////////////////////////////////////////////////////////////////////

    IEventSource* GetEventSource()
    {
        return m_peventSource;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void CalcBounds()
    {
        m_bounds.SetRect(GetRect()->GetValue());
    }

    void Render(Context* pcontextArg)
    {
        if (m_bFirstFrame) {
            m_bFirstFrame = false;
            return;
        }

        if (m_psample == NULL) {
            AllocateSample();
        }

        if (m_psample) {
            if (!m_bStarted) {
                Start();
            }

            bool bTrigger = (m_psample->Update(0, NULL, NULL, 0) != S_OK);

            PrivateContext* pcontext; CastTo(pcontext, pcontextArg);
            WinRect rect = WinRect::Cast(GetRect()->GetValue());

            WinPoint size = m_psurface->GetSize();

            if (
                   size.X() <= rect.XSize() / 2 
                && size.Y() <= rect.YSize() / 2
            ) {
                //
                // Double size sample will fit on screen so double it
                //

                WinPoint offset = rect.Min() + (rect.Size() - size * 2) / 2;

                pcontext->GetSurface()->BitBlt(
                    WinRect(
                        offset,
                        offset + size * 2
                    ),
                    m_psurface
                );
            } else {
                //
                // Center the sample
                //

                pcontext->GetSurface()->BitBlt(
                    rect.Min() + (rect.Size() - size) / 2,
                    m_psurface
                );
            }

            if (bTrigger) {
                End();
            }
        } else {
            End();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "VideoImage"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

TRef<VideoImage> CreateVideoImage(
    Engine*        pengine,
    RectValue*     prect, 
    const ZString& str
) {
    PrivateEngine* pprivateEngine; CastTo(pprivateEngine, pengine);

    return new VideoImageImpl(pprivateEngine, prect, str);
}
