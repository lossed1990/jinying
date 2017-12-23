#include "PartPool.h"
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

TPartPool::TPartPool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TPartPool::~TPartPool()
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

int TPartPool::AddPart(string& strName, string& strTypeName, float nPrice)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetName() == strName && item->GetTypeName() == strTypeName)
		{
			m_csList.Leave();
			return -1;  //新增失败，已存在
		}
	}

	TPartEntity* pNewEntity = new TPartEntity(strName, strTypeName, nPrice);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TPartPool::DeletePart(string& strName, string& strTypeName)
{
	m_csList.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		if ((*iter)->GetName() == strName && (*iter)->GetTypeName() == strTypeName)
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

void TPartPool::EachPart(function<void(TPartEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TPartPool::GetPartCount()
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

bool TPartPool::GetPartPriceByName(string& strName, float& fPrice)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetName() == strName)
		{
			fPrice = item->GetPrice();
			m_csList.Leave();
			return true;
		}
	}

	m_csList.Leave();
	return false;
}

bool TPartPool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\part.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		char* pcBuff = new char[4 * 1024 * 1024];
		memset(pcBuff, 0, 4 * 1024 * 1024);
		fread(pcBuff, 1, 4 * 1024 * 1024, pFile);

		Document doc;
		doc.Parse(pcBuff);

		if (doc.IsObject())
		{
			if (doc["data"].IsArray())
			{
				for (SizeType i = 0; i < doc["data"].Size(); i++)
				{
					//char pcTemp[256] = { 0 };
					//CEncodingTools::ConvertUTF8ToGB(doc["data"][i]["name"].GetString(), pcTemp, 256);
					//string strName(pcTemp);
					string strName = doc["data"][i]["name"].GetString();

					//memset(pcTemp, 256, 0);
					//CEncodingTools::ConvertUTF8ToGB(doc["data"][i]["type"].GetString(), pcTemp, 256);
					//string strTypeName(pcTemp);
					string strTypeName = doc["data"][i]["type"].GetString();

					float nPrice = doc["data"][i]["price"].GetFloat();

					TPartEntity* pNewEntity = new TPartEntity(strName, strTypeName, nPrice);
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

int TPartPool::SaveToConfig()
{
	//构造json串
	string strReturn = "{\"data\":[";
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"name\":\"%s\",\"type\":\"%s\",\"price\":%f}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetName().c_str(), m_vecList[i]->GetTypeName().c_str(), m_vecList[i]->GetPrice());
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

		strcat(tpcPath, "\\part.json");

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

