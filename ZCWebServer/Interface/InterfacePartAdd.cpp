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

#include "InterfacePartAdd.h"
using namespace rapidjson;

CInterfacePartAdd::CInterfacePartAdd()
{
}

CInterfacePartAdd::~CInterfacePartAdd()
{
}

string CInterfacePartAdd::GetUrl()
{
	return I_PART_ADD;
}

void CInterfacePartAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增配件名称不能为空！\"}";
		return;
	}

	memset(pcTemp, 0, 256);
	CEncodingTools::ConvertUTF8ToGB(doc["type"].GetString(), pcTemp, 256);
	string strTypeName = string(pcTemp);
	if (strcmp(strTypeName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增配件种类不能为空！\"}";
		return;
	}

	float nPrcie = doc["price"].GetFloat();

	int nRet = CMainModel::Instance()->AddPart(strName, strTypeName, nPrcie);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增配件已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
