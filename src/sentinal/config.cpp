/*-------------------------------------------------------------------------
 * Config.CPP
 * 
 * Implementation information for classes that organize settings used by
 * an application, abstracting where those settings actually come from.
 * 
 * Owner: MarkSn
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
 
//#include "pch.h"

#define __MODULE__ "Sentinal"

#include "zlib.h"
#include "srvdbg.h"
#include "config.h"

BOOL RegConfigReader::DeleteItem(const char *pszItem)
{
	HKEY hkey;
    BOOL fRet = FALSE;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_szKey, 0, KEY_READ | KEY_SET_VALUE, &hkey) == ERROR_SUCCESS)
	{
		if (RegDeleteValue (hkey, pszItem) == ERROR_SUCCESS)
		{
            fRet = TRUE;
		}

		RegCloseKey(hkey);
	}

    return fRet;
}


BOOL RegConfigReader::Open()
{
    if (m_szKey[0])
    {
        DWORD dw; // Gets result of whether it opened or created...
        if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, m_szKey, 0, "", REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &m_hk, &dw) == ERROR_SUCCESS)
            return TRUE;

        SRVDBG_Info2("Missing registry key", m_szKey);
    }

    return FALSE;
}

BOOL RegConfigReader::Close()
{
    return (ERROR_SUCCESS == RegCloseKey(m_hk));
}


BOOL RegConfigReader::FastReadInt(const char *pszItem, int *pnVal, BOOL fRequired /* = TRUE */, int nDefault /* = 0 */)
{
    ZAssert(pszItem);
    ZAssert(pnVal);

    DWORD dwSize = sizeof(DWORD);

	if (RegQueryValueEx(m_hk, pszItem, NULL, NULL, (BYTE *)pnVal, &dwSize) != ERROR_SUCCESS)
	{
        if (fRequired)
        {
            SRVDBG_Info3("Missing registry value", m_szKey, pszItem);
            return FALSE;
        }

        *pnVal = nDefault;
	}

    return TRUE;
}

BOOL RegConfigReader::FastReadSz(const char *pszItem, char *psz, int cchMax, BOOL fRequired /* = TRUE */, const char *pszDefault /* = NULL */)
{
    ZAssert(pszItem);
    ZAssert(psz);
    ZAssert(fRequired || pszDefault);
    ZAssert((NULL == pszDefault) || (lstrlen(pszDefault) < cchMax-1));

    DWORD dwSize = cchMax;

	if (RegQueryValueEx(m_hk, pszItem, NULL, NULL, (BYTE *)psz, &dwSize) != ERROR_SUCCESS)
	{
        if (fRequired)
        {
            SRVDBG_Info3("Missing registry key", m_szKey, pszItem);
            return FALSE;
        }

        lstrcpy(psz, pszDefault);
	}

    return TRUE;
}

BOOL RegConfigReader::FastWriteInt(const char *pszItem, int nVal)
{
    return (RegSetValueEx(m_hk, pszItem, 0, REG_DWORD, (BYTE *)&nVal, sizeof(nVal)) == ERROR_SUCCESS);
}

BOOL RegConfigReader::FastWriteSz (const char *pszItem, const char *psz, int cch)
{
    return (RegSetValueEx(m_hk, pszItem, 0, REG_SZ, (BYTE *)psz, cch) == ERROR_SUCCESS);
}



BOOL RegConfigReader::CompIdToKey()
{
    switch (m_compId)
    {
        case kCompId_Sentinal:
            lstrcpy(m_szKey, HKLM_Sentinal);
            break;
        // add cases for other components here
		// case kCompId_:
        //      break;
        default:
            ZAssert(FALSE);
            SRVDBG_Info("Bogus component specified to config reader");
            m_szKey[0] = 0;
            return FALSE;
    }

    return TRUE;
}

