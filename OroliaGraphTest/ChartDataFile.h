#pragma once
#include "ChartHeader.h"
#include "ChartData.h"

class ChartDataFile
{
public:
	ChartDataFile();
	~ChartDataFile();
	void ClearData();
	// Choose data file and load chart data from the file
	BOOL ProcessChartFile(LPCTSTR _csFile);

private:
	const ULONGLONG m_cullOnePassFileSizeLimit = 10 * 1024 * 1024; // 10 MB
	// Chart data file
	CString m_csDataFile;
	CChartHeader m_ChartHeader;
	CChartData m_ChartData;

private:
	// Parse chart data file
	BOOL ParseChartFile(CStdioFile& _fileData);
	// Show exception error
	static void ErrorBox(LPCTSTR _csMsg, LPCTSTR _csFile, CException* _pEx = NULL);
public:
	CString GetDescription();
};

