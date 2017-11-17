#include "pch.h"
#include "ui.h"
#include "items.hpp"

#include "ns_math.hpp"
#include "ns_color.hpp"
#include "ns_event.hpp"
#include "ns_image.hpp"
#include "ns_file.hpp"
#include "ns_font.hpp"
#include "ns_point.hpp"
#include "ns_screen.hpp"

#include <stdexcept>
#include <fstream>

std::string UiEngine::m_stringLogPath = "";

void WriteLog(const std::string &text)
{
    if (UiEngine::m_stringLogPath == "") {
        return;
    }
    std::ofstream log_file(UiEngine::m_stringLogPath, std::ios_base::out | std::ios_base::app);
    log_file << text << std::endl;
}

std::string UiEngine::m_stringArtPath;
void UiEngine::SetGlobalArtPath(std::string path)
{
    m_stringArtPath = path;
}

class UiScreenConfigurationImpl : public UiScreenConfiguration {

    std::string m_strPath;
    std::map<std::string, TRef<IEventSink>> m_mapSinks;

public:
    UiScreenConfigurationImpl(std::string path, std::map<std::string, TRef<IEventSink>> mapSinks) {
        m_strPath = path;
        m_mapSinks = mapSinks;
    }
    std::string GetPath() {
        return m_strPath;
    }

    IEventSink& GetEventSink(std::string name) {
        auto find = m_mapSinks.find(name);
        if (find == m_mapSinks.end()) {
            throw std::runtime_error("Name of event sink not found: " + name);
        }
        return *(find->second);
    }

};

std::shared_ptr<UiScreenConfiguration> UiScreenConfiguration::Create(std::string path, std::map<std::string, std::function<void()>> event_listeners) {
    std::map<std::string, TRef<IEventSink>> sinks;

    std::for_each(event_listeners.begin(), event_listeners.end(),
        [&sinks](auto& p) {
        sinks[p.first] = new CallbackSink(p.second);
    });

    return std::make_shared<UiScreenConfigurationImpl>(path, sinks);
}

class LoaderImpl : public Loader {

private:
    PathFinder m_pathfinder;
    sol::state* m_pLua;

public:

    LoaderImpl(sol::state& lua, Engine* pEngine, std::vector<std::string> paths)
        : m_pathfinder(paths)
    {
        m_pLua = &lua;

        //m_pLua->new_usertype<Image>("Image");// , sol::base_classes, sol::bases<TRef<Image>>());

        
    }

    ~LoaderImpl() {
    }

    void InitNamespaces(LuaScriptContext& context) {
        auto pLua = &context.GetLua();

        pLua->open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math);
        pLua->set("loadfile", [](std::string path) {
            throw std::runtime_error("The standard function loadfile is disabled, use File.LoadLua(string)");
        });
        pLua->set("load", []() {
            throw std::runtime_error("The standard function load is disabled, what would you use this for?");
        });

        ImageNamespace::AddNamespace(context);
        EventNamespace::AddNamespace(pLua);

        NumberNamespace::AddNamespace(pLua);
        RectNamespace::AddNamespace(pLua);
        PointNamespace::AddNamespace(pLua);
        BooleanNamespace::AddNamespace(pLua);

        ColorNamespace::AddNamespace(pLua);

        FileNamespace::AddNamespace(pLua, this);

        FontNamespace::AddNamespace(pLua);

        ScreenNamespace::AddNamespace(context);

        pLua->new_usertype<MouseEventImage>("MouseEventImage",
            sol::base_classes, sol::bases<Image>()
            );
    }

    sol::function LoadScript(std::string subpath) {
        std::string path = m_pathfinder.FindPath(subpath);
        if (path == "") {
            throw std::runtime_error("File not found: " + subpath);
        }

        sol::load_result script = m_pLua->load_file(path);
        if (script.valid() == false) {
            sol::error error = script;
            throw error;
        }
        sol::function function = script;

        sol::environment envScript(*m_pLua, sol::create, (*m_pLua).globals());
        sol::set_environment(envScript, function);

        return function;
    }
};

class Executor {

public:

    template <class T>
    T Execute(sol::function script) {
        try {
            sol::function_result result = script.call();
            if (result.valid() == false) {
                sol::error err = result;
                throw err;
            }
            sol::object object = result;
            if (object.is<T>() == false) {
                throw std::runtime_error("Expected return value to be of a specific type");
            }
            T image = result;
            return image;
        }
        catch (const sol::error& e) {
            throw e;
        }
        catch (const std::runtime_error& e) {
            throw e;
        }
    }
};

class LuaScriptContextImpl : public LuaScriptContext {
private:
    TRef<Engine> m_pEngine;
    TRef<ISoundEngine> m_pSoundEngine;
    LoaderImpl m_loader;
    PathFinder m_pathFinder;
    std::shared_ptr<UiScreenConfiguration> m_pConfiguration;
    std::function<void(std::string)> m_funcOpenWebsite;

    sol::state m_lua;

public:

    LuaScriptContextImpl(Engine* pEngine, ISoundEngine* pSoundEngine, std::string stringArtPath, const std::shared_ptr<UiScreenConfiguration>& pConfiguration, std::function<void(std::string)> funcOpenWebsite) :
        m_pEngine(pEngine),
        m_pSoundEngine(pSoundEngine),
        m_pConfiguration(pConfiguration),
        m_loader(LoaderImpl(m_lua, pEngine, {
            stringArtPath + "/PBUI",
            stringArtPath
        })),
        m_pathFinder(PathFinder({
            stringArtPath + "/PBUI",
            stringArtPath
        })),
        m_funcOpenWebsite(funcOpenWebsite)
    {
        m_loader.InitNamespaces(*this);
    }

