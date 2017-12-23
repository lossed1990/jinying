#pragma once
#include "PressEntity.h"
#include <vector>
#include <functional>
#include "../ThreadSync/ThreadSync.h"
using std::vector;
using std::function;

class TPressPool
{
public:
	TPressPool();
	virtual ~TPressPool();

public: //页面接口
	int AddPress(string& strName, int nLen, int nWidth);
	int DeletePress(string& strName);
	void EachPress(function<void(TPressEntity*)> pCallBack);
	int GetPressCount();

public: //业务接口
	/**
	 * @brief 获取符合规格的印刷机列表
	 */
	void GetRightPressList(int nLen, int nWidth, std::vector<string>& vecReturn);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TPressEntity*> m_vecList;
	CMyCriticalSection m_csList;				//临界区
};


