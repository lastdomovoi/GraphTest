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
	BOOL ParseDataLineAndCollectLimits(CString& _csStr);
	BOOL ParseDataLine(CString& _csStr, ChartPoint& _Point);

public:
	// Minimum values of data fields
	ChartPoint m_MinLimits;
	// Maximum values of data fields
	ChartPoint m_MaxLimits;
	//// Chart data measures container
	vector<ChartPoint> m_ChartMeasures;
	// Store measurement data in memory
	BOOL m_bStoreDataInMemory;
private:
	// If limits are already present
	BOOL bLimitsDefined;
};

