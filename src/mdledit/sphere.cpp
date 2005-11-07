//////////////////////////////////////////////////////////////////////////////
//
// Recursively subdivide a triangle mesh to produce a sphere
//
//////////////////////////////////////////////////////////////////////////////

typedef WORD Index;

class Edge {
public:
    Index v0;
    Index v1;

    Edge() {};

    Edge(Index v0In, Index v1In) :
        v0(v0In),
        v1(v1In)
    {
    }

    Edge(const Edge& edge) :
        v0(edge.v0),
        v1(edge.v1)
    {
    }
};

class Triangle {
public:
    Index edge[3];

    Triangle() {};

    Triangle(Index e0, Index e1, Index e2)
    {
        edge[0] = e0;
        edge[1] = e1;
        edge[2] = e2;
    }

    Triangle(const Triangle& triangle)
    {
        edge[0] = triangle.edge[0];
        edge[1] = triangle.edge[1];
        edge[2] = triangle.edge[2];
    }
};

class TriangleMesh : public IObject {
private:
    TVector<Vector>   m_positions;
    TVector<Edge>     m_edges;
    TVector<Triangle> m_triangles;

public:
    TriangleMesh(
        int vcount, Vector*   pvector,
        int ecount, Edge*     pedge,
        int tcount, Triangle* ptriangle
    ) :
        m_positions(vcount, pvector, false),
        m_edges(ecount, pedge, false),
        m_triangles(tcount, ptriangle, false)
    {
    }

    TriangleMesh(
        const TVector<Vector>&   positions,
        const TVector<Edge>&     edges,
        const TVector<Triangle>& triangles
    ) :
        m_positions(positions),
        m_edges(edges),
        m_triangles(triangles)
    {
    }

