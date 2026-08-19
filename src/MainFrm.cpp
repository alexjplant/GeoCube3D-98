// MainFrm.cpp : implementation of the CAMainFrame class
//

#include "stdafx.h"
#include "Launch.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAMainFrame

IMPLEMENT_DYNCREATE(CAMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CAMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_BAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CAMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAMainFrame construction/destruction

CAMainFrame::CAMainFrame()
{
	// TODO: add member initialization code here
	
}

CAMainFrame::~CAMainFrame()
{
}

BOOL CAMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
             WS_MINIMIZEBOX | WS_SIZEBOX;

  CWindowDC dc(NULL);
  int width = dc.GetDeviceCaps(HORZRES);
  int height = dc.GetDeviceCaps(VERTRES);

  cs.cx = 438;
  cs.cy = 280;
  cs.x = (width / 2) - (cs.cx / 2);
  cs.y = (height / 2) - (cs.cy / 2);

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAMainFrame diagnostics

#ifdef _DEBUG
void CAMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CAMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAMainFrame message handlers

int CAMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "Bar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CAMainFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_BAR
	//   5. Assign the item a Caption: Bar

	// TODO: Change the value of CG_ID_VIEW_BAR to an appropriate value:
	//   1. Open the file resource.h
	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndBar
		if (!m_wndBar.Create(this, CG_IDD_BAR,
			CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_BAR))
		{
			TRACE0("Failed to create dialog bar m_wndBar\n");
			return -1;		// fail to create
		}

//		m_wndBar.EnableDocking(CBRS_ALIGN_LEFT);
//		EnableDocking(CBRS_ALIGN_LEFT);
//		DockControlBar(&m_wndBar);
	}

	return 0;
}
