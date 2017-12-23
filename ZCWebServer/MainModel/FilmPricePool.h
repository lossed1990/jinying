#pragma once
#include "../ThreadSync/ThreadSync.h"
#include "MoreruleEntity.h"
#include <vector>
#include <functional>
using std::vector;
using std::function;

//覆膜费
class TFilmPricePool
{
public:
	TFilmPricePool();
	virtual ~TFilmPricePool();

public: //对外接口
	int AddFilmMorerule(int nBegin, int nEnd, int nCount);
	int DeleteFilmMorerule(string& strRange);
	void EachFilmMorerules(function<void(TMoreruleEntity*)> pCallBack);
	int GetFilmMoreruleCount();

	float GetFilmPrice();
	void SetFilmPrice(float nPrice);

public: //业务接口
	/**
	 * @brief 通过需求量，获取覆膜出数
	 *
	 * 匹配第一个区间
	 */
	int GetFilmMoreCountByMoreRule(int nNumber);

private:
	bool LoadFromConfig(); 
	int SaveToConfig(); 

private:
	float m_nPrice = 0;                        //每平米单价
	CMyCriticalSection m_csPrice;			   //临界区

	std::vector<TMoreruleEntity*> m_vecList;
};


