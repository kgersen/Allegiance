/////////////////////////////////////////////////////////////////////////////

// Create the Deployment object
var objDeploy = new ActiveXObject("TCDeployment");

// Get the version information of the deployment object
var objVerInfo = objDeploy.Version;

// Format the text of the message
var str = "Fixed Information:\n";

// Initialization
str += "\n";
str += "FileName               \t" + objVerInfo.FileName                + "\n";

// File Version
str += "\n";
str += "FileVersionString      \t" + objVerInfo.FileVersionString       + "\n";
str += "FileVersionMSHigh      \t" + objVerInfo.FileVersionMSHigh       + "\n";
str += "FileVersionMSLow       \t" + objVerInfo.FileVersionMSLow        + "\n";
str += "FileVersionLSHigh      \t" + objVerInfo.FileVersionLSHigh       + "\n";
str += "FileVersionLSLow       \t" + objVerInfo.FileVersionLSLow        + "\n";
str += "FileBuildNumber        \t" + objVerInfo.FileBuildNumber         + "\n";

// Product Version
str += "\n";
str += "ProductVersionString   \t" + objVerInfo.ProductVersionString    + "\n";
str += "ProductVersionMSHigh   \t" + objVerInfo.ProductVersionMSHigh    + "\n";
str += "ProductVersionMSLow    \t" + objVerInfo.ProductVersionMSLow     + "\n";
str += "ProductVersionLSHigh   \t" + objVerInfo.ProductVersionLSHigh    + "\n";
str += "ProductVersionLSLow    \t" + objVerInfo.ProductVersionLSLow     + "\n";
str += "ProductBuildNumber     \t" + objVerInfo.ProductBuildNumber      + "\n";

// File Flags
str += "\n";
str += "IsDebug                \t" + objVerInfo.IsDebug                 + "\n";
str += "IsInfoInferred         \t" + objVerInfo.IsInfoInferred          + "\n";
str += "IsPatched              \t" + objVerInfo.IsPatched               + "\n";
str += "IsPreRelease           \t" + objVerInfo.IsPreRelease            + "\n";
str += "IsPrivateBuild         \t" + objVerInfo.IsPrivateBuild          + "\n";
str += "IsSpecialBuild         \t" + objVerInfo.IsSpecialBuild          + "\n";

// Target Operating Systems
str += "\n";
str += "IsTargetUnknown        \t" + objVerInfo.IsTargetUnknown         + "\n";
str += "IsTargetDOS            \t" + objVerInfo.IsTargetDOS             + "\n";
str += "IsTargetOS216          \t" + objVerInfo.IsTargetOS216           + "\n";
str += "IsTargetOS232          \t" + objVerInfo.IsTargetOS232           + "\n";
str += "IsTargetNT             \t" + objVerInfo.IsTargetNT              + "\n";
str += "IsTargetBase           \t" + objVerInfo.IsTargetBase            + "\n";
str += "IsTargetWin16          \t" + objVerInfo.IsTargetWin16           + "\n";
str += "IsTargetPM16           \t" + objVerInfo.IsTargetPM16            + "\n";
str += "IsTargetPM32           \t" + objVerInfo.IsTargetPM32            + "\n";
str += "IsTargetWin32          \t" + objVerInfo.IsTargetWin32           + "\n";
str += "IsTargetDOSWin16       \t" + objVerInfo.IsTargetDOSWin16        + "\n";
str += "IsTargetDOSWin32       \t" + objVerInfo.IsTargetDOSWin32        + "\n";
str += "IsTargetOS216PM16      \t" + objVerInfo.IsTargetOS216PM16       + "\n";
str += "IsTargetOS232PM32      \t" + objVerInfo.IsTargetOS232PM32       + "\n";
str += "IsTargetNTWin32        \t" + objVerInfo.IsTargetNTWin32         + "\n";

// File Types
str += "\n";
str += "IsApp                  \t" + objVerInfo.IsApp                   + "\n";
str += "IsDll                  \t" + objVerInfo.IsDll                   + "\n";
str += "IsDriver               \t" + objVerInfo.IsDriver                + "\n";
str += "IsFont                 \t" + objVerInfo.IsFont                  + "\n";
str += "IsVxd                  \t" + objVerInfo.IsVxd                   + "\n";
str += "IsStaticLib            \t" + objVerInfo.IsStaticLib             + "\n";

