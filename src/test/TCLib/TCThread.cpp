/////////////////////////////////////////////////////////////////////////////
// TCThread.cpp | Implementation of the TCThread class.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "TCThread.h"


/////////////////////////////////////////////////////////////////////////////
// TCThread


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

TCThread::TCThread() :
  m_pThreadParams(NULL),
  m_pfnThreadProc(NULL),
  m_hThread(NULL)
{
  // Perform common construction
  CommonConstruct();
}

TCThread::TCThread(TC_THREADPROC pfnThreadProc, LPVOID pParam) :
  m_pThreadParams(pParam),
  m_pfnThreadProc(pfnThreadProc)
{
  // Perform common construction
  CommonConstruct();
}

void TCThread::CommonConstruct()
{
  m_hThread = NULL;
  m_nThreadID = 0;
  m_bAutoDelete = true;
  m_bError = false;
  m_bMsgQueue = false;
}

TCThread::~TCThread()
{
  // free thread object
  if (NULL != m_hThread)
    CloseHandle(m_hThread);
}

TCThread* TCThread::BeginThread(TC_THREADPROC pfnThreadProc, LPVOID pParam,
  int nPriority, UINT nStackSize, DWORD dwCreateFlags,
  LPSECURITY_ATTRIBUTES pSecurityAttrs)
{
  PRIVATE_ASSERTE(pfnThreadProc);
  if (pfnThreadProc)
  {
    TCThread* pThread = new TCThread(pfnThreadProc, pParam);
    if (!pThread->CreateThread(false, dwCreateFlags | CREATE_SUSPENDED,
      nStackSize, pSecurityAttrs))
    {
      pThread->Delete();
      return NULL;
    }

    PRIVATE_VERIFYE(pThread->SetThreadPriority(nPriority));
    if (!(dwCreateFlags & CREATE_SUSPENDED))
      PRIVATE_VERIFYE(pThread->ResumeThread() != (DWORD)-1);

    return pThread;
  }

  return NULL;
}

TCThread* TCThread::BeginMsgThread(TC_THREADPROC pfnThreadProc, LPVOID pParam,
  int nPriority, UINT nStackSize, DWORD dwCreateFlags,
  LPSECURITY_ATTRIBUTES pSecurityAttrs)
{
  PRIVATE_ASSERTE(pfnThreadProc);
  if (pfnThreadProc)
  {
    TCThread* pThread = new TCThread(pfnThreadProc, pParam);
    if (!pThread->CreateThread(true, dwCreateFlags | CREATE_SUSPENDED,
      nStackSize, pSecurityAttrs))
    {
      pThread->Delete();
      return NULL;
    }

    PRIVATE_VERIFYE(pThread->SetThreadPriority(nPriority));
    if (!(dwCreateFlags & CREATE_SUSPENDED))
      PRIVATE_VERIFYE(pThread->ResumeThread() != (DWORD)-1);

    return pThread;
  }

  return NULL;
}

bool TCThread::CreateThread(bool bMsgQueue, DWORD dwCreateFlags,
  UINT nStackSize, LPSECURITY_ATTRIBUTES pSecurityAttrs)
{
  PRIVATE_ASSERTE(m_hThread == NULL);  // already created?

  m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
  m_hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
  m_hEventExit = ::CreateEvent(NULL, TRUE, FALSE, NULL);
  m_dwCreateFlags = dwCreateFlags;
  if (!m_hEvent || !m_hEvent2 || !m_hEventExit)
  {
    _TRACE0("Warning: CreateEvent failed in TCThread::CreateThread.\n");
    m_hEvent = NULL;
    m_hEvent2 = NULL;
    m_hEventExit = NULL;
    return false;
  }

  // create the thread (it may or may not start to run)
  m_hThread = (HANDLE)_beginthreadex(pSecurityAttrs, nStackSize,
    (m_bMsgQueue = bMsgQueue) ? TCMsgThreadEntry : TCThreadEntry, this,
    dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);

  if (NULL == m_hThread)
    return false;

  // start the thread just for initialization
  PRIVATE_VERIFYE(ResumeThread() != (DWORD)-1);
  PRIVATE_VERIFYE(::WaitForSingleObject(m_hEvent, INFINITE) == WAIT_OBJECT_0);
  m_hEvent = NULL;

  // if created suspended, suspend it until resume thread wakes it up
  if (dwCreateFlags & CREATE_SUSPENDED)
    PRIVATE_VERIFYE(DWORD(-1) != ::SuspendThread(m_hThread));

  // if error during startup, shut things down
  if (m_bError)
  {
    PRIVATE_VERIFYE(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hThread, INFINITE));
    m_hEvent = NULL;
    m_hEvent2 = NULL;
    m_hEventExit = NULL;
    m_hThread = NULL;
    return false;
  }

  // allow thread to continue, once resumed (it may already be resumed)
  ::SetEvent(m_hEvent2);
  return true;
}

