#include "DieCutterPool.h"
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

TDieCutterPool::TDieCutterPool()
{
	m_csList.Enter();
	LoadFromConfig();
	m_csList.Leave();
}

TDieCutterPool::~TDieCutterPool()
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

int TDieCutterPool::AddDieCutter(string& strTypeName,int nLen, int nWidth, int nHeight)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetTypeName() == strTypeName && item->GetLen() == nLen && item->GetWidth() == nWidth && item->GetHeight() == nHeight)
		{
			m_csList.Leave();
			return -1;  //新增失败，已存在
		}
	}

	TDieCutterEntity* pNewEntity = new TDieCutterEntity(strTypeName, nLen, nWidth, nHeight);
	m_vecList.push_back(pNewEntity);
	SaveToConfig();
	m_csList.Leave();
	return 0;
}

int TDieCutterPool::DeleteDieCutter(string& strTypeName, string& strName)
{
	m_csList.Enter();
	for (auto iter = m_vecList.begin(); iter != m_vecList.end(); ++iter)
	{
		char chRange[64] = { 0 }; //数量区间
		sprintf_s(chRange, "%d*%d*%d", (*iter)->GetLen(), (*iter)->GetWidth(), (*iter)->GetHeight());
		if ((*iter)->GetTypeName() == strTypeName && strName == chRange)
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

void TDieCutterPool::EachDieCutter(function<void(TDieCutterEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

void TDieCutterPool::SetDieCutterPrice(float nPrice)
{
	m_csList.Enter();
	m_nPrice = nPrice;
	SaveToConfig();
	m_csList.Leave();
}

void TDieCutterPool::SetBoxDieCutterPrice(float nPrice)
{
	m_csList.Enter();
	m_nBoxPrice = nPrice;
	SaveToConfig();
	m_csList.Leave();
}

int TDieCutterPool::GetDieCutterCount()
{
	m_csList.Enter();
	int nCount = m_vecList.size();
	m_csList.Leave();
	return nCount;
}

float TDieCutterPool::GetDieCutterPrice()
{
	m_csList.Enter();
	float fPrice =  m_nPrice;
	m_csList.Leave();
	return fPrice;
}

float TDieCutterPool::GetBoxDieCutterPrice()
{
	m_csList.Enter();
	float fPrice = m_nBoxPrice;
	m_csList.Leave();
	return fPrice;
}

bool TDieCutterPool::IsHavingDieCutter(int nLen, int nWidth, int nHeight)
{
	bool bRet = false;
	m_csList.Enter();
	for (auto& item : m_vecList)
	{
		if (item->GetLen() == nLen && item->GetWidth() == nWidth && item->GetHeight() == nHeight)
		{
			bRet = true;
			break;
		}
	}
	m_csList.Leave();
	return bRet;
}

bool TDieCutterPool::LoadFromConfig()
{
	m_vecList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\diecutter.json");

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
					string strTypeName = doc["data"][i]["typename"].GetString();
					int nLen = doc["data"][i]["l"].GetInt();
					int nWidth = doc["data"][i]["w"].GetInt();
					int nHeight = doc["data"][i]["h"].GetInt();

					TDieCutterEntity* pNewEntity = new TDieCutterEntity(strTypeName,nLen, nWidth, nHeight);
					m_vecList.push_back(pNewEntity);
				}

				m_nPrice = doc["price"].GetFloat();
				m_nBoxPrice = doc["boxprice"].GetFloat();
			}
		}

		delete[] pcBuff;
		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TDieCutterPool::SaveToConfig()
{
	char chHead[64] = { 0 };
	sprintf_s(chHead, 64, "{\"price\":%f,\"boxprice\":%f,\"data\":[", m_nPrice, m_nBoxPrice);

	//构造json串
	string strReturn = chHead;
	for (int i = 0; i != m_vecList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"typename\":\"%s\",\"l\":%d,\"w\":%d,\"h\":%d}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecList[i]->GetTypeName().c_str(),m_vecList[i]->GetLen(), m_vecList[i]->GetWidth(), m_vecList[i]->GetHeight());
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

		strcat(tpcPath, "\\diecutter.json");

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

