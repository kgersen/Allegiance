#pragma once
#ifndef __XMLPtrs_h__
#define __XMLPtrs_h__

/////////////////////////////////////////////////////////////////////////////
// XMLPtrs.h : Smart pointer declarations for MSXML interface pointers.
//

#include "..\..\Extern\sbn\include\msxml.h"

#ifdef __cplusplus
  extern "C++"
  {
    /////////////////////////////////////////////////////////////////////////
    // Smart Pointer Declarations

    #include <comdef.h>

    #ifndef TC_COM_SMARTPTR_TYPEDEF
      #define TC_COM_SMARTPTR_TYPEDEF(Interface) \
        _COM_SMARTPTR_TYPEDEF(Interface, __uuidof(Interface))
    #endif // TC_COM_SMARTPTR_TYPEDEF

    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMImplementation);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMNode);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMDocumentFragment);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMDocument);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMNodeList);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMNamedNodeMap);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMCharacterData);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMAttribute);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMElement);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMText);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMComment);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMProcessingInstruction);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMCDATASection);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMDocumentType);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMNotation);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMEntity);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMEntityReference);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDOMParseError);
    TC_COM_SMARTPTR_TYPEDEF(IXTLRuntime);
    TC_COM_SMARTPTR_TYPEDEF(IXMLHttpRequest);
    TC_COM_SMARTPTR_TYPEDEF(IXMLDSOControl);
//    TC_COM_SMARTPTR_TYPEDEF(IXMLElementCollection); //already in <comdef.h>
//    TC_COM_SMARTPTR_TYPEDEF(IXMLDocument);          //already in <comdef.h>
    TC_COM_SMARTPTR_TYPEDEF(IXMLDocument2);
//    TC_COM_SMARTPTR_TYPEDEF(IXMLElement);           //already in <comdef.h>
    TC_COM_SMARTPTR_TYPEDEF(IXMLElement2);
    TC_COM_SMARTPTR_TYPEDEF(IXMLAttribute);
//    TC_COM_SMARTPTR_TYPEDEF(IXMLError);             //already in <comdef.h>

  } // extern "C++"
#endif // __cplusplus


/////////////////////////////////////////////////////////////////////////////

#endif // !__XMLPtrs_h__
