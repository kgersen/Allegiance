
#include "pch.h"

#include "ConfigurationStore.h"

#include <sstream>

DWORD GetDWordRegValue(HKEY hkey, LPCSTR appKey, std::string key, DWORD valueDefault) {
    HKEY hKey;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(hkey, appKey,
        0, KEY_READ, &hKey))
    {
        DWORD dwResult;
        DWORD dwSize = sizeof(dwResult);
        DWORD dwType = REG_DWORD;

        ::RegQueryValueEx(hKey, key.c_str(), NULL, &dwType, (BYTE*)&dwResult, &dwSize);
        ::RegCloseKey(hKey);

        if (dwType == REG_DWORD) {
            return dwResult;
        }
    }
    return valueDefault;
}

void SetDWordRegValue(HKEY hkey, LPCSTR appKey, std::string key, DWORD value) {
    HKEY hKey;

    if (ERROR_SUCCESS == ::RegCreateKeyEx(hkey, appKey,
        0, "", REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))
    {
        ::RegSetValueEx(hKey, key.c_str(), NULL, REG_DWORD, (const BYTE*)&value, sizeof(value));
        ::RegCloseKey(hKey);
    }
}

std::string GetStringRegValue(HKEY hkey, LPCSTR appKey, std::string key, std::string valueDefault) {
    HKEY hKey;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(hkey, appKey,
        0, KEY_READ, &hKey))
    {
        DWORD cbValue = 512;
        char result[512];
        result[0] = '\0';
        ::RegQueryValueEx(hKey, "CfgFile", NULL, NULL, (LPBYTE)&result, &cbValue);

        return std::string(result);
    }
    return valueDefault;
}

void SetStringRegValue(HKEY hkey, LPCSTR appKey, std::string key, std::string value) {
    HKEY hKey;

    if (ERROR_SUCCESS == ::RegCreateKeyEx(hkey, appKey,
        0, "", REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))
    {
        char target[512];
        lstrcpy(target, value.c_str());

        ::RegSetValueEx(hKey, key.c_str(), NULL, REG_DWORD, (const BYTE*)&target, sizeof(target));
        ::RegCloseKey(hKey);
    }
}

bool RegistryConfigurationStore::ReadBool(std::string key, bool valueDefault) {
    return (0 != GetDWordRegValue(mHKey, mRegistryPath, key, valueDefault ? 1 : 0));
}

void RegistryConfigurationStore::SetBool(std::string key, bool value) {
    SetDWordRegValue(mHKey, mRegistryPath, key, value ? 1 : 0);
}

int RegistryConfigurationStore::ReadInt(std::string key, int valueDefault) {
    return (int)GetDWordRegValue(mHKey, mRegistryPath, key, (DWORD)valueDefault);
}

void RegistryConfigurationStore::SetInt(std::string key, int value) {
    SetDWordRegValue(mHKey, mRegistryPath, key, (DWORD)value);
}

float RegistryConfigurationStore::ReadFloat(std::string key, float valueDefault) {
    auto regvalue = GetStringRegValue(mHKey, mRegistryPath, key, "");

    if (regvalue == "") {
        return valueDefault;
    }

    float fResult = std::stof(regvalue);
    if (std::isnan(fResult) == true) {
        return valueDefault;
    }

    return fResult;
}

void RegistryConfigurationStore::SetFloat(std::string key, float value) {
    std::ostringstream ss;
    ss << value;
    std::string s(ss.str());

    SetStringRegValue(mHKey, mRegistryPath, key, std::to_string(value));
}

std::string RegistryConfigurationStore::ReadString(std::string key, std::string valueDefault) {
    return GetStringRegValue(mHKey, mRegistryPath, key, valueDefault);
}

void RegistryConfigurationStore::SetString(std::string key, std::string value) {
    SetStringRegValue(mHKey, mRegistryPath, key, value);
}