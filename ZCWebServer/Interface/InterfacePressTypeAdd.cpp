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

#include "InterfacePressTypeAdd.h"
using namespace rapidjson;

CInterfacePressTypeAdd::CInterfacePressTypeAdd()
{
}

CInterfacePressTypeAdd::~CInterfacePressTypeAdd()
{
}

string CInterfacePressTypeAdd::GetUrl()
{
	return I_PRESSTYPE_ADD;
}

void CInterfacePressTypeAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增印刷方式名称不能为空！\"}";
		return;
	}

	int nRet = CMainModel::Instance()->AddPressType(strName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"印刷方式已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