void TCThread::Delete()
{
  // delete thread if it is auto-deleting
  if (m_bAutoDelete)
    delete this;
}

void TCThread::EndThread(UINT nExitCode, bool bDelete)
{
  if (bDelete)
    Delete();

  // allow C-runtime to cleanup, and exit the thread
  _endthreadex(nExitCode);
}

UINT __stdcall TCThread::TCThreadEntry(void* pParam)
{
  TCThread* pThread = (TCThread*)pParam;
  PRIVATE_ASSERTE(NULL != pThread);
  PRIVATE_ASSERTE(!pThread->m_hEvent.IsNull());
  PRIVATE_ASSERTE(!pThread->m_bError);
  PRIVATE_ASSERTE(!pThread->m_hEventExit.IsNull());

  // allow the creating thread to return from TCThread::CreateThread
  BOOL bSucceeded = ::SetEvent(pThread->m_hEvent);
  PRIVATE_ASSERTE(bSucceeded);

  // wait for thread to be resumed
  DWORD dwWait = ::WaitForSingleObject(pThread->m_hEvent2, INFINITE);
  PRIVATE_ASSERTE(WAIT_OBJECT_0 == dwWait);
  pThread->m_hEvent2 = NULL;

  // Call the passed in thread proc
  DWORD nResult = 0;
  if (NULL != pThread->m_pfnThreadProc)
  {
    nResult = (*pThread->m_pfnThreadProc)(pThread->m_pThreadParams);
    PRIVATE_ASSERTE(pThread);
    pThread->EndThread(nResult, pThread->m_bAutoDelete);
  }

  return 0;   // not reached
}

UINT __stdcall TCThread::TCMsgThreadEntry(void* pParam)
{
  TCThread* pThread = (TCThread*)pParam;
  PRIVATE_ASSERTE(NULL != pThread);
  PRIVATE_ASSERTE(!pThread->m_hEvent.IsNull());
  PRIVATE_ASSERTE(!pThread->m_bError);
  PRIVATE_ASSERTE(!pThread->m_hEventExit.IsNull());

  // Force a message queue to be created
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

  // allow the creating thread to return from TCThread::CreateMsgThread
  BOOL bSucceeded = ::SetEvent(pThread->m_hEvent);
  PRIVATE_ASSERTE(bSucceeded);

  // wait for thread to be resumed
  DWORD dwWait = ::WaitForSingleObject(pThread->m_hEvent2, INFINITE);
  PRIVATE_ASSERTE(WAIT_OBJECT_0 == dwWait);
  pThread->m_hEvent2 = NULL;

  // Call the passed in thread proc
  DWORD nResult = 0;
  if (NULL != pThread->m_pfnThreadProc)
  {
    nResult = (*pThread->m_pfnThreadProc)(pThread->m_pThreadParams);
    PRIVATE_ASSERTE(pThread);
    pThread->EndThread(nResult, pThread->m_bAutoDelete);
  }

  return 0;   // not reached
}

