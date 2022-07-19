#include "bounds.h"

#include <matrix.h>

//////////////////////////////////////////////////////////////////////////////
//
// Bounds2
//
//////////////////////////////////////////////////////////////////////////////

Bounds2 g_bounds2Empty(Rect(0, 0, 0, 0));

const Bounds2& Bounds2::GetEmpty()
{
    return g_bounds2Empty;
}

Bounds2::Bounds2(const Rect& rect) :
    m_rect(rect)
{
}

Bounds2::Bounds2(const Bounds2& bounds) :
    m_rect(bounds.m_rect)
{
}

void Bounds2::SetRect(const Rect& rect)
{
    m_rect = rect;
}

Bounds2 Bounds2::Transform(const Matrix2& mat) const
{
    Point vec1 = mat.Transform(m_rect.Min());
    Point vec2 = mat.Transform(m_rect.Max());
    Point vec3 = mat.Transform(Point(m_rect.Min().X(), m_rect.Max().Y()));
    Point vec4 = mat.Transform(Point(m_rect.Max().X(), m_rect.Min().Y()));

    return
        Bounds2(
            Rect(
                std::min({ vec1.X(), vec2.X(), vec3.X(), vec4.X() }),
                std::min({ vec1.Y(), vec2.Y(), vec3.Y(), vec4.Y() }),
                std::max({ vec1.X(), vec2.X(), vec3.X(), vec4.X() }),
                std::max({ vec1.Y(), vec2.Y(), vec3.Y(), vec4.Y() })
            )
        );
}

Bounds2 Union(const Bounds2& bounds1, const Bounds2& bounds2)
{
    return
        Bounds2(
            Rect(
                std::min(bounds1.GetRect().XMin(), bounds2.GetRect().XMin()),
                std::min(bounds1.GetRect().YMin(), bounds2.GetRect().YMin()),
                std::max(bounds1.GetRect().XMax(), bounds2.GetRect().XMax()),
                std::max(bounds1.GetRect().YMax(), bounds2.GetRect().YMax())
            )
        );
}

Bounds2 Intersect(const Bounds2& bounds1, const Bounds2& bounds2)
{
    return
        Bounds2(
            Rect(
                std::max(bounds1.GetRect().XMin(), bounds2.GetRect().XMin()),
                std::max(bounds1.GetRect().YMin(), bounds2.GetRect().YMin()),
                std::min(bounds1.GetRect().XMax(), bounds2.GetRect().XMax()),
                std::min(bounds1.GetRect().YMax(), bounds2.GetRect().YMax())
            )
        );
}

//////////////////////////////////////////////////////////////////////////////
//
// Bounds
//
//////////////////////////////////////////////////////////////////////////////

/*
Bounds g_boundsEmpty(Vector::GetZero(), Vector::GetZero());

const Bounds& Bounds::GetEmpty()
{
    return g_boundsEmpty;
}

Bounds::Bounds(const Vector& vecMin, const Vector& vecMax) :
    m_vecMin(vecMin),
    m_vecMax(vecMax)
{
    CalcDiagonal();
}

Bounds::Bounds(const Bounds& bounds) :
    m_vecMin(bounds.m_vecMin),
    m_vecMax(bounds.m_vecMax)
{
    CalcDiagonal();
}

void Bounds::CalcDiagonal()
{
    m_diagonal = (m_vecMax - m_vecMin).Length();
}

void Bounds::SetVectors(const Vector& vecMin, const Vector& vecMax)
{
    m_vecMin = vecMin;
    m_vecMax = vecMax;
    CalcDiagonal();
}

Bounds Bounds::Transform(const Matrix& mat)
{
    Vertex vec[8] = {
        mat.Transform(Vertex(m_vecMin.X(), m_vecMin.Y(), m_vecMin.Z())),
        mat.Transform(Vertex(m_vecMin.X(), m_vecMin.Y(), m_vecMax.Z())),
        mat.Transform(Vertex(m_vecMin.X(), m_vecMax.Y(), m_vecMin.Z())),
        mat.Transform(Vertex(m_vecMin.X(), m_vecMax.Y(), m_vecMax.Z())),
        mat.Transform(Vertex(m_vecMax.X(), m_vecMin.Y(), m_vecMin.Z())),
        mat.Transform(Vertex(m_vecMax.X(), m_vecMin.Y(), m_vecMax.Z())),
        mat.Transform(Vertex(m_vecMax.X(), m_vecMax.Y(), m_vecMin.Z())),
        mat.Transform(Vertex(m_vecMax.X(), m_vecMax.Y(), m_vecMax.Z()))
    };

    Vector vecMin(vec[0].X(), vec[0].Y(), vec[0].Z());
    Vector vecMax(vec[0].X(), vec[0].Y(), vec[0].Z());

    for(int index = 1; index < 8; index++) {
        if (vec[index].X() < vecMin.X()) vecMin.SetX(vec[index].X());
        if (vec[index].X() > vecMax.X()) vecMax.SetX(vec[index].X());
        if (vec[index].Y() < vecMin.Y()) vecMin.SetY(vec[index].Y());
        if (vec[index].Y() > vecMax.Y()) vecMax.SetY(vec[index].Y());
        if (vec[index].Z() < vecMin.Z()) vecMin.SetZ(vec[index].Z());
        if (vec[index].Z() > vecMax.Z()) vecMax.SetZ(vec[index].Z());
    }

    return Bounds(vecMin, vecMax);
}

Bounds Union(const Bounds& bounds1, const Bounds& bounds2)
{
    return
        Bounds(
            Vector(
                std::min(bounds1.GetMin().X(), bounds2.GetMin().X()),
                std::min(bounds1.GetMin().Y(), bounds2.GetMin().Y()),
                std::min(bounds1.GetMin().Z(), bounds2.GetMin().Z())
            ),
            Vector(
                std::max(bounds1.GetMax().X(), bounds2.GetMax().X()),
                std::max(bounds1.GetMax().Y(), bounds2.GetMax().Y()),
                std::max(bounds1.GetMax().Z(), bounds2.GetMax().Z())
            )
        );
}
*/
