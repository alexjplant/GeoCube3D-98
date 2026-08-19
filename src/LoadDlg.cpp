// LoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launch.h"
#include "LoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALoadDlg dialog


CALoadDlg::CALoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CALoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CALoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CALoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CALoadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CALoadDlg, CDialog)
	//{{AFX_MSG_MAP(CALoadDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALoadDlg message handlers
