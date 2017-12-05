
#pragma once

#include "ui.h"
#include "items.hpp"

template <typename ResultEntryType, typename OriginalEntryType>
class MappedList : public UiList<ResultEntryType> {
private:
    TRef<UiList<OriginalEntryType>> m_sourceList;
    std::function<ResultEntryType(OriginalEntryType, TRef<Number>)> m_callback;

public:
    MappedList(UiList<OriginalEntryType>* list, std::function<ResultEntryType(OriginalEntryType, TRef<Number>)> callback) :
        m_sourceList(list),
        m_callback(callback),
        UiList({})
    {}

    void Evaluate() override {
        std::list<ResultEntryType> list;
        int i = 0;
        for (auto entry : m_sourceList->GetList()) {
            TRef<Number> index = new Number((float)i);

            list.push_back(m_callback(entry, index));
            ++i;
        }

        GetListInternal() = list;
    }
};

class ListNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["MapToImages"] = [&context](TRef<ContainerList> list, sol::function callback) {
            auto wrapped_callback = context.WrapCallback<TRef<Image>, TRef<UiObjectContainer>, TRef<Number>>(callback, Image::GetEmpty());

            return (TRef<ImageList>)new MappedList<TRef<Image>, TRef<UiObjectContainer>>(list, wrapped_callback);
        };

        context.GetLua().set("List", table);
    }
};
