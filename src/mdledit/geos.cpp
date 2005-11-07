//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class StripGeo : public WrapGeo {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class StripData {
    public:
        TRef<Geo>                   m_pgeo;
        TRef<ModifiableVectorValue> m_ppos;
        TRef<ModifiableVectorValue> m_pright;
        float                       m_dangle1;
        float                       m_dangle2;
        float                       m_dangle3;
        float                       m_timeStart;

        void Initialize(Number* ptime)
        {
            m_timeStart = ptime->GetValue();
            m_ppos   = new ModifiableVectorValue(Vector(0, 0, 1));
            m_pright = new ModifiableVectorValue(Vector(0.25, 0, 0));

            Color color;

            color.SetHSBA(random(0, 1), 1.0f, 0.1f);

            m_pgeo   =
                CreateTrailGeo(
                    color,
                    m_ppos,
                    m_pright,
                    ptime
                );

            m_dangle1 = random( 1.75f, 2.25f);
            m_dangle2 = random(-0.25f, 0.25f);
            m_dangle3 = random(-0.25f, 0.25f);
        }

        void UpdatePosition(float timeNow)
        {
            float time = timeNow - m_timeStart;

            Matrix mat;

            mat.SetRotateX(m_dangle1 * time);
            mat.Rotate(Vector(0, 1, 0), m_dangle2 * time);
            mat.Rotate(Vector(0, 0, 1), m_dangle3 * time);

            Vector pos   = mat.Transform(Vector(0, 0, 1));
            Vector right =
                CrossProduct(
                    pos,
                    pos - m_ppos->GetValue()
                ).Normalize();

            m_ppos->SetValue(pos);
            m_pright->SetValue(right * 0.25f);
        }
    };

    typedef TList<StripData> StripList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    StripList      m_listStrips;
    TRef<GroupGeo> m_pgroup;

    Number* GetTime() { return Number::Cast(GetChild(1)); }

public:
    StripGeo(Modeler* pmodeler, Number* ptime) :
        WrapGeo(Geo::GetEmpty(), ptime)
    {
        m_pgroup = GroupGeo::Create();
        SetGeo(m_pgroup);

        for (int index = 0; index < 8; index++) {
            AddStrip();
        }
    }

    void AddStrip()
    {
        m_listStrips.PushFront();
        m_listStrips.GetFront().Initialize(GetTime());
        m_pgroup->AddGeo(m_listStrips.GetFront().m_pgeo);
    }

    void Evaluate()
    {
        float time = GetTime()->GetValue();

        StripList::Iterator iter(m_listStrips);

        while (!iter.End()) {
            iter.Value().UpdatePosition(time);
            iter.Next();
        }
    }
};

TRef<StripGeo> CreateStripGeo(Modeler* pmodeler, Number* ptime)
{
    return new StripGeo(pmodeler, ptime);
}
