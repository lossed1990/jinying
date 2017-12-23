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

#include "InterfacePressPriceAdd.h"
using namespace rapidjson;

CInterfacePressPriceAdd::CInterfacePressPriceAdd()
{
}

CInterfacePressPriceAdd::~CInterfacePressPriceAdd()
{
}

string CInterfacePressPriceAdd::GetUrl()
{
	return I_PRESSPRICE_ADD;
}

void CInterfacePressPriceAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["pressname"].GetString(), pcTemp, 256);
	string strPressName = string(pcTemp);
	if (strcmp(strPressName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增印刷机名称不能为空！\"}";
		return;
	}

	memset(pcTemp, 0, 256);
	CEncodingTools::ConvertUTF8ToGB(doc["presstypename"].GetString(), pcTemp, 256);
	string strPressTypeName = string(pcTemp);
	if (strcmp(strPressTypeName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增印刷方式名称不能为空！\"}";
		return;
	}

	int nBegin = doc["begin"].GetInt();
	int nEnd = doc["end"].GetInt();
	float nPrcie = doc["price"].GetFloat();

	int nRet = CMainModel::Instance()->AddPressprice(strPressName, strPressTypeName, nBegin, nEnd, nPrcie);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增印刷价格规则已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
