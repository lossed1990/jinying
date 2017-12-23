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

#include "InterfaceMoreruleAdd.h"
using namespace rapidjson;

CInterfaceMoreruleAdd::CInterfaceMoreruleAdd()
{
}

CInterfaceMoreruleAdd::~CInterfaceMoreruleAdd()
{
}

string CInterfaceMoreruleAdd::GetUrl()
{
	return I_MORERULE_ADD;
}

void CInterfaceMoreruleAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	bool bPress = doc["bpress"].GetBool();
	bool bTangjin = doc["btangjin"].GetBool();

	int nRet = CMainModel::Instance()->AddMorerule(bPress, bTangjin, nBegin, nEnd, nCount);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"规则区间已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
