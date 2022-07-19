#ifndef __WindowFinder_h__
#define __WindowFinder_h__

/////////////////////////////////////////////////////////////////////////////
// WindowFinder.h : Declaration of the TCWindowFinder class.
//

class TCWindowFinder
{
// Construction
public:
  TCWindowFinder(UINT idTitle, bool bIgnoreCase = false,
    DWORD dwProcessID = 0);
  TCWindowFinder(CString strTitle, bool bIgnoreCase = false,
    DWORD dwProcessID = 0);

// Attributes
public:
  WNDENUMPROC GetEnumProc();
  LPARAM GetLPARAM();
  HWND GetHwnd() const;

// Operators
public:
  operator WNDENUMPROC();
  operator LPARAM();
  operator HWND() const;

// Implementation
protected:
  static BOOL CALLBACK WindowFindProc(HWND hwnd, TCWindowFinder* pThis);

// Data Members
protected:
  CString m_strTitle;
  HWND    m_hwnd;
  bool    m_bNoCase;
  DWORD   m_dwPID;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

inline TCWindowFinder::TCWindowFinder(UINT idTitle, bool bIgnoreCase,
  DWORD dwProcessID) :
  m_hwnd(NULL),
  m_strTitle(reinterpret_cast<LPCTSTR>(idTitle)),
  m_bNoCase(bIgnoreCase),
  m_dwPID(dwProcessID)
{
}

inline TCWindowFinder::TCWindowFinder(CString strTitle, bool bIgnoreCase,
  DWORD dwProcessID) :
  m_hwnd(NULL),
  m_strTitle(strTitle),
  m_bNoCase(bIgnoreCase),
  m_dwPID(dwProcessID)
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

inline WNDENUMPROC TCWindowFinder::GetEnumProc()
{
  return reinterpret_cast<WNDENUMPROC>(WindowFindProc);
}

inline LPARAM TCWindowFinder::GetLPARAM()
{
  return reinterpret_cast<LPARAM>(this);
}

inline HWND TCWindowFinder::GetHwnd() const
{
  return m_hwnd;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

inline TCWindowFinder::operator WNDENUMPROC()
{
  return GetEnumProc();
}

inline TCWindowFinder::operator LPARAM()
{
  return GetLPARAM();
}

inline TCWindowFinder::operator HWND() const
{
  return GetHwnd();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

inline BOOL CALLBACK TCWindowFinder::WindowFindProc(HWND hwnd,
  TCWindowFinder* pThis)
{
  // If a process ID was specified, check the window process ID
  if (pThis->m_dwPID)
  {
    DWORD dwPID;
    GetWindowThreadProcessId(hwnd, &dwPID);
    if (pThis->m_dwPID != dwPID)
      return true;
  }

  // Get the text of the specified window
  TCHAR sz[_MAX_PATH];
  GetWindowText(hwnd, sz, sizeof(sz) / sizeof(sz[0]));

  // Compare the strings, as specified
  bool bFound = 0 == (pThis->m_bNoCase ?
    pThis->m_strTitle.CompareNoCase(sz) : pThis->m_strTitle.Compare(sz));

  // Save the hwnd, if this is the one
  if (bFound)
    pThis->m_hwnd = hwnd;

  // Continue or stop the enumeration
  return !bFound;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__WindowFinder_h__
