/////////////////////////////////////////////////////////////////////////////
// TCNullStream.cpp | Implementation of the CTCNullStream class, which
// implements the CLSID_TCNullStream component object.<nl><nl>
// This object provides an implementation of the IStream interface that does
// nothing more that count the number of bytes being written to it.  Since
// ATL (currently 3.0) implements IPersistStreamInitImpl::GetSizeMax by
// returning E_NOTIMPL, this object can be used in an override of that
// method. This is accomplished by creating an instance of this null stream
// and 'persisting' an object to it, thereby counting the number of bytes
// needed to persist the object without allocating any memory.
//

#include "pch.h"
#include "TCNullStream.h"


/////////////////////////////////////////////////////////////////////////////
// CTCNullStream

TC_OBJECT_EXTERN_IMPL(CTCNullStream)


