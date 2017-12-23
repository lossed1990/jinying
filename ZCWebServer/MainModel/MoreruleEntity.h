#pragma once
#include <string>

using std::string;

//��������
class TMoreruleEntity
{
public:
	TMoreruleEntity(bool bPress,bool bTangjin,int nBegin,int nEnd,int nCount);
	~TMoreruleEntity();

public:
	int GetBegin() { return m_nBegin; }
	int GetEnd() { return m_nEnd; }
	int GetCount() { return m_nCount; }
	bool GetIsPress() { return m_bPress; }
	bool GetIsTangjin() { return m_bTangjin; }

	string GetConditionString();   //��ȡ�������ʽ

private:
	int m_nBegin;     
	int m_nEnd;
	int m_nCount;

	bool m_bPress;     //�Ƿ�Ĥ  ��Ӱ���������
	bool m_bTangjin;   //�Ƿ��̽�  ��Ӱ���������
};


