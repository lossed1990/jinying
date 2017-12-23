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

#include "InterfacePressPriceDelete.h"
using namespace rapidjson;

CInterfacePressPriceDelete::CInterfacePressPriceDelete()
{
}

CInterfacePressPriceDelete::~CInterfacePressPriceDelete()
{
}

string CInterfacePressPriceDelete::GetUrl()
{
	return I_PRESSPRICE_DEL;
}

void CInterfacePressPriceDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CEncodingTools::ConvertUTF8ToGB(doc["pressname"].GetString(), pcTemp, 256);
	string chPressName = string(pcTemp);

	memset(pcTemp, 256, 0);
	CEncodingTools::ConvertUTF8ToGB(doc["presstypename"].GetString(), pcTemp, 256);
	string chPressTypeName = string(pcTemp);

	memset(pcTemp, 256, 0);
	CEncodingTools::ConvertUTF8ToGB(doc["range"].GetString(), pcTemp, 256);
	string chRangeName = string(pcTemp);

	int nRet = CMainModel::Instance()->DeletePressprice(chPressName, chPressTypeName, chRangeName);
	if (nRet == -1)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"待删除的印刷价格不存在！\"}";
		return;
	}

	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
