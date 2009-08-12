#include "pch.h"
#include "dinput.h"

//////////////////////////////////////////////////////////////////////////////
//
// DDWrapers
//
//////////////////////////////////////////////////////////////////////////////

class DIDeviceCaps : public TZeroFillWithSize<DIDEVCAPS> {
public:
};

class DIDeviceInstance : public TZeroFillWithSize<DIDEVICEINSTANCE> {
public:
};

class DIDataFormat : public TZeroFillWithSize<DIDATAFORMAT> {
public:
};

class DIObjectDataFormat : public TZeroFill<DIOBJECTDATAFORMAT> {
public:
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

const DIDATAFORMAT* g_pdfDIMouse;

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
    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    BOOL EnumObjectsCallback(
        LPCDIDEVICEOBJECTINSTANCE pddoi
    ) {
        if (
               pddoi->dwType & DIDFT_AXIS
            || pddoi->dwType & DIDFT_POV
        ) {
            int index;

                   if (pddoi->guidType == GUID_XAxis) {
                index = 0;                           
            } else if (pddoi->guidType == GUID_YAxis) {
                index = 1;
            } else if (pddoi->guidType == GUID_ZAxis) {
                index = 2;
            } else {
                index = -1;
            }

            ValueDDInputObject* pobject =
                new ValueDDInputObject(
                    pddoi->tszName,
                    pddoi->dwType,
                    pddoi->guidType
                );

            if (index == -1) {
                m_vvalueObject.PushEnd(pobject);
            } else {
                m_vvalueObject.Set(index, pobject);
            }
        } else if (pddoi->dwType & DIDFT_PSHBUTTON) {
            ButtonDDInputObject* pobject =
                new ButtonDDInputObject(
                    pddoi->tszName,
                    pddoi->dwType,
                    pddoi->guidType
                );

            m_vbuttonObject.PushEnd(pobject);
        } 

        return DIENUM_CONTINUE;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    static BOOL CALLBACK StaticEnumObjectsCallback(
        LPCDIDEVICEOBJECTINSTANCE lpddoi,
        LPVOID                    pvRef
    ) {
        MouseInputStreamImpl* pthis = (MouseInputStreamImpl*)pvRef;
        return pthis->EnumObjectsCallback(lpddoi);
    }
 
    //////////////////////////////////////////////////////////////////////////////
    //
    // members
    //
    //////////////////////////////////////////////////////////////////////////////
    
//    TRef<IDirectInputDevice2>           m_pdid;
	TRef<IDirectInputDevice8>			m_pdid;					// kg: DInput8
    TRef<ButtonEvent::SourceImpl>       m_pbuttonEventSource;
    DIDeviceCaps                        m_didc;
    DIDeviceInstance                    m_didi;
    TVector<TRef<ValueDDInputObject > > m_vvalueObject;
    TVector<TRef<ButtonDDInputObject> > m_vbuttonObject;
    Rect                                m_rect;
    Point                               m_point;
    float                               m_z;
    bool                                m_bBuffered;
    bool                                m_bEnabled;
    int                                 m_threshold1;
    int                                 m_threshold2;
    int                                 m_acceleration;
    float                               m_sensitivity;
	HWND								m_hwnd;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////
    
//    MouseInputStreamImpl(IDirectInputDevice2* pdid, HWND hwnd) :
	MouseInputStreamImpl(IDirectInputDevice8* pdid, HWND hwnd) :		// kg: DInput8
        m_pdid(pdid),
		m_hwnd(hwnd),
        m_rect(0, 0, 0, 0),                                                                         
        m_point(0, 0),
        m_vvalueObject(3),
        m_bEnabled(false),
        m_bBuffered(true),
        m_pbuttonEventSource(ButtonEvent::Source::Create())
    {
        //
        // Are we running on NT
        //

        /*!!!
        OSVERSIONINFO osvi = { sizeof(osvi) };
        ZVerify(GetVersionEx(&osvi));

        if ((VER_PLATFORM_WIN32_NT & osvi.dwPlatformId) != 0) {
            m_bBuffered = false;
        }
        */

        //
        // Enumerate the buttons and values
        //

        DDCall(m_pdid->EnumObjects(StaticEnumObjectsCallback, this, DIDFT_ALL));

        //
        // Setup the device
        //

        SetupDevice();

        //
        // We only need mouse input when we are in the foreground
        //

        DDCall(m_pdid->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));

        //
        // Get the mouse acceleration values
        //

        int pvalue[3];

        ZVerify(SystemParametersInfo(SPI_GETMOUSE, 0, pvalue, 0));

        m_threshold1   = pvalue[0];
        m_threshold2   = pvalue[1];
        m_acceleration = pvalue[2];
        m_sensitivity  = 1.0f;

        /* !!! this only works on NT50
        int speed;
        ZVerify(SystemParametersInfo(SPI_GETMOUSESPEED, 0, &speed, 0));

        if (speed <= 2) {
           m_sensitivity = float(speed) / 32.0f;
        } else if(speed >= 3 && speed <= 10 ) {
           m_sensitivity = float(speed-2) / 8.0f; 
        } else {
           m_sensitivity = float(speed-6) / 4.0f;
        }
        */
    }

