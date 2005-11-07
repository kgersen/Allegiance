#ifdef _VERINFO_H
#error verinfo.h included twice
#endif
#define _VERINFO_H

//-----------------------------------------------------------------------------
// @doc
// @module VERINFO.H - Declaration of VersionInfo class | 
//
// Copyright <cp> 1995 Microsoft Corporation, All Rights Reserved
//
// Mark Snyder, Software Design Engineer
// Microsoft Travel Products
//

#if defined(DEBUG) && defined(INLINE)
#undef THIS_FILE
static char BASED_CODE VERINFO_H[] = "verinfo.h";
#define THIS_FILE VERINFO_H
#endif

//-----------------------------------------------------------------------------
// @class The VersionInfo class contains the version info for all of the 
//        modules needed under the current configuration.
//
class VersionInfo
{
protected:  // @access Protected Members
    // @cmember Initializes all the values in the object.  Called from both
    //          constructors.  
    void Initialize();

public:  // @access Public Members
    // @cmember Constructor.  Creates an empty VersionInfo object.
    VersionInfo();

    // @cmember Constructor.  Creates a VersionInfo object containing the version
    //          information for the specified filename.
    VersionInfo(char* pszFileName);

    // @cmember The name of the product.
    char  m_szProductName[48];    
    
    // @cmember The text representation of the version information.
    char  m_szProductVersion[64];

    // @cmember The text representation of the build flavor, i.e., "Debug", "Test", or "Retail"
    char  m_szBuildFlavor[16];

    // @cmember The high-order 32 bits of the version.
    DWORD m_dwVersionMS;

    // @cmember The low-order 32 bits of the version.
    DWORD m_dwVersionLS;

    // @cmember The bits in this field indicate the type of build.  See VS_FF_ definitions.
    //          Note that an off bit value can mean either FALSE or indeterminate from the 
    //          version resource, but an on bit means TRUE.
    DWORD m_dwFileFlags;
};

typedef VersionInfo* PVI;

