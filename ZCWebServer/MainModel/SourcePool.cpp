#include "SourcePool.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "SourceSelectPlanEntity.h"
using namespace rapidjson;

TSourcePool::TSourcePool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TSourcePool::~TSourcePool()
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

int TSourcePool::AddSource(string& strName, string& strTypeName, int nLen, int nWidth, int nKezhong, float nDunjia, int nUseType)
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

	TSourceEntity* pNewEntity = new TSourceEntity(strName, strTypeName, nLen, nWidth, nKezhong, nDunjia, nUseType);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TSourcePool::DeleteSource(string& strName)
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
	return -1;  //删除失败，不存在
}

void TSourcePool::EachSource(function<void(TSourceEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TSourcePool::GetSourceCount()
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

bool TSourcePool::CountSourceMinPrice(string& strTypeName, int nOpenLen, int nOpenWidth, int nTotalCount,int nNeedCount, float& fPrice, vector<LogInfo>& vecLogInfo)
{
	m_csList.Enter();
	float fMinPrice = 100000000;
	bool bSuccess = false;
	for (auto& item : m_vecList)
	{
		if (item->GetTypeName() == strTypeName)
		{
			TSourceSelectPlanEntity tInfo(nOpenLen, nOpenWidth, nTotalCount, nNeedCount, item);
			float fCurrentPrice = 0;
			bool bRet = tInfo.GetMinPrice(fCurrentPrice, vecLogInfo);
			if (bRet)
			{
				if (fCurrentPrice < fMinPrice)
				{
					fMinPrice = fCurrentPrice;
				}

				bSuccess = true;
			}
		}
	}
	fPrice = fMinPrice;
	m_csList.Leave();
	return bSuccess;
}

bool TSourcePool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\source.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//配置文件不会超过1M
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
					string strName = doc["data"][i]["name"].GetString();
					string strTypeName = doc["data"][i]["typename"].GetString();

					int nLen = doc["data"][i]["l"].GetInt();
					int nWidth = doc["data"][i]["w"].GetInt();
					int nKezhong = doc["data"][i]["kez"].GetInt();
					float nDunjia = doc["data"][i]["dunj"].GetFloat();
					int nUseType = doc["data"][i]["usetype"].GetInt();

					TSourceEntity* pNewEntity = new TSourceEntity(strName, strTypeName, nLen, nWidth, nKezhong, nDunjia, nUseType);
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

int TSourcePool::SaveToConfig()
{
	//构造json串
	string strReturn = "{\"data\":[";
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"name\":\"%s\",\"typename\":\"%s\",\"l\":%d,\"w\":%d,\"kez\":%d,\"dunj\":%f,\"usetype\":%d}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetName().c_str(), m_vecList[i]->GetTypeName().c_str(), m_vecList[i]->GetLen(), m_vecList[i]->GetWidth(), m_vecList[i]->GetKezhong(), m_vecList[i]->GetDunjia(), m_vecList[i]->GetUseType());
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

		strcat(tpcPath, "\\source.json");

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

