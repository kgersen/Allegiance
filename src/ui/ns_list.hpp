
#pragma once

#include "ui.h"
#include "items.hpp"
#include "UiState.h"
#include <list>

template <typename OriginalEntryType>
class ListCount : public Number {

public:
    ListCount(TRef<UiList<OriginalEntryType>> list) :
        Number(list)
    {
    }

    TRef<UiList<OriginalEntryType>> GetSourceList() {
        return (TRef<UiList<OriginalEntryType>>)(UiList<OriginalEntryType>*)GetChild(0);
    }

    void Evaluate() override {
        const std::list<OriginalEntryType>& list = GetSourceList()->GetList();
        GetValueInternal() = (float)(list.size());
    }
};

class ListNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["Map"] = [&context](TRef<UiList<sol::object>> list, sol::function callback) {
            auto wrapped_callback = context.WrapCallback<sol::object, sol::object, TRef<Number>>(callback, sol::nil);

            return (TRef<UiList<sol::object>>)new MappedList<sol::object, sol::object>(list, wrapped_callback);
        };
        table["MapToImages"] = table["Map"];

        table["Count"] = [](TRef<UiList<sol::object>> list) {
            return (TRef<Number>)new ListCount<sol::object>(list);
        };

        context.GetLua().set("List", table);
    }
};
