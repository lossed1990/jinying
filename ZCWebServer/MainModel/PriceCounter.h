#pragma once
#include <string>
#include <vector>
#include <map>
#include "CommonDefine.h"

using std::string;
using std::vector;
using std::map;

struct CounterParam{
	int nProductType;
	int nNumber;
	int nLen;
	int nWidth;
	int nHeight;
	bool bTangjin;
	bool bFilm; 
	bool bPress;
	string strSourceTypeName;
	string strPressType;
};

//价格计算器基类
class CPriceCounter
{
public:
	CPriceCounter();
	~CPriceCounter();

public:
	float GetPrice(int nSourceUserType, CounterParam& tParam, vector<LogInfo>& vecLogInfo);

public:
	bool CountSourceMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);      //计算材料最低成本
	bool CountPressMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);       //计算印刷最低成本
	bool CountFilmMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);        //计算覆膜成本
	bool CountImpressPrice(float& fPrice, vector<LogInfo>& vecLogInfo);        //计算压痕成本

private:
	/**
	 * @brief 根据产品及方式计算展开面积
	 *
	 * 手提袋区分半片、整片制作方式；盒子区分灰板及包纸方式；
	 */
	void CountOpenArea();   
	/**
	 * @brief 计算成本数量（含出数）
	 */
	void CountTotalNumber(); 
	/**
	 * @brief 计算印刷数量
	 *
	 * 手提袋半片制作时，数量乘以2，印刷不包含出数
	 */
	void CountPressNumber(); 

	void CountFilmNumber();

private: //请求参数
	int m_nNumber;             //需求数量
	int m_nLen;                //需求尺寸
	int m_nWidth;			   
	int m_nHeight;			   
							   
	string m_strSourceTypeName;    //材质名称
							   
	bool m_bFilm;             //是否覆膜
	bool m_bPress;             //是否印刷
	bool m_bTangjin;
	string m_strPressType;     //印刷方式

	int m_nProductType;        //产品类型  0-手提袋 1-盒盖 2-盒底 
	int m_nSourceUserType;     //材质使用类型  手提袋:0-整片制作 1-单片制作；盒子区分：0-灰板 1-包纸

private: //计算参数
	int m_nOpenLen = 0;             //展开尺寸
	int m_nOpenWidth = 0;
	int m_nTatalNumber = 0;         //需求+出数数量
	int m_nPressNumber = 0;         //印刷数量
	int m_nFilmNumber = 0;          //覆膜数量 

	string m_strProductInfo;
	string m_strOpenAreaInfo;

private:
	map<bool, string> m_mapFilmTip;
	map<bool, string> m_mapPressTip;


};


