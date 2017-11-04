#ifndef _VIDEOSETTINGS_H_
#define _VIDEOSETTINGS_H_

#include <windows.h>
#include <zstring.h>

//Imago added bool bStartFullscreen, bool bRaise 6/29/09, only prompts when "Safe Mode" and restored the -windowed command line switch
bool PromptUserForVideoSettings(bool bStartFullscreen, bool bRaise, int iAdapter, HINSTANCE hInstance, PathString & szArtPath, ZString lpSubKey );

#endif // _VIDEOSETTINGS_H_
