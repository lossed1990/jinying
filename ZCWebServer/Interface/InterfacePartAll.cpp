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

#include "InterfacePartAll.h"
using namespace rapidjson;

CInterfacePartAll::CInterfacePartAll()
{
}

CInterfacePartAll::~CInterfacePartAll()
{
}

string CInterfacePartAll::GetUrl()
{
	return I_PART_ALL;
}

void CInterfacePartAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TPartEntity* pItem){
		
		Value object(kObjectType);

		Value vName(kStringType);
		string strName = pItem->GetName();
		vName.SetString(strName.c_str(), strName.size(), allocator);
		object.AddMember("name", vName, allocator);

		Value vTypeName(kStringType);
		string strTypeName = pItem->GetTypeName();
		vTypeName.SetString(strTypeName.c_str(), strTypeName.size(), allocator);
		object.AddMember("type", vTypeName, allocator);

		//Value vPrice(kNumberType);
		//vPrice.SetFloat(pItem->GetPrice());
		//object.AddMember("price", vPrice, allocator);

		Value vPrice(kStringType);
		char chPrice[64] = { 0 };
		sprintf_s(chPrice, "%.2f", pItem->GetPrice());
		vPrice.SetString(chPrice, strlen(chPrice), allocator);
		object.AddMember("price", vPrice, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachPart(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetPartCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
