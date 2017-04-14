
// OroliaGraphTestDlg.h : header file
//

#pragma once

#include "ChartDataFile.h"
#include "afxwin.h"
#include "atltypes.h"

DWORD WINAPI ProcessChartThread(LPVOID _lpParam);

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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_csDataDesc;
	CStatic m_StaticChart;
	CStatic m_StaticDesc;
	BOOL m_bLowMemoryUsage;

public:
	afx_msg void OnBnClickedButtonLoadFile();
	afx_msg void OnBnClickedCheckLessMemUsage();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	LRESULT OnSetFileTitle(WPARAM _wParam, LPARAM _lParam);
	LRESULT OnSetDescription(WPARAM _wParam, LPARAM _lParam);
	LRESULT OnUpdateChart(WPARAM _wParam, LPARAM _lParam);
	CButton m_CheckLessMemUsage;
	CButton m_ButtonLoadChart;
	void UpdateChart();
	UINT_PTR m_nThreadTimer;
	BOOL PrepareChartDC();
private:
	HANDLE m_hThread;
	// Min window size
	CSize m_SizeMin;
	CRect m_rcCurrent;
public:
	static CDC m_dcMem;
	static CMutex* m_pmxMemDC;
	static CSingleLock* m_pslMemDC;
	CBitmap m_bmpBitmap;
	static CDC& GetMemDC();
private:
	CBitmap* m_pbmpOrg;
public:
	CRect m_rcScreen;
	void RefreshChart();
	//UINT_PTR m_nChartTimer;
	HCURSOR m_hCursorOrg;
};

