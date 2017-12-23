#pragma once
#include "PressPriceEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TPressPricePool
{
public:
	TPressPricePool();
	virtual ~TPressPricePool();

public: //界面接口
	int AddPressprice(string& strPressName, string& strPressTypeName, int nBegin, int nEnd, float nPrice);
	int DeletePressprice(string& strPressName, string& strPressTypeName, string& strRange);
	void EachPressprice(function<void(TPressPriceEntity*)> pCallBack);
	int GetPresspriceCount();

public: //业务接口
	bool CountPressprice(string& strPressName, string& strPressTypeName, int nCount, float& fPrice);

private:
	bool LoadFromConfig();
	int SaveToConfig();

private:
	std::vector<TPressPriceEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


