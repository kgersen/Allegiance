#ifndef _Viewport_h_
#define _Viewport_h_

//////////////////////////////////////////////////////////////////////////////
//
// Viewport header
//
//////////////////////////////////////////////////////////////////////////////

class Viewport : public Value {
public:
    DefineCastMember(Viewport);

    Viewport(Camera* pcamera, RectValue* prectView) :
        Value(pcamera, prectView)
    {
    }

    void SetCamera(Camera* pvalue)      { SetChild(0, pvalue); }
    void SetViewRect(RectValue* pvalue) { SetChild(1, pvalue); }

    Camera*    GetCamera()   { return Camera::Cast(GetChild(0));    }
    RectValue* GetViewRect() { return RectValue::Cast(GetChild(1)); }

    const Rect&    GetRect();
          float    GetAspect();
          Point    TransformNDCToImage(const Point& point);
          WinPoint TransformNDCToScreen(const Point& point);
          WinPoint TransformImageToScreen(const Point& point);
};

#endif
