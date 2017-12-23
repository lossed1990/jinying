#pragma once

#include "ServerThread.h"
#include "../ThreadSync/ThreadSync.h"
#include "ServerTask.h"

#include <list>
using std::list;

#define MAX_SERVER_THREAD_POOL_SIZE		30		//�̳߳�����߳���

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
	CServerTask* getTask();											//��ȡһ���߳�����

private:
	CServerThread* m_arrThreads[MAX_SERVER_THREAD_POOL_SIZE];		//�̳߳�
	CMyCriticalSection m_csThreas;									//�����̳߳��ٽ���

	list<CServerTask*> m_lstServerTasks;							//�߳������б�
	CMyCriticalSection m_csTasks;									//���������б��ٽ���

	int m_nTotalThreadCount;

	BOOL m_bIsExit;			//�˳���־
};

