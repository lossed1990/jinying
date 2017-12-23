#pragma once

#include <vector>
#include <string>
using std::vector;
using std::wstring;

typedef struct _PROCESSOR_STATUS_INFO {
	char pcCPUUsage[8];		//ʹ�ðٷֱȣ������ٷֺ�
	char pcCPUTime[8];
} PROCESSOR_STATUS_INFO;

//����ص�����
typedef void(__stdcall *PROCESSOR_INFO_CALLBAK)(PROCESSOR_STATUS_INFO* pInfo, void* pUser);

class CProcessorCounter
{
public:
	CProcessorCounter();
	~CProcessorCounter();

public:
	bool InitCounter();

	bool StartCounter();
	bool GetTotalCPUUsagePercentage(PROCESSOR_STATUS_INFO& info);
	bool StopCounter();

	bool StartMonitorCPU(PROCESSOR_INFO_CALLBAK pfCB, void* pUser, int nSampleSec = 1);
	bool StopMonitorCPU();

private:
	void ExtractNeededCounterInstance();
	void resetProcessInfo(PROCESSOR_STATUS_INFO& info);

	static unsigned int __stdcall ThreadFunction(void *pParam);		//�̺߳���

private:

	vector<wstring> m_vecCounters;		//������
	vector<wstring> m_vecInstans;		//ʾ��

	//����ʵ��
	wstring m_strInstanceTotal;			//��������ʵ��
	vector<wstring> m_vecInstansCores;	//�߼�������ʵ��


	//���ü�����
	wstring m_strCounterIdleTime;		//����ʱ��
	wstring m_strCounterProcessTime;	//����ʱ��

	//���������
	void* m_hProcessorTimeCounter;
	void* m_hIdleTimeCounter;

	void* m_hQuery;
	void* m_hEvent;

	//�̼߳��ӱ���
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	PROCESSOR_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//����ʱ����
};

