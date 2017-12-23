#pragma once
#include "MoreruleEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TMorerulePool
{
public:
	TMorerulePool();
	virtual ~TMorerulePool();

public: //ҳ��ӿ�
	int AddMorerule(bool bPress, bool bTangjin, int nBegin, int nEnd, int nCount);
	int DeleteMorerule(string& strCondition,string& strRange);
	void EachMorerules(function<void(TMoreruleEntity*)> pCallBack);
	int GetMoreruleCount();

public: //ҵ��ӿ�
	/**
	 * @brief ͨ������������ȡ����
	 * 
	 * ƥ���һ������
	 */
	int GetMoreCountByMoreRule(bool bPress, bool bTangjin, int nNumber);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TMoreruleEntity*> m_vecList;
	CMyCriticalSection m_csList;				//�ٽ���
};


