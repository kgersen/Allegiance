#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// ConeGeo
//
//////////////////////////////////////////////////////////////////////////////

int g_hSegments = 16;
int g_vSegments = 4;

class ConeGeo : 
    public Geo
{
private:
    TVector<Vertex>    m_vvertex;
    TVector<MeshIndex> m_vindex;
    TRef<Material>     m_pmaterial;
    
    float GetAngle() { return Number::Cast(GetChild(0))->GetValue(); }

public:
    ConeGeo(Number* pangle) :
        Geo(pangle),
        m_vvertex(g_hSegments * g_vSegments + 2),
        m_vindex(
               6 * g_hSegments * (g_vSegments - 1) 
            +  3 * g_hSegments * 2
        )
    {
        InitializeIndices();
    }

    void InitializeVertices()
    {
        m_vvertex.Set(0, Vertex(Vector(0, 0, 1), Vector(0, 0,  1)));
        m_vvertex.Set(1, Vertex(Vector(0, 0, 0), Vector(0, 0, -1)));

        float angle = GetAngle();

        for(int vIndex = 0; vIndex < g_vSegments; vIndex++) {
            float vAngle = angle * ((float)(vIndex + 1) / (float)g_vSegments);
            int    index = vIndex * g_hSegments + 2;
            float radius = sin(vAngle);
            float      z = cos(vAngle);

            for (int hIndex = 0; hIndex < g_hSegments; hIndex++) {
                float hAngle = 2 * pi * hIndex / g_hSegments;

                m_vvertex.Set(
                    index + hIndex,
                    Vertex(
                        Vector(
                            radius * cos(hAngle), 
                            radius * sin(hAngle), 
                            z
                        ),
                        Vector(
                            radius * cos(hAngle), 
                            radius * sin(hAngle), 
                            z
                        )
                    )
                );
            }
        }
    }

    void InitializeIndices()
    {
        int indexIndex = 0;

        //
        // The first band
        //

        {
            int indexVertex = 2;

            for (int hIndex = 0; hIndex < g_hSegments; hIndex++) {
                int delta = 
                      (hIndex == (g_hSegments - 1)) 
                    ? (1 - g_hSegments) 
                    : 1;

                m_vindex.Set(indexIndex + 0, 0);
                m_vindex.Set(indexIndex + 1, indexVertex + hIndex + delta);
                m_vindex.Set(indexIndex + 2, indexVertex + hIndex        );

                indexIndex += 3;
            }
        }

        //
        // The middle bands
        //

        for(int vIndex = 0; vIndex < g_vSegments - 1; vIndex++) {
            int indexVertex = vIndex * g_hSegments + 2;

            for (int hIndex = 0; hIndex < g_hSegments; hIndex++) {
                int delta = 
                      (hIndex == (g_hSegments - 1)) 
                    ? (1 - g_hSegments) 
                    : 1;

                m_vindex.Set(indexIndex + 0, indexVertex + hIndex                      );
                m_vindex.Set(indexIndex + 1, indexVertex + hIndex + g_hSegments + delta);
                m_vindex.Set(indexIndex + 2, indexVertex + hIndex + g_hSegments        );
                                            
                m_vindex.Set(indexIndex + 3, indexVertex + hIndex                      );
                m_vindex.Set(indexIndex + 4, indexVertex + hIndex               + delta);
                m_vindex.Set(indexIndex + 5, indexVertex + hIndex + g_hSegments + delta);

                indexIndex += 6;
            }
        }

        //
        // The last band
        //

        {
            int indexVertex = (g_vSegments - 1) * g_hSegments + 2;

            for (int hIndex = 0; hIndex < g_hSegments; hIndex++) {
                int delta = 
                      (hIndex == (g_hSegments - 1)) 
                    ? (1 - g_hSegments) 
                    : 1;

                m_vindex.Set(indexIndex + 0, 1);
                m_vindex.Set(indexIndex + 1, indexVertex + hIndex        );
                m_vindex.Set(indexIndex + 2, indexVertex + hIndex + delta);

                indexIndex += 3;
            }
        }
    }

    void Evaluate()
    {
        InitializeVertices();
    }

    void Render(Context* pcontext)
    {
        pcontext->DrawTriangles(m_vvertex, m_vindex);
    }
};

TRef<Geo> CreateConeGeo(Number* pangle)
{
    return new ConeGeo(pangle);
}

