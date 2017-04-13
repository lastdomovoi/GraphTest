
// OroliaGraphTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OroliaGraphTest.h"
#include "OroliaGraphTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnStnClickedMainframe();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_STN_CLICKED(IDR_MAINFRAME, &CAboutDlg::OnStnClickedMainframe)
END_MESSAGE_MAP()


// COroliaGraphTestDlg dialog


CDC COroliaGraphTestDlg::m_dcMem;


COroliaGraphTestDlg::COroliaGraphTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COroliaGraphTestDlg::IDD, pParent)
	, m_csDataDesc(_T(""))
	, m_bLowMemoryUsage(FALSE)
	, m_nThreadTimer(NULL)
	, m_SizeMin(0, 0)
	, m_hThread(NULL)
	, m_rcScreen(0, 0, 0, 0)
	//, m_nChartTimer(0)
	, m_hCursorOrg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COroliaGraphTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DATA_DESC, m_csDataDesc);
	DDX_Control(pDX, IDC_STATIC_CHART, m_StaticChart);
	DDX_Control(pDX, IDC_STATIC_DATA_DESC, m_StaticDesc);
	DDX_Check(pDX, IDC_CHECK_LOW_MEM_USAGE, m_bLowMemoryUsage);
	DDX_Control(pDX, IDC_BUTTON_LOAD_CHART, m_ButtonLoadChart);
	DDX_Control(pDX, IDC_CHECK_LOW_MEM_USAGE, m_CheckLowMemUsage);
}

BEGIN_MESSAGE_MAP(COroliaGraphTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATA, &COroliaGraphTestDlg::OnBnClickedButtonLoadFile)
	//ON_STN_CLICKED(IDC_STATIC_DATA_DESC, &COroliaGraphTestDlg::OnStnClickedStaticDataDesc)
	ON_BN_CLICKED(IDC_CHECK_LOW_MEM_USAGE, &COroliaGraphTestDlg::OnBnClickedCheckLowMemUsage)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_SET_FILE_TITLE, &COroliaGraphTestDlg::OnSetFileTitle)
	ON_MESSAGE(WM_SET_DESCRIPTION, &COroliaGraphTestDlg::OnSetDescription)
	ON_MESSAGE(WM_UPDATE_CHART, &COroliaGraphTestDlg::OnUpdateChart)
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COroliaGraphTestDlg message handlers

BOOL COroliaGraphTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if ((0 == m_SizeMin.cx) && (0 == m_SizeMin.cy))
	{
		CRect rcDlg;
		GetWindowRect(rcDlg);
		m_SizeMin = rcDlg.Size();
	}

	m_rcScreen = CRect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));

	BOOL bRet = PrepareChartDC();
	if (!bRet)
	{
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COroliaGraphTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COroliaGraphTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();

		CPaintDC dc(&m_StaticChart);
		CRect rc;
		m_StaticChart.GetClientRect(&rc);

		BOOL bRet = dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &m_dcMem,
			0, 0, m_rcScreen.Width(), m_rcScreen.Height(), SRCCOPY);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COroliaGraphTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CAboutDlg::OnStnClickedMainframe()
//{
//	// TODO: Add your control notification handler code here
//}

void COroliaGraphTestDlg::OnBnClickedButtonLoadFile()
{
	m_ButtonLoadChart.EnableWindow(FALSE);

	m_hThread = ::CreateThread(NULL, 0, &(ProcessChartThread), (LPVOID)GetSafeHwnd(), 0, NULL);
	if (!m_hThread)
	{
		AfxMessageBox(_T("Failed to create thread"), IDOK, MB_ICONERROR);
	}

	m_nThreadTimer = SetTimer(TIMER_CHECK_THREAD, 1 * 1000, NULL);
	if (!m_nThreadTimer)
	{
		AfxMessageBox(_T("SetTimer failed"), IDOK | MB_ICONERROR);
	}
}


void COroliaGraphTestDlg::OnBnClickedCheckLowMemUsage()
{
	UpdateData(TRUE);
}


void COroliaGraphTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) && (0 == cy))
	{
		return;
	}

	const LONG lDX = cx - m_rcCurrent.Width();
	const LONG lDY = cy - m_rcCurrent.Height();
	CRect rc;

	if (m_StaticChart.m_hWnd)
	{
		m_StaticChart.GetWindowRect(rc);
		rc.right += lDX;
		rc.bottom += lDY;
		ScreenToClient(rc);
		m_StaticChart.MoveWindow(rc);
	}

	if (m_StaticDesc.m_hWnd)
	{
		m_StaticDesc.GetWindowRect(rc);
		rc.left += lDX;
		rc.right += lDX;
		rc.bottom += lDY;
		ScreenToClient(rc);
		m_StaticDesc.MoveWindow(rc);
	}

	if (m_CheckLowMemUsage.m_hWnd)
	{
		m_CheckLowMemUsage.GetWindowRect(rc);
		rc.left += lDX;
		rc.right += lDX;
		rc.top += lDY;
		rc.bottom += lDY;
		ScreenToClient(rc);
		m_CheckLowMemUsage.MoveWindow(rc);
	}

	if (m_ButtonLoadChart.m_hWnd)
	{
		m_ButtonLoadChart.GetWindowRect(rc);
		rc.left += lDX;
		rc.right += lDX;
		rc.top += lDY;
		rc.bottom += lDY;
		ScreenToClient(rc);
		m_ButtonLoadChart.MoveWindow(rc);
	}

	GetClientRect(&m_rcCurrent);

	//if (m_dcMem.GetSafeHdc())
	//{
	//	m_dcMem.DeleteDC();
	//}

	//RefreshChart();
}


void COroliaGraphTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();

	if (m_hThread)
	{
		DWORD dwWait = ::WaitForSingleObject(m_hThread, 3 * 1000);
		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			::TerminateThread(m_hThread, (DWORD)-1);
			break;
		}

		m_hThread = NULL;
	}
}


void COroliaGraphTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (TIMER_CHECK_THREAD == nIDEvent)
	{
		if (m_hThread)
		{
			DWORD dwExitCode = 0;
			BOOL bRet = ::GetExitCodeThread(m_hThread, &dwExitCode);
			if (bRet)
			{
				if (STILL_ACTIVE != dwExitCode)
				{
					m_ButtonLoadChart.EnableWindow(TRUE);
					KillTimer(m_nThreadTimer), m_nThreadTimer = NULL;
					//KillTimer(m_nChartTimer), m_nChartTimer = NULL;

					// Restore cursor
					if (m_hCursorOrg)
					{
						//SetCursor(m_hCursorOrg);
						SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)m_hCursorOrg);
					}
				}
			}
		}
	}
	//else if (TIMER_UPDATE_CHART == nIDEvent)
	//{
	//	RefreshChart();
	//}

	CDialogEx::OnTimer(nIDEvent);
}


LRESULT COroliaGraphTestDlg::OnSetFileTitle(WPARAM _wParam, LPARAM _lParam)
{
	CString* pcsTitle = (CString*)_lParam;
	SetWindowText(pcsTitle ? *pcsTitle : _T(""));

	m_dcMem.FillSolidRect(m_rcScreen, RGB(0xE0, 0xE0, 0xE0));
	
	return LRESULT();
}


LRESULT COroliaGraphTestDlg::OnSetDescription(WPARAM _wParam, LPARAM _lParam)
{
	CString* pcsDesc = (CString*)_lParam;
	m_csDataDesc = pcsDesc ? *pcsDesc : _T("");
	UpdateData(FALSE);

	// Set wait cursor
	HCURSOR hCursorWait = LoadCursor(NULL, IDC_WAIT);
//	m_hCursorOrg = SetCursor(hCursorWait);
	m_hCursorOrg = GetCursor();
	SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)hCursorWait);

	//m_nChartTimer = SetTimer(TIMER_UPDATE_CHART, 500, NULL);

	return LRESULT();
}

LRESULT COroliaGraphTestDlg::OnUpdateChart(WPARAM _wParam, LPARAM _lParam)
{
	RefreshChart();

	return LRESULT();
}


void COroliaGraphTestDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialogEx::OnGetMinMaxInfo(lpMMI);

	if (lpMMI->ptMinTrackSize.x < m_SizeMin.cx)
	{
		lpMMI->ptMinTrackSize.x = m_SizeMin.cx;
	}
	if (lpMMI->ptMinTrackSize.y < m_SizeMin.cy)
	{
		lpMMI->ptMinTrackSize.y = m_SizeMin.cy;
	}
}


void COroliaGraphTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_nThreadTimer)
	{
		KillTimer(m_nThreadTimer);
	}

	if (m_pbmpOrg)
	{
		m_dcMem.SelectObject(m_pbmpOrg);
	}
	m_dcMem.DeleteDC();
}


