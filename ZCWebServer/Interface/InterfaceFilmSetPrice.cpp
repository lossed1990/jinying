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

#include "InterfaceFilmSetPrice.h"
using namespace rapidjson;

CInterfaceFilmSetPrice::CInterfaceFilmSetPrice()
{
}

CInterfaceFilmSetPrice::~CInterfaceFilmSetPrice()
{
}

string CInterfaceFilmSetPrice::GetUrl()
{
	return I_FILM_SETPRICE;
}

void CInterfaceFilmSetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CMainModel::Instance()->SetFilmPrice(nPrice);


	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	return;
}
