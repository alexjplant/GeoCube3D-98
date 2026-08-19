#if !defined(AFX_ONLINECLG_H__EA24D8A8_0662_11D1_90FF_444553540000__INCLUDED_)
#define AFX_ONLINECLG_H__EA24D8A8_0662_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OnlineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAOnlineDlg dialog

class CAOnlineDlg : public CDialog
{
// Construction
public:
	CAOnlineDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAOnlineDlg)
	enum { IDD = IDD_ONLINE_GAMES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAOnlineDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINECLG_H__EA24D8A8_0662_11D1_90FF_444553540000__INCLUDED_)
