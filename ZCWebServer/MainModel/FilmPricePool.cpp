#include "FilmPricePool.h"
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

TFilmPricePool::TFilmPricePool()
{
	m_csPrice.Enter();
	LoadFromConfig();
	m_csPrice.Leave();
}

TFilmPricePool::~TFilmPricePool()
{
}

int TFilmPricePool::AddFilmMorerule(int nBegin, int nEnd, int nCount)
{
	m_csPrice.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetBegin() == nBegin && item->GetEnd() == nEnd)
		{
			m_csPrice.Leave();
			return -1;  //新增失败，范围已存在
		}
	}

	TMoreruleEntity* pNewEntity = new TMoreruleEntity(false, false, nBegin, nEnd, nCount);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csPrice.Leave();
	return 0;
}

int TFilmPricePool::DeleteFilmMorerule(string& strRange)
{
	m_csPrice.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		char chRange[64] = { 0 }; //数量区间
		sprintf_s(chRange, "%d-%d", (*iter)->GetBegin(), (*iter)->GetEnd());
		if (strRange == chRange)
		{
			delete (*iter);
			*iter = nullptr;
			m_vecList.erase(iter);
			SaveToConfig();
			m_csPrice.Leave();
			return 0;
		}
	}

	m_csPrice.Leave();
	return -1;  //删除失败，范围不存在
}

void TFilmPricePool::EachFilmMorerules(function<void(TMoreruleEntity*)> pCallBack)
{
	m_csPrice.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csPrice.Leave();
}

int TFilmPricePool::GetFilmMoreruleCount()
{
	m_csPrice.Enter();
	int nCount = m_vecList.size();
	m_csPrice.Leave();
	return nCount;
}

int TFilmPricePool::GetFilmMoreCountByMoreRule(int nNumber)
{
	m_csPrice.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetBegin() <= nNumber && item->GetEnd() >= nNumber)
		{
			int nCount = item->GetCount();
			m_csPrice.Leave();
			return nCount;
		}
	}

	m_csPrice.Leave();
	return 0;
}

void TFilmPricePool::SetFilmPrice(float nPrice)
{
	m_csPrice.Enter();
	m_nPrice = nPrice;         //单价
	SaveToConfig();
	m_csPrice.Leave();
}

float TFilmPricePool::GetFilmPrice()
{
	m_csPrice.Enter();
	float fPrice = m_nPrice;
	m_csPrice.Leave();
	return fPrice;
}

bool TFilmPricePool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\filmmorerule.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//配置文件不会超过8K
		char pcBuff[8192] = { 0 };
		fread(pcBuff, 1, 8192, pFile);

		Document doc;
		doc.Parse(pcBuff);

		if (doc.IsObject())
		{
			if (doc["data"].IsArray())
			{
				for (SizeType i = 0; i < doc["data"].Size(); i++)
				{
					int nBegin = doc["data"][i]["begin"].GetInt();
					int nEnd = doc["data"][i]["end"].GetInt();
					int nCount = doc["data"][i]["count"].GetInt();

					TMoreruleEntity* pNewEntity = new TMoreruleEntity(false, false, nBegin, nEnd, nCount);
					m_vecList.push_back(pNewEntity);
				}
			}

			m_nPrice = doc["price"].GetFloat();
		}

		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TFilmPricePool::SaveToConfig()
{
	//构造json串
	const char* SUBTEMPLATE = "{\"price\":%f,\"data\":[";
	char pcHeadTemp[256] = { 0 };
	sprintf_s(pcHeadTemp, 256, SUBTEMPLATE, m_nPrice);

	string strReturn = pcHeadTemp;
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"begin\":%d,\"end\":%d,\"count\":%d}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetBegin(), m_vecList[i]->GetEnd(), m_vecList[i]->GetCount());
		strReturn = strReturn + pcSubTemp;
		if (i != (m_vecList.size() - 1))
		{
			strReturn = strReturn + ",";
		}
	}
	strReturn = strReturn + "]}";

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

		strcat(tpcPath, "\\filmmorerule.json");

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

