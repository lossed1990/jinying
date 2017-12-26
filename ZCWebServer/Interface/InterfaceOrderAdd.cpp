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

#include "InterfaceOrderAdd.h"
#include "../sqlite/DBHelper.h"
using namespace rapidjson;

CInterfaceOrderAdd::CInterfaceOrderAdd()
{
	////创建空数据库，只需要执行一次即可
	//auto pCallBack = [](void *data, int argc, char **argv, char **azColName) -> int
	//{
	//	int i;
	//	fprintf(stderr, "%s: ", (const char*)data);
	//	for (i = 0; i < argc; i++){
	//		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	//	}
	//	printf("\n");
	//	return 0;
	//};

	///* Create SQL statement */
	//char* sql = "CREATE TABLE g_orders("  \
		//	"ORDERRNAME     TEXT      PRIMARY KEY   NOT NULL," \
		//	"CUSTOMERNAME   CHAR(64)  NOT NULL," \
		//	"USERNAME       CHAR(64)  NOT NULL," \
		//	"PRICE          REAL      NOT NULL," \
		//	"FINALPRICE     REAL      NOT NULL," \
		//	"TIME           DATETIME  NOT NULL," \
		//	"CONDITION      TEXT      NOT NULL );";

	//fprintf(stdout, "CDBHelper::CREATE TABLE\n");
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

	if (tReqDoc.FindMember("type") != tReqDoc.MemberEnd() && 
		tReqDoc.FindMember("order") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("customer") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("user") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("price") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("finalprice") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("condition") != tReqDoc.MemberEnd())
	{
		int nType = tReqDoc["type"].GetInt(); //0-新增  1-修改
		string pcOrderNameTemp = tReqDoc["order"].GetString();
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
			for (i = 0; i < argc; i++){
				printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			}
			printf("\n");
			return 0;
		};

		//主键存在，则覆盖之前的，否则新增
		/*char chSql[8096] = { 0 };
		sprintf_s(chSql, 8096, "INSERT OR REPLACE INTO g_orders (ORDERRNAME,CUSTOMERNAME,USERNAME,PRICE,FINALPRICE,TIME,CONDITION) VALUES ('%s', '%s', '%s', %.2f,  %.2f, datetime('now','localtime'),'%s');",
			pcOrderNameTemp.c_str(), pcCustomerTemp.c_str(), pcUserTemp.c_str(), fPriceTemp, fFinalPriceTemp, pcConditionTemp.c_str());*/
		char chSql[8096] = { 0 };
		if (nType == 0)
		{
			sprintf_s(chSql, 8096, "INSERT INTO g_orders (ORDERRNAME,CUSTOMERNAME,USERNAME,PRICE,FINALPRICE,TIME,CONDITION) VALUES ('%s', '%s', '%s', %.2f,  %.2f, datetime('now','localtime'),'%s');",
				pcOrderNameTemp.c_str(), pcCustomerTemp.c_str(), pcUserTemp.c_str(), fPriceTemp, fFinalPriceTemp, pcConditionTemp.c_str());
		}
		else
		{
			sprintf_s(chSql, 8096, "INSERT OR REPLACE INTO g_orders (ORDERRNAME,CUSTOMERNAME,USERNAME,PRICE,FINALPRICE,TIME,CONDITION) VALUES ('%s', '%s', '%s', %.2f,  %.2f, datetime('now','localtime'),'%s');",
				pcOrderNameTemp.c_str(), pcCustomerTemp.c_str(), pcUserTemp.c_str(), fPriceTemp, fFinalPriceTemp, pcConditionTemp.c_str()); 
		}
		
		bool bRet = CDBHelper::Instance()->ExecSql(chSql, pCallBack);
		if (bRet)
		{
			strReturn = "{\"ok\":0,\"errorinfo\":\"\"}";
			return;
		}
		else
		{
			strReturn = "{\"ok\":1,\"errorinfo\":\"订单已存在，请修改订单名称！\"}";
			return;
		}
	}

	strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数不合法，请稍后重试！\"}";
	return;
}
