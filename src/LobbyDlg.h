#if !defined(AFX_LOBBY_H__D8DC07C2_604A_11D1_90FF_444553540000__INCLUDED_)
#define AFX_LOBBY_H__D8DC07C2_604A_11D1_90FF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Lobby.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CALobbyDlg dialog

class CALobbyDlg : public CDialog
{
// Construction
public:
	CALobbyDlg(CWnd* pParent = NULL);   // standard constructor
  ~CALobbyDlg();

// Dialog Data
	//{{AFX_DATA(CALobbyDlg)
	enum { IDD = IDD_LOBBY };
	CEdit	m_ctlChat;
	CListBox	m_lbPlayers;
	CListBox	m_lbConversation;
	CString	m_sChat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALobbyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  void LoadPlayers();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CALobbyDlg)
	afx_msg void OnSend();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
  afx_msg LONG OnDpChatString( UINT, LONG );
  afx_msg LONG OnDpCreatePlayerOrGroup( UINT, LONG );
  afx_msg LONG OnDpDestroyPlayerOrGroup( UINT, LONG );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOBBY_H__D8DC07C2_604A_11D1_90FF_444553540000__INCLUDED_)
