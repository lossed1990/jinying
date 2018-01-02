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

#include "InterfaceUserAdd.h"
using namespace rapidjson;

CInterfaceUserAdd::CInterfaceUserAdd()
{
}

CInterfaceUserAdd::~CInterfaceUserAdd()
{
}

string CInterfaceUserAdd::GetUrl()
{
	return I_USER_ADD;
}

void CInterfaceUserAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	string chUserName = string(pcTemp);
	if (strcmp(chUserName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增用户名不能为空！\"}";
		return;
	}

	CEncodingTools::ConvertUTF8ToGB(doc["pw"].GetString(), pcTemp, 256);
	string chPwName = string(pcTemp);
	if (strcmp(chPwName.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"新增用户密码不能为空！\"}";
		return;
	}

	int nUserType = doc["type"].GetInt();
	int nRet = CMainModel::Instance()->AddUser(chUserName, chPwName, nUserType);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"用户名已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "新增用户");
	return;
}
