/////////////////////////////////////////////////////////////////////////////
// TCNullStream.cpp | Implementation of the CTCNullStreamImpl class.<nl><nl>
// This object provides an implementation of the IStream interface that does
// nothing more that count the number of bytes being written to it.  Since
// ATL (currently 3.0) implements IPersistStreamInitImpl::GetSizeMax by
// returning E_NOTIMPL, this object can be used in an override of that
// method. This is accomplished by creating an instance of this null stream
// and 'persisting' an object to it, thereby counting the number of bytes
// needed to persist the object without allocating any memory.
//


/////////////////////////////////////////////////////////////////////////////
// CTCNullStreamImpl

#include "TCNullStreamImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Construction

CTCNullStreamImpl::CTCNullStreamImpl() :
  m_nPosition(0),
  m_nSize(0)
{
  // Get the current FILETIME
  GetSystemTimeAsFileTime(&m_ftCreated);
  m_ftModified = m_ftAccessed = m_ftCreated;
}


/////////////////////////////////////////////////////////////////////////////
// ISequentialStream Interface Methods

STDMETHODIMP CTCNullStreamImpl::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
  HRESULT hr = S_OK;
  __try
  {
    // Lock the object
    Lock();

    // Compute the number of bytes that can be read
    ULONG nMax = m_nSize - m_nPosition;
    nMax = min(nMax, cb);

    // Clear the specified buffer for the maximum number of bytes
    ZeroMemory(pv, nMax);

    // Update the current position
    m_nPosition += nMax;

    // Get the current FILETIME
    GetSystemTimeAsFileTime(&m_ftAccessed);

    // Return the number of bytes read, if requested
    if (pcbRead)
      *pcbRead = nMax;
  }
  __except(1)
  {
    hr = STG_E_INVALIDPOINTER;
  }

  // Unlock the object
  Unlock();

  // Return the last result
  return hr;
}

