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

#include "InterfaceDieCutterAll.h"
using namespace rapidjson;

CInterfaceDieCutterAll::CInterfaceDieCutterAll()
{
}

CInterfaceDieCutterAll::~CInterfaceDieCutterAll()
{
}

string CInterfaceDieCutterAll::GetUrl()
{
	return I_DIECUTTER_ALL;
}

void CInterfaceDieCutterAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TDieCutterEntity* pItem){
		
		Value object(kObjectType);

		Value vProductType(kStringType);
		//char chProductType[64] = { 0 }; //材料规格
		//sprintf_s(chProductType, "%d*%d*%d", pItem->GetLen(), pItem->GetWidth(), pItem->GetHeight());

		string chProductType = pItem->GetTypeName();
		vProductType.SetString(chProductType.c_str(), chProductType.length(), allocator);
		object.AddMember("producttype", vProductType, allocator);

		Value vType(kStringType);
		char chType[64] = { 0 }; //材料规格
		sprintf_s(chType, "%d*%d*%d", pItem->GetLen(), pItem->GetWidth(), pItem->GetHeight());
		vType.SetString(chType, strlen(chType), allocator);
		object.AddMember("type", vType, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachDieCutter(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetDieCutterCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
