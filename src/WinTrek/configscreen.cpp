#include "pch.h"
#include "valuetransform.h"

#include "ui.h"

class ConfigScreen :
    public Screen
{
private:
    TRef<UiEngine>           m_pUiEngine;
    TRef<GameConfigurationWrapper> m_pConfiguration;
    TRef<Image>         m_pimage;
    TRef<TrekApp> m_pTrekApp;

    TRef<IEventSink> m_pEventSinkClose;

    TRef<SimpleModifiableValue<bool>> m_pHasChanges;

public:
    Image* GetImage()
    {
        return m_pimage;
    }

    ConfigScreen(TrekApp* pTrekApp, UiEngine* pUiEngine, UpdatingConfiguration* pconfiguration, IEventSink* pEventSinkClose) :
        m_pTrekApp(pTrekApp),
        m_pUiEngine(pUiEngine),
        m_pEventSinkClose(pEventSinkClose)
    {
        m_pConfiguration = new GameConfigurationWrapper(new UpdatingConfiguration(std::make_shared<UpdatingConfigurationStoreTransformer>(pconfiguration)));
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

        map["Configuration.Online.CharacterName"] = TypeExposer<TRef<SimpleModifiableValue<ZString>>>::Create(m_pConfiguration->GetOnlineCharacterName());
        map["Configuration.Online.SquadTag"] = TypeExposer<TRef<SimpleModifiableValue<ZString>>>::Create(m_pConfiguration->GetOnlineSquadTag());
        map["Configuration.Online.OfficerToken"] = TypeExposer<TRef<SimpleModifiableValue<ZString>>>::Create(m_pConfiguration->GetOnlineOfficerToken());

        map["AvailableSquadTags"] = TypeExposer<sol::as_table_t<std::vector<std::string>>>::Create(GetAvailableSquadTags());
        map["AvailableTokens"] = TypeExposer<TRef<UiList<TRef<StringValue>>>>::Create(GetAvailableTokens());

        m_pimage = pUiEngine->LoadImageFromLua(UiScreenConfiguration::Create("menuconfig/configscreen.lua", map));
    }

    std::vector<std::string> GetAvailableSquadTags() {
        std::vector<std::string> result = std::vector<std::string>({
            ""
        });

        for (std::shared_ptr<CallsignSquad> squad : m_pTrekApp->GetCallsignHandler()->GetAvailableSquads()) {
            result.push_back(std::string(squad->GetCleanedTag()));
        }

        return result;
    }

    TRef<UiList<TRef<StringValue>>> GetAvailableTokens() {
        return new TransformedList<TRef<StringValue>, ZString>([this](ZString tag) {
            std::shared_ptr<CallsignSquad> squad = m_pTrekApp->GetCallsignHandler()->GetSquadForTag(tag);

            std::vector<TRef<StringValue>> result = std::vector<TRef<StringValue>>({
                new StringValue("")
            });
            if (squad != nullptr) {
                for (ZString str : squad->GetAvailableOfficerTokens()) {
                    result.push_back(new StringValue(str));
                }
            }
            return result;
        }, (TRef<StringValue>)m_pConfiguration->GetOnlineSquadTag());
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

TRef<Screen> CreateConfigScreen(TrekApp* pTrekApp, UiEngine* pUiEngine, UpdatingConfiguration* pconfiguration, IEventSink* pEventSinkClose)
{
    return new ConfigScreen(pTrekApp, pUiEngine, pconfiguration, pEventSinkClose);
}


