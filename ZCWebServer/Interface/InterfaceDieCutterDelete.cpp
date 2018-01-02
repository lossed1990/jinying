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

#include "InterfaceDieCutterDelete.h"
using namespace rapidjson;

CInterfaceDieCutterDelete::CInterfaceDieCutterDelete()
{
}

CInterfaceDieCutterDelete::~CInterfaceDieCutterDelete()
{
}

string CInterfaceDieCutterDelete::GetUrl()
{
	return I_DIECUTTER_DEL;
}

void CInterfaceDieCutterDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["type"].GetString(), pcTemp, 256);
	string chName = string(pcTemp);

	memset(pcTemp,0,256);
	CEncodingTools::ConvertUTF8ToGB(doc["producttype"].GetString(), pcTemp, 256);
	string chTypeName = string(pcTemp);

	int nRet = CMainModel::Instance()->DeleteDieCutter(chTypeName, chName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的刀模不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "删除刀模规格");
	return;
}
