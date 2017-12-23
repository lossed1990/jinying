#pragma once
#include <string>

using std::string;

//µ¶Ä£
class TDieCutterEntity
{
public:
	TDieCutterEntity(string& strName,int nLen, int nWidth, int nHeight);
	~TDieCutterEntity();

public:
	string GetTypeName(){ return m_strTypeName; }
	int GetLen() { return m_nLen; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }

private:
	string m_strTypeName;
	int m_nLen;
	int m_nWidth;
	int m_nHeight;
};
