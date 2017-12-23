#include "ProcessCounter.h"
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

#include <process.h>
#include <Shlwapi.h>

#include "../JsonRequest/EncodingToolsClass.h"

#define COUNTER_PROCETIME1	L"%"
#define COUNTER_PROCETIME2	L"Processor"	//处理时间
#define COUNTER_PROCETIME3	L"Time"

#define COUNTER_HANDLECOUNT1 L"Handle"		//句柄数
#define COUNTER_HANDLECOUNT2 L"Count"

#define COUNTER_THREADCOUNT1 L"Thread"		//线程数
#define COUNTER_THREADCOUNT2 L"Count"

#define COUNTER_ECLAPSED1	 L"Elapsed"		//已运行时间秒
#define COUNTER_ECLAPSED2	 L"Time"

#define COUNTER_MEM_WS1		 L"Working"		//内存工作集
#define COUNTER_MEM_WS2		 L"Set"

#define COUNTER_MEM_WSPK1	 L"Working"		//内存巅峰工作集
#define COUNTER_MEM_WSPK2	 L"Set"
#define COUNTER_MEM_WSPK3	 L"Peak"

#define COUNTER_MEM_WSPV1	 L"Working"		//内存私有工作集
#define COUNTER_MEM_WSPV2	 L"Set"
#define COUNTER_MEM_WSPV3	 L"Private"

#define	COUNTER_OBJECT_NAME		L"Process"

CProcessCounter::CProcessCounter()
{
	m_dwProcessCoresCount = 0;
}


CProcessCounter::~CProcessCounter()
{
}

wstring CProcessCounter::getCurrentProcessName()
{
	wstring strRet = L"";

	WCHAR pcPath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, pcPath, MAX_PATH);

	//移除路径
	WCHAR* pcExeFileName = ::PathFindFileNameW(pcPath);

	//移除扩展名
	if (pcExeFileName)
	{
		::PathRemoveExtensionW(pcExeFileName);
		strRet = pcExeFileName;
	}

	return strRet;
}

void CProcessCounter::resetProcessInfo(PROCESS_STATUS_INFO& info)
{
	memset(&info, 0, sizeof(info));

	//初始化进程名和百分比
	CEncodingTools::ConvertUNICODEToUTF8(m_strInstanceProcessName.c_str(), info.pcProcessName, 64);
	strcpy(info.pcCPUUsage, "0.00");
}

bool CProcessCounter::extractNeededCounterInstance(wstring& strProcessName)
{
	bool bIsInstanceFound = false;

	//检查指定进程是否存在实例中
	for (vector<wstring>::iterator it = m_vecInstans.begin(); it != m_vecInstans.end(); ++it)
	{
		//检查进程是否存在
		if (strProcessName == *it)
		{
			bIsInstanceFound = true;
			break;
		}
	}

	//尽管前置判断是进程存在，这里仍需要确认，如果对应实例没找到，则无法计数
	if (!bIsInstanceFound)
	{
		return false;
	}

	//更新进程名
	m_strInstanceProcessName = strProcessName;

	//检查对应计数器对象是否存在
	for (vector<wstring>::iterator it = m_vecCounters.begin(); it != m_vecCounters.end(); ++it)
	{
		//CPU使用率
		if (it->find(COUNTER_PROCETIME1) != wstring::npos &&
			it->find(COUNTER_PROCETIME2) != wstring::npos &&
			it->find(COUNTER_PROCETIME3) != wstring::npos)
		{
			m_strCounterProcessTime = *it;
		}
		else if (it->find(COUNTER_HANDLECOUNT1) != wstring::npos &&
			it->find(COUNTER_HANDLECOUNT2) != wstring::npos)
		{
			//句柄数
			m_strCounterHandleCount = *it;
		}
		else if (it->find(COUNTER_THREADCOUNT1) != wstring::npos &&
			it->find(COUNTER_THREADCOUNT2) != wstring::npos)
		{
			//线程数
			m_strCounterThreadCount = *it; 
		}
		else if (it->find(COUNTER_ECLAPSED1) != wstring::npos &&
			it->find(COUNTER_ECLAPSED2) != wstring::npos)
		{
			//已运行时间
			m_strCounterElapsedTime = *it;
		}
		else if (it->find(COUNTER_MEM_WSPV1) != wstring::npos &&
			it->find(COUNTER_MEM_WSPV2) != wstring::npos &&
			it->find(COUNTER_MEM_WSPV3) != wstring::npos)
		{
			//私有工作集，winxp上没有这个计数器
			m_strCounterWorkPrivate = *it;
			continue;		//避免跟普通工作集搞混
		}
		else if (it->find(COUNTER_MEM_WSPK1) != wstring::npos &&
			it->find(COUNTER_MEM_WSPK2) != wstring::npos &&
			it->find(COUNTER_MEM_WSPK3) != wstring::npos)
		{
			//巅峰工作集
			m_strCounterWorkSetPeak = *it;
			continue;		//避免跟普通工作集搞混
		}
		else if (it->find(COUNTER_MEM_WS1) != wstring::npos &&
			it->find(COUNTER_MEM_WS2) != wstring::npos)
		{
			//普通工作集
			m_strCounterWorkSet = *it;
		}
	}

	return true;
}

