#pragma once
#include "SourceTypeEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TSourceTypePool
{
public:
	TSourceTypePool();
	virtual ~TSourceTypePool();

public: //对外接口
	int AddSourceType(string& strName);
	int DeleteSourceType(string& strName);
	void EachSourceType(function<void(TSourceTypeEntity*)> pCallBack);
	int GetSourceTypeCount();

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TSourceTypeEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


