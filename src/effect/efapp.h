#ifndef _efapp_h_
#define _efapp_h_

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class EffectApp : public EngineApp {
private:
    TRef<INameSpace> m_pns;

public:
    TRef<INameSpace> OptimizeThingGeo(const ZString& str, Geo* pgeo, Number* pnumber);

    //
    // Win32App methods
    //

    HRESULT Initialize(const ZString& strCommandLine, HWND hWindow);
    void Terminate();
};

//////////////////////////////////////////////////////////////////////////////
//
// TrekResources
//
//////////////////////////////////////////////////////////////////////////////

class TrekResources {
public:
	
    static IEngineFont* SmallFont();
    static IEngineFont* SmallBoldFont();
    static IEngineFont* LargeFont();
    static IEngineFont* LargeBoldFont();
    static IEngineFont* HugeFont();
    static IEngineFont* HugeBoldFont();

    static COLORREF WhiteColor() { return RGB(255,255,255); }
    static COLORREF BlackColor() { return RGB(0,0,0); }
    static COLORREF CyanColor() { return RGB(117,217,210); }
    static COLORREF YellowColor() { return RGB(207,255,55); }
    static COLORREF GridTextColor() { return RGB(117,217,210); }
    static COLORREF GridSelTextColor() { return RGB(0,0,0); }

    static void Initialize(Modeler* pmodeler);
    static void Terminate();

	//<Djole date="2014-09-19">
	//Added for font adjustments
	static void chatFontSize(int size);
	static int maxChatFontSize();
	static int minChatFontSize();
	static void isCombat(bool val);
	static bool isCombat();

	static IEngineFont* pilotChatFont(bool combat=isCombat());
	static IEngineFont* commanderChatFont(bool combat=isCombat());
	static int chatFontSize();
private:
	static void initChatFonts(HFONT pilot, HFONT commander);
	
	//</Djole>
};

#endif
