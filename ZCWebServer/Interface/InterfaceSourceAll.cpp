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

#include "InterfaceSourceAll.h"
using namespace rapidjson;

CInterfaceSourceAll::CInterfaceSourceAll()
{
}

CInterfaceSourceAll::~CInterfaceSourceAll()
{
}

string CInterfaceSourceAll::GetUrl()
{
	return I_SOURCE_ALL;
}

void CInterfaceSourceAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TSourceEntity* pItem){
		
		Value object(kObjectType);

		Value vName(kStringType);
		string strName = pItem->GetName();
		vName.SetString(strName.c_str(), strName.size(), allocator);
		object.AddMember("name", vName, allocator);

		Value vTypeName(kStringType);
		string strTypeName = pItem->GetTypeName();
		vTypeName.SetString(strTypeName.c_str(), strTypeName.size(), allocator);
		object.AddMember("typename", vTypeName, allocator);

		Value vType(kStringType);
		char chType[64] = { 0 }; //材料规格
		sprintf_s(chType, "%d*%d", pItem->GetLen(),pItem->GetWidth());
		vType.SetString(chType, strlen(chType), allocator);
		object.AddMember("type", vType, allocator);

		Value vKezhong(kNumberType);
		vKezhong.SetInt(pItem->GetKezhong());
		object.AddMember("kez", vKezhong, allocator);

		Value vDunjia(kStringType);
		char chPrice[64] = { 0 };
		sprintf_s(chPrice, "%.2f", pItem->GetDunjia());
		vDunjia.SetString(chPrice, strlen(chPrice), allocator);
		object.AddMember("dunj", vDunjia, allocator);

		Value vUseType(kStringType);
		string strUseType = (pItem->GetUseType() == 1) ? "包装盒" : "手提袋";
		vUseType.SetString(strUseType.c_str(), strUseType.size(), allocator);
		object.AddMember("usetype", vUseType, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachSource(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetSourceCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
