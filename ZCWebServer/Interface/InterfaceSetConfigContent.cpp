#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceSetConfigContent.h"

CInterfaceSetConfigContent::CInterfaceSetConfigContent()
{
}

CInterfaceSetConfigContent::~CInterfaceSetConfigContent()
{
}

string CInterfaceSetConfigContent::GetUrl()
{
	return I_CONFIG_SET_CONFIG_CONTENT;
}

void CInterfaceSetConfigContent::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	string strFullFile;
	string strFileContent;
	if (tReqDoc.FindMember("path") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("file") != tReqDoc.MemberEnd() &&
		tReqDoc.FindMember("data") != tReqDoc.MemberEnd())
	{
		string strReqFile = tReqDoc["path"].GetString();
		strReqFile += "\\";
		strReqFile += tReqDoc["file"].GetString();
		char* pTemp = NULL;
		int nSize = strReqFile.size() + 1;
		pTemp = new char[nSize];
		memset(pTemp, 0, nSize);
		CEncodingTools::ConvertUTF8ToGB(strReqFile.c_str(), pTemp, nSize);
		strFullFile = pTemp;
		delete[] pTemp;

		string strReqContent;
		Value &cValueArray = tReqDoc["data"];
		if (cValueArray.IsArray())
		{
			for (int i = 0; i < cValueArray.Size(); i++)
			{
				const Value& object = cValueArray[i];
				strReqContent += object.GetString();
				strReqContent += "\r\n";
			}
		}
		pTemp = NULL;
		nSize = strReqContent.size() + 1;
		pTemp = new char[nSize];
		memset(pTemp, 0, nSize);
		CEncodingTools::ConvertUTF8ToGB(strReqContent.c_str(), pTemp, nSize);
		strFileContent = pTemp;
		delete[] pTemp;
	}

	Document tDoc;
	tDoc.SetObject();

	FILE* pFile = fopen(strFullFile.c_str(), "wb");
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	if (pFile)
	{
		tDoc.AddMember("ok", 0, allocator);
		tDoc.AddMember("errorinfo", "", allocator);

		fwrite(strFileContent.c_str(), strFileContent.length(), 1, pFile);
		fclose(pFile);
	}
	else
	{
		tDoc.AddMember("ok", 1, allocator);
		tDoc.AddMember("errorinfo", "打开配置文件失败", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}
