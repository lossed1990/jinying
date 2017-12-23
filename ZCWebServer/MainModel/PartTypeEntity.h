#pragma once
#include <string>
using std::string;

//Åä¼şÀà±ğ
class TPartTypeEntity
{
public:
	TPartTypeEntity(string& strName);
	~TPartTypeEntity();

public:
	string GetName() { return m_strName; }

private:
	string m_strName;
};


