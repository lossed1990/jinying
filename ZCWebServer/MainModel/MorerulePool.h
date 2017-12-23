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

public: //页面接口
	int AddMorerule(bool bPress, bool bTangjin, int nBegin, int nEnd, int nCount);
	int DeleteMorerule(string& strCondition,string& strRange);
	void EachMorerules(function<void(TMoreruleEntity*)> pCallBack);
	int GetMoreruleCount();

public: //业务接口
	/**
	 * @brief 通过需求量，获取出数
	 * 
	 * 匹配第一个区间
	 */
	int GetMoreCountByMoreRule(bool bPress, bool bTangjin, int nNumber);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TMoreruleEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


