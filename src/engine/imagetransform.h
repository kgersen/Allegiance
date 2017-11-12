
#pragma once

#include "model.h"
#include "enginep.h"

class ImageTransform {
public:
    static TRef<Image> Translate(Image* pImage, PointValue* pPoint);
    static TRef<Image> Scale(Image* pImage, PointValue* pPoint);

    static TRef<PointValue> Size(Image* pImage);

    static TRef<Image> Justify(Image* pImage, PointValue* pContainer, Justification justification);
    static TRef<Image> ScaleFit(Image* pImage, PointValue* pContainer, Justification justification);
    static TRef<Image> ScaleFill(Image* pImage, PointValue* pContainer, Justification justification);

    static TRef<Image> Switch(Boolean* pValue, std::map<bool, TRef<Image>> mapOptions);
    static TRef<Image> Switch(Number* pValue, std::map<int, TRef<Image>> mapOptions);
    static TRef<Image> Switch(TStaticValue<ZString>* pValue, std::map<std::string, TRef<Image>> mapOptions);

    static TRef<Image> Clip(Image* pImage, RectValue* pRect);
    static TRef<Image> Cut(Image* pImage, RectValue* pRect);
};