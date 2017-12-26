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

#include "InterfaceOrderGetCondition.h"
#include "../sqlite/DBHelper.h"
using namespace rapidjson;

CInterfaceOrderGetCondition::CInterfaceOrderGetCondition()
{
}

CInterfaceOrderGetCondition::~CInterfaceOrderGetCondition()
{
}

string CInterfaceOrderGetCondition::GetUrl()
{
	return I_ORDER_GETCONDITION;
}

void CInterfaceOrderGetCondition::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//返回参数
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	bool bSuccess = false;

	//解析参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (tReqDoc.FindMember("id") != tReqDoc.MemberEnd())
	{
		int nId = tReqDoc["id"].GetInt();
		auto pCallBack = [&tDoc, &allocator, &bSuccess](sqlite3_stmt* pStmt)
		{
			const char* pCondition = (const char*)sqlite3_column_text(pStmt, 6);
			char pcConditionTemp[8196] = { 0 };
			CEncodingTools::ConvertUTF8ToGB(pCondition, pcConditionTemp, 8196);
			Value vCondition(kStringType);
			vCondition.SetString(pcConditionTemp, strlen(pcConditionTemp), allocator);

			tDoc.AddMember("condition", vCondition, allocator);
			bSuccess = true;
		};

		char chSql[512] = { 0 };
		sprintf_s(chSql, 512, "SELECT * from g_orders where rowid='%d'", nId);

		CDBHelper::Instance()->ExecSearch(chSql, pCallBack);
	}

	if (bSuccess)
	{
		Value vOk(kNumberType);
		vOk.SetInt(0);
		tDoc.AddMember("ok", vOk, allocator);
	}
	else
	{
		Value vOk(kNumberType);
		vOk.SetInt(1);
		tDoc.AddMember("ok", vOk, allocator);

		Value vErrorInfo(kStringType);
		char chErroInfo[64] = { 0 };
		strcpy_s(chErroInfo, 64, "查询详情失败!");
		vErrorInfo.SetString(chErroInfo, strlen(chErroInfo), allocator);
		tDoc.AddMember("errorinfo", vErrorInfo, allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
