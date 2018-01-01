#include "frameimage.h"

#include <base.h>
#include <event.h>

#include "context.h"
#include "enginep.h"
#include "image.h"
#include "model.h"

//////////////////////////////////////////////////////////////////////////////
//
// FrameImage
//
//////////////////////////////////////////////////////////////////////////////

class FrameImage : public Image {
private:
    TRef<Image>   m_pimageBackground;
    TRef<Surface> * m_ppsurface;
    TRef<IObject> m_pobjectMemory;
    int           m_nFrame;
    int           m_frameCurrent;
    DWORD*        m_pdwOffsets;
    BYTE*         m_prle;

public:
    FrameImage(
        Image*   pimageBackground,
        Number*  pframe,
        int      nFrames,
        DWORD*   pdwOffsets,
        BYTE*    prle,
        IObject* pobjectMemory
    ) :
        Image(pframe, pimageBackground),
        m_pimageBackground(pimageBackground),
        m_nFrame(nFrames),
        m_pdwOffsets(pdwOffsets),
        m_prle(prle),
        m_pobjectMemory(pobjectMemory)
    {
		int i;
		BYTE * pSrcCopy;
		HRESULT hResult;
		D3DLOCKED_RECT lockRect;

        TRef<Surface> psurfaceBackground = m_pimageBackground->GetSurface();
		PrivateSurface* pprivateSurface; CastTo(pprivateSurface, psurfaceBackground );

		pSrcCopy = new BYTE[pprivateSurface->GetPixelFormat()->PixelBytes() * 
							pprivateSurface->GetSize().x * pprivateSurface->GetSize().y];
		m_ppsurface = new TRef<Surface> [ m_nFrame ]; //Fix memory leak -Imago 8/2/09

		for( i=0; i<m_nFrame; i++ )
		{
			// Create a texture for each frame.
			m_ppsurface[i] = psurfaceBackground->CreateCompatibleSurface(
								psurfaceBackground->GetSize(),
								SurfaceType2D() );

			m_ppsurface[i]->BitBlt( WinPoint(0, 0), psurfaceBackground, true );

			// On first pass, grab out a copy of the source data.
			if( i == 0 )
			{
				int y;
				PrivateSurface* pFrame0Surface; CastTo(pFrame0Surface, m_ppsurface[i]);
				TEXHANDLE hTexture = pFrame0Surface->GetTexHandle( );

				hResult = CVRAMManager::Get()->LockTexture( hTexture, &lockRect );
                ZAssert( hResult == D3D_OK );

				// Copy the original texture frame data into our temp buffer.
				BYTE * pSrc = (BYTE*) lockRect.pBits;
				int iPixelSize = pFrame0Surface->GetPixelFormat()->PixelBytes();
				for( y=0; y<pFrame0Surface->GetSize().y; y++ )
				{
					memcpy( &pSrcCopy[ y * ( pFrame0Surface->GetSize().x * iPixelSize ) ], 
							&pSrc[ y * lockRect.Pitch ],
							pFrame0Surface->GetSize().x * iPixelSize );
				}
				CVRAMManager::Get()->UnlockTexture( hTexture );
			}

			// Extract this animation frame.
			ExtractRLEFrame( i, m_ppsurface[i], pSrcCopy );
		}

		// Tidy up.
		delete pSrcCopy;

		// Reset vars.
        m_frameCurrent = 0;
        m_bounds.SetRect( Rect( Point(0, 0), Point::Cast( psurfaceBackground->GetSize()) ));
    }

