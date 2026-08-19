// OnlinePages.h : header file
//

#ifndef __ONLINEPAGES_H__
#define __ONLINEPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// CAOnlineMainPage dialog

class CAOnlineMainPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOnlineMainPage)

// Construction
public:
	CAOnlineMainPage();
	~CAOnlineMainPage();

// Dialog Data
	//{{AFX_DATA(CAOnlineMainPage)
	enum { IDD = IDD_MAINPAGE };
	int		m_iHostOrJoin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineMainPage)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOnlineMainPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CAOnlineTypePage dialog

class CAOnlineTypePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOnlineTypePage)

// Construction
public:
	CAOnlineTypePage();
	~CAOnlineTypePage();

// Dialog Data
	//{{AFX_DATA(CAOnlineTypePage)
	enum { IDD = IDD_TYPEPAGE };
	CListBox	m_lbList;
	CString	m_sType;
	//}}AFX_DATA

  GUID m_guid;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineTypePage)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOnlineTypePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CAOnlineNewPage dialog

class CAOnlineNewPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOnlineNewPage)

// Construction
public:
	CAOnlineNewPage();
	~CAOnlineNewPage();

// Dialog Data
	//{{AFX_DATA(CAOnlineNewPage)
	enum { IDD = IDD_NEWPAGE };
	int		m_nMaxPlayers;
	CString	m_sSession;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineNewPage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOnlineNewPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CAOnlineListPage dialog

class CAOnlineListPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOnlineListPage)

// Construction
public:
	CAOnlineListPage();
	~CAOnlineListPage();

// Dialog Data
	//{{AFX_DATA(CAOnlineListPage)
	enum { IDD = IDD_LISTPAGE };
	CListBox	m_lbList;
	CString	m_sSession;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOnlineListPage)
	public:
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  GUID m_guid;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOnlineListPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class CAOnlinePlayerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOnlinePlayerPage)

// Construction
public:
	CAOnlinePlayerPage();
	~CAOnlinePlayerPage();

// Dialog Data
	//{{AFX_DATA(CAOnlinePlayerPage)
	enum { IDD = IDD_PLAYERPAGE };
	CString	m_sPlayer;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOnlinePlayerPage)
	public:
	virtual BOOL OnWizardFinish();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOnlinePlayerPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // __ONLINEPAGES_H__
