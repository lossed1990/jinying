#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "InterfaceProcessOpenByName.h"
using namespace rapidjson;

CInterfaceProcessOpenByName::CInterfaceProcessOpenByName()
{
	InitProcessList();
}

CInterfaceProcessOpenByName::~CInterfaceProcessOpenByName()
{
}

string CInterfaceProcessOpenByName::GetUrl()
{
	return I_PROCESS_OPENBYNAME;
}

void CInterfaceProcessOpenByName::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//检查参数
	Document doc;
	doc.Parse(pReqBody);

	if (!doc.IsObject())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数为非法json数据\"}";
		return;
	}

	char pcTemp[256] = { 0 };
	CEncodingTools::ConvertUTF8ToGB(doc["name"].GetString(), pcTemp, 256);
	string chProcessName = string(pcTemp);

	//名称为空，表示获取所有待选择的进程
	if (strcmp(chProcessName.c_str(), "") == 0)
	{
		Document tDoc;
		tDoc.SetObject();
		Document::AllocatorType& allocator = tDoc.GetAllocator();
		if (m_vecProcess.size() > 0)
		{
			tDoc.AddMember("ok", 0, allocator);
			tDoc.AddMember("errorinfo", "", allocator);

			char szTmp[1024] = { 0 };
			Value array(kArrayType);
			for (int i = 0; i < m_vecProcess.size(); i++)
			{
				memset(szTmp, 0, sizeof(szTmp));
				strncpy_s(szTmp, sizeof(szTmp), m_vecProcess[i].name.c_str(), _TRUNCATE);
				Value vPath(kStringType);
				int nSize = m_vecProcess[i].name.size();
				vPath.SetString(m_vecProcess[i].name.c_str(), m_vecProcess[i].name.size(), allocator);
				array.PushBack(vPath, allocator);
			}

			tDoc.AddMember("data", array, allocator);
		}
		else
		{
			tDoc.AddMember("ok", 1, allocator);
			tDoc.AddMember("errorinfo", "没有找到待选择的进程", allocator);
		}

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		tDoc.Accept(writer);
		string strData = buffer.GetString();

		strReturn = strData;
	
		return;
	}

	CEncodingTools::ConvertUTF8ToGB(doc["path"].GetString(), pcTemp, 256);
	string chCurrentClientPath = string(pcTemp);
	if (strcmp(chCurrentClientPath.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"安装路径未知\"}";
		return;
	}

	//根据名称，开启进程
	strReturn = "{\"ok\":1,\"errorinfo\":\"请求进程名称未知\"}";
	for (int i = 0; i != m_vecProcess.size(); ++i)
	{
		if (m_vecProcess[i].name == chProcessName)
		{
			string chExeAddress = chCurrentClientPath + m_vecProcess[i].path + "\\" + m_vecProcess[i].exename;
			// Check for existence.   
			if ((_access_s(chExeAddress.c_str(), 0)) != 0)
			{
				strReturn = "{\"ok\":1,\"errorinfo\":\"" + chExeAddress + "不存在\"}";
				return;
			}

			string chExeAddress1 = chCurrentClientPath + m_vecProcess[i].path;
			::ShellExecute(NULL, "open", chExeAddress.c_str(), NULL, chExeAddress1.c_str(), SW_MINIMIZE);

			strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
			break;
		}	
	}

	return;
}

void CInterfaceProcessOpenByName::InitProcessList()
{
	m_vecProcess.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");

	//config file
	strcat(tpcPath, "\\process.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//配置文件不会超过4K
		char pcBuff[4096] = { 0 };
		fread(pcBuff, 1, 4096, pFile);

		Document doc;
		doc.Parse(pcBuff);

		if (doc.IsObject())
		{
			if (doc["openselect"].IsArray())
			{
				for (SizeType i = 0; i < doc["openselect"].Size(); i++)
				{
					PROCESS_OPENSELECTINFO sInfo;
					char pcTemp[256] = { 0 };

					CEncodingTools::ConvertUTF8ToGB(doc["openselect"][i]["name"].GetString(), pcTemp, 256);
					sInfo.name = string(pcTemp);

					CEncodingTools::ConvertUTF8ToGB(doc["openselect"][i]["path"].GetString(), pcTemp, 256);
					sInfo.path = string(pcTemp);

					CEncodingTools::ConvertUTF8ToGB(doc["openselect"][i]["exe"].GetString(), pcTemp, 256);
					sInfo.exename = string(pcTemp);

					m_vecProcess.push_back(sInfo);
				}
			}
		}

		fclose(pFile);
		pFile = NULL;
	}
}