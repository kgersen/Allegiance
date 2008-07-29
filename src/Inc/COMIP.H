/***
* comip.h - Native C++ compiler COM support - COM interface pointers header
*
*	Copyright (C) 1996-1997 Microsoft Corporation
*	All rights reserved.
*
****/

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(_INC_COMIP)
#define _INC_COMIP

#include <ole2.h>
#include <malloc.h>

#include <comutil.h>

#pragma warning(push)
#pragma warning(disable: 4290)

class _com_error;

void __stdcall _com_issue_error(HRESULT);
struct __declspec(uuid("00000000-0000-0000-c000-000000000046")) IUnknown;

// Provide Interface to IID association
//
template<typename _Interface, const IID* _IID /*= &__uuidof(_Interface)*/> class _com_IIID {
public:
	typedef _Interface Interface;

	static _Interface* GetInterfacePtr() ALLEGIANCE_THROW0()
	{
		return NULL;
	}

	static _Interface& GetInterface() ALLEGIANCE_THROW0()
	{
		return *GetInterfacePtr();
	}

	static const IID& GetIID() ALLEGIANCE_THROW0()
	{
		return *_IID;
	}
};

template<typename _IIID> class _com_ptr_t {
public:
	// Declare interface type so that the type may be available outside
	// the scope of this template.
	//
	typedef _IIID ThisIIID;
	typedef typename _IIID::Interface Interface;

	// When the compiler supports references in template parameters,
	// _CLSID will be changed to a reference.  To avoid conversion
	// difficulties this function should be used to obtain the
	// CLSID.
	//
	static const IID& GetIID() ALLEGIANCE_THROW0()
	{ 
		return ThisIIID::GetIID(); 
	}

	// Constructs a smart-pointer from any interface pointer.
	//
	template<typename _InterfacePtr> _com_ptr_t(const _InterfacePtr& p) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		if (p) {
			HRESULT hr = _QueryInterface(p);

			if (FAILED(hr) && (hr != E_NOINTERFACE)) {
				_com_issue_error(hr);
			}
		}
	}

	// Disable conversion using _com_ptr_t* specialization of
	// template<typename _InterfacePtr> _com_ptr_t(const _InterfacePtr& p)
	template<> explicit _com_ptr_t(_com_ptr_t* const & p) ALLEGIANCE_THROW(_com_error)
	{
		if (p != NULL) {
			_com_issue_error(E_POINTER);
		}
		else {
			m_pInterface = p->m_pInterface;
			AddRef();
		}
	}

	// Default constructor.
	//
	_com_ptr_t() ALLEGIANCE_THROW0()
		: m_pInterface(NULL)
	{
	}

	// This constructor is provided to allow NULL assignment. It will issue
	// an error if any value other than null is assigned to the object.
	//
	_com_ptr_t(int null) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		if (null != 0) {
			_com_issue_error(E_POINTER);
		}
	}

	// Copy the pointer and AddRef().
	//

	// mdvalley: Stupid changes to VC 2005.
//	template<> _com_ptr_t(const _com_ptr_t& cp) ALLEGIANCE_THROW0()
	_com_ptr_t(const _com_ptr_t& cp) ALLEGIANCE_THROW0()
		: m_pInterface(cp.m_pInterface)
	{ 
		_AddRef(); 
	}

	// Saves the interface.
	//
	_com_ptr_t(Interface* pInterface) ALLEGIANCE_THROW0()
		: m_pInterface(pInterface)
	{ 
		_AddRef(); 
	}

	// Copies the pointer. If fAddRef is TRUE, the interface will
	// be AddRef()ed.
	//
	_com_ptr_t(Interface* pInterface, bool fAddRef) ALLEGIANCE_THROW0()
		: m_pInterface(pInterface)
	{
		if (fAddRef) {
			_AddRef();
		}
	}

	// Construct a pointer for a _variant_t object.
	//
	template<> _com_ptr_t(const _variant_t& varSrc) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		HRESULT hr = QueryStdInterfaces(varSrc);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}
	}

	// Calls CoCreateClass with the provided CLSID.
	//
	explicit _com_ptr_t(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		HRESULT hr = CreateInstance(clsid, pOuter, dwClsContext);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}
	}

	// Calls CoCreateClass with the provided CLSID retrieved from
	// the string.
	//
	explicit _com_ptr_t(LPOLESTR str, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		HRESULT hr = CreateInstance(str, pOuter, dwClsContext);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}
	}

	// Calls CoCreateClass with the provided SBCS CLSID retrieved from
	// the string.
	//
	explicit _com_ptr_t(LPCSTR str, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW(_com_error)
		: m_pInterface(NULL)
	{
		HRESULT hr = CreateInstance(str, pOuter, dwClsContext);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}
	}

	// Queries for interface.
	//
	template<typename _InterfacePtr> _com_ptr_t& operator=(const _InterfacePtr& p) ALLEGIANCE_THROW(_com_error)
	{
		HRESULT hr = _QueryInterface(p);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}

		return *this;
	}

	// Saves the interface.
	//
	_com_ptr_t& operator=(Interface* pInterface) ALLEGIANCE_THROW0()
	{
		if (m_pInterface != pInterface) {
			Interface* pOldInterface = m_pInterface;

			m_pInterface = pInterface;

			_AddRef();

			if (pOldInterface != NULL) {
				pOldInterface->Release();
			}
		}

		return *this;
	}

	// Copies and AddRef()'s the interface.
	//

	// mdvalley: Another stupid VC2005 incompatibility.
