/*-------------------------------------------------------------------------
  AutoUpdate.h
  
  Management of the AutoUpdate system
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


class IAutoUpdate
{
public:

    virtual void     LoadSettings(HKEY hk)           = 0;
    virtual void     LoadCRC(char * szFileName)      = 0;
        
    virtual char *   GetFTPServer()      = 0;
    virtual char *   GetFTPInitialDir()  = 0;
    virtual char *   GetFTPAccount()     = 0;
    virtual char *   GetFTPPassword()    = 0;

    virtual int      GetFileListCRC()    = 0;
    virtual unsigned GetFileListSize()   = 0;
};

void CreateAutoUpdate(HKEY hk, char * szFileName);

extern IAutoUpdate * g_pAutoUpdate; // this is NULL is g_pAutoUpdate is not enabled

