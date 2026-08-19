// NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launch.h"
#include "NewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CANewDlg dialog


CANewDlg::CANewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CANewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CANewDlg)
	m_sPlayerName = _T("");
	//}}AFX_DATA_INIT
}


void CANewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CANewDlg)
	DDX_Text(pDX, IDC_PLAYER_NAME, m_sPlayerName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CANewDlg, CDialog)
	//{{AFX_MSG_MAP(CANewDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CANewDlg message handlers
