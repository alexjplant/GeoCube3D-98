// LaunchView.h : interface of the CALaunchView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAUNCHVIEW_H__8E13A042_101A_11D1_90FF_444553540000__INCLUDED_)
#define AFX_LAUNCHVIEW_H__8E13A042_101A_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CALaunchView : public CFormView
{
protected: // create from serialization only
	CALaunchView();
	DECLARE_DYNCREATE(CALaunchView)

public:
	//{{AFX_DATA(CALaunchView)
	enum{ IDD = IDD_LAUNCH_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CALaunchDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALaunchView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CALaunchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CALaunchView)
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LaunchView.cpp
inline CALaunchDoc* CALaunchView::GetDocument()
   { return (CALaunchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHVIEW_H__8E13A042_101A_11D1_90FF_444553540000__INCLUDED_)
