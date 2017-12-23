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

#include "InterfacePressAll.h"
using namespace rapidjson;

CInterfacePressAll::CInterfacePressAll()
{
}

CInterfacePressAll::~CInterfacePressAll()
{
}

string CInterfacePressAll::GetUrl()
{
	return I_PRESS_ALL;
}

void CInterfacePressAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TPressEntity* pItem){
		
		Value object(kObjectType);

		Value vName(kStringType);
		string strName = pItem->GetName();
		vName.SetString(strName.c_str(), strName.size(), allocator);
		object.AddMember("name", vName, allocator);

		Value vType(kStringType);
		char chType[64] = { 0 }; //规格
		sprintf_s(chType, "%d*%d", pItem->GetLen(),pItem->GetWidth());
		vType.SetString(chType, strlen(chType), allocator);
		object.AddMember("type", vType, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachPress(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetPressCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
