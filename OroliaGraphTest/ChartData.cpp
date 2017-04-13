#include "stdafx.h"
#include "ChartData.h"


CChartData::CChartData()
: bLimitsDefined(FALSE)
, m_bStoreDataInMemory(FALSE)
{
}


CChartData::~CChartData()
{
}


void CChartData::Clear()
{
	bLimitsDefined = FALSE;
	m_ChartMeasures.clear();
}


BOOL CChartData::ParseDataLine(CString& _csStr, ChartPoint& _Point)
{
	if (_csStr.IsEmpty())
	{
		return TRUE;
	}

	double dblLabel = 0, dblValue = 0;
	int iRet = _stscanf_s(_csStr, _T("%lf %lf"), &dblLabel, &dblValue);
	if (2 != iRet) // 2 values should be parsed
	{
		CString csErr = _T("Invalid chart data string: ") + _csStr;
		AfxMessageBox(csErr, IDOK | MB_ICONSTOP);
		return FALSE;
	}

	_Point = { dblLabel, dblValue };

	if (m_bStoreDataInMemory)
	{
		m_ChartMeasures.push_back(_Point);
	}

	return TRUE;
}


BOOL CChartData::ParseDataLineAndCollectLimits(CString& _csStr)
{
	ChartPoint point;
	BOOL bRet = ParseDataLine(_csStr, point);
	if (!bRet)
	{
		return FALSE;
	}

	// update limits
	if (!bLimitsDefined)
	{
		m_MinLimits = point;
		m_MaxLimits = point;
		bLimitsDefined = TRUE;
	}
	else
	{
		// Label limits updating may be ommited since all data lines are sorted by label.
		// But let's keep it just to be sure that we have real label limits for possible unsorted files.
		if (point.m_dblLabel < m_MinLimits.m_dblLabel)
		{
			m_MinLimits.m_dblLabel = point.m_dblLabel;
		}
		if (point.m_dblLabel > m_MaxLimits.m_dblLabel)
		{
			m_MaxLimits.m_dblLabel = point.m_dblLabel;
		}

		if (point.m_dblValue < m_MinLimits.m_dblValue)
		{
			m_MinLimits.m_dblValue = point.m_dblValue;
		}
		if (point.m_dblValue > m_MaxLimits.m_dblValue)
		{
			m_MaxLimits.m_dblValue = point.m_dblValue;
		}
	}

	return TRUE;
}
