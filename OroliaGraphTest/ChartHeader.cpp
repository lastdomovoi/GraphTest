#include "stdafx.h"
#include "ChartHeader.h"

CChartHeader::CChartHeader()
: m_csHeaderPrefix(_T("#"))
{
	Clear();

	m_sizeHeaderPrefix = m_csHeaderPrefix.GetLength();
}


CChartHeader::~CChartHeader()
{
}


// Clear all data
void CChartHeader::Clear()
{
	m_bFilled = FALSE;
	m_CurHeaderId = OrgAndApp;
	m_csOrganization.Empty();
	m_csApplication.Empty();
	m_csMeasurementType.Empty();
	m_csStartTime.Empty();
	m_csMeasuringTime.Empty();
	m_csSingle.Empty();
	m_csInputA.Empty();
	m_csFilter.Empty();
	m_csInputB.Empty();
	m_csCommon.Empty();
	m_csExtArm.Empty();
	m_csRefOsc.Empty();
	m_csHoldOff.Empty();
	m_csUnknown.Empty();
}


// Parsing chart file string and store header data
// _csStr - string to parse
// _bHeaderLine - set to TRUE if header line detected, FALSE otherwise
// return TRUE on success, FALSE on any failure.
BOOL CChartHeader::ParseHeaderLine(CString& _csStr)
{
	BOOL ret = FALSE;
	CString csErr;

	if (_csStr.IsEmpty())
	{
		return TRUE;
	}

	try
	{
		// check if header prefix present
		CString csStr = _csStr.Left(m_sizeHeaderPrefix);
		BOOL bHeaderLine = (0 == m_csHeaderPrefix.Compare(csStr));
		if (!bHeaderLine)
		{
			csErr.Format(_T("Not a header line: %s"), (LPCTSTR)_csStr);
			throw csErr;
		}

		// cut prefix
		csStr = _csStr.Mid(m_sizeHeaderPrefix).Trim();

		// Header format:
		//
		// # Pendulum Instruments AB, TimeView32 V1.01
		// # FREQUENCY A
		// # Fri Nov 14 15:25 : 48 2003
		// # Measuring time : 100 us                      Single : Off
		// # Input A : Auto, 1Mê, AC, X1, Pos             Filter : Off
		// # Input B : Auto, 1Mê, DC, X1, Pos             Common : On
		// # Ext.arm: Chan E, Pos  Dly : Time             Ref.osc : Internal
		// # Hold off : Off                               Statistics : Off

		CString csField;
		int iPos;
		switch (m_CurHeaderId)
		{
		case OrgAndApp:
			iPos = csStr.Find(_T(","));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (organization/application string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csOrganization = csStr.Left(iPos).Trim();
				m_csApplication = csStr.Mid(iPos + 1).Trim();
			}
			break;
		case MeasType:
			m_csMeasurementType = csStr;
			break;
		case StartTime:
			m_csStartTime = csStr;
			break;
		case MeasTimeAndSingle:
			iPos = csStr.Find(_T("Single"));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (Measuring time/Single string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csMeasuringTime = csStr.Left(iPos).Trim();
				m_csSingle = csStr.Mid(iPos).Trim();
				break;
			}
		case InputAAndFilter:
			iPos = csStr.Find(_T("Filter"));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (Input A/Filter string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csInputA = csStr.Left(iPos).Trim();
				m_csFilter = csStr.Mid(iPos).Trim();
				break;
			}
		case InputBAndCommon:
			iPos = csStr.Find(_T("Common"));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (Input B/Common string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csInputB = csStr.Left(iPos).Trim();
				m_csCommon = csStr.Mid(iPos).Trim();
				break;
			}
		case ExtArmAndRefOsc:
			iPos = csStr.Find(_T("Ref.osc"));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (Ext.arm/Ref.osc string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csExtArm = csStr.Left(iPos).Trim();
				m_csRefOsc = csStr.Mid(iPos).Trim();
				break;
			}
		case HoldOffAndStats:
			iPos = csStr.Find(_T("Statistics"));
			if (-1 == iPos)
			{
				csErr.Format(_T("Invalid header (Hold off/Statistics string invalid: %s)"), (LPCTSTR)_csStr);
				throw csErr;
			}
			else
			{
				m_csHoldOff = csStr.Left(iPos).Trim();
				m_csStatistics = csStr.Mid(iPos).Trim();
				break;
			}

		}

		if (HoldOffAndStats == m_CurHeaderId)
		{
			// Full header is successfully filled
			m_bFilled = TRUE;
		}
		else
		{
			m_CurHeaderId = static_cast<HeaderId>(static_cast<int>(m_CurHeaderId)+1);
		}

		ret = TRUE;
	}
	catch (CString csErr)
	{
		AfxMessageBox(csErr, IDOK | MB_ICONERROR);
		ret = FALSE;
	}

	return ret;
}


CString CChartHeader::GetDescription()
{
	CString csStr;
	csStr.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s"), 
		(LPCTSTR)m_csOrganization,
		(LPCTSTR)m_csApplication,
		(LPCTSTR)m_csMeasurementType,
		(LPCTSTR)m_csStartTime,
		(LPCTSTR)m_csMeasuringTime,
		(LPCTSTR)m_csSingle,
		(LPCTSTR)m_csInputA,
		(LPCTSTR)m_csFilter,
		(LPCTSTR)m_csInputB,
		(LPCTSTR)m_csCommon,
		(LPCTSTR)m_csExtArm,
		(LPCTSTR)m_csRefOsc,
		(LPCTSTR)m_csHoldOff,
		(LPCTSTR)m_csStatistics,
		(LPCTSTR)m_csUnknown
		);

	return csStr;
}


// Check if header is already filled with correct data
BOOL CChartHeader::IsFilled()
{
	return m_bFilled;
}
