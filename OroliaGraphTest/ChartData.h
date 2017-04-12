#pragma once
#include <vector>

using std::vector;

struct ChartPoint
{
	double m_dblLabel;
	double m_dblValue;
};

class CChartData
{
public:
	CChartData();
	~CChartData();
	void Clear();
	BOOL ParseDataLine(CString& _csStr);
public:
	// Minimum values of data fields
	ChartPoint m_MinLimits;
	// Maximum values of data fields
	ChartPoint m_MaxLimits;
	// Chart data measures container
	vector<ChartPoint> m_ChartMeasures;
private:
	// If limits are already present
	BOOL bLimitsDefined;
};

