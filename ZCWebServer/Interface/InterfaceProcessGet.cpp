#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceProcessGet.h"

CInterfaceProcessGet::CInterfaceProcessGet()
{
}

CInterfaceProcessGet::~CInterfaceProcessGet()
{
}

string CInterfaceProcessGet::GetUrl()
{
	return I_PROCESS_GET_ALL;
}

void CInterfaceProcessGet::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	GetAllProcessInfo(strReturn);
}

bool CInterfaceProcessGet::GetAllProcessInfo(string& strProcess)
{
	//从配置文件获取需要管理的进程
	vector<string> vecConfigProcess;
	bool bRet = GetProcessStrings(vecConfigProcess);
	if (!bRet || vecConfigProcess.size() == 0)
	{
		strProcess = "{\"ok\":1,\"errorinfo\":\"获取进程配置文件失败！\"}";
		return false;
	}

	static const char* TEMPLATE = "{\"ok\":0,\"errorinfo\":\"\",\"data\":[] }";
	vector<PROCESSENTRY32> vecProcess;

	//获取系统所有进程
	PROCESSENTRY32 entry = { 0 };
	entry.dwSize = sizeof(PROCESSENTRY32);

    /*MODULEENTRY32 pes;
	pes.dwSize = sizeof(MODULEENTRY32);*/

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnapshot, &entry) == TRUE)
	{
		while (Process32Next(hSnapshot, &entry) == TRUE)
		{
			//HANDLE hfilename = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, entry.th32ProcessID);
			//Module32First(hfilename, &pes);

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

			//判断是否为需要管理的进程
			for (auto proInfo : vecConfigProcess)
			{
				if (stricmp(entry.szExeFile, proInfo.c_str()) == 0)
				{
					//放到集合里过滤
					vecProcess.push_back(entry);
					break;
				}
			}		
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

		//获取安装路径信息
		HANDLE pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,item.th32ProcessID);//必须有这两个权限
		char exeFullPath[MAX_PATH] = {0};
		if (pHandle)
		{
			GetModuleFileNameEx(pHandle, NULL, exeFullPath, MAX_PATH);
			CloseHandle(pHandle);
		}

		Value vProcessPath(kStringType);
		vProcessPath.SetString(exeFullPath, doc.GetAllocator());
		vProcessObj.AddMember("path", vProcessPath, doc.GetAllocator());

		doc["data"].PushBack(vProcessObj, doc.GetAllocator());
	}

	StringBuffer outBuff;
	Writer<StringBuffer> jsonWriter(outBuff);
	doc.Accept(jsonWriter);

	strProcess = outBuff.GetString();

	return true;
}

string CInterfaceProcessGet::GetProcessConfigBufferString()
{
	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");

	//config file
	strcat(tpcPath, "\\process.json");

	string strContent;

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//配置文件不会超过4K
		char pcBuff[4096] = { 0 };
		fread(pcBuff, 1, 4096, pFile);

		strContent = pcBuff;
		fclose(pFile);
		pFile = NULL;
	}

	return strContent;
}

bool CInterfaceProcessGet::GetProcessStrings(vector<string>& strVecApps)
{
	Document doc;
	doc.Parse(GetProcessConfigBufferString().c_str());

	if (doc.IsObject())
	{
		if (doc["process"].IsArray())
		{
			for (SizeType i = 0; i < doc["process"].Size(); i++)
			{
				char pcTemp[256] = { 0 };
				CEncodingTools::ConvertUTF8ToGB(doc["process"][i].GetString(), pcTemp, 256);
				strVecApps.push_back(string(pcTemp));
			}

			return true;
		}
	}

	return false;
}