    static TRef<TriangleMesh> SubdivideSphere(TriangleMesh* pmesh)
    {
        int pcount = pmesh->m_positions.GetCount();
        int ecount = pmesh->m_edges.GetCount();
        int tcount = pmesh->m_triangles.GetCount();

            int m = 4;

        TVector<Vector>   positions(pcount + ecount);
        TVector<Edge>     edges(ecount * 2 + tcount * 3);
        TVector<Triangle> triangles(tcount * m);

        //
        // Copy the old positions
        //

        for (int iposition = 0; iposition < pcount; iposition++) {
            positions.Set(iposition, pmesh->m_positions[iposition]);
        }

        //
        // Subdivide each edge
        //

        for (int iedge = 0; iedge < ecount; iedge++) {
            const Edge& edge = pmesh->m_edges[iedge];

            positions.Set(
                pcount + iedge,
                (pmesh->m_positions[edge.v0] + pmesh->m_positions[edge.v1]).Normalize()
            );

            edges.Set(iedge,          Edge(edge.v0,        pcount + iedge));
            edges.Set(iedge + ecount, Edge(pcount + iedge, edge.v1       ));
        }

        //
        // Subdivide the triangles
        //

        for (int itriangle = 0; itriangle < tcount; itriangle++) {
            const Triangle& triangle = pmesh->m_triangles[itriangle];

            Index a = triangle.edge[0];
            Index b = triangle.edge[1];
            Index c = triangle.edge[2];

            //
            // add three new edges
            //

            Index ab = ecount * 2 + itriangle * 3 + 0;
            Index bc = ecount * 2 + itriangle * 3 + 1;
            Index ca = ecount * 2 + itriangle * 3 + 2;

            edges.Set(ab, Edge(pcount + a, pcount + b));
            edges.Set(bc, Edge(pcount + b, pcount + c));
            edges.Set(ca, Edge(pcount + c, pcount + a));

            //
            // store 4 triangles
            //

            Index a0 = pmesh->m_edges[a].v0;
            Index a1 = pmesh->m_edges[a].v1;
            Index b0 = pmesh->m_edges[b].v0;
            Index b1 = pmesh->m_edges[b].v1;
            Index c0 = pmesh->m_edges[c].v0;
            Index c1 = pmesh->m_edges[c].v1;

            if (a0 == b0) {
                triangles.Set(itriangle * m + 0, Triangle(a, b, ab));

                if (a1 == c0) {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, c, ca));
                    triangles.Set(itriangle * 4 + 2, Triangle(b + ecount, c + ecount, ab));
                } else {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, c + ecount, ca));
                    triangles.Set(itriangle * 4 + 2, Triangle(b + ecount, c, ab));
                }
            } else if (a0 == b1) {
                triangles.Set(itriangle * m + 0, Triangle(a, b + ecount, ab));

                if (a1 == c0) {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, c, ca));
                    triangles.Set(itriangle * 4 + 2, Triangle(b, c + ecount, ab));
                } else {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, c + ecount, ca));
                    triangles.Set(itriangle * 4 + 2, Triangle(b, c, ab));
                }
            } else if (a0 == c0) {
                triangles.Set(itriangle * m + 0, Triangle(a, c, ca));

                if (a1 == b0) {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, b, ab));
                    triangles.Set(itriangle * 4 + 2, Triangle(c + ecount, b + ecount, bc));
                } else {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, b + ecount, ab));
                    triangles.Set(itriangle * 4 + 2, Triangle(c + ecount, b, bc));
                }
            } else {
                ZAssert(a0 == c1);

                triangles.Set(itriangle * m + 0, Triangle(a, c + ecount, ca));

                if (a1 == b0) {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, b, ab));
                    triangles.Set(itriangle * 4 + 2, Triangle(c, b + ecount, bc));
                } else {
                    triangles.Set(itriangle * 4 + 1, Triangle(a + ecount, b + ecount, ab));
                    triangles.Set(itriangle * 4 + 2, Triangle(c, b, bc));
                }
            }

            triangles.Set(itriangle * m + 3, Triangle(ab, bc, ca));
        }

        //
        // Create the new Mesh
        //

        return new TriangleMesh(positions, edges, triangles);
    }

    TRef<Geo> ConvertToMeshGeo()
    {
        int vcount = m_positions.GetCount();
        int tcount = m_triangles.GetCount();

        TVector<Vertex> vertices(vcount);
        TVector<WORD>   indices(tcount * 3);

        int index;

        for (index = 0; index < vcount; index++) {
            vertices.Get(index).SetPosition(m_positions[index]);
            vertices.Get(index).SetNormal(m_positions[index]);
            vertices.Get(index).u = 0;
            vertices.Get(index).v = 0;
        }

        for (index = 0; index < tcount; index++) {
            Index i00 = m_edges[m_triangles[index].edge[0]].v0;
            Index i01 = m_edges[m_triangles[index].edge[0]].v1;

            Index i10 = m_edges[m_triangles[index].edge[1]].v0;
            Index i11 = m_edges[m_triangles[index].edge[1]].v1;

            if (i00 == i10 || i00 == i11) {
                indices.Get(index * 3 + 0) = i01;
                indices.Get(index * 3 + 1) = i00;
                if (i10 == i00) {
                    indices.Get(index * 3 + 2) = i11;
                } else {
                    indices.Get(index * 3 + 2) = i10;
                }
            } else {
                indices.Get(index * 3 + 0) = i00;
                indices.Get(index * 3 + 1) = i01;
                if (i10 == i01) {
                    indices.Get(index * 3 + 2) = i11;
                } else {
                    indices.Get(index * 3 + 2) = i10;
                }
            }
        }

        return Geo::CreateMesh(vertices, indices);
    }

    static TRef<TriangleMesh> CreateIcosahedron()
    {
        const float s = 0.5256f;
        const float b = 0.8506f;

        Vector positions[12] =
            {
                Vector(0.0f,   -s,  -b),
                Vector(0.0f,    s,  -b),
                Vector(0.0f,    s,   b),
                Vector(0.0f,   -s,   b),
                Vector(  -b, 0.0f,  -s),
                Vector(  -b, 0.0f,   s),
                Vector(   b, 0.0f,   s),
                Vector(   b, 0.0f,  -s),
                Vector(  -s,  -b, 0.0f),
                Vector(   s,  -b, 0.0f),
                Vector(   s,   b, 0.0f),
                Vector(  -s,   b, 0.0f)
            };

        Edge edges[30] =
            {
                Edge( 0,  1),  // (x = 0 && z < 0)
                Edge( 2,  3),  // (x = 0 && z > 0)
                Edge( 4,  5),  // (y = 0 && x < 0)
                Edge( 6,  7),  // (y = 0 && x > 0)
                Edge( 8,  9),  // (z = 0 && y < 0)
                Edge(10, 11),  // (z = 0 && y > 0)

                Edge( 1,  4),  // (x = 0 && z < 0) to (y = 0 && x < 0)
                Edge( 4,  0),
                Edge( 0,  7),  // (x = 0 && z < 0) to (y = 0 && x > 0)
                Edge( 7,  1),
                Edge( 3,  5),  // (x = 0 && z > 0) to (y = 0 && x < 0)
                Edge( 5,  2),
                Edge( 2,  6),  // (x = 0 && z > 0) to (y = 0 && x > 0)
                Edge( 6,  3),

                Edge( 5,  8),  // (y = 0 && x < 0) to (z = 0 && y < 0)
                Edge( 8,  4),
                Edge( 4, 11),  // (y = 0 && x < 0) to (z = 0 && y > 0)
                Edge(11,  5),
                Edge( 7,  9),  // (y = 0 && x > 0) to (z = 0 && y < 0)
                Edge( 9,  6),
                Edge( 6, 10),  // (y = 0 && x > 0) to (z = 0 && y > 0)
                Edge(10,  7),

                Edge( 9,  0),  // (z = 0 && y < 0) to (x = 0 && z < 0)
                Edge( 0,  8),
                Edge( 8,  3),  // (z = 0 && y < 0) to (x = 0 && z > 0)
                Edge( 3,  9),
                Edge(11,  1),  // (z = 0 && y > 0) to (x = 0 && z < 0)
                Edge( 1, 10),
                Edge(10,  2),  // (z = 0 && y > 0) to (x = 0 && z > 0)
                Edge( 2, 11)
            };

        Triangle triangles[20] =
            {
                Triangle( 0,  6,  7),   // connect two planes
                Triangle( 0,  8,  9),
                Triangle( 1, 10, 11),
                Triangle( 1, 12, 13),
                Triangle( 2, 14, 15),
                Triangle( 2, 16, 17),
                Triangle( 3, 18, 19),
                Triangle( 3, 20, 21),
                Triangle( 4, 22, 23),
                Triangle( 4, 24, 25),
                Triangle( 5, 26, 27),
                Triangle( 5, 28, 29),

                Triangle(26, 16,  6),   // connect three planes
                Triangle(17, 29, 11),
                Triangle(15, 23,  7),
                Triangle(24, 14, 10),
                Triangle(22, 18,  8),
                Triangle(19, 25, 13),
                Triangle(21, 27,  9),
                Triangle(28, 20, 12)
            };

        return new TriangleMesh(12, positions, 30, edges, 20, triangles);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> MakeSphere(int level)
{
    TRef<TriangleMesh> pmesh = TriangleMesh::CreateIcosahedron();

    for (int index = 0; index < level; index++) {
        pmesh = TriangleMesh::SubdivideSphere(pmesh);
    }

    return pmesh->ConvertToMeshGeo();
}
