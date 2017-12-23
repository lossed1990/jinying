#include "MoreruleEntity.h"

TMoreruleEntity::TMoreruleEntity(bool bPress, bool bTangjin, int nBegin, int nEnd, int nCount)
{
	m_nBegin = nBegin;
	m_nEnd = nEnd;
	m_nCount = nCount;

	m_bPress = bPress;     
	m_bTangjin = bTangjin; 
}

TMoreruleEntity::~TMoreruleEntity()
{

}

string TMoreruleEntity::GetConditionString()
{
	string strIsPress = GetIsPress() ? "印刷" : "不印刷";
	string strIsTangjin = GetIsTangjin() ? "烫金" : "不烫金";
	string strReturn = strIsPress + "+" + strIsTangjin;
	return strReturn;
}
