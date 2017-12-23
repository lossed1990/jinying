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

//����������ؼ���
#define INSTANCE_TOTAL		L"Total"		//��������ʵ��

#define COUNTER_IDLETIME1	L"%"
#define COUNTER_IDLETIME2	L"Idle"			//���м�����
#define COUNTER_IDLETIME3	L"Time"

#define COUNTER_PROCETIME1	L"%"
#define COUNTER_PROCETIME2	L"Processor"	//����ʱ��
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

	//��������Ҫ��ͳ�ƶ���ͼ�����
	if (bRet)
	{
		ExtractNeededCounterInstance();
	}

	return bRet;
}

//��������Ҫͳ�Ƶļ�������ʵ��
void CProcessorCounter::ExtractNeededCounterInstance()
{
	//������
	for (vector<wstring>::iterator it = m_vecCounters.begin(); it != m_vecCounters.end(); ++it)
	{
		//���м�����
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

	//ʵ��
	for (vector<wstring>::iterator it = m_vecInstans.begin(); it != m_vecInstans.end(); ++it)
	{
		//�ܴ�����ʵ��
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

	//����Ӧ�������Ͷ����Ƿ��걸
	if (m_strCounterIdleTime.length() <= 0
		|| m_strCounterProcessTime.length() <= 0
		|| m_strInstanceTotal.length() <= 0)
	{
		return false;
	}

	//�򿪼�����
	PDH_STATUS  pdhStatus = ERROR_SUCCESS;

	//open a query for stat
	pdhStatus = ::PdhOpenQueryW(NULL, 1, &m_hQuery);

	WCHAR szCounterProcessTime[260];
	WCHAR szCounterIdleTime[260];

	//������ȫ��
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
			//�����ȴ��¼�
			m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

			bRet = true;
		}
	}

	return bRet;
}

//�������Processor Time������cpu��ʹ��ʱ�䣬��������������ʹ������1-idle time
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

			//������ʱ�䣬һ��Ȱٷֱ�С
			sprintf_s(info.pcCPUTime, 8, "%.02f", cvProcessTime.doubleValue);

			//�ٷֱ�ʱ�䣬100.0- ����ʱ��
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

//�̺߳���
unsigned int __stdcall CProcessorCounter::ThreadFunction(void *pParam)
{
	CProcessorCounter* pThis = (CProcessorCounter*)pParam;

	bool bIsFirstData = true;

	//�����̱߳���
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//��ʼ����
	while (!pThis->m_bIsStopMonitor)
	{
		bool bRet = pThis->InitCounter();

		//����δ�������ȴ�
		if (!bRet)
		{
			Sleep(1000);
			continue;
		}

		//�����Ѵ���
		bRet = pThis->StartCounter();
		if (!bRet)
		{
			//������;�����жϣ�����ָ��������δ�ҵ�
			pThis->StopCounter();

			Sleep(1000);
			continue;
		}

		//��ʼ�������
		PROCESSOR_STATUS_INFO info;
		pThis->resetProcessInfo(info);

		//��ʼʱ�ص�ȫ0���ݣ�Ϊ��ͳ�ƽ��̿�ʼ�ͽ���
		if (!bIsFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}
		
		while (pThis->GetTotalCPUUsagePercentage(info))
		{
			//����Ƿ�Ҫ�˳�
			if (pThis->m_bIsStopMonitor)
			{
				break;
			}

			//�ص����û�
			pThis->m_pfCB(&info, pThis->m_pUserData);

			bIsFirstData = false;
		}

		//Ϊ��ͳ�ƹ��ϣ���ʱ�ص���ȫ0����
		if (!pThis->m_bIsStopMonitor)
		{
			pThis->resetProcessInfo(info);
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		//��;��س���һ���ǽ����˳����ص���ʼ����
		pThis->StopCounter();
	}

	::SetEvent(pThis->m_hWaitStopEvent);

	return 0;
}

bool CProcessorCounter::StartMonitorCPU(PROCESSOR_INFO_CALLBAK pfCB, void* pUser, int nSampleSec)
{
	//������
	if (pfCB == NULL)
	{
		return false;
	}

	//��¼�û�����
	m_pfCB = pfCB;
	m_pUserData = pUser;
	m_nSampleSecs = nSampleSec;

	//׼���̱߳������ֶ����ã���ʼ��Ч
	m_hWaitStopEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);

	//�����̣߳����̲�������һֱ����
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