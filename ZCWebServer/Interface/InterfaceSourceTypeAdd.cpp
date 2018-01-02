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

#include "InterfaceSourceTypeAdd.h"
using namespace rapidjson;

CInterfaceSourceTypeAdd::CInterfaceSourceTypeAdd()
{
}

CInterfaceSourceTypeAdd::~CInterfaceSourceTypeAdd()
{
}

string CInterfaceSourceTypeAdd::GetUrl()
{
	return I_SOURCETYPE_ADD;
}

void CInterfaceSourceTypeAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增材料类别名称不能为空！\"}";
		return;
	}

	int nRet = CMainModel::Instance()->AddSourceType(strName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"材料类别已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "新增材料类别");
	return;
}
