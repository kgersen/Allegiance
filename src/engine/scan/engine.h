//////////////////////////////////////////////////////////////////////////////
//
// Engine headers
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _Engine_h_
#define _Engine_h_

//////////////////////////////////////////////////////////////////////////////
//
// Basic Types
//
//////////////////////////////////////////////////////////////////////////////

typedef Number float;

//////////////////////////////////////////////////////////////////////////////
//
// Values
//
//////////////////////////////////////////////////////////////////////////////

class Value {
public:
    virtual void SetValue(Value* pvalue);
};

//////////////////////////////////////////////////////////////////////////////
//
// Transforms
//
//////////////////////////////////////////////////////////////////////////////

class Transform : public Value {
public:
};

class ComponentTransform : public Transform {
public:
    virtual void SetOrientation(const Orientation& orientation);
    virtual void SetScale(Number scale);
    virtual void SetPosition(const Vector& position);
};

//////////////////////////////////////////////////////////////////////////////
//
// Images
//
//////////////////////////////////////////////////////////////////////////////

class Image : public Value {
public:
    static Image* GetEmpty();

    virtual void Render(ISurface* psurface);
};

class GeometryImage : public Image {
public:
    void AddImage(Image* pimage);
};

//////////////////////////////////////////////////////////////////////////////
//
// Geometries
//
//////////////////////////////////////////////////////////////////////////////

class Geometry : public Value {
public:
    static Geometry* GetEmpty();
};

class MeshGeometry : public Geometry {
public:
};

class TransformGeometry : public Geometry {
public:
    void SetTransform(Transform* ptrans);
};

class GroupGeometry : public Geometry {
public:
    void AddGeometry(Geometry* pgeo);
};

//////////////////////////////////////////////////////////////////////////////
//
// High Level Geometries
//
//////////////////////////////////////////////////////////////////////////////

class FedGeometry {
private:
    TRef<Geometry>  m_pgeo;
    TRef<Transform> m_ptrans;
    TRef<Geometry>  m_pvalue;

public:
    FedGeometry(Geometry pgeo) :
        m_pgeo(pgeo),
    {
        m_ptrans = new ComponentTransform(),
        m_pvalue = new TransformGeometry(pgeo, m_ptrans);
    }

    operator Geometry*()
    {
        return m_pvalue;
    }

    void Remove()
    {
        m_pvalue->SetValue(Geometry::GetEmpty());
    }

    void SetGeometry(Geometry* pgeo)
    {
        m_pgeo->SetValue(pgeo);
    }

    void SetOrientation(const Orientation& orientation)
    {
        m_ptrans->SetOrientation(orientation);
    }

    void SetScale(Number scale)
    {
        m_ptrans->GetScale(scale);
    }

    void SetPosition(const Vector& position)
    {
        m_ptrans->GetPosition(position);
    }
};

#endif
