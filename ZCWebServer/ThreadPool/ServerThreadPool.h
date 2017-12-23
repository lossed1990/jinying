#pragma once

#include "ServerThread.h"
#include "../ThreadSync/ThreadSync.h"
#include "ServerTask.h"

#include <list>
using std::list;

#define MAX_SERVER_THREAD_POOL_SIZE		30		//线程池最大线程数

class CServerThreadPool
{
public:
	CServerThreadPool();
	~CServerThreadPool();

public:
	void InitThreadPool(int nInitActiveThread = (MAX_SERVER_THREAD_POOL_SIZE/2));
	void AddTaskToPool(CServerTask* pTask);

	BOOL IsPoolShouldEnd();
	void MarkPoolShouldEnd();
	CServerTask* getTask();											//获取一个线程任务

private:
	CServerThread* m_arrThreads[MAX_SERVER_THREAD_POOL_SIZE];		//线程池
	CMyCriticalSection m_csThreas;									//保护线程池临界区

	list<CServerTask*> m_lstServerTasks;							//线程任务列表
	CMyCriticalSection m_csTasks;									//保护任务列表临界区

	int m_nTotalThreadCount;

	BOOL m_bIsExit;			//退出标志
};

