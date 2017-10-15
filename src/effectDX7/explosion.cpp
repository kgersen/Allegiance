#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// ExplosionGeo
//
//////////////////////////////////////////////////////////////////////////////

class ExplosionGeoImpl :
    public ExplosionGeo
{
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class ExplosionData : IObject {
    public:
        TRef<AnimatedImage> m_pimage;
        Vector              m_position;
        Vector              m_dposition;
        float               m_angle;
        float               m_timeStart;
        float               m_scale;
    };

    class ShockwaveData : IObject {
    public:
        TRef<Image>         m_pimageShockwave;
        Vector              m_position;
        Vector              m_dposition;
        Vector              m_forward;
        Vector              m_right;
        Color               m_color;
        float               m_timeStart;
        float               m_scale;
    };

    typedef TList<ExplosionData> ExplosionDataList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TList<ShockwaveData>       m_listShockwave;
    TVector<ExplosionDataList> m_vlistExplosion;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value Members
    //
    //////////////////////////////////////////////////////////////////////////////

    Number* GetTime() { return Number::Cast(GetChild(0)); }

public:
    ExplosionGeoImpl(Number* ptime) :
        ExplosionGeo(ptime),
        m_vlistExplosion(24)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddExplosion(
        const Vector&                 position,
        const Vector&                 forward,
        const Vector&                 right,
        const Vector&                 dposition,
		float                         radiusExplosion,
        float                         radiusShockWave,
        const Color&                  color,
        int                           countDecals,
        TVector<TRef<AnimatedImage> > vpimage,
        Image*                        pimageShockwave
    ) {
        //
        // Add the shockwave
        //

		if (pimageShockwave != NULL) {
			m_listShockwave.PushFront();
			ShockwaveData& sdata = m_listShockwave.GetFront();

			sdata.m_timeStart       = GetTime()->GetValue();
			sdata.m_pimageShockwave = pimageShockwave;
			sdata.m_color           = color;
			sdata.m_position        = position;
			sdata.m_dposition       = dposition;
			sdata.m_scale           = radiusShockWave;
			sdata.m_forward         = forward;
			sdata.m_right           = right;
		}

        //
        // Add the little explosions
        //

        int countImage = vpimage.GetCount();
        int indexImage = 0;

        for (int index = 0; index < countDecals; index++) {
            ExplosionDataList& list  = m_vlistExplosion.Get(index);
            list.PushFront();
            ExplosionData&     edata = list.GetFront();

            edata.m_timeStart = GetTime()->GetValue() + index * 0.25f;
            edata.m_pimage    = vpimage[indexImage];
            edata.m_position  = position + Vector::RandomPosition(radiusExplosion * 0.5f);
            edata.m_dposition = dposition;
            edata.m_angle     = random(0, 2 * pi);
            edata.m_scale     = radiusExplosion;

            indexImage++;
            if (indexImage >= countImage) {
                indexImage = 0;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "ExplosionGeo"; }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Geometry Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    float RenderShockwaves(Context* pcontext)
    {
        float fill = 0;

        TList<ShockwaveData>::Iterator iter(m_listShockwave);

        while (!iter.End()) {
            ShockwaveData& sdata = iter.Value();

            float time   = GetTime()->GetValue() - sdata.m_timeStart;
            float bright = 1.0f - time * time;

            if (bright <= 0) {
                iter.Remove();
            } else {
                float scale = time * sdata.m_scale;

                fill +=
                    pcontext->DrawDecal(
                        sdata.m_pimageShockwave->GetSurface(),
                        bright * sdata.m_color,
                        sdata.m_position + time * sdata.m_dposition,
                        scale * sdata.m_forward,
                        scale * sdata.m_right,
                        0,
                        0
                    );

                iter.Next();
            }
        };

        return fill;
    }

    float RenderExplosions(
        Context*           pcontext,
        ExplosionDataList& list
    ) {
        float fill = 0;

        ExplosionDataList::Iterator iter(list);

        while (!iter.End()) {
            ExplosionData& edata = iter.Value();

            float time = GetTime()->GetValue() - edata.m_timeStart;
            if (time >= 0) {
                int frame = (int)(time * 20.0f);

                if (frame >= edata.m_pimage->GetCount()) {
                    iter.Remove();
                    continue;
                } else {
                    fill +=
                        pcontext->DrawDecal(
                            edata.m_pimage->GetSurface(frame),
                            Color::White(),
                            edata.m_position + time * edata.m_dposition,
                            Vector::GetZero(),
                            Vector::GetZero(),
                            edata.m_scale,
                            edata.m_angle
                        );
                }
            }  

            iter.Next();
        }

        return fill;
    }

    void Render(Context* pcontext)
    {
        float fill = 0;
        
        fill += RenderShockwaves(pcontext);

        int count = m_vlistExplosion.GetCount();

        for (int index = 0; index < count; index++) {
            fill += RenderExplosions(pcontext, m_vlistExplosion.Get(index));

            //
            // If we have passed the fill limit throw away the rest of the explosions
            //

            if (fill > 2.0f) {
                for (int index2 = index + 1; index2 < count; index2++) {
                    m_vlistExplosion.Get(index2).SetEmpty();
                }
                return;
            }
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ExplosionGeo Factory
//
//////////////////////////////////////////////////////////////////////////////

TRef<ExplosionGeo> CreateExplosionGeo(Number* ptime) 
{
    return new ExplosionGeoImpl(ptime);
}
