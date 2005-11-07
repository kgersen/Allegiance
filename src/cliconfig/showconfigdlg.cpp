// ShowConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cliconfig.h"
#include "ShowConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowConfigDlg dialog


CShowConfigDlg::CShowConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowConfigDlg)
	m_strContents = _T("");
	//}}AFX_DATA_INIT
}


void CShowConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowConfigDlg)
	DDX_Text(pDX, IDC_CONTENTS, m_strContents);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CShowConfigDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowConfigDlg message handlers
