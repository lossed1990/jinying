#pragma once
#include <string>

using std::string;

//出数规则
class TPartEntity
{
public:
	TPartEntity(string& strName, string& strTypeName, float nPrice);
	~TPartEntity();

public:
	string GetName() { return m_strName; }
	string GetTypeName() { return m_strTypeName; }
	float GetPrice() { return m_nPrice; }

private:
	string m_strName;
	string m_strTypeName;
	float m_nPrice;
};