    void SetupDevice() 
    {
        //
        // Set the data format
        //

        DDCall(m_pdid->SetDataFormat(g_pdfDIMouse));

        //
        // Make some buffer space
        //

        if (m_bBuffered) {
            DIPROPDWORD dipdw; 

            dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
            dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
            dipdw.diph.dwObj        = 0; 
            dipdw.diph.dwHow        = DIPH_DEVICE; 
            dipdw.dwData            = 32; 

            DDCall(m_pdid->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Methods
    //
    //////////////////////////////////////////////////////////////////////////////
    
    void DoClip()
    {
        if (
               m_rect.XMax() > m_rect.XMin()
            && m_rect.YMax() > m_rect.YMin()
        ) {
            if (m_point.X() < m_rect.XMin()) {
                m_point.SetX(m_rect.XMin());
            }

            if (m_point.X() >= m_rect.XMax()) {
                m_point.SetX(m_rect.XMax() - 1);
            }

            if (m_point.Y() < m_rect.YMin()) {
                m_point.SetY(m_rect.YMin());
            }

            if (m_point.Y() >= m_rect.YMax()) {
                m_point.SetY(m_rect.YMax() - 1);
            }
        }
    }

    float CalculateDelta(int delta)
    {
        if (abs(delta) > m_threshold1 && m_acceleration >= 1) {
            if (abs(delta) > m_threshold2 && m_acceleration >= 2) {
                return float(delta) * 4.0f * m_sensitivity;
            } else {
                return float(delta) * 2.0f * m_sensitivity;
            }
        }

        return float(delta) * m_sensitivity;
    }

    void DeltaPosition(int& dx, int& dy)
    {
        if (dx != 0 || dy != 0) {
            //ZDebugOutput("MouseMove: (" + ZString(dx) + ", " + ZString(dy) + ")\n");

            m_point.SetX(m_point.X() + CalculateDelta(dx));
            m_point.SetY(m_point.Y() - CalculateDelta(dy));

            dx = 0;
            dy = 0;

            //
            // Clip to the screen rect if required
            //

            DoClip();

            //
            // Update outputs
            //

            m_vvalueObject[0]->GetValue()->SetValue(m_point.X());
            m_vvalueObject[1]->GetValue()->SetValue(m_point.Y());
        }
    }

    void DeltaWheel(int dz)
    {
        if (dz != 0 ) {
            //ZDebugOutput("MouseDZ: " + ZString(dz) + "\n");
            m_z += float(dz);

            if (m_vvalueObject.GetCount() >= 3) {
                m_vvalueObject[0]->GetValue()->SetValue(m_z);
            }
        }
    }

    void ButtonChanged(int index, bool bDown)
    {
        //ZDebugOutput("MouseButton: " + ZString(index) + (bDown ? " down" : " up") + "\n");

        m_vbuttonObject[index]->GetValue()->SetValue(bDown);
        m_pbuttonEventSource->Trigger(ButtonEventData(index, bDown));
    }

    void UpdateBuffered()
    {
        //
        // Get the data
        //

        DIDEVICEOBJECTDATA didod;
        DWORD count = 1;
        int dx = 0;
        int dy = 0;

        while (count == 1) {
            HRESULT hr = m_pdid->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &didod, &count, 0);

            if (FAILED(hr)) {
                return;
            }

            if (count == 1) {
                //
                // Unpack the data
                //

                switch  (didod.dwOfs) {
                    case DIMOFS_BUTTON0: 
                        DeltaPosition(dx, dy);
                        ButtonChanged(0, ((didod.dwData & 0x80) != 0)); 
                        break;

                    case DIMOFS_BUTTON1: 
                        ButtonChanged(1, ((didod.dwData & 0x80) != 0)); 
                        break;

                    case DIMOFS_BUTTON2: 
                        ButtonChanged(2, ((didod.dwData & 0x80) != 0)); 
                        break;

                    case DIMOFS_BUTTON3: 
                        ButtonChanged(3, ((didod.dwData & 0x80) != 0)); 
                        break;

					// mdvalley: More buttons

					case DIMOFS_BUTTON4:
						ButtonChanged(4, ((didod.dwData & 0x80) != 0));
						break;

					case DIMOFS_BUTTON5:
						ButtonChanged(5, ((didod.dwData & 0x80) != 0));
						break;

					case DIMOFS_BUTTON6:
						ButtonChanged(6, ((didod.dwData & 0x80) != 0));
						break;

					case DIMOFS_BUTTON7:
						ButtonChanged(7, ((didod.dwData & 0x80) != 0));
						break;

                    case DIMOFS_X:
                        dx += int(didod.dwData);
                        break;

                    case DIMOFS_Y:
                        dy += int(didod.dwData);
                        break;

                    case DIMOFS_Z:
                        DeltaWheel(int(didod.dwData));
                        break;
                }
            }

            //
            // do the mouse change
            //

            DeltaPosition(dx, dy);
        }
    }

    void UpdatePolled()
    {
        //
        // Poll the device
        //

        m_pdid->Poll();

        //
        // Get the data
        //

//        DIMOUSESTATE dims;
		DIMOUSESTATE2 dims;		// mdvalley: Mousestate2 allows 8 buttons
        DDCall(m_pdid->GetDeviceState(sizeof(dims), &dims));

        //
        // Unpack the data
        //

        int dx = int(dims.lX);
        int dy = int(dims.lY);
        int dz = int(dims.lZ);

        DeltaPosition(dx, dy);
        DeltaWheel(dz);

        int count = m_vbuttonObject.GetCount();

        for (int index = 0; index < count; index++) {
            bool               bDown = ((dims.rgbButtons[index] & 0x80) != 0);
            ModifiableBoolean* pbool = m_vbuttonObject[index]->GetValue();

            if (bDown != pbool->GetValue()) {
                pbool->SetValue(bDown);
                ButtonChanged(index, bDown);
            }
        }
    }

    void Update()
    {
        //ZDebugOutput("Mouse Update\n");

        if (m_bEnabled) {
            //ZDebugOutput("Mouse Enabled\n");
            HRESULT hr = m_pdid->Acquire();

            //
            // We have to handle the case where another app has captured the
            // mouse, since we may have been switched out.
            //

            if (hr != DIERR_OTHERAPPHASPRIO) {
                DDCall(hr);

                if (m_bBuffered) {
                    UpdateBuffered();
                } else {
                    UpdatePolled();
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // MouseInputStream 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    void SetClipRect(const Rect& rect)
    {
        m_rect = rect;
        DoClip();
    }

    void SetPosition(const Point& point)
    {
        m_point = point;
        DoClip();
    }

    void SetWheelPosition(float pos)
    {
        m_z = pos;
    }

    const Point& GetPosition()
    {
        return m_point;
    }

    void SetEnabled(bool bEnabled)
    {
        if (m_bEnabled != bEnabled) {
            m_bEnabled = bEnabled;

            if (m_bEnabled) {
                DDCall(m_pdid->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND));
            } else {
//                DDCall(m_pdid->SetCooperativeLevel(m_hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND));
                DDCall(m_pdid->Unacquire());
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // InputStream
    //
    //////////////////////////////////////////////////////////////////////////////
    
    int GetValueCount()
    {
        return m_vvalueObject.GetCount();
    }

    int GetButtonCount()
    {
        return m_vbuttonObject.GetCount();
    }

    Boolean* IsDown(int id)
    {
        if (id < m_vbuttonObject.GetCount()) {
            return m_vbuttonObject[id]->GetValue();
        } else {
            return NULL;
        }
    }

    Number* GetValue(int id)
    {
        if (id < m_vvalueObject.GetCount()) {
            return m_vvalueObject[id]->GetValue();
        } else {
            return NULL;
        }
    }

    ButtonEvent::Source* GetEventSource()
    {
        return m_pbuttonEventSource;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class JoystickInputStreamImpl : public JoystickInputStream {
private:
//    TRef<IDirectInputDevice2>           m_pdid;
	TRef<IDirectInputDevice8>			m_pdid;		// kg: DInput8
    DIDeviceCaps                        m_didc;
    DIDeviceInstance                    m_didi;
    TVector<TRef<ValueDDInputObject > > m_vvalueObject;
    TVector<TRef<ButtonDDInputObject> > m_vbuttonObject;
    BYTE*                               m_pbyteData;
    DWORD                               m_sizeData;
    bool                                m_bFocus;
	CLogFile * 							m_pLogFile; //Imago 8/12/09

    TRef<IDirectInputEffect> m_peffectBounce;
    TRef<IDirectInputEffect> m_peffectFire;
    TRef<IDirectInputEffect> m_peffectExplode;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    BOOL EnumObjectsCallback(
        LPCDIDEVICEOBJECTINSTANCE pddoi
    ) {
		LPOLESTR szGUID = new WCHAR [39];
		char chGUID[39];
		StringFromGUID2(pddoi->guidType,szGUID,39);
		WideCharToMultiByte( CP_ACP, 0, szGUID, -1, chGUID, 39, 0, 0 );
		m_pLogFile->OutputStringV("\t\tpddoi->guidType: %s\n",chGUID);
		m_pLogFile->OutputStringV("\t\tpddoi->tszName: %s\n",pddoi->tszName);
		m_pLogFile->OutputStringV("\t\tpddoi->dwType: %x (instance: %x)\n",pddoi->dwType, DIDFT_GETINSTANCE(pddoi->dwType));
		m_pLogFile->OutputStringV("\t\tpddoi->wUsage: %x (page: %x)\n",pddoi->wUsage,pddoi->wUsagePage);

        if (
               pddoi->dwType & DIDFT_AXIS
            || pddoi->dwType & DIDFT_POV
        ) {
            int index;

                   if (pddoi->guidType == GUID_XAxis ) {
                index = 0;                           
            } else if (pddoi->guidType == GUID_YAxis ) {
                index = 1;
            } else if (pddoi->guidType == GUID_Slider && 
				pddoi->wUsage != 0x0037) { //Imago 8/12/09 fixes "buged" Saitek X45 Flight Control Stick driver
                index = 2;
            } else if (pddoi->guidType == GUID_RzAxis) {
                index = 3;
            } else if (pddoi->guidType == GUID_POV   ) {
                index = 4;
            } else {
                index = -1;
            }

            ValueDDInputObject* pobject =
                new ValueDDInputObject(
                    pddoi->tszName,
                    pddoi->dwType,
                    pddoi->guidType
                );

            if (index == -1) {
                m_vvalueObject.PushEnd(pobject);
            } else {
                m_vvalueObject.Set(index, pobject);
            }
        } else if (pddoi->dwType & DIDFT_PSHBUTTON) {
            ButtonDDInputObject* pobject =
                new ButtonDDInputObject(
                    pddoi->tszName,
                    pddoi->dwType,
                    pddoi->guidType
                );

            m_vbuttonObject.PushEnd(pobject);
        } 

        return DIENUM_CONTINUE;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    //////////////////////////////////////////////////////////////////////////////
    
    static BOOL CALLBACK StaticEnumObjectsCallback(
        LPCDIDEVICEOBJECTINSTANCE lpddoi,
        LPVOID                    pvRef
    ) {
        JoystickInputStreamImpl* pthis = (JoystickInputStreamImpl*)pvRef;
		pthis->m_pLogFile->OutputString("\tStaticEnumObjectsCallback:\n");
        return pthis->EnumObjectsCallback(lpddoi);
    }
 
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

//    JoystickInputStreamImpl(IDirectInputDevice2* pdid, HWND hwnd) :
	JoystickInputStreamImpl(IDirectInputDevice8* pdid, HWND hwnd, CLogFile * pLogFile) :		// kg: DInput8  Imago 8/12/09
        m_pdid(pdid),
        m_bFocus(false),
        m_vvalueObject(5),
		m_pLogFile(pLogFile)
    {
        DDCall(m_pdid->GetCapabilities(&m_didc));
        DDCall(m_pdid->GetDeviceInfo(&m_didi));

		m_pLogFile->OutputStringV("\tInitialized joystick stream - Axes: %d, Buttons: %d, POVs: %d\n",
			m_didc.dwAxes,m_didc.dwButtons,m_didc.dwPOVs);

        //
        // Enumerate the buttons and values
        //

        DDCall(m_pdid->EnumObjects(StaticEnumObjectsCallback, this, DIDFT_ALL));

        //
        // Remove any holes in the value vector
        //

        int index;
        int countValues = m_vvalueObject.GetCount();

		m_pLogFile->OutputStringV("\tInitial value count: %i\n",countValues);

        index = 0;
        while (index < countValues) {
            if (m_vvalueObject[index] == NULL) {
                if (index != countValues - 1) {
                    m_vvalueObject.Set(index, m_vvalueObject[countValues - 1]);
                }

                countValues--;
                m_vvalueObject.SetCount(countValues);
            } else {
                index++;
            }
        }

		m_pLogFile->OutputStringV("\tFinal value count: %i\n",countValues);

        //
        // Build the data format
        //

        int countButtons = m_vbuttonObject.GetCount();

        m_sizeData  = NextMultipleOf(4, countValues * 4 + countButtons * 1);

        DIDataFormat didf;

        didf.dwObjSize  = sizeof(DIOBJECTDATAFORMAT);
        didf.dwFlags    = 0;
        didf.dwDataSize = m_sizeData;
        didf.dwNumObjs  = countValues + countButtons;

        DIObjectDataFormat* pdiodf = new DIObjectDataFormat[didf.dwNumObjs];
        didf.rgodf      =  pdiodf;

        for (index = 0; index < countValues; index++) {
            ValueDDInputObject* pobject = m_vvalueObject[index];
            DIOBJECTDATAFORMAT& diodf   = didf.rgodf[index];

            diodf.pguid   = (GUID*)&(pobject->GetGUID());
            diodf.dwOfs   = index * 4;
            diodf.dwType  = pobject->GetDWType();
            diodf.dwFlags = DIDOI_ASPECTPOSITION;
        }

        for (index = 0; index < countButtons; index++) {
            ButtonDDInputObject* pobject = m_vbuttonObject[index];
            DIOBJECTDATAFORMAT&  diodf   = didf.rgodf[countValues + index];

            diodf.pguid   = (GUID*)&(pobject->GetGUID());
            diodf.dwOfs   = countValues * 4 + index;
            diodf.dwType  = pobject->GetDWType();
            diodf.dwFlags = DIDOI_ASPECTPOSITION;
        }

        DDCall(m_pdid->SetDataFormat(&didf));

        delete pdiodf;

        //
        // Allocate a data receptical
        //

        m_pbyteData = new BYTE[m_sizeData];

        //
        // We only need joystick input when we are in the foreground
        //

        DDCall(m_pdid->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND));

        //
        // Set ranges
        //

        SetRanges();
    }

    ~JoystickInputStreamImpl()
    {
        delete m_pbyteData;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetRanges()
    {
        m_pdid->Unacquire();

        int countValues  = m_vvalueObject.GetCount();

        for (int index = 0; index < countValues; index++) {
            if (m_vvalueObject[index] != NULL) {
                if ((m_vvalueObject[index]->GetDWType() & DIDFT_POV) == 0) {
                    DIPROPRANGE dipr;

                    dipr.diph.dwSize        = sizeof(dipr);
                    dipr.diph.dwHeaderSize  = sizeof(dipr.diph);
                    dipr.diph.dwHow         = DIPH_BYID;
                    dipr.diph.dwObj         = m_vvalueObject[index]->GetDWType();
                    dipr.lMin               = -100000;  
                    dipr.lMax               =  100000;   

                    DDCall(m_pdid->SetProperty(DIPROP_RANGE, &dipr.diph));
                }

                //
                //
                //

                DIPROPDWORD dipdw;  

                dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
                dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
                dipdw.diph.dwObj        = 0; 
                dipdw.diph.dwHow        = DIPH_DEVICE; 

                DDCall(m_pdid->GetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));
            }
        }
    }

    void Update()
    {
        HRESULT hr = m_pdid->Acquire();

        //
        // We have to handle the case where another app has captured the 
        // joystick, since we may be switched out.
        // Or the joystick may have been unplugged.
        //

        if (SUCCEEDED(hr)) {
            //
            // Poll the device
            //

            m_pdid->Poll();

            //
            // The MS Gamepad will return error from this funtion if the mode button is pressed
            // Don't assert in that case.  Just don't unpack the data.
            //

            hr = m_pdid->GetDeviceState(m_sizeData, m_pbyteData);

            if (SUCCEEDED(hr)) {
                //
                // Unpack the data
                //

                int countValues  = m_vvalueObject.GetCount();
                int countButtons = m_vbuttonObject.GetCount();

                for (int index = 0; index < countValues; index++) {
                    if ((m_vvalueObject[index]->GetDWType() & DIDFT_POV) != 0) {
                        int   ivalue = ((int*)m_pbyteData)[index];
                        float value;
                    
                        if (ivalue == -1) {
                            value = -2;
                        } else if (ivalue > 18000) {
                            value = float(ivalue - 36000) / 18000;
                        } else {
                            value = float(ivalue) / 18000;
                        }

                        //ZDebugOutput("RawHat: " + ZString(ivalue) + ", " + ZString(value) + "\n");

                        m_vvalueObject[index]->GetValue()->SetValue(value);
                    } else {
                        float value = float(((int*)m_pbyteData)[index]) / 100000;

                        value = bound(value, -1.0f, 1.0f);

                        m_vvalueObject[index]->GetValue()->SetValue(value);
                    }
                }

                for (int index = 0; index < countButtons; index++) {
                    m_vbuttonObject[index]->GetValue()->SetValue(
                        m_pbyteData[countValues * 4 + index] != 0
                    );
                }
            }
        }
    }

    void SetFocus(bool bFocus)
    {
        if (m_bFocus != bFocus) {
            m_bFocus = bFocus;

            if (m_bFocus) {
                //
                // Aquire the joystick
                //
        
                // !!! this doesn't work.  I guess that at the time when this gets called
                //     DInput doesn't think we are foreground yet.
                // DDCall(m_pdid->Acquire());
            } else {
                // !!! this gets called automatically
                // DDCall(m_pdid->Unacquire());
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Force Feedback
    //
    //////////////////////////////////////////////////////////////////////////////

    static BOOL CALLBACK EnumEffectTypeProc(LPCDIEFFECTINFO pei, LPVOID pv)
    {
        GUID *pguidEffect = NULL;

        // report back the guid of the effect we enumerated

        if (pv) {
            pguidEffect = (GUID *)pv;
            *pguidEffect = pei->guid;
        }

        // BUGBUG - look at this some more....

        return DIENUM_STOP;
    }

    void CreateEffects()
    {
	    GUID            guidEffect;
        DIEFFECT        diEffect;
        DIENVELOPE      diEnvelope;
        DWORD           rgdwAxes[2];
        LONG            rglDirections[2];
        DICONSTANTFORCE dicf;
        DIPERIODIC      dipf;

        //
        // initialize DIEFFECT and DIENVELOPE structures
        //

        ZeroMemory(&diEffect, sizeof(DIEFFECT));
        ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

        //
        // these fields are the same for all effects we will be creating
        //

        diEffect.dwSize                     = sizeof(DIEFFECT);
        diEffect.dwSamplePeriod             = 0; // use default sample period
        diEffect.dwTriggerButton            = DIEB_NOTRIGGER;
        diEffect.dwTriggerRepeatInterval    = 0;
        diEffect.rgdwAxes                   = rgdwAxes;
        diEffect.rglDirection               = rglDirections;
        diEffect.dwGain                     = 7500; // todo: gain selected by user

        //
        // both the "bounce" and "fire" effects will be based on the first
        // constant force effect enumerated
        //

        //
        //   don't check for errors.  DInput sometimes return error, but then works anyway.
        //

        //DDCall(
            m_pdid->EnumEffects(
                (LPDIENUMEFFECTSCALLBACK)EnumEffectTypeProc,
                &guidEffect, 
                DIEFT_CONSTANTFORCE
            //)
        );

        //
        // Create the bounce effect
        //

        dicf.lMagnitude                = 10000;
        rgdwAxes[0]                    = DIJOFS_X;
        rgdwAxes[1]                    = DIJOFS_Y;
        rglDirections[0]               = 0;
        rglDirections[1]               = 0;
        diEffect.dwFlags               = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
        diEffect.dwDuration            = 200000;
        diEffect.cAxes                 = 2;
        diEffect.lpEnvelope            = NULL;
        diEffect.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
        diEffect.lpvTypeSpecificParams = &dicf;

        //DDCall(
            m_pdid->CreateEffect(
                guidEffect,
                &diEffect, 
                &m_peffectBounce, 
                NULL
            //)
        );

        //
        // Create the fire effect
        //

        dicf.lMagnitude                = 10000;
        rgdwAxes[0]                    = DIJOFS_Y;
        rglDirections[0]               = 1;
        diEffect.dwFlags               = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
        diEffect.dwDuration            = 20000;
        diEffect.cAxes                 = 1;
        diEffect.lpEnvelope            = NULL;
        diEffect.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
        diEffect.lpvTypeSpecificParams = &dicf;

        //DDCall(
            m_pdid->CreateEffect(
                guidEffect,
                &diEffect, 
                &m_peffectFire, 
                NULL
            //)
        );

        //
        // the "explode" effect will be based on the first
        // periodic effect enumerated
        //

        //DDCall(
            m_pdid->EnumEffects(
                (LPDIENUMEFFECTSCALLBACK)EnumEffectTypeProc,
                &guidEffect, 
                DIEFT_PERIODIC
            //)
        );

        //
        // Create the explode effect.
        // We want to shape the explode effect so that it starts
        // at it's peak and then fades out
        //

        diEnvelope.dwSize              = sizeof(DIENVELOPE);
        diEnvelope.dwAttackLevel       = 0;
        diEnvelope.dwAttackTime        = 0;
        diEnvelope.dwFadeLevel         = 0;
        diEnvelope.dwFadeTime          = 1000000;
        dipf.dwMagnitude               = 10000;
        dipf.lOffset                   = 0;
        dipf.dwPhase                   = 0;
        dipf.dwPeriod                  = 100000;
        rgdwAxes[0]                    = DIJOFS_X;
        rglDirections[0]               = 0;
        diEffect.dwFlags               = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
        diEffect.dwDuration            = 1000000;
        diEffect.cAxes                 = 1;
        diEffect.lpEnvelope            = &diEnvelope;
        diEffect.cbTypeSpecificParams  = sizeof(DIPERIODIC);
        diEffect.lpvTypeSpecificParams = &dipf;

        //DDCall(
            m_pdid->CreateEffect(
                guidEffect,
                &diEffect, 
                &m_peffectExplode, 
                NULL
            //)
        );
    }

    void PlayFFEffect(short effectID, LONG lDirection)
    {
        //
        // !!! Don't check for errors here.  Sometimes these functions will return error
        //

        switch (effectID) {
            case 0:
                if (m_peffectFire) {
                    m_peffectFire->Start(1, 0);
                }
                break;

            case 1:
                if (m_peffectBounce) {
                    DIEFFECT diEffect;
                    LONG     rglDirections[2] = { 0, 0 };

                    ZeroMemory(&diEffect, sizeof(DIEFFECT));
                    diEffect.dwSize = sizeof(DIEFFECT);

                    rglDirections[0]        = lDirection * 100;
                    diEffect.dwFlags        = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
                    diEffect.cAxes          = 2;
                    diEffect.rglDirection   = rglDirections;

                    m_peffectBounce->SetParameters(&diEffect, DIEP_DIRECTION);
                    m_peffectBounce->Start(1, 0);
                }
                break;

            case 2:
                if (m_peffectExplode) {
                    m_peffectExplode->Start(1, 0);
                }
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // JoystickInputStream methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool HasForceFeedback()
    {
        return (m_didc.dwFlags & DIDC_FORCEFEEDBACK) != 0;
    }

    ZString GetShortDescription(int index)
    {
        return "Joy " + ZString(index);
    }

    ZString GetDescription()
    {
        return m_didi.tszInstanceName;
    }

    ZString GetValueDescription(int id)
    {
        return m_vvalueObject[id]->GetName();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // InputStream methods
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetValueCount()
    {
        return m_vvalueObject.GetCount();
    }

    int GetButtonCount()
    {
        return m_vbuttonObject.GetCount();
    }

    Boolean* IsDown(int id)
    {
        if (id < m_vbuttonObject.GetCount()) {
            return m_vbuttonObject[id]->GetValue();
        } else {
            return NULL;
        }
    }

    Number* GetValue(int id)
    {
        if (id < m_vvalueObject.GetCount()) {
            return m_vvalueObject[id]->GetValue();
        } else {
            return NULL;
        }
    }

    ButtonEvent::Source* GetEventSource()
    {
        ZUnimplemented();
        return NULL;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class InputEngineImpl : public InputEngine {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool EnumDeviceCallback(LPDIDEVICEINSTANCE pdidi)
    {
        TRef<IDirectInputDevice>  pdid;
//        TRef<IDirectInputDevice2> pdid2;
		TRef<IDirectInputDevice8> pdid2;		// mdvalley: DInput7

        DDCall(m_pdi->CreateDevice( pdidi->guidInstance, &pdid, NULL));
//        DDCall(pdid->QueryInterface(IID_IDirectInputDevice2, (void**)&pdid2));
		DDCall(pdid->QueryInterface(IID_IDirectInputDevice8, (void**)&pdid2));
        m_joylog.OutputStringV("\tpdidi->dwDevType: %x (subtype: %x)\n",GET_DIDEVICE_TYPE(pdidi->dwDevType),GET_DIDEVICE_SUBTYPE(pdidi->dwDevType));
        m_joylog.OutputStringV("\tpdidi->tszProductName: %s\n",pdidi->tszProductName);
        

        switch (pdidi->dwDevType & 0xff) {
			case DI8DEVTYPE_MOUSE: // kg Di8 DIDEVTYPE_MOUSE:
                {
                    if (m_pmouseInputStream == NULL) {
                        m_pmouseInputStream = new MouseInputStreamImpl(pdid2, m_hwnd);
                    }
                }
                break;

			case DI8DEVTYPE_JOYSTICK: // kg Di8 DIDEVTYPE_JOYSTICK:
			case DI8DEVTYPE_GAMEPAD: //Imago 7/23/09
			case DI8DEVTYPE_FLIGHT: // --^
                {
                    TRef<JoystickInputStreamImpl> pjoystickInputStream = 
                        new JoystickInputStreamImpl(pdid2, m_hwnd, &m_joylog);

                    m_vjoystickInputStream.PushEnd(pjoystickInputStream);
                }
                break;
        }

        return DIENUM_CONTINUE;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    static BOOL CALLBACK StaticEnumDeviceCallback(LPDIDEVICEINSTANCE pdidi, LPVOID pv)
    {
        InputEngineImpl* pthis = (InputEngineImpl*)pv;
        pthis->m_joylog.OutputString("StaticEnumDeviceCallback:\n");

        return pthis->EnumDeviceCallback(pdidi);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // data members
    //
    //////////////////////////////////////////////////////////////////////////////

    HWND                                    m_hwnd;
    bool                                    m_bFocus;
    TRef<IDirectInput>                      m_pdi;
    TVector<TRef<JoystickInputStreamImpl> > m_vjoystickInputStream;
    TRef<MouseInputStreamImpl>              m_pmouseInputStream;
    HINSTANCE                               m_hdinput;
    CLogFile                                m_joylog; //Imago 8/12/09

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    typedef HRESULT (WINAPI *PFNDirectInputCreate)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA *ppDI, LPUNKNOWN punkOuter);

    InputEngineImpl(HWND hwnd) :
        m_hwnd(hwnd),
        m_bFocus(false),
        m_joylog("DirectInput.log")
    {
        //
        // Create the direct input object
        //

        #ifdef Dynamic_DInput
            m_hdinput = ::LoadLibrary("dinput.dll");
            ZAssert(m_hdinput != NULL);

            PFNDirectInputCreate pfn = (PFNDirectInputCreate)::GetProcAddress(m_hdinput, "DirectInputCreateA");
            ZAssert(pfn != NULL);

            DDCall(pfn(
                GetModuleHandle(NULL), 
                DIRECTINPUT_VERSION, 
                &m_pdi, 
                NULL
            ));

            //
            // grab the address of a few dinput globals
            //

//            g_pdfDIMouse = (DIDATAFORMAT*)::GetProcAddress(m_hdinput, "c_dfDIMouse");
			g_pdfDIMouse = (DIDATAFORMAT*)::GetProcAddress(m_hdinput, "c_dfDIMouse2");		// mdvalley: Mouse2 for more buttons
            ZAssert(g_pdfDIMouse != NULL);
        #else
            DDCall(DirectInput8Create( // KG - Di8 update
                GetModuleHandle(NULL), 
                DIRECTINPUT_VERSION,
				IID_IDirectInput8,
                (LPVOID*)&m_pdi, 
                NULL
            ));

//            g_pdfDIMouse = &c_dfDIMouse;
			g_pdfDIMouse = &c_dfDIMouse2;		// mdvalley: Mouse2 for more buttons
        #endif

        //
        // If we failed then exit the app
        //

        if (m_pdi == NULL) {
            ::MessageBox(NULL, "Error initializing DirectInput.  Check your installation", "Error", MB_OK);
            _exit(0);
        }

        //
        // Enumerate the devices
        //

        m_joylog.OutputString("Initialized DirectInput\n");
        EnumerateJoysticks();
        m_joylog.CloseLogFile();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // InputEngine methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void EnumerateJoysticks()
    {
        //
        // Free up the old devices
        //

        m_vjoystickInputStream.SetEmpty();

        //
        // Enumerate all of the devices
        //

        DDCall(m_pdi->EnumDevices(
            0,//DIDEVTYPE_JOYSTICK,
            (LPDIENUMDEVICESCALLBACK)StaticEnumDeviceCallback,
            this, 
            DIEDFL_ATTACHEDONLY
        ));
    }

    int GetJoystickCount()
    {
        return m_vjoystickInputStream.GetCount();
    }

    JoystickInputStream* GetJoystick(int index)
    {
        if (
               index >= 0
            && index < m_vjoystickInputStream.GetCount()
        ) {
            return m_vjoystickInputStream[index];
        } else {
            return NULL;
        }
    }

    MouseInputStream* GetMouse()
    {
        return m_pmouseInputStream;
    }

    void Update()
    {
        if (m_bFocus) {
            m_pmouseInputStream->Update();

            int count = m_vjoystickInputStream.GetCount();

            for(int index = 0; index < count; index++) {
                m_vjoystickInputStream[index]->Update();
            }
        }
    }

    void SetFocus(bool bFocus)
    {
        if (m_bFocus != bFocus) {
            m_bFocus = bFocus;

            int count = m_vjoystickInputStream.GetCount();

            for(int index = 0; index < count; index++) {
                m_vjoystickInputStream[index]->SetFocus(m_bFocus);
            }
        }
    }
};

TRef<InputEngine> CreateInputEngine(HWND hwnd)
{
    return new InputEngineImpl(hwnd);
}
