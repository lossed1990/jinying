#pragma once
#include "../ThreadSync/ThreadSync.h"

//ѹ�۷�
class TImpressPricePool
{
public:
	TImpressPricePool();
	virtual ~TImpressPricePool();

public: //����ӿ�
	float GetImpressCheckPrice();
	float GetImpressPrice();
	void SetImpressCheckPrice(float nCheckPrice);
	void SetImpressPrice(float nPrice);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	float m_nCheckPrice = 0;    //У���
	float m_nPrice = 0;         //����
	CMyCriticalSection m_csPrice;			   //�ٽ���
};


