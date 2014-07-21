// WatchDlg.h : header file
//

#if !defined(AFX_WATCHDLG_H__05C943F6_A449_442B_A9ED_4CE06B20B071__INCLUDED_)
#define AFX_WATCHDLG_H__05C943F6_A449_442B_A9ED_4CE06B20B071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWatchDlg dialog

class CWatchDlg : public CDialog
{
// Construction
public:
	CWatchDlg(CWnd* pParent = NULL);	// standard constructor
    DWORD IPstart,IPend;
// Dialog Data
	//{{AFX_DATA(CWatchDlg)
	enum { IDD = IDD_WATCH_DIALOG };
	CIPAddressCtrl	m_end;
	CIPAddressCtrl	m_start;
	UINT	m_time;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWatchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    
	// Generated message map functions
	//{{AFX_MSG(CWatchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATCHDLG_H__05C943F6_A449_442B_A9ED_4CE06B20B071__INCLUDED_)
