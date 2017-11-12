
#pragma once

#include "pch.h"
#include "ui.h"
#include "items.hpp"

class ColorNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();
        /*table["Create"] = sol::overload(
            [](float r, float g, float b, sol::optional<float> alpha) {
                return new ColorValue(Color(r, g, b, alpha.value_or(1.0f)));
            },
            [](sol::object r, sol::object g, sol::object b, sol::optional<sol::object> alpha) {
                TRef<Number> pWrappedAlpha = nullptr;
                if (alpha) {
                    pWrappedAlpha = wrapValue<float>(alpha.value());
                }
                return ColorTransform::Create(
                    wrapValue<float>(r),
                    wrapValue<float>(g),
                    wrapValue<float>(b),
                    pWrappedAlpha
                );
            }
        );
        table["Create"] = [](float r, float g, float b, sol::optional<float> alpha) {
            return new ColorValue(Color(r, g, b, alpha.value_or(1.0f)));
        };*/
        table["Create"] = [](sol::object r, sol::object g, sol::object b, sol::optional<sol::object> alpha) {
            TRef<Number> pWrappedAlpha;
            if (alpha) {
                pWrappedAlpha = wrapValue<float>(alpha.value());
            }
            else {
                pWrappedAlpha = new Number(1.0f);
            }
            return ColorTransform::Create(
                wrapValue<float>(r),
                wrapValue<float>(g),
                wrapValue<float>(b),
                pWrappedAlpha
            );
        };

        m_pLua->set("Color", table);
    }
};
