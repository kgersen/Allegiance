//
// a scratch file for testing the sound engine
//

#include "pch.h"
#include "soundengine.h"
#include <vector>

void main()
{
    TRef<ISoundEngine> pengine;
    TRef<ISoundPCMData> pdata;
    TRef<ISoundInstance> psound;

    // create a sound engine (sort of equivalent to the DirectSound object)
    ZFailed(CreateSoundEngine(pengine, GetForegroundWindow()));

    std::vector<ZAdapt<TRef<ISoundInstance> > > vectSounds;

    pengine->EnableHardware(true);
    ZFailed(LoadWaveFile(pdata, "\\\\kendrics4\\fed\\objs\\artwork\\ef10.wav"));
    while (1)
    {
        switch (rand() % 2)
        {
        case 0:
            //ZFailed(pengine->GetBufferSource()->CreateStaticBuffer(psound, pdata, true));
            ZFailed(pengine->GetBufferSource()->CreateASRBuffer(psound, pdata, 24, 16000));
            vectSounds.push_back(psound);
            psound = NULL;
            break;

        case 1:
            if (!vectSounds.empty())
            {
                std::vector<ZAdapt<TRef<ISoundInstance> > >::iterator iterStop;
                iterStop = vectSounds.begin() + rand() * vectSounds.size()/RAND_MAX;
                (*iterStop)->Stop();
                vectSounds.erase(iterStop);
            }
            break;
        }

        pengine->Update();

        static int i;
        if (i++ % 20)
        {
            printf("%d\n", vectSounds.size());
        }
    }
/*
    // create a new virtual sound buffer from a wave file.  Note that the DSound buffer
    // itself will not be created until update is called below.  
    ZFailed(LoadWaveFile(pdata, "\\\\kendrics4\\fed\\objs\\artwork\\ef10.wav"));
    ZFailed(pengine->GetBufferSource()->CreateStaticBuffer(psound, pdata, true));

    // halve the pitch of the wave file
    psound->GetISoundTweakable()->SetPitch(0.5);

    // Choose which virtual sound buffers to play, and start the appropriate ones.  
    ZFailed(pengine->Update());

    // let them play just to verify that is works on most machines
    Sleep(2000);
*/
}
