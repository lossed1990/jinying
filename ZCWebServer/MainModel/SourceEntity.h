#pragma once
#include <string>

using std::string;

class TSourceEntity
{
public:
	TSourceEntity(string& strName, string& strTypeName, int nLen, int nWidth, int nKezhong, float nDunjia, int nUseType);
	~TSourceEntity();

public:
	string GetName(){ return m_strName; }
	string GetTypeName(){ return m_strTypeName; }
	int GetLen() { return m_nLen; }
	int GetWidth() { return m_nWidth; }
	int GetKezhong() { return m_nKezhong; }
	float GetDunjia() { return m_nDunjia; }
	int GetUseType() { return m_nUseType; }

public:
	int GetMaxCutCount(int nOpenLen, int nOpenWidth);

private: //匹配开纸方式
	bool CheckCut(int nLen,int nWidth, int nOpenLen, int nOpenWidth);   //测试是否开纸  nLen-切后的长度 nWidth-切后的宽度 nOpenLen-所需的长度
	//对开
	int CheckCutType2_1(int nOpenLen, int nOpenWidth);
	int CheckCutType2_2(int nOpenLen, int nOpenWidth);
	//三开
	int CheckCutType3_1(int nOpenLen, int nOpenWidth);
	int CheckCutType3_2(int nOpenLen, int nOpenWidth);
	int CheckCutType3_3(int nOpenLen, int nOpenWidth);
	//四开
	int CheckCutType4_1(int nOpenLen, int nOpenWidth);
	int CheckCutType4_2(int nOpenLen, int nOpenWidth);
	int CheckCutType4_3(int nOpenLen, int nOpenWidth);
	//五开
	int CheckCutType5_1(int nOpenLen, int nOpenWidth);
	int CheckCutType5_2(int nOpenLen, int nOpenWidth);
	int CheckCutType5_3(int nOpenLen, int nOpenWidth);
	//六开
	int CheckCutType6_1(int nOpenLen, int nOpenWidth);
	int CheckCutType6_2(int nOpenLen, int nOpenWidth);
	int CheckCutType6_3(int nOpenLen, int nOpenWidth);
	int CheckCutType6_4(int nOpenLen, int nOpenWidth);

private:
	string m_strName;            ///< 名称
	string m_strTypeName;        ///< 名称
	int m_nLen;
	int m_nWidth;
	int m_nKezhong;
	float m_nDunjia;
	int m_nUseType;
};


