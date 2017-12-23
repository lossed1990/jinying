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

public: //ҳ��ӿ�
	int AddPress(string& strName, int nLen, int nWidth);
	int DeletePress(string& strName);
	void EachPress(function<void(TPressEntity*)> pCallBack);
	int GetPressCount();

public: //ҵ��ӿ�
	/**
	 * @brief ��ȡ���Ϲ���ӡˢ���б�
	 */
	void GetRightPressList(int nLen, int nWidth, std::vector<string>& vecReturn);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TPressEntity*> m_vecList;
	CMyCriticalSection m_csList;				//�ٽ���
};


