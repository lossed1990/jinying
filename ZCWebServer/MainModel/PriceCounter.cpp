#include "PriceCounter.h"
#include "../MainModel/MainModel.h"

CPriceCounter::CPriceCounter()
{
	m_mapFilmTip[false] = "不覆膜";
	m_mapFilmTip[true] = "覆膜";

	m_mapPressTip[false] = "不印刷";
	m_mapPressTip[true] = "印刷";
}

CPriceCounter::~CPriceCounter()
{
}

float CPriceCounter::GetPrice(int nSourceUserType, CounterParam& tParam, vector<LogInfo>& vecLogInfo)
{
	m_nNumber = tParam.nNumber;
	m_nLen = tParam.nLen;
	m_nWidth = tParam.nWidth;
	m_nHeight = tParam.nHeight;
	m_strSourceTypeName = tParam.strSourceTypeName;
	m_bTangjin = tParam.bTangjin;
	m_bFilm = tParam.bFilm;
	m_bPress = tParam.bPress;
	m_strPressType = tParam.strPressType;
	m_nProductType = tParam.nProductType;
	m_nSourceUserType = nSourceUserType;

	//计算参数
	CountOpenArea();
	CountTotalNumber();
	CountPressNumber();
	CountFilmNumber();

	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "需求参数明细>>%s；总数：%d；规格(mm)：%d*%d*%d；材质：%s；%s；%s；印刷方式：%s；%s",
		m_strProductInfo.c_str(), m_nNumber, m_nLen, m_nWidth, m_nHeight, m_strSourceTypeName.c_str(), m_mapFilmTip[m_bFilm].c_str(), m_mapPressTip[m_bPress].c_str(), m_strPressType.c_str(), m_strOpenAreaInfo.c_str());
	LogInfo tInfo;
	tInfo.nType = 3;
	tInfo.strLog = chLog;
	vecLogInfo.push_back(tInfo);

	float fTotalPrice = 0;
	float fSourcePrice = 0, fPressPrice = 0, fFilePrice = 0;
	bool bRet = CountSourceMinPrice(fSourcePrice, vecLogInfo);
	if (bRet)
	{
		fTotalPrice += fSourcePrice;
	}

	bRet = CountPressMinPrice(fPressPrice, vecLogInfo);
	if (bRet)
	{
		fTotalPrice += fPressPrice;
	}

	bRet = CountFilmMinPrice(fFilePrice, vecLogInfo);
	if (bRet)
	{
		fTotalPrice += fFilePrice;
	}

	bRet = CountImpressPrice(fFilePrice, vecLogInfo);
	if (bRet)
	{
		fTotalPrice += fFilePrice;
	}
	return fTotalPrice;
}

