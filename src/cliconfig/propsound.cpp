// PropSound.cpp : implementation file
//

#include "stdafx.h"
#include "cliconfig.h"
#include "PropSound.h"
#include "regkey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSound property page

IMPLEMENT_DYNCREATE(CPropSound, CPropertyPage)

CPropSound::CPropSound() : CPropertyPage(CPropSound::IDD)
{
	//{{AFX_DATA_INIT(CPropSound)
	m_bHardwareSound = TRUE;
	m_bMusic = TRUE;
	m_nMusicGain = 10;
	m_nSoundFXGain = 5;
	m_nVoiceOverGain = 0;
	m_nSoundQuality = 1;
	//}}AFX_DATA_INIT
}

CPropSound::~CPropSound()
{
}

void CPropSound::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSound)
	DDX_Check(pDX, IDC_HARDWARESOUND, m_bHardwareSound);
	DDX_Check(pDX, IDC_MUSIC, m_bMusic);
	DDX_Text(pDX, IDC_MUSICGAIN, m_nMusicGain);
	DDX_Text(pDX, IDC_SOUNDFXGAIN, m_nSoundFXGain);
	DDX_Text(pDX, IDC_VOICEOVERGAIN, m_nVoiceOverGain);
	DDX_Slider(pDX, IDC_SOUNDQUALITY, m_nSoundQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropSound, CPropertyPage)
	//{{AFX_MSG_MAP(CPropSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSound message handlers

BOOL CPropSound::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();

    CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SOUNDQUALITY);
    pSlider->SetRange(1, 3);
    pSlider->SetTicFreq(1);
    
    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    DWORD dwValue;
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
    {
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SoundHardwareAcceleration", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bHardwareSound = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Music", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bMusic = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "MusicGain", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nMusicGain = dwValue;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SFXGain", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nSoundFXGain = dwValue;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "VoiceOverGain", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nVoiceOverGain = dwValue;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SoundQuality", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nSoundQuality = dwValue;
        
        RegCloseKey(hKey);
    }

    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropSound::OnOK() 
{
    UpdateData(true);

    HKEY hKey;
    DWORD dw;
    if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw))
    {
        DWORD dwValueOne = 1;
        DWORD dwValueZero = 0;

        ::RegSetValueEx(hKey, "SoundHardwareAcceleration", NULL, REG_DWORD, 
                (const unsigned char*)(m_bHardwareSound ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "Music", NULL, REG_DWORD, 
                (const unsigned char*)(m_bMusic ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "MusicGain", NULL, REG_DWORD, 
                (const unsigned char*)&m_nMusicGain, sizeof(DWORD));

        ::RegSetValueEx(hKey, "SFXGain", NULL, REG_DWORD, 
                (const unsigned char*)&m_nSoundFXGain, sizeof(DWORD));

        ::RegSetValueEx(hKey, "VoiceOverGain", NULL, REG_DWORD, 
                (const unsigned char*)&m_nVoiceOverGain, sizeof(DWORD));

        ::RegSetValueEx(hKey, "SoundQuality", NULL, REG_DWORD, 
                (const unsigned char*)&m_nSoundQuality, sizeof(DWORD));

        ::RegCloseKey(hKey);
    }

    CPropertyPage::OnOK();
}