bool CProcessCounter::InitCounter(wstring& strProcessName)
{
	//如果不指定进程，则默认为当前进程，如果是当前进程，则获取进程名
	if (strProcessName.length() <= 0)
	{
		strProcessName = getCurrentProcessName();
	}

	//刷新进程信息不使用PDH函数，因为效率问题
	if (!CSystemInfoCounter::IsProcessRunning(strProcessName))
	{
		return false;
	}

	//获取CPU核心个数
	SYSTEM_INFO sysInfo = { 0 };
	::GetSystemInfo(&sysInfo);

	m_dwProcessCoresCount = sysInfo.dwNumberOfProcessors;

	//重置对象和计数器
	m_vecCounters.clear();
	m_vecInstans.clear();

	//遍历进程计数器对象
	bool bRet = false;

	PDH_STATUS status = ERROR_SUCCESS;
	LPWSTR pwsCounterListBuffer = NULL;
	DWORD dwCounterListSize = 0;
	LPWSTR pwsInstanceListBuffer = NULL;
	DWORD dwInstanceListSize = 0;

	//根据文档，此处需要刷新列表，否则会一直返回快照
	CSystemInfoCounter::RefreshWindowsSystemCounters();

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
		bRet = extractNeededCounterInstance(strProcessName);
	}

	return bRet;
}

bool CProcessCounter::StartCounter()
{
	bool bRet = false;

	//检查对应计数器和对象是否完备，最基本的计数器为句柄，线程，内存工作集
	if (m_strCounterHandleCount.length() <= 0
		|| m_strCounterThreadCount.length() <= 0
		|| m_strCounterWorkSet.length() <= 0)
	{
		return false;
	}

	//设置各个计数器的可用状态
	m_bIsProcessTimeValid = (m_strCounterProcessTime.length() > 0);
	m_bIsElapsedTimeValid = (m_strCounterElapsedTime.length() > 0);
	m_bIsHandleCountValid = (m_strCounterHandleCount.length() > 0);
	m_bIsThreadCountValid = (m_strCounterThreadCount.length() > 0);
	m_bIsWorksetValid = (m_strCounterWorkSet.length() > 0);
	m_bIsWorksetPeakValid = (m_strCounterWorkSetPeak.length() > 0);
	m_bIsWorksetPrivateValid = (m_strCounterWorkPrivate.length() > 0);

	PDH_STATUS  pdhStatus = ERROR_SUCCESS;

	//open a query for stat
	m_hEvent = NULL;
	m_hQuery = NULL;
	pdhStatus = ::PdhOpenQueryW(NULL, 1, &m_hQuery);

	WCHAR szCounterProcessTime[260];
	WCHAR szElapsedTimeCounter[260];
	WCHAR szHandleCountCounter[260];
	WCHAR szThreadCountCounter[260];
	WCHAR szWorksetCounter[260];
	WCHAR szWorksetPeakCounter[260];
	WCHAR szWorksetPrivateCounter[260];

	//计数器全名
	swprintf_s(szCounterProcessTime, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterProcessTime.c_str());
	swprintf_s(szElapsedTimeCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterElapsedTime.c_str());
	swprintf_s(szHandleCountCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterHandleCount.c_str());
	swprintf_s(szThreadCountCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterThreadCount.c_str());
	swprintf_s(szWorksetCounter,	 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkSet.c_str());
	swprintf_s(szWorksetPeakCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkSetPeak.c_str());
	swprintf_s(szWorksetPrivateCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkPrivate.c_str());

	//根据计数器存在情况添加
	if (m_bIsProcessTimeValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szCounterProcessTime,
			NULL,
			&m_hProcessorTimeCounter);
	}

	if (m_bIsElapsedTimeValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szElapsedTimeCounter,
			NULL,
			&m_hElapsedTimeCounter);
	}

	if (m_bIsHandleCountValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szHandleCountCounter,
			NULL,
			&m_hHandleCountCounter);
	}

	if (m_bIsThreadCountValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szThreadCountCounter,
			NULL,
			&m_hThreadCountCounter);
	}

	if (m_bIsWorksetValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szWorksetCounter,
			NULL,
			&m_hWorksetCounter);
	}

	if (m_bIsWorksetPeakValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szWorksetPeakCounter,
			NULL,
			&m_hWorksetPeakCounter);
	}

	if (m_bIsWorksetPrivateValid)
	{
		pdhStatus |= ::PdhAddCounterW(
			m_hQuery,
			szWorksetPrivateCounter,
			NULL,
			&m_hWorksetPrivateCounter);
	}

	//第一次收集
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

