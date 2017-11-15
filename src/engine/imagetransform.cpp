
#include "imagetransform.h"
#include "image.h"

class ImageSize : public PointValue {
private:
    Image* GetImage() { return Image::Cast(GetChild(0)); }

public:
    ImageSize(Image* pimage) :
        PointValue(pimage)
    {
    }

    void Evaluate()
    {
        GetValueInternal() = Point::Cast(GetImage()->GetBounds().GetRect().Size());
    }
};

template<class OriginalType>
class CallbackImage : public WrapImage {
    typedef std::function<TRef<Image>(OriginalType)> CallbackType;

    CallbackType m_callback;

public:
    CallbackImage(CallbackType callback, TStaticValue<OriginalType>* pvalue1) :
        m_callback(callback),
        WrapImage(Image::GetEmpty(), pvalue1)
    {}

    void Evaluate()
    {
        OriginalType value1 = ((TStaticValue<OriginalType>*)GetChild(1))->GetValue();

        TRef<Image> evaluated = m_callback(value1);

        SetImage(evaluated);
        WrapImage::Evaluate();
    }
};

class CutImage : public WrapImage {
public:
    CutImage(Image* pimage, RectValue* prect) :
        WrapImage(pimage, prect)
    {
    }

    RectValue* GetRect() { return RectValue::Cast(GetChild(1)); }

    void CalcBounds()
    {
        m_bounds = Rect(Point(0, 0), GetRect()->GetValue().Size());
    }

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
        Rect rect = GetRect()->GetValue();
        if (Rect(Point(0, 0), m_bounds.GetRect().Size()).Inside(point)) {
            return MouseResultHit();
        }

        return MouseResult();
    }

    void Render(Context* pcontext)
    {
        pcontext->Clip(Rect(Point(0, 0), GetRect()->GetValue().Size()));
        pcontext->Translate(-GetRect()->GetValue().Min());
        WrapImage::Render(pcontext);
    }

    ZString GetFunctionName() { return "ClipImage"; }

};

TRef<Image> ImageTransform::Translate(Image* pImage, PointValue* pPoint) {
    return new TransformImage(
        pImage,
        new TranslateTransform2(pPoint)
    );
};

TRef<Image> ImageTransform::Scale(Image* pImage, PointValue* pPoint) {
    return new TransformImage(
        pImage,
        new ScaleTransform2(pPoint)
    );
};

TRef<Image> ImageTransform::Rotate(Image* pImage, Number* pNumberRadians) {
    return new TransformImage(
        pImage,
        new RotateTransform2(pNumberRadians)
    );
}

TRef<PointValue> ImageTransform::Size(Image* pImage) {
    return new ImageSize(pImage);
};

TRef<Image> ImageTransform::Justify(Image* pimage, PointValue* pSizeContainer, Justification justification) {
    TRef<PointValue> sizeImage = ImageTransform::Size(pimage);
    TRef<Number> pImageX = PointTransform::X(sizeImage);
    TRef<Number> pImageY = PointTransform::Y(sizeImage);

    TRef<Number> pContainerX = PointTransform::X(pSizeContainer);
    TRef<Number> pContainerY = PointTransform::Y(pSizeContainer);

    TRef<Number> pSpaceX = NumberTransform::Subtract(pContainerX, pImageX);
    TRef<Number> pSpaceY = NumberTransform::Subtract(pContainerY, pImageY);

    TRef<Number> pNumberZero = new Number(0.0f);
    TRef<Number> pNumberHalf = new Number(0.5f);

    TRef<Number> pOffsetX;
    TRef<Number> pOffsetY;

    if (justification.Test(JustifyLeft())) {
        pOffsetX = pNumberZero;
    }
    else if (justification.Test(JustifyRight())) {
        pOffsetX = pSpaceX;
    }
    else {
        pOffsetX = NumberTransform::Multiply(pNumberHalf, pSpaceX);
    }

    if (justification.Test(JustifyTop())) {
        pOffsetY = pSpaceY;
    }
    else if (justification.Test(JustifyBottom())) {
        pOffsetY = pNumberZero;
    }
    else {
        pOffsetY = NumberTransform::Multiply(pNumberHalf, pSpaceY);
    }

    TRef<PointValue> pPointTranslate = PointTransform::Create(pOffsetX, pOffsetY);

    return (TRef<Image>)new TransformImage(
        pimage,
        new TranslateTransform2(pPointTranslate)
    );
}

