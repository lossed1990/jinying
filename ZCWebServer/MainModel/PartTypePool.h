#pragma once
#include "PartTypeEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TPartTypePool
{
public:
	TPartTypePool();
	virtual ~TPartTypePool();

public: //对外接口
	int AddPartType(string& strName);
	int DeletePartType(string& strName);
	void EachPartType(function<void(TPartTypeEntity*)> pCallBack);
	int GetPartTypeCount();

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TPartTypeEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


