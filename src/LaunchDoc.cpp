// LaunchDoc.cpp : implementation of the CALaunchDoc class
//

#include "stdafx.h"
#include "Launch.h"

#include "LaunchDoc.h"
#include "NewDlg.h"
#include "GamePlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALaunchDoc

IMPLEMENT_DYNCREATE(CALaunchDoc, CDocument)

BEGIN_MESSAGE_MAP(CALaunchDoc, CDocument)
	//{{AFX_MSG_MAP(CALaunchDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALaunchDoc construction/destruction

CALaunchDoc::CALaunchDoc()
{
}

CALaunchDoc::~CALaunchDoc()
{
}

void CALaunchDoc::DeleteContents() 
{
	CDocument::DeleteContents();

}

BOOL CALaunchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
void CALaunchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CALaunchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CALaunchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CALaunchDoc commands

BOOL CALaunchDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	return TRUE;
}
