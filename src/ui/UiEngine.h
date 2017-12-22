#pragma once

#include <boost/any.hpp>

class UiScreenConfiguration : public UiObjectContainer {
public:
    UiScreenConfiguration(std::map<std::string, boost::any> map) :
        UiObjectContainer(map)
    {}

    virtual std::string GetPath() = 0;
    virtual IEventSink& GetEventSink(std::string) = 0;

    static std::shared_ptr<UiScreenConfiguration> Create(std::string path, std::map<std::string, std::function<bool()>> event_listeners, std::map<std::string, boost::any> map);
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

class PathFinder {

private:
    // from: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    inline bool file_exists(const std::string& name) {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }
    std::vector<std::string> m_paths;

public:

    PathFinder(std::vector<std::string> paths) :
        m_paths(paths)
    {

    }

    std::string FindPath(std::string& subpath) {
        for (auto& path : m_paths) {
            if (file_exists(path + '/' + subpath) == true) {
                return path + '/' + subpath;
            }
        }
        return "";
    }
};

class LuaScriptContext;
class Loader {

private:
    PathFinder m_pathfinder;
    sol::state* m_pLua;

public:
    Loader(sol::state& lua, Engine* pEngine, std::vector<std::string> paths);
    ~Loader();

    void InitNamespaces(LuaScriptContext& context);

    sol::function LoadScript(std::string subpath);
};

class Executor {
private:
    int m_countInScriptLevel;

public:
    Executor() :
        m_countInScriptLevel(0)
    {}

    bool IsInScript() {
        return m_countInScriptLevel > 0;
    }

    template <class T, typename... TArgs>
    T Execute(sol::function script, TArgs ... args);
};

class LuaScriptContext {
private:
    TRef<Engine> m_pEngine;
    TRef<ISoundEngine> m_pSoundEngine;
    Loader m_loader;
    PathFinder m_pathFinder;
    std::shared_ptr<UiScreenConfiguration> m_pConfiguration;
    std::function<void(std::string)> m_funcOpenWebsite;

    Executor m_executor;

    sol::state m_lua;

public:
    LuaScriptContext(Engine* pEngine, ISoundEngine* pSoundEngine, std::string stringArtPath, const std::shared_ptr<UiScreenConfiguration>& pConfiguration, std::function<void(std::string)> funcOpenWebsite);

    sol::state& GetLua();

    sol::function LoadScript(std::string path);

    std::string FindPath(std::string path);

    Engine* GetEngine();

    ISoundEngine* GetSoundEngine();

    Executor* GetExecutor() {
        return &m_executor;
    }

    IEventSink& GetExternalEventSink(std::string name);

    std::function<void(std::string)> GetOpenWebsiteFunction();

    template <typename T, typename... TArgs>
    std::function<T(TArgs...)> WrapCallback(sol::function callback, T valueDefault);

    UiObjectContainer& GetScreenGlobals();
};