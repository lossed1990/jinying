#pragma once

#include <string>
using std::wstring;

typedef struct _MEMORY_STATUS_INFO {
	char pcMemoUsage[8];		//ʹ�ðٷֱȣ������ٷֺ�
	size_t szTotalMB;
	int nTotalHandle;
	int nTotalProcess;
	int nTotalThread;
} MEMORY_STATUS_INFO;

//����ص�����
typedef void(__stdcall *MEMORY_INFO_CALLBAK)(MEMORY_STATUS_INFO* pInfo, void* pUser);

//��ȡ�ڴ�ʹ��״��
//Ϊ�˼���WIN7������ϵͳ����Ҫ����Ԥ�����PSAPI_VERSION=1
class CMemoryCounter
{
public:
	CMemoryCounter();
	~CMemoryCounter();

public:
	bool GetCurrentSystemMemoryUsage(MEMORY_STATUS_INFO& info);

	bool GetProcessMemoryUsage(unsigned long dwPorcessID, size_t& szPeakWorksetKB, size_t& szWorksetKB, size_t& szPrivateWorksetKB);

public:
	bool StartMonitorMemory(MEMORY_INFO_CALLBAK pfCB, void* pUser, int nSampleSec = 1);
	bool StopMonitorMemory();

private:
	static unsigned int __stdcall ThreadFunction(void *pParam);		//�̺߳���
	void resetProcessInfo(MEMORY_STATUS_INFO& info);

private:
	//�̼߳��ӱ���
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	MEMORY_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//����ʱ����
};