DWORD WINAPI ProcessChartThread(LPVOID _lpParam)
{
	DWORD ret = 0;
	ChartDataFile* pChartDataFile = NULL;

	HWND hDlgWnd = (HWND)_lpParam;
	if (!hDlgWnd)
	{
		AfxMessageBox(_T("Invalid dialog hWND"));
		return (DWORD)-1;
	}

	try
	{
		BOOL bLowMemUsage = FALSE;
		COroliaGraphTestDlg* pDlg = static_cast<COroliaGraphTestDlg*>(CWnd::FromHandle(hDlgWnd));
		if (pDlg)
		{
			CButton* pCheckLowMemUsage = static_cast<CButton*>(pDlg->GetDlgItem(IDC_CHECK_LOW_MEM_USAGE));
			if (pCheckLowMemUsage)
			{
				bLowMemUsage = BST_CHECKED == pCheckLowMemUsage->GetCheck();
			}
		}

		pChartDataFile = new ChartDataFile;
		if (!pChartDataFile)
		{
			ret = (DWORD)-1;
			throw _T("ChartDataFile creation failed");
		}

		//m_ChartDataFile.ClearData();

		//
		// Choose chart data file
		//
		CFileDialog fileDlg(
			TRUE,
			_T("*.rsd|*.ssd"),
			NULL,
			OFN_FILEMUSTEXIST,
			_T("Chart files (*.rsd;*.ssd)|*.rsd; *.ssd|All files (*.*)|*.*||"));
		if (IDOK != fileDlg.DoModal())
		{
			ret = 0;
			throw ret;
		}

		CString strChartFilePath = fileDlg.GetPathName();
		if (strChartFilePath.IsEmpty())
		{
			ret = (DWORD)-1;
			throw _T("Empty chart file path");
		}

		// Set window title to processing chart title
		CString* pcsTitle = new CString(fileDlg.GetFileTitle());
		if (pcsTitle)
		{
			PostMessage(hDlgWnd, WM_SET_FILE_TITLE, NULL, (LPARAM)pcsTitle);
		}

		// Process chosen chart data file (read data and draw chart)
		BOOL bRet = pChartDataFile->ProcessChartFile(strChartFilePath, !bLowMemUsage, pDlg->GetMemDC());
		if (!bRet)
		{
			throw (DWORD)-1;
		}

		PostMessage(hDlgWnd, WM_UPDATE_CHART, NULL, NULL);
		ret = 0;
	}
	catch (CString csErr)
	{
		if (!csErr.IsEmpty())
		{
			AfxMessageBox(csErr, IDOK | MB_ICONERROR);
		}
	}
	catch (DWORD dwRet)
	{
		ret = dwRet;
	}

	if (0 != ret)
	{
		// error
		CString* pcsTitle = new CString(_T(""));
		if (pcsTitle)
		{
			PostMessage(hDlgWnd, WM_SET_FILE_TITLE, NULL, (LPARAM)pcsTitle);
		}
	}

	return ret;
}


BOOL COroliaGraphTestDlg::PrepareChartDC()
{
	BOOL ret = FALSE;

	try
	{
		CClientDC dc(this);

		BOOL bRet = m_dcMem.CreateCompatibleDC(&dc);
		if (!bRet)
		{
			ret = FALSE;
			throw _T("Failed to create compatible DC");
		}

		m_bmpBitmap.CreateCompatibleBitmap(&dc, m_rcScreen.Width(), m_rcScreen.Height());
		m_pbmpOrg = m_dcMem.SelectObject(&m_bmpBitmap);

		//CBrush brushLtGray;
		//brushLtGray.CreateStockObject(LTGRAY_BRUSH);
		//CBrush* pOrgBrush = m_dcMem.SelectObject(&brushLtGray);
		//m_dcMem.PatBlt(0, 0, iScreenWidth, iScreenHeight, PATCOPY);
		//m_dcMem.SelectObject(pOrgBrush);
		//brushLtGray.DeleteObject();
		m_dcMem.FillSolidRect(m_rcScreen, RGB(0xE0, 0xE0, 0xE0));

		//if (pOrgBrush)
		//{
		//	m_dcMem.SelectObject(pOrgBrush);
		//}

		ret = TRUE;
	}
	catch (CString csErr)
	{
		if (!csErr.IsEmpty())
		{
			AfxMessageBox(csErr, IDOK | MB_ICONERROR);
		}
	}
	catch (BOOL bRet)
	{
		ret = bRet;
	}

	return TRUE;
}


CDC& COroliaGraphTestDlg::GetMemDC()
{
	return m_dcMem;
}


void COroliaGraphTestDlg::RefreshChart()
{
	if (m_StaticChart.m_hWnd)
	{
		//CPaintDC dc(&m_StaticChart);
		CRect rc;
		m_StaticChart.GetClientRect(&rc);

		//BOOL bRet = dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &m_dcMem,
		//	0, 0, m_rcScreen.Width(), m_rcScreen.Height(), SRCCOPY);

		//	m_StaticChart.Invalidate(TRUE);
		InvalidateRect(&rc, FALSE);
		UpdateWindow();
	}
}
