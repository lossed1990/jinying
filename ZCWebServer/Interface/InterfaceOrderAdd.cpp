﻿#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "CommonFunction.h"

#include "InterfaceOrderAdd.h"
#include "../sqlite/DBHelper.h"
using namespace rapidjson;

CInterfaceOrderAdd::CInterfaceOrderAdd()
{
	//创建空数据库，只需要执行一次即可
	//auto pCallBack = [](void *data, int argc, char **argv, char **azColName) -> int
	//{
	//	int i;
	//	fprintf(stderr, "%s: ", (const char*)data);
	//	for (i = 0; i<argc; i++){
	//		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	//	}
	//	printf("\n");
	//	return 0;
	//};

	///* Create SQL statement */
	//char* sql = "CREATE TABLE g_orders("  \
	//"CUSTOMERNAME   CHAR(64)  NOT NULL," \
	//"USERNAME       CHAR(64)  NOT NULL," \
	//"PRICE          REAL      NOT NULL," \
	//"FINALPRICE     REAL      NOT NULL," \
	//"TIME           DATETIME  NOT NULL," \
	//"CONDITION      TEXT      NOT NULL );";

	//CDBHelper::Instance()->ExecSql(sql, pCallBack);
}

CInterfaceOrderAdd::~CInterfaceOrderAdd()
{
}

string CInterfaceOrderAdd::GetUrl()
{
	return I_ORDER_ADD;
}

void CInterfaceOrderAdd::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//解析参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (tReqDoc.FindMember("customer") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("user") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("price") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("finalprice") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("condition") != tReqDoc.MemberEnd())
	{
		//char pcCustomerTemp[256] = { 0 };
		//CEncodingTools::ConvertUTF8ToGB(tReqDoc["customer"].GetString(), pcCustomerTemp, 256);
		//char pcUserTemp[256] = { 0 };
		//CEncodingTools::ConvertUTF8ToGB(tReqDoc["user"].GetString(), pcUserTemp, 256);

		string pcCustomerTemp = tReqDoc["customer"].GetString();
		string pcUserTemp = tReqDoc["user"].GetString();
		
		float fPriceTemp = tReqDoc["price"].GetFloat();
		float fFinalPriceTemp = tReqDoc["finalprice"].GetFloat();

		StringBuffer pcConditionBuffer;
		Writer<StringBuffer> writer(pcConditionBuffer);
		tReqDoc["condition"].Accept(writer);
		string pcConditionTemp = pcConditionBuffer.GetString();

		auto pCallBack = [](void *data, int argc, char **argv, char **azColName) -> int
		{
			int i;
			fprintf(stderr, "%s: ", (const char*)data);
			for (i = 0; i<argc; i++){
				printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			}
			printf("\n");
			return 0;
		};

		char chSql[8096] = { 0 };
		sprintf_s(chSql,8096,"INSERT INTO g_orders (CUSTOMERNAME,USERNAME,PRICE,FINALPRICE,TIME,CONDITION) VALUES ('%s', '%s', %.2f,  %.2f, datetime('now','localtime'),'%s');",
			pcCustomerTemp.c_str(), pcUserTemp.c_str(), fPriceTemp, fFinalPriceTemp, pcConditionTemp.c_str());

		bool bRet = CDBHelper::Instance()->ExecSql(chSql, pCallBack);
		if (bRet)
		{
			strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
			return;
		}
		else
		{
			strReturn = "{\"ok\":1,\"errorinfo\":\"插入数据失败，请稍后重试！\"}";
			return;
		}
	}

	strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数不合法，请稍后重试！\"}";
	return;
}
