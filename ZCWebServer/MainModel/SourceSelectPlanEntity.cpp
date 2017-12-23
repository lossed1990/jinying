#include "SourceSelectPlanEntity.h"

TSourceSelectPlanEntity::TSourceSelectPlanEntity(int nOpenLen, int nOpenWidth, int nTotalCount, int nNeedCount, TSourceEntity* pSource)
{
	m_pSource = pSource;

	m_nOpenLen = nOpenLen;
	m_nOpenWidth = nOpenWidth;
	m_nTotalCount = nTotalCount;
	m_nNeedCount = nNeedCount;
}

TSourceSelectPlanEntity::~TSourceSelectPlanEntity()
{

}

bool TSourceSelectPlanEntity::GetMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	if (m_nNeedCount > 500)
	{
		float fPrice1 = 0;
		float fPrice2 = GetPriceByMoreSize(vecLogInfo);
		bool bRet = GetPriceBySetSize(fPrice1, vecLogInfo);
		if (bRet)
		{
			fPrice = fPrice1 < fPrice2 ? fPrice1 : fPrice2;
			return true;
		}
		else
		{
			fPrice = fPrice2;
			return true;
		}
	}
	else
	{
		float fPrice1 = 0;
		bool bRet = GetPriceBySetSize(fPrice1, vecLogInfo);
		if (bRet)
		{
			fPrice = fPrice1;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool TSourceSelectPlanEntity::GetPriceBySetSize(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	int nCutCount = m_pSource->GetMaxCutCount(m_nOpenLen, m_nOpenWidth);
	if (nCutCount <= 0)
	{
		char chLogError[512] = { 0 };
		//sprintf_s(chLogError, 512, "材料成本明细>>材料名称：%s；材料克重：%d；材料吨价：%.2f；国标纸 0开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；尺寸匹配失败",
		//	m_pSource->GetName().c_str(), m_pSource->GetKezhong(), m_pSource->GetDunjia(), m_pSource->GetLen(), m_pSource->GetWidth(), m_nTotalCount);
		sprintf_s(chLogError, 512, "材料成本明细>>材料名称：%s；国标纸 0开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；尺寸匹配失败",
			m_pSource->GetName().c_str(), m_pSource->GetLen(), m_pSource->GetWidth(), m_nTotalCount);
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = chLogError;
		vecLogInfo.push_back(tInfo);
		return false;
	}

	//原材料数量总计:(m_nTotalCount / m_nCutCount)
	//原材料面积(平方米):m_pSource->GetLen() * m_pSource->GetWidth() / 1000 /1000 
	//原材料总面积:(m_nTotalCount / m_nCutCount) * (m_pSource->GetLen() * m_pSource->GetWidth() / 1000 /1000 )
	//原材料总重量（吨）:(m_nTotalCount / m_nCutCount) * (m_pSource->GetLen() * m_pSource->GetWidth() / 1000 /1000 ) * m_pSource->GetKezhong() /1000 /1000
	//总价:(m_nTotalCount / m_nCutCount) * (m_pSource->GetLen() * m_pSource->GetWidth() / 1000 /1000 ) * m_pSource->GetKezhong() /1000 /1000 * m_pSource->GetDunjia()

	//fPrice = (m_nTotalCount / m_nCutCount) * (m_pSource->GetLen() * m_pSource->GetWidth() / 1000 / 1000) * m_pSource->GetKezhong() / 1000 / 1000 * m_pSource->GetDunjia();
	fPrice = (float)m_nTotalCount * m_pSource->GetLen() * m_pSource->GetWidth() * m_pSource->GetKezhong() * m_pSource->GetDunjia() / nCutCount / 1000 / 1000 / 1000 / 1000;
	char chLog[512] = { 0 };
	//sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；材料克重：%d；材料吨价：%.2f；国标纸 %d开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
	//	m_pSource->GetName().c_str(), m_pSource->GetKezhong(), m_pSource->GetDunjia(), nCutCount, m_pSource->GetLen(), m_pSource->GetWidth(), m_nTotalCount, fPrice);
	sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；国标纸 %d开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
		m_pSource->GetName().c_str(), nCutCount, m_pSource->GetLen(), m_pSource->GetWidth(), m_nTotalCount, fPrice);
	LogInfo tInfo1;
	tInfo1.nType = 4;
	tInfo1.strLog = chLog;
	vecLogInfo.push_back(tInfo1);
	return true;
}

float TSourceSelectPlanEntity::GetPriceByMoreSize(vector<LogInfo>& vecLogInfo)
{
	//原材料数量总计:m_nTotalCount
	//原材料面积(平方米):(m_nOpenLen + 30) * m_nOpenWidth / 1000 /1000   ||  m_nOpenLen * (m_nOpenWidth + 20) / 1000 /1000
	//原材料总面积:m_nTotalCount * ((m_nOpenLen + 30) * m_nOpenWidth / 1000 /1000 )
	//原材料总重量（吨）:m_nTotalCount * ((m_nOpenLen + 30) * m_nOpenWidth / 1000 /1000 ) * m_pSource->GetKezhong() /1000 /1000
	//总价:m_nTotalCount * ((m_nOpenLen + 30) * m_nOpenWidth / 1000 /1000 ) * m_pSource->GetKezhong() /1000 /1000 * m_pSource->GetDunjia()

	//长或宽5cm进位  正好5cm整除则不变，否则加到5cm可以整除为止
	int nRightLen = (m_nOpenLen % 50 == 0) ? m_nOpenLen : ((m_nOpenLen / 50 + 1) * 50);
	int nRightWidth = (m_nOpenWidth % 50 == 0) ? m_nOpenWidth : ((m_nOpenWidth / 50 + 1) * 50);

	float fPrice1 = (float)m_nTotalCount * nRightLen * m_nOpenWidth  * m_pSource->GetKezhong() * m_pSource->GetDunjia() / 1000 / 1000 / 1000 / 1000;
	char chLog[512] = { 0 };
	//sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；材料克重：%d；材料吨价：%.2f；非国标纸(长+5cm进位)1开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
	//	m_pSource->GetName().c_str(), m_pSource->GetKezhong(), m_pSource->GetDunjia(), nRightLen, m_nOpenWidth, m_nTotalCount, fPrice1);
	sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；非国标纸(长+5cm进位)1开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
		m_pSource->GetName().c_str(), nRightLen, m_nOpenWidth, m_nTotalCount, fPrice1);
	LogInfo tInfo;
	tInfo.nType = 4;
	tInfo.strLog = chLog;
	vecLogInfo.push_back(tInfo);

	float fPrice2 = (float)m_nTotalCount * m_nOpenLen * nRightWidth  * m_pSource->GetKezhong() * m_pSource->GetDunjia() / 1000 / 1000 / 1000 / 1000;
	memset(chLog,0,512);
	//sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；材料克重：%d；材料吨价：%.2f；非国标纸(宽+5cm进位)1开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
	//	m_pSource->GetName().c_str(), m_pSource->GetKezhong(), m_pSource->GetDunjia(), m_nOpenLen, nRightWidth, m_nTotalCount, fPrice2);
	sprintf_s(chLog, 512, "材料成本明细>>材料名称：%s；非国标纸(宽+5cm进位)1开；长(mm)：%d；宽(mm)：%d；总数(含出数)：%d；总价：%.2f",
		m_pSource->GetName().c_str(), m_nOpenLen, nRightWidth, m_nTotalCount, fPrice2);
	tInfo.nType = 4;
	tInfo.strLog = chLog;
	vecLogInfo.push_back(tInfo);

	float fPrice = fPrice1 < fPrice2 ? fPrice1 : fPrice2;
	return fPrice;
}
