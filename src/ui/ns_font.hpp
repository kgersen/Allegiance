
#pragma once

#include "ui.h"
#include "items.hpp"

using namespace std::literals;

class FontNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        table["Create"] = [](std::string name, int size, sol::optional<sol::table> object) {
            std::map<std::string, bool> props = {
                { "Bold", false },
                { "Italic", false },
                { "Underline", false }
            };

            if (object) {
                object.value().for_each([&props](sol::object key, sol::object value) {
                    std::string strKey = key.as<std::string>();
                    if (props.find(strKey) == props.end()) {
                        throw std::exception("Unknown key. Use 'Bold', 'Italic', 'Underline'");
                    }
                    props[strKey] = value.as<bool>();
                });
            }

            return (TRef<FontValue>)new FontValue(
                CreateEngineFont(name, (int)size, 0, props["Bold"], props["Italic"], props["Underline"])
            );
        };

        table["Height"] = [](FontValue* font) {
            if (!font) {
                throw std::exception("Argument should not be null");
            }
            return (TRef<Number>)new TransformedValue<float, TRef<IEngineFont>>([](IEngineFont* font) {
                return (float)font->GetHeight();
            }, font);
        };

        m_pLua->set("Font", table);
    }
};
