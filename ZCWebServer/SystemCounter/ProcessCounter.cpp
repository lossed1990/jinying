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
#define COUNTER_PROCETIME2	L"Processor"	//����ʱ��
#define COUNTER_PROCETIME3	L"Time"

#define COUNTER_HANDLECOUNT1 L"Handle"		//�����
#define COUNTER_HANDLECOUNT2 L"Count"

#define COUNTER_THREADCOUNT1 L"Thread"		//�߳���
#define COUNTER_THREADCOUNT2 L"Count"

#define COUNTER_ECLAPSED1	 L"Elapsed"		//������ʱ����
#define COUNTER_ECLAPSED2	 L"Time"

#define COUNTER_MEM_WS1		 L"Working"		//�ڴ湤����
#define COUNTER_MEM_WS2		 L"Set"

#define COUNTER_MEM_WSPK1	 L"Working"		//�ڴ��۷幤����
#define COUNTER_MEM_WSPK2	 L"Set"
#define COUNTER_MEM_WSPK3	 L"Peak"

#define COUNTER_MEM_WSPV1	 L"Working"		//�ڴ�˽�й�����
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

	//�Ƴ�·��
	WCHAR* pcExeFileName = ::PathFindFileNameW(pcPath);

	//�Ƴ���չ��
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

	//��ʼ���������Ͱٷֱ�
	CEncodingTools::ConvertUNICODEToUTF8(m_strInstanceProcessName.c_str(), info.pcProcessName, 64);
	strcpy(info.pcCPUUsage, "0.00");
}

bool CProcessCounter::extractNeededCounterInstance(wstring& strProcessName)
{
	bool bIsInstanceFound = false;

	//���ָ�������Ƿ����ʵ����
	for (vector<wstring>::iterator it = m_vecInstans.begin(); it != m_vecInstans.end(); ++it)
	{
		//�������Ƿ����
		if (strProcessName == *it)
		{
			bIsInstanceFound = true;
			break;
		}
	}

	//����ǰ���ж��ǽ��̴��ڣ���������Ҫȷ�ϣ������Ӧʵ��û�ҵ������޷�����
	if (!bIsInstanceFound)
	{
		return false;
	}

	//���½�����
	m_strInstanceProcessName = strProcessName;

	//����Ӧ�����������Ƿ����
	for (vector<wstring>::iterator it = m_vecCounters.begin(); it != m_vecCounters.end(); ++it)
	{
		//CPUʹ����
		if (it->find(COUNTER_PROCETIME1) != wstring::npos &&
			it->find(COUNTER_PROCETIME2) != wstring::npos &&
			it->find(COUNTER_PROCETIME3) != wstring::npos)
		{
			m_strCounterProcessTime = *it;
		}
		else if (it->find(COUNTER_HANDLECOUNT1) != wstring::npos &&
			it->find(COUNTER_HANDLECOUNT2) != wstring::npos)
		{
			//�����
			m_strCounterHandleCount = *it;
		}
		else if (it->find(COUNTER_THREADCOUNT1) != wstring::npos &&
			it->find(COUNTER_THREADCOUNT2) != wstring::npos)
		{
			//�߳���
			m_strCounterThreadCount = *it; 
		}
		else if (it->find(COUNTER_ECLAPSED1) != wstring::npos &&
			it->find(COUNTER_ECLAPSED2) != wstring::npos)
		{
			//������ʱ��
			m_strCounterElapsedTime = *it;
		}
		else if (it->find(COUNTER_MEM_WSPV1) != wstring::npos &&
			it->find(COUNTER_MEM_WSPV2) != wstring::npos &&
			it->find(COUNTER_MEM_WSPV3) != wstring::npos)
		{
			//˽�й�������winxp��û�����������
			m_strCounterWorkPrivate = *it;
			continue;		//�������ͨ���������
		}
		else if (it->find(COUNTER_MEM_WSPK1) != wstring::npos &&
			it->find(COUNTER_MEM_WSPK2) != wstring::npos &&
			it->find(COUNTER_MEM_WSPK3) != wstring::npos)
		{
			//�۷幤����
			m_strCounterWorkSetPeak = *it;
			continue;		//�������ͨ���������
		}
		else if (it->find(COUNTER_MEM_WS1) != wstring::npos &&
			it->find(COUNTER_MEM_WS2) != wstring::npos)
		{
			//��ͨ������
			m_strCounterWorkSet = *it;
		}
	}

	return true;
}

bool CProcessCounter::InitCounter(wstring& strProcessName)
{
	//�����ָ�����̣���Ĭ��Ϊ��ǰ���̣�����ǵ�ǰ���̣����ȡ������
	if (strProcessName.length() <= 0)
	{
		strProcessName = getCurrentProcessName();
	}

	//ˢ�½�����Ϣ��ʹ��PDH��������ΪЧ������
	if (!CSystemInfoCounter::IsProcessRunning(strProcessName))
	{
		return false;
	}

	//��ȡCPU���ĸ���
	SYSTEM_INFO sysInfo = { 0 };
	::GetSystemInfo(&sysInfo);

	m_dwProcessCoresCount = sysInfo.dwNumberOfProcessors;

	//���ö���ͼ�����
	m_vecCounters.clear();
	m_vecInstans.clear();

	//�������̼���������
	bool bRet = false;

	PDH_STATUS status = ERROR_SUCCESS;
	LPWSTR pwsCounterListBuffer = NULL;
	DWORD dwCounterListSize = 0;
	LPWSTR pwsInstanceListBuffer = NULL;
	DWORD dwInstanceListSize = 0;

	//�����ĵ����˴���Ҫˢ���б������һֱ���ؿ���
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

	//��������Ҫ��ͳ�ƶ���ͼ�����
	if (bRet)
	{
		bRet = extractNeededCounterInstance(strProcessName);
	}

	return bRet;
}

