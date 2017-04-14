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
	BOOL ProcessChartFile(LPCTSTR _csFile, BOOL _bUseMemory, CDC& _dcTarget);
	BOOL DrawChart(CStdioFile& _FileData, CDC& _dcTarget);
	CString GetDescription();

private:
	const ULONGLONG m_cullOnePassFileSizeLimit = 10 * 1024 * 1024; // 10 MB
	CChartHeader m_ChartHeader;
	CChartData m_ChartData;
	// File position where measurement data starts
	ULONGLONG m_ullDataFilePos;

private:
	// Parse chart data file
	BOOL ParseChartFile(CStdioFile& _FileData, BOOL _bUseMemory);
	// Show exception error
	void ErrorBox(LPCTSTR _strMsg, CException* _pEx = NULL);
	void DrawPoint(
		CDC& _dc,
		ChartPoint& _Point,
		IN OUT BOOL& _bFirstPoint,
		int _iXBase,
		int _iYBase,
		double _dblLabelMin,
		double _dblValueMin,
		double _dblXFactor,
		double _dblYFactor);
};

