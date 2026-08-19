#if !defined(AFX_NEWDLG_H__EA24D8A7_0662_11D1_90FF_444553540000__INCLUDED_)
#define AFX_NEWDLG_H__EA24D8A7_0662_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CANewDlg dialog

class CANewDlg : public CDialog
{
// Construction
public:
	CANewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CANewDlg)
	enum { IDD = IDD_NEW_GAME };
	CString	m_sPlayerName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CANewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CANewDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDLG_H__EA24D8A7_0662_11D1_90FF_444553540000__INCLUDED_)
