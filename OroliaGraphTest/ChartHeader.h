#pragma once
class CChartHeader
{
public:
	CChartHeader();
	~CChartHeader();
	// Clear all data
	void Clear();
	// Parsing chart file header
	BOOL ParseHeaderLine(CString& _csStr, OUT BOOL& _bHeaderLine);

private:
	// Header string prefix ("#")
	const CString m_csHeaderPrefix;
	size_t m_sizeHeaderPrefix;
	// Organiztion name
	CString m_csOrganization;
	// Application name
	CString m_csApplication;
	// Major application version
	UINT m_uiAppVerMajor;
	// Minor application version
	UINT m_uiAppVerMinor;
	CString m_csMeasurementType;
	// Start time of measurement
	time_t m_tStartTime;
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
public:
	CString GetOrganization();
};

