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

#include "InterfaceImpressSetCheckPrice.h"
using namespace rapidjson;

CInterfaceImpressSetCheckPrice::CInterfaceImpressSetCheckPrice()
{
}

CInterfaceImpressSetCheckPrice::~CInterfaceImpressSetCheckPrice()
{
}

string CInterfaceImpressSetCheckPrice::GetUrl()
{
	return I_IMPRESS_SETCHECKPRICE;
}

void CInterfaceImpressSetCheckPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//检查参数
	Document doc;
	doc.Parse(pReqBody);

	if (!doc.IsObject())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数为非法json数据\"}";
		return;
	}

	float nPrice = doc["checkprice"].GetFloat();
	CMainModel::Instance()->SetImpressCheckPrice(nPrice);


	strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";

	CDBHelper::Instance()->Log("管理员", "系统配置", "设置压痕校板费");
	return;
}
