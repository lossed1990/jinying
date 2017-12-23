#pragma once
#include "SourceEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
#include "CommonDefine.h"
using std::vector;
using std::function;

class TSourcePool
{
public:
	TSourcePool();
	virtual ~TSourcePool();

public: //页面接口
	int AddSource(string& strName, string& strTypeName, int nLen, int nWidth, int nKezhong, float nDunjia, int nUseType);
	int DeleteSource(string& strName);
	void EachSource(function<void(TSourceEntity*)> pCallBack);
	int GetSourceCount();

public: //业务接口
	bool CountSourceMinPrice(string& strTypeName, int nOpenLen, int nOpenWidth, int nTotalCount, int nNeedCount, float& fPrice, vector<LogInfo>& vecLogInfo);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TSourceEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


