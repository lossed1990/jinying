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

#include "InterfaceProcessIsExist.h"
using namespace rapidjson;

CInterfaceProcessIsExist::CInterfaceProcessIsExist()
{
}

CInterfaceProcessIsExist::~CInterfaceProcessIsExist()
{
}

string CInterfaceProcessIsExist::GetUrl()
{
	return I_PROCESS_ISEXIST;
}

void CInterfaceProcessIsExist::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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

	BOOL bRet = CCommonFunction::CheckProcessExist(pcTemp);
	if (bRet)
	{
		strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	}
	else
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"进程不存在\"}";
	}

	return;
}