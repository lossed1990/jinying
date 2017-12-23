#pragma once
#include <string>
using std::string;

class TPressTypeEntity
{
public:
	TPressTypeEntity(string& strName);
	~TPressTypeEntity();

public:
	string GetName() { return m_strName; }

private:
	string m_strName;
};


