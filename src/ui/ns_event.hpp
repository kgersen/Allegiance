
#pragma once

#include "ui.h"
#include "items.hpp"

class EventToBoolean : public ModifiableBoolean, IEventSink {
    TRef<IEventSource> m_pEnableSource;
    TRef<IEventSource> m_pDisableSource;

public:
    EventToBoolean(IEventSource* pEnableSource, IEventSource* pDisableSource) :
        m_pEnableSource(pEnableSource),
        m_pDisableSource(pDisableSource),
        ModifiableBoolean(false)
    {
        pEnableSource->AddSink(this);
        pDisableSource->AddSink(this);
    }

    ~EventToBoolean() {
        m_pEnableSource->RemoveSink(this);
        m_pDisableSource->RemoveSink(this);
    }

    bool OnEvent(IEventSource* source) {
        if (source == m_pEnableSource) {
            SetValue(true);
        }
        else if (source == m_pDisableSource) {
            SetValue(false);
        }
        else {
            ZAssert(false);
        }
        return true;
    }
};

template<typename TypeResult>
class EventToMappedValue : public TWrapValue<TypeResult>, IEventSink {
    typedef TRef<TStaticValue<TypeResult>> TypeWrappedResult;

    std::map<TRef<IEventSource>, TypeWrappedResult> m_mapOptions;

public:
    EventToMappedValue(TypeWrappedResult tDefault, std::map<TRef<IEventSource>, TypeWrappedResult> mapOptions) :
        m_mapOptions(mapOptions),
        TWrapValue(tDefault)
    {
        for (auto kv = m_mapOptions.begin(); kv != m_mapOptions.end(); ++kv)
        {
            kv->first->AddSink(this);
        }
    }

    ~EventToMappedValue() {
        for (auto kv = m_mapOptions.begin(); kv != m_mapOptions.end(); ++kv)
        {
            kv->first->RemoveSink(this);
        }
    }

    bool OnEvent(IEventSource* source) {
        auto find = m_mapOptions.find(source);
        if (find == m_mapOptions.end()) {
            ZAssert(false);
        }
        else {
            SetWrappedValue(find->second);
        }
        return true;
    }
};

class EventToMappedImage : public WrapImage, IEventSink {
    typedef TRef<Image> TypeWrappedResult;

    std::map<TRef<IEventSource>, TypeWrappedResult> m_mapOptions;

public:
    EventToMappedImage(Image* pImage, std::map<TRef<IEventSource>, TypeWrappedResult> mapOptions) :
        m_mapOptions(mapOptions),
        WrapImage(pImage)
    {
        for (auto kv = m_mapOptions.begin(); kv != m_mapOptions.end(); ++kv)
        {
            kv->first->AddSink(this);
        }
    }

    ~EventToMappedImage() {
        for (auto kv = m_mapOptions.begin(); kv != m_mapOptions.end(); ++kv)
        {
            kv->first->RemoveSink(this);
        }
    }

    bool OnEvent(IEventSource* source) {
        auto find = m_mapOptions.find(source);
        if (find == m_mapOptions.end()) {
            ZAssert(false);
        }
        else {
            SetImage(find->second);
        }
        return true;
    }
};

template <typename TypeResult, typename TypeOriginal>
class EventOneToOne {
    typedef TRef<TStaticValue<TypeResult>> WrappedResult;
    typedef TRef<TStaticValue<TypeOriginal>> WrappedOriginal;

public:

    static auto CreateOnEventPropagatorFunction() {


        return [](TEvent<TypeResult>::Sink* pEventSink, TEvent<TypeOriginal>::Source* pEventSource, std::function<WrappedResult(WrappedOriginal)> transformer) {
            TypeOriginal original_default_value;
            TRef<SimpleModifiableValue<TypeOriginal>> start = new SimpleModifiableValue<TypeOriginal>(original_default_value);
            WrappedResult end = transformer(start);

            TRef<TEvent<TypeResult>::Sink> sinkRefCounted = pEventSink;

            pEventSource->AddSink(new CallbackValueSink<TypeOriginal>([start, end, sinkRefCounted](TypeOriginal value) {
                start->SetValue(value);
                end->Update();
                sinkRefCounted->OnEvent(nullptr, end->GetValue());
                return true;
            }));
        };
    }
};

class EventNamespace {
public:
    static void AddNamespace(sol::state* m_pLua) {
        sol::table table = m_pLua->create_table();

        table["OnEvent"] = sol::overload(
            [](IEventSink* pEventSink, IEventSource* pEventSource) {
                if (!pEventSink || !pEventSource) {
                    throw std::runtime_error("Argument should not be null");
                }
                pEventSource->AddSink(pEventSink);
            },
            EventOneToOne<float, float>::CreateOnEventPropagatorFunction(),
            EventOneToOne<float, Point>::CreateOnEventPropagatorFunction(),
            EventOneToOne<Point, float>::CreateOnEventPropagatorFunction(),
            EventOneToOne<Point, Point>::CreateOnEventPropagatorFunction()
        );

        table["Get"] = [](Image* image, std::string string) {
            if (!image) {
                throw std::runtime_error("Argument should not be null");
            }
            MouseEventImage* pMouseEventImage = (MouseEventImage*)(image);
            return pMouseEventImage->GetEventSource(string);
        };

        table["GetPoint"] = [](Image* image, std::string string) {
            if (!image) {
                throw std::runtime_error("Argument should not be null");
            }
            MouseEventImage* pMouseEventImage = (MouseEventImage*)(image);
            return pMouseEventImage->GetPointEventSource(string);
        };

        table["ToBoolean"] = [](sol::table table, sol::object valueDefault) {
            std::map<TRef<IEventSource>, TRef<Boolean>> mapOptions;

            table.for_each([&mapOptions](sol::object key, sol::object value) {
                TRef<IEventSource> mapKey = key.as<IEventSource*>();
                mapOptions[mapKey] = wrapValue<bool>(value);
            });
            
            return (TRef<Boolean>)new EventToMappedValue<bool>(wrapValue<bool>(valueDefault), mapOptions);
        };

        table["ToNumber"] = [](sol::table table, sol::object valueDefault) {
            std::map<TRef<IEventSource>, TRef<Number>> mapOptions;

            table.for_each([&mapOptions](sol::object key, sol::object value) {
                TRef<IEventSource> mapKey = key.as<IEventSource*>();
                mapOptions[mapKey] = wrapValue<float>(value);
            });

            return (TRef<Number>)new EventToMappedValue<float>(wrapValue<float>(valueDefault), mapOptions);
        };

        table["ToString"] = [](sol::table table, sol::object valueDefault) {
            std::map<TRef<IEventSource>, TRef<StringValue>> mapOptions;

            table.for_each([&mapOptions](sol::object key, sol::object value) {
                TRef<IEventSource> mapKey = key.as<IEventSource*>();
                mapOptions[mapKey] = wrapString(value);
            });

            return (TRef<StringValue>)new EventToMappedValue<ZString>(wrapString(valueDefault), mapOptions);
        };

        table["ToImage"] = [](sol::table table, Image* valueDefault) {
            std::map<TRef<IEventSource>, TRef<Image>> mapOptions;

            table.for_each([&mapOptions](sol::object key, sol::object value) {
                TRef<IEventSource> mapKey = key.as<IEventSource*>();
                mapOptions[mapKey] = value.as<Image*>();
            });

            return (TRef<Image>)new EventToMappedImage(valueDefault, mapOptions);
        };

        m_pLua->set("Event", table);
    }
};