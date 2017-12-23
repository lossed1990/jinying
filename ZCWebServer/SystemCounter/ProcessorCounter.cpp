#include "ProcessorCounter.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <pdh.h>
#include <pdhmsg.h>
#pragma comment(lib, "pdh.lib")

#include <process.h>

//定义计数器关键字
#define INSTANCE_TOTAL		L"Total"		//处理器总实例

#define COUNTER_IDLETIME1	L"%"
#define COUNTER_IDLETIME2	L"Idle"			//空闲计数器
#define COUNTER_IDLETIME3	L"Time"

#define COUNTER_PROCETIME1	L"%"
#define COUNTER_PROCETIME2	L"Processor"	//处理时间
#define COUNTER_PROCETIME3	L"Time"

#define	COUNTER_OBJECT_NAME		L"Processor"

CProcessorCounter::CProcessorCounter()
{
}


CProcessorCounter::~CProcessorCounter()
{
}

bool CProcessorCounter::InitCounter()
{
	bool bRet = false;

	PDH_STATUS status = ERROR_SUCCESS;
	LPWSTR pwsCounterListBuffer = NULL;
	DWORD dwCounterListSize = 0;
	LPWSTR pwsInstanceListBuffer = NULL;
	DWORD dwInstanceListSize = 0;

	// Determine the required buffer size for the data. 
	status = ::PdhEnumObjectItemsW(
				NULL,                   // real-time source
				NULL,                   // local machine
				COUNTER_OBJECT_NAME, // object to enumerate
				pwsCounterListBuffer,   // pass NULL and 0
				&dwCounterListSize,     // to get required buffer size
				pwsInstanceListBuffer,
				&dwInstanceListSize,
				PERF_DETAIL_WIZARD,     // counter detail level
				0);

	if (status == PDH_MORE_DATA)
	{
		// Allocate the buffers and try the call again.
		pwsCounterListBuffer = (LPWSTR)malloc(dwCounterListSize * sizeof(WCHAR));
		pwsInstanceListBuffer = (LPWSTR)malloc(dwInstanceListSize * sizeof(WCHAR));

		if (NULL != pwsCounterListBuffer && NULL != pwsInstanceListBuffer)
		{
			status = ::PdhEnumObjectItemsW(
				NULL,                   // real-time source
				NULL,                   // local machine
				COUNTER_OBJECT_NAME, // object to enumerate
				pwsCounterListBuffer,
				&dwCounterListSize,
				pwsInstanceListBuffer,
				&dwInstanceListSize,
				PERF_DETAIL_WIZARD,     // counter detail level
				0);

			if (status == ERROR_SUCCESS)
			{
				// Walk the counters list. The list can contain one
				// or more null-terminated strings. The list is terminated
				// using two null-terminator characters.
				LPWSTR pTemp = NULL;

				//counter
				for (pTemp = pwsCounterListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1)
				{
					m_vecCounters.push_back(std::wstring(pTemp));
				}

				//instance
				for (pTemp = pwsInstanceListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1)
				{
					m_vecInstans.push_back(std::wstring(pTemp));
				}

				bRet = true;
			}
		}
	}

	//free objects
	if (NULL != pwsCounterListBuffer)
	{
		free(pwsCounterListBuffer);
	}

	if (NULL != pwsInstanceListBuffer)
	{
		free(pwsInstanceListBuffer);
	}

	//分析出需要的统计对象和计数器
	if (bRet)
	{
		ExtractNeededCounterInstance();
	}

	return bRet;
}

//分析出需要统计的计数器和实例
void CProcessorCounter::ExtractNeededCounterInstance()
{
	//计数器
	for (vector<wstring>::iterator it = m_vecCounters.begin(); it != m_vecCounters.end(); ++it)
	{
		//空闲计数器
		if (it->find(COUNTER_IDLETIME1) != wstring::npos && 
			it->find(COUNTER_IDLETIME2) != wstring::npos && 
			it->find(COUNTER_IDLETIME3) != wstring::npos)
		{
			m_strCounterIdleTime = *it;
		}
		else if (it->find(COUNTER_PROCETIME1) != wstring::npos && 
			it->find(COUNTER_PROCETIME2) != wstring::npos && 
			it->find(COUNTER_PROCETIME3) != wstring::npos)
		{
			m_strCounterProcessTime = *it;
		}
	}

	//实例
	for (vector<wstring>::iterator it = m_vecInstans.begin(); it != m_vecInstans.end(); ++it)
	{
		//总处理器实例
		if (it->find(INSTANCE_TOTAL) != wstring::npos)
		{
			m_strInstanceTotal = *it;
		}
		else
		{
			m_vecInstansCores.push_back(*it);
		}
	}
}