void CPriceCounter::CountOpenArea()
{
	switch (m_nProductType)
	{
	case 0:  //手提袋
	{
				 if (m_nSourceUserType == 0)  //整片制作
				 {
					 //长（袋子的长 + 宽 * 2 + 2.5cm）
					 int nLen = (m_nLen + m_nWidth) * 2 + 25;
					 //宽（袋子的高 + （宽 / 2 + 2cm） + 4cm）
					 int nWidth = m_nHeight + (m_nWidth / 2 + 20) + 40;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "手提袋(整片制作)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:(L+W)*2+2.5 宽:H+(W/2+2)+4]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //单片制作
				 {
					 //长（袋子的长 + 宽 + 2.5cm）
					 int nLen = m_nLen + m_nWidth + 25;
					 //宽（袋子的高 + （宽 / 2 + 2cm） + 4cm）
					 int nWidth = m_nHeight + (m_nWidth / 2 + 20) + 40;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "手提袋(单片制作)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:L+W+2.5 宽:H+(W/2+2)+4]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 break;
	}
	case 1:  //盒盖
	{
				 if (m_nSourceUserType == 0)  //灰板
				 {
					 //长 ：L + （H * 2） + 1cm
					 int nLen = m_nLen + m_nHeight * 2 + 10;
					 //宽： W + （H * 2） + 1cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 10;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "盒盖(灰板)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:L+(H*2)+1 宽:W+H*2+1]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //包纸
				 {
					 //长： L + 2H + 4cm
					 int nLen = m_nLen + m_nHeight * 2 + 40;
					 // 宽： W + 2H + 5cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 50;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "盒盖(包纸)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:L+(H*2)+4 宽:W+H*2+5]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 break;
	}
	case 2:  //盒底
	{
				 if (m_nSourceUserType == 0)  //灰板
				 {
					 //长 ：L + （H * 2）+ 1cm
					 int nLen = m_nLen + m_nHeight * 2 + 10;
					 //宽： W + （H * 2）+ 1cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 10;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "盒底(灰板)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:L+(H*2)+1 宽:W+H*2+1]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //包纸
				 {
					 //长： L + 2H + 4cm
					 int nLen = m_nLen + m_nHeight * 2 + 40;
					 //宽： W + 2H + 5cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 50;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "盒底(包纸)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "计算公式：[长:L+(H*2)+4 宽:W+H*2+5]；展开规格(mm)：%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 break;
	}
	default:
		break;
	}
}

void CPriceCounter::CountTotalNumber()
{
	int nMoreNumber = CMainModel::Instance()->GetMoreCountByMoreRule(m_bPress, m_bTangjin, m_nNumber);
	m_nTatalNumber = m_nNumber + nMoreNumber;

	if (m_nProductType == 0 && m_nSourceUserType == 1)  //手提袋单片制作
	{
		m_nTatalNumber = m_nTatalNumber * 2;
	}
}

void CPriceCounter::CountPressNumber()
{
	if (m_nProductType == 0 && m_nSourceUserType == 1)  //手提袋单片制作
	{
		m_nPressNumber = m_nNumber * 2;
	}
	else
	{
		m_nPressNumber = m_nNumber;
	}
}

void CPriceCounter::CountFilmNumber()
{
	m_nFilmNumber = m_nNumber + CMainModel::Instance()->GetFilmMoreCountByMoreRule(m_nNumber);

	if (m_nProductType == 0 && m_nSourceUserType == 1)  //手提袋单片制作
	{
		m_nFilmNumber = m_nFilmNumber * 2;
	}
}

bool CPriceCounter::CountSourceMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	return CMainModel::Instance()->TSourcePool::CountSourceMinPrice(m_strSourceTypeName, m_nOpenLen, m_nOpenWidth, m_nTatalNumber, m_nNumber, fPrice, vecLogInfo);
}

bool CPriceCounter::CountPressMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	//不需要印刷
	if (!m_bPress)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 5;
		tInfo.strLog = "印刷成本明细>>不需要印刷";
		vecLogInfo.push_back(tInfo);
		return true;
	}

	//选择规格匹配的印刷机
	vector<string> vecPressList;
	CMainModel::Instance()->GetRightPressList(m_nOpenLen, m_nOpenWidth, vecPressList);
	if (vecPressList.size() == 0)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "印刷成本明细>>未找到匹配的印刷机";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	//计算最低印刷价格
	bool bSuccess = false;
	float fMinPrice = 100000000;
	string strPressName;         //最低价所使用的印刷机名称
	for (auto& item : vecPressList)
	{
		float fTempPrice = 0;
		bool bRet = CMainModel::Instance()->CountPressprice(item, m_strPressType, m_nPressNumber, fTempPrice);
		if (bRet)
		{
			bSuccess = true;
			if (fTempPrice < fMinPrice)
			{
				fMinPrice = fTempPrice;
				strPressName = item;
			}
		}
	}

	if (!bSuccess)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "印刷成本明细>>未找到有效的印刷价格区间";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	fPrice = fMinPrice;
	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "印刷成本明细>>印刷机:%s;印刷方式:%s;印刷数量:%d;印刷成本:%.2f",
		strPressName.c_str(), m_strPressType.c_str(), m_nPressNumber, fMinPrice);
	{
		LogInfo tInfo;
		tInfo.nType = 5;
		tInfo.strLog = chLog;
		vecLogInfo.push_back(tInfo);
	}
	return true;       //成功 
}

bool CPriceCounter::CountFilmMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	//不需要覆膜
	if (!m_bFilm)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 6;
		tInfo.strLog = "覆膜成本明细>>不需要覆膜";
		vecLogInfo.push_back(tInfo);
		return true;
	}

	float fFilePrice = CMainModel::Instance()->GetFilmPrice();
	if (fFilePrice <= 0.01)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "覆膜成本明细>>尚未配置覆膜单价";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	float fAreaSize = (float)m_nFilmNumber * m_nOpenLen * m_nOpenWidth / 1000 / 1000;
	fPrice = fFilePrice * fAreaSize;
	char chAreaSize[128] = { 0 };
	sprintf_s(chAreaSize, 128, "覆膜成本明细>>覆膜面积(m^2)：%.2f；覆膜单价(元)：%.2f；覆膜总价(元)：%.2f；", fAreaSize, fFilePrice, fPrice);
	{
		LogInfo tInfo;
		tInfo.nType = 6;
		tInfo.strLog = chAreaSize;
		vecLogInfo.push_back(tInfo);
	}
	return true;
}

bool CPriceCounter::CountImpressPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	float nCheckPrice = CMainModel::Instance()->GetImpressCheckPrice();
	float nImpressPrice = CMainModel::Instance()->GetImpressPrice();
	if (nCheckPrice <= 0.001 || nImpressPrice <= 0.001)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "压痕成本明细>>尚未配置压痕价格规则";
		vecLogInfo.push_back(tInfo);
		return false;
	}


	float nTotalImpressPrice = nCheckPrice + (nImpressPrice * m_nPressNumber);
	fPrice = nTotalImpressPrice;

	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "压痕成本明细>>压痕费用：%.2f；", nTotalImpressPrice);

	LogInfo tInfo1;
	tInfo1.nType = 7;
	tInfo1.strLog = chLog;
	vecLogInfo.push_back(tInfo1);
	return true;
}