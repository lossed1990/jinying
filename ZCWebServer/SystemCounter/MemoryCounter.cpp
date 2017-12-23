#include "MemoryCounter.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <process.h>

//����win7����ǰϵͳ
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
		//�������ڴ�
		size_t szTotalMBPhyMem = (infoPerform.PageSize /1024 * infoPerform.PhysicalTotal) / 1024;
		info.szTotalMB = szTotalMBPhyMem;

		//ʹ����
		float fUsage = (1.0f - (infoPerform.PhysicalAvailable * 1.0f / infoPerform.PhysicalTotal)) * 100.0f;
		sprintf_s(info.pcMemoUsage, "%.02f", fUsage);
		
		//������̣߳�����
		info.nTotalHandle = infoPerform.HandleCount;
		info.nTotalProcess = infoPerform.ProcessCount;
		info.nTotalThread = infoPerform.ThreadCount;
	}

	return true;
}

//��ȡָ�������ڴ�ʹ�ã��������idΪ0���ʾ������
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

//�̺߳���
unsigned int __stdcall CMemoryCounter::ThreadFunction(void *pParam)
{
	CMemoryCounter* pThis = (CMemoryCounter*)pParam;

	bool bFirstData = true;

	//�����̱߳���
	pThis->m_bIsStopMonitor = false;
	::ResetEvent(pThis->m_hWaitStopEvent);

	//��ʼ����
	while (!pThis->m_bIsStopMonitor)
	{
		//��ʼ�������
		MEMORY_STATUS_INFO info;
		pThis->resetProcessInfo(info);

		//����ָ�ʱ�ص�ȫ�㣬Ϊ��ͳ�ƽ��̿�ʼ�ͽ���
		if (!bFirstData)
		{
			pThis->m_pfCB(&info, pThis->m_pUserData);
		}

		while (pThis->GetCurrentSystemMemoryUsage(info))
		{
			//�ص����û�
			pThis->m_pfCB(&info, pThis->m_pUserData);

			//�����Ѿ�����������
			bFirstData = false;

			//�ȴ�ָ��ʱ�����²���
			UINT uiWaitCount = pThis->m_nSampleSecs * 10;
			while (!pThis->m_bIsStopMonitor && uiWaitCount--)
			{
				Sleep(100);
			}

			//����Ƿ�Ҫ�˳�
			if (pThis->m_bIsStopMonitor)
			{
				break;
			}
		}

		//Ϊ��ͳ�ƹ��ϣ���ʱ�ص���ȫ0����
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