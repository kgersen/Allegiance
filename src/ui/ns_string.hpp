
#pragma once

#include "ui.h"
#include "items.hpp"

template<typename TypeResult, typename TypeArgument>
class ValueToMappedValue : public TWrapValue<TypeResult> {
    typedef TStaticValue<TypeResult> TypeWrappedResult;
    typedef TStaticValue<TypeArgument> TypeWrappedArgument;

    TRef<TypeWrappedResult> m_default;
    std::map<TypeArgument, TRef<TypeWrappedResult>> m_mapOptions;

public:
    ValueToMappedValue(TypeWrappedArgument* tValue, std::map<TypeArgument, TRef<TypeWrappedResult>> mapOptions, TypeWrappedResult* tDefault) :
        m_default(tDefault),
        m_mapOptions(mapOptions),
        TWrapValue(tDefault)
    {
        AddChild(tValue);
    }

    void Evaluate()
    {
        TypeArgument value = ((TypeWrappedArgument*)GetChild(1))->GetValue();

        auto find = m_mapOptions.find(value);
        if (find == m_mapOptions.end()) {
            SetWrappedValue(m_default);
        }
        else {
            SetWrappedValue(find->second);
        }
        TWrapValue::Evaluate();
    }
};

class StringNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["Length"] = [](sol::object a) {
            return StringTransform::Length(wrapString(a));
        };

        table["Concat"] = [](sol::object a, sol::object b) {
            return StringTransform::Concat(wrapString(a), wrapString(b));
        };

        table["Switch"] = [](sol::object value, sol::table table, sol::optional<sol::object> valueDefault) {
            int count = table.size();

            TRef<StringValue> valueDefaultNonOptional;
            if (valueDefault) {
                valueDefaultNonOptional = wrapString(valueDefault.value());
            }
            else {
                valueDefaultNonOptional = new StringValue(ZString(""));
            }

            if (value.is<TStaticValue<ZString>>() || value.is<std::string>()) {
                //the wrapped value is a ZString, the unwrapped value a std::string
                std::map<ZString, TRef<StringValue>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object entry_value) {
                    std::string strKey = key.as<std::string>();
                    mapOptions[strKey.c_str()] = wrapString(entry_value);
                });

                return (TRef<StringValue>)new ValueToMappedValue<ZString, ZString>(wrapString(value), mapOptions, valueDefaultNonOptional);
            }
            else if (value.is<Number>() || value.is<float>()) {
                //float/int problems are likely
                std::map<float, TRef<StringValue>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object entry_value) {
                    int fKey = (int)key.as<float>();
                    mapOptions[fKey] = wrapString(entry_value);
                });

                return (TRef<StringValue>)new ValueToMappedValue<ZString, float>(wrapValue<float>(value), mapOptions, valueDefaultNonOptional);
            }
            else if (value.is<Boolean>() || value.is<bool>()) {
                std::map<bool, TRef<StringValue>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object entry_value) {
                    bool bKey = key.as<bool>();
                    mapOptions[bKey] = wrapString(entry_value);
                });

                return (TRef<StringValue>)new ValueToMappedValue<ZString, bool>(wrapValue<bool>(value), mapOptions, valueDefaultNonOptional);
            }
            throw std::runtime_error("Expected value argument of String.Switch to be either a wrapped or unwrapped bool, int, or string");
        };

        context.GetLua().new_usertype<StringValue>("StringValue", 
            sol::meta_function::concatenation, [](sol::object a, sol::object b) {
                return StringTransform::Concat(wrapString(a), wrapString(b));
            }
        );

        context.GetLua().set("String", table);
    }
};
