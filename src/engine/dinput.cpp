#ifdef USE_SDL3
    #include <SDL3/SDL.h>
#else
    #include <dinput.h>
#endif

#include <regkey.h>
#include <zreg.h>

#include "LogFile.h"
#include "ddstruct.h"
#include "enginep.h"
#include "inputengine.h"
#include "value.h"

#ifndef DIFEF_MODIFYIFNEEDED
# define DIFEF_MODIFYIFNEEDED		0x00000010
#endif

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// SDL3 Input Implementation
// DirectInput classes were removed or abstracted

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class DDInputObject : public IObject {
private:
    ZString m_strName;
    DWORD   m_dwType;
    GUID    m_guidType;

public:
    DDInputObject(const ZString& strName, DWORD dwType, const GUID& guidType) :
        m_strName(strName),
        m_dwType(dwType),
        m_guidType(guidType)
    {
    }

    const ZString& GetName() const
    {
        return m_strName;
    }

    DWORD GetDWType() const
    {
        return m_dwType;
    }

    const GUID& GetGUID() const
    {
        return m_guidType;
    }
};

class ValueDDInputObject : public DDInputObject {
private:
    TRef<ModifiableNumber> m_pnumber;

public:
    ValueDDInputObject(const ZString& strName, DWORD dwID, const GUID& guidType) :
        DDInputObject(strName, dwID, guidType),
        m_pnumber(new ModifiableNumber(0))
    {                        
    }

    ModifiableNumber* GetValue() const
    {
        return m_pnumber;
    }
};

class ButtonDDInputObject : public DDInputObject {
private:
    TRef<ModifiableBoolean> m_pbool;

public:
    ButtonDDInputObject(const ZString& strName, DWORD dwID, const GUID& guidType) :
        DDInputObject(strName, dwID, guidType),
        m_pbool(new ModifiableBoolean(false))
    {
    }

