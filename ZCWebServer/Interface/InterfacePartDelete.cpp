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

#include "InterfacePartDelete.h"
using namespace rapidjson;

CInterfacePartDelete::CInterfacePartDelete()
{
}

CInterfacePartDelete::~CInterfacePartDelete()
{
}

string CInterfacePartDelete::GetUrl()
{
	return I_PART_DEL;
}

void CInterfacePartDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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

	memset(pcTemp, 256, 0);
	CEncodingTools::ConvertUTF8ToGB(doc["type"].GetString(), pcTemp, 256);
	string chTypeName = string(pcTemp);

	int nRet = CMainModel::Instance()->DeletePart(chName, chTypeName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的配件不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "删除配件");
	return;
}
