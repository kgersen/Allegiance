#pragma once

#include <boost/any.hpp>

class UiObjectContainer : public IObject {

private:
    std::map<std::string, boost::any> m_map;

public:
    UiObjectContainer(std::map<std::string, boost::any> map) :
        m_map(map)
    {
    }

    template <typename Type>
    Type Get(std::string key) {
        auto found = m_map.find(key);
        if (found == m_map.end()) {
            throw std::runtime_error("Key not found: " + key);
        }

        try
        {
            return boost::any_cast<Type>(found->second);
        }
        catch (const boost::bad_any_cast &)
        {
            throw std::runtime_error("Key found but not of valid type: " + key);
        }
    }

};

class UiScreenConfiguration : public UiObjectContainer {
public:
    UiScreenConfiguration(std::map<std::string, boost::any> map) :
        UiObjectContainer(map)
    {}

    virtual std::string GetPath() = 0;
    virtual IEventSink& GetEventSink(std::string) = 0;

    static std::shared_ptr<UiScreenConfiguration> Create(std::string path, std::map<std::string, std::function<void()>> event_listeners, std::map<std::string, boost::any> map);
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

    virtual std::function<TRef<Image>()> WrapImageCallback(sol::function callback) = 0;

    virtual UiObjectContainer& GetScreenGlobals() = 0;
};