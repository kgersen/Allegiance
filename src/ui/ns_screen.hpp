
#pragma once

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

class ScreenNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();

        table["GetString"] = [&context](std::string name) {
            return context.GetScreenGlobals().Get<TRef<StringValue>>(name);
        };
        table["GetNumber"] = [&context](std::string name) {
            return context.GetScreenGlobals().Get<TRef<Number>>(name);
        };
        table["GetState"] = [&context](std::string name) {
            return context.GetScreenGlobals().Get<TRef<UiStateValue>>(name);
        };

        context.GetLua().new_usertype<UiObjectContainer>("UiObjectContainer",
            "new", sol::no_constructor,
            "GetString", &UiState::Get<TRef<StringValue>>,
            "GetNumber", &UiState::Get<TRef<Number>>,
            "GetState", &UiState::Get<TRef<UiStateValue>>,
            "GetEventSink", &UiState::Get<TRef<IEventSink>>,
            "GetList", &UiState::Get<std::list<TRef<UiObjectContainer>>>
        );

        context.GetLua().new_usertype<UiState>("UiState", 
            "new", sol::no_constructor,
            sol::base_classes, sol::bases<UiObjectContainer>()
        );

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
                return true;
            });
        };
        
        table["GetResolution"] = [&context]() {
            return (TRef<PointValue>)new TransformedValue<Point, WinPoint>([](WinPoint winpoint) {
                return Point((float)winpoint.X(), (float)winpoint.Y());
            }, context.GetEngine()->GetResolutionSizeModifiable());
        };

        context.GetLua().set("Screen", table);
    }
};
