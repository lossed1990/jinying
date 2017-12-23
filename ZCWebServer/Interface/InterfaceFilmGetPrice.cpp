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

#include "InterfaceFilmGetPrice.h"
using namespace rapidjson;

CInterfaceFilmGetPrice::CInterfaceFilmGetPrice()
{
}

CInterfaceFilmGetPrice::~CInterfaceFilmGetPrice()
{
}

string CInterfaceFilmGetPrice::GetUrl()
{
	return I_FILM_GETPRICE;
}

void CInterfaceFilmGetPrice::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	//tDoc.AddMember("price", CMainModel::Instance()->GetFilmPrice(), allocator);

	Value vPrice(kStringType);
	char chPrice[64] = { 0 };
	sprintf_s(chPrice, "%.2f", CMainModel::Instance()->GetFilmPrice());
	vPrice.SetString(chPrice, strlen(chPrice), allocator);
	tDoc.AddMember("price", vPrice, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;

	return;
}
