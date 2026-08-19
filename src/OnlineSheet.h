// OnlineSheet.h : header file
//
// This class defines custom modal property sheet 
// CAOnlineSheet.
 // CAOnlineSheet has been customized to include
// a preview window.
 
#ifndef __ONLINESHEET_H__
#define __ONLINESHEET_H__

#include "OnlinePages.h"
#include "PreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CAOnlineSheet

class CAOnlineSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAOnlineSheet)

// Construction
public:
	CAOnlineSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CAOnlineMainPage m_MainPage;
	CAOnlineTypePage m_TypePage;
	CAOnlineNewPage m_NewPage;
	CAOnlineListPage m_ListPage;
	CAOnlinePlayerPage m_PlayerPage;
	CPreviewWnd m_wndPreview;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAOnlineSheet();
		 virtual BOOL OnInitDialog();

// Generated message map functions
protected:
	//{{AFX_MSG(CAOnlineSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __ONLINESHEET_H__
