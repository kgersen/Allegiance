
#pragma once

#include "ui.h"
#include "enginep.h"
#include "font.h"
#include "valuetransform.h"
#include "imagetransform.h"

typedef TEvent<Point> PointEvent;

class MouseEventImage : public WrapImage {
    std::map<std::string, TRef<EventSourceImpl>> m_mapEventSources;
    std::map<std::string, TRef<PointEvent::SourceImpl>> m_mapPointEventSources;
    bool m_bDown = false;
    Point m_pointDown;

public:

    MouseEventImage(Image* wrapped) :
        WrapImage(wrapped)
    {
    }

    IEventSource* GetEventSource(std::string string) {
        auto found = m_mapEventSources.find(string);
        if (found != m_mapEventSources.end()) {
            return found->second;
        }

        TRef<EventSourceImpl> pEventSource = new EventSourceImpl();
        m_mapEventSources[string] = pEventSource;
        return pEventSource;
    }

    PointEvent::Source* GetPointEventSource(std::string string) {
        auto found = m_mapPointEventSources.find(string);
        if (found != m_mapPointEventSources.end()) {
            return found->second;
        }

        TRef<PointEvent::SourceImpl> pEventSource = new PointEvent::SourceImpl();
        m_mapPointEventSources[string] = pEventSource;
        return pEventSource;
    }

    void Trigger(std::string string) {
        auto found = m_mapEventSources.find(string);
        if (found != m_mapEventSources.end()) {
            found->second->Trigger();
        }
    }

    void TriggerPoint(std::string string, Point& point) {
        auto found = m_mapPointEventSources.find(string);
        if (found != m_mapPointEventSources.end()) {
            found->second->Trigger(point);
        }
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point) {
        Trigger("mouse.enter");
    }

    void MouseLeave(IInputProvider* pprovider) {
        m_bDown = false; //reset state
        Trigger("mouse.leave");
    }

    void TriggerMouseButton(std::string button_name, std::string what) {
        Trigger("mouse." + button_name + "." + what);
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside) override
    {
        if (m_bDown) {
            TriggerPoint("drag", point - m_pointDown);
            m_pointDown = point;
        }
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        // inspired by button.cpp~:~690
        if (button != 0 && button != 1) {
            return MouseResult();
        }
        std::string button_name = button == 0 ? "left" : "right";

        if (bDown) {
            m_bDown = true;
            m_pointDown = point;
            TriggerMouseButton(button_name, "down");

            if (pprovider->IsDoubleClick()) {
                TriggerMouseButton(button_name, "doubleclick");
            }
        }
        else {
            bool wasDown = m_bDown;
            m_bDown = false;
            TriggerMouseButton(button_name, "up");

            if (wasDown) {
                TriggerPoint("drag", point - m_pointDown);
                TriggerMouseButton(button_name, "click");
            }
        }

        return MouseResult();
    }
};

template<typename A>
TRef<TStaticValue<A>> wrapValue(sol::object a) {
    TStaticValue<A>* converted_a;
    if (a.is<A>()) {
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    else if (a.is<TStaticValue<A>*>()) {
        converted_a = a.as<TStaticValue<A>*>();
    }
    else {
        // force a cast, this sometimes still works, exception otherwise
        converted_a = new TStaticValue<A>(a.as<A>());
    }
    TRef<TStaticValue<A>> refcounted = converted_a;
    return refcounted;
};

TRef<StringValue> wrapString(sol::object a) {
    if (a.is<std::string>()) {
        return (TRef<StringValue>)new StringValue(ZString(a.as<std::string>().c_str()));
    }
    return wrapValue<ZString>(a);
};