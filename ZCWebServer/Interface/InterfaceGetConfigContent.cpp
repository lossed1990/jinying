#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <shlwapi.h>
#include <iostream>
#include <fstream>

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "InterfaceGetConfigContent.h"

#pragma comment(lib,"shlwapi.lib")
using namespace rapidjson;

CInterfaceGetConfigContent::CInterfaceGetConfigContent()
{
}

CInterfaceGetConfigContent::~CInterfaceGetConfigContent()
{
}

string CInterfaceGetConfigContent::GetUrl()
{
	return I_CONFIG_GET_CONFIG_CONTENT;
}

void CInterfaceGetConfigContent::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{  
	//解析请求参数
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (!tReqDoc.IsObject() || tReqDoc.FindMember("path") == tReqDoc.MemberEnd() || tReqDoc.FindMember("file") == tReqDoc.MemberEnd())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数错误\"}";
		return;
	}

	string strReqPath = tReqDoc["path"].GetString();
	string strReqFile = tReqDoc["file"].GetString();

	//获取当前配置文件数据
	vector<string> vecCurrentFile;
	string strFullPath = strReqPath + "\\" + strReqFile;
	if (!PathFileExistsA(strFullPath.c_str()))
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"[" + strFullPath + "]文件不存在\"}";
		return;
	}
	ReadFile(strFullPath.c_str(), vecCurrentFile);

	//获取模板配置文件数据
	vector<string> vecBaseFile;
	TCHAR pcLocalPath[MAX_PATH] = { 0 };
	GetExeFileSubDirectory(pcLocalPath, MAX_PATH, _T("baseconfig"));
	strFullPath = pcLocalPath;
	strFullPath = strFullPath + "\\" + strReqFile;
	ReadFile(strFullPath.c_str(), vecBaseFile);

	//比较配置文件信息
	vector<CONFIG_DIFFINFO> vecCompareInfo;
	CompareFile(vecBaseFile, vecCurrentFile, vecCompareInfo);

	//返回结果
	static const char* TEMPLATE = "{\"ok\":0,\"errorinfo\":\"\",\"data\":[] }";
	Document doc;
	doc.Parse(TEMPLATE);

	for (auto item : vecCompareInfo)
	{
		Value vLineObj(kObjectType);

		Value vBaseLine(kStringType);
		vBaseLine.SetString(item.base.c_str(), doc.GetAllocator());
		vLineObj.AddMember("base", vBaseLine, doc.GetAllocator());

		Value vCurrentLine(kStringType);
		vCurrentLine.SetString(item.current.c_str(), doc.GetAllocator());
		vLineObj.AddMember("current", vCurrentLine, doc.GetAllocator());

		Value vStatus(kNumberType);
		vStatus.SetInt64(item.status);
		vLineObj.AddMember("status", vStatus, doc.GetAllocator());

		doc["data"].PushBack(vLineObj, doc.GetAllocator());
	}

	StringBuffer outBuff;
	Writer<StringBuffer> jsonWriter(outBuff);
	doc.Accept(jsonWriter);

	strReturn = outBuff.GetString();
}

void CInterfaceGetConfigContent::ReadFile(const char* chPath, vector<string>& vecFileLines)
{
	std::ifstream ifile(chPath);
	if (ifile && ifile.is_open())
	{
		string s;

		while (getline(ifile, s))
		{
			vecFileLines.push_back(s);
		}

		ifile.close();
		ifile.clear();
	}
}

void CInterfaceGetConfigContent::CompareFile(vector<string>& vecBaseFileLines, vector<string>& vecCurrentFileLines, vector<CONFIG_DIFFINFO>& vecResult)
{
	size_t index = 0;
	size_t nCurrentFileLineSize = vecCurrentFileLines.size();
	for (auto item : vecBaseFileLines)
	{
		CONFIG_DIFFINFO tInfo;
		//相似
		if (index < nCurrentFileLineSize && item[0] == vecCurrentFileLines[index][0])
		{
			tInfo.base = item;
			tInfo.current = vecCurrentFileLines[index];

			//是否相同，设置不同的状态值		
			tInfo.status = (item == vecCurrentFileLines[index]) ? 0 : 1;
			++index;
		}
		else
		{
			//不存在
			tInfo.base = item;
			tInfo.current = "###";  //三个#号代表不存在行
			tInfo.status = 3;
		}

		vecResult.push_back(tInfo);
	}

	if (index < nCurrentFileLineSize)
	{
		for (size_t i = index; i < nCurrentFileLineSize; ++i)
		{
			CONFIG_DIFFINFO tInfo;
			tInfo.base = "###";
			tInfo.current = vecCurrentFileLines[i];
			tInfo.status = 3;
			vecResult.push_back(tInfo);
		}
	}
}