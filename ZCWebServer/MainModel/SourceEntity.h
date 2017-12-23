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

private: //ƥ�俪ֽ��ʽ
	bool CheckCut(int nLen,int nWidth, int nOpenLen, int nOpenWidth);   //�����Ƿ�ֽ  nLen-�к�ĳ��� nWidth-�к�Ŀ�� nOpenLen-����ĳ���
	//�Կ�
	int CheckCutType2_1(int nOpenLen, int nOpenWidth);
	int CheckCutType2_2(int nOpenLen, int nOpenWidth);
	//����
	int CheckCutType3_1(int nOpenLen, int nOpenWidth);
	int CheckCutType3_2(int nOpenLen, int nOpenWidth);
	int CheckCutType3_3(int nOpenLen, int nOpenWidth);
	//�Ŀ�
	int CheckCutType4_1(int nOpenLen, int nOpenWidth);
	int CheckCutType4_2(int nOpenLen, int nOpenWidth);
	int CheckCutType4_3(int nOpenLen, int nOpenWidth);
	//�忪
	int CheckCutType5_1(int nOpenLen, int nOpenWidth);
	int CheckCutType5_2(int nOpenLen, int nOpenWidth);
	int CheckCutType5_3(int nOpenLen, int nOpenWidth);
	//����
	int CheckCutType6_1(int nOpenLen, int nOpenWidth);
	int CheckCutType6_2(int nOpenLen, int nOpenWidth);
	int CheckCutType6_3(int nOpenLen, int nOpenWidth);
	int CheckCutType6_4(int nOpenLen, int nOpenWidth);

private:
	string m_strName;            ///< ����
	string m_strTypeName;        ///< ����
	int m_nLen;
	int m_nWidth;
	int m_nKezhong;
	float m_nDunjia;
	int m_nUseType;
};


