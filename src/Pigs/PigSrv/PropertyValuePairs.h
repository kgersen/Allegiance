

/////////////////////////////////////////////////////////////////////////////
// Types

typedef std::map<CComBSTR, CComBSTR> CPropertyValuePairs;


/////////////////////////////////////////////////////////////////////////////

HRESULT ParsePropertyValuePairs(BSTR bstr, CPropertyValuePairs* pPairs)
{
  // Initialize the specified [out] parameter
  pPairs->clear();

  // Do nothing if bstr is NULL
//  if (!bstr)
    return S_OK;

  #if 0
    CComBSTR bstrProperty, bstrValue;

    // Move past initial white space
    WCHAR pszToken = bstr;
    WCHAR psz = bstr;
    while (iswspace(*psz))
      ++psz;

    // If first character is a double-quote, match it
    if (L'\"' == *psz)
    {
      WCHAR pszFind = wcschr(psz, L'\"');
      if (!pszFind)
        return E_INVALIDARG;
      bstrProperty = CComBSTR((psz = pszFind + 1) - pszToken, pszToken);

      // Skip past any more whitespace
      while (iswspace(*psz))
        ++psz;
    }
  #endif
 
}

