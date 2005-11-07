#pragma once

/*-------------------------------------------------------------------------
 * FedSrvApp
 *-------------------------------------------------------------------------
 * Purpose:
 *    This class is called by the assert code when an assert happens
 */
#define FED_DEBUG_DEBUGOUT 1
#define FED_DEBUG_FILE     2

#ifdef _DEBUG


  class FedSrvApp : public Win32App 
  {
  public:
    FedSrvApp(void)
    :
        m_hFile(NULL),
        m_dwDebug(FED_DEBUG_DEBUGOUT)
    {
    }

    ~FedSrvApp(void)
    {
        CloseLogFile();
    }

    virtual void OpenLogFile(void)
    {
        CloseLogFile();

        char    bfr[200];
        GetModuleFileName(NULL, bfr, 150);
        char*   p = strrchr(bfr, '.');
        if (p)
            *(p + 1) = '\0';
        //_itoa(Time::Now().clock(), bfr + strlen(bfr), 10);
        strcat(bfr, "txt");

        m_hFile = CreateFile(bfr, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_FLAG_OVERLAPPED, NULL);
        assert (m_hFile);

        m_overlapped.Offset = 0;
        m_overlapped.OffsetHigh = 0;

        m_overlapped.hEvent = NULL;
        m_nOffset = 0;
    }

    virtual void CloseLogFile(void)
    {
        if (m_hFile)
        {
            CloseHandle(m_hFile);
            m_hFile = NULL;
        }
    }

    virtual void DebugOutput(const char *psz);
    virtual bool OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
    virtual void OnAssertBreak();

    void SetDebug(DWORD dw)
    {
      if (!(m_dwDebug & FED_DEBUG_FILE) && (dw & FED_DEBUG_FILE))
        OpenLogFile();
      else if ((m_dwDebug & FED_DEBUG_FILE) && !(dw & FED_DEBUG_FILE))
        CloseLogFile();
      char buff[640];
      wsprintf(buff, "Debug level set from %u to %u.\n", m_dwDebug, dw);
      m_dwDebug = dw;
      DebugOutput(buff);
    }
      
    DWORD GetDebug()
    {
        return m_dwDebug;
    }
      

  private:
    void AsyncFileOut(const char *psz);
    
      HANDLE        m_hFile;
      OVERLAPPED    m_overlapped;
      DWORD         m_dwDebug; // See FED_* above
    #pragma pack(push, 4)
      LONG          m_nOffset;
    #pragma pack(pop)
  
  };

  extern FedSrvApp g_app;

#endif
