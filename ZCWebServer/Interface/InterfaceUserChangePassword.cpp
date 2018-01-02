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

#include "InterfaceUserChangePassword.h"
using namespace rapidjson;

CInterfaceUserChangePassword::CInterfaceUserChangePassword()
{
}

CInterfaceUserChangePassword::~CInterfaceUserChangePassword()
{
}

string CInterfaceUserChangePassword::GetUrl()
{
	return I_USER_CHANGEPW;
}

void CInterfaceUserChangePassword::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
		strReturn = "{\"ok\":1,\"errorinfo\":\"修改密码失败，用户名不能为空！\"}";
		return;
	}

	CEncodingTools::ConvertUTF8ToGB(doc["oldpw"].GetString(), pcTemp, 256);
	string chOldPw = string(pcTemp);
	if (strcmp(chOldPw.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"修改密码失败，原始密码不能为空！\"}";
		return;
	}

	CEncodingTools::ConvertUTF8ToGB(doc["newpw"].GetString(), pcTemp, 256);
	string chNewPw = string(pcTemp);
	if (strcmp(chNewPw.c_str(), "") == 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"修改密码失败，新密码不能为空！\"}";
		return;
	}

	int nRet = CMainModel::Instance()->ChangeUserPassword(chUserName, chOldPw, chNewPw);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"修改密码失败，用户不存在！\"}";
		return;
	}
	else if (nRet == -2)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"修改密码失败，原始密码错误！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log(pcTemp, "修改密码", "修改密码");
	return;
}
