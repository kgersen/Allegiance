#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Sound Initialization
//
//////////////////////////////////////////////////////////////////////////////

namespace SoundInit {
    void InitializeSoundTemplates(
        Modeler* pmodeler, 
        TVector<TRef<ISoundTemplate> >& vSoundMap
    ) {
        DWORD dwStartTime = timeGetTime();

        // load the training sound definitions
        TRef<ZFile>         pFile = pmodeler->LoadFile ("trainingsounddef", "mdl", false);
        TRef<INameSpace>    pnsTrainingSoundDef;
        if (pFile == NULL)
        {
            // here we substitute a namespace that we will use
            pnsTrainingSoundDef = pmodeler->CreateNameSpace("trainingsounddef", pmodeler->GetNameSpace("notrainingsounddef"));
        }

        // load the general-purpose sound templates
        TRef<INameSpace>  pnsSoundDefs = pmodeler->GetNameSpace("sounddef");
        TRef<IObjectList> plist; CastTo(plist, pnsSoundDefs->FindMember("soundList"));
        TLookup<ISoundTemplate>::Parse(plist, 0, vSoundMap);

        // unload the training sound namespace
        pmodeler->UnloadNameSpace ("trainingsounddef");

        // make sure all of the IGC sounds were defined
        #define DEFSOUND(id) ZAssert(vSoundMap[id##Sound] != NULL);
        #include "sounds.h"
        #undef DEFSOUND

        debugf("Time reading sounds: %d ms\n", timeGetTime() - dwStartTime);
    }

    void AddMembers(INameSpace* pns) 
    {
        #define DEFSOUND(id) pns->AddMember(#id "SoundId", new Number((float)id##Sound));
        #include "sounds.h"
        #undef DEFSOUND
    }
}


