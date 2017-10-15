#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Camera
//
//////////////////////////////////////////////////////////////////////////////

Camera::Camera() :
    Value(
        new VectorValue(
			Vector(
				0, 0, 0
			)
		),
        new OrientationValue(
            Orientation(
                Vector(0, 0, -1),
                Vector(0, 1, 0)
            )
        )
    ),
    m_znear(-1),
    m_zfar(1),
    m_focus(0),
    m_bInverseValid(false)
{
}

const Vector& Camera::GetPosition()
{
    return GetPositionValue()->GetValue();
}

const Orientation& Camera::GetOrientation()
{
    return GetOrientationValue()->GetValue();
}

void Camera::Evaluate()
{
    if (m_focus == 0) {
        //
        // orthoganal projection
        //

        //  : what is m_focus for an orthogonal camera?

        //
        // 1) xscreen = x
        // 2) zbuffer = a * z + b
        //
        // 3)        0 = a * znear + b
        // 4)        1 = a * zfar  + b
        // 5) 3 =>   b = -a * znear
        // 6) 4,5 => 1 = a * zfar - a * znear
        //           a = 1 / (zfar - znear)
        //


        float a = 1 / (m_zfar - m_znear);
        float b = -a * m_znear;

        m_matPerspective.SetIdentity();
        m_matPerspective.Set(2, 2, a);
        m_matPerspective.Set(2, 3, b);
    } else {
        //
        // perspective projection
        //

        //
        // 1) xscreen = f * x / -z
        // 2) zbuffer = (a * z + b) / -z
        //
        // 3)            0 = (a * znear + b) / -znear
        // 4)            1 = (a * zfar  + b) / -zfar
        // 5) 3   =>     b = -a * znear
        // 6) 4,5 => -zfar = a * zfar - a * znear
        //               a = zfar / (znear - zfar)
        //
        //

        float a = m_zfar / (m_znear - m_zfar);
        float b = -a * m_znear;

        m_matPerspective.SetIdentity();

        m_matPerspective.Set(0, 0, m_focus);
        m_matPerspective.Set(1, 1, m_focus);

        m_matPerspective.Set(2, 2, a);
        m_matPerspective.Set(2, 3, b);

        m_matPerspective.Set(3, 2, -1);
        m_matPerspective.Set(3, 3,  0);
    }

    const Vector&      v = GetPosition();
    const Orientation& o = GetOrientation();

    m_matModel =
        Matrix(
            o[0][0], o[0][1], o[0][2], 0,
            o[1][0], o[1][1], o[1][2], 0,
            o[2][0], o[2][1], o[2][2], 0
        );

    m_matModel.PreTranslate(-v);
    m_bInverseValid = false;
}

void Camera::UpdateInverse()
{
    if (!m_bInverseValid) {
        m_matInverseModel.SetInverse(m_matModel);
    }
}

void Camera::SetPosition(const Vector& vec)
{
    SetPositionValue(new VectorValue(vec));
}

void Camera::SetOrientation(const Orientation& orientation)
{
    SetOrientationValue(new OrientationValue(orientation));
}

void Camera::SetZClip(float znear, float zfar)
{
    m_znear = -znear;
    m_zfar  = -zfar;
    Changed();
}

void Camera::SetOrthogonal()
{
    m_focus = 0;
    Changed();
}

void Camera::SetPerspective(float f)
{
    m_focus  = f;
    Changed();
}

void Camera::SetFOV(float fov)
{
    m_fov = fov;
    SetPerspective(cos(0.5f * fov) / sin(0.5f * fov));
}

Vector Camera::TransformLocalToEye(const Vector& vec)
{
    return GetModelMatrix().Transform(vec);
}

bool Camera::TransformEyeToImage(const Vector& vecEye, Point& point)
{
    if (GetFocus() == 0) {
        point = Point(vecEye.X(), vecEye.Y());
    } else {
        if (vecEye.Z() >= 0) {
            return false;
        }

        point = Point(vecEye.X(), vecEye.Y()) * (GetFocus() / -vecEye.Z());
    }

    return true;
}

bool Camera::TransformLocalToImage(const Vector& vec, Point& point)
{
    return TransformEyeToImage(TransformLocalToEye(vec), point);
}

bool Camera::TransformDirectionToImage(const Vector& vec, Point& point)
{
    HVector hvec = GetModelMatrix().Transform(HVector(vec.X(), vec.Y(), vec.Z(), 0));

    return TransformEyeToImage(Vector(hvec.X(), hvec.Y(), hvec.Z()), point);
}

Vector Camera::TransformImageToDirection(const Point& point)
{
    UpdateInverse();
    HVector hvec = m_matInverseModel.Transform(HVector(point.X(), point.Y(), -GetFocus(), 0));

    return Vector(hvec.X(), hvec.Y(), hvec.Z());
}