    ModifiableBoolean* GetValue() const
    {
        return m_pbool;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class MouseInputStreamImpl : public MouseInputStream {
private:
    TRef<ButtonEvent::SourceImpl>       m_pbuttonEventSource;
    Rect                                m_rect;
    Point                               m_point;
    float                               m_z;
    bool                                m_bEnabled;
    float                               m_sensitivity;
    int                                 m_acceleration;
    HWND                                m_hwnd;
    CLogFile *                          m_pLogFile;
    float                               m_vvalue[3];
    bool                                m_vbutton[10];

public:
    MouseInputStreamImpl(HWND hwnd, CLogFile * pLogFile) :
        m_hwnd(hwnd),
        m_rect(0, 0, 0, 0),
        m_point(0, 0),
        m_bEnabled(false),
        m_pbuttonEventSource(ButtonEvent::Source::Create()),
        m_pLogFile(pLogFile),
        m_z(0),
        m_sensitivity(1.0f),
        m_acceleration(1)
    {
        for (int i = 0; i < 3; i++) m_vvalue[i] = 0;
        for (int i = 0; i < 10; i++) m_vbutton[i] = false;
    }

    void SetSensitivity(const float sens) { m_sensitivity = sens; } 
    void SetAccel(const int accel) { m_acceleration = accel; } 

    void SetClipRect(const Rect& rect)
    {
        m_rect = rect;
    }

    void SetPosition(const Point& point)
    {
        m_point = point;
    }

    void SetWheelPosition(float pos)
    {
        m_z = pos;
    }

    float GetWheelPosition()
    {
        return m_z;
    }

    const Point& GetPosition()
    {
        return m_point;
    }

    bool IsEnabled()
    {
        return m_bEnabled;
    }

    void SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
    }

    void Update()
    {
        if (m_bEnabled) {
            float mouseX, mouseY;
            uint32_t buttons = SDL_GetMouseState(&mouseX, &mouseY);

            m_point.SetX(mouseX);
            m_point.SetY(mouseY);

            m_vvalue[0] = mouseX;
            m_vvalue[1] = mouseY;

            // Handle buttons
            for (int i = 0; i < 5; i++) {
                bool bDown = (buttons & SDL_BUTTON_MASK(i + 1)) != 0;
                if (bDown != m_vbutton[i]) {
                    m_vbutton[i] = bDown;
                    m_pbuttonEventSource->Trigger(ButtonEventData(i, bDown));
                }
            }
        }
    }

    int GetValueCount()  { return 3; }
    int GetButtonCount() { return 10; }

    Boolean* IsDown(int id)
    {
        if (id < 10) {
            return new ModifiableBoolean(m_vbutton[id]);
        }
        return NULL;
    }

    Number* GetValue(int id)
    {
        if (id < 3) {
            return new ModifiableNumber(m_vvalue[id]);
        }
        return NULL;
    }

    ButtonEvent::Source* GetEventSource()
    {
        return m_pbuttonEventSource;
    }
};

class JoystickInputStreamImpl : public JoystickInputStream {
private:
    SDL_Joystick*                       m_pjoystick;
    SDL_JoystickID                      m_instanceID;
    TRef<ButtonEvent::SourceImpl>       m_pbuttonEventSource;
    TVector<float>                      m_vvalue;
    TVector<bool>                       m_vbutton;
    ZString                             m_strName;
    CLogFile*                           m_pLogFile;
    HWND                                m_hwnd;

public:
    JoystickInputStreamImpl(SDL_JoystickID instanceID, HWND hwnd, CLogFile* pLogFile) :
        m_instanceID(instanceID),
        m_hwnd(hwnd),
        m_pLogFile(pLogFile),
        m_pbuttonEventSource(ButtonEvent::Source::Create())
    {
        m_pjoystick = SDL_OpenJoystick(instanceID);
        if (m_pjoystick) {
            m_strName = SDL_GetJoystickName(m_pjoystick);
            int nAxes = SDL_GetNumJoystickAxes(m_pjoystick);
            int nButtons = SDL_GetNumJoystickButtons(m_pjoystick);
            int nHats = SDL_GetNumJoystickHats(m_pjoystick);

            m_vvalue.SetCount(nAxes + nHats * 2);
            for (int i = 0; i < m_vvalue.GetCount(); i++) m_vvalue.Set(i, 0.0f);

            m_vbutton.SetCount(nButtons);
            for (int i = 0; i < nButtons; i++) m_vbutton.Set(i, false);

            if (m_pLogFile) {
                m_pLogFile->OutputStringV("\tInitialized joystick: %s (Axes: %d, Buttons: %d, Hats: %d)\n",
                    (PCC)m_strName, nAxes, nButtons, nHats);
            }
        }
    }

    ~JoystickInputStreamImpl()
    {
        if (m_pjoystick) {
            SDL_CloseJoystick(m_pjoystick);
        }
    }

    void SetRanges() {}
    bool HasForceFeedback() { return false; }
    void CreateEffects() {}
    void PlayFFEffect(short effectID, LONG lDirection) {}

    ZString GetShortDescription(int index) { return "Joy " + ZString(index); }
    ZString GetDescription() { return m_strName; }
    ZString GetValueDescription(int id) { return "Axis " + ZString(id); }

    int GetValueCount() { return m_vvalue.GetCount(); }
    int GetButtonCount() { return m_vbutton.GetCount(); }

    Boolean* IsDown(int id)
    {
        if (id >= 0 && id < m_vbutton.GetCount()) {
            return new ModifiableBoolean(m_vbutton[id]);
        }
        return NULL;
    }

    Number* GetValue(int id)
    {
        if (id >= 0 && id < m_vvalue.GetCount()) {
            return new ModifiableNumber(m_vvalue[id]);
        }
        return NULL;
    }

    ButtonEvent::Source* GetEventSource() { return m_pbuttonEventSource; }

