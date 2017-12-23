#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>

#include "../JsonRequest/EncodingToolsClass.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceProcessCloseById.h"

CInterfaceProcessCloseById::CInterfaceProcessCloseById()
{
}

CInterfaceProcessCloseById::~CInterfaceProcessCloseById()
{
}

string CInterfaceProcessCloseById::GetUrl()
{
	return I_PROCESS_CLOSEBYID;
}

void CInterfaceProcessCloseById::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	int nPid = 0;
	Document doc;
	doc.Parse(pReqBody);

	if (doc.IsObject())
	{
		nPid = doc["id"].GetInt();
	}
	else
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数非法\" }";
		return;
	}

	if (nPid <= 0)
	{
		strReturn = "{\"ok\":1,\"errorinfo\":\"请求参数非法\" }";
		return;
	}

	static const char* TEMPLATE = "TASKKILL /PID %d /T";
	char chCmd[512] = { 0 };
	sprintf_s(chCmd, 512, TEMPLATE, nPid);
	system(chCmd);
	strReturn = "{\"ok\":0,\"errorinfo\":\"\" }";
}

bool CInterfaceProcessCloseById::GetAllProcessInfo(string& strProcess)
{
	static const char* TEMPLATE = "{\"ok\":0,\"errorinfo\":\"\",\"data\":[] }";
	vector<PROCESSENTRY32> vecProcess;

	//获取系统所有进程
	PROCESSENTRY32 entry = { 0 };
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnapshot, &entry) == TRUE)
	{
		while (Process32Next(hSnapshot, &entry) == TRUE)
		{
			//排除不带点号的系统服务
			if (strchr(entry.szExeFile, '.') == NULL) continue;

			//移除一些系统进程
			if (strcmp(entry.szExeFile, "svchost.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "conhost.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "rundll32.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "wininit.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "smss.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "lsass.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "csrss.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "winlogon.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "services.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "dwm.exe") == 0) continue;
			if (strcmp(entry.szExeFile, "spoolsv.exe") == 0) continue;

			//移除点号
			//char* pcDot = strrchr(entry.szExeFile, '.');
			//*pcDot = '\0';

			//放到集合里过滤
			vecProcess.push_back(entry);
		}
	}

	CloseHandle(hSnapshot);

	//生成JSON
	Document doc;
	doc.Parse(TEMPLATE);

	for (auto item : vecProcess)
	{
		Value vProcessObj(kObjectType);
		Value vProcessID(kNumberType);
		vProcessID.SetInt64(item.th32ProcessID);
		vProcessObj.AddMember("id", vProcessID, doc.GetAllocator());

		Value vProcessName(kStringType);
		vProcessName.SetString(item.szExeFile, doc.GetAllocator());
		vProcessObj.AddMember("name", vProcessName, doc.GetAllocator());

		doc["data"].PushBack(vProcessObj, doc.GetAllocator());
	}

	StringBuffer outBuff;
	Writer<StringBuffer> jsonWriter(outBuff);
	doc.Accept(jsonWriter);

	strProcess = outBuff.GetString();

	return true;
}