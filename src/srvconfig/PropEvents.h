#ifndef __PropEvents_h__
#define __PropEvents_h__

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// PropGeneral.h : header file
//

#include <AGC.h>
#include <XMLPtrs.h>


/////////////////////////////////////////////////////////////////////////////
// CPropEvents dialog

class CPropEvents : public CPropertyPage
{
// Declarations
public:
    DECLARE_DYNCREATE(CPropEvents)
    DECLARE_MESSAGE_MAP()

// Construction
public:
    CPropEvents();
    ~CPropEvents();

// Dialog Data
    //{{AFX_DATA(CPropEvents)
    enum { IDD = IDD_EVENTS };
    CListCtrl m_listEvents;
    CButton m_btnLogToDB;
    CButton m_btnLogToNT;
    CEdit   m_staticDescription;
    CStatic m_staticID;
    CStatic m_staticType;
    CTreeCtrl m_tree;
    //}}AFX_DATA

// Attributes
public:
    bool IsDisplayable();

// Overrides
public:
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropEvents)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HRESULT GetEventLogger();
    void    InitEventTree();
    void    InitEventList();
    HRESULT PopulateTreeAndList();
    HRESULT AddXMLNodeToTree(IXMLDOMNode* pNode, HTREEITEM hParent);
    HRESULT GetElementSeverity(IXMLDOMElement* pElem, BSTR* pbstr);
    HRESULT GetElementDisplayName(IXMLDOMElement* pElem, BSTR* pbstr);
    HRESULT GetElementID(IXMLDOMElement* pElem, BSTR* pbstr);
    HRESULT GetElementID(IXMLDOMElement* pElem, int* pID);
    HRESULT UpdateNodeFromItem(LPARAM lNode);
    HRESULT UpdateItemFromNode(LPARAM lNode);
    int     ImageFromSeverity(BSTR bstrSeverity);
    CString TypeFromSeverity(BSTR bstrSeverity);
    void    ToggleCheckBox(UINT id);
    PFNLVCOMPARE SortProcFromColumn(int iSubItem);
protected:
    static int CALLBACK CompareType(LPARAM lp1, LPARAM lp2, LPARAM lpThis);
    static int CALLBACK CompareID  (LPARAM lp1, LPARAM lp2, LPARAM lpThis);
    static int CALLBACK CompareName(LPARAM lp1, LPARAM lp2, LPARAM lpThis);

// Message Handlers
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropEvents)
    virtual BOOL OnInitDialog();
    afx_msg void OnDeleteItemEvents(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnKeyDownEvents(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelChangedEvents(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnByGroup();
    afx_msg void OnFlatList();
    afx_msg void OnDeleteItemEventsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickEventsList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

// Data Members
protected:
    CImageList           m_images;
    IXMLDOMDocumentPtr   m_spXMLDoc;
    IAGCEventIDRangesPtr m_spRangesNT;
    IAGCEventIDRangesPtr m_spRangesDB;
    IAGCEventLoggerPtr   m_spEventLogger;
    CComBSTR             m_bstrEvent;
    CComBSTR             m_bstrEventGroup;
    CComBSTR             m_bstrName;
    CComBSTR             m_bstrDisplayName;
    CComBSTR             m_bstrSeverity;
    CComBSTR             m_bstrID;
    CComBSTR             m_bstrLowerBound;
    CComBSTR             m_bstrUpperBound;
    CComBSTR             m_bstrDescription;
    CComBSTR             m_bstrLogAsNTEvent;
    CComBSTR             m_bstrLogAsDBEvent;
    CComBSTR             m_bstrCanChangeLogAsNTEvent;
    CComBSTR             m_bstrCanChangeLogAsDBEvent;
    LPARAM               m_lParamSelected;
    bool                 m_bLoggerObjectIsAlive;
    int                  m_cxMaxType;
    int                  m_cxMaxID;
    int                  m_nSortDirection;
    int                  m_nSortColumn;
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !__PropEvents_h__
