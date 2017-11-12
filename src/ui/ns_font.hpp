
#pragma once

#include "pch.h"
#include "ui.h"
#include "items.hpp"

using namespace std::literals;

class FontNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        table["Create"] = [](std::string name, int size) {
            return (TRef<FontValue>)new FontValue(
                CreateEngineFont(
                    CreateFont(
                    (int)size,
                        (int)0, 0, 0,
                        false ? FW_BOLD : FW_DONTCARE,
                        FALSE, FALSE, FALSE, ANSI_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN,
                        name.c_str()
                    )
                )
            );
        };
        m_pLua->set("Font", table);
    }
};
