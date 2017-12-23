#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "CommonFunction.h"

#include "InterfaceProcessClose.h"
using namespace rapidjson;

CInterfaceProcessClose::CInterfaceProcessClose()
{
}

CInterfaceProcessClose::~CInterfaceProcessClose()
{
}

string CInterfaceProcessClose::GetUrl()
{
	return I_PROCESS_CLOSE;
}

void CInterfaceProcessClose::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求的进程名称为空\"}";
		return;
	}

	BOOL bRet = CCommonFunction::DestoryProcess(chProcessName.c_str());
	if (bRet)
	{
	    strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	}
	else
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"关闭进程失败！\"}";
	}

	return;
}
