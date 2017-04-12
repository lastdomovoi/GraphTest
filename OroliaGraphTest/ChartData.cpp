#include "stdafx.h"
#include "ChartData.h"


CChartData::CChartData()
: bLimitsDefined(FALSE)
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


BOOL CChartData::ParseDataLine(CString& _csStr)
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

	m_ChartMeasures.push_back(ChartPoint({ dblLabel, dblValue }));

	// update limits
	if (!bLimitsDefined)
	{
		m_MinLimits = { dblLabel, dblValue };
		m_MaxLimits = { dblLabel, dblValue };
		bLimitsDefined = TRUE;
	}
	else
	{
		if (dblLabel < m_MinLimits.m_dblLabel) m_MinLimits.m_dblLabel = dblLabel;
		if (dblLabel > m_MaxLimits.m_dblLabel) m_MaxLimits.m_dblLabel = dblLabel;
		if (dblValue < m_MinLimits.m_dblValue) m_MinLimits.m_dblValue = dblValue;
		if (dblValue > m_MaxLimits.m_dblValue) m_MaxLimits.m_dblValue = dblValue;
	}

	return TRUE;
}
