#include "FTPSession.h"
#include "tref.h"

class CFTPSessionImpl : public IFTPSession {
public:
    CFTPSessionImpl(IFTPSessionUpdateSink* pSink) {}
    bool ConnectToSite(const char* szFTPSite, const char* szDirectory, const char* szUsername, const char* szPassword) override { return false; }
    bool InitiateDownload(const char* const* pszFileList, const char* szDestFolder, bool bDisconnectWhenDone, int nMaxBufferSize) override { return false; }
    bool ContinueDownload() override { return false; }
    bool Disconnect() override { return false; }
    const char* GetDownloadPath() override { return nullptr; }
    const char* GetLastErrorMessage() override { return "Not implemented on Linux"; }
    void Abort(bool bAutoDisconnect) override {}
};

IFTPSession* CreateFTPSession(IFTPSessionUpdateSink* pSink) {
    return nullptr;
}

class CHTTPSessionImpl : public IHTTPSession {
public:
    CHTTPSessionImpl(IHTTPSessionSink* pSink) {}
    bool InitiateDownload(const char* const* pszFileList, const char* szDestFolder, int nMaxBufferSize) override { return false; }
    bool ContinueDownload() override { return false; }
    const char* GetDownloadPath() override { return nullptr; }
    const char* GetLastErrorMessage() override { return "Not implemented on Linux"; }
    void Abort(bool bAutoDisconnect) override {}
};

IHTTPSession* CreateHTTPSession(IHTTPSessionSink* pSink) {
    return nullptr;
}
