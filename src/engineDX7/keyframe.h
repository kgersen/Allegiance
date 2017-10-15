#ifndef _keyframe_h_
#define _keyframe_h_

//////////////////////////////////////////////////////////////////////////////
//
// Key Framed Transforms
//
//////////////////////////////////////////////////////////////////////////////

class KeyFramedTranslateTransform : public Transform {
protected:
    KeyFramedTranslateTransform(Number* pframe) :
        Transform(pframe)
    {
    }

public:
    virtual void AddKey(float frame, const Vector& vec) = 0;
};

class KeyFramedScaleTransform : public Transform {
protected:
    KeyFramedScaleTransform(Number* pframe) :
        Transform(pframe)
    {
    }

public:
    virtual void AddKey(float frame, const Vector& vec) = 0;
};

class KeyFramedRotateTransform : public Transform {
protected:
    KeyFramedRotateTransform(Number* pframe) :
        Transform(pframe)
    {
    }

public:
    virtual void AddKey(float frame, const Quaternion& quaternion) = 0;
};

TRef<KeyFramedTranslateTransform> CreateKeyFramedTranslateTransform(Number* pframe);
TRef<KeyFramedScaleTransform>     CreateKeyFramedScaleTransform(Number* pframe);
TRef<KeyFramedRotateTransform>    CreateKeyFramedRotateTransform(Number* pframe);

#endif
