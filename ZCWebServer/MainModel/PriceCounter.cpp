#include "PriceCounter.h"
#include "../MainModel/MainModel.h"

CPriceCounter::CPriceCounter()
{
	m_mapFilmTip[false] = "����Ĥ";
	m_mapFilmTip[true] = "��Ĥ";

	m_mapPressTip[false] = "��ӡˢ";
	m_mapPressTip[true] = "ӡˢ";
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

	//�������
	CountOpenArea();
	CountTotalNumber();
	CountPressNumber();
	CountFilmNumber();

	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "���������ϸ>>%s��������%d�����(mm)��%d*%d*%d�����ʣ�%s��%s��%s��ӡˢ��ʽ��%s��%s",
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
	case 0:  //�����
	{
				 if (m_nSourceUserType == 0)  //��Ƭ����
				 {
					 //�������ӵĳ� + �� * 2 + 2.5cm��
					 int nLen = (m_nLen + m_nWidth) * 2 + 25;
					 //�����ӵĸ� + ���� / 2 + 2cm�� + 4cm��
					 int nWidth = m_nHeight + (m_nWidth / 2 + 20) + 40;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�����(��Ƭ����)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:(L+W)*2+2.5 ��:H+(W/2+2)+4]��չ�����(mm)��%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //��Ƭ����
				 {
					 //�������ӵĳ� + �� + 2.5cm��
					 int nLen = m_nLen + m_nWidth + 25;
					 //�����ӵĸ� + ���� / 2 + 2cm�� + 4cm��
					 int nWidth = m_nHeight + (m_nWidth / 2 + 20) + 40;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�����(��Ƭ����)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:L+W+2.5 ��:H+(W/2+2)+4]��չ�����(mm)��%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 break;
	}
	case 1:  //�и�
	{
				 if (m_nSourceUserType == 0)  //�Ұ�
				 {
					 //�� ��L + ��H * 2�� + 1cm
					 int nLen = m_nLen + m_nHeight * 2 + 10;
					 //�� W + ��H * 2�� + 1cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 10;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�и�(�Ұ�)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:L+(H*2)+1 ��:W+H*2+1]��չ�����(mm)��%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //��ֽ
				 {
					 //���� L + 2H + 4cm
					 int nLen = m_nLen + m_nHeight * 2 + 40;
					 // �� W + 2H + 5cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 50;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�и�(��ֽ)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:L+(H*2)+4 ��:W+H*2+5]��չ�����(mm)��%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 break;
	}
	case 2:  //�е�
	{
				 if (m_nSourceUserType == 0)  //�Ұ�
				 {
					 //�� ��L + ��H * 2��+ 1cm
					 int nLen = m_nLen + m_nHeight * 2 + 10;
					 //�� W + ��H * 2��+ 1cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 10;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�е�(�Ұ�)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:L+(H*2)+1 ��:W+H*2+1]��չ�����(mm)��%d*%d",
						 m_nOpenLen, m_nOpenWidth);
					 m_strOpenAreaInfo = chLog;
				 }
				 else if (m_nSourceUserType == 1)  //��ֽ
				 {
					 //���� L + 2H + 4cm
					 int nLen = m_nLen + m_nHeight * 2 + 40;
					 //�� W + 2H + 5cm
					 int nWidth = m_nWidth + m_nHeight * 2 + 50;

					 m_nOpenLen = (nLen >= nWidth) ? nLen : nWidth;
					 m_nOpenWidth = (nLen < nWidth) ? nLen : nWidth;

					 m_strProductInfo = "�е�(��ֽ)";
					 char chLog[512] = { 0 };
					 sprintf_s(chLog, 512, "���㹫ʽ��[��:L+(H*2)+4 ��:W+H*2+5]��չ�����(mm)��%d*%d",
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

	if (m_nProductType == 0 && m_nSourceUserType == 1)  //�������Ƭ����
	{
		m_nTatalNumber = m_nTatalNumber * 2;
	}
}

void CPriceCounter::CountPressNumber()
{
	if (m_nProductType == 0 && m_nSourceUserType == 1)  //�������Ƭ����
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

	if (m_nProductType == 0 && m_nSourceUserType == 1)  //�������Ƭ����
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
	//����Ҫӡˢ
	if (!m_bPress)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 5;
		tInfo.strLog = "ӡˢ�ɱ���ϸ>>����Ҫӡˢ";
		vecLogInfo.push_back(tInfo);
		return true;
	}

	//ѡ����ƥ���ӡˢ��
	vector<string> vecPressList;
	CMainModel::Instance()->GetRightPressList(m_nOpenLen, m_nOpenWidth, vecPressList);
	if (vecPressList.size() == 0)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "ӡˢ�ɱ���ϸ>>δ�ҵ�ƥ���ӡˢ��";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	//�������ӡˢ�۸�
	bool bSuccess = false;
	float fMinPrice = 100000000;
	string strPressName;         //��ͼ���ʹ�õ�ӡˢ������
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
		tInfo.strLog = "ӡˢ�ɱ���ϸ>>δ�ҵ���Ч��ӡˢ�۸�����";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	fPrice = fMinPrice;
	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "ӡˢ�ɱ���ϸ>>ӡˢ��:%s;ӡˢ��ʽ:%s;ӡˢ����:%d;ӡˢ�ɱ�:%.2f",
		strPressName.c_str(), m_strPressType.c_str(), m_nPressNumber, fMinPrice);
	{
		LogInfo tInfo;
		tInfo.nType = 5;
		tInfo.strLog = chLog;
		vecLogInfo.push_back(tInfo);
	}
	return true;       //�ɹ� 
}

bool CPriceCounter::CountFilmMinPrice(float& fPrice, vector<LogInfo>& vecLogInfo)
{
	//����Ҫ��Ĥ
	if (!m_bFilm)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 6;
		tInfo.strLog = "��Ĥ�ɱ���ϸ>>����Ҫ��Ĥ";
		vecLogInfo.push_back(tInfo);
		return true;
	}

	float fFilePrice = CMainModel::Instance()->GetFilmPrice();
	if (fFilePrice <= 0.01)
	{
		fPrice = 0;
		LogInfo tInfo;
		tInfo.nType = 1;
		tInfo.strLog = "��Ĥ�ɱ���ϸ>>��δ���ø�Ĥ����";
		vecLogInfo.push_back(tInfo);
		return false;
	}

	float fAreaSize = (float)m_nFilmNumber * m_nOpenLen * m_nOpenWidth / 1000 / 1000;
	fPrice = fFilePrice * fAreaSize;
	char chAreaSize[128] = { 0 };
	sprintf_s(chAreaSize, 128, "��Ĥ�ɱ���ϸ>>��Ĥ���(m^2)��%.2f����Ĥ����(Ԫ)��%.2f����Ĥ�ܼ�(Ԫ)��%.2f��", fAreaSize, fFilePrice, fPrice);
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
		tInfo.strLog = "ѹ�۳ɱ���ϸ>>��δ����ѹ�ۼ۸����";
		vecLogInfo.push_back(tInfo);
		return false;
	}


	float nTotalImpressPrice = nCheckPrice + (nImpressPrice * m_nPressNumber);
	fPrice = nTotalImpressPrice;

	char chLog[512] = { 0 };
	sprintf_s(chLog, 512, "ѹ�۳ɱ���ϸ>>ѹ�۷��ã�%.2f��", nTotalImpressPrice);

	LogInfo tInfo1;
	tInfo1.nType = 7;
	tInfo1.strLog = chLog;
	vecLogInfo.push_back(tInfo1);
	return true;
}