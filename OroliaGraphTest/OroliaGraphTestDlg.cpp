
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



COroliaGraphTestDlg::COroliaGraphTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COroliaGraphTestDlg::IDD, pParent)
	, m_csDataDesc(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COroliaGraphTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DATA_DESC, m_csDataDesc);
	DDX_Control(pDX, IDC_STATIC_CHART, m_StaticChart);
}

BEGIN_MESSAGE_MAP(COroliaGraphTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATA, &COroliaGraphTestDlg::OnBnClickedButtonProcessChartFile)
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

	// TODO: Add extra initialization here

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


void COroliaGraphTestDlg::OnBnClickedButtonProcessChartFile()
{
	m_ChartDataFile.ClearData();

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
		return;
	}

	CString strChartFilePath = fileDlg.GetPathName();
	if (strChartFilePath.IsEmpty())
	{
		return;
	}

	// Set window title to processing chart title
	SetWindowText(fileDlg.GetFileTitle());

	// Set wait cursor
	HCURSOR hCursorWait = LoadCursor(NULL, IDC_WAIT);
	HCURSOR hCursorOrg = SetCursor(hCursorWait);

	// Process chosen chart data file (read data and draw chart)
	BOOL bSuccess = m_ChartDataFile.ProcessChartFile(strChartFilePath, m_StaticChart);

	// Restore cursor
	SetCursor(hCursorOrg);

	// Set window title to processed chart title or clear on error
	SetWindowText(bSuccess ? fileDlg.GetFileTitle() : _T(""));

	if (bSuccess)
	{
		m_csDataDesc = m_ChartDataFile.GetDescription();
		UpdateData(FALSE);
	}
}