bool CProcessCounter::GetProcessStatusInformation(PROCESS_STATUS_INFO& info)
{
	bool bRet = false;

	//初始化传入参数
	resetProcessInfo(info);

	PDH_STATUS pdhStatus = ::PdhCollectQueryDataEx(m_hQuery, m_nSampleSecs, m_hEvent);

	//为了程序尽快退出，这里1秒钟超时重新等待
	DWORD dwWaitResult = WAIT_TIMEOUT;

	while (dwWaitResult == WAIT_TIMEOUT && !m_bIsStopMonitor)
	{
		dwWaitResult = WaitForSingleObject(m_hEvent, 1000);
	}

	//如果触发退出信号，则立即退出
	if (m_bIsStopMonitor)
	{
		return false;
	}
		
	//如果触发事件
	if (WAIT_OBJECT_0 == dwWaitResult)
	{
		DWORD dwCounterType = 0;
		PDH_FMT_COUNTERVALUE cvValue = { 0 };

		//CPU使用率
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsProcessTimeValid)
		{
			//在多核机器上，此值可能超过100，需要指定mask在超过100时不重置
			if (PdhGetFormattedCounterValue(m_hProcessorTimeCounter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				//单个处理器时间，显示数据需要除以核心数目
				double dbShow = cvValue.doubleValue / m_dwProcessCoresCount;
				
				//对结果保留2位小数
				sprintf(info.pcCPUUsage, "%.02f", dbShow);

				//以此参数为指标，获取成功则为成功
				bRet = true;
			}
		}

		//程序运行时间
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsElapsedTimeValid)
		{
			if (PdhGetFormattedCounterValue(m_hElapsedTimeCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unEclapsedSecs = (DWORD)cvValue.doubleValue;
			}
		}

		//句柄数量
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsHandleCountValid)
		{
			if (PdhGetFormattedCounterValue(m_hHandleCountCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unHandleCount = (DWORD)cvValue.doubleValue;
			}
		}

		//线程数量
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsThreadCountValid)
		{
			if (PdhGetFormattedCounterValue(m_hThreadCountCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unThreadCount = (DWORD)cvValue.doubleValue;
			}
		}

		//内存工作集
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//巅峰工作集
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetPeakValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetPeakCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetPeakKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//私有工作集
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetPrivateValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetPrivateCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetPrivateKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//检查，如果私有工作集没拿到，用普通工作集代替
		if (info.unWorksetPrivateKB == 0)
		{
			info.unWorksetPrivateKB = info.unWorksetKB;
		}
	}

	return bRet;
}

bool CProcessCounter::StopCounter()
{
	if (NULL != m_hEvent)
	{
		::CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

	if (NULL != m_hQuery)
	{
		::PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
	}

	return true;
}

//线程函数
unsigned int __stdcall CProcessCounter::ThreadFunction(void *pParam)
{
	CProcessCounter* pThis = (CProcessCounter*)pParam;

	bool bIsFirstData = true;

	//重置线程变量
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//开始工作
	while (!pThis->m_bIsStopMonitor)
	{
		bool bRet = pThis->InitCounter(pThis->m_strInstanceProcessName);

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
		PROCESS_STATUS_INFO info;

		//开始时回调全0数据，为了统计进程开始和结束
		pThis->resetProcessInfo(info);

		if (!bIsFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		while (pThis->GetProcessStatusInformation(info))
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

//开启线程监控
bool CProcessCounter::StartMonitorProcess(wstring& strProcessName, PROCESS_INFO_CALLBAK pfCB, void* pUser, int nSampleSec)
{
	//检查参数
	if (pfCB == NULL)
	{
		return false;
	}

	//记录用户参数
	m_pfCB = pfCB;
	m_pUserData = pUser;
	m_strInstanceProcessName = strProcessName;
	m_nSampleSecs = nSampleSec;

	//准备线程变量，手动重置，初始有效
	m_hWaitStopEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);

	//开启线程，进程不存在则一直重试
	HANDLE hHandle = (HANDLE)::_beginthreadex(NULL, NULL, ThreadFunction, this, NULL, NULL);
	::CloseHandle(hHandle);

	return true;
}

//停止线程监控
bool CProcessCounter::StopMonitorProcess()
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
