#include "ServerThread.h"
#include "ServerThreadPool.h"

CServerThread::CServerThread(CServerThreadPool* pPool) : m_pPool(pPool)
{
	//线程开始必然是挂起状态
	m_nStatus = SERVER_THREAD_SUSPEND;
	m_uiThreadID = 0;
	m_hThreadHandle = NULL;
}

CServerThread::~CServerThread()
{
}

void CServerThread::SetThreadInfo(void* hThreadHandle, unsigned int uiThreadID)
{
	m_uiThreadID = uiThreadID;
	m_hThreadHandle = hThreadHandle;
}

HANDLE CServerThread::GetThreadHandle()
{
	return m_hThreadHandle;
}

void CServerThread::MarkThreadStatus(int nStatus)
{
	m_nStatus = nStatus;
}

//线程函数
unsigned int __stdcall CServerThread::ThreadFunction(void *pParam)
{
	CServerThread* pThis = (CServerThread*)pParam;

	//参数检查
	if (pThis == NULL || pThis->m_pPool == NULL) return 0;

	CServerThreadPool* pPool = pThis->m_pPool;

	while (!pPool->IsPoolShouldEnd())
	{
		CServerTask* pTask = pPool->getTask();

		//获取任务开始执行
		if (pTask)
		{
			//开始执行任务，状态置为忙
			pThis->m_nStatus = SERVER_THREAD_BUSY;

			pTask->prepareTask();
			pTask->executeTask();
			pTask->cleanupTask();

			//执行完cleanup确保task可以安全删除
			delete pTask;
		}

		//任务结束，状态idle
		pThis->m_nStatus = SERVER_THREAD_RUNIDLE;

		Sleep(1);
	}

	//运行到此处，线程已结束
	pThis->m_nStatus = SERVER_THREAD_ENDED;

	return 0;
}