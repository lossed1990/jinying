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

#include "InterfacePartTypeAll.h"
using namespace rapidjson;

CInterfacePartTypeAll::CInterfacePartTypeAll()
{
}

CInterfacePartTypeAll::~CInterfacePartTypeAll()
{
}

string CInterfacePartTypeAll::GetUrl()
{
	return I_PARTTYPE_ALL;
}

void CInterfacePartTypeAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TPartTypeEntity* pItem){
		
		Value object(kObjectType);

		Value vName(kStringType);
		string strName = pItem->GetName();
		vName.SetString(strName.c_str(), strName.size(), allocator);
		object.AddMember("name", vName, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachPartType(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetPartTypeCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
