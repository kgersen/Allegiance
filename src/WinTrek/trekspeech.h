/*==========================================================================
 *
 *  Copyright (C) 1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:	TrekSpeech.h
 *
 *  Purpose:
 *		Class for handling text to speech.  Requires an externally	initialized
 *		DirectSound object, and the presence of a text to speech engine.
 *
 *  Author: Terri Watson
 *
 *==========================================================================*/
#ifndef __TrekSpeech__
#define __TrekSpeech__

#include <dsound.h>
#include "speech.h"

class TrekSpeech {
private:
	boolean				m_HaveSpeech;		// No problems w/ speech set-up
	PIAUDIODIRECT		m_pIAD;				// AudioDirect interface (an audio
											// destination for use with DirectSound)
public:
	PITTSCENTRAL		m_pITTSCentral;		// TTSCentral interface
	TTSMODEINFO			m_TTSModeInfo;		// Desired engine characteristics
	
	TrekSpeech();
	~TrekSpeech();

	//===============
	// Set-up methods
	//===============
	HRESULT InitAudioDest(LPDIRECTSOUND lpDS);	// required DirectSound obj
	HRESULT InitAudioDestination(LPDIRECTSOUND lpDS);	// required DirectSound obj
	void desiredGender(WORD wGender);			// optional gender request
	HRESULT SelectTTSEngine();		// optional
	// SayText will call SelectTTSEngine() if needed, but it may save delay
	// if the setup code calls this.  If non-default characteristics are
	// desired, set them up PRIOR to calling SelectTTSEngine() or SayText().

	//======
	// Speak
	HRESULT SayText(char *text);
};

// Global until I find a home for the speech object
extern TrekSpeech g_TrekSpeech;

#define TRYSAYTEXT(text)	g_TrekSpeech.SayText(text)

#endif // __TrekSpeech__
