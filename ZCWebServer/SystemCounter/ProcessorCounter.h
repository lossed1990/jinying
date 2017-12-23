#pragma once

#include <vector>
#include <string>
using std::vector;
using std::wstring;

typedef struct _PROCESSOR_STATUS_INFO {
	char pcCPUUsage[8];		//使用百分比，不带百分号
	char pcCPUTime[8];
} PROCESSOR_STATUS_INFO;

//定义回调函数
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

	static unsigned int __stdcall ThreadFunction(void *pParam);		//线程函数

private:

	vector<wstring> m_vecCounters;		//计数器
	vector<wstring> m_vecInstans;		//示例

	//可用实例
	wstring m_strInstanceTotal;			//处理器总实例
	vector<wstring> m_vecInstansCores;	//逻辑处理器实例


	//可用计数器
	wstring m_strCounterIdleTime;		//空闲时间
	wstring m_strCounterProcessTime;	//处理时间

	//计数器句柄
	void* m_hProcessorTimeCounter;
	void* m_hIdleTimeCounter;

	void* m_hQuery;
	void* m_hEvent;

	//线程监视变量
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	PROCESSOR_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//采样时间间隔
};

