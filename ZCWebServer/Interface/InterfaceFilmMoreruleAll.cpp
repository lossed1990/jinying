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

#include "InterfaceFilmMoreruleAll.h"
using namespace rapidjson;

CInterfaceFilmMoreruleAll::CInterfaceFilmMoreruleAll()
{
}

CInterfaceFilmMoreruleAll::~CInterfaceFilmMoreruleAll()
{
}

string CInterfaceFilmMoreruleAll::GetUrl()
{
	return I_FILM_MORERULE_ALL;
}

void CInterfaceFilmMoreruleAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TMoreruleEntity* pItem){
		Value object(kObjectType);

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
	CMainModel::Instance()->EachFilmMorerules(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetFilmMoreruleCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
