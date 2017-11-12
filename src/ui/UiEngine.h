#pragma once

class UiScreenConfiguration {
public:
    virtual std::string GetPath() = 0;
    virtual IEventSink& GetEventSink(std::string) = 0;

    static std::shared_ptr<UiScreenConfiguration> Create(std::string path, std::map<std::string, std::function<void()>> event_listeners);
}; 

class UiEngine : public IObject
{
protected:

    static std::string m_stringArtPath;

public:

    static std::string m_stringLogPath;

    static void SetGlobalArtPath(std::string path);
    static UiEngine* UiEngine::Create(Engine* pEngine, ISoundEngine* pSoundEngine, std::function<void(std::string)> funcOpenWebsite);

    //virtual Image* LoadImage(std::string path) = 0;
    virtual TRef<Image> LoadImageFromLua(const std::shared_ptr<UiScreenConfiguration>& screenConfiguration) = 0;

    virtual void TriggerReload() = 0;
};

class Loader {
public:
    virtual sol::function LoadScript(std::string subpath) = 0;
};

class LuaScriptContext {
public:
    virtual sol::state& GetLua() = 0;

    virtual sol::function LoadScript(std::string path) = 0;

    virtual std::string FindPath(std::string path) = 0;

    virtual Engine* GetEngine() = 0;

    virtual ISoundEngine* GetSoundEngine() = 0;

    virtual IEventSink& GetExternalEventSink(std::string name) = 0;

    virtual std::function<void(std::string)> GetOpenWebsiteFunction() = 0;
};