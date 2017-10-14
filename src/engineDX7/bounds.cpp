#include "pch.h"

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

    return
        Bounds2(
            Rect(
                min(vec1.X(), vec2.X()),
                min(vec1.Y(), vec2.Y()),
                max(vec1.X(), vec2.X()),
                max(vec1.Y(), vec2.Y())
            )
        );
}

Bounds2 Union(const Bounds2& bounds1, const Bounds2& bounds2)
{
    return
        Bounds2(
            Rect(
                min(bounds1.GetRect().XMin(), bounds2.GetRect().XMin()),
                min(bounds1.GetRect().YMin(), bounds2.GetRect().YMin()),
                max(bounds1.GetRect().XMax(), bounds2.GetRect().XMax()),
                max(bounds1.GetRect().YMax(), bounds2.GetRect().YMax())
            )
        );
}

Bounds2 Intersect(const Bounds2& bounds1, const Bounds2& bounds2)
{
    return
        Bounds2(
            Rect(
                max(bounds1.GetRect().XMin(), bounds2.GetRect().XMin()),
                max(bounds1.GetRect().YMin(), bounds2.GetRect().YMin()),
                min(bounds1.GetRect().XMax(), bounds2.GetRect().XMax()),
                min(bounds1.GetRect().YMax(), bounds2.GetRect().YMax())
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
                min(bounds1.GetMin().X(), bounds2.GetMin().X()),
                min(bounds1.GetMin().Y(), bounds2.GetMin().Y()),
                min(bounds1.GetMin().Z(), bounds2.GetMin().Z())
            ),
            Vector(
                max(bounds1.GetMax().X(), bounds2.GetMax().X()),
                max(bounds1.GetMax().Y(), bounds2.GetMax().Y()),
                max(bounds1.GetMax().Z(), bounds2.GetMax().Z())
            )
        );
}
*/
