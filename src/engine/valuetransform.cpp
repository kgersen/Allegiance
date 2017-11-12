
#include "valuetransform.h"


class SubtractNumber : public Number {
public:
    SubtractNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = Get0()->GetValue() - Get1()->GetValue();
    }
};

class AddNumber : public Number {
public:
    AddNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = Get0()->GetValue() + Get1()->GetValue();
    }
};

class MultiplyNumber : public Number {
public:
    MultiplyNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = Get0()->GetValue() * Get1()->GetValue();
    }
};

class DivideNumber : public Number {
public:
    DivideNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = Get0()->GetValue() / Get1()->GetValue();
    }
};

class SinNumber : public Number {
public:
    SinNumber(Number* pvalue0) :
        Number(pvalue0)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }

    void Evaluate()
    {
        GetValueInternal() = sin(Get0()->GetValue());
    }
};

class CosNumber : public Number {
public:
    CosNumber(Number* pvalue0) :
        Number(pvalue0)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }

    void Evaluate()
    {
        GetValueInternal() = cos(Get0()->GetValue());
    }
};

class ModNumber : public Number {
public:
    ModNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = mod(Get0()->GetValue(), Get1()->GetValue());
    }
};

class MinNumber : public Number {
public:
    MinNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = std::min(Get0()->GetValue(), Get1()->GetValue());
    }
};

class MaxNumber : public Number {
public:
    MaxNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() = std::max(Get0()->GetValue(), Get1()->GetValue());
    }
};

TRef<Number> NumberTransform::Round(Number* pNumber, int decimals)
{
    return new TransformedValue<float, float>([decimals](float a) {
        float multiplier = pow(10, decimals);
        return round(a * multiplier) / multiplier;
    }, pNumber);
}

TRef<Number> NumberTransform::Subtract(Number* pvalue1, Number* pvalue2)
{
    return new SubtractNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Add(Number* pvalue1, Number* pvalue2)
{
    return new AddNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Multiply(Number* pvalue1, Number* pvalue2)
{
    return new MultiplyNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Divide(Number* pvalue1, Number* pvalue2)
{
    return new DivideNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Mod(Number* pvalue1, Number* pvalue2)
{
    return new ModNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Min(Number* pvalue1, Number* pvalue2)
{
    return new MinNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Max(Number* pvalue1, Number* pvalue2)
{
    return new MaxNumber(pvalue1, pvalue2);
}

TRef<Number> NumberTransform::Sin(Number* pvalue)
{
    return new SinNumber(pvalue);
}

TRef<Number> NumberTransform::Cos(Number* pvalue)
{
    return new CosNumber(pvalue);
};


// ### Point ###

class PointV : public PointValue {
public:
    PointV(Number* px, Number* py) :
        PointValue(px, py)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() =
            Point(
                Get0()->GetValue(),
                Get1()->GetValue()
            );
    }
};

class PointX : public Number {
private:
    PointValue* GetPoint() { return PointValue::Cast(GetChild(0)); }

public:
    PointX(PointValue* ppoint) :
        Number(ppoint)
    {
    }

    void Evaluate()
    {
        GetValueInternal() = GetPoint()->GetValue().X();
    }
};

class PointY : public Number {
private:
    PointValue* GetPoint() { return PointValue::Cast(GetChild(0)); }

public:
    PointY(PointValue* ppoint) :
        Number(ppoint)
    {
    }

    void Evaluate()
    {
        GetValueInternal() = GetPoint()->GetValue().Y();
    }
};

TRef<PointValue> PointTransform::Create(Number* x, Number* y) {
    return (PointValue*)new PointV(
        x,
        y
    );
};

TRef<Number> PointTransform::X(PointValue* ppoint) {
    return new PointX(ppoint);
};

TRef<Number> PointTransform::Y(PointValue* ppoint) {
    return new PointY(ppoint);
};


// ### Color

TRef<ColorValue> ColorTransform::Create(Number* r, Number* g, Number* b, Number* a) {
    return new TransformedValue4<Color, float, float, float, float>(
        [](float r, float g, float b, float a) {
            return Color(r, g, b, a);
        },
        r,
        g,
        b,
        a
    );
};


// ### Rect

TRef<RectValue> RectTransform::Create(Number* xmin, Number* ymin, Number* xmax, Number* ymax) {
    return new TransformedValue4<Rect, float, float, float, float>(
        [](float xmin, float ymin, float xmax, float ymax) {
            return Rect(xmin, ymin, xmax, ymax);
        },
        xmin,
        ymin,
        xmax,
        ymax
    );
};

TRef<RectValue> RectTransform::Create(Number* width, Number* height) {
    return new TransformedValue2<Rect, float, float>(
        [](float width, float height) {
            return Rect(0, 0, width, height);
        },
        width,
        height
    );
};

TRef<RectValue> RectTransform::Create(PointValue* pPoint) {
    return new TransformedValue<Rect, Point>(
        [](Point point) {
            return Rect(0, 0, point.X(), point.Y());
        },
        pPoint
    );
};