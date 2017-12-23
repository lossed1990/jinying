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

#include "InterfaceMoreruleDelete.h"
using namespace rapidjson;

CInterfaceMoreruleDelete::CInterfaceMoreruleDelete()
{
}

CInterfaceMoreruleDelete::~CInterfaceMoreruleDelete()
{
}

string CInterfaceMoreruleDelete::GetUrl()
{
	return I_MORERULE_DEL;
}

void CInterfaceMoreruleDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["range"].GetString(), pcTemp, 256);
	string chName = string(pcTemp);

	memset(pcTemp, 0, 256);
	CEncodingTools::ConvertUTF8ToGB(doc["condition"].GetString(), pcTemp, 256);
	string chConditionName = string(pcTemp);

	int nRet = CMainModel::Instance()->DeleteMorerule(chConditionName, chName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的出数规则不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
