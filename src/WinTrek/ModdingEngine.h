#pragma once

#include <string>
#include <vector>

class ModUploadProgress {
public:
    virtual TRef<StringValue> GetStageString() = 0;

    virtual TRef<Number> GetBytesProcessed() = 0;
    virtual TRef<Number> GetBytesTotal() = 0;
};

class Mod {
private:
    std::string m_strName;

    std::string m_strArtPath;

    TRef<UpdatingConfiguration> m_pModConfiguration;

public:
    Mod(std::string strName, std::string strArtPath) :
        m_strName(strName),
        m_strArtPath(strArtPath)
    {
        m_pModConfiguration = new UpdatingConfiguration(CreateJsonConfigurationStore(GetArtPath() + "\\mod.json"));
    }

    std::string GetName() {
        return m_strName;
    }

    std::string GetArtPath() {
        return m_strArtPath;
    }

    TRef<UpdatingConfiguration> GetModConfiguration() {
        return m_pModConfiguration;
    }

    std::shared_ptr<ModUploadProgress> UploadToWorkshop();

};

class ModdingEngine {
private:
    std::vector<std::shared_ptr<Mod>> m_vMods;

public:

    static const std::string GetPath();

    ModdingEngine(const std::vector<std::shared_ptr<Mod>>& vMods) :
        m_vMods(vMods)
    {

    }

    static std::shared_ptr<ModdingEngine> Create();

    const std::vector<std::shared_ptr<Mod>>& GetMods() {
        return m_vMods;
    }

    std::shared_ptr<Mod> CreateMod(std::string name);
};