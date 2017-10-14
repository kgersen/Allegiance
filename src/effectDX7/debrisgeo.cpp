#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// DebrisGeo
//
//////////////////////////////////////////////////////////////////////////////

const float maxDistance = 100;

class DebrisGeo : public Geo {
private:
    class DebrisData {
    public:
        Vector m_position;
        Color  m_color;
        float  m_startTime;
    };

    typedef TList<DebrisData> DebrisDataList;

    DebrisDataList m_listData;
    TRef<Surface>  m_psurface;
    float          m_time;
    Vector         m_positionLast;

    Number*    GetTime()     { return Number::Cast(GetChild(0));    }
    Viewport*  GetViewport() { return Viewport::Cast(GetChild(1));  }
    Camera*    GetCamera()   { return GetViewport()->GetCamera();   }
    RectValue* GetViewRect() { return GetViewport()->GetViewRect(); }


public:
    DebrisGeo(Modeler* pmodeler, Number* ptime, Viewport* pviewport) :
        Geo(ptime, pviewport)
    {   
        m_positionLast = GetCamera()->GetPosition();
        m_psurface = pmodeler->LoadSurface("debris1bmp", true);
    }

    void Evaluate()
    {
        const Rect&       rect     = GetViewRect()->GetValue();
              Camera*     pcamera  = GetCamera();
              float       focus    = pcamera->GetFocus();
              float       rfocus   = 1.0f / focus;
        const Vector&     position = pcamera->GetPosition();
              Orientation orient   = pcamera->GetOrientation();
              Vector      forward  = orient.GetForward();
              Vector      up       = orient.GetUp();
              Vector      right    = orient.GetRight();

        //
        // save the current time
        //

        m_time = GetTime()->GetValue();

        //
        // how far did we move?
        //

        float length = (m_positionLast - position).Length();

        if (length > 100) {
            //
            // we jumped clear everything
            //
            
            m_listData.SetEmpty();
            length = 0;
        } else {
            //
            // remove any debris that are too far away from the ship
            //

            DebrisDataList::Iterator iter(m_listData);

            while (!iter.End()) {
                DebrisData& data = iter.Value();

                float time     = m_time - data.m_startTime;
                float bright   = 1.0f - 0.25f * time * time;
                float distance = (data.m_position - position).LengthSquared();

                if (bright <= 0 || distance > 250000) {
                    iter.Remove();
                } else {
                    iter.Next();
                }
            }

            //
            // if we are moving add new debris
            //

            while (length > 0) {
                m_listData.PushFront();
                DebrisData& data = m_listData.GetFront();

                float angle    = random(0, 2 * pi);
                float distance = maxDistance * sqrt(random(0, 1));
                float radius   = rfocus * distance * sqrt(random(0, 1));
                float c        = radius * cos(angle);
                float s        = radius * sin(angle);

                data.m_position  = 
                      position 
                    + distance * forward
                    + c * right
                    + s * up;

                data.m_color     = 
                    Color(
                        random(0.50f, 0.75f),
                        random(0.75f, 1.00f),
                        random(0.75f, 1.00f)
                    );
                data.m_startTime = m_time;
                length -= 1.0f;
            }
        }

        m_positionLast = position;
    }

    void Render(Context* pcontext)
    {
        const Vector& position = GetCamera()->GetPosition();
        float scale = 0.1f / GetCamera()->GetFocus();

        DebrisDataList::Iterator iter(m_listData);

        while (!iter.End()) {
            DebrisData& data = iter.Value();

            float time     = m_time - data.m_startTime;
            float bright   = 1.0f - 0.25f * time * time;
            float distance = (data.m_position - position).LengthSquared();

            if (distance > 400) {
                pcontext->DrawDecal(
                    m_psurface,
                    bright * data.m_color,
                    data.m_position,
                    Vector::GetZero(),
                    Vector::GetZero(),
                    scale,
                    0
                );
            }

            iter.Next();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetFunctionName() { return "DebrisGeo"; }
};

TRef<Geo> CreateDebrisGeo(Modeler* pmodeler, Number* ptime, Viewport* pviewport)
{
    return new DebrisGeo(pmodeler, ptime, pviewport);
}
