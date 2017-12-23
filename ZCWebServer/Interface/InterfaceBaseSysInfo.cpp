#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "InterfaceBaseSysInfo.h"

CInterfaceBaseSysInfo::CInterfaceBaseSysInfo()
{
	GetCPUName();
	GetOsName();
	GetOsTypeName();
	GetMemorySize();
}


CInterfaceBaseSysInfo::~CInterfaceBaseSysInfo()
{
}

string CInterfaceBaseSysInfo::GetUrl()
{
	return I_BASE_GET_SYSINFO;
}

void CInterfaceBaseSysInfo::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	static const char* TEMPLATE = "{\"ok\":0,\"errorinfo\":\"\",\"data\":[{\"osname\":\"%s\",\"cpuname\":\"%s\",\"ostypename\":\"%s\",\"memorysize\":\"%s\"}]}";
	char pcTemp[512] = { 0 };
	sprintf_s(pcTemp,512, TEMPLATE, m_chOsName, m_chCPUName, m_chOsTypeName, m_chMemorySize);

	strReturn = pcTemp;
}

void CInterfaceBaseSysInfo::GetCPUName()
{
	HKEY hKeyCPU;
	HRESULT hr = RegOpenKeyA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hKeyCPU);
	if (ERROR_SUCCESS == hr) {
		DWORD iSize = 255, iType = REG_SZ;
		hr = RegQueryValueExA(hKeyCPU, "ProcessorNameString", NULL, &iType, (LPBYTE)m_chCPUName, &iSize);
		RegCloseKey(hKeyCPU);
	}
}

void CInterfaceBaseSysInfo::GetOsName()
{
	HKEY hOS;
	HRESULT hr = RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", &hOS);
	if (ERROR_SUCCESS == hr) {
		DWORD iSize = 255, iType = REG_SZ;
		hr = RegQueryValueExA(hOS, "ProductName", NULL, &iType, (LPBYTE)m_chOsName, &iSize);
		RegCloseKey(hOS);
	}
}

void CInterfaceBaseSysInfo::GetOsTypeName()
{
	HKEY hKeyInfo;
	HRESULT hr = RegOpenKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", &hKeyInfo);
	if (ERROR_SUCCESS == hr) {
		DWORD iSize = 64, iType = REG_SZ;
		char szName[64];
		hr = RegQueryValueExA(hKeyInfo, "PROCESSOR_ARCHITECTURE", NULL, &iType, (LPBYTE)szName, &iSize);
		RegCloseKey(hKeyInfo);

		if (ERROR_SUCCESS == hr) {
			if (strcmp(szName, "x86") == 0) {
				strcpy_s(m_chOsTypeName,64, "32位操作系统");
			}
			else if (strcmp(szName, "AMD64") == 0 || strcmp(szName, "IA64") == 0) {
				strcpy_s(m_chOsTypeName, 64, "64位操作系统");
			}
			else
			{
				strcpy_s(m_chOsTypeName, 64, szName);
			}
		}
	}
}

void CInterfaceBaseSysInfo::GetMemorySize()
{
	MEMORYSTATUS stat;
	GlobalMemoryStatus(&stat);
	MEMORYSTATUSEX stat_ex;
	stat_ex.dwLength = sizeof(stat_ex);
	GlobalMemoryStatusEx(&stat_ex);

	DWORDLONG Size = stat_ex.ullTotalPhys / 1024 / 1024;
	sprintf_s(m_chMemorySize, 64, "%lld (MB)", Size);
}