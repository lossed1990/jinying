﻿#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "CommonFunction.h"

#include "InterfaceSourceDelete.h"
using namespace rapidjson;

CInterfaceSourceDelete::CInterfaceSourceDelete()
{
}

CInterfaceSourceDelete::~CInterfaceSourceDelete()
{
}

string CInterfaceSourceDelete::GetUrl()
{
	return I_SOURCE_DEL;
}

void CInterfaceSourceDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	string chName = string(pcTemp);
	int nRet = CMainModel::Instance()->DeleteSource(chName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的材料名称不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}