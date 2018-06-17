#include "pch.h"
#include "valuetransform.h"

#include "ui.h"

class ConfigScreen :
    public Screen
{
private:
    TRef<UiEngine>           m_pUiEngine;
    TRef<UpdatingConfiguration> m_pConfiguration;
    TRef<Image>         m_pimage;

    TRef<IEventSink> m_pEventSinkClose;

public:
    Image* GetImage()
    {
        return m_pimage;
    }

    ConfigScreen(UiEngine* pUiEngine, UpdatingConfiguration* pconfiguration, IEventSink* pEventSinkClose) :
        m_pUiEngine(pUiEngine),
        m_pConfiguration(pconfiguration),
        m_pEventSinkClose(pEventSinkClose)
    {
        std::map<std::string, std::shared_ptr<Exposer>> map;
        map["Configuration.Graphics.Fullscreen"] = TypeExposer<TRef<SimpleModifiableValue<bool>>>::Create(m_pConfiguration->GetBool("Graphics.Fullscreen", true));

        map["Close sink"] = TypeExposer<TRef<IEventSink>>::Create(m_pEventSinkClose);

        m_pimage = pUiEngine->LoadImageFromLua(UiScreenConfiguration::Create("menuconfig/configscreen.lua", map));
    }

    ~ConfigScreen() {
    }

    Pane* GetPane()
    {
        return nullptr;
    }
};

TRef<Screen> CreateConfigScreen(UiEngine* pUiEngine, UpdatingConfiguration* pconfiguration, IEventSink* pEventSinkClose)
{
    return new ConfigScreen(pUiEngine, pconfiguration, pEventSinkClose);
}


