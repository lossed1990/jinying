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

#include "InterfaceDieCutterGetPrice.h"
using namespace rapidjson;

CInterfaceDieCutterGetPrice::CInterfaceDieCutterGetPrice()
{
}

CInterfaceDieCutterGetPrice::~CInterfaceDieCutterGetPrice()
{
}

string CInterfaceDieCutterGetPrice::GetUrl()
{
	return I_DIECUTTER_GETPRICE;
}

void CInterfaceDieCutterGetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	//tDoc.AddMember("price", CMainModel::Instance()->GetDieCutterPrice(), allocator);

	Value vPrice(kStringType);
	char chPrice[64] = { 0 };
	sprintf_s(chPrice, "%.2f", CMainModel::Instance()->GetDieCutterPrice());
	vPrice.SetString(chPrice, strlen(chPrice), allocator);
	tDoc.AddMember("price", vPrice, allocator);

	Value vBoxPrice(kStringType);
	char chBoxPrice[64] = { 0 };
	sprintf_s(chBoxPrice, "%.2f", CMainModel::Instance()->GetBoxDieCutterPrice());
	vBoxPrice.SetString(chBoxPrice, strlen(chBoxPrice), allocator);
	tDoc.AddMember("boxprice", vBoxPrice, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;

	return;
}
