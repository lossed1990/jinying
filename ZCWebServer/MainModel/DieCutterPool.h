#pragma once
#include "DieCutterEntity.h"
#include <vector>
#include <functional>

#include "../ThreadSync/ThreadSync.h"

using std::vector;
using std::function;

class TDieCutterPool
{
public:
	TDieCutterPool();
	virtual ~TDieCutterPool();

public: //����ӿ�
	int AddDieCutter(string& strTypeName,int nLen, int nWidth, int nHeight);
	int DeleteDieCutter(string& strTypeName, string& strName);
	void EachDieCutter(function<void(TDieCutterEntity*)> pCallBack);
	void SetDieCutterPrice(float nPrice);
	void SetBoxDieCutterPrice(float nPrice);
	int GetDieCutterCount();
	float GetDieCutterPrice();
	float GetBoxDieCutterPrice();

public:
	bool IsHavingDieCutter(int nLen, int nWidth, int nHeight);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	std::vector<TDieCutterEntity*> m_vecList;
	CMyCriticalSection m_csList;				//�ٽ���

	float m_nPrice = 0;      //�������ģ�ĳɱ���
	float m_nBoxPrice = 0;   //��װ�е�ģ�ĳɱ���

};


