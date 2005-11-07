#ifndef	JOYSTICK_H
#define	JOYSTICK_H

#include <mmsystem.h>
#include <dinput.h>

#define MOUSE_BUTTON_1  1
#define MOUSE_BUTTON_2  2

class Joystick
{

private:
    
    static LPDIRECTINPUTDEVICE     s_pDevice;
    static LPDIRECTINPUTDEVICE2    s_pDevice2;
    static LPDIRECTINPUT           s_pDirectInput;
    static LPDIRECTINPUTEFFECT     s_pEffectBounce;
    static LPDIRECTINPUTEFFECT     s_pEffectFire;
    static LPDIRECTINPUTEFFECT     s_pEffectExplode;
    static bool                    s_fSupportForceFeedback;

public:

    static bool Initialize(HINSTANCE hInst, HWND hWnd);
    static void Terminate();
    static bool CreateEffects();
    static bool AcquireDevice();
    static bool PrepareDevice();
    static bool PlayFFEffect(EffectID effectID, LONG lDirection = 0);
    static bool GetJoystick(DIJOYSTATE*  pdijs);
    
    static BOOL CALLBACK EnumDeviceProc(LPDIDEVICEINSTANCE pdidi, LPVOID pv);
    static BOOL CALLBACK EnumEffectTypeProc(LPCDIEFFECTINFO pei, LPVOID pv);

    static void Flush(void);   // remove input (clean up...)

    static int QKeyboard(void);    // 0= no keyboard, 1= exists
    static int QMouse(void);       // 0=no, otherwise num buttons on mouse
    static int QJoystick(void);    // 0=none, else number of joysticks

    static int GetKeyboard(int yourkey);   // ret: 0=no key available
    //  pass number of keys to check, then array of VKEY codes for keys 
    //  to check.  If any key is *not* depressed, returns 0; else 1
    static int QKeyDepressed(int numkeys, int *keyarray);
    //  buttons is bitmap of MOUSE_... returns 0 if no mouse input avail
    static int GetMouse(int &xpos, int& ypos, int& buttons);
};

#endif