BOOL Config::Init(const ConfigInfo *pConfigItems, int nItems, BOOL fRange, int cRange)
{
    ZAssert(pConfigItems);

    KillArgs();

    m_fRange = fRange;
    m_cRange = cRange;
    m_nArgs  = nItems;
    m_pArgs  = new ARGS[nItems];

    ZAssert(m_pArgs);    // all new's are assumed to succeed...

    m_pConfigInfo = new ConfigInfoBuffer[nItems];
    ZAssert(m_pConfigInfo); // all new's assumed to succeed...

    for (int i=0;  i < nItems;  i++)
    {
        m_pConfigInfo[i].info.pszKeyName = new char[lstrlen(pConfigItems[i].pszKeyName) + 1];
        lstrcpy(m_pConfigInfo[i].info.pszKeyName, pConfigItems[i].pszKeyName);

        m_pConfigInfo[i].info.type       = pConfigItems[i].type;
        m_pConfigInfo[i].info.cb         = pConfigItems[i].cb;
        m_pConfigInfo[i].info.nRequired  = pConfigItems[i].nRequired;

        m_pArgs[i].type = pConfigItems[i].type;
        m_pArgs[i].cb   = pConfigItems[i].cb;

        if (m_pArgs[i].type == kConfigType_SZ)
        {
            m_pArgs[i].pszVal = new char[pConfigItems[i].cb * (fRange ? cRange : 1)];
            ZAssert(m_pArgs[i].pszVal);  // all new's are assumed to succeed.

            char *pszDef = pConfigItems[i].pszDefault;
            if (NULL == pszDef)
                pszDef = "";

            m_pConfigInfo[i].info.pszDefault = new char[lstrlen(pszDef) + 1];
            ZAssert(m_pConfigInfo[i].info.pszDefault);  // all new's assumed to succeed.

            lstrcpy(m_pConfigInfo[i].info.pszDefault, pszDef);

            m_pConfigInfo[i].pBuf            = (BYTE *)m_pArgs[i].pszVal;
            m_pConfigInfo[i].info.cb         = pConfigItems[i].cb;
        }
        else
        {
            if (fRange)
            {
                m_pArgs[i].pdwVal = new DWORD[cRange];
                ZAssert(m_pArgs[i].pdwVal); // all new's assumed to succeed.
                m_pConfigInfo[i].pBuf  = (BYTE *)m_pArgs[i].pdwVal;
            }
            else
            {
                m_pConfigInfo[i].pBuf = (BYTE *)&(m_pArgs[i].dwVal);
            }

            m_pConfigInfo[i].info.dwDefault = pConfigItems[i].dwDefault;
            m_pConfigInfo[i].info.cb        = sizeof(DWORD);
        }
    }

    ConfigReader *pMgr = CreateConfigReader(m_compId);
    ZAssert(pMgr);

    BOOL fRet;

    if (fRange)
        fRet = pMgr->ReadRgRg(cRange, m_pConfigInfo, nItems);
    else
        fRet = pMgr->ReadRg(m_pConfigInfo, nItems);

    if (!fRet)
    {
        KillArgs();
    }

    delete pMgr;

    return fRet;
}

////////////////////////////////////////////////////////////////////////
void Config::KillArgs()
{
    if (m_pArgs)
    {
        ZAssert(m_pConfigInfo);

        for (DWORD i=0;  i < m_nArgs;  i++)
        {
            delete [] m_pConfigInfo[i].info.pszKeyName;
// bugbug: Now that we store the whole m_pConfigInfo, we don't need to have ARGS at all...
            if (m_pArgs[i].type == kConfigType_SZ)
            {
                delete [] m_pArgs[i].pszVal;
                delete [] m_pConfigInfo[i].info.pszDefault;
            }
            else if (m_fRange)
            {
                delete [] m_pArgs[i].pdwVal;
            }
        }

        delete [] m_pArgs;

        m_pArgs = NULL;
    }

    m_nArgs = 0;

    delete [] m_pConfigInfo;
    m_pConfigInfo = NULL;

}

