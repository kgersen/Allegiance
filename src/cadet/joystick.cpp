#include "pch.h"
#include "joystick.h"
#include "memory.h"

LPDIRECTINPUTDEVICE     Joystick::s_pDevice = NULL;
LPDIRECTINPUTDEVICE2    Joystick::s_pDevice2 = NULL;
LPDIRECTINPUT           Joystick::s_pDirectInput = NULL;
LPDIRECTINPUTEFFECT     Joystick::s_pEffectBounce = NULL;
LPDIRECTINPUTEFFECT     Joystick::s_pEffectFire = NULL;
LPDIRECTINPUTEFFECT     Joystick::s_pEffectExplode = NULL;
bool                    Joystick::s_fSupportForceFeedback = false;


void Joystick::Terminate()
{
    if (s_pEffectBounce)
        s_pEffectBounce->Release();

    if (s_pEffectFire)
        s_pEffectFire->Release();

    if (s_pEffectExplode)
        s_pEffectExplode->Release();

    if (s_pDevice)
        {
        s_pDevice->Unacquire();
        s_pDevice->Release();
        }

    if (s_pDevice2)
        s_pDevice2->Release();

    if (s_pDirectInput)
        s_pDirectInput->Release();
}

void Joystick::Flush(void)
{
    // Do things this way because, ahem... SetKeyboardState() doesn't appear
    // to work on Win'95...
    for (int x=0; x<256; x++)
    {
        GetKeyboard(x);
    }
}


int Joystick::QKeyboard(void)
{
    // if there is a keyboard, returns 1 else 0
    return 0;
}


int Joystick::QMouse(void)
{
    // if no mouse, 0 else number of buttons on mouse
    return 0; 
}


int Joystick::QJoystick(void)
{
    // if no joystick(s), returns 0 else number of joysticks attached.
    return 1;    
}

int Joystick::GetKeyboard(int key)
{
    // returns 0 if the key has been depressed, else returns 1 and sets key to code recd.
    //return (GetAsyncKeyState(key) != 0);
	return GetKeyState(key) & (~1);
}

int Joystick::QKeyDepressed(int numkeys, int *keyarray)
{
    int x;

    // tells if keys in keyarray are currently depressed.  Returns 0 if not, 1 if all
    if (!numkeys || !keyarray) 
        return(0);

    for (x=0; x<numkeys ; x++)
    {
        // mask off top bit
        if ((GetAsyncKeyState(keyarray[x])) == 0)
            return(0);          
    }
    return(1);  
}

int Joystick::GetMouse(int& xpos, int&ypos, int& buttons)
{
/*    // returns 0 if no mouse action to report; else, 1 and fills in params
    int button1, button2;
    POINT pt;

    if (!GetCursorPos(&pt))
        return(0);
    
    xpos = pt.x;
    ypos = pt.y;
    buttons = 0;

    button1 = GetAsyncKeyState(VK_LBUTTON);
    button2 = GetAsyncKeyState(VK_RBUTTON);
    if (button1)
        buttons |= 1;
    if (button2)
        buttons |= 2;

    return(1);  */
    return 0;
}

int normalize (int val, int minval, int maxval);

