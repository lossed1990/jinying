#pragma once
#include <string>
#include <vector>
#include "SourceEntity.h"
#include "CommonDefine.h"

using std::string;
using std::vector;

//����ѡȡ������
class TSourceSelectPlanEntity
{
public:
	TSourceSelectPlanEntity(int nOpenLen, int nOpenWidth, int nTotalCount, int nNeedCount, TSourceEntity* pSource);
	~TSourceSelectPlanEntity();

public:
	bool GetMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);

private:
	bool GetPriceBySetSize(float& fPrice, vector<LogInfo>& vecLogInfo);    //ƥ��������д�С
	float GetPriceByMoreSize(vector<LogInfo>& vecLogInfo);              //ƥ������ض���С  ��500���ϲ�ʹ�ã�

private: //�������
	TSourceEntity* m_pSource;

	int m_nOpenLen;
	int m_nOpenWidth;
	int m_nTotalCount;
	int m_nNeedCount;  //������  ��������
};