    sol::state& GetLua() {
        return m_lua;
    }

    sol::function LoadScript(std::string path) {
        return m_loader.LoadScript(path);
    }

    std::string FindPath(std::string path) {
        std::string full_path = m_pathFinder.FindPath(path);
        if (full_path == "") {
            throw std::runtime_error("File path not found: " + path);
        }
        return full_path;
    }

    Engine* GetEngine() {
        return m_pEngine;
    }

    ISoundEngine* GetSoundEngine() {
        return m_pSoundEngine;
    }

    std::function<void(std::string)> GetOpenWebsiteFunction() {
        return m_funcOpenWebsite;
    }

    std::function<TRef<Image>()> WrapImageCallback(sol::function callback) override {
        return [callback]() {
            Executor executor = Executor();
            try {
                //WriteLog("(Callback function): Started");

                auto start = std::chrono::high_resolution_clock::now();

                TRef<Image> result = executor.Execute<Image*>(callback);

                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

                WriteLog("(Callback function): Finished " + std::to_string(microseconds) + " ms");
                return result;
            }
            catch (const std::runtime_error& e) {
                WriteLog(std::string("(Callback function): ERROR ") + e.what());
                throw e;
            }
        };
    }

    IEventSink& GetExternalEventSink(std::string name) {
        return m_pConfiguration->GetEventSink(name);
    }

};

class UiEngineImpl : public UiEngine {

private:
    TRef<Engine> m_pEngine;
    TRef<ISoundEngine> m_pSoundEngine;
    std::function<void(std::string)> m_funcOpenWebsite;

    TRef<EventSourceImpl> m_pReloadEventSource;


public:
    UiEngineImpl(Engine* pEngine, ISoundEngine* pSoundEngine, std::function<void(std::string)> funcOpenWebsite) :
        m_pEngine(pEngine),
        m_pSoundEngine(pSoundEngine),
        m_pReloadEventSource(new EventSourceImpl()),
        m_funcOpenWebsite(funcOpenWebsite)
    {
    }

    void TriggerReload() {
        m_pReloadEventSource->Trigger();
    }

    ~UiEngineImpl() {}

    //Image* LoadImage(std::string path) {
    //
    //}

    class ContextImage : public WrapImage {
    private:
        std::unique_ptr<LuaScriptContextImpl> m_pContext;

    public:
        ContextImage(std::unique_ptr<LuaScriptContextImpl> pContext, Image* pImage) :
            WrapImage(pImage),
            m_pContext(std::move(pContext))
        {
        }

        ~ContextImage() {
            //control order of deallocations, first remove all references to sol before clearing the context
            SetImage(Image::GetEmpty());
            m_pContext = nullptr;
        }
    };

    TRef<Image> InnerLoadImageFromLua(const std::shared_ptr<UiScreenConfiguration>& screenConfiguration) {
        std::unique_ptr<LuaScriptContextImpl> pContext = std::make_unique<LuaScriptContextImpl>(m_pEngine, m_pSoundEngine, m_stringArtPath, screenConfiguration, m_funcOpenWebsite);

        Executor executor = Executor();

        auto path = screenConfiguration->GetPath();

        WriteLog(path + ": " + "Loading");
        try {
            sol::function script = pContext->LoadScript(path);

            WriteLog(path + ": " + "Parsed");

            TRef<Image> image = executor.Execute<Image*>(script);

            WriteLog(path + ": " + "Executed");
            return new ContextImage(std::move(pContext), image);
        }
        catch (const std::runtime_error& e) {
            WriteLog(path + ": ERROR " + e.what());
            return Image::GetEmpty();
        }
    }

    class ImageReloadSink : public IEventSink, public WrapImage {

        UiEngineImpl* m_pUiEngine;
        std::shared_ptr<UiScreenConfiguration> m_pConfiguration;

        TRef<IEventSink> m_pSinkDelegate;

    public:
        ImageReloadSink(UiEngineImpl* pUiEngine, const std::shared_ptr<UiScreenConfiguration>& screenConfiguration) :
            m_pUiEngine(pUiEngine),
            m_pConfiguration(screenConfiguration),
            WrapImage(pUiEngine->InnerLoadImageFromLua(screenConfiguration))
        {
            m_pSinkDelegate = IEventSink::CreateDelegate(this);
        }

        ~ImageReloadSink() {
            m_pUiEngine->m_pReloadEventSource->RemoveSink(m_pSinkDelegate);
        }

        IEventSink* GetSinkDelegate() {
            return m_pSinkDelegate;
        }

        bool OnEvent(IEventSource* pevent) {
            SetImage(m_pUiEngine->InnerLoadImageFromLua(m_pConfiguration));
            return true;
        }

        void Render(Context* pContext) override {
            WrapImage::Render(pContext);
        }

    };

    TRef<Image> LoadImageFromLua(const std::shared_ptr<UiScreenConfiguration>& screenConfiguration) {
        auto path = screenConfiguration->GetPath();

        ImageReloadSink* result = new ImageReloadSink(this, screenConfiguration);
        m_pReloadEventSource->AddSink(result->GetSinkDelegate());
        return result;
    }
};

UiEngine* UiEngine::Create(Engine* pEngine, ISoundEngine* pSoundEngine, std::function<void(std::string)> funcOpenWebsite)
{
    return new UiEngineImpl(pEngine, pSoundEngine, funcOpenWebsite);
}