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

#include "InterfaceSourceAdd.h"
using namespace rapidjson;

CInterfaceSourceAdd::CInterfaceSourceAdd()
{
}

CInterfaceSourceAdd::~CInterfaceSourceAdd()
{
}

string CInterfaceSourceAdd::GetUrl()
{
	return I_SOURCE_ADD;
}

void CInterfaceSourceAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增材料名不能为空！\"}";
		return;
	}

	memset(pcTemp, 0, 256);
	CEncodingTools::ConvertUTF8ToGB(doc["typename"].GetString(), pcTemp, 256);
	string strTypeName = string(pcTemp);

	int nLen = doc["l"].GetInt();
	int nWidth = doc["w"].GetInt();
	int nKezhong = doc["kez"].GetInt();
	float nDunjia = doc["dunj"].GetFloat();
	int nUseType = doc["usetype"].GetInt();

	int nRet = CMainModel::Instance()->AddSource(strName, strTypeName, nLen, nWidth, nKezhong, nDunjia, nUseType);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"材料名已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
