#include "pch.h"

#include "ModdingEngine.h"

#include "Win32app.h"

const std::string ModdingEngine::GetPath() {
    return GetExecutablePath() + "\\Mods";
}

std::shared_ptr<ModdingEngine> ModdingEngine::Create() {
    std::vector<std::shared_ptr<Mod>> vMods = {};

    //Go through the mod directory and add each directory in there

    std::string search_path = ModdingEngine::GetPath() + "\\*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName[0] != '.') {
                vMods.push_back(std::make_shared<Mod>(fd.cFileName, ModdingEngine::GetPath() + "\\" + fd.cFileName));
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }

    return std::make_shared<ModdingEngine>(vMods);
}