    void Update()
    {
        if (m_pjoystick) {
            int nAxes = SDL_GetNumJoystickAxes(m_pjoystick);
            for (int i = 0; i < nAxes; i++) {
                m_vvalue.Set(i, (float)SDL_GetJoystickAxis(m_pjoystick, i) / 32767.0f);
            }

            int nButtons = SDL_GetNumJoystickButtons(m_pjoystick);
            for (int i = 0; i < nButtons; i++) {
                bool bDown = SDL_GetJoystickButton(m_pjoystick, i) != 0;
                if (bDown != m_vbutton[i]) {
                    m_vbutton.Set(i, bDown);
                    m_pbuttonEventSource->Trigger(ButtonEventData(i, bDown));
                }
            }

            // Handle Hats as axes for simplicity in this mapping
            int nHats = SDL_GetNumJoystickHats(m_pjoystick);
            for (int i = 0; i < nHats; i++) {
                uint8_t hat = SDL_GetJoystickHat(m_pjoystick, i);
                float hx = 0, hy = 0;
                if (hat & SDL_HAT_LEFT) hx = -1.0f;
                if (hat & SDL_HAT_RIGHT) hx = 1.0f;
                if (hat & SDL_HAT_UP) hy = 1.0f;
                if (hat & SDL_HAT_DOWN) hy = -1.0f;
                int axisIdx = nAxes + i * 2;
                if (axisIdx + 1 < m_vvalue.GetCount()) {
                    m_vvalue.Set(axisIdx, hx);
                    m_vvalue.Set(axisIdx + 1, hy);
                }
            }
        }
    }

    void SetFocus(bool bFocus) {}
};

//////////////////////////////////////////////////////////////////////////////
//
// Input Engine Implementation
//
//////////////////////////////////////////////////////////////////////////////

class InputEngineImpl : public InputEngine {
private:
    HWND                                    m_hwnd;
    bool                                    m_bFocus;
    TVector<TRef<JoystickInputStreamImpl> > m_vjoystickInputStream;
    TRef<MouseInputStreamImpl>              m_pmouseInputStream;
    CLogFile                                m_joylog;

public:
    InputEngineImpl(HWND hwnd) :
        m_hwnd(hwnd),
        m_bFocus(false),
        m_joylog("Input.log")
    {
        m_joylog.OutputString("Initialized Input Engine (SDL3)\n");
        m_pmouseInputStream = new MouseInputStreamImpl(m_hwnd, &m_joylog);
        EnumerateJoysticks();
    }

    void EnumerateJoysticks()
    {
        m_vjoystickInputStream.SetEmpty();
        int nJoysticks = 0;
        SDL_JoystickID* joysticks = SDL_GetJoysticks(&nJoysticks);
        if (joysticks) {
            for (int i = 0; i < nJoysticks; i++) {
                m_vjoystickInputStream.PushEnd(new JoystickInputStreamImpl(joysticks[i], m_hwnd, &m_joylog));
            }
            SDL_free(joysticks);
        }
    }

    int GetJoystickCount() { return m_vjoystickInputStream.GetCount(); }
    JoystickInputStream* GetJoystick(int index)
    {
        if (index >= 0 && index < m_vjoystickInputStream.GetCount()) {
            return m_vjoystickInputStream[index];
        }
        return NULL;
    }

    MouseInputStream* GetMouse() { return m_pmouseInputStream; }

    void Update()
    {
        m_pmouseInputStream->Update();
        for (int i = 0; i < m_vjoystickInputStream.GetCount(); i++) {
            m_vjoystickInputStream[i]->Update();
        }
    }

    void SetFocus(bool bFocus)
    {
        m_bFocus = bFocus;
        m_pmouseInputStream->SetEnabled(bFocus);
    }
};

TRef<InputEngine> CreateInputEngine(HWND hwnd)
{
    return new InputEngineImpl(hwnd);
}
