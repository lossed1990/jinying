#include "ServerThreadPool.h"

#include <process.h>

CServerThreadPool::CServerThreadPool()
{
	m_bIsExit = FALSE;

	//��ʼ������
	memset(m_arrThreads, 0, sizeof(CServerThread*)*MAX_SERVER_THREAD_POOL_SIZE);
	m_nTotalThreadCount = 0;
}

CServerThreadPool::~CServerThreadPool()
{
}

//�̳߳��Ƿ�Ҫ����
BOOL CServerThreadPool::IsPoolShouldEnd()
{
	return m_bIsExit;
}

//��־�̳߳ؽ���
void CServerThreadPool::MarkPoolShouldEnd()
{
	m_bIsExit = TRUE;
}

void CServerThreadPool::InitThreadPool(int nInitActiveThread)
{
	//��������
	if (nInitActiveThread > MAX_SERVER_THREAD_POOL_SIZE)
	{
		nInitActiveThread = MAX_SERVER_THREAD_POOL_SIZE;
	}

	if (nInitActiveThread <= 0)
	{
		nInitActiveThread = MAX_SERVER_THREAD_POOL_SIZE / 2;
	}

	//�����߳�
	m_csThreas.Enter();

	//�������ж���
	for (int i = 0; i < nInitActiveThread;)
	{
		CServerThread* pThread = new CServerThread(this);

		//��Thread��Ϣָ����Ϊ�û�����
		UINT uiThreadID = 0;
		HANDLE hHandle = (HANDLE)_beginthreadex(NULL, NULL, CServerThread::ThreadFunction, pThread, CREATE_SUSPENDED, &uiThreadID);

		//�����ɹ�
		if (hHandle > 0 && pThread)
		{
			//�����߳���Ϣ
			pThread->SetThreadInfo(hHandle, uiThreadID);

			m_arrThreads[i++] = pThread;
		}
		else
		{
			delete pThread;
		}
	}

	//����ָ����Ŀ�Ķ���
	for (int i = 0; i < nInitActiveThread; i++)
	{
		CServerThread* pThread = m_arrThreads[i];

		//�����̣߳����ö�Ӧ״̬
		if (pThread)
		{
			::ResumeThread(pThread->GetThreadHandle());
			pThread->MarkThreadStatus(SERVER_THREAD_RUNIDLE);
		}
	}

	//ʵ�ʴ������߳���
	m_nTotalThreadCount = nInitActiveThread;

	m_csThreas.Leave();
}

//��ȡһ���߳�����
CServerTask* CServerThreadPool::getTask()
{
	if (m_lstServerTasks.empty()) return NULL;

	CServerTask* pTask = NULL;

	m_csTasks.Enter();

	//���μ��
	if (!m_lstServerTasks.empty())
	{
		//ȡ��һ������
		pTask = m_lstServerTasks.front();
		m_lstServerTasks.pop_front();
	}

	m_csTasks.Leave();

	return pTask;
}

//���һ������
void CServerThreadPool::AddTaskToPool(CServerTask* pTask)
{
	if (pTask == NULL) return;

	m_csTasks.Enter();

	m_lstServerTasks.push_back(pTask);

	m_csTasks.Leave();
}
