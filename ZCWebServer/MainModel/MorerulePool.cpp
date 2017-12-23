#include "MorerulePool.h"
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

TMorerulePool::TMorerulePool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TMorerulePool::~TMorerulePool()
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

int TMorerulePool::AddMorerule(bool bPress, bool bTangjin, int nBegin, int nEnd, int nCount)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetIsPress() == bPress && item->GetIsTangjin() == bTangjin && item->GetBegin() == nBegin && item->GetEnd() == nEnd)
		{
			m_csList.Leave();
			return -1;  //新增失败，范围已存在
		}
	}

	TMoreruleEntity* pNewEntity = new TMoreruleEntity(bPress, bTangjin, nBegin, nEnd, nCount);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TMorerulePool::DeleteMorerule(string& strCondition, string& strRange)
{
	m_csList.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		char chRange[64] = { 0 }; //数量区间
		sprintf_s(chRange, "%d-%d", (*iter)->GetBegin(), (*iter)->GetEnd());
		if (strRange == chRange && (*iter)->GetConditionString() == strCondition)
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

void TMorerulePool::EachMorerules(function<void(TMoreruleEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TMorerulePool::GetMoreruleCount() 
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

int TMorerulePool::GetMoreCountByMoreRule(bool bPress, bool bTangjin, int nNumber)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetIsPress() == bPress && item->GetIsTangjin() == bTangjin && item->GetBegin() <= nNumber && item->GetEnd() >= nNumber)
		{
			int nCount = item->GetCount();
			m_csList.Leave();
			return nCount;
		}
	}

	m_csList.Leave();
	return 0;
}

bool TMorerulePool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\morerule.json");

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
					bool bPress = doc["data"][i]["bpress"].GetInt();
					bool bTangjin = doc["data"][i]["btangjin"].GetInt();

					TMoreruleEntity* pNewEntity = new TMoreruleEntity(bPress, bTangjin, nBegin, nEnd, nCount);
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

int TMorerulePool::SaveToConfig()
{
	//构造json串
	string strReturn = "{\"data\":[";
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"bpress\":%d,\"btangjin\":%d,\"begin\":%d,\"end\":%d,\"count\":%d}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetIsPress(), m_vecList[i]->GetIsTangjin(),m_vecList[i]->GetBegin(), m_vecList[i]->GetEnd(), m_vecList[i]->GetCount());
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

		strcat(tpcPath, "\\morerule.json");

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

