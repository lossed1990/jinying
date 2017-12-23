#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "io.h"

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceFilesRemove.h"

CInterfaceFilesRemove::CInterfaceFilesRemove()
{
}

CInterfaceFilesRemove::~CInterfaceFilesRemove()
{
}

string CInterfaceFilesRemove::GetUrl()
{
	return I_FILE_REMOVE_FILE;
}

void CInterfaceFilesRemove::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	if (tReqDoc.HasMember("file"))
	{
		string strReqFile = tReqDoc["file"].GetString();
		char* pTemp = NULL;
		int nSize = strReqFile.size() + 1;
		pTemp = new char[nSize];
		memset(pTemp, 0, nSize);
		CEncodingTools::ConvertUTF8ToGB(strReqFile.c_str(), pTemp, nSize);

		if (access(pTemp, 2) == 0)	// 0:是否存在 1:是否可运行 2:是否可写访问 4:是否可读访问 6 （X_OK） 5:是否可执行
		{
			tDoc.AddMember("ok", 0, allocator);
			tDoc.AddMember("errorinfo", "", allocator);
			remove(pTemp);
		}
		else
		{
			tDoc.AddMember("ok", 1, allocator);
			tDoc.AddMember("errorinfo", "删除文件失败", allocator);
		}

		delete pTemp;
	}
	else
	{
		tDoc.AddMember("ok", 0, allocator);
		tDoc.AddMember("errorinfo", "", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}
