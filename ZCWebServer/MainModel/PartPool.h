#pragma once
#include "PartEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TPartPool
{
public:
	TPartPool();
	virtual ~TPartPool();

public: //页面接口
	int AddPart(string& strName, string& strTypeName, float nPrice);
	int DeletePart(string& strName, string& strTypeName);
	void EachPart(function<void(TPartEntity*)> pCallBack);
	int GetPartCount();

public:
	bool GetPartPriceByName(string& strName, float& fPrice);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TPartEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


