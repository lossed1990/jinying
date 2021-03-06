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

#include "InterfacePressAdd.h"
using namespace rapidjson;

CInterfacePressAdd::CInterfacePressAdd()
{
}

CInterfacePressAdd::~CInterfacePressAdd()
{
}

string CInterfacePressAdd::GetUrl()
{
	return I_PRESS_ADD;
}

void CInterfacePressAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	string strName = string(pcTemp);
	if (strcmp(strName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增印刷机名称不能为空！\"}";
		return;
	}

	int nLen = doc["l"].GetInt();
	int nWidth = doc["w"].GetInt();

	int nRet = CMainModel::Instance()->AddPress(strName, nLen, nWidth);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"印刷机名称已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "新增印刷机");
	return;
}