//	template<> _com_ptr_t& operator=(const _com_ptr_t& cp) ALLEGIANCE_THROW0()
	_com_ptr_t& operator=(const _com_ptr_t& cp) ALLEGIANCE_THROW0()
	{ 
		return operator=(cp.m_pInterface); 
	}

	// This operator is provided to permit the assignment of NULL to the class.
	// It will issue an error if any value other than NULL is assigned to it.
	//
	_com_ptr_t& operator=(int null) ALLEGIANCE_THROW(_com_error)
	{
		if (null != 0) {
			_com_issue_error(E_POINTER);
		}

		return operator=(reinterpret_cast<Interface*>(NULL));
	}

	// Construct a pointer for a _variant_t object.
	//
	template<> _com_ptr_t& operator=(const _variant_t& varSrc) ALLEGIANCE_THROW(_com_error)
	{
		HRESULT hr = QueryStdInterfaces(varSrc);

		if (FAILED(hr) && (hr != E_NOINTERFACE)) {
			_com_issue_error(hr);
		}

		return *this;
	}

	// If we still have an interface then Release() it. The interface
	// may be NULL if Detach() has previously been called, or if it was
	// never set.
	//
	~_com_ptr_t() ALLEGIANCE_THROW0()
	{ 
		_Release(); 
	}

	// Saves/sets the interface without AddRef()ing. This call
	// will release any previously acquired interface.
	//
	void Attach(Interface* pInterface) ALLEGIANCE_THROW0()
	{
		_Release();
		m_pInterface = pInterface;
	}

	// Saves/sets the interface only AddRef()ing if fAddRef is TRUE.
	// This call will release any previously acquired interface.
	//
	void Attach(Interface* pInterface, bool fAddRef) ALLEGIANCE_THROW0()
	{
		_Release();
		m_pInterface = pInterface;

		if (fAddRef) {
			if (pInterface != NULL) {
				pInterface->AddRef();
			}
		}
	}

	// Simply NULL the interface pointer so that it isn't Released()'ed.
	//
	Interface* Detach() ALLEGIANCE_THROW0()
	{
		Interface* const old=m_pInterface;
		m_pInterface = NULL;
		return old;
	}

	// Return the interface. This value may be NULL.
	//
	operator Interface*() const ALLEGIANCE_THROW0()
	{ 
		return m_pInterface; 
	}

	// Queries for the unknown and return it
	// Provides minimal level error checking before use.
	//
	operator Interface&() const ALLEGIANCE_THROW(_com_error)
	{ 
		if (m_pInterface == NULL) {
			_com_issue_error(E_POINTER);
		}

		return *m_pInterface; 
	}

	// Allows an instance of this class to act as though it were the
	// actual interface. Also provides minimal error checking.
	//
	Interface& operator*() const ALLEGIANCE_THROW(_com_error)
	{ 
		if (m_pInterface == NULL) {
			_com_issue_error(E_POINTER);
		}

		return *m_pInterface; 
	}

	// Returns the address of the interface pointer contained in this
	// class. This is useful when using the COM/OLE interfaces to create
	// this interface.
	//
	Interface** operator&() ALLEGIANCE_THROW0()
	{
		_Release();
		m_pInterface = NULL;
		return &m_pInterface;
	}

	// Allows this class to be used as the interface itself.
	// Also provides simple error checking.
	//
	Interface* operator->() const ALLEGIANCE_THROW(_com_error)
	{ 
		if (m_pInterface == NULL) {
			_com_issue_error(E_POINTER);
		}

		return m_pInterface; 
	}

	// This operator is provided so that simple boolean expressions will
	// work.  For example: "if (p) ...".
	// Returns TRUE if the pointer is not NULL.
	//
	operator bool() const ALLEGIANCE_THROW0()
	{ 
		return m_pInterface != NULL; 
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator==(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return _CompareUnknown(p) == 0;
	}

	// Compare with other interface
	//
	template<> bool operator==(Interface* p) ALLEGIANCE_THROW(_com_error)
	{
		return (m_pInterface == p) ? true : _CompareUnknown(p) == 0;
	}

	// Compares 2 _com_ptr_t's
	//
	template<> bool operator==(_com_ptr_t& p) ALLEGIANCE_THROW0()
	{
		return operator==(p.m_pInterface);
	}

	// For comparison to NULL
	//
	template<> bool operator==(int null) ALLEGIANCE_THROW(_com_error)
	{
		if (null != 0) {
			_com_issue_error(E_POINTER);
		}

		return m_pInterface == NULL;
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator!=(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return !(operator==(p));
	}

	// Compare with other interface
	//
	template<> bool operator!=(Interface* p) ALLEGIANCE_THROW(_com_error)
	{
		return !(operator==(p));
	}

	// Compares 2 _com_ptr_t's
	//
	template<> bool operator!=(_com_ptr_t& p) ALLEGIANCE_THROW(_com_error)
	{
		return !(operator==(p));
	}

	// For comparison to NULL
	//
	template<> bool operator!=(int null) ALLEGIANCE_THROW(_com_error)
	{
		return !(operator==(null));
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator<(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return _CompareUnknown(p) < 0;
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator>(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return _CompareUnknown(p) > 0;
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator<=(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return _CompareUnknown(p) <= 0;
	}

	// Compare two pointers
	//
	template<typename _InterfacePtr> bool operator>=(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		return _CompareUnknown(p) >= 0;
	}

	// Provides error-checking Release()ing of this interface.
	//
	void Release() ALLEGIANCE_THROW(_com_error)
	{
		if (m_pInterface == NULL) {
			_com_issue_error(E_POINTER);
		}

		m_pInterface->Release();
		m_pInterface = NULL;
	}

	// Provides error-checking AddRef()ing of this interface.
	//
	void AddRef() ALLEGIANCE_THROW(_com_error)
	{ 
		if (m_pInterface == NULL) {
			_com_issue_error(E_POINTER);
		}

		m_pInterface->AddRef(); 
	}

	// Another way to get the interface pointer without casting.
	//
	Interface* GetInterfacePtr() const ALLEGIANCE_THROW0()
	{ 
		return m_pInterface; 
	}

	// Loads an interface for the provided CLSID.
	// Returns an HRESULT.  Any previous interface is released.
	//
	HRESULT CreateInstance(const CLSID& rclsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW0()
	{
		HRESULT hr;

		_Release();

		if (dwClsContext & (CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER)) {
			IUnknown* pIUnknown;

			hr = CoCreateInstance(rclsid, pOuter, dwClsContext, __uuidof(IUnknown), reinterpret_cast<void**>(&pIUnknown));

			if (FAILED(hr)) {
				return hr;
			}

			hr = OleRun(pIUnknown);

			if (SUCCEEDED(hr)) {
				hr = pIUnknown->QueryInterface(GetIID(), reinterpret_cast<void**>(&m_pInterface));
			}

			pIUnknown->Release();
		}
		else {
			hr = CoCreateInstance(rclsid, pOuter, dwClsContext, GetIID(), reinterpret_cast<void**>(&m_pInterface));
		}

		return hr;
	}

	// Creates the class specified by clsidString.  clsidString may
	// contain a class id, or a prog id string.
	//
	HRESULT CreateInstance(LPOLESTR clsidString, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW0()
	{
		if (clsidString == NULL) {
			return E_INVALIDARG;
		}

		CLSID clsid;
		HRESULT hr;

		if (clsidString[0] == '{') {
			hr = CLSIDFromString(clsidString, &clsid);
		}
		else {
			hr = CLSIDFromProgID(clsidString, &clsid);
		}

		if (FAILED(hr)) {
			return hr;
		}

		return CreateInstance(clsid, pOuter, dwClsContext);
	}

	// Creates the class specified by SBCS clsidString.  clsidString may
	// contain a class id, or a prog id string.
	//
	HRESULT CreateInstance(LPCSTR clsidStringA, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) ALLEGIANCE_THROW0()
	{
		if (clsidStringA == NULL) {
			return E_INVALIDARG;
		}

		int size = lstrlenA(clsidStringA) + 1;
		LPOLESTR clsidStringW = static_cast<LPOLESTR>(_alloca(size * 2));

		clsidStringW[0] = '\0';

		if (MultiByteToWideChar(CP_ACP, 0, clsidStringA, -1, clsidStringW, size) == 0) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		return CreateInstance(clsidStringW, pOuter, dwClsContext);
	}

	// Attach to the active object specified by rclsid.
	// Any previous interface is released.
	//
	HRESULT GetActiveObject(const CLSID& rclsid) ALLEGIANCE_THROW0()
	{
		_Release();

		IUnknown* pIUnknown;

		HRESULT hr = ::GetActiveObject(rclsid, NULL, &pIUnknown);

		if (FAILED(hr)) {
			return hr;
		}

		hr = pIUnknown->QueryInterface(GetIID(), reinterpret_cast<void**>(&m_pInterface));

		if (FAILED(hr)) {
			return hr;
		}

		pIUnknown->Release();

		return hr;
	}

	// Attach to the active object specified by clsidString.
	// First convert the LPOLESTR to a CLSID.
	//
	HRESULT GetActiveObject(LPOLESTR clsidString) ALLEGIANCE_THROW0()
	{
		if (clsidString == NULL) {
			return E_INVALIDARG;
		}

		CLSID clsid;
		HRESULT hr;

		if (clsidString[0] == '{') {
			hr = CLSIDFromString(clsidString, &clsid);
		}
		else {
			hr = CLSIDFromProgID(clsidString, &clsid);
		}

		if (FAILED(hr)) {
			return hr;
		}

		return GetActiveObject(clsid);
	}

	// Attach to the active object specified by clsidStringA.
	// First convert the LPCSTR to a LPOLESTR.
	//
	HRESULT GetActiveObject(LPCSTR clsidStringA) ALLEGIANCE_THROW0()
	{
		if (clsidStringA == NULL) {
			return E_INVALIDARG;
		}

		int size = lstrlenA(clsidStringA) + 1;
		LPOLESTR clsidStringW = static_cast<LPOLESTR>(_alloca(size * 2));

		clsidStringW[0] = '\0';

		if (MultiByteToWideChar(CP_ACP, 0, clsidStringA, -1, clsidStringW, size) == 0) {
			return HRESULT_FROM_WIN32(GetLastError());
		}

		return GetActiveObject(clsidStringW);
	}

	// Performs the QI for the specified IID and returns it in p.
	// As with all QIs, the interface will be AddRef'd.
	//
	template<typename _InterfaceType> HRESULT QueryInterface(const IID& iid, _InterfaceType*& p) ALLEGIANCE_THROW0()
	{
		if (m_pInterface != NULL) {
			return m_pInterface->QueryInterface(iid, reinterpret_cast<void**>(&p));
		}

		return E_POINTER;
	}

	// Performs the QI for the specified IID and returns it in p.
	// As with all QIs, the interface will be AddRef'd.
	//
	template<typename _InterfaceType> HRESULT QueryInterface(const IID& iid, _InterfaceType** p) ALLEGIANCE_THROW0()
	{
		return QueryInterface(iid, *p);
	}

private:
	// The Interface.
	//
	Interface* m_pInterface;

	// Releases only if the interface is not null.
	// The interface is not set to NULL.
	//
	void _Release() ALLEGIANCE_THROW0()
	{
		if (m_pInterface != NULL) {
			m_pInterface->Release();
		}
	}

	// AddRefs only if the interface is not NULL
	//
	void _AddRef() ALLEGIANCE_THROW0()
	{
		if (m_pInterface != NULL) {
			m_pInterface->AddRef();
		}
	}

	// Performs a QI on pUnknown for the interface type returned
	// for this class.  The interface is stored.  If pUnknown is
	// NULL, or the QI fails, E_NOINTERFACE is returned and
	// _pInterface is set to NULL.
	//
	template<typename _InterfacePtr> HRESULT _QueryInterface(const _InterfacePtr& p) ALLEGIANCE_THROW0()
	{
		HRESULT hr;

		// Can't QI NULL
		//
		if (p) {
			// Query for this interface
			//
			Interface* pInterface;
			hr = p->QueryInterface(GetIID(), reinterpret_cast<void**>(&pInterface));

			if (FAILED(hr)) {
				// If failed initialize interface to NULL and return HRESULT.
				//
				Attach(NULL);
				return hr;
			}

			// Save the interface without AddRef()ing.
			//
			Attach(pInterface);
		}
		else {
			operator=(static_cast<Interface*>(NULL));
			hr = E_NOINTERFACE;
		}

		return hr;
	}

	// Compares the provided pointer with this by obtaining IUnknown interfaces
	// for each pointer and then returning the difference.
	//
	template<typename _InterfacePtr> int _CompareUnknown(_InterfacePtr p) ALLEGIANCE_THROW(_com_error)
	{
		IUnknown* pu1, *pu2;

		if (m_pInterface != NULL) {
			HRESULT hr = m_pInterface->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&pu1));

			if (FAILED(hr)) {
				_com_issue_error(hr);
			}

			pu1->Release();
		}
		else {
			pu1 = NULL;
		}

		if (p) {
			HRESULT hr = p->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&pu2));

			if (FAILED(hr)) {
				_com_issue_error(hr);
			}

			pu2->Release();
		}
		else {
			pu2 = NULL;
		}

		return pu1 - pu2;
	}

	// Try to extract either IDispatch* or an IUnknown* from
	// the VARIANT
	//
	HRESULT QueryStdInterfaces(const _variant_t& varSrc) ALLEGIANCE_THROW0()
	{
		if (V_VT(&varSrc) == VT_DISPATCH) {
			return _QueryInterface(V_DISPATCH(&varSrc));
		}

		if (V_VT(&varSrc) == VT_UNKNOWN) {
			return _QueryInterface(V_UNKNOWN(&varSrc));
		}

		// We have something other than an IUnknown or an IDispatch.
		// Can we convert it to either one of these?
		// Try IDispatch first
		//
		VARIANT varDest;
		VariantInit(&varDest);

		HRESULT hr = VariantChangeType(&varDest, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc)), 0, VT_DISPATCH);
		if (SUCCEEDED(hr)) {
			hr = _QueryInterface(V_DISPATCH(&varSrc));
		}

		if (FAILED(hr) && (hr == E_NOINTERFACE)) {
			// That failed ... so try IUnknown
			//
			VariantInit(&varDest);
			hr = VariantChangeType(&varDest, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc)), 0, VT_UNKNOWN);
			if (SUCCEEDED(hr)) {
				hr = _QueryInterface(V_UNKNOWN(&varSrc));
			}
		}

		VariantClear(&varDest);
		return hr;
	}
};

// Reverse comparison operators for _com_ptr_t
//
template<typename _InterfaceType> bool operator==(int null, _com_ptr_t<_InterfaceType>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p == NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator==(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p == i;
}

template<typename _Interface> bool operator!=(int null, _com_ptr_t<_Interface>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p != NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator!=(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p != i;
}

template<typename _Interface> bool operator<(int null, _com_ptr_t<_Interface>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p > NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator<(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p > i;
}

template<typename _Interface> bool operator>(int null, _com_ptr_t<_Interface>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p < NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator>(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p < i;
}

template<typename _Interface> bool operator<=(int null, _com_ptr_t<_Interface>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p >= NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator<=(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p >= i;
}

template<typename _Interface> bool operator>=(int null, _com_ptr_t<_Interface>& p) ALLEGIANCE_THROW(_com_error)
{
	if (null != 0) {
		_com_issue_error(E_POINTER);
	}

	return p <= NULL;
}

template<typename _Interface, typename _InterfacePtr> bool operator>=(_Interface* i, _com_ptr_t<_InterfacePtr>& p) ALLEGIANCE_THROW(_com_error)
{
	return p <= i;
}

#pragma warning(pop)

#endif // _INC_COMIP