TRef<Image> ImageTransform::ScaleFit(Image* pimage, PointValue* pSizeContainer, Justification justification) {
    //this should maybe be redone to return a custom class instead of using so many wrappers, it's a cool example though

    TRef<PointValue> sizeImage = ImageTransform::Size(pimage);
    TRef<Number> pImageX = PointTransform::X(sizeImage);
    TRef<Number> pImageY = PointTransform::Y(sizeImage);

    TRef<Number> pContainerX = PointTransform::X(pSizeContainer);
    TRef<Number> pContainerY = PointTransform::Y(pSizeContainer);

    // What scaling factor would we need for each axis? Pick the smallest one
    TRef<Number> pScale = NumberTransform::Min(
        NumberTransform::Divide(pContainerX, pImageX),
        NumberTransform::Divide(pContainerY, pImageY)
    );

    TRef<PointValue> pPointScale = PointTransform::Create(pScale, pScale);

    return ImageTransform::Justify(
        new TransformImage(
            pimage,
            new ScaleTransform2(pPointScale)
        ),
        pSizeContainer,
        justification
    );
}

TRef<Image> ImageTransform::ScaleFill(Image* pimage, PointValue* pSizeContainer, Justification justification) {
    //this should maybe be redone to return a custom class instead of using so many wrappers, it's a cool example though

    TRef<PointValue> sizeImage = ImageTransform::Size(pimage);
    TRef<Number> pImageX = PointTransform::X(sizeImage);
    TRef<Number> pImageY = PointTransform::Y(sizeImage);

    TRef<Number> pContainerX = PointTransform::X(pSizeContainer);
    TRef<Number> pContainerY = PointTransform::Y(pSizeContainer);

    // What scaling factor would we need for each axis? Pick the largest one
    TRef<Number> pScale = NumberTransform::Max(
        NumberTransform::Divide(pContainerX, pImageX),
        NumberTransform::Divide(pContainerY, pImageY)
    );

    TRef<PointValue> pPointScale = PointTransform::Create(pScale, pScale);

    return ImageTransform::Justify(
        new TransformImage(
            pimage,
            new ScaleTransform2(pPointScale)
        ),
        pSizeContainer,
        justification
    );
}

TRef<Image> ImageTransform::Switch(Boolean* pValue, std::map<bool, TRef<Image>> mapOptions) {
    return new CallbackImage<bool>([mapOptions](bool value) {
        auto find = mapOptions.find(value);
        if (find == mapOptions.end()) {
            return (TRef<Image>)Image::GetEmpty();
        }
        return find->second;
    }, pValue);
}

TRef<Image> ImageTransform::Switch(Number* pValue, std::map<int, TRef<Image>> mapOptions) {
    return new CallbackImage<float>([mapOptions](float value) {
        auto find = mapOptions.find((int)value); //convert to integer
        if (find == mapOptions.end()) {
            return (TRef<Image>)Image::GetEmpty();
        }
        return find->second;
    }, pValue);
}

TRef<Image> ImageTransform::Switch(TStaticValue<ZString>* pValue, std::map<std::string, TRef<Image>> mapOptions) {
    return new CallbackImage<ZString>([mapOptions](ZString value) {
        auto find = mapOptions.find(std::string(value));
        if (find == mapOptions.end()) {
            return (TRef<Image>)Image::GetEmpty();
        }
        return find->second;
    }, pValue);
}

TRef<Image> ImageTransform::Clip(Image* pImage, RectValue* pRect) {
    return CreateClipImage(pImage, pRect);
};

TRef<Image> ImageTransform::Cut(Image* pImage, RectValue* pRect) {
    return new CutImage(pImage, pRect);
};

TRef<Image> ImageTransform::Multiply(Image* pImage, ColorValue* pColor) {
    //ugly cast here
    ConstantImage* pConstantImage = (ConstantImage*)(pImage);

    return CreateConstantImage3D(pImage->GetSurface(), pColor);
};