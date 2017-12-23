#include "MemoryCounter.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <process.h>

//兼容win7及以前系统
#ifndef PSAPI_VERSION
#define PSAPI_VERSION 1
#endif

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

CMemoryCounter::CMemoryCounter()
{
}


CMemoryCounter::~CMemoryCounter()
{
}

bool CMemoryCounter::GetCurrentSystemMemoryUsage(MEMORY_STATUS_INFO& info)
{
	PERFORMANCE_INFORMATION infoPerform = { 0 };
	infoPerform.cb = sizeof(infoPerform);

	BOOL bRet = ::GetPerformanceInfo(&infoPerform, sizeof(infoPerform));
	if (bRet)
	{
		//总物理内存
		size_t szTotalMBPhyMem = (infoPerform.PageSize /1024 * infoPerform.PhysicalTotal) / 1024;
		info.szTotalMB = szTotalMBPhyMem;

		//使用率
		float fUsage = (1.0f - (infoPerform.PhysicalAvailable * 1.0f / infoPerform.PhysicalTotal)) * 100.0f;
		sprintf_s(info.pcMemoUsage, "%.02f", fUsage);
		
		//句柄，线程，进程
		info.nTotalHandle = infoPerform.HandleCount;
		info.nTotalProcess = infoPerform.ProcessCount;
		info.nTotalThread = infoPerform.ThreadCount;
	}

	return true;
}

//获取指定进程内存使用，如果进程id为0则表示本进程
bool CMemoryCounter::GetProcessMemoryUsage(unsigned long dwPorcessID, size_t& szPeakWorksetKB, size_t& szWorksetKB, size_t& szPrivateWorksetKB)
{
	HANDLE hProcess = NULL;
	PROCESS_MEMORY_COUNTERS_EX pmcEx;

	if (dwPorcessID == 0)
	{
		dwPorcessID = ::GetCurrentProcessId();
	}

	hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPorcessID);
	
	if (NULL == hProcess)
	{
		return false;
	}

	pmcEx.cb = sizeof(pmcEx);
	if (::GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmcEx, sizeof(pmcEx)))
	{
		szPeakWorksetKB = pmcEx.PeakWorkingSetSize / 1024;
		szWorksetKB = pmcEx.WorkingSetSize / 1024;
		szPrivateWorksetKB = pmcEx.PrivateUsage / 1024;
	}

	CloseHandle(hProcess);

	return true;
}

void CMemoryCounter::resetProcessInfo(MEMORY_STATUS_INFO& info)
{
	memset(&info, 0, sizeof(info));

	strcpy(info.pcMemoUsage, "0.0");
}

//线程函数
unsigned int __stdcall CMemoryCounter::ThreadFunction(void *pParam)
{
	CMemoryCounter* pThis = (CMemoryCounter*)pParam;

	bool bFirstData = true;

	//重置线程变量
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//开始工作
	while (!pThis->m_bIsStopMonitor)
	{
		//开始连续监控
		MEMORY_STATUS_INFO info;
		pThis->resetProcessInfo(info);

		//程序恢复时回调全零，为了统计进程开始和结束
		if (!bFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		while (pThis->GetCurrentSystemMemoryUsage(info))
		{
			//回调给用户
			pThis->m_pfCB(&info, pThis->m_pUserData);

			//程序已经正常采样过
			bFirstData = false;

			//等待指定时间重新采样
			UINT uiWaitCount = pThis->m_nSampleSecs * 10;
			while (!pThis->m_bIsStopMonitor && uiWaitCount--)
			{
				Sleep(100);
			}

			//检查是否要退出
			if (pThis->m_bIsStopMonitor)
			{
				break;
			}
		}

		//为了统计故障，此时回调出全0数据
		if (!pThis->m_bIsStopMonitor)
		{
			pThis->resetProcessInfo(info);
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}
	}

	::SetEvent(pThis->m_hWaitStopEvent);

	return 0;
}

bool CMemoryCounter::StartMonitorMemory(MEMORY_INFO_CALLBAK pfCB, void* pUser, int nSampleSec)
{
	//检查参数
	if (pfCB == NULL)
	{
		return false;
	}

	//记录用户参数
	m_pfCB = pfCB;
	m_pUserData = pUser;
	m_nSampleSecs = nSampleSec;

	//准备线程变量，手动重置，初始有效
	m_hWaitStopEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);

	//开启线程，进程不存在则一直重试
	HANDLE hHandle = (HANDLE)::_beginthreadex(NULL, NULL, ThreadFunction, this, NULL, NULL);
	::CloseHandle(hHandle);

	return true;
}

bool CMemoryCounter::StopMonitorMemory()
{
	bool bRet = false;

	m_bIsStopMonitor = true;

	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hWaitStopEvent, INFINITE))
	{
		bRet = true;
	}

	::CloseHandle(m_hWaitStopEvent);
	m_hWaitStopEvent = NULL;

	return bRet;
}