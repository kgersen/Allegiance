#ifndef	SOUND_H
#define	SOUND_H

//////////////////////////////////////////////////////////////////////////////
//
//  Types
//
//////////////////////////////////////////////////////////////////////////////

typedef short AmbientSoundGroupID;


//////////////////////////////////////////////////////////////////////////////
//
//  SoundSource
//
//////////////////////////////////////////////////////////////////////////////

class   SoundSource : public IObject
{
public:
    virtual Vector GetPosition();
    virtual Vector GetVelocity();
};


//////////////////////////////////////////////////////////////////////////////
//
//  Sound
//
//////////////////////////////////////////////////////////////////////////////

class   Sound
{
    public:
        Sound(){}

        static void     Initialize(HWND hWnd);
        static void     Terminate(void);
        static bool     GetPlaySounds(void);
        static void     SetPlaySounds(bool    newVal);
        static void     DefineDiscreteSoundEffect(SoundID      id,
                                            const char*  szWave,
                                            bool         fSingleInstance = true,
                                            bool b3D = false);
        static void     DefineLoopingSoundEffect(SoundID       id,
                                            const char*   szWave, bool b3D = false);
        static void     DefineAmbientSoundEffect(	SoundID    idAmbient, 
                                            SoundID    idLooping);
        static void     AddAmbientSoundBit(	SoundID     idAmbient, 
        									SoundID     idDescrete,
        									short		nPlayOdds,
        									float		fMinDelay,
        									float		fMaxDelay);

        static void     UpdateAmbientSounds(Time time);
        static void     PlaySoundEffect(SoundID   id);
        static void     PlaySoundEffect(SoundID   id,  const Vector& pos, const Vector& vel);
        static void     StopSoundEffect(SoundID   id);
        static void     UpdateListener(const Vector& pos, const Vector& vel, const Orientation& orient);
};



#endif
