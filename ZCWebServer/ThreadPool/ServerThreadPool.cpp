#include "ServerThreadPool.h"

#include <process.h>

CServerThreadPool::CServerThreadPool()
{
	m_bIsExit = FALSE;

	//初始化变量
	memset(m_arrThreads, 0, sizeof(CServerThread*)*MAX_SERVER_THREAD_POOL_SIZE);
	m_nTotalThreadCount = 0;
}

CServerThreadPool::~CServerThreadPool()
{
}

//线程池是否要结束
BOOL CServerThreadPool::IsPoolShouldEnd()
{
	return m_bIsExit;
}

//标志线程池结束
void CServerThreadPool::MarkPoolShouldEnd()
{
	m_bIsExit = TRUE;
}

void CServerThreadPool::InitThreadPool(int nInitActiveThread)
{
	//修正参数
	if (nInitActiveThread > MAX_SERVER_THREAD_POOL_SIZE)
	{
		nInitActiveThread = MAX_SERVER_THREAD_POOL_SIZE;
	}

	if (nInitActiveThread <= 0)
	{
		nInitActiveThread = MAX_SERVER_THREAD_POOL_SIZE / 2;
	}

	//创建线程
	m_csThreas.Enter();

	//创建所有对象
	for (int i = 0; i < nInitActiveThread;)
	{
		CServerThread* pThread = new CServerThread(this);

		//以Thread信息指针作为用户参数
		UINT uiThreadID = 0;
		HANDLE hHandle = (HANDLE)_beginthreadex(NULL, NULL, CServerThread::ThreadFunction, pThread, CREATE_SUSPENDED, &uiThreadID);

		//创建成功
		if (hHandle > 0 && pThread)
		{
			//设置线程信息
			pThread->SetThreadInfo(hHandle, uiThreadID);

			m_arrThreads[i++] = pThread;
		}
		else
		{
			delete pThread;
		}
	}

	//激活指定数目的对象
	for (int i = 0; i < nInitActiveThread; i++)
	{
		CServerThread* pThread = m_arrThreads[i];

		//启动线程，并置对应状态
		if (pThread)
		{
			::ResumeThread(pThread->GetThreadHandle());
			pThread->MarkThreadStatus(SERVER_THREAD_RUNIDLE);
		}
	}

	//实际创建的线程数
	m_nTotalThreadCount = nInitActiveThread;

	m_csThreas.Leave();
}

//获取一个线程任务
CServerTask* CServerThreadPool::getTask()
{
	if (m_lstServerTasks.empty()) return NULL;

	CServerTask* pTask = NULL;

	m_csTasks.Enter();

	//二次检查
	if (!m_lstServerTasks.empty())
	{
		//取得一个任务
		pTask = m_lstServerTasks.front();
		m_lstServerTasks.pop_front();
	}

	m_csTasks.Leave();

	return pTask;
}

//添加一个任务
void CServerThreadPool::AddTaskToPool(CServerTask* pTask)
{
	if (pTask == NULL) return;

	m_csTasks.Enter();

	m_lstServerTasks.push_back(pTask);

	m_csTasks.Leave();
}
