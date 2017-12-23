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

#include "InterfacePressPriceAll.h"
using namespace rapidjson;

CInterfacePressPriceAll::CInterfacePressPriceAll()
{
}

CInterfacePressPriceAll::~CInterfacePressPriceAll()
{
}

string CInterfacePressPriceAll::GetUrl()
{
	return I_PRESSPRICE_ALL;
}

void CInterfacePressPriceAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TPressPriceEntity* pItem){
		
		Value object(kObjectType);

		Value vPressName(kStringType);
		string strPressName = pItem->GetPressName();
		vPressName.SetString(strPressName.c_str(), strPressName.size(), allocator);
		object.AddMember("pressname", vPressName, allocator);

		Value vPressTypeName(kStringType);
		string strPressTypeName = pItem->GetPressTypeName();
		vPressTypeName.SetString(strPressTypeName.c_str(), strPressTypeName.size(), allocator);
		object.AddMember("presstypename", vPressTypeName, allocator);

		Value vType(kStringType);
		char chType[64] = { 0 }; //规格
		sprintf_s(chType, "%d-%d", pItem->GetBegin(),pItem->GetEnd());
		vType.SetString(chType, strlen(chType), allocator);
		object.AddMember("range", vType, allocator);

		/*Value vPrice(kNumberType);
		vPrice.SetInt(pItem->GetPrice());
		object.AddMember("price", vPrice, allocator);
*/
		Value vPrice(kStringType);
		char chPrice[64] = { 0 };
		sprintf_s(chPrice, "%.2f", pItem->GetPrice());
		vPrice.SetString(chPrice, strlen(chPrice), allocator);
		object.AddMember("price", vPrice, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachPressprice(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetPresspriceCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
