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

#include "InterfaceImpressGetCheckPrice.h"
using namespace rapidjson;

CInterfaceImpressGetCheckPrice::CInterfaceImpressGetCheckPrice()
{
}

CInterfaceImpressGetCheckPrice::~CInterfaceImpressGetCheckPrice()
{
}

string CInterfaceImpressGetCheckPrice::GetUrl()
{
	return I_IMPRESS_GETCHECKPRICE;
}

void CInterfaceImpressGetCheckPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	//tDoc.AddMember("checkprice", CMainModel::Instance()->GetImpressCheckPrice(), allocator);

	Value vPrice(kStringType);
	char chPrice[64] = { 0 };
	sprintf_s(chPrice, "%.2f", CMainModel::Instance()->GetImpressCheckPrice());
	vPrice.SetString(chPrice, strlen(chPrice), allocator);
	tDoc.AddMember("checkprice", vPrice, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;

	return;
}