bool CProcessorCounter::StartCounter()
{
	bool bRet = false;

	//检查对应计数器和对象是否完备
	if (m_strCounterIdleTime.length() <= 0
		|| m_strCounterProcessTime.length() <= 0
		|| m_strInstanceTotal.length() <= 0)
	{
		return false;
	}

	//打开计数器
	PDH_STATUS  pdhStatus = ERROR_SUCCESS;

	//open a query for stat
	pdhStatus = ::PdhOpenQueryW(NULL, 1, &m_hQuery);

	WCHAR szCounterProcessTime[260];
	WCHAR szCounterIdleTime[260];

	//计数器全名
	swprintf_s(szCounterProcessTime, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceTotal.c_str(), m_strCounterProcessTime.c_str());
	swprintf_s(szCounterIdleTime, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceTotal.c_str(), m_strCounterIdleTime.c_str());

	//add a counter into the query
	pdhStatus |= ::PdhAddCounterW(
		m_hQuery,
		szCounterProcessTime,
		NULL,
		&m_hProcessorTimeCounter);

	pdhStatus |= ::PdhAddCounterW(
		m_hQuery,
		szCounterIdleTime,
		NULL,
		&m_hIdleTimeCounter);

	if (pdhStatus == ERROR_SUCCESS)
	{
		//cpu usage normally need called twice to caculate
		pdhStatus = PdhCollectQueryData(m_hQuery);

		if (pdhStatus == ERROR_SUCCESS)
		{
			//创建等待事件
			m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

			bRet = true;
		}
	}

	return bRet;
}

//计数器里，Processor Time反馈了cpu的使用时间，但任务管理器里的使用率是1-idle time
bool CProcessorCounter::GetTotalCPUUsagePercentage(PROCESSOR_STATUS_INFO& info)
{
	bool bRet = false;

	PDH_STATUS pdhStatus = ::PdhCollectQueryDataEx(m_hQuery, 1, m_hEvent);

	DWORD dwWaitResult = WaitForSingleObject(m_hEvent, 1500);
	if (WAIT_OBJECT_0 == dwWaitResult)
	{
		DWORD dwCounterType = 0;
		PDH_FMT_COUNTERVALUE cvProcessTime = { 0 };
		PDH_FMT_COUNTERVALUE cvIdleTime = { 0 };

		pdhStatus |= PdhGetFormattedCounterValue(m_hProcessorTimeCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvProcessTime);
		pdhStatus |= PdhGetFormattedCounterValue(m_hIdleTimeCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvIdleTime);

		if (pdhStatus == ERROR_SUCCESS)
		{
			memset(&info, 0, sizeof(info));

			//处理器时间，一般比百分比小
			sprintf_s(info.pcCPUTime, 8, "%.02f", cvProcessTime.doubleValue);

			//百分比时间，100.0- 空闲时间
			sprintf_s(info.pcCPUUsage, 8, "%.02f", 100.0 - cvIdleTime.doubleValue);

			bRet = true;
		}
	}

	return bRet;
}

bool CProcessorCounter::StopCounter()
{
	if (NULL != m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

	if (NULL != m_hQuery)
	{
		PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
	}

	return true;
}

void CProcessorCounter::resetProcessInfo(PROCESSOR_STATUS_INFO& info)
{
	strcpy(info.pcCPUTime, "0.0");
	strcpy(info.pcCPUUsage, "0.0");
}

//线程函数
unsigned int __stdcall CProcessorCounter::ThreadFunction(void *pParam)
{
	CProcessorCounter* pThis = (CProcessorCounter*)pParam;

	bool bIsFirstData = true;

	//重置线程变量
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//开始工作
	while (!pThis->m_bIsStopMonitor)
	{
		bool bRet = pThis->InitCounter();

		//进程未开启，等待
		if (!bRet)
		{
			Sleep(1000);
			continue;
		}

		//进程已存在
		bRet = pThis->StartCounter();
		if (!bRet)
		{
			//可能中途进程中断，或者指定计数器未找到
			pThis->StopCounter();

			Sleep(1000);
			continue;
		}

		//开始连续监控
		PROCESSOR_STATUS_INFO info;
		pThis->resetProcessInfo(info);

		//开始时回调全0数据，为了统计进程开始和结束
		if (!bIsFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}
		
		while (pThis->GetTotalCPUUsagePercentage(info))
		{
			//检查是否要退出
			if (pThis->m_bIsStopMonitor)
			{
				break;
			}

			//回调给用户
			pThis->m_pfCB(&info, pThis->m_pUserData);

			bIsFirstData = false;
		}

		//为了统计故障，此时回调出全0数据
		if (!pThis->m_bIsStopMonitor)
		{
			pThis->resetProcessInfo(info);
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		//中途监控出错，一般是进程退出，回到开始重来
		pThis->StopCounter();
	}

	::SetEvent(pThis->m_hWaitStopEvent);

	return 0;
}

bool CProcessorCounter::StartMonitorCPU(PROCESSOR_INFO_CALLBAK pfCB, void* pUser, int nSampleSec)
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

bool CProcessorCounter::StopMonitorCPU()
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