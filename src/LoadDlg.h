#if !defined(AFX_LOADDLG_H__EA24D8A6_0662_11D1_90FF_444553540000__INCLUDED_)
#define AFX_LOADDLG_H__EA24D8A6_0662_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CALoadDlg dialog

class CALoadDlg : public CDialog
{
// Construction
public:
	CALoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CALoadDlg)
	enum { IDD = IDD_LOAD_GAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CALoadDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADDLG_H__EA24D8A6_0662_11D1_90FF_444553540000__INCLUDED_)
