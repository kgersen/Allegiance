#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// BSP Geo
//
//////////////////////////////////////////////////////////////////////////////

class BSPGeo : public Geo {
    //////////////////////////////////////////////////////////////////////////////
    //
    // BSP Tree Nodes
    //
    //////////////////////////////////////////////////////////////////////////////

    class Node : public IObject {
    public:
        virtual void Render(MeshIndex*& pindex, HVector& vec) = 0;
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Split Node
    //
    //////////////////////////////////////////////////////////////////////////////

    class SplitNode : public Node {
    private:
        Plane      m_plane;
        TRef<Node> m_pfront;
        TRef<Node> m_pback;

    public:
        SplitNode(
            const Plane& plane,
            Node* pfront,
            Node* pback
        ) :
            m_plane(plane),
            m_pfront(pfront),
            m_pback(pback)
        {
        }

        void Render(MeshIndex*& pindex, HVector& vec)
        {
            if (m_plane.Distance(vec) >= 0) {
                m_pback->Render(pindex, vec);
                m_pfront->Render(pindex, vec);
            } else {
                m_pfront->Render(pindex, vec);
                m_pback->Render(pindex, vec);
            }
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Leaf Node
    //
    //////////////////////////////////////////////////////////////////////////////

    class LeafNode : public Node {
    private:
        MeshIndex m_index0;
        MeshIndex m_index1;
        MeshIndex m_index2;

    public:
        LeafNode(
            MeshIndex index0,
            MeshIndex index1,
            MeshIndex index2
        ) :
            m_index0(index0),
            m_index1(index1),
            m_index2(index2)
        {
        }

        void Render(MeshIndex*& pindex, HVector& vec)
        {
            pindex[0] = m_index0;
            pindex[1] = m_index1;
            pindex[2] = m_index2;
            pindex += 3;
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    //////////////////////////////////////////////////////////////////////////////

    TVector<VertexL>   m_vvertex;
    TVector<MeshIndex> m_vindex;
    TRef<Node>         m_pnode;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

        /*
        Color color1(1, 0, 0, 0.75f);
        Color color2(0, 0, 1, 0.75f);

        m_vvertex.SetCount(8);

        m_vvertex.Set(0, VertexL(Vector(-1, -1,  1), color1));
        m_vvertex.Set(1, VertexL(Vector( 1, -1,  1), color1));
        m_vvertex.Set(2, VertexL(Vector( 1,  1,  1), color1));
        m_vvertex.Set(3, VertexL(Vector(-1,  1,  1), color1));
        m_vvertex.Set(4, VertexL(Vector(-1, -1, -1), color2));
        m_vvertex.Set(5, VertexL(Vector( 1, -1, -1), color2));
        m_vvertex.Set(6, VertexL(Vector( 1,  1, -1), color2));
        m_vvertex.Set(7, VertexL(Vector(-1,  1, -1), color2));

        m_vindex.SetCount(6);
        m_pnode =
            new SplitNode(
                Plane(
                    Vector(0, 0, 1), 
                    Vector(0, 0, 0)
                ),
                new LeafNode(0, 1, 2),
                new LeafNode(4, 6, 5)
            );
        */

    BSPGeo()  :
        Geo()
    {
    }

    Node* CreateSphereSlice()
    {
    }

    Node* CreateSphereLayer(int layerStart, int layerEnd, int slices)
    {
        if (layerEnd == layerStart + 1) {
            return CreateSphereSlice();
        } else {
            return NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Geo Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    float GetRadius(const Matrix& mat)
    {
        float radius = 0;
        int count = m_vvertex.GetCount();
        for(int index = 0; index < count; index++) {
            Vector vec = mat.Transform(m_vvertex[index].GetPosition());
            radius = max(radius, vec.Length());
        }

        return radius;
    }

    void Render(Context* pcontext)
    {
        MeshIndex* pindex = (MeshIndex*)&(m_vindex[0]);

        m_pnode->Render(
            pindex, 
            HVector(
                pcontext->TransformEyeToLocal(Vector(0, 0, 0))
            )
        );
        pcontext->SetBlendMode(BlendModeSourceAlpha);
        pcontext->SetCullMode(CullModeNone);
        pcontext->DrawTriangles(m_vvertex, m_vindex);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> CreateBSPGeo()
{
    return new BSPGeo();
}