    ~FrameImage()
    {
        if (m_pobjectMemory == NULL) {
            delete[] m_pdwOffsets;
            delete[] m_prle;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // FrameImage methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Number* GetFrameNumber() { return Number::Cast(GetChild(0)); }

    int GetFrame()
    {
        return
            bound(
                (int)(GetFrameNumber()->GetValue() * m_nFrame),
                0,
                m_nFrame - 1
            );
    }

	void ExtractRLEFrame( int iFrame, TRef<Surface> pSurface, BYTE * pSrc )
	{
		int i, x, y;
		D3DLOCKED_RECT lockRect;
		HRESULT hResult;

		// Inflate each frame into a texture.
		PrivateSurface * pprivateSurface; CastTo(pprivateSurface, pSurface );
		TEXHANDLE hTexture = pprivateSurface->GetTexHandle( );

		// Copy RLE data into texture.
		hResult = CVRAMManager::Get()->LockTexture( hTexture, &lockRect );
        ZAssert( hResult == D3D_OK );

		// Inflate RLE data into temp buffer.
		BYTE * pDest = (BYTE*) lockRect.pBits;
		int iPixelSize = pprivateSurface->GetPixelFormat()->PixelBytes();

		// Copy original buffer in first.
		BYTE * pTemp = new BYTE[pprivateSurface->GetPixelFormat()->PixelBytes() * 
								pprivateSurface->GetSize().x * pprivateSurface->GetSize().y];
		memcpy( pTemp, pSrc, pprivateSurface->GetPixelFormat()->PixelBytes() * pprivateSurface->GetSize().x * pprivateSurface->GetSize().y );

		// Update the buffer with RLE data.
		i = -1;
		while( i < iFrame )
		{
			i++;
			PlayRLE(	pTemp, 
						m_prle + m_pdwOffsets[ i ],
						m_prle + m_pdwOffsets[ i + 1 ] );
		}

		// Copy updated frame into texture.
		pDest = (BYTE*) lockRect.pBits;
		iPixelSize = pprivateSurface->GetPixelFormat()->PixelBytes();
		for( y=0; y<pprivateSurface->GetSize().y; y++ )
		{
			for( x=0; x<pprivateSurface->GetSize().x; x++ )
			{
				memcpy( pDest + ( lockRect.Pitch * y ) + ( x * iPixelSize ),
						&pTemp[ ( y * ( pprivateSurface->GetSize().x * iPixelSize ) ) + ( x * iPixelSize ) ],
						iPixelSize );
			}
		}

		// Unlock the texture and tidy up.
		CVRAMManager::Get()->UnlockTexture( hTexture );
		delete pTemp;
	}

    void PlayRLE(BYTE* pd, BYTE* prle, BYTE* pend)
    {
        while (prle < pend) {
            WORD word  = *(WORD*)prle;
            int length = word & RLELengthMask;
            prle += 2;

            switch (word & RLEMask) {
                case RLEMaskFill:
                    {
                        for (int index = length; index > 0; index --) {
                            (*(BYTE*)pd) = (*(BYTE*)pd) ^ (*(BYTE*)prle);
                            pd   += 1;
                            prle += 1;
                        }
                    }
                    break;

                case RLEMaskBYTE:
                    {
                        BYTE byte = *(BYTE*)prle;
                        prle += 1;

                        for (int index = length; index > 0; index --) {
                            (*(BYTE*)pd) = (*(BYTE*)pd) ^ byte;
                            pd += 1;
                        }
                    }
                    break;

                case RLEMaskWORD:
                    {
                        WORD word = *(WORD*)prle;
                        prle += 2;

                        for (int index = length; index > 0; index --) {
                            (*(WORD*)pd) = (*(WORD*)pd) ^ word;
                            pd += 2;
                        }
                    }
                    break;

                case RLEMaskDWORD:
                    {
                        DWORD dword = *(DWORD*)prle;
                        prle += 4;

                        for (int index = length; index > 0; index --) {
                            (*(DWORD*)pd) = (*(DWORD*)pd) ^ dword;
                            pd += 4;
                        }
                    }
                    break;
            }
        }
    }

    void UpdateFrame()
    {
		m_frameCurrent = GetFrame();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Image Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> GetSurface()
    {
        UpdateFrame();
        return m_ppsurface[m_frameCurrent];
    }

    void Render(Context* pcontext)
    {
        UpdateFrame();
        pcontext->DrawImage( m_ppsurface[m_frameCurrent] );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName()
    {
        return "FrameImage";
    }

    void Write(IMDLBinaryFile* pmdlFile)
    {
        WriteChildren(pmdlFile);
        pmdlFile->WriteReference("FrameImage");
        TRef<ZFile> pfile = pmdlFile->WriteBinary();

        pfile->Write((void*)&m_nFrame,    sizeof(m_nFrame)          );
        pfile->Write((void*)m_pdwOffsets, sizeof(DWORD) * (m_nFrame));
        pfile->Write((void*)m_prle,       m_pdwOffsets[m_nFrame - 1]);
        pfile->WritePad(m_pdwOffsets[m_nFrame - 1]);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Factory
//
//////////////////////////////////////////////////////////////////////////////

class FrameImageFactory : public IFunction {
public:
    FrameImageFactory()
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        ZUnimplemented();
        return NULL;
    }

    TRef<IObject> Read(IBinaryReaderSite* psite, ObjectStack& stack)
    {
        Number* pframe = Number::Cast((IObject*)stack.Pop());
        Image*  pimage =  Image::Cast((Value*)(IObject*)stack.Pop());

        DWORD  nFrame     = psite->GetDWORD();
        DWORD* pdwOffsets = (DWORD*)psite->GetPointer();
        BYTE*  prle       = (BYTE*)(pdwOffsets + nFrame);
        psite->MovePointer(
              sizeof(DWORD) * nFrame
            + pdwOffsets[nFrame - 1]
        );

        return 
            (Value*)new FrameImage(
                pimage, 
                pframe, 
                nFrame, 
                pdwOffsets, 
                prle, 
                psite->GetMemoryObject()
            );
    }
};

TRef<IFunction> CreateFrameImageFactory()
{
    return new FrameImageFactory();
}


//////////////////////////////////////////////////////////////////////////////
//
// FrameImageButtonPane
//
//////////////////////////////////////////////////////////////////////////////

class FrameImageButtonPaneImpl :
    public FrameImageButtonPane,
    public Value
{
    TRef<ModifiableNumber> m_pframe;
    TRef<Image>            m_pimage;
    TRef<EventSourceImpl>  m_peventSource;
    TRef<EventSourceImpl>  m_peventMouseEnterSource;
    TRef<EventSourceImpl>  m_peventMouseLeaveSource;
    bool                   m_bInside;
    float                  m_duration;
    float                  m_valueStart;
    float                  m_timeStart;
    float                  m_loop;
    float                  m_finish;

public:
    FrameImageButtonPaneImpl(
        Number*           ptime,
        Image*            pimage,
        ModifiableNumber* pframe,
        float             duration,
        bool              loop,
        bool              finish
    ) :
        Value(ptime),
        m_peventSource(new EventSourceImpl()),
        m_peventMouseEnterSource(new EventSourceImpl()),
        m_peventMouseLeaveSource(new EventSourceImpl()),
        m_pimage(pimage),
        m_pframe(pframe),
        m_duration(duration),
        m_loop(loop),
        m_finish(finish),
        m_bInside(false),
        m_valueStart(0),
        m_timeStart(ptime->GetValue())
    {
        InternalSetSize(
            WinPoint::Cast(m_pimage->GetBounds().GetRect().Size())
        );
    }

    Number* GetTime() { return Number::Cast(GetChild(0)); }

    //////////////////////////////////////////////////////////////////////////////
    //
    // FrameImageButtonPane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    IEventSource* GetEventSource()
    {
        return m_peventSource;
    }

    IEventSource* GetMouseEnterEventSource()
    {
        return m_peventMouseEnterSource;
    }

    IEventSource* GetMouseLeaveEventSource()
    {
        return m_peventMouseLeaveSource;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void ChildChanged(Value* pvalue, Value* pvalueNew)
    {
        Value::ChildChanged(pvalue, pvalueNew);

        NeedPaint();
    }

    void Evaluate()
    {
        float time  = GetTime()->GetValue();
        float value;
        float delta = (time - m_timeStart) / m_duration;

        if (m_bInside) {
            value = m_valueStart + delta;

            if (m_loop)
            {
                // wrap the value back around to the beginning
                value = float(value - (int)value);
            }
        } else {
            if (m_finish)
            {
                value = m_valueStart + delta;

                if (value > 1.0)
                    value = 0.0;
            }
            else if (m_loop)
            {
                value = m_valueStart;
            }
            else
            {
                value = m_valueStart - delta;
            }
        }

        value = bound(value, 0.0f, 1.0f);

        if (value != m_pframe->GetValue()) {
            m_pframe->SetValue(value);
            NeedPaint();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void Paint(Surface* psurface)
    {
        Update();
        psurface->BitBlt(WinPoint(0, 0), m_pimage->GetSurface());
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        m_bInside    = true;
        m_valueStart = m_pframe->GetValue();
        m_timeStart  = GetTime()->GetValue();
        m_peventMouseEnterSource->Trigger();
    }

    void MouseLeave(IInputProvider* pprovider)
    {
        m_bInside    = false;
        m_valueStart = m_pframe->GetValue();
        m_timeStart  = GetTime()->GetValue();
        m_peventMouseLeaveSource->Trigger();
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        if (button == 0) {
            if (bDown) {
                m_peventSource->Trigger();
            }
        }

        return MouseResult();
    }
};

TRef<FrameImageButtonPane> CreateFrameImageButtonPane(
    Number*           ptime,
    Image*            pimage,
    ModifiableNumber* pframe,
    float             duration,
    bool              loop,
    bool              finish
) {
    return new FrameImageButtonPaneImpl(ptime, pimage, pframe, duration, loop, finish);
}
