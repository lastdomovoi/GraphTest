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
	m_csOrganization.Empty();
	m_csApplication.Empty();
	m_csMeasurementType.Empty();
	m_uiAppVerMajor = 0;
	m_uiAppVerMinor = 0;
	m_csMeasuringTime.Empty();
	m_csSingle.Empty();
	m_csInputA.Empty();
	m_csFilter.Empty();
	m_csInputB.Empty();
	m_csCommon.Empty();
	m_csExtArm.Empty();
	m_csRefOsc.Empty();
	m_csHoldOff.Empty();
}


// Parsing chart file string and store header data
// _csStr - string to parse
// _bHeaderLine - set to TRUE if header line detected, FALSE otherwise
// return TRUE on success, FALSE on any failure.
BOOL CChartHeader::ParseHeaderLine(CString& _csStr, OUT BOOL& _bHeaderLine)
{
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

	if (_csStr.IsEmpty())
	{
		_bHeaderLine = FALSE;
		return TRUE;
	}

	// check if header prefix present
	CString csStr = _csStr.Left(m_sizeHeaderPrefix);
	_bHeaderLine = (0 == m_csHeaderPrefix.Compare(csStr));
	if (!_bHeaderLine)
	{
		// not a header line - finish
		return TRUE;
	}

	// cut prefix
	csStr = _csStr.Mid(m_sizeHeaderPrefix);
	csStr.TrimLeft();
	csStr.TrimRight();

	m_csOrganization += csStr + _T("\n");	// TODO: Parse header line

	return TRUE;
}


CString CChartHeader::GetOrganization()
{
	return m_csOrganization;
}
