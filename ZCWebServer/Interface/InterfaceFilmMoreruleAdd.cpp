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

#include "InterfaceFilmMoreruleAdd.h"
using namespace rapidjson;

CInterfaceFilmMoreruleAdd::CInterfaceFilmMoreruleAdd()
{
}

CInterfaceFilmMoreruleAdd::~CInterfaceFilmMoreruleAdd()
{
}

string CInterfaceFilmMoreruleAdd::GetUrl()
{
	return I_FILM_MORERULE_ADD;
}

void CInterfaceFilmMoreruleAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//检查参数
	Document doc;
	doc.Parse(pReqBody);

	if (!doc.IsObject())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数为非法json数据\"}";
		return;
	}

	int nBegin = doc["begin"].GetInt();
	int nEnd = doc["end"].GetInt();
	int nCount = doc["count"].GetInt();

	int nRet = CMainModel::Instance()->AddFilmMorerule(nBegin, nEnd, nCount);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"规则区间已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "新增覆膜出数规则");
	return;
}
