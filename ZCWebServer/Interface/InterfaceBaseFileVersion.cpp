#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "InterfaceBaseFileVersion.h"
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "CommonFunction.h"
using namespace rapidjson;

CInterfaceBaseFileVersion::CInterfaceBaseFileVersion()
{
	InitFilesList();
}

CInterfaceBaseFileVersion::~CInterfaceBaseFileVersion()
{
}

string CInterfaceBaseFileVersion::GetUrl()
{
	return I_BASE_GET_FILEVERSION;
}

void CInterfaceBaseFileVersion::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//检查参数
	Document doc;
	doc.Parse(pReqBody);

	if (!doc.IsObject())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数为非法json数据\"}";
		return;
	}

	string chCurrentClientPath = doc["path"].GetString();
	if (strcmp(chCurrentClientPath.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求路径为空\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\",\"data\":[";
	for (int i = 0; i != m_vecFiles.size();++i)
	{
		char pcSubTemp[256] = { 0 };
		static const char* SUBTEMPLATE = "{\"tip\":\"%s\",\"name\":\"%s\",\"version\":\"%s\"}";

		char chPath[_MAX_PATH] = { 0 };
		sprintf_s(chPath, _MAX_PATH, "%s%s\\%s", chCurrentClientPath.c_str(), m_vecFiles[i].path.c_str(),m_vecFiles[i].name.c_str());
		BOOL bRet = CCommonFunction::GetVersion(chPath);
		if (bRet){
			sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecFiles[i].tip.c_str(), m_vecFiles[i].name.c_str(), chPath);
		}
		else{
			sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecFiles[i].tip.c_str(), m_vecFiles[i].name.c_str(), "未知");
		}

		strReturn = strReturn + pcSubTemp;

		if (i != (m_vecFiles.size() - 1))
		{
			strReturn = strReturn + ",";
		}
	}

	strReturn = strReturn + "]}";
	return;
}

void CInterfaceBaseFileVersion::InitFilesList()
{
	m_vecFiles.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");

	//config file
	strcat(tpcPath, "\\basefiles.json");

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
			if (doc["files"].IsArray())
			{
				for (SizeType i = 0; i < doc["files"].Size(); i++)
				{
					BASE_FILEINFO sInfo;
					char pcTemp[256] = { 0 };

					CEncodingTools::ConvertUTF8ToGB(doc["files"][i]["tip"].GetString(), pcTemp, 256);
					sInfo.tip = string(pcTemp);

					CEncodingTools::ConvertUTF8ToGB(doc["files"][i]["name"].GetString(), pcTemp, 256);
					sInfo.name = string(pcTemp);

					CEncodingTools::ConvertUTF8ToGB(doc["files"][i]["path"].GetString(), pcTemp, 256);
					sInfo.path = string(pcTemp);

					m_vecFiles.push_back(sInfo);
				}
			}
		}

		fclose(pFile);
		pFile = NULL;
	}
}