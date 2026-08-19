#if !defined(AFX_OPTIONSDLG_H__EA24D8A9_0662_11D1_90FF_444553540000__INCLUDED_)
#define AFX_OPTIONSDLG_H__EA24D8A9_0662_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAOptionsDlg dialog

class CAOptionsDlg : public CDialog
{
// Construction
public:
	CAOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAOptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CComboBox	m_cbVideoMode;
	BOOL	m_bEffects;
	BOOL	m_bMusic;
	BOOL	m_bWindowed;
	int		m_iController;
	//}}AFX_DATA
  int   m_iVideoMode;
  int   m_iModeX;
  int   m_iModeY;
  int   m_iModeBPP;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAOptionsDlg)
	afx_msg void OnDefaults();
	afx_msg void OnSetupJoystick();
	afx_msg void OnSetupKeyboard();
	afx_msg void OnSetupMouse();
	afx_msg void OnMusic();
	afx_msg void OnEffects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__EA24D8A9_0662_11D1_90FF_444553540000__INCLUDED_)
