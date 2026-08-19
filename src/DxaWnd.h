#ifndef __DXAWND_H__
#define __DXAWND_H__

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class CADxaWnd : public CWnd
{
public:
	CADxaWnd();
	virtual ~CADxaWnd();

	BOOL Create(HWND hwndParent = NULL, BOOL bFullScreen = TRUE, CWnd **ppBackPtr = NULL );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADxaWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

  // overrides
  virtual BOOL InitGame() = 0;
  virtual BOOL Poll() = 0;
  virtual BOOL Render() = 0;
  virtual BOOL ExitGame() = 0;
  virtual void DisplayGDI( HDC hdc, CRect *pRect ) {};

// Implementation
public:
  BOOL InitSurfaces();
  BOOL UpdateFrame();
  void FlipScreen();
  void DisplayFrameRate( HDC hdc, int x, int y );

  CADirectDrawSurface  *m_pPrimary;
  CADirectDrawSurface  *m_pBack;
  DWORD                 m_nBuffers;
  CADirectDrawClipper  *m_pClipper;
  CADirectDrawPalette  *m_pPalette;
  CADirect3DRMDevice   *m_pDevice;
  CADirect3DRMViewport *m_pViewport;

  BOOL m_bFullScreen;
  BOOL m_bActive;
  int  m_BPP;

//  DWORD    m_dwTickCount;
//  DWORD    m_dwTickDiff;
//  DWORD    m_dwLastTick;

  CWnd **m_ppBackPtr;

  CRect m_rClient;    // screen coords
  CRect m_rViewport;  // screen coords
  CRect m_rPanel;     // client coords
  CRect m_rBack;

	// Generated message map functions
protected:
	//{{AFX_MSG(CADxaWnd)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif