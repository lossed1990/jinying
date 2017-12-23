#pragma once
#include <string>
using std::string;

class TSourceTypeEntity
{
public:
	TSourceTypeEntity(string& strName);
	~TSourceTypeEntity();

public:
	string GetName() { return m_strName; }

private:
	string m_strName;
};