////////////////////////////////////////////////////////////////////////
BOOL Config::GetDWORDArg(DWORD nArg, DWORD *pdw, int nRange)
{
    ZAssert(pdw);
    ZAssert(nArg < m_nArgs);
    ZAssert(!m_fRange || (nRange < m_cRange));
    ZAssert(m_pArgs);

    if ((nArg >= m_nArgs) || (m_pArgs[nArg].type != kConfigType_DWORD) || (m_fRange && (nRange >= m_cRange)))
    {
        SRVDBG_Info("Failed to retrieve DWORD from registry");
        return FALSE;
    }

    if (m_fRange)
        *pdw = m_pArgs[nArg].pdwVal[nRange];
    else
        *pdw = m_pArgs[nArg].dwVal;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
BOOL Config::GetStringArg(DWORD nArg, char *pszBuf, int cchBuf, int nRange)
{
    ZAssert(pszBuf);
    ZAssert(nArg < m_nArgs);
    ZAssert(m_pArgs);
    ZAssert(!m_fRange || (nRange < m_cRange));

    if ((nArg >= m_nArgs) || (m_pArgs[nArg].type != kConfigType_SZ))
    {
        SRVDBG_Info("Failed to retrieve string from registry");
        return FALSE;
    }

    if (m_fRange)
    {
        if (nRange >= m_cRange)
        {
            SRVDBG_Info("Failed to retrieve string range from registry");
            return FALSE;
        }

        if (cchBuf < lstrlen(&m_pArgs[nArg].pszVal[nRange * m_pArgs[nArg].cb])+1)
        {
            SRVDBG_Info("Failed to retrieve string from registry");
            return FALSE;
        }

        lstrcpy(pszBuf, &m_pArgs[nArg].pszVal[nRange * m_pArgs[nArg].cb]);
    }
    else
    {
        if (cchBuf < lstrlen(m_pArgs[nArg].pszVal)+1)
        {
            SRVDBG_Info("Failed to retrieve string from registry");
            return FALSE;
        }

        lstrcpy(pszBuf, m_pArgs[nArg].pszVal);
    }

    return TRUE;
}

ConfigInfo *Config::GetConfigInfo(int *pcConfig)
{
    ZAssert(m_pConfigInfo);
    ZAssert(pcConfig);

    *pcConfig = m_nArgs;

    if (m_nArgs)
    {
        ConfigInfo *pInfo = new ConfigInfo[m_nArgs];
        ZAssert(pInfo); // all new's assumed to succeed.

        for (DWORD i=0;  i < m_nArgs;  i++)
        {
            pInfo[i] = m_pConfigInfo[i].info;
        }

        return pInfo;
    }

    return NULL;
}



void ConfigCache::DeleteConfigList()
{
    for (int i=0;  i < kCompId_Last;  i++)
    {
        while (m_rgpConfigList[i])
        {
            CONFIG_LIST *pNext = m_rgpConfigList[i]->pNext;

            delete m_rgpConfigList[i]->pConfig;
            delete m_rgpConfigList[i];
            
            m_rgpConfigList[i] = pNext;
        }
    }
}

Config * ConfigCache::FindConfig(ComponentId compId)
{
    ZAssert(compId < kCompId_Last);

    // todo: with a lot of settings stored, linear search here sucks...
    //       Should go to a tree based search.  Perhaps, each node in the tree
    //       being an array of items with close product ID's.  Also might be nice
    //       to allocate all these nodes out of one heap -- for better memory locality,
    //       to avoid paging...
    CONFIG_LIST *pNode = m_rgpConfigList[compId];
    
    while (pNode)
    {
        ZAssert(pNode->pConfig);
        ZAssert(pNode->pConfig->GetComponentId() == compId);

        Config *pCfg = pNode->pConfig;

        return pCfg;

        pNode = pNode->pNext;
    }

    return NULL;
}

void ConfigCache::AddConfig(Config *pConfig)
{
    ZAssert(pConfig);
    ZAssert(pConfig->GetComponentId() < kCompId_Last);

    ComponentId compId = pConfig->GetComponentId();

    CONFIG_LIST *pNode = new CONFIG_LIST;
    ZAssert(pNode);  // we assume all new's succeed...

    m_pcsEditList->Enter();

    pNode->dwTimeAdded      = GetTickCount();
    pNode->pConfig          = pConfig;
    pNode->pNext            = m_rgpConfigList[compId];
    m_rgpConfigList[compId] = pNode;

    m_pcsEditList->Leave();
}

void ConfigCache::AddConfigToTail(Config *pConfig)
{
    ZAssert(pConfig);
    ZAssert(pConfig->GetComponentId() < kCompId_Last);

    ComponentId compId = pConfig->GetComponentId();

    CONFIG_LIST *pNewNode = new CONFIG_LIST;
    ZAssert(pNewNode);  // we assume all new's succeed...

    m_pcsEditList->Enter();

    pNewNode->dwTimeAdded      = GetTickCount();
    pNewNode->pConfig          = pConfig;
    pNewNode->pNext            = NULL;

    CONFIG_LIST **ppNode = &m_rgpConfigList[compId];

    while (NULL != *ppNode)
        ppNode = &((*ppNode)->pNext);

    *ppNode = pNewNode;

    m_pcsEditList->Leave();
}

Config * ConfigCache::RemoveHead(ComponentId compId)
{
    ZAssert(compId < kCompId_Last);

    Config *pRet = NULL;

    m_pcsEditList->Enter();

    if (NULL != m_rgpConfigList[compId])
    {
        CONFIG_LIST *pOld = m_rgpConfigList[compId];

        pRet = pOld->pConfig;

        m_rgpConfigList[compId] = pOld->pNext;

        delete pOld;
    }

    m_pcsEditList->Leave();

    return pRet;
}



Config * ConfigCache::RemoveHeadIfOld(ComponentId compId, DWORD dwDeltaOld)
{
    ZAssert(compId < kCompId_Last);

    Config *pRet = NULL;

    m_pcsEditList->Enter();

    DWORD dwNow = GetTickCount();

    if (NULL != m_rgpConfigList[compId])
    {
        BOOL fOld = FALSE;

        if (m_rgpConfigList[compId]->dwTimeAdded < dwNow)
        {
            if (dwNow - m_rgpConfigList[compId]->dwTimeAdded >= dwDeltaOld)
            {
                fOld = TRUE;
            }
        }
        else
        {
            if ((dwNow > dwDeltaOld) || ((dwNow + (MAXDWORD - m_rgpConfigList[compId]->dwTimeAdded)) > dwDeltaOld))
                fOld = TRUE;
        }

        if (fOld)
        {
            CONFIG_LIST *pOld = m_rgpConfigList[compId];

            pRet = pOld->pConfig;

            m_rgpConfigList[compId] = pOld->pNext;

            delete pOld;
        }
    }

    m_pcsEditList->Leave();

    return pRet;
}


// This routine simply removes the item from the list by setting the
// previous item's pNext to point at what the given node's pNext is...
Config * ConfigCache::ReplaceConfig(ComponentId compId, Config *pConfig)
{
    ZAssert(pConfig);
    ZAssert(pConfig->GetComponentId() < kCompId_Last);
    ZAssert(pConfig->GetComponentId() == compId);

    Config *pRet = NULL;

    m_pcsEditList->Enter();

    // todo: with a lot of settings stored, linear search here sucks...
    CONFIG_LIST *pNode = m_rgpConfigList[compId];
    
    while (pNode)
    {
        ZAssert(pNode->pConfig);
        ZAssert(pNode->pConfig->GetComponentId() == compId);

        break;

        pNode = pNode->pNext;
    }

    // If found, replace.  Else, add...
    if (NULL != pNode)
    {
        pRet = pNode->pConfig;
        pNode->pConfig = pConfig;
    }
    else
    {
        AddConfig(pConfig);
    }

    m_pcsEditList->Leave();

    return pRet;
}


ConfigManager::~ConfigManager()
{
    if (m_hthrGarbageCollect)
    {
        ZAssert(m_hevtDie);

        SetEvent(m_hevtDie);

        if (WAIT_TIMEOUT == WaitForSingleObject(m_hthrGarbageCollect, 20000))
            SRVDBG_Error("Failed while waiting for thread");

        CloseHandle(m_hthrGarbageCollect);
    }

    if (m_hevtDie)
        CloseHandle(m_hevtDie);

    delete m_pcsAdd;
}


BOOL ConfigManager::Init()
{
    ZAssert(NULL == m_hevtDie);
    ZAssert(NULL == m_hthrGarbageCollect);

    if (NULL != m_hevtDie)
    {
        SRVDBG_Warning("Called ConfigManager::Init() twice.");
        return FALSE;
    }

    if (NULL == m_pcsAdd)
    {
        m_pcsAdd = new CriticalSection;
        ZAssert(m_pcsAdd);   // all new's assumed to succeed...
    }

    m_hevtDie = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == m_hevtDie)
    {
        SRVDBG_Error("Failed to create ConfigManager event");
        return FALSE;
    }

    DWORD dwId;
    m_hthrGarbageCollect = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE) GarbageCollectThreadThunk, this, 0, &dwId);

    if (NULL == m_hthrGarbageCollect)
    {
        SRVDBG_Error("Failed to create ConfigManager thread");
        return FALSE;
    }

    return TRUE;
}