STDMETHODIMP CTCNullStreamImpl::Write(const void* pv, ULONG cb,
  ULONG* pcbWritten)
{
  UNUSED(pv);

  HRESULT hr = S_OK;
  __try
  {
    // Lock the object
    Lock();

    // Increment the current position
    m_nPosition += cb;

    // Compute the new size
    if (m_nPosition > m_nSize)
      m_nSize = m_nPosition;

    // Get the current FILETIME
    GetSystemTimeAsFileTime(&m_ftModified);

    // Return the number of bytes written, if requested
    if (pcbWritten)
      *pcbWritten = cb;
  }
  __except(1)
  {
    hr = STG_E_INVALIDPOINTER;
  }

  // Unlock the object
  Unlock();

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// IStream Interface Methods

STDMETHODIMP CTCNullStreamImpl::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin,
  ULARGE_INTEGER* plibNewPosition)
{
  // Clear the specified [out] parameter, if not NULL
  if (plibNewPosition)
    TCZeroMemory(plibNewPosition);

  // Lock the object
  CLock lock(this);

  switch (dwOrigin)
  {
    case STREAM_SEEK_SET:
    {
      // Validate the position pointer as unsigned and in 32-bit range
      if (0 != dlibMove.HighPart)
        return STG_E_INVALIDFUNCTION;

      // Compute the new position
      m_nPosition = dlibMove.LowPart;
      break;
    }

    case STREAM_SEEK_CUR:
    {
      // Validate the position pointer as signed and in 32-bit range
      if (0 != dlibMove.HighPart && -1 != dlibMove.HighPart)
        return STG_E_INVALIDFUNCTION;

      // Compute the new position
      __int64 nPositionNew = __int64(m_nPosition) + LONG(dlibMove.LowPart);
      if (nPositionNew > ULONG_MAX)
        return STG_E_MEDIUMFULL;
      m_nPosition = ULONG(nPositionNew);
      break;
    }

    case STREAM_SEEK_END:
    {
      // Validate the position pointer as signed and in 32-bit range
      if (0 != dlibMove.HighPart && -1 != dlibMove.HighPart)
        return STG_E_INVALIDFUNCTION;

      // Compute the new position
      __int64 nPositionNew = __int64(m_nSize) + LONG(dlibMove.LowPart);
      if (nPositionNew > ULONG_MAX)
        return STG_E_MEDIUMFULL;
      m_nPosition = ULONG(nPositionNew);

      break;
    }

    default:
      return STG_E_INVALIDFUNCTION;
  }

  // Compute the new size, if position is beyond the end
  if (m_nPosition > m_nSize)
    m_nSize = m_nPosition;

  // Return the new position, if requested
  if (plibNewPosition)
    plibNewPosition->LowPart = m_nPosition;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCNullStreamImpl::SetSize(ULARGE_INTEGER libNewSize)
{
  // Validate the specified size as in 32-bit range
  if (0 == libNewSize.HighPart)
    return STG_E_INVALIDFUNCTION;

  // Lock the object
  CLock lock(this);

  // Set the new size of the stream
  m_nSize = libNewSize.LowPart;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCNullStreamImpl::CopyTo(IStream* pstm, ULARGE_INTEGER cb,
  ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
  // Initialize the [out] parameters
  if (pcbRead)
    TCZeroMemory(pcbRead);
  if (pcbWritten)
    TCZeroMemory(pcbWritten);

  // Ensure that the specified stream pointer is valid
  IStreamPtr spstm;
  RETURN_FAILED(TCSafeQI(pstm, &spstm));

  // Validate the specified size as in 32-bit range
  if (0 == cb.HighPart)
    return STG_E_INVALIDFUNCTION;

  // Initialize a chunk of memory to copy into the specified stream
  BYTE pBuf[2048];
  const int cbBuf = sizeof(pBuf);
  ZeroMemory(pBuf, cbBuf);

  // Lock the object
  CLock lock(this);

  // Compute the number of bytes that can be read
  ULONG nMax = m_nSize - m_nPosition;
  nMax = min(nMax, cb.LowPart);

  // Copy the NULL memory chunk for the specified number of bytes
  HRESULT hr;

  ULONG nRemaining;

  for (nRemaining = nMax; nRemaining > cbBuf; nRemaining -= cbBuf)
    if (FAILED(hr = spstm->Write(pBuf, cbBuf, NULL)))
      return hr;

  // Copy any remaining bytes
  if (nRemaining && FAILED(hr = spstm->Write(pBuf, nRemaining, NULL)))
    return hr;

  // Update the current position
  m_nPosition += nMax;

  // Get the current FILETIME
  GetSystemTimeAsFileTime(&m_ftAccessed);

  // Return the number of bytes read, if requested
  if (pcbRead)
    pcbRead->LowPart = nMax;

  // Return the number of bytes written, if requested
  if (pcbWritten)
    pcbWritten->LowPart = nMax;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCNullStreamImpl::Commit(DWORD grfCommitFlags)
{
  UNUSED(grfCommitFlags);
  return E_NOTIMPL;
}

STDMETHODIMP CTCNullStreamImpl::Revert(void)
{
  return E_NOTIMPL;
}

STDMETHODIMP CTCNullStreamImpl::LockRegion(ULARGE_INTEGER libOffset,
  ULARGE_INTEGER cb, DWORD dwLockType)
{
  UNUSED(libOffset);
  UNUSED(cb);
  UNUSED(dwLockType);
  return E_NOTIMPL;
}

STDMETHODIMP CTCNullStreamImpl::UnlockRegion(ULARGE_INTEGER libOffset,
  ULARGE_INTEGER cb, DWORD dwLockType)
{
  UNUSED(libOffset);
  UNUSED(cb);
  UNUSED(dwLockType);
  return E_NOTIMPL;
}

STDMETHODIMP CTCNullStreamImpl::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
  UNUSED(grfStatFlag);

  // Initialize the specified structures
  TCZeroMemory(pstatstg);

  // Lock the object
  CLock lock(this);

  // Populate the fields of the specified structure
  pstatstg->pwcsName          = NULL;
  pstatstg->type              = STGTY_STREAM; 
  pstatstg->cbSize.HighPart   = 0;
  pstatstg->cbSize.LowPart    = m_nSize;
  pstatstg->mtime             = m_ftModified;
  pstatstg->ctime             = m_ftCreated;
  pstatstg->atime             = m_ftAccessed;
  pstatstg->grfMode           = STGM_READWRITE | STGM_SHARE_DENY_NONE;
  pstatstg->grfLocksSupported = 0;
  pstatstg->clsid             = CLSID_NULL;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCNullStreamImpl::Clone(IStream** ppstm)
{
  // Initialize the [out] parameter
  CLEAROUT(ppstm, (IStream*)NULL);

  // Create an new instance of ourself
  typedef CComObject<CTCNullStreamImpl> CNullStreamObject;
  CNullStreamObject* pObject = NULL;
  HRESULT hr = CNullStreamObject::CreateInstance(&pObject);
  RETURN_FAILED(hr);

  // The QueryInterface will put a refcount of 1 on the new object
  IStreamPtr pstm(pObject->GetUnknown());

  // Lock the object
  CLock lock(this);

  // Initialize the new stream with our seek pointer and size
  LARGE_INTEGER dlibMove = {0, m_nPosition};
  ULARGE_INTEGER libNewSize = {0, m_nSize};
  _SVERIFY(hr = pstm->Seek(dlibMove, STREAM_SEEK_SET, NULL));
  _SVERIFY(hr = pstm->SetSize(libNewSize));

  // Detach the new stream to the specified [out] parameter
  *ppstm = pstm;
  pstm.Detach();

  // Indicate success
  return S_OK;
}

