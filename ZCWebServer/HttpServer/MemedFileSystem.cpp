#include "MemedFileSystem.h"
#include "../JsonRequest/EncodingToolsClass.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include "../SystemCounter/MemoryCounter.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include <set>
using std::set;

CMemedFileSystem::CMemedFileSystem()
{
	StartMemFileSystem();

	m_mapMemFiles[string("systeminfo.json")] = &CMemedFileSystem::MF_GETSYSTEMINFO;
	m_mapMemFiles[string("process.json")] = &CMemedFileSystem::MF_GETPROCESSESINFO;
}


CMemedFileSystem::~CMemedFileSystem()
{
}

bool CMemedFileSystem::MF_GETSYSTEMINFO(string& strSysinfo)
{
	static const char* TEMPLATE = "{\"cpuusage\":%s,\"cputime\":%s,\"memusage\":%s,\"totalmemmb\":%d,\"totalhandle\":%d,\"totalprocess\":%d,\"totalthread\":%d,\"timestamp\":%llu}";

	char pcTemp[512] = { 0 };

	//CPU数据
	PROCESSOR_STATUS_INFO infoCPU;
	m_csProcessor.Enter();
	memcpy(&infoCPU, &m_infoProcessor, sizeof(PROCESSOR_STATUS_INFO));
	m_csProcessor.Leave();

	//内存数据
	MEMORY_STATUS_INFO infoMemo;
	m_csMemory.Enter();
	memcpy(&infoMemo, &m_infoMemory, sizeof(MEMORY_STATUS_INFO));
	m_csMemory.Leave();

	//打印数据
	sprintf(pcTemp, TEMPLATE, infoCPU.pcCPUUsage, infoCPU.pcCPUTime, infoMemo.pcMemoUsage, infoMemo.szTotalMB, infoMemo.nTotalHandle, infoMemo.nTotalProcess, infoMemo.nTotalThread, CEncodingTools::GetCurrentEpochTimestamp());
	strSysinfo = pcTemp;

	return true;
}

bool CMemedFileSystem::MF_GETPROCESSESINFO(string& strProcess)
{
	static const char* TEMPLATE = "{\"process\":[] }";
	set<string> setProcess;

	//获取系统所有进程
	PROCESSENTRY32 entry = {0};
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
			char* pcDot = strrchr(entry.szExeFile, '.');
			*pcDot = '\0';

			//放到集合里过滤
			setProcess.insert(string(entry.szExeFile));
		}
	}

	CloseHandle(hSnapshot);

	//生成JSON
	Document doc;
	doc.Parse(TEMPLATE);

	for (set<string>::iterator it = setProcess.begin(); it != setProcess.end(); ++it)
	{
		Value vProcess(kStringType);
		vProcess.SetString(it->c_str(), doc.GetAllocator());

		doc["process"].PushBack(vProcess, doc.GetAllocator());
	}

	StringBuffer outBuff;
	Writer<StringBuffer> jsonWriter(outBuff);
	doc.Accept(jsonWriter);

	strProcess = outBuff.GetString();

	return true;
}

void __stdcall CMemedFileSystem::_PROCESSOR_INFO_CALLBAK(PROCESSOR_STATUS_INFO* pInfo, void* pUser)
{
	CMemedFileSystem* pThis = (CMemedFileSystem*)pUser;

	pThis->m_csProcessor.Enter();

	memcpy(&pThis->m_infoProcessor, pInfo, sizeof(PROCESSOR_STATUS_INFO));

	pThis->m_csProcessor.Leave();
}

void __stdcall CMemedFileSystem::_MEMORY_INFO_CALLBAK(MEMORY_STATUS_INFO* pInfo, void* pUser)
{
	CMemedFileSystem* pThis = (CMemedFileSystem*)pUser;

	pThis->m_csMemory.Enter();

	memcpy(&pThis->m_infoMemory, pInfo, sizeof(MEMORY_STATUS_INFO));

	pThis->m_csMemory.Leave();
}

void CMemedFileSystem::StartMemFileSystem()
{
	m_ctProcessor.StartMonitorCPU(_PROCESSOR_INFO_CALLBAK, this);
	m_ctMemory.StartMonitorMemory(_MEMORY_INFO_CALLBAK, this);
}

void CMemedFileSystem::StopMemFileSystem()
{
	m_ctProcessor.StopMonitorCPU();
	m_ctMemory.StopMonitorMemory();
}

bool CMemedFileSystem::GetFileInfoByPath(CStaticFileInfo& fileInfo)
{
	if (m_mapMemFiles.find(fileInfo.GetFileRelPath()) != m_mapMemFiles.end())
	{
		return true;
	}

	return false;
}

int CMemedFileSystem::StoreFileInSystem(CStaticFileInfo& fileInfo)
{
	return 0;
}

void* CMemedFileSystem::PrepareFile(CStaticFileInfo& fileInfo)
{
	GETFILECONTENTPF pf = m_mapMemFiles.find(fileInfo.GetFileRelPath())->second;

	string* pStrResult = new string;
	bool bRet = (this->*pf)(*pStrResult);

	//设置读取位置0
	fileInfo.BeginRead();
	fileInfo.SetFileSize(pStrResult->length());

	return pStrResult;
}

int CMemedFileSystem::ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded)
{
	if (pSourcePtr)
	{
		string* pString = (string*)pSourcePtr;

		size_t szBeginPos = pString->length() - fileInfo.GetLeftCount();
		string strTemp = pString->substr(szBeginPos, szRecvBuffSize);

		if (strTemp.length() > 0)
		{
			szReaded = strTemp.length();
			fileInfo.Consume(szReaded);

			memcpy(pRecvBuff, strTemp.c_str(), szReaded);

			return fileInfo.GetLeftCount();
		}
	}

	return -1;
}

void CMemedFileSystem::ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr)
{
	fileInfo.EndRead();

	if (pSourcePtr)
	{
		string* pString = (string*)pSourcePtr;
		delete pString;
	}
}