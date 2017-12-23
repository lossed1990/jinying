#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceFilesReplace.h"

CInterfaceFilesReplace::CInterfaceFilesReplace()
{
}

CInterfaceFilesReplace::~CInterfaceFilesReplace()
{
}

string CInterfaceFilesReplace::GetUrl()
{
	return I_FILE_REPLACE_FILE;
}

void CInterfaceFilesReplace::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	vector<int> vecFiles;

	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	if (vecFiles.size() > 0)
	{
	}
	else
	{
		tDoc.AddMember("ok", 1, allocator);
		tDoc.AddMember("errorinfo", "没有找到文件或文件夹", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}
