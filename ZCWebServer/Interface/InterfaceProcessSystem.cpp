#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "InterfaceProcessSystem.h"
using namespace rapidjson;

CInterfaceProcessSystem::CInterfaceProcessSystem()
{
}

CInterfaceProcessSystem::~CInterfaceProcessSystem()
{
}

string CInterfaceProcessSystem::GetUrl()
{
	return I_PROCESS_SYSTEM;
}

void CInterfaceProcessSystem::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["command"].GetString(), pcTemp, 256);
	string chCommandName = string(pcTemp);
	if (strcmp(chCommandName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求名利为空\"}";
		return;
	}

	system(chCommandName.c_str());

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}