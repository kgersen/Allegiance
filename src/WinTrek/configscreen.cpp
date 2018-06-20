#include "pch.h"
#include "valuetransform.h"

#include "ui.h"

class ConfigScreen :
    public Screen
{
private:
    TRef<UiEngine>           m_pUiEngine;
    TRef<EngineConfigurationWrapper> m_pConfiguration;
    TRef<Image>         m_pimage;

    TRef<IEventSink> m_pEventSinkClose;

    TRef<SimpleModifiableValue<bool>> m_pHasChanges;

public:
    Image* GetImage()
    {
        return m_pimage;
    }

    ConfigScreen(UiEngine* pUiEngine, UpdatingConfiguration* pconfiguration, IEventSink* pEventSinkClose) :
        m_pUiEngine(pUiEngine),
        m_pEventSinkClose(pEventSinkClose)
    {
        m_pConfiguration = new EngineConfigurationWrapper(new UpdatingConfiguration(std::make_shared<UpdatingConfigurationStoreTransformer>(pconfiguration)));
        m_pHasChanges = new SimpleModifiableValue<bool>(false);

        std::map<std::string, std::shared_ptr<Exposer>> map;
        map["Has changes"] = BooleanExposer::Create(m_pHasChanges);
        map["Commit changes"] = TypeExposer<TRef<IEventSink>>::Create(new CallbackSink([this]() {
            m_pConfiguration->Update();
            return true;
        }));
        map["Close sink"] = TypeExposer<TRef<IEventSink>>::Create(m_pEventSinkClose);
        map["time"] = NumberExposer::Create(GetEngineWindow()->GetTime());

        map["Configuration.Graphics.Fullscreen"] = TypeExposer<TRef<SimpleModifiableValue<bool>>>::Create(m_pConfiguration->GetGraphicsFullscreen());
        map["Configuration.Graphics.ResolutionX"] = TypeExposer<TRef<SimpleModifiableValue<float>>>::Create(m_pConfiguration->GetGraphicsResolutionX());
        map["Configuration.Graphics.ResolutionY"] = TypeExposer<TRef<SimpleModifiableValue<float>>>::Create(m_pConfiguration->GetGraphicsResolutionY());
        map["Configuration.Graphics.UseVSync"] = TypeExposer<TRef<SimpleModifiableValue<bool>>>::Create(m_pConfiguration->GetGraphicsUseVSync());
        map["Configuration.Graphics.UseAntiAliasing"] = TypeExposer<TRef<SimpleModifiableValue<bool>>>::Create(m_pConfiguration->GetGraphicsUseAntiAliasing());

        map["Configuration.Graphics.MaxTextureSizeLevel"] = TypeExposer<TRef<SimpleModifiableValue<float>>>::Create(m_pConfiguration->GetGraphicsMaxTextureSizeLevel());

        m_pimage = pUiEngine->LoadImageFromLua(UiScreenConfiguration::Create("menuconfig/configscreen.lua", map));
    }

    ~ConfigScreen() {
        m_pConfiguration->Update();
    }

    void OnFrame() override {
        if (m_pHasChanges->GetValue() != m_pConfiguration->HasChanged()) {
            m_pHasChanges->SetValue(m_pConfiguration->HasChanged());
        }
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


