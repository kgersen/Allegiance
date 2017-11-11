#include <event.h>
#include <geometry.h>
#include <model.h>
#include <surface.h>

#include "efart.h"

//////////////////////////////////////////////////////////////////////////////
//
// AlephGeo
//
//////////////////////////////////////////////////////////////////////////////

class AlephGeo : public Geo, public IGeoCallback, public TEvent<float>::Sink {
private:
    int              m_countDepth;
    int              m_countSection;
    TVector<VertexL> m_vertices;
    TVector<WORD>    m_indices;
    TRef<Surface>    m_psurfaceTexture;
    TRef<Image>      m_pimageStar;
    float            m_time;
    float            m_fExplodeTime;
    float            m_fExplodeDuration;
    TRef<TEvent<float>::Sink>   m_peventSinkDelegate;
    TRef<TEvent<float>::Source>  m_pevent;

    Number* GetTime() { return Number::Cast(GetChild(0)); }

public:
    //
    // Constructor
    //

    AlephGeo(Modeler* pmodeler, Number* ptime, TEvent<float>::Source* pevent, int countDepth, int countSection) :
        Geo(ptime),
        m_countDepth(countDepth),
        m_countSection(countSection),
        m_vertices(
            (m_countDepth + 1) * (m_countSection + 1),
            (m_countDepth + 1) * (m_countSection + 1)
        ),
        m_indices(
            m_countDepth * m_countSection * 6,
            m_countDepth * m_countSection * 6
        ),
        m_fExplodeTime (-1.0f),
        m_pevent(pevent)
    {
        m_peventSinkDelegate = CreateDelegate (this);
        pevent->AddSink(m_peventSinkDelegate);

        m_pimageStar = pmodeler->LoadImage(AWF_EFFECT_ALEPH_STAR, true);

        InitializeVertices();
        InitializeIndices();
    }

    ~AlephGeo()
    {
        m_pevent->RemoveSink(m_peventSinkDelegate);
    }

    //
    // Members
    //

    bool OnEvent(TEvent<float>::Source* pevent, float fExplodeTime)
    {
        m_fExplodeDuration = fExplodeTime;
        m_fExplodeTime = m_fExplodeDuration + m_time;

        return true;
    }

    void InitializeVertices()
    {
        for(int indexDepth = 0; indexDepth < m_countDepth + 1; indexDepth++) {
            int    index = indexDepth * (m_countSection + 1);
            float radius = (float)indexDepth / m_countDepth;
            float      z = (1 - radius) * (radius - 1); //-pow(nradius, 4);
            float bright = 2.0f * (radius * (1.0f - radius));

            for (int indexSection = 0; indexSection < (m_countSection + 1); indexSection++) {
                float angle = 2 * pi * indexSection / m_countSection;

                m_vertices.Set(
                    index + indexSection,
                    VertexL(
                        radius * cos(angle), 
                        radius * sin(angle), 
                        z,
                        bright,
                        bright,
                        bright,
                        1,
                        0,
                        0
                    )
                );
            }
        }
    }

    void InitializeIndices()
    {
        int indexIndex = 0;

        for(int indexDepth = 0; indexDepth < m_countDepth; indexDepth++) {
            int indexVertex = indexDepth * (m_countSection + 1);

            for(int indexSection = 0; indexSection < m_countSection; indexSection++) {
                m_indices.Set(indexIndex + 0, indexVertex + indexSection);
                m_indices.Set(indexIndex + 1, indexVertex + indexSection + (m_countSection + 1));
                m_indices.Set(indexIndex + 2, indexVertex + indexSection + (m_countSection + 1) + 1);
                                            
                m_indices.Set(indexIndex + 3, indexVertex + indexSection);
                m_indices.Set(indexIndex + 4, indexVertex + indexSection + (m_countSection + 1) + 1);
                m_indices.Set(indexIndex + 5, indexVertex + indexSection + 1);

                indexIndex += 6;
            }
        }
    }

