
// OroliaGraphTestDlg.h : header file
//

#pragma once

#include "ChartDataFile.h"

// COroliaGraphTestDlg dialog
class COroliaGraphTestDlg : public CDialogEx
{
// Construction
public:
	COroliaGraphTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OROLIAGRAPHTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	ChartDataFile m_ChartDataFile;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonProcessChartFile();
	CString m_csDataDesc;
};
