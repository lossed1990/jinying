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

#include "InterfaceImpressGetPrice.h"
using namespace rapidjson;

CInterfaceImpressGetPrice::CInterfaceImpressGetPrice()
{
}

CInterfaceImpressGetPrice::~CInterfaceImpressGetPrice()
{
}

string CInterfaceImpressGetPrice::GetUrl()
{
	return I_IMPRESS_GETPRICE;
}

void CInterfaceImpressGetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	//tDoc.AddMember("price", CMainModel::Instance()->GetImpressPrice(), allocator);

	Value vPrice(kStringType);
	char chPrice[64] = { 0 };
	sprintf_s(chPrice, "%.2f", CMainModel::Instance()->GetImpressPrice());
	vPrice.SetString(chPrice, strlen(chPrice), allocator);
	tDoc.AddMember("price", vPrice, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;

	return;
}
