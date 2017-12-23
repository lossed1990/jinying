#pragma once

#include <vector>
#include <string>
using std::vector;
using std::wstring;

typedef struct _PROCESS_STATUS_INFO {
	char pcProcessName[64];	//��������UTF8
	char pcCPUUsage[8];		//ʹ�ðٷֱȣ������ٷֺ�
	unsigned int unEclapsedSecs;
	unsigned int unHandleCount;
	unsigned int unThreadCount;
	unsigned int unWorksetKB;
	unsigned int unWorksetPeakKB;
	unsigned int unWorksetPrivateKB;
} PROCESS_STATUS_INFO;

//����ص�����
typedef void(__stdcall *PROCESS_INFO_CALLBAK)(PROCESS_STATUS_INFO* pInfo, void* pUser);

//���ڼ�ؽ���״����ÿ��ʵ�����ɼ��һ������
class CProcessCounter
{
public:
	CProcessCounter();
	~CProcessCounter();

public:
	bool InitCounter(wstring& strProcessName);

	bool StartCounter();
	bool GetProcessStatusInformation(PROCESS_STATUS_INFO& info);
	bool StopCounter();

	bool StartMonitorProcess(wstring& strProcessName, PROCESS_INFO_CALLBAK pfCB, void* pUser, int nSampleSec = 1);
	bool StopMonitorProcess();

private:
	wstring getCurrentProcessName();
	void resetProcessInfo(PROCESS_STATUS_INFO& info);
	bool extractNeededCounterInstance(wstring& strProcessName);

	static unsigned int __stdcall ThreadFunction(void *pParam);		//�̺߳���

private:
	vector<wstring> m_vecCounters;		//������
	vector<wstring> m_vecInstans;		//ʾ��

	unsigned int m_dwProcessCoresCount;		//CPU�߼����������������ڼ��㵥������cpuռ��
	wstring m_strInstanceProcessName;		//��Ҫ���ӵĽ�������

	wstring m_strCounterProcessTime;		//����cpuռ�ü�����
	wstring m_strCounterElapsedTime;		//��������ʱ��������
	wstring m_strCounterHandleCount;		//�������������
	wstring m_strCounterThreadCount;		//�߳�����������
	wstring m_strCounterWorkSet;			//������������
	wstring m_strCounterWorkSetPeak;		//�۷幤����������
	wstring m_strCounterWorkPrivate;		//˽�й�����

	//���ø����������Ŀ���״̬
	bool m_bIsProcessTimeValid;
	bool m_bIsElapsedTimeValid;
	bool m_bIsHandleCountValid;
	bool m_bIsThreadCountValid;
	bool m_bIsWorksetValid;
	bool m_bIsWorksetPeakValid;
	bool m_bIsWorksetPrivateValid;

	//����counter���
	void* m_hProcessorTimeCounter;
	void* m_hElapsedTimeCounter;
	void* m_hHandleCountCounter;
	void* m_hThreadCountCounter;
	void* m_hWorksetCounter;
	void* m_hWorksetPeakCounter;
	void* m_hWorksetPrivateCounter;

	//������
	void* m_hQuery;
	void* m_hEvent;

	//�̼߳��ӱ���
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	PROCESS_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//����ʱ����
};