bool CProcessCounter::StartCounter()
{
	bool bRet = false;

	//����Ӧ�������Ͷ����Ƿ��걸��������ļ�����Ϊ������̣߳��ڴ湤����
	if (m_strCounterHandleCount.length() <= 0
		|| m_strCounterThreadCount.length() <= 0
		|| m_strCounterWorkSet.length() <= 0)
	{
		return false;
	}

	//���ø����������Ŀ���״̬
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

	//������ȫ��
	swprintf_s(szCounterProcessTime, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterProcessTime.c_str());
	swprintf_s(szElapsedTimeCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterElapsedTime.c_str());
	swprintf_s(szHandleCountCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterHandleCount.c_str());
	swprintf_s(szThreadCountCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterThreadCount.c_str());
	swprintf_s(szWorksetCounter,	 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkSet.c_str());
	swprintf_s(szWorksetPeakCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkSetPeak.c_str());
	swprintf_s(szWorksetPrivateCounter, 260, L"\\%s(%s)\\%s", COUNTER_OBJECT_NAME, m_strInstanceProcessName.c_str(), m_strCounterWorkPrivate.c_str());

	//���ݼ���������������
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

	//��һ���ռ�
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

bool CProcessCounter::GetProcessStatusInformation(PROCESS_STATUS_INFO& info)
{
	bool bRet = false;

	//��ʼ���������
	resetProcessInfo(info);

	PDH_STATUS pdhStatus = ::PdhCollectQueryDataEx(m_hQuery, m_nSampleSecs, m_hEvent);

	//Ϊ�˳��򾡿��˳�������1���ӳ�ʱ���µȴ�
	DWORD dwWaitResult = WAIT_TIMEOUT;

	while (dwWaitResult == WAIT_TIMEOUT && !m_bIsStopMonitor)
	{
		dwWaitResult = WaitForSingleObject(m_hEvent, 1000);
	}

	//��������˳��źţ��������˳�
	if (m_bIsStopMonitor)
	{
		return false;
	}
		
	//��������¼�
	if (WAIT_OBJECT_0 == dwWaitResult)
	{
		DWORD dwCounterType = 0;
		PDH_FMT_COUNTERVALUE cvValue = { 0 };

		//CPUʹ����
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsProcessTimeValid)
		{
			//�ڶ�˻����ϣ���ֵ���ܳ���100����Ҫָ��mask�ڳ���100ʱ������
			if (PdhGetFormattedCounterValue(m_hProcessorTimeCounter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				//����������ʱ�䣬��ʾ������Ҫ���Ժ�����Ŀ
				double dbShow = cvValue.doubleValue / m_dwProcessCoresCount;
				
				//�Խ������2λС��
				sprintf(info.pcCPUUsage, "%.02f", dbShow);

				//�Դ˲���Ϊָ�꣬��ȡ�ɹ���Ϊ�ɹ�
				bRet = true;
			}
		}

		//��������ʱ��
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsElapsedTimeValid)
		{
			if (PdhGetFormattedCounterValue(m_hElapsedTimeCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unEclapsedSecs = (DWORD)cvValue.doubleValue;
			}
		}

		//�������
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsHandleCountValid)
		{
			if (PdhGetFormattedCounterValue(m_hHandleCountCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unHandleCount = (DWORD)cvValue.doubleValue;
			}
		}

		//�߳�����
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsThreadCountValid)
		{
			if (PdhGetFormattedCounterValue(m_hThreadCountCounter, PDH_FMT_DOUBLE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unThreadCount = (DWORD)cvValue.doubleValue;
			}
		}

		//�ڴ湤����
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//�۷幤����
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetPeakValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetPeakCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetPeakKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//˽�й�����
		memset(&cvValue, 0, sizeof(cvValue));
		if (m_bIsWorksetPrivateValid)
		{
			if (PdhGetFormattedCounterValue(m_hWorksetPrivateCounter, PDH_FMT_LARGE, &dwCounterType, &cvValue) == ERROR_SUCCESS)
			{
				info.unWorksetPrivateKB = (DWORD)(cvValue.largeValue / 1024);
			}
		}

		//��飬���˽�й�����û�õ�������ͨ����������
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

//�̺߳���
unsigned int __stdcall CProcessCounter::ThreadFunction(void *pParam)
{
	CProcessCounter* pThis = (CProcessCounter*)pParam;

	bool bIsFirstData = true;

	//�����̱߳���
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//��ʼ����
	while (!pThis->m_bIsStopMonitor)
	{
		bool bRet = pThis->InitCounter(pThis->m_strInstanceProcessName);

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
		PROCESS_STATUS_INFO info;

		//��ʼʱ�ص�ȫ0���ݣ�Ϊ��ͳ�ƽ��̿�ʼ�ͽ���
		pThis->resetProcessInfo(info);

		if (!bIsFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		while (pThis->GetProcessStatusInformation(info))
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

//�����̼߳��
bool CProcessCounter::StartMonitorProcess(wstring& strProcessName, PROCESS_INFO_CALLBAK pfCB, void* pUser, int nSampleSec)
{
	//������
	if (pfCB == NULL)
	{
		return false;
	}

	//��¼�û�����
	m_pfCB = pfCB;
	m_pUserData = pUser;
	m_strInstanceProcessName = strProcessName;
	m_nSampleSecs = nSampleSec;

	//׼���̱߳������ֶ����ã���ʼ��Ч
	m_hWaitStopEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);

	//�����̣߳����̲�������һֱ����
	HANDLE hHandle = (HANDLE)::_beginthreadex(NULL, NULL, ThreadFunction, this, NULL, NULL);
	::CloseHandle(hHandle);

	return true;
}

//ֹͣ�̼߳��
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
