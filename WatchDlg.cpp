// WatchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Watch.h"
#include "WatchDlg.h"
#define MAX_THREAD 256
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
typedef struct tagTHREADPARAM 
{
	DWORD dwIp;					// 扫描的IP
	DWORD dwPort;				// 扫描的端口
	CWatchDlg * pDlg;		// 对话框指针
	HANDLE hCopyOkEvent;		// 同步事件句柄
	HANDLE hSemaphore;			// 控制线程数的信号量
}THREADPARAM,*PTHREADPARAM;

typedef struct tagMAINTHREADPARAM 
{
	DWORD dwStartIp;			// 扫描的开始IP
	DWORD dwEndIp;				// 扫描的结束IP
	DWORD dwStartPort;			// 扫描的开始端口
	DWORD dwEndPort;			// 扫描的结束端口
	CWatchDlg * pDlg;		// 对话框指针
	HANDLE hCopyOkEvent;		// 同步事件句柄	
}MAINTHREADPARAM,*PMAINTHREADPARAM;
/////////////////////////////////////////////////////////////////////////////
// CWatchDlg dialog
DWORD WINAPI Check(LPVOID pParam);
DWORD WINAPI MainThread(LPVOID lpParam);
CWatchDlg::CWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWatchDlg)
	m_time = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_time=1;
}

void CWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWatchDlg)
	DDX_Control(pDX, IDC_IPADDRESS2, m_end);
	DDX_Control(pDX, IDC_IPADDRESS1, m_start);
	DDX_Text(pDX, IDC_EDIT2, m_time);
	DDV_MinMaxUInt(pDX, m_time, 1, 4294967295);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWatchDlg, CDialog)
	//{{AFX_MSG_MAP(CWatchDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWatchDlg message handlers

BOOL CWatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	int start,end;
	start=inet_addr("1.1.168.192");
    end=inet_addr("255.1.168.192");
	m_start.SetAddress(start);
	m_end.SetAddress(end);
	WSADATA wsaData;
	
	WSAStartup(MAKEWORD(2,2),&wsaData);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWatchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWatchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWatchDlg::OnButton1() 
{
	UpdateData();
    m_start.GetAddress(IPstart);
	m_end.GetAddress(IPend);
	SetTimer(1,m_time*60*1000,NULL);
	ShowWindow(SW_HIDE);
	
}

void CWatchDlg::OnTimer(UINT nIDEvent) 
{
	UpdateData(TRUE);
	DWORD dwStartIp;
	DWORD dwEndIp;
	m_start.GetAddress(IPstart);
	m_end.GetAddress(IPend);

	// 定义线程参数变量
	MAINTHREADPARAM Param = {0};
	HANDLE hCopyOkEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	Param.hCopyOkEvent = hCopyOkEvent;
	

	Param.dwStartIp = IPstart;
	Param.dwEndIp = IPend;
	Param.dwStartPort = 1;
	Param.dwEndPort = 100;
	Param.pDlg = this;

	// 开启主线程
	CreateThread(NULL,0,MainThread,&Param,0,NULL);	
	WaitForSingleObject(hCopyOkEvent,INFINITE);
	ResetEvent(hCopyOkEvent);
	
}
DWORD WINAPI Check(LPVOID pParam)
{
	THREADPARAM Param = {0};
	MoveMemory(&Param,pParam,sizeof(Param));
	SetEvent(Param.hCopyOkEvent);
	SOCKADDR_IN sockAddr;
	SOCKET sock;

	// 创建面向连接的socket
	sock =socket(PF_INET,SOCK_STREAM,0); 
	// 使用TCP/IP协议
	sockAddr.sin_family=AF_INET; 
	// 指定连接的端口
	sockAddr.sin_port=htons(Param.dwPort);
	// 指定连接的IP地址
	sockAddr.sin_addr.S_un.S_addr= htonl(Param.dwIp);

	char * szIp = inet_ntoa(sockAddr.sin_addr);
	CString str;
	// 发起连接
	if(connect(sock,(LPSOCKADDR)&sockAddr,sizeof(sockAddr)) == 0)
	{		
		// 连接成功			
		CTime tm = CTime::GetCurrentTime();
		str.Format("发现IP：%s 端口 %d",szIp,Param.dwPort);
        MessageBox(NULL,str,"Watch",0);
	}
	
	ReleaseSemaphore(Param.hSemaphore,1,NULL);
	closesocket(sock);
	return 0;	
}
DWORD WINAPI MainThread(LPVOID lpParam)
{
	MAINTHREADPARAM Param = {0};
	MoveMemory(&Param,lpParam,sizeof(Param));
	SetEvent(Param.hCopyOkEvent);

	// 定义线程参数变量
	THREADPARAM ThreadParam = {0};
	HANDLE hCopyOkEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	ThreadParam.hCopyOkEvent = hCopyOkEvent;
	HANDLE hSemaphore = CreateSemaphore(NULL,MAX_THREAD,MAX_THREAD,NULL);
	ThreadParam.hSemaphore = hSemaphore;
	// 第二步 循环connect
	for (DWORD dwIp = Param.dwStartIp;dwIp <= Param.dwEndIp;dwIp++)
	{
		for (DWORD dwPort = Param.dwStartPort;dwPort <= Param.dwEndPort;dwPort++)
		{
			DWORD dwWaitRes = WaitForSingleObject(hSemaphore,200);
			if (dwWaitRes == WAIT_OBJECT_0)
			{
				ThreadParam.dwIp = dwIp;
				ThreadParam.dwPort = dwPort;
				ThreadParam.pDlg = Param.pDlg;
				CreateThread(NULL,0,Check,&ThreadParam,0,NULL);	
				WaitForSingleObject(hCopyOkEvent,INFINITE);
				ResetEvent(hCopyOkEvent);
			}
			else if	(dwWaitRes == WAIT_TIMEOUT)
			{
				// 线程数已经用完
				dwPort--;
				continue;
			}		
		}
	}

	return 0;
}