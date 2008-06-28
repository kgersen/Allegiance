/////////////////////////////////////////////////////////////////////////////
// AppModeBind.cpp : Declaration/implementation of the CAppModeBind class.
//

#include "pch.h"
#include "MSRGuard.h"
#include "AppMode.h"
#include "AppModeBind.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
//
//
class CAppModeBind :
  public IAppMode
{
// IAppMode Interface Methods
public:
  STDMETHODIMP Run(int argc, TCHAR* argv[])
  {
    // Get the current module
    TCHAR szModule[_MAX_PATH];
    ::GetModuleFileName(NULL, szModule, sizeofArray(szModule));

    // Get the name of the bind module
    ZString strBind(g.GetOptionString("bind"));
    assert(!strBind.IsEmpty());

    // Copy the current module to the specified path
    if (!::CopyFile(szModule, strBind, false))
      return g.HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_BINDCOPY, szModule, (LPCTSTR)strBind);

    // Begin updating the resources of the specified file    
    HANDLE hUpdRes = ::BeginUpdateResource(strBind, false);
    if (!hUpdRes)
      return g.HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_BINDRES, (LPCTSTR)strBind);

    // Update the CONFIG resource, if an override is specified
    ZString strINI;
    if (g.GetOptionValue("INI", strINI))
    {
      // Read the specified INI file
      ZFile file(strINI);

      // Enumerate the languages of the resource to be updated
      std::vector<WORD> languages;
      if (!::EnumResourceLanguages(g.GetResourceInstance(), TEXT("CONFIG"),
        MAKEINTRESOURCE(1), EnumLangProc, (LPARAM)&languages))
      {
        HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
        ::EndUpdateResource(hUpdRes, true);
        return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
      }

      // Update the CONFIG resource of each language
      for (WORD* it = languages.begin(); it != languages.end(); ++it)
      {
        // Update the CONFIG resource
        if (!::UpdateResource(hUpdRes, TEXT("CONFIG"), MAKEINTRESOURCE(1),
          *it, file.GetPointer(), file.GetLength()))
        {
          HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
          ::EndUpdateResource(hUpdRes, true);
          return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
        }
      }
    }

    // Update the MINI resource, if an override is specified
    if (g.GetOptionExists("MINI"))
    {
      // Get the MINI text
      ZString strMINI;
      ZSucceeded(g.LoadMINI(strMINI));

      // Enumerate the languages of the resource to be updated
      std::vector<WORD> languages;
      if (!::EnumResourceLanguages(g.GetResourceInstance(), TEXT("MINI"),
        MAKEINTRESOURCE(1), EnumLangProc, (LPARAM)&languages))
      {
        HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
        ::EndUpdateResource(hUpdRes, true);
        return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
      }

      // Update the MINI resource of each language
      for (WORD* it = languages.begin(); it != languages.end(); ++it)
      {
        // Update the MINI resource
        if (!::UpdateResource(hUpdRes, TEXT("MINI"), MAKEINTRESOURCE(1),
          *it, const_cast<void*>((const void*)(LPCTSTR)strMINI), strMINI.GetLength()))
        {
          HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
          ::EndUpdateResource(hUpdRes, true);
          return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
        }
      }
    }

    // Update the RT_HTML resource, if an override is specified
    if (g.GetOptionExists("HTML"))
    {
      // Get the HTML text
      ZString strHTML;
      ZSucceeded(g.LoadHTML(strHTML));

      // Enumerate the languages of the resource to be updated
      std::vector<WORD> languages;
      if (!::EnumResourceLanguages(g.GetResourceInstance(), RT_HTML,
        MAKEINTRESOURCE(1), EnumLangProc, (LPARAM)&languages))
      {
        HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
        ::EndUpdateResource(hUpdRes, true);
        return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
      }

      // Update the RT_HTML resource of each language
      for (WORD* it = languages.begin(); it != languages.end(); ++it)
      {
        // Update the RT_HTML resource
        if (!::UpdateResource(hUpdRes, RT_HTML, MAKEINTRESOURCE(1),
          *it, const_cast<void*>((const void*)(LPCTSTR)strHTML), strHTML.GetLength()))
        {
          HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
          ::EndUpdateResource(hUpdRes, true);
          return g.HandleError(hr, IDS_E_FMT_BINDRES, (LPCTSTR)strBind);
        }
      }
    }

    // End updating resources
    if (!::EndUpdateResource(hUpdRes, false))
      return g.HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_BINDRES, (LPCTSTR)strBind);

    // Display a success message
    g.DisplayMessage(IDS_S_FMT_BINDRES, (LPCTSTR)strBind);

    // Indicate success
    return S_OK;
  }

// Implementation
protected:
  static BOOL CALLBACK EnumLangProc(HINSTANCE, LPCTSTR, LPCTSTR,
    WORD wIDLanguage, LPARAM lParam)
  {
    std::vector<WORD>* pLangs = reinterpret_cast<std::vector<WORD>*>(lParam);
    pLangs->push_back(wIDLanguage);
    return true;
  }
};


/////////////////////////////////////////////////////////////////////////////
// Instantiation Method
//
IAppMode* CreateAppModeBind()
{
  IAppMode* pAppMode = new CAppModeBind;
  if (pAppMode)
    pAppMode->AddRef();
  return pAppMode;
}

