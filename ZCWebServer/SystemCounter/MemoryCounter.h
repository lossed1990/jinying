#pragma once

#include <string>
using std::wstring;

typedef struct _MEMORY_STATUS_INFO {
	char pcMemoUsage[8];		//使用百分比，不带百分号
	size_t szTotalMB;
	int nTotalHandle;
	int nTotalProcess;
	int nTotalThread;
} MEMORY_STATUS_INFO;

//定义回调函数
typedef void(__stdcall *MEMORY_INFO_CALLBAK)(MEMORY_STATUS_INFO* pInfo, void* pUser);

//获取内存使用状况
//为了兼容WIN7及更早系统，需要定义预处理宏PSAPI_VERSION=1
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
	static unsigned int __stdcall ThreadFunction(void *pParam);		//线程函数
	void resetProcessInfo(MEMORY_STATUS_INFO& info);

private:
	//线程监视变量
	bool m_bIsStopMonitor;
	void* m_hWaitStopEvent;

	MEMORY_INFO_CALLBAK m_pfCB;
	void* m_pUserData;
	int m_nSampleSecs;			//采样时间间隔
};

