#include "stdafx.h"
#include "ChartDataFile.h"

#include <string>

ChartDataFile::ChartDataFile()
{
}


ChartDataFile::~ChartDataFile()
{
}


// Clear chart data
void ChartDataFile::ClearData()
{
	m_ChartHeader.Clear();
	m_ChartData.Clear();
}


// Choose data file and load chart data from the file
BOOL ChartDataFile::ProcessChartFile(LPCTSTR _csFile, CStatic& _ChartArea)
{
	BOOL ret = FALSE;
	CStdioFile fileChartData;

	try
	{
		fileChartData.m_pStream = NULL;

		if (!_csFile)
		{
			ErrorBox(_T("Chart file name is not set"), _csFile);
			throw FALSE;
		}

		ClearData();

		CFileException ex;
		BOOL bRet = fileChartData.Open(_csFile, CFile::modeRead
			| CFile::typeText | CFile::shareDenyWrite
			| CFile::osSequentialScan, &ex);
		if (!bRet)
		{
			//ex.ReportError(MB_OK | MB_ICONEXCLAMATION);
			ErrorBox(_T("File open failed"), _csFile, &ex);
			throw FALSE;
		}

		bRet = ParseChartFile(fileChartData);
		if (!bRet)
		{
			throw FALSE;
		}

		bRet = DrawChart(_ChartArea);
		if (!bRet)
		{
			throw FALSE;
		}

		ret = TRUE;
	}
	catch (BOOL ex)
	{
		ret = ex;
	}

//	try
//	{
//	}
//	catch (CException* pEx)
//	{
////			ex.ReportError(MB_OK | MB_ICONEXCLAMATION);
//		ErrorBox(_T("Chart processing failed"), _csFile, pEx);
//
//		pEx->Delete();
//	}

	if (fileChartData.m_pStream)
	{
		fileChartData.Close(), fileChartData.m_pStream = NULL;
	}

	return ret;
}


// Parse chart data file
BOOL ChartDataFile::ParseChartFile(CStdioFile& _fileData)
{
	BOOL bRet;

	// Main cycle: parsing file string by string.
	// First of are trying to parse all header lines.
	// When we found a non-header line the rest of the file are chart data
	// and we can skip header checking.
	CString csStr;
	BOOL bHeaderLine = TRUE; // first line should be a header line
	while (_fileData.ReadString(csStr))
	{
		if (csStr.IsEmpty())
		{
			continue;
		}

		// if we are still parsing a header
		if (bHeaderLine)
		{
			// check next line type and parse if it is a header line
			bRet = m_ChartHeader.ParseHeaderLine(csStr, bHeaderLine);
			if (!bRet)
			{
				return FALSE;
			}

			if (bHeaderLine)
			{
				continue;
			}
		}

		// process data line
		bRet = m_ChartData.ParseDataLine(csStr);
		if (!bRet)
		{
			return FALSE;
		}
	}

	//ULONGLONG ullFileSize = _fileData.GetLength();
	//if (ullFileSize <= m_cullOnePassFileSizeLimit)
	//{
	//	// small file - process all content in a single pass
	//}
	//else
	//{
	//	// big file -
	//	//   first pass: count limits
	//	//   second pass: draw chart
	//	//		bRet = ParseChartFile(fileChartData, FALSE);
	//}

	return TRUE;
}


// Show error message box
void ChartDataFile::ErrorBox(LPCTSTR _csMsg, LPCTSTR _csFile, CException* _pEx)
{
	CString strError;
	strError.Format(_T("%s\nFile: %s"), _csMsg, _csFile);

	const size_t sizeCause = 1000; // buffer size for exception message
	CString strCause;

	if (_pEx)
	{
		LPTSTR pBuf = strCause.GetBuffer(sizeCause);
		_pEx->GetErrorMessage(pBuf, sizeCause);
		strCause.ReleaseBuffer();
		strError += _T("\nException: ") + strCause;
	}

	AfxMessageBox(strError, IDOK | MB_ICONSTOP);
}


CString ChartDataFile::GetDescription()
{
	return m_ChartHeader.GetOrganization();
}


BOOL ChartDataFile::DrawChart(CStatic& _ChartArea)
{
	// Get draw area size
	CRect rc;
	_ChartArea.GetClientRect(&rc);
	const int iAreaWidth = rc.Width();
	const int iAreaHeight = rc.Height();
	// Define axis offsets
	const int iXAxisOffset = 150;
	const int iYAxisOffset = 100;
	const int iDashSize = 5;
	const int iBorder = 15;

	// Count conversion factors
	const int iDrawWidth = iAreaWidth - (2 * iBorder + iXAxisOffset);
	const int iDrawHeight = iAreaHeight - (2 * iBorder + iYAxisOffset);
	const double dblLabelMin = m_ChartData.m_MinLimits.m_dblLabel;
	const double dblValueMin = m_ChartData.m_MinLimits.m_dblValue;
	const double dblXFactor = (double)(iDrawWidth) / (m_ChartData.m_MaxLimits.m_dblLabel - dblLabelMin);
	const double dblYFactor = (double)(iDrawHeight) / (m_ChartData.m_MaxLimits.m_dblValue - dblValueMin);

	//
	// Draw chart
	//
	CClientDC dc(&_ChartArea);
	CPen pnPenBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pnPenBlue(PS_SOLID, 1, RGB(0, 0, 0xF0));
	CPen* pOldPen = dc.SelectObject(&pnPenBlack);
	dc.FillSolidRect(rc, RGB(0xE0, 0xE0, 0xE0));

	// Draw axis
	int x_start = iBorder + iXAxisOffset;
	int y_start = iAreaHeight - (iBorder + iYAxisOffset);
	dc.MoveTo(x_start - iDashSize, y_start);
	dc.LineTo(x_start + iDrawWidth, y_start);
	dc.MoveTo(x_start, y_start + iDashSize);
	dc.LineTo(x_start, y_start - iDrawHeight);

	// Draw measurements chart
	dc.SelectObject(pnPenBlue);
//	dc.MoveTo(x_start, y_start);
	//for (int i = 0; i < w - x_start - 2; i += 3)
	//{
	//	dc.LineTo(x_start + i, y_start - int(h / 3 * (1 - sin((float)i))));
	//}
	for (vector<ChartPoint>::iterator iter = m_ChartData.m_ChartMeasures.begin();
		iter != m_ChartData.m_ChartMeasures.end(); iter++)
	{
		ChartPoint pt = *iter;
		int iX = x_start + static_cast<int>((pt.m_dblLabel - dblLabelMin) * dblXFactor);
		int iY = y_start - static_cast<int>((pt.m_dblValue - dblValueMin) * dblYFactor);

		if (iter == m_ChartData.m_ChartMeasures.begin())
		{
			dc.MoveTo(iX, iY);
		}
		else
		{
			dc.LineTo(iX, iY);
		}
	}

	//int cPoints = 1;
	//for (int i = 3; i < iAreaWidth - x_start - 2; i += 10)
	//{
	//	cPoints++;
	//}
	//POINT* pptPoints = new POINT[cPoints];
	//int nPoint = 0;
	//pptPoints[nPoint++] = { x_start, y_start };
	//for (int i = 3; i < iAreaWidth - x_start - 2; i += 10)
	//{
	//	pptPoints[nPoint++] = { x_start + i, y_start - int(iAreaHeight / 3 * (1 - sin((float)i))) };
	//}

	//BOOL bRet = dc.Polyline(pptPoints, cPoints);

	//delete[] pptPoints;

	dc.SelectObject(pOldPen);

	return TRUE;
}
