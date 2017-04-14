#include "stdafx.h"
#include "ChartDataFile.h"
#include "OroliaGraphTest.h"
#include "Resource.h"
#include <string>

ChartDataFile::ChartDataFile()
: m_ullDataFilePos(0)
{
	ClearData();
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
BOOL ChartDataFile::ProcessChartFile(LPCTSTR _csFile, BOOL _bUseMemory, CDC& _dcTarget)
{
	BOOL ret = FALSE;
	CStdioFile fileChartData;

	try
	{
		fileChartData.m_pStream = NULL;

		if (!_csFile)
		{
			ErrorBox(_T("Chart file name is not set"));
			throw FALSE;
		}


		ClearData();

		CFileException ex;
		BOOL bRet = fileChartData.Open(_csFile, CFile::modeRead
			| CFile::typeText | CFile::shareDenyWrite
			| CFile::osNoBuffer, &ex);
		if (!bRet)
		{
			//ex.ReportError(MB_OK | MB_ICONEXCLAMATION);
			CString csErr;
			csErr.Format(_T("File open failed\nFile: %s"), _csFile);
			ErrorBox(csErr, &ex);
			throw FALSE;
		}

		bRet = ParseChartFile(fileChartData, _bUseMemory);
		if (!bRet)
		{
			throw FALSE;
		}

		bRet = DrawChart(fileChartData, _dcTarget);
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

	if (fileChartData.m_pStream)
	{
		fileChartData.Close(), fileChartData.m_pStream = NULL;
	}

	return ret;
}


// Parse chart data file
BOOL ChartDataFile::ParseChartFile(CStdioFile& _FileData, BOOL _bUseMemory)
{
	BOOL bRet;

	m_ChartData.m_bStoreDataInMemory = _bUseMemory;

	// Main cycle: parsing file string by string.
	// First of are trying to parse all header lines.
	// When we found a non-header line the rest of the file are chart data
	// and we can skip header checking.
	CString csStr;
	ULONGLONG ullPos = 0;
	for (;;)
	{
		bRet = _FileData.ReadString(csStr);
		if (!bRet)
		{
			// EOF
			break;
		}

		if (csStr.IsEmpty())
		{
			continue;
		}

		// If we are still parsing a header
		if (!m_ChartHeader.IsFilled())
		{
			// Parse header line
			bRet = m_ChartHeader.ParseHeaderLine(csStr);
			if (!bRet)
			{
				return FALSE;
			}

			if (m_ChartHeader.IsFilled())
			{
				if (!_bUseMemory)
				{
					// store measurements data start file position
					m_ullDataFilePos = _FileData.GetPosition();
				}

				CString* pcsDesc = new CString(GetDescription());
				if (pcsDesc)
				{
					if (theApp.m_pMainWnd)
					{
						PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SET_DESCRIPTION, NULL, (LPARAM)pcsDesc);
					}
					else
					{
						delete pcsDesc, pcsDesc = NULL;
					}
				}
			}
			
			continue;
		}

		// process data line
		bRet = m_ChartData.ParseDataLineAndCollectLimits(csStr);
		if (!bRet)
		{
			return FALSE;
		}
	}

	return TRUE;
}


// Show error message box
void ChartDataFile::ErrorBox(LPCTSTR _strMsg, CException* _pEx)
{
	CString csError = _strMsg;

	if (_pEx)
	{
		const size_t sizeCause = 1000; // buffer size for exception message
		CString strCause;
		LPTSTR pBuf = strCause.GetBuffer(sizeCause);
		_pEx->GetErrorMessage(pBuf, sizeCause);
		strCause.ReleaseBuffer();
		csError += _T("\nException: ") + strCause;
	}

	AfxMessageBox(csError, IDOK | MB_ICONSTOP);
}


CString ChartDataFile::GetDescription()
{
	return m_ChartHeader.GetDescription();
}


// Read measurement data from memory or from file depending on the mode and draw chart
BOOL ChartDataFile::DrawChart(CStdioFile& _FileData, CDC& _dcTarget)
{
	BOOL ret = FALSE;
	CPen* ppnOrg = NULL;
	CPen pnPenBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pnPenBlue(PS_SOLID, 1, RGB(0, 0, 0xF0));

	try
	{
		// Get draw area size
		const int iAreaWidth = ::GetSystemMetrics(SM_CXSCREEN);
		const int iAreaHeight = ::GetSystemMetrics(SM_CYSCREEN);
		CRect rc(0, 0, iAreaWidth, iAreaHeight);

		// Define other sizes and offsets
		const int iXAxisOffset = 150;
		const int iYAxisOffset = 100;
		const int iDashSize = 10;
		const int iBorder = 30;

		// Perform some calculations
		const int iDrawWidth = iAreaWidth - (2 * iBorder + iXAxisOffset);
		const int iDrawHeight = iAreaHeight - (2 * iBorder + iYAxisOffset);
		const double dblLabelMin = m_ChartData.m_MinLimits.m_dblLabel;
		const double dblValueMin = m_ChartData.m_MinLimits.m_dblValue;
		const double dblXFactor = (double)(iDrawWidth) / (m_ChartData.m_MaxLimits.m_dblLabel - dblLabelMin);
		const double dblYFactor = (double)(iDrawHeight) / (m_ChartData.m_MaxLimits.m_dblValue - dblValueMin);

		//
		// Draw chart
		//
		_dcTarget.FillSolidRect(0, 0, iAreaWidth, iAreaHeight, RGB(0xE0, 0xE0, 0xE0));
		ppnOrg = _dcTarget.SelectObject(&pnPenBlack);

		// Draw axis
		int iXBase = iBorder + iXAxisOffset;
		int iYBase = iAreaHeight - (iBorder + iYAxisOffset);
		_dcTarget.MoveTo(iXBase - iDashSize, iYBase);
		_dcTarget.LineTo(iXBase + iDrawWidth, iYBase);
		_dcTarget.MoveTo(iXBase, iYBase + iDashSize);
		_dcTarget.LineTo(iXBase, iYBase - iDrawHeight);

		BOOL bUseMemory = m_ChartData.m_bStoreDataInMemory;
		if (!bUseMemory)
		{
			// Restore measurement data file position
			try
			{
				_FileData.Seek(m_ullDataFilePos, CFile::begin);
			}
			catch (CFileException* pEx)
			{
				ErrorBox(_T("File seek operation failed"), pEx);
				pEx->Delete();
			}
		}

		//
		// Draw measurements chart
		//
		_dcTarget.SelectObject(&pnPenBlue);

		ChartPoint point;
		BOOL bFirstPoint = TRUE;

		if (bUseMemory)
		{
			for (vector<ChartPoint>::iterator iter = m_ChartData.m_ChartMeasures.begin();
				iter != m_ChartData.m_ChartMeasures.end(); iter++)
			{
				point = *iter;

				DrawPoint(_dcTarget, point, bFirstPoint, iXBase, iYBase, dblLabelMin,
					dblValueMin, dblXFactor, dblYFactor);
			}
		}
		else
		{
			CString csStr;

			while (_FileData.ReadString(csStr))
			{
				if (csStr.IsEmpty())
				{
					continue;
				}

				BOOL bRet = m_ChartData.ParseDataLine(csStr, point);
				if (!bRet)
				{
					throw FALSE;
				}

				DrawPoint(_dcTarget, point, bFirstPoint, iXBase, iYBase, dblLabelMin,
					dblValueMin, dblXFactor, dblYFactor);
			}
		}

		ret = TRUE;
	}
	catch (BOOL bRet)
	{
		ret = bRet;
	}

	if (ppnOrg)
	{
		_dcTarget.SelectObject(ppnOrg);
	}
	pnPenBlack.DeleteObject();
	pnPenBlue.DeleteObject();

	return ret;
}


void ChartDataFile::DrawPoint(
	CDC& _dc,
	ChartPoint& _Point,
	IN OUT BOOL& _bFirstPoint,
	int _iXBase,
	int _iYBase,
	double _dblLabelMin,
	double _dblValueMin,
	double _dblXFactor,
	double _dblYFactor)
{
	int iX = _iXBase + static_cast<int>((_Point.m_dblLabel - _dblLabelMin) * _dblXFactor);
	int iY = _iYBase - static_cast<int>((_Point.m_dblValue - _dblValueMin) * _dblYFactor);

	if (_bFirstPoint)
	{
		_dc.MoveTo(iX, iY);
		_bFirstPoint = FALSE;
	}
	else
	{
		_dc.LineTo(iX, iY);
	}
}
