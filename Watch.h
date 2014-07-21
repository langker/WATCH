// Watch.h : main header file for the WATCH application
//

#if !defined(AFX_WATCH_H__27657E32_73FD_4790_A5DE_70D7938A595A__INCLUDED_)
#define AFX_WATCH_H__27657E32_73FD_4790_A5DE_70D7938A595A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWatchApp:
// See Watch.cpp for the implementation of this class
//

class CWatchApp : public CWinApp
{
public:
	CWatchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWatchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWatchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATCH_H__27657E32_73FD_4790_A5DE_70D7938A595A__INCLUDED_)
