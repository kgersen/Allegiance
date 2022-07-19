/////////////////////////////////////////////////////////////////////////////
// VersionInfo.cpp : Implementation of the ZVersionInfo class.
//

#include "VersionInfo.h"

/////////////////////////////////////////////////////////////////////////////
// ZVersionInfo


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction
class ZVersionInfoPrivate {
public:
    int dummy;
};

ZVersionInfo::ZVersionInfo(uintptr_t /*hinstance*/)
{
}

ZVersionInfo::ZVersionInfo(bool)
{
}

ZVersionInfo::ZVersionInfo(LPCTSTR /*szModule*/)
{
}

ZVersionInfo::~ZVersionInfo()
{
}

bool ZVersionInfo::Load(LPCTSTR /*szModule*/)
{
    return true;
}

bool ZVersionInfo::Load(uintptr_t /*hinstance*/)
{
    return true;
}

bool ZVersionInfo::Load(const void* pvVerInfo, UINT cbVerInfo)
{
  return true;
}

void ZVersionInfo::Unload()
{
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// String Values

ZString ZVersionInfo::GetStringValue(LPCTSTR /*pszKey*/, bool* pbExists) const
{
  if (pbExists)
      *pbExists = true;
  // Indicate success
  SetLastError(0);
  return ZString();  
}

ZString ZVersionInfo::GetFileVersionString() const
{
  return ZString("1.1.1.1");
}

uint16_t ZVersionInfo::GetFileVersionMSHigh() const
{
    return 0;
}

uint16_t ZVersionInfo::GetFileVersionMSLow() const
{
    return 0;
}

uint16_t ZVersionInfo::GetFileVersionLSHigh() const
{
    return 0;
}

uint16_t ZVersionInfo::GetFileVersionLSLow() const
{
    return 0;
}

uint16_t ZVersionInfo::GetFileBuildNumber() const
{
    return 0;
}

ZString ZVersionInfo::GetProductVersionString() const
{
    return ZString("1.1.1.1");
}

uint16_t ZVersionInfo::GetProductVersionMSHigh() const
{
    return 0;
}

uint16_t ZVersionInfo::GetProductVersionMSLow() const
{
    return 0;
}

uint16_t ZVersionInfo::GetProductVersionLSHigh() const
{
    return 0;
}

uint16_t ZVersionInfo::GetProductVersionLSLow() const
{
    return 0;
}

uint16_t ZVersionInfo::GetProductBuildNumber() const
{
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// File Flags

bool ZVersionInfo::IsDebug() const
{
    return false;
}

bool ZVersionInfo::IsInfoInferred() const
{
    return false;
}

bool ZVersionInfo::IsPatched() const
{
    return false;
}

bool ZVersionInfo::IsPreRelease() const
{
    return false;
}

bool ZVersionInfo::IsPrivateBuild() const
{
    return false;
}

bool ZVersionInfo::IsSpecialBuild() const
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////
// Target Operating Systems

bool ZVersionInfo::IsTargetUnknown() const
{
    return false;
}

bool ZVersionInfo::IsTargetDOS() const
{
    return false;
}

bool ZVersionInfo::IsTargetOS216() const
{
    return false;
}

bool ZVersionInfo::IsTargetOS232() const
{
    return false;
}

bool ZVersionInfo::IsTargetNT() const
{
    return false;
}

bool ZVersionInfo::IsTargetBase() const
{
    return false;
}

bool ZVersionInfo::IsTargetWin16() const
{
    return false;
}

bool ZVersionInfo::IsTargetPM16() const
{
    return false;
}

bool ZVersionInfo::IsTargetPM32() const
{
    return false;
}

bool ZVersionInfo::IsTargetWin32() const
{
    return false;
}

bool ZVersionInfo::IsTargetDOSWin16() const
{
    return false;
}

bool ZVersionInfo::IsTargetDOSWin32() const
{
    return false;
}

bool ZVersionInfo::IsTargetOS216PM16() const
{
    return false;
}

bool ZVersionInfo::IsTargetOS232PM32() const
{
    return false;
}

bool ZVersionInfo::IsTargetNTWin32() const
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////
// File Types

bool ZVersionInfo::IsApp() const
{
    return false;
}

bool ZVersionInfo::IsDll() const
{
    return false;
}

bool ZVersionInfo::IsDriver() const
{
    return false;
}

bool ZVersionInfo::IsFont() const
{
    return false;
}

bool ZVersionInfo::IsVxd() const
{
    return false;
}

bool ZVersionInfo::IsStaticLib() const
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////
// Driver Types

bool ZVersionInfo::IsDriverUnknown() const
{
    return false;
}

bool ZVersionInfo::IsDriverPrinter() const
{
    return false;
}

bool ZVersionInfo::IsDriverKeyboard() const
{
    return false;
}

bool ZVersionInfo::IsDriverLanguage() const
{
    return false;
}

bool ZVersionInfo::IsDriverDisplay() const
{
    return false;
}

bool ZVersionInfo::IsDriverMouse() const
{
    return false;
}

bool ZVersionInfo::IsDriverNetwork() const
{
    return false;
}

bool ZVersionInfo::IsDriverSystem() const
{
    return false;
}

bool ZVersionInfo::IsDriverInstallable() const
{
    return false;
}

bool ZVersionInfo::IsDriverSound() const
{
    return false;
}

bool ZVersionInfo::IsDriverComm() const
{
    return false;
}

bool ZVersionInfo::IsDriverInputMethod() const
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////
// Font Types

bool ZVersionInfo::IsFontRaster() const
{
    return false;
}

bool ZVersionInfo::IsFontVector() const
{
    return false;
}

bool ZVersionInfo::IsFontTrueType() const
{
    return false;
}


/////////////////////////////////////////////////////////////////////////////
// String Values

void ZVersionInfo::SetLanguageID(LANGID /*wLangID*/)
{
}

LANGID ZVersionInfo::GetLanguageID() const
{
  return -1;
}

void ZVersionInfo::SetCodePage(uint16_t /*wCodePage*/)
{
}

uint16_t ZVersionInfo::GetCodePage() const
{
  return -1;
}


/////////////////////////////////////////////////////////////////////////////
// Common String Values

ZString ZVersionInfo::GetCompanyName(bool* pbExists) const
{
  return GetStringValue(("CompanyName"), pbExists);
}

ZString ZVersionInfo::GetFileDescription(bool* pbExists) const
{
  return GetStringValue(("FileDescription"), pbExists);
}

ZString ZVersionInfo::GetFileVersion(bool* pbExists) const
{
  return GetStringValue(("FileVersion"), pbExists);
}

ZString ZVersionInfo::GetInternalName(bool* pbExists) const
{
  return GetStringValue(("InternalName"), pbExists);
}

ZString ZVersionInfo::GetLegalCopyright(bool* pbExists) const
{
  return GetStringValue(("LegalCopyright"), pbExists);
}

ZString ZVersionInfo::GetOriginalFilename(bool* pbExists) const
{
  return GetStringValue(("OriginalFilename"), pbExists);
}

ZString ZVersionInfo::GetProductName(bool* pbExists) const
{
  return GetStringValue(("ProductName"), pbExists);
}

ZString ZVersionInfo::GetProductVersion(bool* pbExists) const
{
  return GetStringValue(("ProductVersion"), pbExists);
}

ZString ZVersionInfo::GetSpecialBuild(bool* pbExists) const
{
  return GetStringValue(("SpecialBuild"), pbExists);
}

ZString ZVersionInfo::GetOLESelfRegister(bool* pbExists) const
{
  return GetStringValue(("OLESelfRegister"), pbExists);
}


/////////////////////////////////////////////////////////////////////////////
// Raw Data Access

const uint8_t* ZVersionInfo::GetVerInfo() const
{
  return nullptr;
}

UINT ZVersionInfo::GetVerInfoSize() const
{
  return 0;
}

const uintptr_t ZVersionInfo::GetFixed() const
{
  return 0;
}
