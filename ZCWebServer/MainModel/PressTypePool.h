#pragma once
#include "PressTypeEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TPressTypePool
{
public:
	TPressTypePool();
	virtual ~TPressTypePool();

public: //����ӿ�
	int AddPressType(string& strName);
	int DeletePressType(string& strName);
	void EachPressType(function<void(TPressTypeEntity*)> pCallBack);
	int GetPressTypeCount();

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TPressTypeEntity*> m_vecList;
	CMyCriticalSection m_csList;				//�ٽ���
};


