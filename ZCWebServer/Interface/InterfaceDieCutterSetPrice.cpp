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

#include "InterfaceDieCutterSetPrice.h"
using namespace rapidjson;

CInterfaceDieCutterSetPrice::CInterfaceDieCutterSetPrice()
{
}

CInterfaceDieCutterSetPrice::~CInterfaceDieCutterSetPrice()
{
}

string CInterfaceDieCutterSetPrice::GetUrl()
{
	return I_DIECUTTER_SETPRICE;
}

void CInterfaceDieCutterSetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//检查参数
	Document doc;
	doc.Parse(pReqBody);

	if (!doc.IsObject())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数为非法json数据\"}";
		return;
	}

	float nPrice = doc["price"].GetFloat();
	CMainModel::Instance()->SetDieCutterPrice(nPrice);

	float nBoxPrice = doc["boxprice"].GetFloat();
	CMainModel::Instance()->SetBoxDieCutterPrice(nBoxPrice);


	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
