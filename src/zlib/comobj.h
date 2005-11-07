#ifndef _comobj_H_
#define _comobj_H_

//////////////////////////////////////////////////////////////////////////////
//
// Non aggregated COM Object
//
//////////////////////////////////////////////////////////////////////////////

template<class Interface>
class ComObject : public Interface {
protected:
    typedef ComObject<Interface> QIType;
    int m_count;

public:
    __declspec(dllexport) ComObject() :
        m_count(0)
    {
    }

    virtual ~ComObject()
    {
    }

    __declspec(dllexport) STDMETHODIMP_(ULONG) AddRef()
    {
        return ++m_count;
    }

    __declspec(dllexport) STDMETHODIMP_(ULONG) Release()
    {
        if (--m_count == 0) {
            delete this;
            return 0;
        } else {
            return m_count;
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// COM Object with single inheritance
//
//////////////////////////////////////////////////////////////////////////////

template<class Interface, const IID* piid>
class SingleComObject : public ComObject<Interface> {
protected:
    typedef SingleComObject<Interface, piid> QIType;

public:
    virtual ~SingleComObject()
    {
    }

    __declspec(dllexport) STDMETHODIMP QueryInterface(const IID &riid, void** ppvObj) { \
        if (riid == IID_IUnknown) {
            this->AddRef();
            *ppvObj = (IUnknown*)(QIType*)this;
            return S_OK;
        }

        if (riid == *piid) {
            this->AddRef();
            *ppvObj = (Interface*)this;
            return S_OK;
        }

        return E_NOINTERFACE;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Implementation macros
//
//////////////////////////////////////////////////////////////////////////////

#define IUNKNOWN_IMP() \
    STDMETHODIMP_(ULONG) AddRef()  { return QIType::AddRef();  } \
    STDMETHODIMP_(ULONG) Release() { return QIType::Release(); }

#define BEGIN_INSIDE_QI() \
    HRESULT NonDelegatingQueryInterface(const IID &riid, void** ppvObj) {

#define BEGIN_QI() \
    STDMETHODIMP QueryInterface(const IID &riid, void** ppvObj) { \
        if (riid == IID_IUnknown) {                 \
            this->AddRef();                               \
            *ppvObj = (IUnknown*)(QIType*)this;    \
            return S_OK;                            \
        }

#define QI_INTERFACE(Interface) \
    if (riid == IID_##Interface) {     \
        this->AddRef();                \
        *ppvObj = (Interface*)this;    \
        return S_OK;                   \
    }

#define QI_INSIDE_INTERFACE(Interface) \
    if (riid == IID_##Interface) {     \
        object.m_punk->AddRef();       \
        *ppvObj = (Interface*)this;    \
        return S_OK;                   \
    }

#define END_QI() return E_NOINTERFACE; }
#define END_INSIDE_QI() return E_NOINTERFACE; }
#define END_OUTSIDE_QI() return m_punk->QueryInterface(riid, ppvObj); }

//////////////////////////////////////////////////////////////////////////////
//
// Aggregated COM Object
//
//////////////////////////////////////////////////////////////////////////////

template<class ObjectType>
class ComInsideObject : public IUnknown {
private:
    int m_count;
    ObjectType* m_pobject;

public:
    IUnknown* m_punk;

    ComInsideObject(IUnknown* punk, ObjectType* pobject) :
        m_punk(punk),
        m_pobject(pobject),
        m_count(0)
    {
    }

    STDMETHODIMP QueryInterface(const IID &riid, void** ppvObj)
    {
        if (riid == IID_IUnknown) {
            AddRef();
            *ppvObj = this;
            return S_OK;
        }
        return m_pobject->NonDelegatingQueryInterface(riid, ppvObj);
    }

    STDMETHODIMP_(ULONG) AddRef()
    {
        return ++m_count;
    }

    STDMETHODIMP_(ULONG) Release()
    {
        if (--m_count == 0) {
            delete m_pobject;
            return 0;
        } else {
            return m_count;
        }
    }
};

#define IUNKNOWN_INSIDE_IMP() \
    STDMETHODIMP_(ULONG) AddRef()  { return object.m_punk->AddRef(); }   \
    STDMETHODIMP_(ULONG) Release() { return object.m_punk->Release(); }  \
    STDMETHODIMP QueryInterface(const IID &riid, void** ppvObj) { \
        return object.m_punk->QueryInterface(riid, ppvObj); }

//////////////////////////////////////////////////////////////////////////////
//
// Tear off interface implementation
//
//////////////////////////////////////////////////////////////////////////////

#define TEAR_OFF_INTERFACE(Interface, ObjectType)           \
    if (riid == IID_##Interface) {                          \
        *ppvObj = new Interface##TearOff<Interface, ObjectType>(this); \
        return S_OK;                                        \
    }

template<class Interface, class Type>
class TTearOff : public ComObject<Interface> {
private:
    TRef<Type> m_pobject;

protected:
    Type* Object() { return m_pobject; }

public:
    TTearOff(Type* pobject) :
        m_pobject(pobject)
    {}

    STDMETHODIMP QueryInterface(const IID &riid, void** ppvObj)
    {
        return m_pobject->QueryInterface(riid, ppvObj);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Example usage
//
//////////////////////////////////////////////////////////////////////////////

#if 0

template<class Type>
class IEvaluateTearOff : public TTearOff<Type> {
public:
    STDMETHOD(Eval)(float time) { return Object()->Eval(time); }
};

class IImageImpl : public ComObject<IImage> {
public:
    BEGIN_QI()
        QI_INTERFACE(IUnknown)
        QI_INTERFACE(IImage)
        TEAR_OFF_INTERFACE(IEvaluate, IImageImpl)
    END_QI()

    // IImage members

    STDMETHOD_(bool, GetRect)(Rect*) PURE;
    STDMETHOD_(bool, CanRender)(ITransform*, IRegion*) PURE;
    STDMETHOD(GetTileSurface)(ISurface** ppsurface, Rect* prectNeed) PURE;
    STDMETHOD(GetSurface)(ISurface**, Rect* prectNeed, Rect* prectSource) PURE;
    STDMETHOD(Render)(ISurface*, ITransform*, IRegion*) PURE;

    // IEvaluate members

    STDMETHOD(Eval)(float time) PURE;
};

#endif

#endif
