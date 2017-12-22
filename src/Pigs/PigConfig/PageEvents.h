#pragma once

/////////////////////////////////////////////////////////////////////////////
// PageEvents.h : Declaration of the CPageEvents class.
//

#include <AGC.h>
#include <XMLPtrs.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigConfigSheet;


/////////////////////////////////////////////////////////////////////////////
// CPageEvents dialog

class CPageEvents : public CPropertyPage
{
// Declarations
public:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CPageEvents(CPigConfigSheet* pSheet);
  ~CPageEvents();

// Dialog Data
  //{{AFX_DATA(CPageEvents)
  enum { IDD = IDD_EVENTS };
  CListCtrl m_listEvents;
  CButton m_btnLogToDB;
  CButton m_btnLogToNT;
  CEdit m_staticDescription;
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
  //{{AFX_VIRTUAL(CPageEvents)
  public:
  virtual void OnOK();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  CPigConfigSheet* GetSheet();
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
  //{{AFX_MSG(CPageEvents)
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
  CPigConfigSheet*     m_pSheet;
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
// Implementation

inline CPigConfigSheet* CPageEvents::GetSheet()
{
  return m_pSheet;
}


/////////////////////////////////////////////////////////////////////////////