bool Joystick::Initialize(HINSTANCE hInst, HWND hWnd)
{
    HRESULT             hr;
    DIDEVCAPS           didc;
    GUID                guidDevice;

    // create the base DirectInput object
    if (FAILED(hr = DirectInputCreate(hInst, DIRECTINPUT_VERSION, &s_pDirectInput, NULL)))
        return false;

    // enumerate for joystick devices
    if (FAILED(hr = s_pDirectInput->EnumDevices(DIDEVTYPE_JOYSTICK,
                    (LPDIENUMDEVICESCALLBACK)EnumDeviceProc,
                    &guidDevice, DIEDFL_ATTACHEDONLY)))
        return false;

    // get the joystick
    if (FAILED(hr = s_pDirectInput->CreateDevice(guidDevice, &s_pDevice, NULL)))
        return false;

    // need InputDevice2 for Force Feedback
    if (FAILED(hr = s_pDevice->QueryInterface(
                        IID_IDirectInputDevice2, (void**)&s_pDevice2)))
        return false;

    // This tells the device object to act like a specific device -- in our case, like a joystick
    if (FAILED(hr = s_pDevice->SetDataFormat(&c_dfDIJoystick)))
        return false;

    // ForceFeedback requires Exclusive access to the device.
    if (FAILED(hr = s_pDevice->SetCooperativeLevel(hWnd,
                        DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
        return false;

    // set ranges, deadzones, etc.
    if (!PrepareDevice())
        return false;

    // get the device capabilities
    didc.dwSize = sizeof(DIDEVCAPS);
    if (FAILED(hr = s_pDevice->GetCapabilities(&didc)))
        return false;

    if (didc.dwFlags & DIDC_FORCEFEEDBACK)
        {
        s_fSupportForceFeedback = true;
        if (!CreateEffects())
            return false;
        }

    Flush();

    return true;
}


BOOL CALLBACK Joystick::EnumDeviceProc(LPDIDEVICEINSTANCE pdidi, LPVOID pv)
{
    GUID *pguidDevice = NULL;

    // report back the instance guid of the device we enumerated
    if(pv)
       {
        pguidDevice = (GUID *)pv;
        *pguidDevice = pdidi->guidInstance;
        }

    // BUGBUG for now, stop after the first device has been found
    return DIENUM_STOP;
}


BOOL CALLBACK Joystick::EnumEffectTypeProc(LPCDIEFFECTINFO pei, LPVOID pv)
{
    GUID *pguidEffect = NULL;

    // report back the guid of the effect we enumerated
    if(pv)
        {
        pguidEffect = (GUID *)pv;
        *pguidEffect = pei->guid;
        }

    // BUGBUG - look at this some more....
    return DIENUM_STOP;
}


bool Joystick::AcquireDevice()
{
    if (!s_pDevice)
        return false;

    // reacquire the device
    if (FAILED(s_pDevice->Acquire()))
        return false;

    if (!CreateEffects())
        return false;

    return false;
}

bool Joystick::CreateEffects()
{
	GUID            guidEffect;
    DIEFFECT        diEffect;
    DIENVELOPE      diEnvelope;
    DWORD           rgdwAxes[2];
    LONG            rglDirections[2];
    DICONSTANTFORCE dicf;
    DIPERIODIC      dipf;

    // make sure that we have a non-NULL device object
    if (!s_pDevice2)
        return false;

    // initialize DIEFFECT and DIENVELOPE structures
    memset(&diEffect, 0, sizeof(DIEFFECT));
    memset(&diEnvelope, 0, sizeof(DIENVELOPE));

    // these fields are the same for all effects we will be creating
    diEffect.dwSize                     = sizeof(DIEFFECT);
    diEffect.dwSamplePeriod             = 0; // use default sample period
    diEffect.dwTriggerButton            = DIEB_NOTRIGGER;
    diEffect.dwTriggerRepeatInterval    = 0;
    diEffect.rgdwAxes                   = rgdwAxes;
    diEffect.rglDirection               = rglDirections;
    diEffect.dwGain                     = 7500; // todo: gain selected by user

    // both the "bounce" and "fire" effects will be based on the first
    // constant force effect enumerated
    if (FAILED(s_pDevice2->EnumEffects(
                (LPDIENUMEFFECTSCALLBACK)EnumEffectTypeProc,
                &guidEffect, DIEFT_CONSTANTFORCE)))
        return false;

    // (Re)create the bounce effect
    if (s_pEffectBounce)
        {
        s_pEffectBounce->Release();
        s_pEffectBounce= NULL;
        }

    dicf.lMagnitude                     = 10000;
    rgdwAxes[0]                         = DIJOFS_X;
    rgdwAxes[1]                         = DIJOFS_Y;
    rglDirections[0]                    = 0;
    rglDirections[1]                    = 0;
    diEffect.dwFlags                    = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
    diEffect.dwDuration                 = 200000;
    diEffect.cAxes                      = 2;
    diEffect.lpEnvelope                 = NULL;
    diEffect.cbTypeSpecificParams       = sizeof(DICONSTANTFORCE);
    diEffect.lpvTypeSpecificParams      = &dicf;

    if (FAILED(s_pDevice2->CreateEffect(guidEffect,
                    &diEffect, &s_pEffectBounce, NULL)))
        return false;

    // (Re)create the fire effect
    if (s_pEffectFire)
        {
        s_pEffectFire->Release();
        s_pEffectFire = NULL;
        }

    dicf.lMagnitude                     = 10000;
    rgdwAxes[0]                         = DIJOFS_Y;
    rglDirections[0]                    = 1;
    diEffect.dwFlags                    = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
    diEffect.dwDuration                 = 20000;
    diEffect.cAxes                      = 1;
    diEffect.lpEnvelope                 = NULL;
    diEffect.cbTypeSpecificParams       = sizeof(DICONSTANTFORCE);
    diEffect.lpvTypeSpecificParams      = &dicf;

    if (FAILED(s_pDevice2->CreateEffect(guidEffect,
                    &diEffect, &s_pEffectFire, NULL)))
        return false;

    // the "explode" effect will be based on the first
    // periodic effect enumerated
    if (FAILED(s_pDevice2->EnumEffects(
                (LPDIENUMEFFECTSCALLBACK)EnumEffectTypeProc,
                &guidEffect, DIEFT_PERIODIC)))
        return false;

    // (Re)create the explode effect
    if (s_pEffectExplode)
        {
        s_pEffectExplode->Release();
        s_pEffectExplode = NULL;
        }

    // We want to shape the explode effect so that it starts
    // at it's peak and then fades out
    diEnvelope.dwSize                   = sizeof(DIENVELOPE);
    diEnvelope.dwAttackLevel            = 0;
    diEnvelope.dwAttackTime             = 0;
    diEnvelope.dwFadeLevel              = 0;
    diEnvelope.dwFadeTime               = 1000000;
    dipf.dwMagnitude                    = 10000;
    dipf.lOffset                        = 0;
    dipf.dwPhase                        = 0;
    dipf.dwPeriod                       = 100000;
    rgdwAxes[0]                         = DIJOFS_X;
    rglDirections[0]                    = 0;
    diEffect.dwFlags                    = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
    diEffect.dwDuration                 = 1000000;
    diEffect.cAxes                      = 1;
    diEffect.lpEnvelope                 = &diEnvelope;
    diEffect.cbTypeSpecificParams       = sizeof(DIPERIODIC);
    diEffect.lpvTypeSpecificParams      = &dipf;

    if (FAILED(s_pDevice2->CreateEffect(guidEffect,
                    &diEffect, &s_pEffectExplode, NULL)))
        return false;

    return true;
}

bool Joystick::GetJoystick(DIJOYSTATE*  pdijs)
{
    HRESULT     hr;
    DWORD       dwInput = 0;

    memset(pdijs, 0, sizeof(DIJOYSTATE));

    if (!s_pDevice)
        return false;

    if (s_pDevice2)
        hr = s_pDevice2->Poll();
        
    hr = s_pDevice->GetDeviceState(sizeof(DIJOYSTATE), pdijs);

    if (FAILED(hr))
        {
        if ((hr == DIERR_INPUTLOST))
            {
            AcquireDevice();
            }
        else
            AcquireDevice();
        return true;
        }

    
    return true;
}


bool Joystick::PrepareDevice()
{
    DIPROPRANGE   dipr;

    // quick check to make sure that the object pointer is non-NULL
    if(!s_pDevice)
        return false;

    s_pDevice->Unacquire();


    // set the axis ranges for the device
    dipr.diph.dwSize        = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize  = sizeof(dipr.diph);
	dipr.diph.dwHow         = DIPH_BYOFFSET;
	dipr.lMin               = -100;  // negative to the left/top
	dipr.lMax               = 100;  // positive to the right/bottom

    dipr.diph.dwObj         = DIJOFS_X;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_Y;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_Z;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_RZ;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

/*
    dipr.diph.dwObj         = DIJOFS_RX;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_RY;
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_SLIDER(0);
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;

    dipr.diph.dwObj         = DIJOFS_SLIDER(1);
    if (FAILED(s_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph)))
        return false;
*/
    AcquireDevice();

    return true;
}


bool Joystick::PlayFFEffect(EffectID effectID, LONG lDirection)
{
    DIEFFECT        diEffect;
    
    LONG            rglDirections[2] = { 0, 0 };

    // initialize DIEFFECT structure
    memset(&diEffect, 0, sizeof(DIEFFECT));
    diEffect.dwSize = sizeof(DIEFFECT);

    switch (effectID)
        {
        case effectBounce:
            if(s_pEffectBounce)
                {
                rglDirections[0]        = lDirection * 100;
                diEffect.dwFlags        = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
                diEffect.cAxes          = 2;
                diEffect.rglDirection   = rglDirections;

                if (FAILED(s_pEffectBounce->SetParameters(&diEffect, DIEP_DIRECTION)))
                    return false;

                // play the effect
                if (FAILED(s_pEffectBounce->Start(1, 0)))
                    return false;
                }
            break;

        case effectFire:
            if(s_pEffectFire)
                {
                if (FAILED(s_pEffectFire->Start(1, 0)))
                    return false;
                }
            break;

        case effectExplode:
            if (s_pEffectExplode)
                {
                if (FAILED(s_pEffectExplode->Start(1, 0)))
                       return false;
                }
            break;
        }

    return true;
}



