
#pragma once

#include "ui.h"
#include "items.hpp"

template<typename TypeResult, typename A, typename B>
auto createAutoWrappingFunction(std::function<TypeResult(A, B)> callback) {

    return [callback](sol::object a, sol::object b) {
        return (TStaticValue<TypeResult>*)new TransformedValue2<TypeResult, A, B>(callback, wrapValue<A>(a), wrapValue<B>(b));
    };
};

class NumberNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        m_pLua->new_usertype<EventValue<float>>("EventValue<float>",
            sol::base_classes, sol::bases<Number, TEvent<float>::Sink>()
        );

        table["Create"] = [](float a) {
            return new Number(a);
        };

        table["CreateEventSink"] = [](float a) {
            return (TRef<EventValue<float>>)new EventValue<float>(a, [](float fOld, float fNew) {
                return fNew;
            });
        };

        table["ToString"] = [](sol::object a, sol::optional<int> decimals) {
            
            return NumberTransform::ToString(wrapValue<float>(a), decimals.value_or(0));
        };

        table["Add"] = [](sol::object a, sol::object b) {
            return NumberTransform::Add(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Subtract"] = [](sol::object a, sol::object b) {
            return NumberTransform::Subtract(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Multiply"] = [](sol::object a, sol::object b) {
            return NumberTransform::Multiply(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Divide"] = [](sol::object a, sol::object b) {
            return NumberTransform::Divide(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Mod"] = [](sol::object a, sol::object b) {
            return NumberTransform::Mod(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Min"] = [](sol::object a, sol::object b) {
            return NumberTransform::Min(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Max"] = [](sol::object a, sol::object b) {
            return NumberTransform::Max(wrapValue<float>(a), wrapValue<float>(b));
        };
        table["Clamp"] = [](sol::object min, sol::object max, sol::object value) {
            return NumberTransform::Min(
                wrapValue<float>(max),
                NumberTransform::Max(wrapValue<float>(value), wrapValue<float>(min))
            );
        };
        table["Round"] = [](sol::object a, sol::optional<int> decimals) {
            return NumberTransform::Round(wrapValue<float>(a), decimals.value_or(0));
        };
        table["Sin"] = [](sol::object a) {
            return NumberTransform::Sin(wrapValue<float>(a));
        };
        table["Cos"] = [](sol::object a) {
            return NumberTransform::Cos(wrapValue<float>(a));
        };
        m_pLua->new_usertype<Number>("Number");

        m_pLua->set("Number", table);
    }
};

class RectNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        table["Create"] = sol::overload(
            [](int xmin, int ymin, int xmax, int ymax) {
                return (TRef<RectValue>)new RectValue(Rect(xmin, ymin, xmax, ymax));
            },
            [](sol::object xmin, sol::object ymin, sol::object xmax, sol::object ymax) {
                return RectTransform::Create(
                    wrapValue<float>(xmin),
                    wrapValue<float>(ymin),
                    wrapValue<float>(xmax),
                    wrapValue<float>(ymax)
                );
            }
        );

        m_pLua->set("Rect", table);
    }
};

class BooleanNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        table["And"] = [](sol::object a, sol::object b) {
            return (TRef<Boolean>)BooleanTransform::And(wrapValue<bool>(a), wrapValue<bool>(b));
        };
        table["Or"] = [](sol::object a, sol::object b) {
            return (TRef<Boolean>)BooleanTransform::Or(wrapValue<bool>(a), wrapValue<bool>(b));
        };
        table["Not"] = [](sol::object a) {
            return (TRef<Boolean>)BooleanTransform::Not(wrapValue<bool>(a));
        };

        m_pLua->set("Boolean", table);
    }
};