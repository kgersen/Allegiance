#if !defined(__AsyncDebugOutput_h__)
#define __AsyncDebugOutput_h__

/////////////////////////////////////////////////////////////////////////////
// AsyncDebug.h

#include "AutoHandle.h"


/////////////////////////////////////////////////////////////////////////////
// 
class TCAsyncDebugOutput
{
// Construction / Destruction
public:
  TCAsyncDebugOutput(LPCTSTR pszPath, LPCTSTR pszPrefix = TEXT("Async"),
    LPCTSTR pszExt = TEXT("log"));
  ~TCAsyncDebugOutput();

// Operations
public:
  void Write(LPCTSTR pszFmt, ...);
  void WriteV(LPCTSTR pszFmt, va_list argptr);
  void WriteNoTime(LPCTSTR pszFmt, ...);
  void WriteNoTimeV(LPCTSTR pszFmt, va_list argptr);
  void WriteLen(DWORD cchMax, LPCTSTR pszFmt, ...);
  void WriteLenV(DWORD cchMax, LPCTSTR pszFmt, va_list argptr);
  void WriteLenNoTime(DWORD cchMax, LPCTSTR pszFmt, ...);
  void WriteLenNoTimeV(DWORD cchMax, LPCTSTR pszFmt, va_list argptr);

// Implementation
protected:
  static void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered, OVERLAPPED* pov);

// Data Members
protected:
  enum {c_cchMax = 512,};
  TCHandle     m_shFile;
  TCHeapHandle m_shHeap;
  OVERLAPPED   m_ov;
#pragma pack(push, 4)
  LONG         m_nRefs;
#pragma pack(pop)
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction


/////////////////////////////////////////////////////////////////////////////
//
inline TCAsyncDebugOutput::TCAsyncDebugOutput(LPCTSTR pszPath,
  LPCTSTR pszPrefix, LPCTSTR pszExt) :
  m_nRefs(0)
{
  // Initialize the OVERLAPPED structure
  ZeroMemory(&m_ov, sizeof(m_ov));

  // Replace environment variables in the specified path
  TCHAR szPath[_MAX_PATH * 2];
  ::ExpandEnvironmentStrings(pszPath, szPath, sizeof(szPath) / sizeof(TCHAR));

  // Derive a filename from the processID and threadID
  TCHAR szFName[_MAX_PATH];
  _stprintf(szFName, "%s%08X_%08X", pszPrefix ? pszPrefix : TEXT(""),
    ::GetCurrentProcessId(), ::GetCurrentThreadId());

  // Compose the fully-qualified file name
  TCHAR szFileName[_MAX_PATH * 3];
  _tmakepath(szFileName, NULL, szPath, szFName, pszExt);

  // Open the file for async writing
  DWORD dwFlags =
    FILE_ATTRIBUTE_NOT_CONTENT_INDEXED |
    FILE_FLAG_OVERLAPPED               |
    FILE_FLAG_SEQUENTIAL_SCAN;
  m_shFile = ::CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
    CREATE_ALWAYS, dwFlags, NULL);
  if (INVALID_HANDLE_VALUE == m_shFile.GetHandle())
    m_shFile = NULL;

  // Create a private heap
  if (!m_shFile.IsNull())
    m_shHeap = ::HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
}


/////////////////////////////////////////////////////////////////////////////
//
inline TCAsyncDebugOutput::~TCAsyncDebugOutput()
{
  // Wait for all outstanding I/O's to complete
  while (m_nRefs)
    ::SleepEx(10, true);
}


