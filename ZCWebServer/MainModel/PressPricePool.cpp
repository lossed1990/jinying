#include "PressPricePool.h"
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

TPressPricePool::TPressPricePool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TPressPricePool::~TPressPricePool()
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		delete item;
		item = nullptr;
	}
	m_vecList.clear();
	m_csList.Leave();
}

int TPressPricePool::AddPressprice(string& strPressName, string& strPressTypeName, int nBegin, int nEnd, float nPrice)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetPressName() == strPressName && item->GetPressTypeName() == strPressTypeName && item->GetBegin() == nBegin && item->GetEnd() == nEnd)
		{
			m_csList.Leave();
			return -1;  //新增失败，已存在
		}
	}

	TPressPriceEntity* pNewEntity = new TPressPriceEntity(strPressName, strPressTypeName, nBegin, nEnd, nPrice);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TPressPricePool::DeletePressprice(string& strPressName, string& strPressTypeName, string& strRange)
{
	m_csList.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		char chRange[64] = { 0 }; //数量区间
		sprintf_s(chRange, "%d-%d", (*iter)->GetBegin(), (*iter)->GetEnd());
		if ((*iter)->GetPressName() == strPressName && (*iter)->GetPressTypeName() == strPressTypeName && strRange == chRange)
		{
			delete (*iter);
			*iter = nullptr;
			m_vecList.erase(iter);
			SaveToConfig();
			m_csList.Leave();
			return 0;
		}
	}

	m_csList.Leave();
	return -1;  //删除失败，范围不存在
}

void TPressPricePool::EachPressprice(function<void(TPressPriceEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TPressPricePool::GetPresspriceCount()
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

bool TPressPricePool::CountPressprice(string& strPressName, string& strPressTypeName, int nCount, float& fPrice)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetPressName() == strPressName && item->GetPressTypeName() == strPressTypeName && item->GetBegin() <= nCount && item->GetEnd() >= nCount)
		{
			fPrice = item->GetPrice();
			m_csList.Leave();
			return true;
		}
	}

	m_csList.Leave();
	return false;
}

bool TPressPricePool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\pressprice.json");

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
					//char pcTemp[256] = { 0 };
					//CEncodingTools::ConvertUTF8ToGB(doc["data"][i]["pressname"].GetString(), pcTemp, 256);
					//string strPressName(pcTemp);
					string strPressName = doc["data"][i]["pressname"].GetString();

					//memset(pcTemp, 256, 0);
					//CEncodingTools::ConvertUTF8ToGB(doc["data"][i]["presstypename"].GetString(), pcTemp, 256);
					//string strPressTypeName(pcTemp);
					string strPressTypeName = doc["data"][i]["presstypename"].GetString();

					int nBeign = doc["data"][i]["begin"].GetInt();
					int nEnd = doc["data"][i]["end"].GetInt();
					float nPrice = doc["data"][i]["price"].GetFloat();

					TPressPriceEntity* pNewEntity = new TPressPriceEntity(strPressName, strPressTypeName, nBeign, nEnd, nPrice);
					m_vecList.push_back(pNewEntity);
				}
			}
		}

		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TPressPricePool::SaveToConfig()
{
	//构造json串
	string strReturn = "{\"data\":[";
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"pressname\":\"%s\",\"presstypename\":\"%s\",\"begin\":%d,\"end\":%d,\"price\":%f}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetPressName().c_str(), m_vecList[i]->GetPressTypeName().c_str(), m_vecList[i]->GetBegin(), m_vecList[i]->GetEnd(), m_vecList[i]->GetPrice());
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

		strcat(tpcPath, "\\pressprice.json");

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

