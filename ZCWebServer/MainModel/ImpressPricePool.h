#pragma once
#include "../ThreadSync/ThreadSync.h"

//压痕费
class TImpressPricePool
{
public:
	TImpressPricePool();
	virtual ~TImpressPricePool();

public: //对外接口
	float GetImpressCheckPrice();
	float GetImpressPrice();
	void SetImpressCheckPrice(float nCheckPrice);
	void SetImpressPrice(float nPrice);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	float m_nCheckPrice = 0;    //校板费
	float m_nPrice = 0;         //单价
	CMyCriticalSection m_csPrice;			   //临界区
};


