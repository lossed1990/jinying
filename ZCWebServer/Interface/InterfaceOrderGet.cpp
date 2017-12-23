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

#include "InterfaceOrderGet.h"
#include "../sqlite/DBHelper.h"
using namespace rapidjson;

CInterfaceOrderGet::CInterfaceOrderGet()
{
}

CInterfaceOrderGet::~CInterfaceOrderGet()
{
}

string CInterfaceOrderGet::GetUrl()
{
	return I_ORDER_GET;
}

void CInterfaceOrderGet::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//返回参数
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	Value array(kArrayType);

	//解析参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (tReqDoc.FindMember("user") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("begin") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("end") != tReqDoc.MemberEnd())
	{
		/*char pcUserTemp[128] = { 0 };
		CEncodingTools::ConvertUTF8ToGB(tReqDoc["user"].GetString(), pcUserTemp, 128);
		char pcBeginTemp[32] = { 0 };
		CEncodingTools::ConvertUTF8ToGB(tReqDoc["begin"].GetString(), pcBeginTemp, 32);
		char pcEndTemp[32] = { 0 };
		CEncodingTools::ConvertUTF8ToGB(tReqDoc["end"].GetString(), pcEndTemp, 32);*/

		string pcUserTemp = tReqDoc["user"].GetString();
		string pcBeginTemp = tReqDoc["begin"].GetString();
		string pcEndTemp = tReqDoc["end"].GetString();

		auto pCallBack = [&allocator, &array](sqlite3_stmt* pStmt)
		{
			Value object(kObjectType);

			int id = sqlite3_column_int(pStmt, 0);
			Value vID(kNumberType);
			vID.SetInt(id);
			object.AddMember("id", vID, allocator);

			const char* pCustomerName = (const char*)sqlite3_column_text(pStmt, 1);
			char pcCustomerNameTemp[64] = { 0 };
			CEncodingTools::ConvertUTF8ToGB(pCustomerName, pcCustomerNameTemp, 64);
			Value vCustomerName(kStringType);
			vCustomerName.SetString(pcCustomerNameTemp, strlen(pcCustomerNameTemp), allocator);
			object.AddMember("customername", vCustomerName, allocator);

			const char* pUserName = (const char*)sqlite3_column_text(pStmt, 2);
			char pcUserNameTemp[64] = { 0 };
			CEncodingTools::ConvertUTF8ToGB(pUserName, pcUserNameTemp, 64);
			Value vUserName(kStringType);
			vUserName.SetString(pcUserNameTemp, strlen(pcUserNameTemp), allocator);
			object.AddMember("userrname", vUserName, allocator);

			double fPrice = sqlite3_column_double(pStmt, 3);
			char chPrice[16] = { 0 };
			sprintf_s(chPrice, 16, "%.2f", fPrice);
			Value vPrice(kStringType);
			vPrice.SetString(chPrice, strlen(chPrice), allocator);
			object.AddMember("price", vPrice, allocator);

			double fFinalPrice = sqlite3_column_double(pStmt, 4);
			char chFinalPrice[16] = { 0 };
			sprintf_s(chFinalPrice, 16, "%.2f", fFinalPrice);
			Value vFinalPrice(kStringType);
			vFinalPrice.SetString(chFinalPrice, strlen(chFinalPrice), allocator);
			object.AddMember("finalprice", vFinalPrice, allocator);

			const char* pTime = (const char*)sqlite3_column_text(pStmt, 5);
			Value vTime(kStringType);
			vTime.SetString(pTime, strlen(pTime), allocator);
			object.AddMember("time", vTime, allocator);

			/*const char* pCondition = (const char*)sqlite3_column_text(pStmt, 6);
			Value vCondition(kStringType);
			vCondition.SetString(pCondition, strlen(pCondition), allocator);
			object.AddMember("condition", vCondition, allocator);*/

			array.PushBack(object, allocator);
		};

		char chSql[512] = { 0 };
		sprintf_s(chSql, 512, "SELECT rowid, * from g_orders where USERNAME='%s'and datetime(TIME)>=datetime('%s') and datetime(TIME)<datetime('%s')",
			pcUserTemp.c_str(), pcBeginTemp.c_str(), pcEndTemp.c_str());

		CDBHelper::Instance()->ExecSearch(chSql, pCallBack);
	}

	int nCount = array.GetArray().Size();
	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", nCount, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
