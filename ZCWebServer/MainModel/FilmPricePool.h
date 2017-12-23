#pragma once
#include "../ThreadSync/ThreadSync.h"
#include "MoreruleEntity.h"
#include <vector>
#include <functional>
using std::vector;
using std::function;

//��Ĥ��
class TFilmPricePool
{
public:
	TFilmPricePool();
	virtual ~TFilmPricePool();

public: //����ӿ�
	int AddFilmMorerule(int nBegin, int nEnd, int nCount);
	int DeleteFilmMorerule(string& strRange);
	void EachFilmMorerules(function<void(TMoreruleEntity*)> pCallBack);
	int GetFilmMoreruleCount();

	float GetFilmPrice();
	void SetFilmPrice(float nPrice);

public: //ҵ��ӿ�
	/**
	 * @brief ͨ������������ȡ��Ĥ����
	 *
	 * ƥ���һ������
	 */
	int GetFilmMoreCountByMoreRule(int nNumber);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	float m_nPrice = 0;                        //ÿƽ�׵���
	CMyCriticalSection m_csPrice;			   //�ٽ���

	std::vector<TMoreruleEntity*> m_vecList;
};


