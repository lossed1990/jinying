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

#include "InterfaceDieCutterAdd.h"
using namespace rapidjson;

CInterfaceDieCutterAdd::CInterfaceDieCutterAdd()
{
}

CInterfaceDieCutterAdd::~CInterfaceDieCutterAdd()
{
}

string CInterfaceDieCutterAdd::GetUrl()
{
	return I_DIECUTTER_ADD;
}

void CInterfaceDieCutterAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["producttype"].GetString(), pcTemp, 256);
	string strName = string(pcTemp);

	int nLen = doc["l"].GetInt();
	int nWidth = doc["w"].GetInt();
	int nHeight = doc["h"].GetInt();

	int nRet = CMainModel::Instance()->AddDieCutter(strName, nLen, nWidth, nHeight);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"刀模规格已存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "新增刀模规格");
	return;
}