    void UpdateTextureCoordinates()
    {
        float time = mod(m_time / 10, 1);

        for(int indexDepth = 0; indexDepth < m_countDepth + 1; indexDepth++) {
            int         index = indexDepth * (m_countSection + 1);
            float       value = (float)indexDepth / m_countDepth;
            float           z = time + value;
            float angleOffset = 5 * (1 - pow(1 - value, 2));

            for (int indexSection = 0; indexSection < (m_countSection + 1); indexSection++) {
                float angle = 4 * (float)indexSection / m_countSection + angleOffset;

                VertexL& vertex = m_vertices.Get(index + indexSection);

                vertex.u = angle;
                vertex.v = z;
            }
        }
    }

    void Evaluate()
    {
        m_time = GetTime()->GetValue();

    }

    float GetRadius(const Matrix& mat)
    {
        return 1;
    }

    void Render(Context* pcontext)
    {
        float   fScale = 0.1f;  // the size of the star on the aleph by default
        float   fExplodeDelta = m_fExplodeTime - m_time; // how long 'til an explosion occurs
        float   fPostExplosionDuration = 0.25f;

        if (fExplodeDelta > -fPostExplosionDuration) // if an explosion is impending or has *just* happened
        {
            float   fMaxSwellSize = 6.0f - fScale;
            if (fExplodeDelta >= 0.0f)
            {
                // an explosion is pending, but hasn't happened yet
                fExplodeDelta = 1.0f - (fExplodeDelta / m_fExplodeDuration);

                // scale the star on the aleph accordingly
                float   fNewScale = (fExplodeDelta * fExplodeDelta * fMaxSwellSize);

                // now add a "resonant pulse" to it
                float   fNumPulses = 8.0f;
                float   fCosine = fabsf (sinf (pi * (fNumPulses + 0.5f) * fExplodeDelta));
                fScale += (fNewScale * fCosine * fCosine) + (fNewScale * 0.8f);
            }
            else
            {
                // an explosion has just happened
                fExplodeDelta = 1.0f + (fExplodeDelta / fPostExplosionDuration);

                // scale the star on the aleph accordingly
                fScale += (fExplodeDelta * fExplodeDelta * fMaxSwellSize);
            }
        }

        pcontext->DrawDecal(
            m_pimageStar->GetSurface(), 
            Color::White(),
            Vector(0, 0, -1),
            Vector(0, 0, 0),
            Vector(0, 0, 0),
            fScale,
            0.25f * m_time
        );

        pcontext->DrawDecal(
            m_pimageStar->GetSurface(), 
            Color::White(),
            Vector(0, 0, -1),
            Vector(0, 0, 0),
            Vector(0, 0, 0),
            fScale,
            -0.375f * m_time
        );

        m_psurfaceTexture = pcontext->GetTexture();
        pcontext->DrawCallbackGeo(this, false);
    }

    void RenderCallback(Context* pcontext)
    {
        UpdateTextureCoordinates();

        pcontext->SetCullMode(CullModeNone);
        pcontext->SetBlendMode(BlendModeAdd);
        pcontext->SetShadeMode(ShadeModeGouraud);
        pcontext->SetZWrite(false);
		pcontext->SetTexture(m_psurfaceTexture);

/*		if( m_psurfaceTexture->HasColorKey() == true )
		{
			CD3DDevice9::SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			CD3DDevice9::SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			CD3DDevice9::SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			CD3DDevice9::SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			CD3DDevice9::SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			CD3DDevice9::SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		}*/

        pcontext->DrawTriangles(
            &m_vertices[0],
            m_vertices.GetCount(),
            &m_indices[0],
            m_indices.GetCount()
        );
    }

    //
    // Value methods
    //

    ZString GetFunctionName() { return "AlephGeo"; }
};

TRef<Geo> CreateAlephGeo(Modeler* pmodeler, TEvent<float>::Source* pevent, Number* ptime)
{
    TRef<LODGeo> plodGeo =
        LODGeo::Create(
            new AlephGeo(pmodeler, ptime, pevent, 8, 32)
        );

    plodGeo->AddGeo(new AlephGeo(pmodeler, ptime, pevent, 4, 16), 64);
    plodGeo->AddGeo(new AlephGeo(pmodeler, ptime, pevent, 2, 16), 32);
    plodGeo->AddGeo(new AlephGeo(pmodeler, ptime, pevent, 2,  8), 16);

    return plodGeo;
}
