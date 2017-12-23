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

#include "InterfaceOrderDelete.h"
#include "../sqlite/DBHelper.h"
using namespace rapidjson;

CInterfaceOrderDelete::CInterfaceOrderDelete()
{
}

CInterfaceOrderDelete::~CInterfaceOrderDelete()
{
}

string CInterfaceOrderDelete::GetUrl()
{
	return I_ORDER_DEL;
}

void CInterfaceOrderDelete::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//返回参数
	bool bSuccess = false;

	//解析参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (tReqDoc.FindMember("id") != tReqDoc.MemberEnd())
	{
		int nId = tReqDoc["id"].GetInt();
		auto pCallBack = [](sqlite3_stmt* pStmt)
		{
		};

		char chSql[512] = { 0 };
		sprintf_s(chSql, 512, "DELETE FROM g_orders where rowid='%d'", nId);
		bSuccess = CDBHelper::Instance()->ExecSearch(chSql, pCallBack);
	}

	if (bSuccess)
	{
		strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
		return;
	}

	strReturn = "{\"ok\":1,\"errorinfo\":\"删除订单失败，请稍后重试！\"}";
	return;
}
