#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// Engine App
//
//////////////////////////////////////////////////////////////////////////////

HRESULT EngineApp::Initialize(const ZString& strCommandLine)
{
    //
    // Initialize the pane code
    //

    Pane::Initialize();

    //
    // Parse the command line for device options
    //

    bool bSoftwareOnly;
    bool bPrimaryOnly;

    ParseCommandLine(strCommandLine, bSoftwareOnly, bPrimaryOnly);

    //
    // Create the engine
    //

    m_pengine = CreateEngine(!bSoftwareOnly, !bPrimaryOnly);

    if (!m_pengine->IsValid()) {
        return E_FAIL;
    }

    //
    // Create the modeler
    //

    m_pmodeler = Modeler::Create(m_pengine);

    //
    // Create the popup container
    //

    m_ppopupContainer = CreatePopupContainer();

    return S_OK;
}

void EngineApp::Terminate()
{
    if (m_pmodeler)
        m_pmodeler->Terminate();
    if (m_pengine)
        m_pengine->Terminate();

    m_pmodeler = NULL;
    m_pengine = NULL;

    Win32App::Terminate();
}

void EngineApp::ParseCommandLine(
    const ZString& strCommandLine,
    bool& bSoftwareOnly,
    bool& bPrimaryOnly
) {
    bPrimaryOnly     = false;
    bSoftwareOnly    = false;

    PCC pcc = strCommandLine;
    CommandLineToken token(pcc, strCommandLine.GetLength());

    while (token.MoreTokens()) {
        ZString str;
        if (token.IsMinus(str)) {
            if (str == "primary") {
                bPrimaryOnly = true;
            } else if (str == "software") {
                bSoftwareOnly = true;
            }
        } else {
            token.IsString(str);
        }
    }
}

void EngineApp::SetMouse(MouseInputStream* pmouse)
{
    m_pmouse = pmouse;
}

int EngineApp::OnException(DWORD code, ExceptionData* pdata)
{
    if (m_pengine && m_pengine->IsFullscreen()) {
        m_pengine->DebugSetWindowed();
        if (m_pmouse) {
            m_pmouse->SetEnabled(false);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

#ifdef _DEBUG
    bool EngineApp::OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule)
    {
        OnException(0, NULL);
        return Win32App::OnAssert(psz, pszFile, line, pszModule);
    }
#endif
