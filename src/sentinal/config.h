/*-------------------------------------------------------------------------
 * Config.h
 * 
 * The definitions, etc. for a set of classes that are helpful in 
 * maintaining a set of settings in an organized fashion.  The class is
 * generic and extendible to support any implementaion of where the
 * settings are stored (e.g., registry, database, etc.).
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifdef _CONFIG_H
#error config.h included twice
#endif
#define _CONFIG_H

#define HKLM_Sentinal		"SYSTEM\\CurrentControlSet\\Services\\Sentinal\\Parameters"

// Settings are grouped by "component".  Here's the list of different components:
typedef enum
{
    kCompId_Sentinal,                 // Settings related to Sentinal

    // !!NOTE!! If you add to this table, update REGCFG.CPP's CompIdToKey()
    // so when we run in registry mode we know where to look...

    // !!NOTE!! Also, since data can be stored in the database, you should coordinate
    // adding values in here with everyone, so we don't get database
    // corruption, where two people use the same component ID in the database to store
    // different component's settings...

    kCompId_Last    // Must be last...
} ComponentId;

typedef enum
{
    kConfigType_SZ,
    kConfigType_DWORD,
} ConfigItemType;


class ConfigItem
{
public:
    ConfigItemType m_type;       // Item is an int or string?
    ZString              m_zsName;     // Item's name.
    ZString              m_zsValue;    // Item's value.
};

typedef struct
{
	char *               pszKeyName;
	ConfigItemType       type;
	int	                 cb;
	int                  nRequired; // for ReadMany:   True/False on if required.
	union                           // for ReadRgMany: Required up and including to this index...
	{
		DWORD            dwDefault;
		char *           pszDefault;
	};
} ConfigInfo;

typedef struct
{
    ConfigInfo info;
    BYTE *           pBuf;
} ConfigInfoBuffer;      

// pure virtual base class for configuration reader.
class ConfigReader
{
public:
    ConfigReader(ComponentId compId) :
        m_compId(compId)
    {
    }
    virtual ~ConfigReader() {}

    virtual BOOL ReadRg    (const ConfigInfoBuffer *prgItems, int cItems);
    virtual BOOL ReadRgRg  (int cRepeat, const ConfigInfoBuffer *prgItems, int cItems);    // ReadMany() but, the szKeyName has "00x" appended to it (where x is 0 thru cRepeat).  pb is assumed to be an array...
    virtual BOOL ReadInt   (const char *pszItem, int *pnVal, BOOL fRequired, int nDefault);
    virtual BOOL ReadSz    (const char *pszItem, char *psz, int cchMax, BOOL fRequired, const char *pszDefault);
    virtual BOOL WriteInt  (const char *pszItem, int nVal);
    virtual BOOL WriteSz   (const char *pszItem, const char *psz, int cch);
    virtual BOOL DeleteItem(const char *pszItem) = 0;

protected:
    ComponentId  m_compId;

    virtual BOOL Open() = 0;
    virtual BOOL Close() = 0;
    virtual BOOL FastReadInt (const char *pszItem, int *pnVal, BOOL fRequired, int nDefault) = 0;
    virtual BOOL FastReadSz  (const char *pszItem, char *psz, int cchMax, BOOL fRequired, const char *pszDefault) = 0;
    virtual BOOL FastWriteInt(const char *pszItem, int nVal) = 0;
    virtual BOOL FastWriteSz (const char *pszItem, const char *psz, int cch) = 0;
};
                         
// Registry based configuration reader:
class RegConfigReader : public ConfigReader
{
public:
    RegConfigReader(ComponentId compId) :
        ConfigReader(compId),
        m_hk(NULL) 
    { 
        CompIdToKey(); 
    }

    virtual ~RegConfigReader() 
    {
    }

    virtual BOOL DeleteItem(const char *pszItem);

protected:
    virtual BOOL Open();
    virtual BOOL Close();
    virtual BOOL FastReadInt (const char *pszItem, int *pnVal, BOOL fRequired, int nDefault);
    virtual BOOL FastReadSz  (const char *pszItem, char *psz, int cchMax, BOOL fRequired, const char *pszDefault);
    virtual BOOL FastWriteInt(const char *pszItem, int nVal);
    virtual BOOL FastWriteSz (const char *pszItem, const char *psz, int cch);

private:
    BOOL CompIdToKey();

    char m_szKey[255];
    HKEY m_hk;
};

// This routine can be used to create the "appropriate" configuration reader type (registry 
// or whatever) based on whether a #define is set...
// Currently, though we just have a Registry-based solution, so it doesn't matter much.
ConfigReader *   CreateConfigReader   (ComponentId compId);
RegConfigReader *CreateRegConfigReader(ComponentId compId);

////////////////////////////////////////////////////////////////////////
// This object contains a list of settings stored for a given ComponentId, and list of settings.
class Config
{
public:
    Config(ComponentId compId) : m_compId(compId), m_pArgs(NULL), m_nArgs(0), m_pConfigInfo(NULL) {}
    virtual ~Config() { KillArgs(); }

    BOOL Init(const ConfigInfo *pConfigInfo, int cItems, BOOL fRange, int cRange);
    BOOL GetDWORDArg(DWORD nArg, DWORD *pdw, int nRange);   // only look at nRange, if m_fRange
    BOOL GetStringArg(DWORD nArg, char *pszBuf, int cchBuf, int nRange); // only look at nRange, if m_fRange

    // Caller must delete pointer returned:
    ConfigInfo *GetConfigInfo(int *pcConfig);

    inline ComponentId GetComponentId() const { return m_compId; }

private:
    ComponentId m_compId;
    BOOL              m_fRange;    // TRUE if we're storing a bunch of settings (e.g. a bunch of strings or a bunch of DWORDs).
    BOOL              m_cRange;

    typedef struct
    {
        ConfigItemType type;
        DWORD                cb;
    	union                           // for ReadRgMany: Required up and including to this index...
    	{
    		DWORD dwVal;    // Used if we're not storing a range, and type == kConfigType_DWORD
    		char *pszVal;   // Used to store strings, for both range and non range storage
            DWORD *pdwVal;  // Used to store DWORD's when we're storing a range.
    	};
    } ARGS;

    ARGS *m_pArgs;
    DWORD m_nArgs;
    ConfigInfoBuffer *m_pConfigInfo;

    void KillArgs();
};

class ConfigCache
{
public:
    ConfigCache() 
    { 
        m_pcsEditList = new CriticalSection;
        ZeroMemory(&m_rgpConfigList, sizeof(m_rgpConfigList)); 
    }
    virtual ~ConfigCache() 
    { 
        DeleteConfigList(); 
        delete m_pcsEditList;
    }

    Config * FindConfig(ComponentId compId);
    void     AddConfig(Config *pConfig);
    Config * ReplaceConfig(ComponentId compId, Config *pConfig);

    void     AddConfigToTail(Config *pConfig);
    Config * RemoveHead(ComponentId compId);
    Config * RemoveHeadIfOld(ComponentId compId, DWORD dwDeltaOld);

private:
    typedef struct tag_CONFIG_LIST
    {
        Config * pConfig;
        DWORD    dwTimeAdded;
        struct tag_CONFIG_LIST *pNext;
    } CONFIG_LIST;
    
    CONFIG_LIST *   m_rgpConfigList[kCompId_Last];
    CriticalSection *m_pcsEditList; // this is a pointer, solely cuz we CriticalSection isn't __declspec(dllexport)...

    void DeleteConfigList();
};

// This class maintains a bunch of different configuration information for various components.
// It allows a new configuration to be loaded to replace an existing one in the list in a thread
// safe manner, without needing to use a CriticalSection to guard list read access...  It does
// this by using garbage collection when it deletes a node, making the assumption that some thread
// that accesses the list of configurations will traverse the entire list in a short period of time.
class ConfigManager
{
public:
    ConfigManager() : m_hevtDie(NULL), m_hthrGarbageCollect(NULL), m_pcsAdd(NULL) {}
    virtual ~ConfigManager();

    BOOL Init();

    BOOL LoadConfig             (ComponentId compId, const ConfigInfo *pInfo, int cInfo);
    BOOL ReloadConfig           (ComponentId compId);
    BOOL IsConfigLoaded         (ComponentId compId);

    // Routines for getting at our settings.
    BOOL GetConfigDWORD         (ComponentId compId, DWORD nArg, DWORD *pdw);
    BOOL GetConfigString        (ComponentId compId, DWORD nArg, char *pszBuf, int cchBuf);

    virtual void GarbageCollectThread();

private:
    ConfigCache m_cacheComponent;           // Settings stored by Component
    ConfigCache m_cacheGarbage;             // Garbage collection guys -- we lazy delete these.
    CriticalSection * m_pcsAdd;             // Guards adding to our list/deleting old guy.  This is a ptr, solely cuz CriticalSection isn't __declspec(dllexport).
    HANDLE            m_hevtDie;            // Signalled when the Garbage Collect thread should go away.
    HANDLE            m_hthrGarbageCollect; // Handle to garbage collector thread.

    static int CALLBACK GarbageCollectThreadThunk(ConfigManager *pMgr)
    {
        ZAssert(pMgr);
        pMgr->GarbageCollectThread();
        return 0;
    }
};

