#pragma once
#include <string>

using std::string;

//出数规则
class TPressPriceEntity
{
public:
	TPressPriceEntity(string& strPressName, string& strPressTypeName, int nBegin, int nEnd, float nPrice);
	~TPressPriceEntity();

public:
	string GetPressName() { return m_strPressName; }
	string GetPressTypeName() { return m_strPressTypeName; }

	int GetBegin() { return m_nBegin; }
	int GetEnd() { return m_nEnd; }
	float GetPrice() { return m_nPrice; }

private:
	string m_strPressName;
	string m_strPressTypeName;
	int m_nBegin;
	int m_nEnd;
	float m_nPrice;
};


