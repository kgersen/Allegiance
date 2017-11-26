
#pragma once

#include "model.h"
#include "enginep.h"
#include "engine.h"

template<class OriginalType>
class CallbackImage : public WrapImage {
    typedef std::function<TRef<Image>(OriginalType)> CallbackType;

    CallbackType m_callback;
    bool bUpdating;

protected:
    virtual void ChildChanged(Value* pvalue, Value* pvalueNew) override {
        if (!bUpdating) {
            Value::ChildChanged(pvalue, pvalueNew);
        }
    }

public:
    CallbackImage(CallbackType callback, TStaticValue<OriginalType>* pvalue1) :
        m_callback(callback),
        WrapImage(Image::GetEmpty(), pvalue1),
        bUpdating(false)
    {}

    void Evaluate()
    {
        if (!bUpdating) {
            OriginalType value1 = ((TStaticValue<OriginalType>*)GetChild(1))->GetValue();

            TRef<Image> evaluated = m_callback(value1);
            evaluated->Evaluate();

            SetImage(evaluated);

            bUpdating = true;
            Update();
            WrapImage::Evaluate();
        }
        else {
            WrapImage::Evaluate();
        }
        bUpdating = false;
    }
};

class ImageTransform {
public:
    static TRef<Image> Translate(Image* pImage, PointValue* pPoint);
    static TRef<Image> Scale(Image* pImage, PointValue* pPoint);
    static TRef<Image> Rotate(Image* pImage, Number* pNumberRadians);

    static TRef<PointValue> Size(Image* pImage);
    static TRef<RectValue> Bounds(Image* pImage);

    static TRef<Image> Justify(Image* pImage, PointValue* pContainer, Justification justification);
    static TRef<Image> ScaleFit(Image* pImage, PointValue* pContainer, Justification justification);
    static TRef<Image> ScaleFill(Image* pImage, PointValue* pContainer, Justification justification);

    static TRef<Image> Switch(Boolean* pValue, std::map<bool, TRef<Image>> mapOptions);
    static TRef<Image> Switch(Number* pValue, std::map<int, TRef<Image>> mapOptions);
    static TRef<Image> Switch(TStaticValue<ZString>* pValue, std::map<std::string, TRef<Image>> mapOptions);

    static TRef<Image> Clip(Image* pImage, RectValue* pRect);
    static TRef<Image> Cut(Image* pImage, RectValue* pRect);

    static TRef<Image> Multiply(Image* pImage, ColorValue* pColor);

    static TRef<Image> Lazy(std::function<TRef<Image>()> callback);

    static TRef<Image> String(FontValue* font, ColorValue* color, Number* width, StringValue* string, Justification justification);
};