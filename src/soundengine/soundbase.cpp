#include "soundbase.h"

//
// These includes are the real heart of this file.  They were all originally
// part of this file, but I spun them off into .h files when they became too
// big to be managable.
//

#include "ds3dutil.h"
#include "ds3dbuffer.h"
#include "ds3dvirtualbuffer.h"
#include "ds3dengine.h"


namespace SoundEngine {


// create a default sound engine
HRESULT CreateSoundEngine(TRef<ISoundEngine>& psoundengine, HWND hwnd, bool bUseDSound8)
{
    TRef<DS3DSoundEngine> psoundengineimpl = new DS3DSoundEngine();

    HRESULT hr = psoundengineimpl->Init(hwnd, bUseDSound8);

    if (hr == DSERR_NODRIVER || hr == DSERR_ALLOCATED || ZFailed(hr))
        return hr;

    psoundengine = psoundengineimpl;

    return S_OK;
}


#ifdef _DEBUG
// a helper function for trying to dump an arbitrary object
ZString SoundDebugDump(ISoundObject* pobject, const ZString& strIndent)
{
    ISoundDebugDump* pdebug = dynamic_cast<ISoundDebugDump*>(pobject);

    if (pdebug)
    {
        return pdebug->DebugDump(strIndent);
    }
    else
    {
        return strIndent + "<unknown>\n";
    }
}
#endif

};
