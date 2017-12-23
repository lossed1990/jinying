#include "ServerThread.h"
#include "ServerThreadPool.h"

CServerThread::CServerThread(CServerThreadPool* pPool) : m_pPool(pPool)
{
	//�߳̿�ʼ��Ȼ�ǹ���״̬
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

//�̺߳���
unsigned int __stdcall CServerThread::ThreadFunction(void *pParam)
{
	CServerThread* pThis = (CServerThread*)pParam;

	//�������
	if (pThis == NULL || pThis->m_pPool == NULL) return 0;

	CServerThreadPool* pPool = pThis->m_pPool;

	while (!pPool->IsPoolShouldEnd())
	{
		CServerTask* pTask = pPool->getTask();

		//��ȡ����ʼִ��
		if (pTask)
		{
			//��ʼִ������״̬��Ϊæ
			pThis->m_nStatus = SERVER_THREAD_BUSY;

			pTask->prepareTask();
			pTask->executeTask();
			pTask->cleanupTask();

			//ִ����cleanupȷ��task���԰�ȫɾ��
			delete pTask;
		}

		//���������״̬idle
		pThis->m_nStatus = SERVER_THREAD_RUNIDLE;

		Sleep(1);
	}

	//���е��˴����߳��ѽ���
	pThis->m_nStatus = SERVER_THREAD_ENDED;

	return 0;
}