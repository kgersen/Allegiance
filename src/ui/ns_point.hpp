
#pragma once

#include "ui.h"
#include "items.hpp"

using namespace std::literals;

class PointNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        m_pLua->new_usertype<EventValue<Point>>("EventValue<Point>",
            sol::base_classes, sol::bases<PointValue, TEvent<Point>::Sink>()
        );
        
        table["Create"] = [](sol::object x, sol::object y) {
            return PointTransform::Create(
                wrapValue<float>(x),
                wrapValue<float>(y)
            );
        };

        table["CreateEventPoint"] = [](float x, float y) {
            return (TRef<EventValue<Point>>)new EventValue<Point>(Point(x, y), [](const Point& old, const Point& eventPoint) {
                return eventPoint;
            });
        };

        table["CreateEventPointAdd"] = [](float x, float y) {
            return (TRef<EventValue<Point>>)new EventValue<Point>(Point(x, y), [](const Point& old, const Point& eventPoint) {
                return old + eventPoint;
            });
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
