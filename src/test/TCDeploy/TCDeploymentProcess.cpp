/////////////////////////////////////////////////////////////////////////////
// TCDeploymentProcess.cpp : Implementation of the TCDeploymentProcess class.

#include "pch.h"
#include "TCDeploy.h"
#include "TCDeploymentProcess.h"


/////////////////////////////////////////////////////////////////////////////
// TCDeploymentProcess

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(TCDeploymentProcess)


/////////////////////////////////////////////////////////////////////////////
// ITCDeploymentProcess Interface Methods

STDMETHODIMP TCDeploymentProcess::get_IsCompleted(VARIANT_BOOL* pbCompleted)
{
  assert(!m_shProcess.IsNull());

  // Get the completion status of the process
  bool bCompleted = WAIT_OBJECT_0 == WaitForSingleObject(m_shProcess, 0);
  
  // Initialize the [out] parameter
  CLEAROUT(pbCompleted, VARBOOL(bCompleted));

  // Indicate success
  return S_OK;
}

STDMETHODIMP TCDeploymentProcess::get_ExitCode(long* pnExitCode)
{
  assert(!m_shProcess.IsNull());

  // Initialize the [out] parameter
  CLEAROUT(pnExitCode, 0L);

  // Get the exit code of the process
  DWORD dwExitCode;
  if (!GetExitCodeProcess(m_shProcess, &dwExitCode))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Copy the exit code to the [out] parameter
  *pnExitCode = (long)dwExitCode;

  // Indicate success
  return S_OK;
}

