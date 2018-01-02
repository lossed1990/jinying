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

#include "InterfacePartTypeDelete.h"
using namespace rapidjson;

CInterfacePartTypeDelete::CInterfacePartTypeDelete()
{
}

CInterfacePartTypeDelete::~CInterfacePartTypeDelete()
{
}

string CInterfacePartTypeDelete::GetUrl()
{
	return I_PARTTYPE_DEL;
}

void CInterfacePartTypeDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	string chName = string(pcTemp);
	int nRet = CMainModel::Instance()->DeletePartType(chName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的配件种类不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "删除配件种类");
	return;
}