/////////////////////////////////////////////////////////////////////////////
// Operations


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::Write(LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  WriteV(pszFmt, argptr);
  va_end(argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteV(LPCTSTR pszFmt, va_list argptr)
{
  WriteLenV(c_cchMax, pszFmt, argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteNoTime(LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  WriteNoTimeV(pszFmt, argptr);
  va_end(argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteNoTimeV(LPCTSTR pszFmt, va_list argptr)
{
  WriteLenNoTimeV(c_cchMax, pszFmt, argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteLen(DWORD cchMax, LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  WriteLenV(cchMax, pszFmt, argptr);
  va_end(argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteLenV(DWORD cchMax, LPCTSTR pszFmt, va_list argptr)
{
  if (!m_shFile.IsNull())
  {
    // Increment cchMax to account for the formatted SYSTEMTIME
    const DWORD c_cchPrefix = 19; // time format: "12/31 23:59:59.999 "
    cchMax += c_cchPrefix;

    // Calculate the number of bytes to allocate
    DWORD cb = cchMax * sizeof(TCHAR) + sizeof(OVERLAPPED) + sizeof(this);
    void* pv = ::HeapAlloc(m_shHeap, 0, cb);
    TCAsyncDebugOutput** ppThis = reinterpret_cast<TCAsyncDebugOutput**>(pv);
    if (ppThis)
    {
      // Save the 'this' pointer just before the OVERLAPPED structure
      *ppThis = this;
      OVERLAPPED* pov = reinterpret_cast<OVERLAPPED*>(ppThis + 1);

      // Initialize the OVERLAPPED structure
      CopyMemory(pov, &m_ov, sizeof(*pov));

      // Determine the pointer value for the text string
      LPTSTR pszMsg = reinterpret_cast<TCHAR*>(pov + 1);

      // Format the local time into the allocated buffer
      SYSTEMTIME st;
      ::GetLocalTime(&st);
      int cchTime = _stprintf(pszMsg, TEXT("%02d/%02d %02d:%02d:%02d.%03d "),
        st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

      // Format the message into the allocated buffer
      _vstprintf(pszMsg + cchTime, pszFmt, argptr);

      // Determine the actual size of the write
      DWORD cbMsg = _tcslen(pszMsg) * sizeof(TCHAR);

      // Write the message to the file
      WriteFileEx(m_shFile, pszMsg, cbMsg, pov, FileIOCompletionRoutine);
      m_ov.Offset += cbMsg;

      // Increment the outstanding file I/O refcount
      ::InterlockedIncrement(&m_nRefs);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteLenNoTime(DWORD cchMax, LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  WriteLenNoTimeV(cchMax, pszFmt, argptr);
  va_end(argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
inline void TCAsyncDebugOutput::WriteLenNoTimeV(DWORD cchMax, LPCTSTR pszFmt, va_list argptr)
{
  if (!m_shFile.IsNull())
  {
    // Calculate the number of bytes to allocate
    DWORD cb = cchMax * sizeof(TCHAR) + sizeof(OVERLAPPED) + sizeof(this);
    void* pv = ::HeapAlloc(m_shHeap, 0, cb);
    TCAsyncDebugOutput** ppThis = reinterpret_cast<TCAsyncDebugOutput**>(pv);
    if (ppThis)
    {
      // Save the 'this' pointer just before the OVERLAPPED structure
      *ppThis = this;
      OVERLAPPED* pov = reinterpret_cast<OVERLAPPED*>(ppThis + 1);

      // Initialize the OVERLAPPED structure
      CopyMemory(pov, &m_ov, sizeof(*pov));

      // Determine the pointer value for the text string
      LPTSTR pszMsg = reinterpret_cast<TCHAR*>(pov + 1);

      // Format the message into the allocated buffer
      _vstprintf(pszMsg, pszFmt, argptr);

      // Determine the actual size of the write
      DWORD cbMsg = _tcslen(pszMsg) * sizeof(TCHAR);

      // Write the message to the file
      WriteFileEx(m_shFile, pszMsg, cbMsg, pov, FileIOCompletionRoutine);
      m_ov.Offset += cbMsg;

      // Increment the outstanding file I/O refcount
      ::InterlockedIncrement(&m_nRefs);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline void CALLBACK TCAsyncDebugOutput::FileIOCompletionRoutine(
  DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, OVERLAPPED* pov)
{
  // Reinterpret the specified pointer
  TCAsyncDebugOutput** ppThis = reinterpret_cast<TCAsyncDebugOutput**>(pov) - 1;
  TCAsyncDebugOutput* pThis = *(ppThis);

  // Free the memory
  ::HeapFree(pThis->m_shHeap, 0, ppThis);

  // Decrement the outstanding file I/O refcount
  ::InterlockedDecrement(&pThis->m_nRefs);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__AsyncDebugOutput_h__)