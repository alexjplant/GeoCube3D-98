// OnlineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launch.h"
#include "OnlineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAOnlineDlg dialog


CAOnlineDlg::CAOnlineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAOnlineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAOnlineDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAOnlineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOnlineDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAOnlineDlg, CDialog)
	//{{AFX_MSG_MAP(CAOnlineDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAOnlineDlg message handlers
