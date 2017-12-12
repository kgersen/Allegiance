#pragma once

#ifndef __MODULE__
#define __MODULE__ "Ui"
#endif

#include "soundengine.h"
#include "model.h"
#include "image.h"
#include "engine.h"

#include "UiState.h"

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ENABLE_INTEROP 1

#include "sol.hpp"

#include "ui_types.h"


class CallbackSink : public IEventSink {
private:
    std::function<bool()> m_funcCallback;

public:
    CallbackSink(std::function<bool()> funcCallback) :
        m_funcCallback(funcCallback)
    {}

    bool OnEvent(IEventSource* pevent) {
        return m_funcCallback();
    }
};

template <class ...Types>
class CallbackValueSink : public TEvent<Types...>::Sink {
private:
    std::function<bool(Types...)> m_funcCallback;

public:
    CallbackValueSink(std::function<bool(Types...)> funcCallback) :
        m_funcCallback(funcCallback)
    {}

    bool OnEvent(typename TEvent<Types...>::Source* pevent, Types ... values) override {
        return m_funcCallback(values...);
    }
};

#include "UiEngine.h"
