#pragma once

#include <string>
#include <vector>


class Mod {
private:
    std::string m_strName;

    std::string m_strArtPath;

public:
    Mod(std::string strName, std::string strArtPath) :
        m_strName(strName),
        m_strArtPath(strArtPath)
    {

    }

    std::string GetName() {
        return m_strName;
    }

    std::string GetArtPath() {
        return m_strArtPath;
    }
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
};