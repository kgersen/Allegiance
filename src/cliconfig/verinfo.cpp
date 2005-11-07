//-----------------------------------------------------------------------------
// @doc
//
// @module VERINFO.CPP - VersionInfo object implementation |
//
// This is the implementation of the VersionInfo object, which handles the 
// extraction of version information about a module in a class that can be
// used in a generic array.  This provides one mechanism by which we can collect
// version information from dependent modules to be displayed in the about box,
// for example.
//
// Copyright <cp> 1995 Microsoft Corporation, All Rights Reserved
//
// Mark Snyder, Software Design Engineer
// Microsoft Travel Products
//

#include "stdafx.h"
#include "verinfo.h"

VersionInfo::VersionInfo()
{
    Initialize();
}

VersionInfo::VersionInfo(char* pszFileName)
{
    Initialize();
    if (pszFileName==NULL)
        return;

    DWORD dwBogus;
	DWORD cbVer = GetFileVersionInfoSize(pszFileName, &dwBogus);
    if (!cbVer)
        return;

    BYTE* pbVer = new BYTE[cbVer];
	if (!pbVer)
        return;

    if (!GetFileVersionInfo(pszFileName, 0, cbVer, pbVer))
    {
        delete [] pbVer;
        return;
    }

    PVOID pvValue;
    UINT  cbValue;
    if (VerQueryValue(pbVer, TEXT("\\"), &pvValue, &cbValue) && pvValue!=NULL)
    {
        m_dwVersionMS = ((VS_FIXEDFILEINFO*)pvValue)->dwProductVersionMS;
        m_dwVersionLS = ((VS_FIXEDFILEINFO*)pvValue)->dwProductVersionLS;
        m_dwFileFlags = ((VS_FIXEDFILEINFO*)pvValue)->dwFileFlagsMask 
                        & ((VS_FIXEDFILEINFO*)pvValue)->dwFileFlags;
    }
    if (VerQueryValue(pbVer, TEXT("\\StringFileInfo\\040904b0\\ProductName"),    &pvValue, &cbValue))
        memcpy(m_szProductName,    pvValue, min(cbValue, sizeof(m_szProductName)-1));
    if (VerQueryValue(pbVer, TEXT("\\StringFileInfo\\040904b0\\ProductVersion"), &pvValue, &cbValue))
        memcpy(m_szProductVersion, pvValue, min(cbValue, sizeof(m_szProductVersion)-1));
    if (VerQueryValue(pbVer, TEXT("\\StringFileInfo\\040904b0\\BuildFlavor"), &pvValue, &cbValue))
        memcpy(m_szBuildFlavor, pvValue, min(cbValue, sizeof(m_szBuildFlavor)-1));
    
    delete [] pbVer;
}

void VersionInfo::Initialize()
{
    memset(m_szProductName,    0x00, sizeof(m_szProductName));
    memset(m_szProductVersion, 0x00, sizeof(m_szProductVersion));
    memset(m_szBuildFlavor,    0x00, sizeof(m_szBuildFlavor));
    m_dwVersionMS     = 0;
    m_dwVersionLS     = 0;
    m_dwFileFlags     = 0;
}