BOOL ConfigManager::LoadConfig(ComponentId compId, const ConfigInfo *pInfo, int cInfo)
{
    m_pcsAdd->Enter();

    Config *pCfg = new Config(compId);
    ZAssert(pCfg);   // all new's are assumed to succeed.

    BOOL fRet = pCfg->Init(pInfo, cInfo, FALSE, 0); // if fails, will have reported error to event log

    if (fRet) 
    {
        Config *pOld = m_cacheComponent.ReplaceConfig(compId, pCfg);

        // todo: could do this outside the critsec...
        if (pOld)
            m_cacheGarbage.AddConfigToTail(pOld);
    }
    else
    {
        delete pCfg;
    }

    m_pcsAdd->Leave();

    return fRet;
}

BOOL ConfigManager::ReloadConfig(ComponentId compId)
{
    Config *pCfg = m_cacheComponent.FindConfig(compId);

    if (pCfg)
    {
        int cInfo;

        ConfigInfo *pInfo = pCfg->GetConfigInfo(&cInfo);
        
        if (pInfo)
        {
            BOOL fRet = LoadConfig(compId, pInfo, cInfo);
            delete [] pInfo;

            return fRet;
        }
    }

    return FALSE;
// bugbug:   ... We should log a load...
// bugbug:   ... We should log a reload, too ...
}

