#include "stdafx.h"
#include "ChartDataFile.h"

#include <string>

ChartDataFile::ChartDataFile()
: m_csDataFile(_T(""))
{
}


ChartDataFile::~ChartDataFile()
{
}


// Clear chart data
void ChartDataFile::ClearData()
{
	m_csDataFile.Empty();
	m_ChartHeader.Clear();
	m_ChartData.Clear();
}


// Choose data file and load chart data from the file
BOOL ChartDataFile::ProcessChartFile(LPCTSTR _csFile)
{
	BOOL ret = FALSE;
	CStdioFile fileChartData;

	ClearData();

	try
	{
		if (!_csFile)
		{
			ErrorBox(_T("Chart file name is not set"), _csFile);
			throw FALSE;
		}

		CFileException ex;
		BOOL bRet = fileChartData.Open(_csFile, CFile::modeRead | CFile::typeText, &ex);
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

		// store successfully parsed file path
		m_csDataFile = _csFile;
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
