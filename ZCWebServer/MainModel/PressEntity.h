#pragma once
#include <string>

using std::string;

class TPressEntity
{
public:
	TPressEntity(string& strName, int nLen, int nWidth);
	~TPressEntity();

public:
	string GetName() { return m_strName; }
	int GetLen() { return m_nLen; }
	int GetWidth() { return m_nWidth; }

private:
	string m_strName;
	int m_nLen;
	int m_nWidth;
};