BOOL ConfigManager::GetConfigDWORD(ComponentId compId, DWORD nArg, DWORD *pdw)
{
    Config *pCfg = m_cacheComponent.FindConfig(compId);

    if (pCfg)
        return pCfg->GetDWORDArg(nArg, pdw, 0);

    return FALSE;
}

BOOL ConfigManager::GetConfigString(ComponentId compId, DWORD nArg, char *pszBuf, int cchBuf)
{
    Config *pCfg = m_cacheComponent.FindConfig(compId);

    if (pCfg)
        return pCfg->GetStringArg(nArg, pszBuf, cchBuf, 0);

    return FALSE;
}

BOOL ConfigManager::IsConfigLoaded(ComponentId compId)
{
    return (NULL != m_cacheComponent.FindConfig(compId));
}

void ConfigManager::GarbageCollectThread()
{
    ZAssert(this);

    while (TRUE)
    {
        // todo: No hardcoding numbers here, or below...

        // Garbage collector wakes up every so often to delete old, replaced nodes.
        // If the "die now" event is signalled, it deletes all the garbage immediately,
        // and exits...
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_hevtDie, 3*60000L))
        {
            for (int i=0;  i < kCompId_Last;  i++)
            {
                Config *pCfg = m_cacheGarbage.RemoveHead((ComponentId) i);

                while (pCfg)
                {
                    delete pCfg;
                    pCfg = m_cacheGarbage.RemoveHead((ComponentId) i);
                }
            }

            // we're outta here...
            break;
        }
        
        // We don't want to delete a node if it might still be being accessed, so we
        // wait for a while after we've added it to the garbage collector...
        for (int i=0;  i < kCompId_Last;  i++)
        {
            Config *pCfg = m_cacheGarbage.RemoveHeadIfOld((ComponentId) i, 10*60000L);

            while (pCfg)
            {
                delete pCfg;
                pCfg = m_cacheGarbage.RemoveHeadIfOld((ComponentId) i, 10*60000L);
            }
        }
    }
}

BOOL ConfigReader::ReadRg(const ConfigInfoBuffer *prgItems, int cItems)
{
    ZAssert(prgItems);
    ZAssert(cItems);

    BOOL fRet = Open();

    if (fRet)
    {
        // If we fail loading one key -- we keep going, as we'll be putting errors
        // in the event log -- so we might was well show all the missing entries,
        // rather than the 1st one...
        for (int i=0;  i < cItems;  i++)
        {
            switch (prgItems[i].info.type)
            {
                case kConfigType_SZ:
                    fRet &= FastReadSz(prgItems[i].info.pszKeyName, (char *) prgItems[i].pBuf, prgItems[i].info.cb, prgItems[i].info.nRequired, prgItems[i].info.pszDefault);
                    break;

                case kConfigType_DWORD:
                    ZAssert(sizeof(int) == prgItems[i].info.cb);
                    fRet &= FastReadInt(prgItems[i].info.pszKeyName, (int *) prgItems[i].pBuf, prgItems[i].info.nRequired, prgItems[i].info.dwDefault);
                    break;

                default:
                    ZAssert(FALSE);
                    SRVDBG_Error2("Bad reg key type specified", prgItems[i].info.pszKeyName);
                    fRet = FALSE;
                    break;
            }
        }

        Close();
    }

    return fRet;
}

