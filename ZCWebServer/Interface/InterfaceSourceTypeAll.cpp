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

#include "InterfaceSourceTypeAll.h"
using namespace rapidjson;

CInterfaceSourceTypeAll::CInterfaceSourceTypeAll()
{
}

CInterfaceSourceTypeAll::~CInterfaceSourceTypeAll()
{
}

string CInterfaceSourceTypeAll::GetUrl()
{
	return I_SOURCETYPE_ALL;
}

void CInterfaceSourceTypeAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TSourceTypeEntity* pItem){
		
		Value object(kObjectType);

		Value vName(kStringType);
		string strName = pItem->GetName();
		vName.SetString(strName.c_str(), strName.size(), allocator);
		object.AddMember("name", vName, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachSourceType(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetSourceTypeCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
