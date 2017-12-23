#pragma once
#include <string>
#include <vector>
#include "SourceEntity.h"
#include "CommonDefine.h"

using std::string;
using std::vector;

//材料选取方案类
class TSourceSelectPlanEntity
{
public:
	TSourceSelectPlanEntity(int nOpenLen, int nOpenWidth, int nTotalCount, int nNeedCount, TSourceEntity* pSource);
	~TSourceSelectPlanEntity();

public:
	bool GetMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);

private:
	bool GetPriceBySetSize(float& fPrice, vector<LogInfo>& vecLogInfo);    //匹配材料已有大小
	float GetPriceByMoreSize(vector<LogInfo>& vecLogInfo);              //匹配材料特定大小  （500以上才使用）

private: //请求参数
	TSourceEntity* m_pSource;

	int m_nOpenLen;
	int m_nOpenWidth;
	int m_nTotalCount;
	int m_nNeedCount;  //需求数  不含出数
};


