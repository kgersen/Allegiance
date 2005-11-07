#ifndef __AutoHandle_h__
#define __AutoHandle_h__

/////////////////////////////////////////////////////////////////////////////
// AutoHandle.h | Declaration of the TCAutoHandle template class.
//


/////////////////////////////////////////////////////////////////////////////
// Encapsulates a handle type, /H/, which is any type for which an open/close
// (or an allocate/release) concept must be honored.
//
// The versatility of this class is due mostly to the /FNCLOSE/ template
// argument, which specifies a function class used to close (or release) the
// encapsualated handle.
//
// With the increased use of try-catch blocks, it is useful to have all
// objects able to clean up after themselves when they go out of scope.  This
// also makes it easy to return from a function as soon as one or more
// conditions are met, without having to worry about closing everything that
// has been opened up to that point.  Overall, self-cleaning objects will
// generally lead to stronger code with fewer memory and resource leaks.
// 
// Since this is common pattern, it seemed to be a likely candidate for a
// simple and generic template class. This has resulted in the template
// class, TCAutoHandle, to automatically clean-up a Win32 handle. Because
// this is a template, it was also quite simple to make the class work for a
// large number of cases where objects or pointers can close themselves upon
// leaving scope.
// 
// Since the template class only concerns itself with the closing (or
// releasing) of handles, it does not need to be concerned with the endless
// possibilities of creating, opening, or initializing the objects. To
// overcome this, it just assumes ownership of the handle through a
// constructor, the assignment operator, the Attach method, or the
// address-of operator&. Be careful with those last two, since they will
// first close any currently attached handle.
// 
// The class also has a casting operator for the handle type.  This makes it
// easy to use an instance of the class anywhere you would normally use the
// handle type.  Use common sense with this operator since it could be used
// incorrectly in one imaginable way. Consider, for example, you pass an auto
// handle object to the global function that closes it (the CloseHandle API
// for TCHandle). It will be closed but the auto handle object will still
// hold the handle value. When the auto handle object is destroyed, it will
// close the handle a second time. However, this is easily avoidable since
// the whole idea of this template is to shield you from having to ever call
// the global closing function. Instead, you should either call the Close
// method of the auto handle object, or just allow it to go out of scope.
// 
// Notice that the second template argument, /FNCLOSE/, is a "function
// object" class, which just means a class that has an operator(), the
// parenthesis (or function call) operator.  Using a function object is quite
// efficient since the function call operator is inline and the whole class
// optimizes out. This is also the technique used by the Standard Template
// Library (STL) to achieves much of its generality.  The template is
// implemented with a function object so that different types of handles (or
// pointers) can be closed in a way that makes sense for that type. Since
// most of the Win32 object closing API's take one argument, the object
// handle, and return a success/failure code, this was fairly straightforward
// to make generic.
// 
// Most of the usable classes in this group are actually just typedef's that
// specify the handle type and a function object used to close the handle.
// The following are the types currently defined. If you can think of any
// other objects that would fit into this pattern, please update this file
// and *be sure to update these comments.* Note that several of these are
// unnecessary when using MFC, since that framework already declares
// encapsulations of some of the Win32 objects - the GDI objects,
// specifically.
//
// + TCHandle
// + TCFileFindHandle
// + TCViewOfFile
// + TCRegKeyHandle
// + TCServiceHandle
// + TCGDIObjHandle
// + TCClientDC
// + TCWindowDC
// + TCCreatedDC
// + TCRedrawScope
// + TCSysString
// + TCSafeArrayAccess
// + TCSafeArrayLock
// + TCDeferWindowPosHandle
// + TCPtr
// + TCSinglePtr
// + TCArrayPtr
// 
template <class H, class FNCLOSE>
class TCAutoHandle
{
// Construction / Destruction
public:
  TCAutoHandle();
  TCAutoHandle(H h);
  ~TCAutoHandle();
  void Attach(H h);
  H Detach();

// Attributes
public:
  H GetHandle() const;
  bool IsNull() const;
  TCAutoHandle* This();
  const TCAutoHandle* This() const;

// Operations
public:
  bool Close();

// Operators
public:
  operator H();
  operator H() const;
  TCAutoHandle& operator=(H h);
  H* operator &();

// Data Members
protected:
  // Description: The encapsulated handle.
  H m_h;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>::TCAutoHandle()
  : m_h((H)0)
{
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   h - The handle value to be encapsulated.
//
// See Also: TCAutoHandle::operator=, TCAutoHandle::Attach
template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>::TCAutoHandle(H h)
  : m_h(h)
{
}

/////////////////////////////////////////////////////////////////////////////
// Remarks: Closes the handle, if any.
//
// See Also: TCAutoHandle::Close
template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>::~TCAutoHandle()
{
  Close();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Encapsulates the specified handle.
//
// Parameters:
//   h - The handle value to be encapsulated.
//
// Remarks: Encapsulates the specified handle.  If another object is
// currently being encapsulated, it will be closed first.
//
// See Also: TCAutoHandle::Close, TCAutoHandle::operator=,
// TCAutoHandle::constructor
template <class H, class FNCLOSE>
inline void TCAutoHandle<H, FNCLOSE>::Attach(H h)
{
  Close();
  m_h = h;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Detaches the encapsulated handle.
//
// Return Value: The encapsulated handle value, which is no longer
// encapsulated by the object.
//
// Remarks:
// Detaches the encapsulated handle and returns it. The returned handle is no
// longer encapsulated by the object.
template <class H, class FNCLOSE>
inline H TCAutoHandle<H, FNCLOSE>::Detach()
{
  H h = m_h;
  m_h = (H)0;
  return h;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the encapsulated handle value.
//
// Return Value: The encapsulated handle value.
//
// Remarks: Returns the encapsulated handle value.
template <class H, class FNCLOSE>
inline H TCAutoHandle<H, FNCLOSE>::GetHandle() const
{
  return m_h;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Compares the encapsulated handle value to NULL.
//
// Return Value: *true* if the encapsulated handle value is equal to NULL,
// otherwise *false*.
//
// Remarks: Compares the encapsulated handle value to NULL.
template <class H, class FNCLOSE>
inline bool TCAutoHandle<H, FNCLOSE>::IsNull() const
{
  return NULL == m_h;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Replacement for address-of operator
//
// Return Value: The real address of the auto handle object.
//
// Remarks: Since the address-of operator is overloaded to return the address
// of the encapsulated handle, these methods can be used when the *real*
// address of the TCAutoHandle object is needed. This is useful when
// initializing an array of TCAutoHandle pointers.
template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>* TCAutoHandle<H, FNCLOSE>::This()
{
  return this;
}

template <class H, class FNCLOSE>
inline const TCAutoHandle<H, FNCLOSE>* TCAutoHandle<H, FNCLOSE>::This() const
{
  return this;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Closes (releases) the encapsulated handle.
//
// Remarks:
// The encapsulated handle is closed using the function call (parenthesis)
// operator of the class specified by the FNCLOSE template parameter.
template <class H, class FNCLOSE>
inline bool TCAutoHandle<H, FNCLOSE>::Close()
{
  if (m_h)
  {
    bool bSuccess = FNCLOSE()(m_h);
    m_h = (H)0;
    return bSuccess;
  }
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Description: Used to typecast the object to the encapsulated handle type.
// Return Value: The value of the encapsulated handle.
template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>::operator H()
{
  return m_h;
}

template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>::operator H() const
{
  return m_h;
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   h - The handle value to be encapsulated.
//
// Remarks: Encapsulates the specified handle.  If another object is
// currently being encapsulated, it will be closed first.
//
// See Also: TCAutoHandle::Close, TCAutoHandle::Attach,
// TCAutoHandle::constructor
template <class H, class FNCLOSE>
inline TCAutoHandle<H, FNCLOSE>& TCAutoHandle<H, FNCLOSE>::operator=(H h)
{
  Close();
  m_h = h;
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Address-of operator.
//
// Return Value: The address of the encapsulated handle.
//
// Remarks: Prior to returning the address of the encapsulated handle, this
// method will close any currently encapsulated handle.
template <class H, class FNCLOSE>
inline H* TCAutoHandle<H, FNCLOSE>::operator &()
{
  Close();
  return &m_h;
}


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// Function Classes and Typed Variations
//
// NOTE: The function classes following are declared with the struct keyword
// just to avoid having to declare the parenthesis operators as public:
//
// Maintain the following table to aid in seeing "at a glance" which handles
// are encapsulated by the TCAutoHandle template class.
//
//  CLASS NAME                  HANDLE TYPE        HANDLE CLOSING FUNCTION
// ---------------------------+-----------------+----------------------------
//  TCHandle                 | HANDLE          | CloseHandle             
//  TCFileFindHandle         | HANDLE          | FindClose               
//  TCChangeNotifyHandle     | HANDLE          | FindCloseChangeNotification
//  TCViewOfFile             | LPVOID          | UnmapViewOfFile         
//  TCRegKeyHandle           | HKEY            | RegCloseKey             
//  TCServiceHandle          | SC_HANDLE       | CloseServiceHandle      
//  TCGDIObjHandle           | HGDIOBJ         | DeleteObject            
//  TCClientDC               | HDC             | ReleaseDC               
//  TCWindowDC               | HDC             | ReleaseDC               
//  TCCreatedDC              | HDC             | DeleteDC                
//  TCRedrawScope            | HWND            | WM_SETREDRAW(TRUE),     
//                            |                 | InvalidateRect(NULL)    
//  TCSysString              | BSTR            | SysFreeString           
//  TCSafeArrayAccess        | SAFEARRAY*      | SafeArrayUnaccessData   
//  TCSafeArrayLock          | SAFEARRAY*      | SafeArrayUnlock         
//  TCDeferWindowPosHandle   | HDWP            | EndDeferWindowPos       
//  TCPtr<T>                 | T*              | (none)
//  TCSinglePtr<T>           | T*              | delete
//  TCArrayPtr<T>            | T*              | delete []
// ---------------------------+-----------------+----------------------------
//
// Because of the following #ifdef's this file should be included *AFTER* any
// system include files.
//


#ifdef _WINBASE_

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCCloseHandle
  {
    bool operator()(HANDLE h) {return ::CloseHandle(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HANDLE type and will
  // close the handle using the Win32 CloseHandle API.
  // 
  // Typical usage of this would be with files, pipes, mailslots, events,
  // mutexes, semaphores, waitable timers, file-mapping objects, etc.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HANDLE, TCCloseHandle> TCHandle;

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCFindClose
  {
    bool operator()(HANDLE h) {return ::FindClose(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HANDLE, and will
  // close the handle using the Win32 FindClose API.
  // 
  // Use this type only with handles returned from the Win32 FindFirstFile
  // API.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HANDLE, TCFindClose> TCFileFindHandle;


  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCFindCloseChangeNotify
  {
    bool operator()(HANDLE h) {return ::FindCloseChangeNotification(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HANDLE, and will
  // close the handle using the Win32 FindCloseChangeNotification API.
  // 
  // Use this type only with handles returned from the Win32
  // FindFirstChangeNotification API.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HANDLE, TCFindCloseChangeNotify>
    TCChangeNotifyHandle;


  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCUnmapViewOfFile
  {
    bool operator()(LPVOID h) {return ::UnmapViewOfFile(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a void pointer that refers to
  // a memory-mapped file, and will release the pointer using the Win32
  // UnmapViewOfFile API.
  // 
  // Use this type only with pointers returned from the Win32 MapViewOfFile
  // or MapViewOfFileEx API's.
  // 
  // Since this type maintains a void*, it may be more usable to create a new
  // type that encapsulates a pointer to the actual data stored in the memory
  // mapped file. For example, if the memory-mapped file stores a structure,
  // CMyData, as follows:
  // 
  //        struct CMyData
  //        {
  //          DWORD cbSize;           // Size of structure
  //          int   nType;            // Type of data
  //          DWORD cbData;           // Size of data
  //          TCHAR szName[MAXNAME];  // Name of data
  //        };
  // 
  // The following typedef would be more convenient tha TCViewOfFile:
  // 
  //        typedef TCAutoHandle<CMyData*, TCUnmapViewOfFile> TCViewOfMyData;
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<LPVOID, TCUnmapViewOfFile> TCViewOfFile;

#endif // _WINBASE_


#ifdef _WINREG_

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCRegCloseKey
  {
    bool operator()(HKEY h) {return ERROR_SUCCESS == ::RegCloseKey(h);}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HKEY type and will
  // close the handle using the Win32 RegCloseKey API.
  // 
  // Use this type with the HKEY handles used in the Win32 registry API's.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HKEY, TCRegCloseKey> TCRegKeyHandle;

#endif // _WINREG_


#ifdef _WINSVC_

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCCloseServiceHandle
  {
    bool operator()(SC_HANDLE h) {return ::CloseServiceHandle(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 SC_HANDLE type and
  // will close the handle using the Win32 CloseServiceHandle API.
  // 
  // Use this type when accessing device drivers or services through the
  // Windows NT Service Control Manager (SCM), as well as the SCM itself.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<SC_HANDLE, TCCloseServiceHandle> TCServiceHandle;

#endif // _WINSVC_


#ifdef _WINGDI_

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCDeleteObject
  {
    bool operator()(HGDIOBJ h) {return ::DeleteObject(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HGDIOBJ type and will
  // close the handle using the Win32 DeleteObject API.
  // 
  // This type can be used for Graphics Device Interface (GDI) objects
  // including logical pens, brushes, fonts, bitmaps, regions, and palettes.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HGDIOBJ, TCDeleteObject> TCGDIObjHandle;

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCReleaseDC
  {
    bool operator()(HDC h)
    {
      ::RestoreDC(h, -1);
      return ::ReleaseDC(::WindowFromDC(h), h) != 0;
    }
  };

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  class TCClientDC : public TCAutoHandle<HDC, TCReleaseDC>
  {
  public:
    TCClientDC(HWND hwnd) :
      TCAutoHandle<HDC, TCReleaseDC>(::GetDC(hwnd))
    {
      ::SaveDC(HDC(*this));
    }
  };

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  class TCWindowDC : public TCAutoHandle<HDC, TCReleaseDC>
  {
  public:
    TCWindowDC(HWND hwnd) :
      TCAutoHandle<HDC, TCReleaseDC>(::GetWindowDC(hwnd))
    {
      ::SaveDC(HDC(*this));
    }
  };

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCSetRedraw
  {
    bool operator()(HWND h)
    {
      ::SendMessage(h, WM_SETREDRAW, TRUE, 0);
      return FALSE != ::InvalidateRect(h, NULL, TRUE);
    }
  };
  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  class TCRedrawScope : public TCAutoHandle<HWND, TCSetRedraw>
  {
  public:
    TCRedrawScope(HWND h) : TCAutoHandle<HWND, TCSetRedraw>(h)
    {
      ::SendMessage(h, WM_SETREDRAW, FALSE, 0);
    }
  };

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCEndDeferWindowPos
  {
    bool operator()(HDWP h) {return ::EndDeferWindowPos(h) != 0;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 HDWP type that is
  // closed using the Win32 EndDeferWindowPos function.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<HDWP, TCEndDeferWindowPos> TCDeferWindowPosHandle;

#endif // _WINGDI_


#if defined( _OLEAUTO_H_ )

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCSysFreeString
  {
    bool operator()(BSTR h) {::SysFreeString(h); return true;}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 COM BSTR type and
  // will release the string using the Win32 SysFreeString API.
  // 
  // Since the Visual C++ 5.0 _bstr_t type does not have an address-of
  // operator which would make it useable as an [out] parameter of many COM
  // object method calls, and since CComBSTR is only available to ATL-based
  // projects, this type could prove itself useful in many COM client
  // (hosting) applications.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<BSTR, TCSysFreeString> TCSysString;

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCSafeArrayUnaccessData
  {
    bool operator()(SAFEARRAY* h) {return SUCCEEDED(SafeArrayUnaccessData(h));}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 SAFEARRAY* that is
  // released using the Win32 SafeArrayUnaccessData function.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<SAFEARRAY*, TCSafeArrayUnaccessData> TCSafeArrayAccess;

  ///////////////////////////////////////////////////////////////////////////
  // {secret}
  struct TCSafeArrayUnlock
  {
    bool operator()(SAFEARRAY* h) {return SUCCEEDED(SafeArrayUnlock(h));}
  };

  ///////////////////////////////////////////////////////////////////////////
  // Declares a TCAutoHandle that encapsulates a Win32 SAFEARRAY* that is
  // closed using the Win32 SafeArrayUnlock function.
  // 
  // See Also: TCAutoHandle
  typedef TCAutoHandle<SAFEARRAY*, TCSafeArrayUnlock> TCSafeArrayLock;


#endif // defined( _OLEAUTO_H_ )


/////////////////////////////////////////////////////////////////////////////
// {secret}
template <class T>
struct TCPtrDelete
{
  bool operator()(T* h) {return true;}
};

/////////////////////////////////////////////////////////////////////////////
// {secret}
template <class T>
struct TCSinglePtrDelete
{
  bool operator()(T* h) {::delete h; return true;}
};

/////////////////////////////////////////////////////////////////////////////
// {secret}
template <class T>
struct TCArrayPtrDelete
{
  bool operator()(T* h) {::delete [] h; return true;}
};

/////////////////////////////////////////////////////////////////////////////
// Declares a TCAutoHandle that encapsulates a pointer to any type /T/ that
// does not get deleted.
//
// See Also: TCAutoHandle, TCSinglePtr, TCArrayPtr
template <class T>
class TCPtr : public TCAutoHandle<T*, TCPtrDelete<T> >
{
protected:
  typedef TCAutoHandle<T*, TCPtrDelete<T> > TCPtrBase;
public:
  TCPtr(T* p);
  TCPtr();
  T* GetPtr() const;
  bool Delete();
  TCPtr& operator=(T* p);
  T* operator->();
};


/////////////////////////////////////////////////////////////////////////////
// Declares a TCAutoHandle that encapsulates a pointer to any type /T/ that
// does gets deleted using the global delete operator. The pointer should
// have been allocated using the singular (non-array) global new operator.
//
// See Also: TCAutoHandle, TCPtr, TCArrayPtr
template <class T>
class TCSinglePtr : public TCAutoHandle<T*, TCSinglePtrDelete<T> >
{
protected:
  typedef TCAutoHandle<T*, TCSinglePtrDelete<T> > TCSinglePtrBase;
public:
  TCSinglePtr(T* p);
  TCSinglePtr();
  T* GetPtr() const;
  bool Delete();
  TCSinglePtr& operator=(T* p);
  T* operator->();
};

/////////////////////////////////////////////////////////////////////////////
// Declares a TCAutoHandle that encapsulates a pointer to any type /T/ that
// does gets deleted using the global delete [] operator. The pointer should
// have been allocated using the (array) global new [] operator.
//
// See Also: TCAutoHandle, TCPtr, TCSinglePtr
template <class T>
class TCArrayPtr : public TCAutoHandle<T*, TCArrayPtrDelete<T> >
{
protected:
  typedef TCAutoHandle<T*, TCArrayPtrDelete<T> > TCArrayPtrBase;
public:
  TCArrayPtr(T* p);
  TCArrayPtr();
  T* GetPtr() const;
  bool Delete();
  TCArrayPtr& operator=(T* p);
  T* operator->();
};



/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// See Also: TCAutoHandle, TCAutoHandle::constructor
template <class T>
inline TCPtr<T>::TCPtr(T* p) : TCPtrBase(p)
{
}

template <class T>
inline TCPtr<T>::TCPtr()
{
}

/////////////////////////////////////////////////////////////////////////////
// {partof:TCPtr::constructor}
template <class T>
inline TCSinglePtr<T>::TCSinglePtr(T* p) : TCSinglePtrBase(p)
{
}

template <class T>
inline TCSinglePtr<T>::TCSinglePtr()
{
}

/////////////////////////////////////////////////////////////////////////////
// {partof:TCPtr::constructor}
template <class T>
inline TCArrayPtr<T>::TCArrayPtr(T* p) : TCArrayPtrBase(p)
{
}

template <class T>
inline TCArrayPtr<T>::TCArrayPtr()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// This is really just an alias for the TCAutoHandle::GetHandle base class
// method.
//
// See Also: TCAutoHandle, TCAutoHandle::GetHandle
template <class T>
inline T* TCPtr<T>::GetPtr() const
{
  return GetHandle();
}

/////////////////////////////////////////////////////////////////////////////
// {partof:TCPtr::GetPtr}
template <class T>
inline T* TCSinglePtr<T>::GetPtr() const
{
  return GetHandle();
}

/////////////////////////////////////////////////////////////////////////////
// {partof:TCPtr::GetPtr}
template <class T>
inline T* TCArrayPtr<T>::GetPtr() const
{
  return GetHandle();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// This is really just an alias for the TCAutoHandle::Close base class
// method.
//
// See Also: TCAutoHandle, TCAutoHandle::Close
template <class T>
inline bool TCPtr<T>::Delete()
{
  return Close();
}

/////////////////////////////////////////////////////////////////////////////
// This is really just an alias for the TCAutoHandle::Close base class
// method.
//
// See Also: TCAutoHandle, TCAutoHandle::Close
template <class T>
inline bool TCSinglePtr<T>::Delete()
{
  return Close();
}

/////////////////////////////////////////////////////////////////////////////
// This is really just an alias for the TCAutoHandle::Close base class
// method.
//
// See Also: TCAutoHandle, TCAutoHandle::Close
template <class T>
inline bool TCArrayPtr<T>::Delete()
{
  return Close();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// See Also: TCAutoHandle, TCAutoHandle::operator=
template <class T>
inline TCPtr<T>& TCPtr<T>::operator=(T* p)
{
  TCAutoHandle<T*, TCPtrDelete<T> >::operator=(p);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Allows the encapsulated pointer to be dereferenced when used
// to access a member.
// Return Value: The value of the encapsulated pointer.
template <class T>
inline T* TCPtr<T>::operator->()
{
  return GetPtr();
}

/////////////////////////////////////////////////////////////////////////////
// See Also: TCAutoHandle, TCAutoHandle::operator=
template <class T>
inline TCSinglePtr<T>& TCSinglePtr<T>::operator=(T* p)
{
  TCAutoHandle<T*, TCSinglePtrDelete<T> >::operator=(p);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Allows the encapsulated pointer to be dereferenced when used
// to access a member.
// Return Value: The value of the encapsulated pointer.
template <class T>
inline T* TCSinglePtr<T>::operator->()
{
  return GetPtr();
}

/////////////////////////////////////////////////////////////////////////////
// See Also: TCAutoHandle, TCAutoHandle::operator=
template <class T>
inline TCArrayPtr<T>& TCArrayPtr<T>::operator=(T* p)
{
  TCAutoHandle<T*, TCArrayPtrDelete<T> >::operator=(p);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Allows the encapsulated pointer to be dereferenced when used
// to access a member.
// Return Value: The value of the encapsulated pointer.
template <class T>
inline T* TCArrayPtr<T>::operator->()
{
  return GetPtr();
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__AutoHandle_h__

