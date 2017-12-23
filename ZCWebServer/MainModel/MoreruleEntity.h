#pragma once
#include <string>

using std::string;

//出数规则
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

	string GetConditionString();   //获取条件表达式

private:
	int m_nBegin;     
	int m_nEnd;
	int m_nCount;

	bool m_bPress;     //是否覆膜  （影响出数规则）
	bool m_bTangjin;   //是否烫金  （影响出数规则）
};


