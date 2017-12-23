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

#include "InterfaceMoreruleAll.h"
using namespace rapidjson;

CInterfaceMoreruleAll::CInterfaceMoreruleAll()
{
}

CInterfaceMoreruleAll::~CInterfaceMoreruleAll()
{
}

string CInterfaceMoreruleAll::GetUrl()
{
	return I_MORERULE_ALL;
}

void CInterfaceMoreruleAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TMoreruleEntity* pItem){
		Value object(kObjectType);

		Value vCondition(kStringType);
		string strIsPress = pItem->GetConditionString();
		vCondition.SetString(strIsPress.c_str(), strIsPress.length(), allocator);
		object.AddMember("condition", vCondition, allocator);

		Value vRange(kStringType);
		char chRange[64] = { 0 }; //数量区间
		sprintf_s(chRange, "%d-%d", pItem->GetBegin(), pItem->GetEnd());
		vRange.SetString(chRange, strlen(chRange), allocator);
		object.AddMember("range", vRange, allocator);

		Value vCount(kNumberType);
		vCount.SetInt(pItem->GetCount());
		object.AddMember("count", vCount, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachMorerules(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetMoreruleCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
