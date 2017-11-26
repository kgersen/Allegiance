
#pragma once

#include "ui.h"
#include "items.hpp"

class ListNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["MapToImages"] = [&context](std::list<TRef<UiObjectContainer>> list, sol::function callback) {
            auto wrapped_callback = context.WrapCallback<TRef<Image>, TRef<UiObjectContainer>, TRef<Number>>(callback, Image::GetEmpty());

            std::list<TRef<Image>> result;
            int i = 0;
            for (auto entry : list) {
                TRef<Number> index = new Number((float)i);
                result.push_back(wrapped_callback(entry, index));
                ++i;
            }
            return result;
        };

        context.GetLua().set("List", table);
    }
};
