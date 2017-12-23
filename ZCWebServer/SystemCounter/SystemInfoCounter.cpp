#include "SystemInfoCounter.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <pdh.h>
#include <pdhmsg.h>
#pragma comment(lib, "pdh.lib")

#include <Tlhelp32.h>
#include <Shlwapi.h>

CSystemInfoCounter::CSystemInfoCounter()
{
}

CSystemInfoCounter::~CSystemInfoCounter()
{
}

//枚举系统所有计数器对象
bool CSystemInfoCounter::RefreshWindowsSystemCounters()
{
	bool bRet = false;

	PDH_STATUS status = ERROR_SUCCESS;
	LPWSTR pwsObjectsListBuffer = NULL;
	DWORD dwObjectsListSize = 0;

	// Determine the required buffer size for the data. 
	status = ::PdhEnumObjectsW(
				NULL,                   // real-time source
				NULL,                   // local machine
				pwsObjectsListBuffer,   // pass NULL and 0
				&dwObjectsListSize,     // to get required buffer size
				PERF_DETAIL_WIZARD,     // counter detail level
				TRUE);

	if (status == PDH_MORE_DATA)
	{
		// Allocate the buffers and try the call again.
		pwsObjectsListBuffer = (LPWSTR)malloc(dwObjectsListSize * sizeof(WCHAR));

		if (NULL != pwsObjectsListBuffer)
		{
			status = ::PdhEnumObjectsW(
							NULL,                   // real-time source
							NULL,                   // local machine
							pwsObjectsListBuffer,
							&dwObjectsListSize,
							PERF_DETAIL_WIZARD,     // counter detail level
							FALSE);					// According to doc, first call true, sec for false

			if (status == ERROR_SUCCESS)
			{
				// Walk the counters list. The list can contain one
				// or more null-terminated strings. The list is terminated
				// using two null-terminator characters.
				LPWSTR pTemp = NULL;
				for (pTemp = pwsObjectsListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1)
				{
					//m_vecSysObjects.push_back(wstring(pTemp));
				}

				//OK now
				bRet = true;
			}
		}
	}

	//free objects
	if (NULL != pwsObjectsListBuffer)
	{
		free(pwsObjectsListBuffer);
	}

	return bRet;
}

bool CSystemInfoCounter::IsProcessRunning(wstring& wstrProcessName)
{
	bool bFound = false;
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (::Process32FirstW(hSnapshot, &entry) == TRUE)
	{
		while (::Process32NextW(hSnapshot, &entry) == TRUE)
		{
			//脱去扩展名检查
			::PathRemoveExtensionW(entry.szExeFile);

			if (wstrProcessName.compare(entry.szExeFile) == 0)
			{
				bFound = true;
				break;
			}
		}
	}

	CloseHandle(hSnapshot);

	return bFound;
}
