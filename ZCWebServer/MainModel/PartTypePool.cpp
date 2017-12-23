#include "PartTypePool.h"
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

TPartTypePool::TPartTypePool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TPartTypePool::~TPartTypePool()
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

int TPartTypePool::AddPartType(string& strName)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetName() == strName)
		{
			m_csList.Leave();
			return -1;  //新增失败，已存在
		}
	}

	TPartTypeEntity* pNewEntity = new TPartTypeEntity(strName);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TPartTypePool::DeletePartType(string& strName)
{
	m_csList.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		if ((*iter)->GetName() == strName)
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

void TPartTypePool::EachPartType(function<void(TPartTypeEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TPartTypePool::GetPartTypeCount()
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

bool TPartTypePool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\parttype.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		char* pcBuff = new char[1024 * 1024];
		memset(pcBuff, 0, 1024 * 1024);
		fread(pcBuff, 1, 1024 * 1024, pFile);

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

					TPartTypeEntity* pNewEntity = new TPartTypeEntity(strName);
					m_vecList.push_back(pNewEntity);
				}
			}
		}

		delete[] pcBuff;
		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TPartTypePool::SaveToConfig()
{
	//构造json串
	string strReturn = "{\"data\":[";
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"name\":\"%s\"}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetName().c_str());
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

		strcat(tpcPath, "\\parttype.json");

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

