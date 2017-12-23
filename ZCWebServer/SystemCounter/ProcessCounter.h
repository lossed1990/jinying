#pragma once

#include <vector>
#include <string>
using std::vector;
using std::wstring;

typedef struct _PROCESS_STATUS_INFO {
	char pcProcessName[64];	//进程名，UTF8
	char pcCPUUsage[8];		//使用百分比，不带百分号
	unsigned int unEclapsedSecs;
	unsigned int unHandleCount;
	unsigned int unThreadCount;
	unsigned int unWorksetKB;
	unsigned int unWorksetPeakKB;
	unsigned int unWorksetPrivateKB;
} PROCESS_STATUS_INFO;

//定义回调函数
typedef void(__stdcall *PROCESS_INFO_CALLBAK)(PROCESS_STATUS_INFO* pInfo, void* pUser);

//用于监控进程状况，每个实例仅可监控一个进程
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

	static unsigned int __stdcall ThreadFunction(void *pParam);		//线程函数

private:
	vector<wstring> m_vecCounters;		//计数器
	vector<wstring> m_vecInstans;		//示例

	unsigned int m_dwProcessCoresCount;		//CPU逻辑处理器个数，用于计算单个进程cpu占用
	wstring m_strInstanceProcessName;		//需要监视的进程名称

	wstring m_strCounterProcessTime;		//进程cpu占用计数器
	wstring m_strCounterElapsedTime;		//进程运行时长计数器
	wstring m_strCounterHandleCount;		//句柄数量计数器
	wstring m_strCounterThreadCount;		//线程数量计数器
	wstring m_strCounterWorkSet;			//工作集计数器
	wstring m_strCounterWorkSetPeak;		//巅峰工作集计数器
	wstring m_strCounterWorkPrivate;		//私有工作集

	//设置各个计数器的可用状态
	bool m_bIsProcessTimeValid;
	bool m_bIsElapsedTimeValid;
	bool m_bIsHandleCountValid;
	bool m_bIsThreadCountValid;
	bool m_bIsWorksetValid;
	bool m_bIsWorksetPeakValid;
	bool m_bIsWorksetPrivateValid;

	//各个counter句柄
	void* m_hProcessorTimeCounter;
	void* m_hElapsedTimeCounter;
	void* m_hHandleCountCounter;
	void* m_hThreadCountCounter;
	void* m_hWorksetCounter;
	void* m_hWorksetPeakCounter;
	void* m_hWorksetPrivateCounter;

	//计数器
	void* m_hQuery;
	void* m_hEvent;

	//线程监视变量
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	PROCESS_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//采样时间间隔
};

