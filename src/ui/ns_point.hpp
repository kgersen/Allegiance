
#pragma once

#include "ui.h"
#include "items.hpp"

using namespace std::literals;

class PointNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        
        table["Create"] = [](sol::object x, sol::object y) {
            return PointTransform::Create(
                wrapValue<float>(x),
                wrapValue<float>(y)
            );
        };
        
        table["X"] = [](PointValue* pPoint) {
            if (!pPoint) {
                throw std::runtime_error("Argument should not be null");
            }
            return PointTransform::X(pPoint);
        };
        table["Y"] = [](PointValue* pPoint) {
            if (!pPoint) {
                throw std::runtime_error("Argument should not be null");
            }
            return PointTransform::Y(pPoint);
        };
        m_pLua->set("Point", table);
    }
};
