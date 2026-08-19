// LaunchView.cpp : implementation of the CALaunchView class
//

#include "stdafx.h"
#include "Launch.h"

#include "LaunchDoc.h"
#include "LaunchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALaunchView

IMPLEMENT_DYNCREATE(CALaunchView, CFormView)

BEGIN_MESSAGE_MAP(CALaunchView, CFormView)
	//{{AFX_MSG_MAP(CALaunchView)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALaunchView construction/destruction

CALaunchView::CALaunchView()
	: CFormView(CALaunchView::IDD)
{
	//{{AFX_DATA_INIT(CALaunchView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CALaunchView::~CALaunchView()
{
}

void CALaunchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CALaunchView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CALaunchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CALaunchView diagnostics

#ifdef _DEBUG
void CALaunchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CALaunchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CALaunchDoc* CALaunchView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CALaunchDoc)));
	return (CALaunchDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CALaunchView message handlers


void CALaunchView::OnMove(int x, int y) 
{
	CFormView::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}
