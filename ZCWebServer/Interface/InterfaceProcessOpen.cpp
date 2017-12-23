#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

#include "InterfaceProcessOpen.h"
using namespace rapidjson;

CInterfaceProcessOpen::CInterfaceProcessOpen()
{
}

CInterfaceProcessOpen::~CInterfaceProcessOpen()
{
}

string CInterfaceProcessOpen::GetUrl()
{
	return I_PROCESS_OPEN;
}

void CInterfaceProcessOpen::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	if (strcmp(chProcessName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求进程名为空\"}";
		return;
	}

	CEncodingTools::ConvertUTF8ToGB(doc["path"].GetString(), pcTemp, 256);
	string chCurrentClientPath = string(pcTemp);
	if (strcmp(chCurrentClientPath.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求安装路径为空\"}";
		return;
	}

	//根据名称，开启进程
	strReturn = "{\"ok\":1,\"errorinfo\":\"请求进程名称未知\"}";
	
	string chExeAddress = chCurrentClientPath + "\\" + chProcessName;
	// Check for existence.   
	if ((_access_s(chExeAddress.c_str(), 0)) != 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"路径下文件不存在\"}";
		return;
	}

	string chExeAddress1 = chCurrentClientPath;
	::ShellExecute(NULL, "open", chExeAddress.c_str(), NULL, chExeAddress1.c_str(), SW_SHOW);

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}