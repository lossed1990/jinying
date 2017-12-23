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
	//�����������
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	if (!tReqDoc.IsObject() || tReqDoc.FindMember("path") == tReqDoc.MemberEnd() || tReqDoc.FindMember("file") == tReqDoc.MemberEnd())
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"�����������\"}";
		return;
	}

	string strReqPath = tReqDoc["path"].GetString();
	string strReqFile = tReqDoc["file"].GetString();

	//��ȡ��ǰ�����ļ�����
	vector<string> vecCurrentFile;
	string strFullPath = strReqPath + "\\" + strReqFile;
	if (!PathFileExistsA(strFullPath.c_str()))
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"[" + strFullPath + "]�ļ�������\"}";
		return;
	}
	ReadFile(strFullPath.c_str(), vecCurrentFile);

	//��ȡģ�������ļ�����
	vector<string> vecBaseFile;
	TCHAR pcLocalPath[MAX_PATH] = { 0 };
	GetExeFileSubDirectory(pcLocalPath, MAX_PATH, _T("baseconfig"));
	strFullPath = pcLocalPath;
	strFullPath = strFullPath + "\\" + strReqFile;
	ReadFile(strFullPath.c_str(), vecBaseFile);

	//�Ƚ������ļ���Ϣ
	vector<CONFIG_DIFFINFO> vecCompareInfo;
	CompareFile(vecBaseFile, vecCurrentFile, vecCompareInfo);

	//���ؽ��
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
		//����
		if (index < nCurrentFileLineSize && item[0] == vecCurrentFileLines[index][0])
		{
			tInfo.base = item;
			tInfo.current = vecCurrentFileLines[index];

			//�Ƿ���ͬ�����ò�ͬ��״ֵ̬		
			tInfo.status = (item == vecCurrentFileLines[index]) ? 0 : 1;
			++index;
		}
		else
		{
			//������
			tInfo.base = item;
			tInfo.current = "###";  //����#�Ŵ���������
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