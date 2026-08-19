#if !defined(AFX_LAUNCHDOC_H__8E13A040_101A_11D1_90FF_444553540000__INCLUDED_)
#define AFX_LAUNCHDOC_H__8E13A040_101A_11D1_90FF_444553540000__INCLUDED_

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class CALaunchDoc : public CDocument
{
protected: // create from serialization only
	CALaunchDoc();
	DECLARE_DYNCREATE(CALaunchDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALaunchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CALaunchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CALaunchDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHDOC_H__8E13A040_101A_11D1_90FF_444553540000__INCLUDED_)
