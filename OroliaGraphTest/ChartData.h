#pragma once
class CChartData
{
public:
	CChartData();
	~CChartData();
	void Clear();
	BOOL ParseDataLine(CString& _csStr);
};