//////////////////////////////////////////////////////////////////////////////
//
// MotionCamera
//
//////////////////////////////////////////////////////////////////////////////

class MotionCamera : public Value, public IKeyboardInput {
private:
	//////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
	//////////////////////////////////////////////////////////////////////////////

    class MotionCameraImage : public Image {
        TRef<MotionCamera> m_pcamera;

    public:
        MotionCameraImage(MotionCamera* pcamera) :
            m_pcamera(pcamera)
        {
        }

        MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
        { 
            return MouseResultHit(); 
        }

        void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
        {
            m_pcamera->MouseMove(pprovider, point, bCaptured, bInside);
        }

        MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
        {
            return m_pcamera->Button(pprovider, point, button, bCaptured, bInside, bDown);
        }

        //
        // Value members
        //

        ZString GetFunctionName() { return "MotionCameraImage"; }
    };

    class MotionVector : public VectorValue {
		MotionCamera* GetMotionCamera() { return MotionCamera::Cast(GetChild(0)); };

    public:
        MotionVector(MotionCamera* pmotionCamera) :
            VectorValue(pmotionCamera)
        {
        }

        //
        // Value members
        //

        void Evaluate()
        {
            GetValueInternal() = GetMotionCamera()->m_position;
        }
    };

    class MotionOrientation : public OrientationValue {
		MotionCamera* GetMotionCamera() { return MotionCamera::Cast(GetChild(0)); };

    public:
        MotionOrientation(MotionCamera* pmotionCamera) :
            OrientationValue(pmotionCamera)
        {
        }

        //
        // Value members
        //

        void Evaluate()
        {
            GetValueInternal() = GetMotionCamera()->m_orient;
        }
    };

    friend class MotionVector;
    friend class MotionOrientation;

	//////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
	//////////////////////////////////////////////////////////////////////////////

    float        m_timeLast;

    Vector       m_position;
    Orientation  m_orientLast;
    Orientation  m_orient;
    float        m_speed;

    float        m_angleRoll;
    float        m_angleYaw;
    float        m_anglePitch;

    bool         m_bForward;
    bool         m_bBackward;
    bool         m_bLeft;
    bool         m_bRight;
    bool         m_bUp;
    bool         m_bDown;
    bool         m_bAccelerate;

    bool         m_bLeftButtonDown;
    bool         m_bRightButtonDown;
    Point        m_pointStart;

    float GetTime() { return Number::Cast(GetChild(0))->GetValue(); }

public:
    DefineCastMember(MotionCamera);

    MotionCamera(
		const Vector& vec, 
		Number* ptime, 
		TRef<Image>& pimage,
		TRef<VectorValue>& pvectorValue,
		TRef<OrientationValue>& porientationValue
	) :
        Value(ptime),
        m_position(vec)
    {
        pimage            = new MotionCameraImage(this);
        pvectorValue      = new MotionVector(this);
        porientationValue = new MotionOrientation(this);

        m_timeLast    = ptime->GetValue();

        m_anglePitch  = 0;
        m_angleYaw    = 0;
        m_angleRoll   = 0;

        m_speed       = 0;

        m_bForward    = false;
        m_bBackward   = false;
        m_bLeft       = false;
        m_bRight      = false;
        m_bUp         = false;
        m_bDown       = false;
        m_bAccelerate = false;

        m_bLeftButtonDown  = false;
        m_bRightButtonDown = false;
    }

	//////////////////////////////////////////////////////////////////////////////
    //
    // IKeboardInput Methods
    //
	//////////////////////////////////////////////////////////////////////////////

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        switch (ks.vk) {
            case 'W'      : m_bForward    = ks.bDown; return true;
            case 'S'      : m_bBackward   = ks.bDown; return true;
            case 'A'      : m_bLeft       = ks.bDown; return true;
            case 'D'      : m_bRight      = ks.bDown; return true;
            case 'E'      : m_bUp         = ks.bDown; return true;
            case 'Q'      : m_bDown       = ks.bDown; return true;
            case VK_SHIFT : m_bAccelerate = ks.bDown; return true;
        }

