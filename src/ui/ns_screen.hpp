
#pragma once

#include "pch.h"
#include "ui.h"
#include "items.hpp"

using namespace std::literals;

class PlaySoundSink : public IEventSink {
private:
    TRef<ISoundEngine> m_pSoundEngine;
    TRef<ISoundTemplate> m_pTemplate;

public:

    PlaySoundSink(ISoundEngine* pSoundEngine, ISoundTemplate* pTemplate) :
        m_pSoundEngine(pSoundEngine),
        m_pTemplate(pTemplate)
    {}

    bool OnEvent(IEventSource* pevent) {

        TRef<ISoundInstance> pSoundInstance;

        m_pTemplate->CreateSound(pSoundInstance, m_pSoundEngine->GetBufferSource(), NULL);

        return true;
    }
};

class CallbackSink : public IEventSink {
private:
    std::function<void()> m_funcCallback;

public:
    CallbackSink(std::function<void()> funcCallback) :
        m_funcCallback(funcCallback)
    {}

    bool OnEvent(IEventSource* pevent) {
        m_funcCallback();

        return true;
    }
};

class ScreenNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["GetExternalEventSink"] = [&context](std::string path) {
            IEventSink& sink = context.GetExternalEventSink(path);
            return (TRef<IEventSink>)&sink;
        };

        table["CreatePlaySoundSink"] = [&context](std::string path) {
            TRef<ISoundTemplate> pTemplate;

            std::string full_path = context.FindPath(path);

            ZSucceeded(CreateWaveFileSoundTemplate(pTemplate, full_path.c_str()));
            return (TRef<IEventSink>)new PlaySoundSink(context.GetSoundEngine(), pTemplate);
        };

        table["CreateOpenWebsiteSink"] = [&context](std::string strWebsite) {
            auto openWebsite = context.GetOpenWebsiteFunction();

            return (TRef<IEventSink>)new CallbackSink([openWebsite, strWebsite]() {
                openWebsite(strWebsite);
            });
        };
        
        table["GetResolution"] = [&context]() {
            return (TRef<PointValue>)new TransformedValue<Point, WinPoint>([](WinPoint winpoint) {
                return Point(winpoint.X(), winpoint.Y());
            }, context.GetEngine()->GetResolutionSizeModifiable());
        };

        context.GetLua().set("Screen", table);
    }
};