// Driver Types
str += "\n";
str += "IsDriverUnknown        \t" + objVerInfo.IsDriverUnknown         + "\n";
str += "IsDriverPrinter        \t" + objVerInfo.IsDriverPrinter         + "\n";
str += "IsDriverKeyboard       \t" + objVerInfo.IsDriverKeyboard        + "\n";
str += "IsDriverLanguage       \t" + objVerInfo.IsDriverLanguage        + "\n";
str += "IsDriverDisplay        \t" + objVerInfo.IsDriverDisplay         + "\n";
str += "IsDriverMouse          \t" + objVerInfo.IsDriverMouse           + "\n";
str += "IsDriverNetwork        \t" + objVerInfo.IsDriverNetwork         + "\n";
str += "IsDriverSystem         \t" + objVerInfo.IsDriverSystem          + "\n";
str += "IsDriverInstallable    \t" + objVerInfo.IsDriverInstallable     + "\n";
str += "IsDriverSound          \t" + objVerInfo.IsDriverSound           + "\n";
str += "IsDriverComm           \t" + objVerInfo.IsDriverComm            + "\n";
str += "IsDriverInputMethod    \t" + objVerInfo.IsDriverInputMethod     + "\n";

// Font Types
str += "\n";
str += "IsFontRaster           \t" + objVerInfo.IsFontRaster            + "\n";
str += "IsFontVector           \t" + objVerInfo.IsFontVector            + "\n";
str += "IsFontTrueType         \t" + objVerInfo.IsFontTrueType          + "\n";

// Echo the first page of text
WScript.Echo(str);

// Prepare the second page
var str = "Variable Information:\n";

// Common String Values
str += "\n";
str += "CompanyName     \t" + (objVerInfo.Exists("CompanyName"     ) ? "\"" + objVerInfo("CompanyName"     ) + "\"" : "(does not exist)") + "\n";
str += "FileDescription \t" + (objVerInfo.Exists("FileDescription" ) ? "\"" + objVerInfo("FileDescription" ) + "\"" : "(does not exist)") + "\n";
str += "FileVersion     \t" + (objVerInfo.Exists("FileVersion"     ) ? "\"" + objVerInfo("FileVersion"     ) + "\"" : "(does not exist)") + "\n";
str += "InternalName    \t" + (objVerInfo.Exists("InternalName"    ) ? "\"" + objVerInfo("InternalName"    ) + "\"" : "(does not exist)") + "\n";
str += "LegalCopyright  \t" + (objVerInfo.Exists("LegalCopyright"  ) ? "\"" + objVerInfo("LegalCopyright"  ) + "\"" : "(does not exist)") + "\n";
str += "OriginalFilename\t" + (objVerInfo.Exists("OriginalFilename") ? "\"" + objVerInfo("OriginalFilename") + "\"" : "(does not exist)") + "\n";
str += "ProductName     \t" + (objVerInfo.Exists("ProductName"     ) ? "\"" + objVerInfo("ProductName"     ) + "\"" : "(does not exist)") + "\n";
str += "ProductVersion  \t" + (objVerInfo.Exists("ProductVersion"  ) ? "\"" + objVerInfo("ProductVersion"  ) + "\"" : "(does not exist)") + "\n";
str += "SpecialBuild    \t" + (objVerInfo.Exists("SpecialBuild"    ) ? "\"" + objVerInfo("SpecialBuild"    ) + "\"" : "(does not exist)") + "\n";
str += "OLESelfRegister \t" + (objVerInfo.Exists("OLESelfRegister" ) ? "\"" + objVerInfo("OLESelfRegister" ) + "\"" : "(does not exist)") + "\n";

// Common String Values (direct property calls)
str += "CompanyName     \t" + ("\"" + objVerInfo.CompanyName      + "\"\n");
str += "FileDescription \t" + ("\"" + objVerInfo.FileDescription  + "\"\n");
str += "FileVersion     \t" + ("\"" + objVerInfo.FileVersion      + "\"\n");
str += "InternalName    \t" + ("\"" + objVerInfo.InternalName     + "\"\n");
str += "LegalCopyright  \t" + ("\"" + objVerInfo.LegalCopyright   + "\"\n");
str += "OriginalFilename\t" + ("\"" + objVerInfo.OriginalFilename + "\"\n");
str += "ProductName     \t" + ("\"" + objVerInfo.ProductName      + "\"\n");
str += "ProductVersion  \t" + ("\"" + objVerInfo.ProductVersion   + "\"\n");
str += "SpecialBuild    \t" + ("\"" + objVerInfo.SpecialBuild     + "\"\n");
str += "OLESelfRegister \t" + ("\"" + objVerInfo.OLESelfRegister  + "\"\n");

// Echo the second page of text
WScript.Echo(str);

