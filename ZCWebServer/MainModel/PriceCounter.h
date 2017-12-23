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

//�۸����������
class CPriceCounter
{
public:
	CPriceCounter();
	~CPriceCounter();

public:
	float GetPrice(int nSourceUserType, CounterParam& tParam, vector<LogInfo>& vecLogInfo);

public:
	bool CountSourceMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);      //���������ͳɱ�
	bool CountPressMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);       //����ӡˢ��ͳɱ�
	bool CountFilmMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo);        //���㸲Ĥ�ɱ�
	bool CountImpressPrice(float& fPrice, vector<LogInfo>& vecLogInfo);        //����ѹ�۳ɱ�

private:
	/**
	 * @brief ���ݲ�Ʒ����ʽ����չ�����
	 *
	 * ��������ְ�Ƭ����Ƭ������ʽ���������ֻҰ弰��ֽ��ʽ��
	 */
	void CountOpenArea();   
	/**
	 * @brief ����ɱ���������������
	 */
	void CountTotalNumber(); 
	/**
	 * @brief ����ӡˢ����
	 *
	 * �������Ƭ����ʱ����������2��ӡˢ����������
	 */
	void CountPressNumber(); 

	void CountFilmNumber();

private: //�������
	int m_nNumber;             //��������
	int m_nLen;                //����ߴ�
	int m_nWidth;			   
	int m_nHeight;			   
							   
	string m_strSourceTypeName;    //��������
							   
	bool m_bFilm;             //�Ƿ�Ĥ
	bool m_bPress;             //�Ƿ�ӡˢ
	bool m_bTangjin;
	string m_strPressType;     //ӡˢ��ʽ

	int m_nProductType;        //��Ʒ����  0-����� 1-�и� 2-�е� 
	int m_nSourceUserType;     //����ʹ������  �����:0-��Ƭ���� 1-��Ƭ�������������֣�0-�Ұ� 1-��ֽ

private: //�������
	int m_nOpenLen = 0;             //չ���ߴ�
	int m_nOpenWidth = 0;
	int m_nTatalNumber = 0;         //����+��������
	int m_nPressNumber = 0;         //ӡˢ����
	int m_nFilmNumber = 0;          //��Ĥ���� 

	string m_strProductInfo;
	string m_strOpenAreaInfo;

private:
	map<bool, string> m_mapFilmTip;
	map<bool, string> m_mapPressTip;


};


