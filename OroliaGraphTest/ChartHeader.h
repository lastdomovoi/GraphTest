#pragma once

enum HeaderId
{
	OrgAndApp,
	MeasType,
	StartTime,
	MeasTimeAndSingle,
	InputAAndFilter,
	InputBAndCommon,
	ExtArmAndRefOsc,
	HoldOffAndStats,
};

class CChartHeader
{
public:
	CChartHeader();
	~CChartHeader();
	// Clear all data
	void Clear();
	// Parsing chart file header
	BOOL ParseHeaderLine(CString& _csStr);

private:
	// Header string prefix ("#")
	const CString m_csHeaderPrefix;
	HeaderId m_CurHeaderId;
	size_t m_sizeHeaderPrefix;
	// Organiztion name
	CString m_csOrganization;
	// Application name
	CString m_csApplication;
	// Measurement
	CString m_csMeasurementType;
	// Start time of measurement
	CString m_csStartTime;
	// Measuring time
	CString m_csMeasuringTime;
	// Single
	CString m_csSingle;
	// Input A
	CString m_csInputA;
	// Filter
	CString m_csFilter;
	// Input B
	CString m_csInputB;
	// Common
	CString m_csCommon;
	// Ext.arm
	CString m_csExtArm;
	// Ref.osc
	CString m_csRefOsc;
	// Hold off
	CString m_csHoldOff;
	// Statistics
	CString m_csStatistics;
	// Unknown
	CString m_csUnknown;
public:
	CString GetDescription();
	// Check if header is already filled with correct data
	BOOL IsFilled();
private:
	bool m_bFilled;
};

