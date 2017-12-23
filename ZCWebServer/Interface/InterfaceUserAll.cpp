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

#include "InterfaceUserAll.h"
using namespace rapidjson;

CInterfaceUserAll::CInterfaceUserAll()
{
}

CInterfaceUserAll::~CInterfaceUserAll()
{
}

string CInterfaceUserAll::GetUrl()
{
	return I_USER_ALL;
}

void CInterfaceUserAll::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	//tDoc.AddMember("ok", 0, allocator);
	//tDoc.AddMember("errorinfo", "", allocator);

	//tDoc.AddMember("recordsFiltered", CMainModel::Instance()->GetUserCount(), allocator);
	
	Value array(kArrayType);
	auto pCallBack = [&array, &allocator](TUserEntity* pItem){
		
		Value object(kObjectType);

		Value vUserName(kStringType);
		string strUserName = pItem->GetName();
		vUserName.SetString(strUserName.c_str(), strUserName.size(), allocator);
		object.AddMember("name", vUserName, allocator);

		Value vUserType(kStringType);
		string strUserType = (pItem->GetType() == 1) ? "管理员" : "员工";
		vUserType.SetString(strUserType.c_str(), strUserType.size(), allocator);
		object.AddMember("type", vUserType, allocator);

		array.PushBack(object, allocator);
	};
	CMainModel::Instance()->EachUsers(pCallBack);

	tDoc.AddMember("data", array, allocator);
	tDoc.AddMember("recordsTotal", CMainModel::Instance()->GetUserCount(), allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
	return;
}
