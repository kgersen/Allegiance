#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Material
//
//////////////////////////////////////////////////////////////////////////////

class MaterialImpl : public Material {
private:
    void SetDiffuse(ColorValue* pvalue)  { SetChild(0, pvalue); }
    void SetSpecular(ColorValue* pvalue) { SetChild(1, pvalue); }
    void SetEmissive(ColorValue* pvalue) { SetChild(2, pvalue); }
    void SetPower(Number* pvalue)        { SetChild(3, pvalue); }

    ColorValue* GetDiffuseValue()  const { return ColorValue::Cast(GetChild(0)); }
    ColorValue* GetSpecularValue() const { return ColorValue::Cast(GetChild(1)); }
    ColorValue* GetEmissiveValue() const { return ColorValue::Cast(GetChild(2)); }
    Number*     GetPowerValue()    const { return     Number::Cast(GetChild(3)); }

public:
    MaterialImpl(
        ColorValue* pcolorDiffuse,
        ColorValue* pcolorSpecular,
        ColorValue* pcolorEmissive,
        Number*     power
    ) :
        Material(pcolorDiffuse, pcolorSpecular, pcolorEmissive, power)
    {
    }

    MaterialImpl(
        const Color& colorDiffuse,
        const Color& colorSpecular,
        const Color& colorEmissive,
        float ppower
    ) :
        Material(
            new ColorValue(colorDiffuse),
            new ColorValue(colorSpecular),
            new ColorValue(colorEmissive),
            new Number(ppower)
        )
    {
    }

    Color GetDiffuse()  const { return GetDiffuseValue()->GetValue();  }
    Color GetSpecular() const { return GetSpecularValue()->GetValue(); }
    Color GetEmissive() const { return GetEmissiveValue()->GetValue(); }
	float GetPower() const { return GetPowerValue()->GetValue(); }

    void SetDiffuse(const Color& color)  {  SetDiffuse(new ColorValue(color));  }
    void SetSpecular(const Color& color) { SetSpecular(new ColorValue(color)); }
    void SetEmissive(const Color& color) { SetEmissive(new ColorValue(color)); }

    //
    // Value members
    //

    ZString GetFunctionName() { return "Material"; }
};

TRef<Material> CreateMaterial(
    const Color& colorDiffuse,
    const Color& colorSpecular,
    const Color& colorEmissive,
    float power
) {
    return new MaterialImpl(colorDiffuse, colorSpecular, colorEmissive, power);
}

TRef<Material> CreateMaterial(
    ColorValue* pcolorDiffuse,
    ColorValue* pcolorSpecular,
    ColorValue* pcolorEmissive,
    Number*     ppower
) {
    return new MaterialImpl(pcolorDiffuse, pcolorSpecular, pcolorEmissive, ppower);
}