        return false;
    }

    void MouseDown(IInputProvider* pprovider, const Point& point)
    {
        ZAssert(m_angleYaw == 0);
        m_angleRoll        = 0;
        m_angleYaw         = 0;
        m_anglePitch       = 0;

        m_pointStart = point;
        //pprovider->ShowCursor(false);
    }

    void MouseUp(IInputProvider* pprovider)
    {
        m_orientLast       = m_orient;
        m_bLeftButtonDown  = false;
        m_bRightButtonDown = false;
        m_angleRoll        = 0;
        m_angleYaw         = 0;
        m_anglePitch       = 0;

        //pprovider->SetCursorPos(m_pointStart);
        //pprovider->ShowCursor(true);
    }

	//////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput Methods
    //
	//////////////////////////////////////////////////////////////////////////////

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    {
        if (bCaptured) {
            if (point != m_pointStart) {
                float scale = 0.0025f;
                float dx = scale * (point.X() - m_pointStart.X());
                float dy = scale * (point.Y() - m_pointStart.Y());

                if (m_bRightButtonDown) {
                    m_angleRoll -= dx;
                } else {
                    m_angleYaw  -= dx;
                    m_anglePitch =
                        bound(
                            m_anglePitch + dy,
                            -pi/2,
                            pi/2
                        );
                }

                pprovider->SetCursorPos(m_pointStart);

                Changed();
            }
        }
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        switch (button) {
            case 0:
                if (bDown) {
                    if (!m_bRightButtonDown) {
                        m_bLeftButtonDown = true;
                        MouseDown(pprovider, point);
                        return MouseResultCapture();
                    }
                } else {
                    if (m_bLeftButtonDown) {
                        m_bLeftButtonDown = false;
                        MouseUp(pprovider);
                        return MouseResultRelease();
                    }
                }
                break;

            case 1:
                if (bDown) {
                    if (!m_bLeftButtonDown) {
                        m_bRightButtonDown = true;
                        MouseDown(pprovider, point);
                        return MouseResultCapture();
                    }
                } else {
                    if (m_bRightButtonDown) {
                        m_bRightButtonDown = false;
                        MouseUp(pprovider);
                        return MouseResultRelease();
                    }
                }
                break;

        }

        return MouseResult();
    }

	//////////////////////////////////////////////////////////////////////////////
    //
    // Value methods
    //
	//////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        float time  = GetTime();
        float dtime = time - m_timeLast;
        m_timeLast  = time;

        //
        // The orientation
        //

        m_orient = m_orientLast;

        if (m_bLeftButtonDown || m_bRightButtonDown) {
            m_orient.Yaw(m_angleYaw);
            m_orient.Pitch(m_anglePitch);
            m_orient.Roll(m_angleRoll);
        }

        //
        // The position
        //

        Vector vecDPos(0, 0, 0);

        if (m_bForward) {
            vecDPos -= m_orient.GetBackward() * dtime;
        }

        if (m_bBackward) {
            vecDPos += m_orient.GetBackward() * dtime;
        }

        if (m_bRight) {
            vecDPos += m_orient.GetRight() * dtime;
        }

        if (m_bLeft) {
            vecDPos -= m_orient.GetRight() * dtime;
        }

        if (m_bUp) {
            vecDPos += m_orient.GetUp() * dtime;
        }

        if (m_bDown) {
            vecDPos -= m_orient.GetUp() * dtime;
        }

        if (
               m_bAccelerate
            && (m_bForward || m_bBackward || m_bRight || m_bLeft || m_bUp || m_bDown)
        ) {
            m_speed *= 1.1f;
            if (m_speed > 100) {
                m_speed = 100;
            }
        } else {
            m_speed = 10;
        }

        //
        // Setup the camera
        //

        m_position += vecDPos * m_speed;
    }

    ZString GetFunctionName() { return "MotionCamera"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// MotionCamera
//
//////////////////////////////////////////////////////////////////////////////

TRef<Camera> CreateMotionCamera(
    const Vector& vec,
    Number* ptime,
    TRef<Image>& pimage,
    TRef<IKeyboardInput>& pkeyboardInput
) {
	TRef<VectorValue>      pvectorValue;
	TRef<OrientationValue> porientationValue;

    TRef<MotionCamera> pmotionCamera = 
		new MotionCamera(
			vec, 
			ptime, 
			pimage,
			pvectorValue,
			porientationValue
		);

    pkeyboardInput = pmotionCamera;

    return new Camera(pvectorValue, porientationValue);
}

//////////////////////////////////////////////////////////////////////////////
//
// SmoothPosition
//
//////////////////////////////////////////////////////////////////////////////

class SmoothPosition : public ISmoothPosition {
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class Data : public IObject {
    public:
        Vector m_vec;
        float  m_dtime;

        Data(const Vector& vec, float dtime) :
            m_vec(vec),
            m_dtime(dtime)
        {
        }
    };

    typedef TList<TRef<Data> > DataList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    float GetTime() { return Number::Cast(GetChild(0))->GetValue(); }

    DataList m_listData;
    Vector   m_positionStart;
    float    m_timeStart;

public:
    SmoothPosition(
		Number* ptime, 
		const Vector& vec
	) :
        ISmoothPosition(ptime),
        m_positionStart(vec),
        m_timeStart(0)
    {
        GetValueInternal() = m_positionStart;
    }

    void Goto(const Vector& vector, float dtime)
    {
		if (m_listData.IsEmpty()) {
			m_timeStart = GetTime();
		}

        m_listData.PushEnd(new Data(vector, dtime));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        float time  = GetTime();
        float dtime = time - m_timeStart;
        Data* pdata;

        while (true) {
            if (m_listData.IsEmpty()) {
                return;
            }

            pdata = m_listData.GetFront();

            if (dtime <= pdata->m_dtime) {
                break;
            }

            GetValueInternal() = pdata->m_vec;
            m_positionStart    = pdata->m_vec;
            m_timeStart       += pdata->m_dtime;
			dtime             -= pdata->m_dtime;

            m_listData.PopFront();
        }

        GetValueInternal() =
            Interpolate(
                m_positionStart,
                pdata->m_vec,
                SmoothInterpolant(dtime / pdata->m_dtime)
            );
    }
};

TRef<ISmoothPosition> CreateSmoothPosition(Number* ptime, const Vector& vec)
{
    return new SmoothPosition(ptime, vec);
}

//////////////////////////////////////////////////////////////////////////////
//
// SmoothOrientation
//
//////////////////////////////////////////////////////////////////////////////

class SmoothOrientation : public ISmoothOrientation
{
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class Data : public IObject
    {
    public:
        Quaternion  m_quaternion;
        float       m_dtime;

        Data(const Orientation& orientation, float dtime)
        :
            m_quaternion(orientation),
            m_dtime(dtime)
        {
        }
    };

    typedef TList<TRef<Data> > DataList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

    float GetTime() { return Number::Cast(GetChild(0))->GetValue(); }

    DataList    m_listData;
    Quaternion  m_qStart;
    float       m_timeStart;

public:
    SmoothOrientation(Number*              ptime, 
		              const Orientation&   orientation)
    :
        ISmoothOrientation(ptime),
        m_qStart(orientation),
        m_timeStart(0)
    {
        GetValueInternal() = orientation;
    }

    void Goto(const Orientation& orientation, float dtime)
    {
		if (m_listData.IsEmpty())
        {
			m_timeStart = GetTime();
		}

        m_listData.PushEnd(new Data(orientation, dtime));
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        if (m_listData.IsEmpty())
                return;

        float time  = GetTime();
        float dtime = time - m_timeStart;

        Data* pdata;

        while (true)
        {
            pdata = m_listData.GetFront();

            if (dtime <= pdata->m_dtime)
                break;

            m_qStart           = pdata->m_quaternion;
            m_timeStart       += pdata->m_dtime;
			dtime             -= pdata->m_dtime;

            m_listData.PopFront();

            if (m_listData.IsEmpty())
            {
                GetValueInternal() = m_qStart;
                return;
            }
        }

        GetValueInternal() = Slerp(m_qStart, pdata->m_quaternion, dtime / pdata->m_dtime);
    }
};

TRef<ISmoothOrientation> CreateSmoothOrientation(Number* ptime, const Orientation& o)
{
    return new SmoothOrientation(ptime, o);
}

//////////////////////////////////////////////////////////////////////////////
//
// LookAtOrientation
//
//////////////////////////////////////////////////////////////////////////////

class LookAtOrientation : public OrientationValue {
    //////////////////////////////////////////////////////////////////////////////
    //
    // Arguments
    //
    //////////////////////////////////////////////////////////////////////////////

    const Vector&   GetAt() { return VectorValue::Cast(GetChild(0))->GetValue(); }
    const Vector& GetFrom() { return VectorValue::Cast(GetChild(1))->GetValue(); }
    const Vector&   GetUp() { return VectorValue::Cast(GetChild(2))->GetValue(); }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Value members
    //
    //////////////////////////////////////////////////////////////////////////////

    void Evaluate()
    {
        const Vector& vecAt   = GetAt();
        const Vector& vecFrom = GetFrom();
        const Vector& vecUp   = GetUp();

        GetValueInternal().Set(
            (vecAt - vecFrom).Normalize(),
            vecUp
        );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////

public:
    LookAtOrientation(
        VectorValue* pvectorAt,
        VectorValue* pvectorFrom,
        VectorValue* pvectorUp
     ) :
        OrientationValue(
            pvectorAt,
            pvectorFrom,
            pvectorUp
        )
    {
    }
};

TRef<OrientationValue> CreateLookAtOrientation(
    VectorValue* pvectorAt,
    VectorValue* pvectorFrom,
    VectorValue* pvectorUp
) {
    return new LookAtOrientation(pvectorAt, pvectorFrom, pvectorUp);
}
