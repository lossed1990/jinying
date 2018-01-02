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

#include "InterfaceImpressSetPrice.h"
using namespace rapidjson;

CInterfaceImpressSetPrice::CInterfaceImpressSetPrice()
{
}

CInterfaceImpressSetPrice::~CInterfaceImpressSetPrice()
{
}

string CInterfaceImpressSetPrice::GetUrl()
{
	return I_IMPRESS_SETPRICE;
}

void CInterfaceImpressSetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
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
	CMainModel::Instance()->SetImpressPrice(nPrice);


	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
	CDBHelper::Instance()->Log("管理员", "系统配置", "设置压痕单价");
	return;
}
