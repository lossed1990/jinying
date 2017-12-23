#include "ImpressPricePool.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
using namespace rapidjson;

TImpressPricePool::TImpressPricePool()
{
	m_csPrice.Enter();
	LoadFromConfig();
	m_csPrice.Leave();
}

TImpressPricePool::~TImpressPricePool()
{
}

float TImpressPricePool::GetImpressCheckPrice()
{
	m_csPrice.Enter();
	float fPrice = m_nCheckPrice;
	m_csPrice.Leave();
	return fPrice;
}
float TImpressPricePool::GetImpressPrice()
{
	m_csPrice.Enter();
	float fPrice = m_nPrice;
	m_csPrice.Leave();
	return fPrice;
}

void TImpressPricePool::SetImpressCheckPrice(float nCheckPrice)
{
	m_csPrice.Enter();
	m_nCheckPrice = nCheckPrice;    //校板费
	SaveToConfig();
	m_csPrice.Leave();
}

void TImpressPricePool::SetImpressPrice(float nPrice)
{
	m_csPrice.Enter();
	m_nPrice = nPrice;         //单价
	SaveToConfig();
	m_csPrice.Leave();
}

bool TImpressPricePool::LoadFromConfig()
{
	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\impressprice.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//配置文件不会超过1K
		char pcBuff[1024] = { 0 };
		fread(pcBuff, 1, 1024, pFile);

		Document doc;
		doc.Parse(pcBuff);

		if (doc.IsObject())
		{
			m_nCheckPrice = doc["checkprice"].GetFloat();
			m_nPrice = doc["price"].GetFloat();
		}

		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TImpressPricePool::SaveToConfig()
{
	const char* SUBTEMPLATE = "{\"checkprice\":%f,\"price\":%f}";
	char pcSubTemp[256] = { 0 };
	sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_nCheckPrice, m_nPrice);

	//构造json串
	string strReturn = pcSubTemp;

	//检查文件
	Document doc;
	doc.Parse(strReturn.c_str());
	if (doc.IsObject())
	{
		StringBuffer outBuff;
		Writer<StringBuffer> jsonWriter(outBuff);
		doc.Accept(jsonWriter);

		//直接写入文件
		TCHAR tpcPath[260] = { 0 };
		GetExeFileSubDirectory(tpcPath, 260, "html");

		strcat(tpcPath, "\\impressprice.json");

		FILE* pFile = fopen(tpcPath, "wb");
		if (pFile)
		{
			fwrite(outBuff.GetString(), 1, outBuff.GetSize(), pFile);

			fclose(pFile);
			pFile = NULL;

			return 0;
		}
	}

	return -1;
}