BOOL ConfigReader::ReadRgRg(int cRepeat, const ConfigInfoBuffer *prgItems, int cItems)
{
    ZAssert(prgItems);
    ZAssert(cItems);
    ZAssert(cRepeat);

    // todo?  We currently don't allow a missing hive to be OK, even if all the
    // items had defaults...

    BOOL fRet = Open();

	if (fRet) 
    {
        for (int i=0;  i < cItems;  i++)
        {
            BYTE *pb = prgItems[i].pBuf;

            // If we fail loading one key -- we keep going, as we'll be putting errors
            // in the event log -- so we might was well show all the missing entries,
            // rather than the 1st one...
            for (int j=0;  j < cRepeat;  j++)
            {
                char szT[1024];
    			::wsprintf(szT, "%s%03d", prgItems[i].info.pszKeyName, j);

                switch (prgItems[i].info.type)
                {
                    case kConfigType_SZ:
                        fRet &= FastReadSz(szT, (char *) pb, prgItems[i].info.cb, (j <= prgItems[i].info.nRequired), prgItems[i].info.pszDefault);
                        break;

                    case kConfigType_DWORD:
                        ZAssert(sizeof(int) == prgItems[i].info.cb);
                        fRet &= FastReadInt(szT, (int *) pb, (j <= prgItems[i].info.nRequired), prgItems[i].info.dwDefault);
                        break;

                    default:
                        ZAssert(FALSE);
                        SRVDBG_Error2("Bad reg key type specified", prgItems[i].info.pszKeyName);
                        fRet = FALSE;
                        break;
                }

                pb += prgItems[i].info.cb;
            }
        }

        Close();
    }

    return fRet;
}

BOOL ConfigReader::ReadInt(const char *pszItem, int *pnVal, BOOL fRequired, int nDefault)
{
    ZAssert(pszItem);
    ZAssert(pnVal);

    BOOL fRet = Open();

	if (fRet) 
    {
        fRet = FastReadInt(pszItem, pnVal, fRequired, nDefault);
        Close();
    }
    else
	{
        if (!fRequired)
        {
            fRet   = TRUE;
            *pnVal = nDefault;
        }
	}

    return fRet;
}

BOOL ConfigReader::ReadSz(const char *pszItem, char *psz, int cchMax, BOOL fRequired, const char *pszDefault)
{
    ZAssert(pszItem);
    ZAssert(psz);
    ZAssert(fRequired || pszDefault);
    ZAssert((NULL == pszDefault) || (lstrlen(pszDefault) < cchMax-1));

    BOOL fRet = Open();

	if (fRet) 
    {
        fRet = FastReadSz(pszItem, psz, cchMax, fRequired, pszDefault);
        Close();
    }
    else
	{
        if (!fRequired)
        {
            fRet = TRUE;
            lstrcpy(psz, pszDefault);
        }
	}

    return fRet;
}

BOOL ConfigReader::WriteInt(const char *pszItem, int nVal)
{
    ZAssert(pszItem);

    BOOL fRet = Open();

	if (fRet) 
    {
        fRet = FastWriteInt(pszItem, nVal);
        Close();
    }

    return fRet;
}

BOOL ConfigReader::WriteSz(const char *pszItem, const char *psz, int cch)
{
    ZAssert(pszItem);
    ZAssert(psz);

    BOOL fRet = Open();

	if (fRet) 
    {
        fRet = FastWriteSz(pszItem, psz, cch);
        Close();
    }

    return fRet;
}

ConfigReader *CreateConfigReader(ComponentId compId)
{
// right now, we have no other reader than the registry-based one
#ifdef CONFIG_OTHER_BASED
    return CreateOtherConfigReader(compId);
#else
    return CreateRegConfigReader(compId);
#endif
}

RegConfigReader *CreateRegConfigReader(ComponentId compId)
{
    return new RegConfigReader(compId);
}
