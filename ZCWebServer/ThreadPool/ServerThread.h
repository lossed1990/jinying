#pragma once

typedef enum _ENU_SERVER_THREAD_STATUS {
	SERVER_THREAD_SUSPEND = 0,	//线程挂起
	SERVER_THREAD_RUNIDLE,		//线程运行，无任务
	SERVER_THREAD_BUSY,			//线程正在执行任务
	SERVER_THREAD_ENDED,		//线程已停止

} ENU_SERVER_THREAD_STATUS;

//前置声明
class CServerThreadPool;

class CServerThread
{
public:
	CServerThread(CServerThreadPool* pPool);
	~CServerThread();

private:
	int m_nStatus;
	unsigned int m_uiThreadID;
	void* m_hThreadHandle;

	CServerThreadPool* m_pPool;

public:
	static unsigned int __stdcall ThreadFunction(void *pParam);		//线程函数

public:
	void SetThreadInfo(void* hThreadHandle, unsigned int uiThreadID);
	void MarkThreadStatus(int nStatus);

	void* GetThreadHandle();
};

